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

#include "AircraftProcessor.h"

#include <cmath>
#include <cstdio>
#include <limits>

#include "../../Math.hpp"

using namespace object;

namespace data
{
namespace processor
{
AircraftProcessor::AircraftProcessor() : AircraftProcessor(std::numeric_limits<std::int32_t>::max())
{}

AircraftProcessor::AircraftProcessor(std::int32_t vMaxDist)
    : Processor<object::Aircraft>(), mMaxDistance(vMaxDist)
{}

AircraftProcessor::~AircraftProcessor() noexcept
{}

void AircraftProcessor::process(Aircraft& rAircraft)
{
    calcRelativePosition(rAircraft);
    rAircraft.set_serialized(
        (mtDistance <= mMaxDistance ? genPflauStr(rAircraft) + genPflaaStr(rAircraft) : ""));
}

void AircraftProcessor::setRefered(const Position& crRefPosition, double vAtmPress)
{
    mtReferedPosition = crRefPosition;
    mtAtmPressure     = vAtmPress;
}

void AircraftProcessor::calcRelativePosition(const Aircraft& crAircraft)
{
    mtReferedLatRad  = math::radian(mtReferedPosition.latitude);
    mtReferedLonRad  = math::radian(mtReferedPosition.longitude);
    mtAircraftLonRad = math::radian(crAircraft.get_position().longitude);
    mtAircraftLatRad = math::radian(crAircraft.get_position().latitude);
    mtLonDistance    = mtAircraftLonRad - mtReferedLonRad;
    mtLatDistance    = mtAircraftLatRad - mtReferedLatRad;

    double a = std::pow(std::sin(mtLatDistance / 2.0), 2.0)
               + std::cos(mtReferedLatRad) * std::cos(mtAircraftLatRad)
                     * std::pow(std::sin(mtLonDistance / 2.0), 2.0);
    mtDistance
        = math::doubleToInt(6371000.0 * (2.0 * std::atan2(std::sqrt(a), std::sqrt(1.0 - a))));

    mtBearingRel = math::degree(
        std::atan2(std::sin(mtAircraftLonRad - mtReferedLonRad) * std::cos(mtAircraftLatRad),
                   std::cos(mtReferedLatRad) * std::sin(mtAircraftLatRad)
                       - std::sin(mtReferedLatRad) * std::cos(mtAircraftLatRad)
                             * std::cos(mtAircraftLonRad - mtReferedLonRad)));
    mtBearingAbs = std::fmod((mtBearingRel + 360.0), 360.0);

    mtRelNorth    = math::doubleToInt(std::cos(math::radian(mtBearingAbs)) * mtDistance);
    mtRelEast     = math::doubleToInt(std::sin(math::radian(mtBearingAbs)) * mtDistance);
    mtRelVertical = crAircraft.get_targetType() == Aircraft::TargetType::TRANSPONDER
                        ? crAircraft.get_position().altitude - math::icaoHeight(mtAtmPressure)
                        : crAircraft.get_position().altitude - mtReferedPosition.altitude;
}

std::string AircraftProcessor::genPflauStr(const Aircraft& crAircraft)
{
    std::snprintf(mBuffer, sizeof(mBuffer), "$PFLAU,,,,1,0,%d,0,%d,%d,%s*",
                  math::doubleToInt(mtBearingRel), mtRelVertical, mtDistance,
                  crAircraft.get_id().c_str());
    std::string tmp(mBuffer);
    finishSentence(tmp);
    return tmp;
}

std::string AircraftProcessor::genPflaaStr(const Aircraft& crAircraft)
{
    if(crAircraft.get_fullInfo())
    {
        std::snprintf(
            mBuffer, sizeof(mBuffer), "$PFLAA,0,%d,%d,%d,%d,%s,%03d,,%d,%3.1lf,%1x*", mtRelNorth,
            mtRelEast, mtRelVertical, static_cast<std::int32_t>(crAircraft.get_idType()),
            crAircraft.get_id().c_str(), math::doubleToInt(crAircraft.get_movement().heading),
            math::doubleToInt(crAircraft.get_movement().gndSpeed * math::MS_2_KMH),
            crAircraft.get_movement().climbRate,
            static_cast<std::uint32_t>(crAircraft.get_aircraftType()));
    }
    else
    {
        std::snprintf(mBuffer, sizeof(mBuffer), "$PFLAA,0,%d,%d,%d,1,%s,,,,,%1x*", mtRelNorth,
                      mtRelEast, mtRelVertical, crAircraft.get_id().c_str(),
                      static_cast<std::uint32_t>(crAircraft.get_aircraftType()));
    }
    std::string tmp(mBuffer);
    finishSentence(tmp);
    return tmp;
}

}  // namespace processor
}  // namespace data
