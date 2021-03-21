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

#include "concurrent/Mutex.hpp"
#include "server/net/IAcceptor.hpp"

#include "Types.hpp"

namespace vfrb::server::net
{
class CSocketTest;

class CAcceptorTest : public IAcceptor<CSocketTest>
{
    NOT_MOVABLE(CAcceptorTest)
    NOT_COPYABLE(CAcceptorTest)

    vfrb::concurrent::Mutex                   m_mutex;
    std::function<void(Result<void>&&)>       m_onAcceptFn;
    String                                    m_stagedAddress;
    bool                                      m_running = false;
    bool                                      m_fail    = false;
    Vector<Pair<CSocketTest, Shared<String>>> m_sockets;

public:
    explicit CAcceptorTest();
    ~CAcceptorTest() noexcept override = default;

    void
    Run() override;

    void
    Stop() override REQUIRES(!m_mutex);

    void
    OnAccept(Callback cb_) override;

    void
    Close() override;

    auto
    StartConnection() -> CConnection<CSocketTest> override;

    [[nodiscard]] auto
    StagedAddress() const -> String override;

    auto
    Connect(String const& addr_, bool failAccept_, bool failWrite_) -> usize;

    [[nodiscard]] auto
    Socket(usize i_) const -> CSocketTest const&;

    [[nodiscard]] auto
    Buffer(usize i_) const -> String const&;

    [[nodiscard]] auto
    Sockets() const -> usize;

    void
    FailOnConnect(bool fail_);
};
}  // namespace vfrb::server::net
