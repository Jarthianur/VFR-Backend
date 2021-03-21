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

#include "feed/parser/CAprsParser.hpp"

#include <algorithm>
#include <stdexcept>
#include <utility>

#include "math/Math.hpp"
#include "object/CAircraft.hpp"
#include "object/CGpsPosition.hpp"
#include "object/CTimestamp.hpp"
#include "util/StringUtils.hpp"

using vfrb::object::CAircraft;
using vfrb::object::SLocation;
using vfrb::object::CTimestamp;
using vfrb::str_util::AsStringView;

namespace vfrb::feed::parser
{
CAprsParser::CAprsParser(i32 maxHeight_)
    : IParser<CAircraft>(),
      m_aprsRe(
          "^(?:\\S+?)>APRS,\\S+?(?:,\\S+?)?:/"
          "(\\d{6})h"                               // time
          "(\\d{4}\\.\\d{2})([NS])[\\S\\s]+?"       // lat, lat-dir
          "(\\d{5}\\.\\d{2})([EW])[\\S\\s]+?"       // lon, lon-dir
          "(?:(\\d{3})/(\\d{3}))?/A=(\\d{6})\\s+?"  // head, gnd-spd, alt
          "(?:[\\S\\s]+?)?"
          "id([0-9A-F]{2})([0-9A-F]{6})\\s?"                // type, id
          "(?:([\\+-]\\d{3})fpm\\s+?)?"                     // climb
          "(?:([\\+-]\\d+?\\.\\d+?)rot)?(?:[\\S\\s]+?)?$",  // turn
          std::regex::optimize | std::regex::icase),
      m_maxHeight(maxHeight_) {}

auto
CAprsParser::Parse(String&& str_, u32 prio_) const -> CAircraft {
    std::cmatch match;
    if ((!str_.empty() && str_[0] == '#') || !std::regex_match(str_.c_str(), match, m_aprsRe)) {
        throw error::CParseError();
    }
    try {
        auto [id, idT, aT] = parseComment(match);
        auto mov           = parseMovement(match);
        if (mov) {
            return {prio_,
                    id,
                    idT,
                    aT,
                    CAircraft::ETargetType::FLARM,
                    parseLocation(match, m_maxHeight),
                    *mov,
                    parseTimeStamp(match)};
        }
        return {prio_,
                id,
                idT,
                aT,
                CAircraft::ETargetType::FLARM,
                parseLocation(match, m_maxHeight),
                parseTimeStamp(match)};
    } catch ([[maybe_unused]] str_util::error::CConversionError const&) {
    } catch ([[maybe_unused]] object::error::CTimestampParseError const&) {
    }
    throw error::CParseError();
}

auto
CAprsParser::parseLocation(std::cmatch const& match_, i32 maxHeight_) -> SLocation {
    SLocation pos{};
    pos.Latitude = math::DmToDeg(str_util::Parse<f64>(match_[RE_APRS_LAT]));
    if (AsStringView(match_[RE_APRS_LAT_DIR]) == "S") {
        pos.Latitude = -pos.Latitude;
    }
    pos.Longitude = math::DmToDeg(str_util::Parse<f64>(match_[RE_APRS_LON]));
    if (AsStringView(match_[RE_APRS_LON_DIR]) == "W") {
        pos.Longitude = -pos.Longitude;
    }
    pos.Altitude = math::DoubleToInt(str_util::Parse<f64>(match_[RE_APRS_ALT]) * math::FEET_2_M);
    if (pos.Altitude <= maxHeight_) {
        return pos;
    }
    throw error::CParseError();
}

auto
CAprsParser::parseComment(std::cmatch const& match_) -> CAprsParser::AircraftInfo {
    u32 type = str_util::Parse<x32>(match_[RE_APRS_TYPE]);
    return {AsStringView(match_[RE_APRS_ID]), static_cast<CAircraft::EIdType>(type & ID_TYPE_BITS),
            static_cast<CAircraft::EAircraftType>((type & AIRCRAFT_TYPE_BITS) >> 2U)};
}

auto
CAprsParser::parseMovement(std::cmatch const& match_) -> Optional<CAircraft::SMovement> {
    try {
        return CAircraft::SMovement{
            str_util::Parse<f64>(match_[RE_APRS_HEAD]),
            str_util::Parse<f64>(match_[RE_APRS_GND_SPD]) * math::KTS_2_MS,
            std::max(CLIMB_RATE_MIN,
                     std::min(CLIMB_RATE_MAX, str_util::Parse<f64>(match_[RE_APRS_CR]) * math::FPM_2_MS))};
    } catch ([[maybe_unused]] str_util::error::CConversionError const&) {
        return None;
    }
}

auto
CAprsParser::parseTimeStamp(std::cmatch const& match_) -> CTimestamp {
    return CTimestamp(AsStringView(match_[RE_APRS_TIME]));
}
}  // namespace vfrb::feed::parser
