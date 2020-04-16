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

#include <istream>

#include "error/IError.hpp"

#include "CProperties.hpp"
#include "types.hpp"

namespace vfrb::config
{
/// Read a config in INI format.
class CConfigReader
{
    std::istream& m_stream;  ///< The input stream

public:
    explicit CConfigReader(std::istream& stream_);

    /// @throw vfrb::config::error::CReadFileError
    CProperties Read();
};

namespace error
{
/// Error to indicate that file read failed
class CReadFileError : public vfrb::error::IError
{
    String const m_fname;  ///< The name of file tried to read

public:
    explicit CReadFileError(String const& file_);

    str Message() const noexcept override;
};
}  // namespace error
}  // namespace vfrb::config