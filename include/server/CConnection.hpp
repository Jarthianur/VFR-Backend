/*
    Copyright (C) 2016 Jarthianur
    A detailed list of copyright holders can be found in the file "docs/AUTHORS.md".

    This file is part of VirtualFlightRadar-Backend.

    VirtualFlightRadar-Backend is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    VirtualFlightRadar-Backend is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with VirtualFlightRadar-Backend.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include <utility>

#include "net/error/CSocketError.hpp"
#include "util/ClassUtils.hpp"

#include "CLogger.hpp"
#include "Types.hpp"

namespace vfrb::server
{
template<typename SocketT>
class CConnection
{
    SocketT m_socket;
    String  m_address;

public:
    MOVABLE(CConnection)
    NOT_COPYABLE(CConnection)

    explicit CConnection(SocketT&& sock_);

    ~CConnection() noexcept = default;

    auto
    Write(String const& str_) -> bool;

    GETTER_REF(Address, m_address)
};

template<typename SocketT>
CConnection<SocketT>::CConnection(SocketT&& sock_)
    : m_socket(std::move(sock_)), m_address(m_socket.Address()) {}

template<typename SocketT>
CConnection<SocketT>::CConnection(CConnection&& other_) noexcept
    : m_socket(std::move(other_.m_socket)), m_address(std::move(other_.m_address)) {}

template<typename SocketT>
auto
CConnection<SocketT>::operator=(CConnection&& other_) noexcept -> CConnection<SocketT>& {
    m_socket  = std::move(other_.m_socket);
    m_address = std::move(other_.m_address);
    return *this;
}

template<typename SocketT>
auto
CConnection<SocketT>::Write(String const& str_) -> bool {
    try {
        return m_socket.Write(str_);
    } catch (net::error::CSocketError const& e) {
        CLogger::Instance().Debug("(Connection) write: ", e.Message());
    }
    return false;
}
}  // namespace vfrb::server
