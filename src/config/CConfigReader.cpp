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

#include "config/CConfigReader.hpp"

#include <utility>

#include "boost/property_tree/exceptions.hpp"
#include "boost/property_tree/ini_parser.hpp"
#include "boost/property_tree/ptree.hpp"

using boost::property_tree::ptree;
using boost::property_tree::ini_parser_error;

namespace vfrb::config
{
CConfigReader::CConfigReader(std::istream& stream_) : m_stream(stream_) {
    if (!m_stream) {
        throw error::CReadFileError("Unable to read file");
    }
}

auto
CConfigReader::Read() -> CProperties {
    ptree tree;
    try {
        read_ini(m_stream, tree);
    } catch (ini_parser_error const& e) {
        throw error::CReadFileError("Not a valid INI file");
    }
    return CProperties(std::move(tree));
}

namespace error
{
CReadFileError::CReadFileError(String const& msg_) : m_msg(msg_) {}

auto
CReadFileError::Message() const noexcept -> str {
    return m_msg.c_str();
}
}  // namespace error
}  // namespace vfrb::config
