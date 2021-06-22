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

#include "client/FlarmClient.h"

#include "util/Logger.hpp"

#ifdef COMPONENT
#    undef COMPONENT
#endif
#define COMPONENT "(FlarmClient)"

namespace client
{
using namespace net;

FlarmClient::FlarmClient(const Endpoint& endpoint, std::shared_ptr<Connector> connector)
    : Client(endpoint, COMPONENT, connector)
{}

void FlarmClient::handleConnect(ErrorCode error)
{
    std::lock_guard<std::mutex> lk(m_mutex);
    if (m_state == State::CONNECTING)
    {
        if (error == ErrorCode::SUCCESS)
        {
            m_state = State::RUNNING;
            logger.info(m_component, " connected to ", m_endpoint.host, ":", m_endpoint.port);
            read();
        }
        else
        {
            logger.warn(m_component, " failed to connect to ", m_endpoint.host, ":",
                        m_endpoint.port);
            reconnect();
        }
    }
}
}  // namespace client
