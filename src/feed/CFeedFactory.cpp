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

#include "feed/CFeedFactory.hpp"

#include "config/CConfiguration.hpp"
#include "data/CAircraftData.hpp"
#include "data/CAtmosphereData.hpp"
#include "data/CGpsData.hpp"
#include "data/CWindData.hpp"
#include "feed/CAprscFeed.hpp"
#include "feed/CAtmosphereFeed.hpp"
#include "feed/CGpsFeed.hpp"
#include "feed/CSbsFeed.hpp"
#include "feed/CWindFeed.hpp"
#include "util/string_utils.hpp"

using namespace vfrb::config;
using namespace vfrb::data;
using namespace vfrb::str_util;

namespace vfrb::feed
{
CFeedFactory::CFeedFactory(SPtr<config::CConfiguration> config_, SPtr<CAircraftData> aircraftData_,
                           SPtr<CAtmosphereData> atmosData_, SPtr<CGpsData> gpsData_,
                           SPtr<CWindData> windData_)
    : m_config(config_),
      m_aircraftData(aircraftData_),
      m_atmosData(atmosData_),
      m_gpsData(gpsData_),
      m_windData(windData_) {}

template<>
SPtr<CAprscFeed> CFeedFactory::makeFeed<CAprscFeed>(String const& name_) {
    return std::make_shared<CAprscFeed>(name_, m_config->FeedProperties.at(name_), m_aircraftData,
                                        m_config->MaxHeight);
}

template<>
SPtr<CGpsFeed> CFeedFactory::makeFeed<CGpsFeed>(String const& name_) {
    return std::make_shared<CGpsFeed>(name_, m_config->FeedProperties.at(name_), m_gpsData);
}

template<>
SPtr<CSbsFeed> CFeedFactory::makeFeed<CSbsFeed>(String const& name_) {
    return std::make_shared<CSbsFeed>(name_, m_config->FeedProperties.at(name_), m_aircraftData,
                                      m_config->MaxHeight);
}

template<>
SPtr<CWindFeed> CFeedFactory::makeFeed<CWindFeed>(String const& name_) {
    return std::make_shared<CWindFeed>(name_, m_config->FeedProperties.at(name_), m_windData);
}

template<>
SPtr<CAtmosphereFeed> CFeedFactory::makeFeed<CAtmosphereFeed>(String const& name_) {
    return std::make_shared<CAtmosphereFeed>(name_, m_config->FeedProperties.at(name_), m_atmosData);
}

SPtr<IFeed> CFeedFactory::createFeed(String const& name_) {
    if (name_.find(CConfiguration::SECT_KEY_APRSC) != String::npos) {
        return makeFeed<CAprscFeed>(name_);
    }
    if (name_.find(CConfiguration::SECT_KEY_SBS) != String::npos) {
        return makeFeed<CSbsFeed>(name_);
    }
    if (name_.find(CConfiguration::SECT_KEY_GPS) != String::npos) {
        return makeFeed<CGpsFeed>(name_);
    }
    if (name_.find(CConfiguration::SECT_KEY_WIND) != String::npos) {
        return makeFeed<CWindFeed>(name_);
    }
    if (name_.find(CConfiguration::SECT_KEY_ATMOS) != String::npos) {
        return makeFeed<CAtmosphereFeed>(name_);
    }
    throw error::CFeedCreationError();
}

namespace error
{
CFeedCreationError::CFeedCreationError()
    : m_msg(MakeStr("no keywords found; be sure feed names contain one of ", CConfiguration::SECT_KEY_APRSC,
                    ", ", CConfiguration::SECT_KEY_SBS, ", ", CConfiguration::SECT_KEY_WIND, ", ",
                    CConfiguration::SECT_KEY_ATMOS, ", ", CConfiguration::SECT_KEY_GPS)) {}

str CFeedCreationError::Message() const noexcept {
    return m_msg.c_str();
}
}  // namespace error
}  // namespace vfrb::feed