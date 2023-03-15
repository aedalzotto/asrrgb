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

#include <unordered_map>

#include <boost/program_options/parsers.hpp>
#include "Cmdline.hpp"

namespace po = boost::program_options;

std::unordered_map<std::string, std::pair<uint8_t, uint8_t>> const Cmdline::region_table {
	{"hdr0",  std::make_pair<uint8_t, uint8_t>(0, 0)},
	{"hdr1",  std::make_pair<uint8_t, uint8_t>(1, 0)},
	{"ahdr0", std::make_pair<uint8_t, uint8_t>(2, 0)},
	{"ahdr1", std::make_pair<uint8_t, uint8_t>(3, 0)},
	{"pch",   std::make_pair<uint8_t, uint8_t>(4, 0)},
	{"io",    std::make_pair<uint8_t, uint8_t>(5, 0)},
	{"audio", std::make_pair<uint8_t, uint8_t>(6, 0)},
	{"pcb",   std::make_pair<uint8_t, uint8_t>(7, 0)},
	{"all",   std::make_pair<uint8_t, uint8_t>(0, 0)},
};

std::unordered_map<std::string, std::tuple<uint8_t, bool, bool>> const Cmdline::mode_table {
	{"off",     std::make_tuple<uint8_t, bool, bool>( 0,  true,  true)},
	{"static",  std::make_tuple<uint8_t, bool, bool>( 1, false,  true)},
	{"breath",  std::make_tuple<uint8_t, bool, bool>( 2, false, false)},
	{"strobe",  std::make_tuple<uint8_t, bool, bool>( 3, false, false)},
	{"cycle",   std::make_tuple<uint8_t, bool, bool>( 4,  true, false)},
	{"rand",    std::make_tuple<uint8_t, bool, bool>( 5,  true, false)},
	{"music",   std::make_tuple<uint8_t, bool, bool>( 6,  true,  true)},
	{"wave",    std::make_tuple<uint8_t, bool, bool>( 7,  true, false)},
	{"spring",  std::make_tuple<uint8_t, bool, bool>( 8, false, false)},
	{"stack",   std::make_tuple<uint8_t, bool, bool>( 9, false, false)},
	{"cram",    std::make_tuple<uint8_t, bool, bool>(10, false, false)},
	{"scan",    std::make_tuple<uint8_t, bool, bool>(11, false, false)},
	{"neon",    std::make_tuple<uint8_t, bool, bool>(12, false, false)},
	{"water",   std::make_tuple<uint8_t, bool, bool>(13,  true, false)},
	{"rainbow", std::make_tuple<uint8_t, bool, bool>(14,  true, false)},
};

Cmdline::Cmdline(int argc, char *argv[]) :
	desc("Allowed options")
{
	std::string region_arg;
	std::string mode_arg;
	std::string color_arg;

	desc.add_options()
		("help", "produce help message")
		("region", po::value<std::string>(&region_arg)->default_value("all"), "region to change the lights: hdr0, hdr1, ahdr0, ahdr1, pch, io, audio, pcb or all")
		("mode", po::value<std::string>(&mode_arg)->default_value("static"), "mode of illumination: off, static, breath, strobe, cycle, rand, wave, spring, stack, cram, scan, neon, water or rainbow")
		("color", po::value<std::string>(&color_arg)->default_value("ffffff"), "hex code of the color, ignored case mode is off, cycle, rand, wave, water or rainbow")
		("speed", po::value<int>(&speed)->default_value(50), "0 to 100 speed of the mode, ignored for off and static")
	;

	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if(vm.count("help"))
		return;

	try {
		region = region_table.at(region_arg).first;
		all    = region_table.at(region_arg).second;
	} catch(...) {
		throw std::invalid_argument("invalid region selected");
	}

	bool no_color = false;
	bool no_speed = false;
	try {
		mode     = std::get<0>(mode_table.at(mode_arg));
		no_color = std::get<1>(mode_table.at(mode_arg));
		no_speed = std::get<2>(mode_table.at(mode_arg));
	} catch(...){
		throw std::invalid_argument("invalid mode selected");
	}

	if(mode == 6)
		throw std::invalid_argument("music mode not implemented yet");

	if(no_color){
		color = 0x00FFFFFF;
	} else if(color_arg.length() == 6){
		try {
			color = std::stoul(color_arg, 0, 16);
		} catch(std::invalid_argument const&) {
			throw std::invalid_argument("invalid color code");
		}
	} else {
		throw std::invalid_argument("invalid color length");
	}

	if(no_speed){
		speed = 0x80;
	} else if(speed > 100 || speed < 0){
		throw std::invalid_argument("invalid speed");
	} else {
		speed = (speed * 255 / 100 - 255) * -1;
	}

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
	return region;
}

uint8_t Cmdline::get_mode()
{
	return mode;
}

uint32_t Cmdline::get_rgb()
{
	return color;
}

uint8_t Cmdline::get_speed()
{
	return speed;
}

uint8_t Cmdline::get_all()
{
	return all;
}
