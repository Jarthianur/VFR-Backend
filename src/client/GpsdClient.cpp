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

#include "client/GpsdClient.h"

#include <chrono>
#include <condition_variable>
#include <thread>
#include <utility>

#include "util/Logger.hpp"

using namespace vfrb::client::net;
using namespace vfrb::concurrent;

namespace vfrb::client
{
constexpr auto     LOG_PREFIX = "(GpsdClient) ";
static auto const& logger     = CLogger::Instance();

CGpsdClient::CGpsdClient(SEndpoint const& endpoint, SPtr<IConnector> connector) : IClient(endpoint, connector) {}

void CGpsdClient::handleConnect(EErrc error)
{
    LockGuard lk(m_mutex);
    if (m_state == EState::CONNECTING)
    {
        if (error == EErrc::SUCCESS)
        {
            m_connector->onWrite("?WATCH={\"enable\":true,\"nmea\":true}\r\n",
                                 std::bind(&CGpsdClient::handleWatch, this, std::placeholders::_1));
        }
        else
        {
            logger.warn(LOG_PREFIX, "failed to connect to ", m_endpoint.Host, ":", m_endpoint.Port);
            reconnect();
        }
    }
}

void CGpsdClient::stop()
{
    if (m_state == EState::RUNNING)
    {
        Mutex                       sync;
        UniqueLock                  lk(sync);
        std::condition_variable_any cv;
        bool                        sent = false;
        m_connector->onWrite("?WATCH={\"enable\":false}\r\n", [&sent, &cv]([[maybe_unused]] EErrc) {
            logger.info(LOG_PREFIX, "stopped watch");
            sent = true;
            cv.notify_one();
        });
        cv.wait_for(lk, std::chrono::milliseconds(500), [&] { return sent; });
    }
    IClient::stop();
}

void CGpsdClient::handleWatch(EErrc error)
{
    LockGuard lk(m_mutex);
    if (m_state == EState::CONNECTING)
    {
        if (error == EErrc::SUCCESS)
        {
            m_state = EState::RUNNING;
            m_backoff.reset();
            logger.info(LOG_PREFIX, "connected to ", m_endpoint.Host, ":", m_endpoint.Port);
            Read();
        }
        else
        {
            logger.error(LOG_PREFIX, "send watch request failed");
            reconnect();
        }
    }
}

char const* CGpsdClient::logPrefix() const
{
    return LOG_PREFIX;
}
}  // namespace vfrb::client
