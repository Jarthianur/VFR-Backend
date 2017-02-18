/*
 Copyright_License {

 Copyright (C) 2017 VirtualFlightRadar-Backend
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

#ifndef PARAMETERS_H_
#define PARAMETERS_H_

/**
 * This file contains some parameters,
 * which should be fixed at compile-time.
 *
 * !!! Note that some of those values may have a high impact
 * on the performance, memory/CPU usage, stability and
 * even may cause the program not to work properly !!!
 *
 * 4 is a magic number here, because XCSoar removes aircrafts
 * after 4 seconds without updates.
 */

/**
 * VFRB_SYNC_TIME:
 * The round-about duration, in seconds, of one cycle.
 * Data will be processed and sent one time each cycle.
 * [1 <= x <= 4]
 */
#define VFRB_SYNC_TIME 1

/**
 * AC_INVALIDATE:
 * Number of cycles without updates until an aircraft
 * is outdated and deleted.
 * [4 <= x]
 * Invalidating an aircraft is much more expensive
 * than processing it, hence values < 4 are discouraged.
 * An aircraft has about 70 Bytes in size.
 */
#define AIRCRAFT_INVALIDATE 4

/**
 * C_CON_WAIT_TIMEVAL:
 * Input-clients wait for this duration, in seconds, until
 * attempting a connect (reconnect).
 * [1 <= x]
 * This value depends on Your infrastructure.
 * E.g. if Your receivers run on different hosts, which get
 * turned on 1 hour after VFR-B, it is pointless to attempt connect
 * every second. As well as waiting 1 hour if all receivers are up
 * after 2 minutes.
 */
#define CLIENT_CONNECT_WAIT_TIMEVAL 120

/**
 * WC_RCV_TIMEOUT:
 * Due to unstable hardware/drivers, it became apparent that
 * it is necessary to timeout the wind-sensors input.
 * [1 <= x]
 * A good way is to set this value to 1.5 * Y, where
 * Y is the time the wind-sensor sends its data.
 */
#define WINDCLIENT_RECEIVE_TIMEOUT 5

/**
 * S_MAX_CLIENTS:
 * Maximal amount of clients, which can connect to the VFR-B's
 * internal NMEA-server.
 * [1 <= x]
 * More clients, more network traffic; but at least 1 client is recommended.
 * Consider someone else wants surrounding traffic displayed from
 * somewhere else, like pilots and flight instructors.
 */
#define SERVER_MAX_CLIENTS 5


#endif /* PARAMETERS_H_ */