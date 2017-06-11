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

#ifndef SRC_FEED_GPSFEED_H_
#define SRC_FEED_GPSFEED_H_

#include "Feed.h"

namespace feed
{

/**
 * The GpsFeed class.
 *
 * This inherits from Feed and
 * resolves to GPS handling Client and Parser.
 */
class GpsFeed: public Feed
{
public:
    /**
     * Construct an GpsFeed.
     * Pass all parameters to parent constructor.
     */
    GpsFeed(const std::string& /*cr_name*/, std::int32_t /*prio*/,
            const std::unordered_map<std::string, std::string>& /*cr_kvmap*/);
    /**
     * Destructor
     *
     * @exceptsafe no-throw
     */
    virtual ~GpsFeed() noexcept;
};

} // namespace feed

#endif /* SRC_FEED_GPSFEED_H_ */
