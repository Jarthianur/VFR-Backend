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

#include <functional>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include "util/class_utils.hpp"

#include "CGuardedThread.hpp"
#include "types.hpp"

namespace vfrb::concurrent
{
using SignalHandler = std::function<void(boost::system::error_code const&, int const)>;

/// Catch and handle system signals.
class CSignalListener
{
    NOT_COPYABLE(CSignalListener)

    boost::asio::io_service m_ioService;  ///< Internal IO-service
    boost::asio::signal_set m_sigSet;     ///< Internal signal set
    CGuardedThread          m_thread;     ///< The underlying thread

public:
    CSignalListener();
    ~CSignalListener() noexcept;

    void Run();
    void Stop();

    /**
     * Add a handler function to be executed when a signal is caught.
     * @param handler_ The handler to invoke
     */
    void AddHandler(SignalHandler&& handler_);
};
}  // namespace vfrb::concurrent