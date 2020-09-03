# asrrgb
Control ASRock's B550 and TRX40 motherboard series RGB LEDs

## About
This program is intended to control the motherboard RGB LEDs and headers without needing to boot to Windows. Currently, it is unable to identify the capabilities of the target device, so use with caution.

For more information, check the [protocol reverse engineering](/docs/nuc121z1c.md).

## Usage

Check the usage running
```
./asrrgb --help
```

To run as a normal user, give the udev permissions copying the file `60-asrrgb.rules` to `/etc/udev/rules.d/`.

## Installation

### Arch Linux

There is a `asrrgb-bin` package in AUR.

## Compiling from source

You will need:
* C++ compiler (g++)
* Boost libs (for boost::program_options)
* Libusb

Run
```
make
```
