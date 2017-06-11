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

#include "Client.h"

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time.hpp>
#include <iostream>
#include "../../feed/Feed.h"
#include "../../util/Logger.h"

using namespace util;

namespace tcp
{
namespace client
{

Client::Client(const std::string& cr_host, const std::string& cr_port,
               const std::string& cr_comp, feed::Feed& r_feed)
        : mIOservice(),
          mSocket(mIOservice),
          mResolver(mIOservice),
          mHost(cr_host),
          mPort(cr_port),
          mComponent(cr_comp),
          mrFeed(r_feed),
          mConnectTimer(mIOservice)

{
}

Client::~Client() noexcept
{
}

void Client::run(boost::asio::signal_set& r_sigset)
{
    r_sigset.async_wait([this](const boost::system::error_code&, int)
    {
        stop();
    });
    mIOservice.run();
}

void Client::timedConnect() noexcept
{
    mConnectTimer.expires_from_now(
            boost::posix_time::seconds(C_CON_WAIT_TIMEVAL));
    mConnectTimer.async_wait(
            boost::bind(&Client::handleTimedConnect, this,
                    boost::asio::placeholders::error));
}

void Client::stop() noexcept
{
    Logger::info(mComponent + " stop connection to: ", mHost + ":" + mPort);
    mConnectTimer.expires_at(boost::posix_time::pos_infin);
    mConnectTimer.cancel();
    boost::system::error_code ec;
    mSocket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
    if (mSocket.is_open())
    {
        mSocket.close();
    }
}

void Client::read() noexcept
{
    boost::asio::async_read_until(mSocket, mBuffer, "\r\n",
            boost::bind(&Client::handleRead, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
}

void Client::handleTimedConnect(const boost::system::error_code& cr_ec) noexcept
{
    if (!cr_ec)
    {
        Logger::info(mComponent + " try connect to: ", mHost + ":" + mPort);
        connect();
    } else
    {
        Logger::error(mComponent + " cancel connect: ", cr_ec.message());
        if (cr_ec != boost::asio::error::operation_aborted)
        {
            stop();
        }
    }
}

void Client::handleRead(const boost::system::error_code& cr_ec, std::size_t s)
noexcept
{
    if (!cr_ec)
    {
        std::istream is(&mBuffer);
        std::getline(is, mResponse);
        process();
        read();
    } else if (cr_ec != boost::system::errc::bad_file_descriptor)
    {
        Logger::error(mComponent + " read: ", cr_ec.message());
        if (cr_ec != boost::asio::error::operation_aborted)
        {
            stop();
        }
    }
}

}  // namespace client
}  // namespace tcp
