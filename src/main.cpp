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

		Asrrgb device(region, mode, rgb, speed, all);
		device.write();
	} catch(std::invalid_argument& e){
		std::cerr << e.what() << std::endl;
		return -1;
	} catch(std::runtime_error& e){
		std::cerr << e.what() << std::endl;
		return -1;
	}
	return 0;
}