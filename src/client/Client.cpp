/*
 Copyright_License {

 Copyright (C) 2016 VirtualFlightRadar-Backend
 A detailed list of copyright holders can be found in the file "AUTHORS".

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License version 3
 as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 }
 */

#include "client/Client.h"

#include <chrono>
#include <condition_variable>
#include <utility>

#include <boost/functional/hash.hpp>

#include "feed/Feed.h"
#include "util/Logger.hpp"

#include "parameters.h"

static auto const& logger = vfrb::Logger::instance();

using namespace vfrb::client::net;

namespace vfrb::client
{
Client::Client(Endpoint const& endpoint, s_ptr<Connector> connector)
    : m_connector(connector), m_endpoint(endpoint)
{}

bool Client::equals(Client const& other) const
{
    return this->m_endpoint == other.m_endpoint;
}

usize Client::hash() const
{
    usize seed = 0;
    boost::hash_combine(seed, boost::hash_value(m_endpoint.host));
    boost::hash_combine(seed, boost::hash_value(m_endpoint.port));
    return seed;
}

void Client::subscribe(s_ptr<feed::Feed> feed)
{
    std::lock_guard lk(m_mutex);
    m_feeds.push_back(feed);
}

void Client::run()
{
    if (std::unique_lock lk(m_mutex); m_state == State::NONE)
    {
        connect();
        lk.unlock();
        m_connector->run();
        m_state = State::NONE;
    }
}

void Client::connect()
{
    m_state = State::CONNECTING;
    m_connector->onConnect(m_endpoint, std::bind(&Client::handleConnect, this, std::placeholders::_1));
}

void Client::reconnect()
{
    if (m_state != State::STOPPING)
    {
        m_state = State::CONNECTING;
        logger.info(logPrefix(), "schedule reconnect to ", m_endpoint.host, ":", m_endpoint.port);
        m_connector->close();
        timedConnect();
    }
}

void Client::timedConnect()
{
    m_connector->onTimeout(std::bind(&Client::handleTimedConnect, this, std::placeholders::_1),
                           param::CLIENT_CONNECT_WAIT_TIMEVAL);
}

void Client::stop()
{
    if (m_state == State::RUNNING || m_state == State::CONNECTING)
    {
        m_state = State::STOPPING;
        logger.info(logPrefix(), "disconnect from ", m_endpoint.host, ":", m_endpoint.port);
        m_connector->stop();
    }
}

void Client::scheduleStop()
{
    std::condition_variable cond_ready;
    std::unique_lock        lk(m_mutex);
    cond_ready.wait_for(lk, std::chrono::milliseconds(100), [this] { return m_state != State::NONE; });
    stop();
}

void Client::read()
{
    m_connector->onRead(std::bind(&Client::handleRead, this, std::placeholders::_1, std::placeholders::_2));
}

void Client::handleTimedConnect(ErrorCode error)
{
    if (error == ErrorCode::SUCCESS)
    {
        std::lock_guard lk(m_mutex);
        logger.info(logPrefix(), "try connect to ", m_endpoint.host, ":", m_endpoint.port);
        connect();
    }
    else
    {
        logger.error(logPrefix(), "failed to connect after timeout");
        scheduleStop();
    }
}

void Client::handleRead(ErrorCode error, str const& response)
{
    if (std::lock_guard lk(m_mutex); m_state == State::RUNNING)
    {
        if (error == ErrorCode::SUCCESS)
        {
            for (auto& it : m_feeds)
            {
                if (!it->process(response))
                {
                    stop();
                }
            }
            read();
        }
        else
        {
            logger.error(logPrefix(), "failed to read message");
            reconnect();
        }
    }
}
}  // namespace vfrb::client
