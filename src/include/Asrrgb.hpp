#pragma once

#include <cstdint>
#include <array>
#include <stdexcept>

#include <libusb-1.0/libusb.h>

class Asrrgb {
public:
	Asrrgb(uint8_t region, uint8_t mode, uint32_t rgb, uint8_t speed, uint8_t all);
	~Asrrgb();
	void write();
private:
	libusb_context *ctx;
	libusb_device_handle *dev;

	std::array<uint8_t, 64> packet;
};