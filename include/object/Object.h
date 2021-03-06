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

#include <cstdint>
#include <string>

#include "util/defines.h"

/// Times until aircraft is outdated
#define OBJ_OUTDATED 4

namespace object
{
/**
 * @brief Base object class
 */
class Object
{
public:
    DEFAULT_CTOR(Object)
    DEFAULT_VIRTUAL_DTOR(Object)

    /**
     * @brief Constructor
     * @param priority The initial priority
     */
    explicit Object(std::uint32_t priority);

    /**
     * @brief Try to update this Object.
     * @note If the other Object cannot update this, nothing happens.
     * @param other   The other Object
     * @return true on success, else false
     */
    virtual bool tryUpdate(Object&& other);

    /**
     * @brief Set the string representation of this Objects data.
     * @param serialized The string representation
     */
    virtual void set_serialized(std::string&& serialized);

    /**
     * @brief Get the string representation of this Objects data.
     * @return m_serialized
     */
    virtual const std::string& get_serialized() const;

    /**
     * @brief Increment the update age.
     * @return this
     */
    Object& operator++();

protected:
    /**
     * @brief Assign other objects values to this.
     * @param other The other Object
     */
    virtual void assign(Object&& other);

    /**
     * @brief Check whether this Object can update the other one.
     * @param other   The other Object
     * @return true if yes, else false
     */
    virtual bool canUpdate(const Object& other) const;

    /// Got last update with this priority.
    std::uint32_t m_lastPriority = 0;

    /// Times processed without update.
    std::uint32_t m_updateAge = 0;

    /// The string representation of this Objects data.
    std::string m_serialized;

public:
    /**
     * Getters
     */
    GETTER_V(updateAge)
};
}  // namespace object
