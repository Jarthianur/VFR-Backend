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

#include <string>
#include <boost/regex.hpp>

#include "../../src/data/AircraftData.h"
#include "../../src/data/AtmosphereData.h"
#include "../../src/data/GpsData.h"
#include "../../src/data/WindData.h"
#include "../../src/feed/parser/AprsParser.h"
#include "../../src/feed/parser/SbsParser.h"
#include "../Helper.hpp"
#include "../framework/src/framework.h"

#ifdef assert
#undef assert
#endif

using namespace data;
using namespace testsuite;
using namespace comparator;

void test_data(TestSuitesRunner& runner)
{
    feed::parser::SbsParser sbsParser;
    feed::parser::AprsParser aprsParser;

    describe<AircraftData>("Container functions", runner)
        ->test(
            "invalidate aircraft",
            [&]() {
                AircraftData data;
                object::Aircraft ac;
                object::Position pos{49.0, 8.0, 0};
                double press     = 1013.25;
                std::size_t slot = data.registerSlot();
                sbsParser.unpack(
                    "MSG,3,0,0,BBBBBB,0,2017/02/16,20:11:30.772,2017/02/16,20:11:30.772,,3281,,,49.000000,8.000000,,,,,,0",
                    ac);
                data.update(std::move(ac), slot);
                for(int i = 0; i < AC_OUTDATED; ++i)
                {
                    data.processAircrafts(pos, press);
                }
                assert(data.getSerialized(), std::string(""), helper::equalsStr);
            })
        ->test(
            "delete aircraft",
            [&]() {
                AircraftData data;
                object::Aircraft ac;
                object::Position pos{49.0, 8.0, 0};
                double press     = 1013.25;
                std::size_t slot = data.registerSlot();
                sbsParser.unpack(
                    "MSG,3,0,0,BBBBBB,0,2017/02/16,20:11:30.772,2017/02/16,20:11:30.772,,3281,,,49.000000,8.000000,,,,,,0",
                    ac);
                data.update(std::move(ac), slot);
                for(int i = 0; i < AC_DELETE_THRESHOLD; ++i)
                {
                    data.processAircrafts(pos, press);
                }
            })
        ->test(
            "prefer FLARM, accept again if no input",
            [&]() {
                AircraftData data;
                boost::smatch match;
                object::Aircraft ac;
                object::Position pos{49.0, 8.0, 0};
                double press     = 1013.25;
                std::size_t slot = data.registerSlot();

                sbsParser.unpack(
                    "MSG,3,0,0,BBBBBB,0,2017/02/16,20:11:30.772,2017/02/16,20:11:30.772,,3281,,,49.000000,8.000000,,,,,,0",
                    ac);
                data.update(std::move(ac), slot);
                data.processAircrafts(pos, press);
                aprsParser.unpack(
                    "FLRBBBBBB>APRS,qAS,XXXX:/201131h4900.00N/00800.00E'180/090/A=002000 id0ABBBBBB +010fpm +0.3rot",
                    ac);
                data.update(std::move(ac), slot);
                data.processAircrafts(pos, press);
                sbsParser.unpack(
                    "MSG,3,0,0,BBBBBB,0,2017/02/16,20:11:32.000,2017/02/16,20:11:32.000,,3281,,,49.000000,8.000000,,,,,,0",
                    ac);
                data.update(std::move(ac), slot);
                data.processAircrafts(pos, press);
                std::string proc = data.getSerialized();
                bool matched     = boost::regex_search(proc, match, helper::pflauRe);
                assert(matched, true, helper::equalsBool);
                assert(match.str(2), std::string("610"), helper::equalsStr);
                for(int i = 0; i < AC_NO_FLARM_THRESHOLD; ++i)
                {
                    data.processAircrafts(pos, press);
                }
                sbsParser.unpack(
                    "MSG,3,0,0,BBBBBB,0,2017/02/16,20:11:33.000,2017/02/16,20:11:33.000,,3281,,,49.000000,8.000000,,,,,,0",
                    ac);
                data.update(std::move(ac), slot);
                data.processAircrafts(pos, press);
                proc    = data.getSerialized();
                matched = boost::regex_search(proc, match, helper::pflauRe);
                assert(matched, true, helper::equalsBool);
                assert(match.str(2), std::string("1000"), helper::equalsStr);
            })
        ->test("write after attempt", [&]() {
            object::Aircraft ac2(2);
            AircraftData data;
            object::Position pos{49.0, 8.0, 0};
            double press = 1013.25;
            boost::smatch match;
            std::size_t slot = data.registerSlot();
            aprsParser.unpack(
                "FLRBBBBBB>APRS,qAS,XXXX:/201131h4900.00N/00800.00E'180/090/A=002000 id0ABBBBBB +010fpm +0.3rot",
                ac2);
            data.update(std::move(ac2), slot);
            object::Aircraft ac1(1);
            aprsParser.unpack(
                "FLRBBBBBB>APRS,qAS,XXXX:/201131h4900.00N/00800.00E'180/090/A=001000 id0ABBBBBB +010fpm +0.3rot",
                ac1);
            data.update(std::move(ac1), slot);
            data.processAircrafts(pos, press);
            std::string proc = data.getSerialized();
            bool matched     = boost::regex_search(proc, match, helper::pflauRe);
            assert(matched, true, helper::equalsBool);
            assert(match.str(2), std::string("610"), helper::equalsStr);
            aprsParser.unpack(
                "FLRBBBBBB>APRS,qAS,XXXX:/201131h4900.00N/00800.00E'180/090/A=001000 id0ABBBBBB +010fpm +0.3rot",
                ac1);
            data.update(std::move(ac1), slot);
            data.processAircrafts(pos, press);
            proc    = data.getSerialized();
            matched = boost::regex_search(proc, match, helper::pflauRe);
            assert(matched, true, helper::equalsBool);
            assert(match.str(2), std::string("305"), helper::equalsStr);
        });

    describeParallel<GpsData>("gps string", runner)
        ->test("correct gps position",
               []() {
                   GpsData data;
                   std::size_t slot = data.registerSlot();
                   object::GpsPosition pos({10.0, 85.0, 100}, 40.0);
                   data.update(std::move(pos), slot);
                   assert(data.getPosition().latitude, 10.0, helper::equalsD);
                   assert(data.getPosition().longitude, 85.0, helper::equalsD);
                   assert(data.getPosition().altitude, 100, helper::equalsInt);
                   std::string fix = data.getSerialized();
                   boost::smatch match;
                   bool matched = boost::regex_search(fix, match, helper::gpsRe);
                   assert(matched, true, helper::equalsBool);
               })
        ->test("write higher priority",
               []() {
                   GpsData data;
                   std::size_t slot = data.registerSlot();
                   object::GpsPosition pos0(0);
                   object::GpsPosition pos1(1);
                   pos0.setPosition({0.0, 0.0, 1000});
                   pos1.setPosition({0.0, 0.0, 2000});
                   data.update(std::move(pos0), slot);
                   assert(data.getPosition().altitude, 1000, helper::equalsInt);
                   data.update(std::move(pos1), slot);
                   assert(data.getPosition().altitude, 2000, helper::equalsInt);
                   data.update(std::move(pos0), slot);
                   assert(data.getPosition().altitude, 2000, helper::equalsInt);
               })
        ->test("write after attempts", []() {
            GpsData data;
            std::size_t slot = data.registerSlot();
            object::GpsPosition pos1(1);
            object::GpsPosition pos2(2);
            pos1.setPosition({0.0, 0.0, 1000});
            pos2.setPosition({0.0, 0.0, 2000});
            data.update(std::move(pos2), slot);
            assert(data.getPosition().altitude, 2000, helper::equalsInt);
            data.update(std::move(pos1), slot);
            assert(data.getPosition().altitude, 2000, helper::equalsInt);
            data.update(std::move(pos1), slot);
            assert(data.getPosition().altitude, 1000, helper::equalsInt);
        });

    describeParallel<WindData>("wind data", runner)
        ->test("extract WIMWV",
               []() {
                   WindData data;
                   object::Wind wind;
                   std::size_t slot = data.registerSlot();
                   wind.setSerialized("$WIMWV,242.8,R,6.9,N,A*20\r\n");
                   data.update(std::move(wind), slot);
                   assert(data.getSerialized(),
                          std::string("$WIMWV,242.8,R,6.9,N,A*20\r\n"),
                          helper::equalsStr);
                   assert(data.getSerialized(), std::string(""), helper::equalsStr);
               })
        ->test(
            "write higher priority",
            []() {
                WindData data;
                object::Wind wind0(0);
                object::Wind wind1(1);
                std::size_t slot = data.registerSlot();
                wind0.setSerialized("$WIMWV,242.8,R,6.9,N,A*20\r\n");
                wind1.setSerialized("updated");
                data.update(std::move(wind0), slot);
                data.update(std::move(wind1), slot);
                assert(data.getSerialized(), std::string("updated"), helper::equalsStr);
                data.update(std::move(wind0), slot);
                assert(data.getSerialized(), std::string("updated"), helper::equalsStr);
            })
        ->test("write after attempt", []() {
            WindData data;
            object::Wind wind1(1);
            object::Wind wind2(2);
            std::size_t slot = data.registerSlot();
            wind1.setSerialized("lower");
            wind2.setSerialized("higher");
            data.update(std::move(wind2), slot);
            assert(data.getSerialized(), std::string("higher"), helper::equalsStr);
            data.update(std::move(wind1), slot);
            assert(data.getSerialized(), std::string(""), helper::equalsStr);
            data.update(std::move(wind1), slot);
            assert(data.getSerialized(), std::string("lower"), helper::equalsStr);
        });

    describeParallel<AtmosphereData>("atmosphere data", runner)
        ->test("get WIMDA, pressure",
               [&]() {
                   AtmosphereData data;
                   object::Atmosphere atm;
                   std::size_t slot = data.registerSlot();
                   atm.setPressure(1009.1);
                   atm.setSerialized(
                       "$WIMDA,29.7987,I,1.0091,B,14.8,C,,,,,,,,,,,,,,*3E\r\n");
                   data.update(std::move(atm), slot);
                   assert(data.getSerialized(),
                          std::string(
                              "$WIMDA,29.7987,I,1.0091,B,14.8,C,,,,,,,,,,,,,,*3E\r\n"),
                          helper::equalsStr);
                   assert(data.getAtmPressure(), 1009.1, helper::equalsD);
               })
        ->test("write higher priority",
               []() {
                   AtmosphereData data;
                   object::Atmosphere atm0(0);
                   object::Atmosphere atm1(1);
                   std::size_t slot = data.registerSlot();
                   atm0.setPressure(1009.1);
                   atm1.setPressure(900.0);
                   data.update(std::move(atm0), slot);
                   assert(data.getAtmPressure(), 1009.1, helper::equalsD);
                   data.update(std::move(atm1), slot);
                   assert(data.getAtmPressure(), 900.0, helper::equalsD);
               })
        ->test("write after attempt", []() {
            AtmosphereData data;
            object::Atmosphere atm1(1);
            object::Atmosphere atm2(2);
            std::size_t slot = data.registerSlot();
            atm1.setPressure(1009.1);
            atm2.setPressure(900.0);
            data.update(std::move(atm2), slot);
            assert(data.getAtmPressure(), 900.0, helper::equalsD);
            data.update(std::move(atm1), slot);
            assert(data.getAtmPressure(), 900.0, helper::equalsD);
            data.update(std::move(atm1), slot);
            assert(data.getAtmPressure(), 1009.1, helper::equalsD);
        });
}
