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

#include "../object/Aircraft.h"
#include "../object/Position.h"

/// @def AP_BUFF_S
/// Internal buffer size
#define AP_BUFF_S 8191

/// @def AP_L_BUFF_S
/// Internal buffer less size
#define AP_L_BUFF_S 128

namespace feed
{
namespace data
{
/// @namespace aircraft
namespace processing
{
/**
 * @class AircraftProcessor
 * @brief Process Aircrafts relative to the base position.
 */
class AircraftProcessor
{
public:
    AircraftProcessor();
    /**
     * @fn AircraftProcessor
     * @brief Constructor
     * @param vMaxDist The max distance filter
     */
    explicit AircraftProcessor(std::int32_t vMaxDist);

    /**
     * @fn ~AircraftProcessor
     * @brief Destructor
     */
    virtual ~AircraftProcessor() noexcept;

    /**
     * @fn process
     * @brief Generate NMEA report for an Aircraft with relative position etc.
     * @note Resulting string has trailing <cr><lf>.
     * @param crAircraft The Aircraft to process
     * @param crRelPos   The position to relate
     * @param vAtmPress  The atmospheric pressure
     * @param vMaxDist   The max distance
     * @return the NMEA string
     */
    void process(object::Aircraft& crAircraft, const object::GpsPosition& crRelPos,
                 double vAtmPress);

private:
    /**
     * @fn calcRelPosToBase
     * @brief Calcutale an Aircrafts position relative to the base.
     * @param crAircraft The Aircraft to calculate for
     * @param crRelPos   The position to relate
     * @param vAtmPress  The atmospheric pressure
     */
    void calcRelativePosition(const object::Aircraft& crAircraft,
                              const object::GpsPosition& crRelPos, double vAtmPress);

    /**
     * @fn buildPflauStr
     * @brief Build PFLAU sentence for an aircraft.
     * @param crAircraft The Aircaft
     * @param rDestStr   The destination string
     */
    void buildPflauStr(const object::Aircraft& crAircraft, std::string& rDestStr);

    /**
     * @fn buildPflaaStr
     * @brief Build PFLAA sentence for an aircraft.
     * @param crAircraft The Aircaft
     * @param rDestStr   The destination string
     */
    void buildPflaaStr(const object::Aircraft& crAircraft, std::string& rDestStr);

    /**
     * @fn appendChecksum
     * @brief Append the checksum and line ending <cr><lf>
     *        and write the buffered sentence.
     * @note The checksum is computed upon the internal buffer.
     * @param rDestStr The destination string
     */
    void finishSentence(std::string& rDestStr);

    /// @var mMaxDistance
    /// Max distance to process an aircraft
    std::int32_t mMaxDistance;

    /// @var mBuffer
    /// Internal buffer
    char mBuffer[AP_BUFF_S + 1];

    /// @var mtRadLatB
    /// Base latitude as radian
    double mtRadLatB = 0.0;

    ///@var mtRadLatAc
    /// Aircraft latitude as radian
    double mtRadLatAc = 0.0;

    /// @var mtLatDist
    /// Distance/Difference between Aircrafts and bases latitude
    double mtLatDist = 0.0;

    /// @var mtRadLongB
    /// Base longitude as radian
    double mtRadLongB = 0.0;

    ///@var mtRadLongAc
    /// Aircraft longitude as radian
    double mtRadLongAc = 0.0;

    /// @var mtLongDist
    /// Distance/Difference between Aircraft and base longitude
    double mtLongDist = 0.0;

    /// @var mtBearingRel
    /// Relative bearing
    double mtBearingRel = 0.0;

    /// @var mtBearingAbs
    /// Absolute bearing
    double mtBearingAbs = 0.0;

    /// @var mtRelN
    /// Relative distance in northern direction; m
    std::int32_t mtRelN = 0;

    /// @var mtRelE
    /// Relative distance in eastern direction; m
    std::int32_t mtRelE = 0;

    /// @var mtRelV
    /// Relative vertical distance; m
    std::int32_t mtRelV = 0;

    /// @var mtDist
    /// Distance between Aircraft and base; m
    std::int32_t mtDist = 0;
};

}  // namespace aircraft
}
}
