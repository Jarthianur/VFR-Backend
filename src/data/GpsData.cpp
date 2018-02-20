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

#include "GpsData.h"

#include <stdexcept>
#include <boost/thread/lock_guard.hpp>

namespace data
{
GpsData::GpsData(struct util::ExtGpsPosition vPosition)
{
    std::uint64_t dummy = 0;
    mBasePos.trySetValue(vPosition, 0, dummy);
}

GpsData::~GpsData() noexcept
{}

void GpsData::update(const struct util::ExtGpsPosition& crPosition,
                     std::uint32_t vPriority, std::uint64_t& rAttempts)
{
    if(mPosLocked)
    {
        throw std::runtime_error("Position was locked before.");
    }
    if(mBasePos.trySetValue(crPosition, vPriority, rAttempts))
    {
        rAttempts = 0;
    }
}

std::string GpsData::getGpsStr()
{
    boost::lock_guard<boost::mutex> lock(mMutex);
    std::string gps = mGpsModule.genGprmcStr(mBasePos.getValue());
    gps.append(mGpsModule.genGpggaStr(mBasePos.getValue()));
    return gps;
}

struct util::GpsPosition GpsData::getBasePos()
{
    boost::lock_guard<boost::mutex> lock(mMutex);
    return mBasePos.getValue().position;
}

void GpsData::lockPosition()
{
    mPosLocked = true;
}

}  // namespace data
