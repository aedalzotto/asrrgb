/**
 * @file Cmdline.hpp
 * 
 * @brief This class defines the argument parser for the assrgb.
 */

#pragma once

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

class Cmdline {
public:
	/**
	 * @brief Constructs the argument parser
	 * 
	 * @param argc Argument count
	 * @param argv Pointer to the array of arguments
	 */
	Cmdline(int argc, char *argv[]);

	/**
	 * @brief Checks if help option should be called
	 * 
	 * @return True if help must be shown
	 */
	bool is_help();

	/**
	 * @brief Gets the help message
	 * 
	 * @return Reference to arguments description
	 */
	const boost::program_options::options_description& help();

	/**
	 * @brief Gets the region code chosen
	 * 
	 * @return A byte of the encoded region
	 */
	uint8_t get_region();

	/**
	 * @brief Gets the mode chosen
	 * 
	 * @return A byte of the encoded mode
	 */
	uint8_t get_mode();

	/**
	 * @brief Gets the color code
	 * 
	 * @return The RGB color code
	 */
	uint32_t get_rgb();

	/**
	 * @brief Gets the speed chosen
	 * 
	 * @return A byte of the encoded speed
	 */
	uint8_t get_speed();

	/**
	 * @brief Gets if all regions must be set
	 * 
	 * @return A encoded byte of the ALL command
	 */
	uint8_t get_all();

private:
	static std::unordered_map<std::string, std::pair<uint8_t, uint8_t>> const region_table;
	static std::unordered_map<std::string, std::tuple<uint8_t, bool, bool>> const mode_table;

	boost::program_options::options_description desc;
	boost::program_options::variables_map vm;

	uint8_t region;
	uint8_t mode;
	uint32_t color;
	int speed;
	uint8_t all;
};
