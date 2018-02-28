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

#include "SensorFeed.h"

#include <memory>

#include "../config/Configuration.h"
#include "client/SensorClient.h"
#include "data/object/Sensor.h"

namespace feed
{
SensorFeed::SensorFeed(const std::string& crName, const config::KeyValueMap& crKvMap,
                       std::shared_ptr<data::WindData> pWindData,
                       std::shared_ptr<data::AtmosphereData> pAtmosData)
    : Feed(crName, crKvMap), mpWindData(pWindData), mpAtmosphereData(pAtmosData)

{
    mpClient   = std::unique_ptr<client::Client>(new client::SensorClient(
        mKvMap.find(KV_KEY_HOST)->second, mKvMap.find(KV_KEY_PORT)->second, *this));
    mWindSlot  = mpWindData->registerFeed();
    mAtmosSlot = mpAtmosphereData->registerFeed();
}

SensorFeed::~SensorFeed() noexcept
{}

void SensorFeed::process(const std::string& crResponse) noexcept
{
    data::object::Climate climate{data::object::Wind(getPriority()),
                                  data::object::Atmosphere(getPriority())};
    if(mParser.unpack(crResponse, climate))
    {
        if(climate.hasWind())
        {
            mpWindData->update(climate.mWind, mWindSlot);
        }
        if(climate.hasAtmosphere())
        {
            mpAtmosphereData->update(climate.mAtmosphere, mAtmosSlot);
        }
    }
}

}  // namespace feed
