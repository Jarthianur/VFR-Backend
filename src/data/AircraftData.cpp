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

#include "AircraftData.h"

#include <iterator>
#include <stdexcept>

#include "../Parameters.h"

#ifndef ESTIMATED_TRAFFIC
/// @def ESTIMATED_TRAFFIC
/// Amount of aircrafts estimated, for initial container size
#define ESTIMATED_TRAFFIC 1
#endif

using namespace object;

namespace data
{
AircraftData::AircraftData() : AircraftData(0)
{}

AircraftData::AircraftData(std::int32_t vMaxDist) : Data(), mProcessor(vMaxDist)
{
    mContainer.reserve(ESTIMATED_TRAFFIC);
    mIndexMap.reserve(ESTIMATED_TRAFFIC * 2);
}

AircraftData::~AircraftData() noexcept
{}

std::string AircraftData::getSerialized()
{
    std::lock_guard<std::mutex> lock(mMutex);
    std::string tmp;
    tmp.reserve(mContainer.size() * 128);
    for(const auto& it : mContainer)
    {
        tmp += it.get_updateAge() < OBJ_OUTDATED ? it.get_serialized() : "";
    }
    return tmp;
}

bool AircraftData::update(Object&& rvAircraft)
{
    std::lock_guard<std::mutex> lock(this->mMutex);
    Aircraft&& rvUpdate = static_cast<Aircraft&&>(rvAircraft);
    const auto index    = mIndexMap.find(rvUpdate.get_id());

    if(index != mIndexMap.end())
    {
        return mContainer[index->second].tryUpdate(std::move(rvAircraft));
    }
    insert(rvUpdate);
    return true;
}

void AircraftData::processAircrafts(const Position& crRefPosition, double vAtmPress) noexcept
{
    std::lock_guard<std::mutex> lock(mMutex);
    std::size_t index = 0;
    bool del          = false;
    auto it           = mContainer.begin();

    while(it != mContainer.end())
    {
        ++(*it);
        try
        {
            // if no FLARM msg received after x, assume target has Transponder
            if(it->get_updateAge() == AC_NO_FLARM_THRESHOLD)
            {
                it->set_targetType(Aircraft::TargetType::TRANSPONDER);
            }
            if(it->get_updateAge() >= AC_DELETE_THRESHOLD)
            {
                del = true;
                mIndexMap.erase(it->get_id());
                it = mContainer.erase(it);
            }
            else
            {
                if(it->get_updateAge() == 1)
                {
                    mProcessor.setRefered(crRefPosition, vAtmPress);
                    mProcessor.process(*it);
                }
                ++it;
                ++index;
            }
            if(del && it != mContainer.end())
            {
                mIndexMap.at(it->get_id()) = index;
            }
        }
        catch(const std::exception&)
        {
        }
    }
}

void AircraftData::insert(const object::Aircraft& crAircraft)
{
    mIndexMap.insert({crAircraft.get_id(), mContainer.size()});
    mContainer.push_back(crAircraft);
}
}  // namespace data
