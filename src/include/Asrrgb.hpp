/**
 * @file Asrrgb.hpp
 * 
 * @brief This class defines the packet format and USB communication for the 
 * ASRock Polychrome RGB controller.
 * 
 * @details This is intended to be used with B550 and TRX40 motherboard series 
 * with the USB Nuvoton NUC121ZC2 microcontroller.
 */

#pragma once

#include <cstdint>
#include <array>
#include <stdexcept>

#include <libusb-1.0/libusb.h>

class Asrrgb {
public:
	/**
	 * @brief Constructs the object and initializes the USB device.
	 * 
	 * @throw std::runtime_error if USB error occurs.
	 */
	Asrrgb();

	/**
	 * @brief Destroys the object and gracefully release the USB device
	 */
	~Asrrgb();

	/**
	 * @brief Writes the assembled packet to the USB device.
	 * 
	 * @details For details about the codes, check the documentation.
	 * 
	 * @param region	Code of the region to be changed. Ignored case all==0x01.
	 * @param mode		Code of the selected illumination mode.
	 * @param rgb		Code of the hex formatted color. Ignored for off, cycle, rand, wave, water and rainbow modes.
	 * @param speed		Code of the illumination effect speed. Ignored for off and static modes.
	 * @param all		Apply the settings for all regions.
	 * 
	 * @throw std::runtime_error if USB error occurs.
	 */
	void write(uint8_t region, uint8_t mode, uint32_t rgb, uint8_t speed, uint8_t all);

private:
	libusb_context *ctx;
	libusb_device_handle *dev;

	std::array<uint8_t, 64> packet;
};