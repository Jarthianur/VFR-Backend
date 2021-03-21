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

#pragma once

#include "config/CProperties.hpp"
#include "error/IError.hpp"
#include "util/ClassUtils.hpp"

#include "Types.hpp"

namespace vfrb::config
{
class CConfiguration;
}  // namespace vfrb::config

namespace vfrb::data
{
class CAircraftData;
class CAtmosphereData;
class CGpsData;
class CWindData;
}  // namespace vfrb::data

namespace vfrb::feed
{
class IFeed;

class CFeedFactory
{
    Shared<config::CConfiguration> m_config;
    Shared<data::CAircraftData>    m_aircraftData;
    Shared<data::CAtmosphereData>  m_atmosData;
    Shared<data::CGpsData>         m_gpsData;
    Shared<data::CWindData>        m_windData;

    template<typename T, ENABLE_IF(EXTENDS(T, IFeed))>
    auto
    makeFeed(String const& name_) -> Shared<T>;

public:
    CFeedFactory(Shared<config::CConfiguration> config_, Shared<data::CAircraftData> aircraftData_,
                 Shared<data::CAtmosphereData> atmosData_, Shared<data::CGpsData> gpsData_,
                 Shared<data::CWindData> windData_);

    auto
    createFeed(String const& name_) -> Shared<IFeed>;
};

namespace error
{
class CFeedCreationError : public vfrb::error::IError
{
    String const m_msg;

public:
    CFeedCreationError();

    [[nodiscard]] auto
    Message() const noexcept -> str override;
};
}  // namespace error
}  // namespace vfrb::feed
