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

#include <functional>

#include "util/defines.h"

namespace object
{
class Object;
}  // namespace object

namespace data
{
using accessor_fn = std::function<void(const object::Object&)>;

/**
 * @brief The Data interface
 */
class Data
{
public:
    DEFAULT_CTOR(Data)
    DEFAULT_VIRTUAL_DTOR(Data)

    /**
     * @brief Attempt to update this data.
     * @param _1 The new Object
     * @return true on success, else false
     */
    virtual bool update(object::Object&& _1) = 0;

    virtual void access(const accessor_fn& func) = 0;
};
}  // namespace data
