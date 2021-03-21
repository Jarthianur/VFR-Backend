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

#include "util/ClassUtils.hpp"

#include "Types.hpp"

namespace vfrb::server::net
{
class CSocketTest
{
    Shared<String> m_buffer;
    String         m_address;
    bool           m_failWrite = false;

public:
    MOVABLE(CSocketTest)
    NOT_COPYABLE(CSocketTest)

    CSocketTest(String const& addr_, Shared<String> buf_, bool fail_);
    ~CSocketTest() noexcept = default;

    [[nodiscard]] auto
    Address() const -> String;

    auto
    Write(StringView const& msg_) -> bool;

    void
    Close();
};
}  // namespace vfrb::server::net
