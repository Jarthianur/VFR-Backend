/*
 Copyright_License {

 Copyright (C) 2017 VirtualFlightRadar-Backend
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

#include "feed/parser/GpsParser.h"

#include <stdexcept>
#include <type_traits>

#include "object/TimeStamp.hpp"
#include "object/impl/DateTimeImplBoost.h"
#include "util/math.hpp"

/// @def RE_GGA_TIME
/// GGA regex match capture group of time
#define RE_GGA_TIME 1

/// @def RE_GGA_LAT
/// GGA regex match capture group of latitude
#define RE_GGA_LAT 2

/// @def RE_GGA_LAT_DIR
/// GGA regex match capture group of latitude orientation
#define RE_GGA_LAT_DIR 3

/// @def RE_GGA_LON
/// GGA regex match capture group of longitude
#define RE_GGA_LON 4

/// @def RE_GGA_LON_DIR
/// GGA regex match capture group of longitude orientation
#define RE_GGA_LON_DIR 5

/// @def RE_GGA_FIX
/// GGA regex match capture group of fix quality
#define RE_GGA_FIX 6

/// @def RE_GGA_SAT
/// GGA regex match capture group of sitallite count
#define RE_GGA_SAT 7

/// @def RE_GGA_DIL
/// GGA regex match capture group of dilution
#define RE_GGA_DIL 8

/// @def RE_GGA_ALT
/// GGA regex match capture group of altitude
#define RE_GGA_ALT 9

/// @def RE_GGA_GEOID
/// GGA regex match capture group of geoid separation
#define RE_GGA_GEOID 10

using namespace object;

namespace feed
{
namespace parser
{
const boost::regex GpsParser::s_GPGGA_RE(
    "^\\$[A-Z]{2}GGA,(\\d{6}),(\\d{4}\\.\\d{3,4}),([NS]),(\\d{5}\\.\\d{3,4}),([EW]),(\\d),(\\d{2}),(\\d+(?:\\.\\d+)?),(\\d+(?:\\.\\d+)?),M,(\\d+(?:\\.\\d+)?),M,,\\*[0-9A-F]{2}\\s*?$",
    boost::regex::optimize | boost::regex::icase);

GpsParser::GpsParser() : Parser<GpsPosition>() {}

bool GpsParser::unpack(const std::string& sentence, GpsPosition& position) noexcept
{
    try
    {
        boost::smatch match;
        return std::stoi(sentence.substr(sentence.rfind('*') + 1, 2), nullptr, 16) ==
                   math::checksum(sentence.c_str(), sentence.length()) &&
               boost::regex_match(sentence, match, s_GPGGA_RE) && parsePosition(match, position);
    }
    catch (const std::logic_error&)
    {
        return false;
    }
}

bool GpsParser::parsePosition(const boost::smatch& match, GpsPosition& position)
{
    Position pos;
    pos.latitude = math::dmToDeg(std::stod(match.str(RE_GGA_LAT)));

    if (match.str(RE_GGA_LAT_DIR).compare("S") == 0)
    {
        pos.latitude = -pos.latitude;
    }
    pos.longitude = math::dmToDeg(std::stod(match.str(RE_GGA_LON)));

    if (match.str(RE_GGA_LON_DIR).compare("W") == 0)
    {
        pos.longitude = -pos.longitude;
    }
    pos.altitude = math::doubleToInt(std::stod(match.str(RE_GGA_ALT)));
    position.set_position(pos);
    position.set_timeStamp(
        TimeStamp<timestamp::DateTimeImplBoost>(match.str(RE_GGA_TIME), timestamp::Format::HHMMSS));
    position.set_fixQuality(
        static_cast<std::result_of<decltype (&GpsPosition::get_fixQuality)(GpsPosition)>::type>(
            std::stoi(match.str(RE_GGA_FIX))));
    position.set_nrOfSatellites(
        static_cast<std::result_of<decltype (&GpsPosition::get_nrOfSatellites)(GpsPosition)>::type>(
            std::stoi(match.str(RE_GGA_SAT))));
    position.set_dilution(std::stod(match.str(RE_GGA_DIL)));
    position.set_geoid(std::stod(match.str(RE_GGA_GEOID)));
    return true;
}

}  // namespace parser
}  // namespace feed
