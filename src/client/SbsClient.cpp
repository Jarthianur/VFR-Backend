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

#include "client/SbsClient.h"

#include "util/Logger.hpp"

using namespace vfrb::client::net;
using namespace vfrb::concurrent;

namespace vfrb::client
{
constexpr auto     LOG_PREFIX = "(SbsClient) ";
static auto const& logger     = CLogger::Instance();

CSbsClient::CSbsClient(SEndpoint const& endpoint, SPtr<IConnector> connector) : IClient(endpoint, connector) {}

void CSbsClient::handleConnect(EErrc error)
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
            logger.warn(LOG_PREFIX, "failed to connect to ", m_endpoint.Host, ":", m_endpoint.Port);
            reconnect();
        }
    }
}

char const* CSbsClient::logPrefix() const
{
    return LOG_PREFIX;
}
}  // namespace vfrb::client
