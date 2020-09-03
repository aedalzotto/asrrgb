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

#include <iostream>

#include "Cmdline.hpp"
#include "Asrrgb.hpp"

int main(int argc, char *argv[])
{
	Cmdline cmd(argc, argv);
	if(cmd.is_help()){
		std::cout << cmd.help() << std::endl;
		return 1;
	}
	try {
		uint8_t region = cmd.get_region();
		uint8_t mode = cmd.get_mode();
		uint32_t rgb = cmd.get_rgb();
		uint8_t speed = cmd.get_speed();
		uint8_t all = cmd.get_all();

		Asrrgb device;
		device.write(region, mode, rgb, speed, all);
	} catch(std::invalid_argument& e){
		std::cerr << e.what() << std::endl;
		return -1;
	} catch(std::runtime_error& e){
		std::cerr << e.what() << std::endl;
		return -1;
	}
	return 0;
}