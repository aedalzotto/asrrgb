#include "Asrrgb.hpp"

Asrrgb::Asrrgb(uint8_t region, uint8_t mode, uint32_t rgb, uint8_t speed, uint8_t all) :
	ctx(nullptr),
	dev(nullptr)
{
	packet.fill(0);
	packet[0] = 0x10;
	packet[2] = region;
	packet[3] = mode;
	packet[4] = rgb >> 16;
	packet[5] = rgb >> 8;
	packet[6] = rgb;
	packet[7] = speed;
	packet[8] = 0xFF;
	packet[9] = all;

	if(libusb_init(&ctx) < 0)
		throw std::runtime_error("failed to init libusb");

	dev = libusb_open_device_with_vid_pid(ctx, 0x26ce, 0x01a2);
	if(!dev)
		throw std::runtime_error("failed to open ASRock LED Controller, check permissions");

	if(libusb_kernel_driver_active(dev, 0)){
		if(libusb_detach_kernel_driver(dev, 0))
			throw std::runtime_error("failed to detach kernel driver");
	}

	if(libusb_claim_interface(dev, 0) < 0)
		throw std::runtime_error("failed to claim interface");

}

Asrrgb::~Asrrgb()
{
	libusb_release_interface(dev, 0);
	libusb_close(dev);
	dev = nullptr;
	libusb_exit(ctx);
	ctx = nullptr;
}

void Asrrgb::write()
{
	int written = 0;
	int ret = libusb_bulk_transfer(dev, (2 | LIBUSB_ENDPOINT_OUT), packet.data(), 64, &written, 1000);
	if(ret || written != 64)
		throw std::runtime_error("failed to write data");
}