/*
 * ParserOGN.cpp
 *
 *  Created on: 17.04.2016
 *      Author: lula
 */

#include "ParserOGN.h"
#include <cmath>
#include <cstdio>
#include <ctime>
#include <sstream>

ParserOGN::ParserOGN(long double b_lat, long double b_long, int b_alt)
: Parser(b_lat, b_long, b_alt),
  aprs_re("(.+?)>APRS,.+,(.+?):/(\\d{6})+h(\\d{4}\\.\\d{2})(N|S)(\\\\|/)(\\d{5}\\.\\d{2})(E|W).((\\d{3})/(\\d{3}))?/A=(\\d{6})\\s(.*)"),
  addr_re("id(\\S{2})(\\S{6})"),
  climb_re("([\\+|-]\\d+)fpm")
{
}

ParserOGN::~ParserOGN()
{
}

int ParserOGN::unpack(const std::string& sentence, AircraftContainer& ac_cont)
{
    if (sentence.at(0) == '#') {
        return -1;
    }
    int i;
    Aircraft* ac_ext = nullptr;

    try {
        std::smatch match;
        if (std::regex_search(sentence, match, aprs_re)) {
            //comment
            comment.clear();
            if (match.str(13).size() > 0) comment = match.str(13);
            else return -1;

            // climbrate / address
            splitToTokens(comment);
            for (std::string part : tokens) {
                std::smatch addr_match;
                std::smatch climb_match;
                if (std::regex_match(part, addr_match, addr_re)) {
                    id = addr_match.str(2);
                    addr_t = std::stoi(addr_match.str(1), nullptr, 16) & 0x03;
                    ac_t = (std::stoi(addr_match.str(1), nullptr, 16) & 0x7C) >> 2;
                } else if (std::regex_match(part, climb_match, climb_re)) {
                    climb_r = ldToI(std::stold(climb_match.str(1)) * fpm2ms);
                }
            }

            if ((i = ac_cont.find(id)) == -1) {
                ac_ext = new Aircraft();
                ac_ext->id = id;
                ac_ext->addr_type = addr_t;
                ac_ext->aircraft_type = ac_t;
            } else {
                ac_ext = ac_cont.getAircraft(i);
                ac_ext->valid = 0;
            }

            //latitude
            ac_ext->latitude = dmsToDeg(std::stold(match.str(4)) / 100.0L);
            if (match.str(5).compare("S") == 0) ac_ext->latitude = -ac_ext->latitude;
            //longitue
            ac_ext->longitude = dmsToDeg(std::stold(match.str(7)) / 100.0L);
            if (match.str(8).compare("W") == 0) ac_ext->longitude = -ac_ext->longitude;

            //track/gnd_speed
            if (match.str(9).size() > 0) {
                ac_ext->heading = std::stoi(match.str(10));
                ac_ext->gnd_speed = ldToI(std::stold(match.str(11)) * kts2kmh);
            } else {
                ac_ext->heading = 0;
                ac_ext->gnd_speed = 0;
            }

            //altitude initial 0, or not changed
            if (match.str(12).size() > 0) ac_ext->altitude = ldToI(std::stold(match.str(12)));

            if (i == -1) ac_cont.pushAircraft(ac_ext);
        } else {
            return -1;
        }
    } catch (std::regex_error& e) {
        printf("ERROR %d\n", e.code());
        delete ac_ext;
        return -1;
    }
    return 0;
}

void ParserOGN::process(Aircraft* ac_ext, std::string& nmea_str)
{

    calcPosInfo(ac_ext);
    nmea_str.clear();

    //PFLAU
    snprintf(buffer, BUFF_OUT_S, "$PFLAU,,,,1,0,%d,0,%d,%u,%s*", ldToI(bearing_rel),
            ldToI(rel_V), ldToI(dist), ac_ext->id.c_str());
    int csum = checksum(buffer);
    nmea_str.append(buffer);
    snprintf(buffer, LESS_BUFF_S, "%02x\r\n", csum);
    nmea_str.append(buffer);
    //PFLAA
    snprintf(buffer, BUFF_OUT_S, "$PFLAA,0,%d,%d,%d,%d,%s,%03.0f,,%d,%d,%d*", ldToI(rel_N),
            ldToI(rel_E), ldToI(rel_V), ac_ext->addr_type, ac_ext->id.c_str(), ac_ext->heading, ac_ext->gnd_speed, ac_ext->climb_rate, ac_ext->aircraft_type);
    csum = checksum(buffer);
    nmea_str.append(buffer);
    snprintf(buffer, LESS_BUFF_S, "%02x\r\n", csum);
    nmea_str.append(buffer);
    return;
}

long double ParserOGN::dmsToDeg(long double dms) const
{
    long double absDms = std::abs(dms);
    long double d = std::floor(absDms);
    long double m = (absDms - d) * 100.0L / 60.0L;
    return d + m;
}

void ParserOGN::splitToTokens(const std::string& str) {
    tokens.clear();
    std::stringstream ss(str);
    std::string item;
    while (std::getline(ss, item, ' ')) {
        tokens.push_back(item);
    }
    return;
}
