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

#include "data/AircraftData.h"

#include <iterator>
#include <stdexcept>

#include "parameters.h"

#ifndef ESTIMATED_TRAFFIC
/// @def ESTIMATED_TRAFFIC
/// Amount of aircrafts estimated, for initial container size
#    define ESTIMATED_TRAFFIC 1
#endif

using namespace object;

namespace data
{
AircraftData::AircraftData() : AircraftData(0) {}

AircraftData::AircraftData(std::int32_t maxDist) : Data(), m_processor(maxDist)
{
    m_container.reserve(ESTIMATED_TRAFFIC);
    m_index.reserve(ESTIMATED_TRAFFIC * 2);
}

void AircraftData::get_serialized(std::string& dest)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    for (const auto& it : m_container)
    {
        dest += it.get_updateAge() < OBJ_OUTDATED ? it.get_serialized() : "";
    }
}

bool AircraftData::update(Object&& aircraft)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    Aircraft&&                  update = static_cast<Aircraft&&>(aircraft);
    const auto                  index  = m_index.find(update.get_id());

    if (index != m_index.end())
    {
        return m_container[index->second].tryUpdate(std::move(aircraft));
    }
    insert(std::move(update));
    return true;
}

void AircraftData::processAircrafts(const Position& position, double atmPress) noexcept
{
    std::lock_guard<std::mutex> lock(m_mutex);
    std::size_t                 index = 0;
    bool                        del   = false;
    auto                        it    = m_container.begin();
    m_processor.referTo(position, atmPress);

    while (it != m_container.end())
    {
        ++(*it);
        try
        {
            // if no FLARM msg received after x, assume target has Transponder
            if (it->get_updateAge() == AC_NO_FLARM_THRESHOLD)
            {
                it->set_targetType(Aircraft::TargetType::TRANSPONDER);
            }
            if (it->get_updateAge() >= AC_DELETE_THRESHOLD)
            {
                del = true;
                m_index.erase(it->get_id());
                it = m_container.erase(it);
            }
            else
            {
                if (it->get_updateAge() == 1)
                {
                    m_processor.process(*it);
                }
                ++it;
                ++index;
            }
            if (del && it != m_container.end())
            {
                m_index.at(it->get_id()) = index;
            }
        }
        catch (const std::exception&)
        {}
    }
}

void AircraftData::insert(object::Aircraft&& aircraft)
{
    m_index.insert({aircraft.get_id(), m_container.size()});
    m_container.push_back(std::move(aircraft));
}
}  // namespace data
