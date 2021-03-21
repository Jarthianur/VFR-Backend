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

#include "concurrent/CWorkerThread.hpp"
#include "parser/CSbsParser.hpp"

#include "IFeed.hpp"

namespace vfrb::data
{
class CAircraftData;
}  // namespace vfrb::data

namespace vfrb::feed
{
class CSbsFeed : public IFeed
{
    parser::CSbsParser const          m_parser;
    concurrent::CWorkerThread<String> m_worker;

public:
    CSbsFeed(String const& name_, config::CProperties const& prop_, Shared<data::CAircraftData> data_,
             i32 maxHeight_);

    auto
    Protocol() const -> EProtocol override;

    auto
    Process(String&& str_) -> bool override;
};
}  // namespace vfrb::feed
