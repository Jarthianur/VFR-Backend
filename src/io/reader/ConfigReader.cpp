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

#include "ConfigReader.h"

#include <boost/regex.hpp>
#include <fstream>
#include <stdexcept>
#include <utility>

#include "../logger/Logger.h"


ConfigReader::ConfigReader(const std::string& file)
        : file(file),
          conf_re("^(\\S+?)(?:\\s+?)?=(?:\\s+?)?(\\S+?[\\s\\S]*?)$",
                  boost::regex_constants::optimize)
{
}

ConfigReader::~ConfigReader()
{
}

void ConfigReader::read()
{
    std::ifstream src(file);
    std::string key;
    std::string value;
    std::string line;
    while (std::getline(src, line))
    {
        try
        {
            if (line.at(0) == '#')
            {
                continue;
            }
            boost::smatch match;
            if (boost::regex_match(line, match, conf_re))
            {
                key = match.str(1);
                value = match.str(2);
                config.insert(
                { key, value });
            }
            else
            {
                Logger::error("(ConfigReader) malformed param: ", line);
            }
        }
        catch (const boost::regex_error& e)
        {
            Logger::error("(ConfigReader) read config: ", e.what());
            break;
        }
        catch (const std::out_of_range& e)
        {
            continue;
        }
    }
}

const std::string& ConfigReader::getProperty(const std::string& key,
        const std::string& defaultValue) const
{
    auto it = config.find(key);
    if (it == config.end())
    {
        return defaultValue;
    }
    else
    {
        return it->second;
    }
}