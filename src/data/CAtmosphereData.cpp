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

#include "data/CAtmosphereData.hpp"

#include "concurrent/Mutex.hpp"
#include "object/CAtmosphere.hpp"
#include "object/CObject.hpp"

using vfrb::object::CAtmosphere;
using vfrb::object::CObject;
using vfrb::concurrent::ImmutableLock;
using vfrb::str_util::StringInserter;

namespace vfrb::data
{
CAtmosphereData::CAtmosphereData(CAtmosphere&& atm_) : m_atmosphere(std::move(atm_)) {}

void
CAtmosphereData::CollectInto(StringInserter si_) {
    ImmutableLock lk(m_mutex);
    ++m_atmosphere;
    si_.Copy(m_atmosphere.Nmea());
}

auto
CAtmosphereData::Update(CObject&& atm_) -> bool {
    ImmutableLock lk(m_mutex);
    return m_atmosphere.TryUpdate(std::move(atm_));
}

auto
CAtmosphereData::Pressure() const -> decltype(m_atmosphere.Pressure()) {
    ImmutableLock lk(m_mutex);
    return m_atmosphere.Pressure();
}

auto
CAtmosphereData::Size() const -> usize {
    return 1;
}
}  // namespace vfrb::data
