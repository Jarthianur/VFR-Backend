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

#pragma once

#include <string>

#include "Client.h"
#include "parameters.h"

#ifdef APRSCCLIENT_BEACON_INTERVAL
#    define AC_BEACON_INT APRSCCLIENT_BEACON_INTERVAL
#else
#    define AC_BEACON_INT 600
#endif

namespace client
{
/**
 * @brief Client for APRSC servers
 */
class AprscClient : public Client
{
public:
    NOT_COPYABLE(AprscClient)
    DEFAULT_DTOR(AprscClient)

    /**
     * @brief Constructor
     * @param endpoint  The remote endpoint
     * @param login     The login string
     * @param connector The Connector interface
     */
    AprscClient(const net::Endpoint& endpoint, const std::string& login,
                std::shared_ptr<net::Connector> connector);

    bool equals(const Client& other) const override;

    std::size_t hash() const override;

private:
    /**
     * @brief Schedule sending of a keep-alive beacon.
     */
    void sendKeepAlive();

    /**
     * @brief Implement Client::handleConnect
     * @threadsafe
     */
    void handleConnect(net::ErrorCode error) override;

    /**
     * @brief Handler for sending of the login string.
     * @param error The error indicator
     * @threadsafe
     */
    void handleLogin(net::ErrorCode error);

    /**
     * @brief Handler for sending a keep-alive beacon.
     * @param error The error indicator
     * @threadsafe
     */
    void handleSendKeepAlive(net::ErrorCode error);

    /// Login string
    const std::string m_login;
};

}  // namespace client
