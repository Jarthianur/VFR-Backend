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

#include <cstdio>
#include <string>

#include "../../Math.hpp"

#define PROC_BUFF_S 4096

/// @namespace data
namespace data
{
/// @namespace processor
namespace processor
{
/**
 * @class Processor
 * @brief Processor base class/interface.
 * @tparam T The type of object to process
 */
template<typename T>
class Processor
{
public:
    /**
     * @fn Processor
     * @brief Constructor
     */
    Processor()
    {}

    /**
     * @fn ~Processor
     * @brief Destructor
     */
    virtual ~Processor() noexcept
    {}

    /**
     * @fn process
     * @brief Process a given object.
     * @param _1 The object of type T
     * @return the resulting string
     */
    virtual std::string process(const T& _1) = 0;

protected:
    /**
     * @fn finishSentence
     * @brief End a given string with checksum and CRLF.
     * @param rDestStr The target string
     */
    void finishSentence(std::string& rDestStr)
    {
        std::snprintf(mBuffer, sizeof(mBuffer), "%02x\r\n",
                      math::checksum(mBuffer, sizeof(mBuffer)));
        rDestStr.append(mBuffer);
    }

    /// @var mBuffer
    /// The internal buffer for format strings
    char mBuffer[PROC_BUFF_S] = "";
};
}  // namespace processor
}  // namespace data
