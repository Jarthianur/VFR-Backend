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

#ifndef SRC_DATA_ATMOSPHEREDATA_H_
#define SRC_DATA_ATMOSPHEREDATA_H_

#include <cstdint>
#include <string>

#include "../util/Sensor.h"
#include "Data.hpp"
#include "../util/Wrapper.hpp"

namespace data
{

/**
 * @class AtmosphereData implements Data
 * @brief Manage sensor information.
 * @see Data.hpp
 */
class AtmosphereData: public Data<struct util::Atmosphere>
{
public:
	/**
	 * @fn AtmosphereData
	 * @brief Constructor
	 */
	AtmosphereData();

	/**
	 * @fn ~AtmosphereData
	 * @brief Destructor
	 */
	virtual ~AtmosphereData() noexcept;

	/**
	 * @fn init
	 * @brief Initialize the sensor information.
	 * @param info The initial data
	 * @override Data::init
	 */
	void init(struct util::Atmosphere vAtmos) override;

	/**
	 * @fn update
	 * @brief Try to update the sensor information.
	 * @note Splits the given info, using setters.
	 * @param cr_info The new sensor information.
	 * @param prio    The attempts priority
	 * @override Data::update
	 * @threadsafe
	 */
	void update(const struct util::Atmosphere& crAtmos, std::uint32_t vPriority,
	        std::uint64_t& rAttempts) override;

	/**
	 * @fn getMdaStr
	 * @brief Get the MDA sentence.
	 * @note MDA is invalid after this operation.
	 * @return the MDA sentence, if valid, else empty string
	 * @threadsafe
	 */
	std::string getMdaStr();

	/**
	 * @fn getPress
	 * @brief Get pressure.
	 * @return the pressure
	 * @threadsafe
	 */
	double getAtmPress();

private:

	/// Holding atmospheric pressure
	struct util::Wrapper<util::Atmosphere> mAtmosphere;
};

} // namespace data

#endif /* SRC_DATA_ATMOSPHEREDATA_H_ */