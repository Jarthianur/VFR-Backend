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

#include "Aircraft.h"

Aircraft::~Aircraft() noexcept
{
}

Aircraft::Aircraft(const std::string& id, double lat, double lon, std::int32_t alt)
        : mID(id),
          mLatitude(lat),
          mLongitude(lon),
          mAltitude(alt)
{
}

Aircraft::Aircraft(const std::string& id, double lat, double lon, std::int32_t alt,
                   double gnd_spd, std::uint32_t id_t, std::int32_t ac_t, double climb_r,
                   double turn_r, double heading)
        : mID(id),
          mIDtype(id_t),
          mAircraftType(ac_t),
          mLatitude(lat),
          mLongitude(lon),
          mAltitude(alt),
          mGndSpeed(gnd_spd),
          mHeading(heading),
          mClimbRate(climb_r),
          mTurnRate(turn_r)
{
}

bool Aircraft::operator==(const Aircraft& other) const
{
    return this->mID == other.mID;
}

Aircraft::Aircraft(BOOST_RV_REF(Aircraft) other)
: mID(std::move(other.mID)),
mIDtype(other.mIDtype),
mAircraftType(other.mAircraftType),
mTargetType(other.mTargetType),
mFullInfo(other.mFullInfo),
mUpdateAge(other.mUpdateAge),
mLastPriority(other.mLastPriority),
mAttemptValid(other.mAttemptValid),
mLatitude(other.mLatitude),
mLongitude(other.mLongitude),
mAltitude(other.mAltitude),
mGndSpeed(other.mGndSpeed),
mHeading(other.mHeading),
mClimbRate(other.mClimbRate),
mTurnRate(other.mTurnRate)
{
}

Aircraft& Aircraft::operator =(BOOST_RV_REF(Aircraft))
{
    return *this;
}

void Aircraft::update(const Aircraft& ac, Priority prio)
{
    // no update for ID
    this->mIDtype = ac.mIDtype;
    this->mAircraftType = ac.mAircraftType;
    this->mLatitude = ac.mLatitude;
    this->mLongitude = ac.mLongitude;
    this->mAltitude = ac.mAltitude;
    this->mGndSpeed = ac.mGndSpeed;
    this->mHeading = ac.mHeading;
    this->mClimbRate = ac.mClimbRate;
    this->mTurnRate = ac.mTurnRate;
    this->mFullInfo = ac.mFullInfo;
    this->mTargetType = ac.mTargetType;
    this->mUpdateAge = 0;
    this->mLastPriority = prio;
    this->mAttemptValid = (prio == Priority::LESSER);
}
