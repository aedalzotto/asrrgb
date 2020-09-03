/** 
 * asrrgb - Control ASRock's B550 and TRX40 motherboard series RGB LEDs
 * Copyright (C) 2020  aedalzotto
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include <boost/program_options/parsers.hpp>
#include "Cmdline.hpp"

namespace po = boost::program_options;

// https://stackoverflow.com/a/16388610
constexpr unsigned int str2int(const char* str, int h = 0)
{
    return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
}

Cmdline::Cmdline(int argc, char *argv[]) :
	desc("Allowed options")
{
	desc.add_options()
		("help", "produce help message")
		("region", po::value<std::string>(&region)->default_value("all"), "region to change the lights: hdr0, hdr1, ahdr0, ahdr1, pch, io, audio, pcb or all")
		("mode", po::value<std::string>(&mode)->default_value("static"), "mode of illumination: off, static, breath, strobe, cycle, rand, wave, spring, stack, cram, scan, neon, water or rainbow")
		("color", po::value<std::string>(&color)->default_value("ffffff"), "hex code of the color, ignored case mode is off, cycle, rand, wave, water or rainbow")
		("speed", po::value<uint8_t>(&speed)->default_value(50), "0 to 100 speed of the mode, ignored for off and static")
	;

	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);
}

bool Cmdline::is_help()
{
	return vm.count("help");
}

const po::options_description& Cmdline::help()
{
	return desc;
}

uint8_t Cmdline::get_region()
{
	switch(str2int(region.c_str())){
		case str2int("hdr0"):
			all = 0;
			return 0;
		case str2int("hdr1"):
			all = 0;
			return 1;
		case str2int("ahdr0"):
			all = 0;
			return 2;
		case str2int("ahdr1"):
			all = 0;
			return 3;
		case str2int("pch"):
			all = 0;
			return 4;
		case str2int("io"):
			all = 0;
			return 5;

		case str2int("audio"):	/* These 2 last options might be swapped */
			all = 0;
			return 6;
		case str2int("pcb"):
			all = 0;
			return 7;
		case str2int("all"):
			all = 1;
			return 0;
		default:
			throw std::invalid_argument("invalid region selected");
	}
}

uint8_t Cmdline::get_mode()
{
	switch(str2int(mode.c_str())){
		case str2int("off"):
			no_color = true;
			no_speed = true;
			return 0;
		case str2int("static"):
			no_color = false;
			no_speed = true;
			return 1;
		case str2int("breath"):
			no_color = false;
			no_speed = false;
			return 2;
		case str2int("strobe"):
			no_color = false;
			no_speed = false;
			return 3;
		case str2int("cycle"):
			no_color = true;
			no_speed = false;
			return 4;
		case str2int("rand"):
			no_color = true;
			no_speed = false;
			return 5;
		case str2int("music"):
			no_color = true;
			no_speed = true;
			throw std::invalid_argument("mode not implemented yet");
			return 6;
		case str2int("wave"):
			no_color = true;
			no_speed = false;
			return 7;
		case str2int("spring"):
			no_color = false;
			no_speed = false;
			return 8;
		case str2int("stack"):
			no_color = false;
			no_speed = false;
			return 9;
		case str2int("cram"):
			no_color = false;
			no_speed = false;
			return 10;
		case str2int("scan"):
			no_color = false;
			no_speed = false;
			return 11;
		case str2int("neon"):
			no_color = false;
			no_speed = false;
			return 12;
		case str2int("water"):
			no_color = true;
			no_speed = false;
			return 13;
		case str2int("rainbow"):
			no_color = true;
			no_speed = false;
			return 14;
		default:
			throw std::invalid_argument("invalid mode selected");
	}
}

uint32_t Cmdline::get_rgb()
{
	if(no_color){
		return 0x00FFFFFF;
	} else if(color.length() == 6){
		try {
			return std::stoul(color, 0, 16);
		} catch(std::invalid_argument) {
			throw std::invalid_argument("invalid color code");
		}
	} else {
		throw std::invalid_argument("invalid color length");
	}
}

uint8_t Cmdline::get_speed()
{
	if(no_speed){
		return 0x80;
	} else if(speed > 100){
		throw std::invalid_argument("invalid speed");
	}

	return speed * 255 / 100;
}

uint8_t Cmdline::get_all()
{
	return all;
}