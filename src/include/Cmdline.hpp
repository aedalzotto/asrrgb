#pragma once

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

class Cmdline {
public:
	Cmdline(int argc, char *argv[]);
	bool is_help();
	const boost::program_options::options_description& help();
	uint8_t get_region();
	uint8_t get_mode();
	uint32_t get_rgb();
	uint8_t get_speed();
	uint8_t get_all();

private:
	boost::program_options::options_description desc;
	boost::program_options::variables_map vm;

	std::string region;
	std::string mode;
	std::string color;
	uint8_t speed;
	uint8_t all;

	bool no_speed;
	bool no_color;
};
