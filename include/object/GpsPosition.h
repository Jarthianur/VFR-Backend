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

#include "impl/DateTimeImplBoost.h"
#include "util/utility.hpp"

#include "Object.h"
#include "TimeStamp.hpp"

namespace data
{
class GpsData;
namespace processor
{
class GpsProcessor;
}  // namespace processor
}  // namespace data
namespace feed
{
namespace parser
{
class GpsParser;
}  // namespace parser
}  // namespace feed

namespace object
{
/**
 * @brief A position on earth.
 */
struct Location
{
    //< begin members >//
    double       latitude;   ///< Latitude; deg
    double       longitude;  ///< Longitude; deg
    std::int32_t altitude;   ///< Altitude; m
    //< end members >//
};

/**
 * @brief Extend Object to a GPS position.
 */
class GpsPosition : public Object
{
    friend class data::GpsData;
    friend class data::processor::GpsProcessor;
    friend class feed::parser::GpsParser;

    //< begin constants >//
    static constexpr const std::size_t NMEA_SIZE = 4096;
    //< end constants >//

    //< begin members >//
    Location                           m_location{0.0, 0.0, 0};  ///< The location
    double                             m_geoid = 0.0;            ///< The geoid separation
    TimeStamp<time::DateTimeImplBoost> m_timeStamp;              ///< The timestamp of this position
    double                             m_dilution       = 0.0;   ///< The position dilution
    std::uint8_t                       m_nrOfSatellites = 1;     ///< The number of satellites
    std::int8_t                        m_fixQuality     = 5;     ///< The GPS fix quality
    util::CString<NMEA_SIZE>           m_nmea;
    //< end members >//

    //< begin methods >//
    /**
     * @brief Override Object::assign.
     */
    void assign(Object&& other) override;

    /**
     * @brief Override Object::canUpdate.
     */
    bool canUpdate(const Object& other) const override;
    //< end methods >//

public:
    GpsPosition();
    explicit GpsPosition(std::uint32_t priority);  ///< @param priority The initial priority

    /**
     * @param position The position
     * @param geoid    The geoid
     */
    GpsPosition(const Location& position, double geoid);
    ~GpsPosition() noexcept override = default;

    //< begin interfaces >//
    util::CStringPack getNMEA() const override;
    //< end interfaces >//
};
}  // namespace object
