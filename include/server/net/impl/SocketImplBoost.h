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

#include <string_view>

#include <boost/asio.hpp>
#include <boost/move/move.hpp>

#include "util/class_utils.h"

#include "types.h"

namespace vfrb::server::net
{
/**
 * @brief Socket implementation using boost
 */
class CSocketBoost
{
    boost::asio::ip::tcp::socket m_socket;  ///< Underlying socket

public:
    MOVABLE_BUT_NOT_COPYABLE(CSocketBoost)

    explicit CSocketBoost(BOOST_RV_REF(boost::asio::ip::tcp::socket)
                              socket_);  ///< @param socket The underlying socket
    ~CSocketBoost() noexcept;

    /**
     * @brief Get the remote IP address.
     * @return the address
     * @throw SocketException if the socket is closed
     */
    Str Address() const;

    /**
     * @brief Write a message on the socket to the endpoint.
     * @param msg The message
     * @return true on success, else false
     * @throw SocketException if the socket is closed
     */
    bool Write(std::string_view const& msg_);

    /**
     * @brief Close the socket.
     */
    void Close();

    /**
     * @brief Get the underlying socket.
     * @return the socket
     */
    boost::asio::ip::tcp::socket& Get();
};
}  // namespace vfrb::server::net
