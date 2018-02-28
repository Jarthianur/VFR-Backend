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

#include "AtmosphereData.h"

#include <boost/thread/lock_guard.hpp>

namespace feed
{
using namespace data::object;

namespace data
{
AtmosphereData::AtmosphereData() : Data()
{}

AtmosphereData::AtmosphereData(const Atmosphere& crAtmos) : Data(), mAtmosphere(crAtmos)
{}

AtmosphereData::~AtmosphereData() noexcept
{}

std::string AtmosphereData::getSerialized()
{
    boost::lock_guard<boost::mutex> lock(mMutex);
    return mAtmosphere.getMdaStr();
}

bool AtmosphereData::update(const Object& crAtmos, std::size_t vSlot)
{
    boost::lock_guard<boost::mutex> lock(mMutex);
    try
    {
        bool updated = mAtmosphere.tryUpdate(crAtmos, ++mFeedAttempts.at(vSlot));
        if(updated)
        {
            clearAttempts(mFeedAttempts);
        }
        return updated;
    }
    catch(const std::out_of_range&)
    {
        return false;
    }
}

double AtmosphereData::getAtmPress()
{
    boost::lock_guard<boost::mutex> lock(mMutex);
    return mAtmosphere.getPressure();
}
}
}  // namespace data
