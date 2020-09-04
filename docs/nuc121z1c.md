# polychrome-nuc121ZC2-re
Reverse engineering of the protocol used for Polychrome Sync in ASRock B550 and TRX40 boards

## Informations
The ASRock B550 and TRX40 boards moved the SMBus connected microcontroller to a Nuvoton NUC121ZC2 ARM microcontroller connected to USB.
This device is responsible for controlling the RGB illumination of the motherboard in up to 8 distinct regions, on/off mode plus 13 special effects independent for each region.

Write to endpoint 2, read from endpoint 1.

## Configurable fields

### Available commands
| Code (hex) |    COMMAND   |
|:----------:|:------------:|
|     10     |     WRITE    |
|     11     |     READ     |
|     12     |     EXIT     |
|     14     |  READ HEADER |
|     15     | WRITE HEADER |
|     a4     | UNIDENTIFIED |
|     a6     |    ANSWER    |

### Region identification
| Code (hex) |         REGION         |
|:----------:|:----------------------:|
|     00     |      RGB Header 1      |
|     01     |      RGB Header 2      |
|     02     |  Addressable Header 1  |
|     03     |  Addressable Header 2  |
|     04     |      PCH Heatsink      |
|     05     |        I/O Cover       |
|     06     | PCB back / Audio Cover |
|     07     | Audio Cover / PCB back |

It is possible to set all regions at once setting the field ALL to 1. In this case, the region field will be ignored.
For more details, check the ALL field identification below.

### Mode identification
| Code (hex) |    MODE   |
|:----------:|:---------:|
|     00     |    Off    |
|     01     |   Static  |
|     02     | Breathing |
|     03     |  Strobe   |
|     04     |  Cycling  |
|     05     |  Random   |
|     06     |   Music   |
|     07     |   Wave    |
|     08     |  Spring   |
|     09     |   Stack   |
|     0a     |   Cram    |
|     0b     |   Scan    |
|     0c     |   Neon    |
|     0d     |   Water   |
|     0e     |  Rainbow  |

Modes Off, Cycling, Random, Wave, Water and Rainbow will ignore the color configuration.
Modes Off, Static and Music will ignore the speed configuration.

The Music mode is a special one, where the software needs to send the colors accordingly to the music being played. This is not yet identified.

### Color identification
Each color is represented by 1 byte of R, G and B.

### Speed identification
The speed of the special effects is controlled by 1 byte. Maximum speed is set by 0, and minimum speed is set by 255 (ff).

### ALL identification
It is possible to write to all regions in a single command setting a byte to 1. This way, the region field will be ignored and the same mode, color and speed will be applied to all 7 regions. To manipulate regions individually, set this field to 0.

### Header options
The addressable header command has some options:
| Code (hex) |    OPTION    |
|:----------:|:------------:|
|     01     | UNIDENTIFIED |
|     02     |  ADDRESS CFG |
|     03     |  RGSWAP CFG  |

### RG Swap identification
All 2 non-addressable headers and the 2 addressable ones are capable of RG swapping accordingly to the table below:

| Code (hex) | ahdr1 | ahdr0 | hdr1 | hdr0 |
|:----------:|:-----:|:-----:|:----:|:----:|
|     40     |   NO  |   NO  |  NO  |  NO  |
|     41     |   NO  |   NO  |  NO  |  YES |
|     42     |   NO  |   NO  |  YES |  NO  |
|     43     |   NO  |   NO  |  YES |  YES |
|     44     |   NO  |  YES  |  NO  |  NO  |
|     45     |   NO  |  YES  |  NO  |  YES |
|     46     |   NO  |  YES  |  YES |  NO  |
|     47     |   NO  |  YES  |  YES |  YES |
|     48     |  YES  |   NO  |  NO  |  NO  |
|     49     |  YES  |   NO  |  NO  |  YES |
|     4a     |  YES  |   NO  |  YES |  NO  |
|     4b     |  YES  |   NO  |  YES |  YES |
|     4c     |  YES  |  YES  |  NO  |  NO  |
|     4d     |  YES  |  YES  |  NO  |  YES |
|     4e     |  YES  |  YES  |  YES |  NO  |
|     4f     |  YES  |  YES  |  YES |  YES |

### Other fields

#### Second command byte
This byte is always 0.

#### READ/WRITE second-least byte
This byte is always ff in READ and WRITE modes.

#### a4 command
This commands are sent on Polychrome Sync software initialization. All of it's fiels are unidentified.

## Command details
All commands must have 64 bytes. The padding is made by zeroed bytes.

### Read
This command reads the value of the specific region requested.

| 11 (READ) | 00 | REGION |
|:---------:|:--:|:------:|

The device answers with the region confirmation, the selected mode, colors, speed and if all regions are synchronized.

| a6 (ANSWER) | 00 | REGION | MODE | R | G | B | SPEED | ff | ALL |
|:-----------:|:--:|:------:|:----:|:-:|:-:|:-:|:-----:|:--:|:---:|

### Write
This command writes the R, G, B, colors with the MODE selected at SPEED, optionally syncing all other regions with the field ALL.
In case the mode is Off, Static and Music, the speed will be ignored.
In case the mode is Off, Cycling, Random, Wave, Water and Rainbow, the colors will be ignored.

| 10 (WRITE) | 00 | REGION | MODE | R | G | B | SPEED | ff | ALL |
|:----------:|:--:|:------:|:----:|:-:|:-:|:-:|:-----:|:--:|:---:|

The device sends a confirmation

| a6 (ANSWER) | 00 | 00 (?) | 07 (OK?) |
|:-----------:|:--:|:------:|:--------:|

### Read header
This command reads the addressable and non-addressable headers configurations:

| 14 (READ HDR) | 00 | 01 (?) |
|:-------------:|:--:|:------:|

The device sends the answer still unknown.

| a6 (ANSWER) | 00 | 00 (?) | 01 (?) | 3f (?) |
|:-----------:|:--:|:------:|:------:|:------:|

This command reads the addressable headers configured addresses:

| 14 (READ HDR) | 00 | 02 (ADDRESS CFG) |
|:-------------:|:--:|:----------------:|

The device answers in the following format:

| a6 (ANSWER) | 00 | 00 (?) | 02 (ADDRESS CFG) | 01 (?) | 01 (?) | ahdr0 ADDRESS | ahdr1 ADDRESS | 05 (?) | 07 (?) | 1e (?) | 1e (?) |
|:-----------:|:--:|:------:|:----------------:|:------:|:------:|:-------------:|:-------------:|:------:|:------:|:------:|:------:|

This command reads the headers RG swap:

| 14 (READ HDR) | 00 | 03 (RGSWAP CFG) |
|:-------------:|:--:|:---------------:|

The device answers in the following format, see above the RGSwap identification for more information.

| a6 (ANSWER) | 00 | 00 (?) | 03 (RGSWAP CFG) | RGSwap |
|:-----------:|:--:|:------:|:---------------:|:------:|

### Write header
This command configures the addressable and non-addressable headers RGSwap and selected addresses (when applied).

To configure the addresses, where ahdr0 ADDRESS and ahdr1 ADDRESS are the adressable headers 0 and 1 LED addresses in hex:

| 15 (WRITE HDR) | 00 | 00 (?) | 02 (ADDRESS CFG) | 01 (?) | 01 (?) | ahdr0 ADDRESS | ahdr1 ADDRESS | 05 (?) | 07 (?) | 1e (?) | 1e (?) |
|:--------------:|:--:|:------:|:----------------:|:------:|:------:|:-------------:|:-------------:|:------:|:------:|:------:|:------:|

The device sends a confirmation

| a6 (ANSWER) | 00 | 00 (?) | 07 (OK?) |
|:-----------:|:--:|:------:|:--------:|

To configure the RGSwap, send the following command:
| 15 (WRITE HDR) | 00 | 00 (?) | 03 (RGSWAP CFG) | RGSWAP |
|:--------------:|:--:|:------:|:---------------:|:------:|

The device sends a confirmation

| a6 (ANSWER) | 00 | 00 (?) | 07 (OK?) |
|:-----------:|:--:|:------:|:--------:|

### Exit

On Polychrome Sync exit, it sends the device a command:

| 12 (EXIT) |
|:---------:|

### Command a4
This is not yet identified. This is the first command to run when the Polychrome Sync software is initialized.

| a4 (?) | 00 | 00 (?) | 01 (?) | 00 (?) | 00 (?) | 00 (?) | 01 (?) |
|:------:|:--:|:------:|:------:|:------:|:------:|:------:|:------:|

The answer received in the ASRock B550 Steel Legend with Polychrome Sync v2.62 is:

| a6 (ANSWER) | 00 | 00 (?) | 00 (?) | 02 (?) |
|:-----------:|:--:|:------:|:------:|:------:|

and the aswer received in the ASRock B550 Steel Legend with Polychrome Sync v2.66 (beta), after suffering a firmware update is:

| a6 (ANSWER) | 00 | 00 (?) | 00 (?) | 02 (?) |
|:-----------:|:--:|:------:|:------:|:------:|

So this probably isn't the firmware version.

Sometimes the device answers like this:

| a6 (ANSWER) | 00 | 00 (?) | 07 (?) | 02 (?) |
|:-----------:|:--:|:------:|:------:|:------:|

And it triggers a whole new type of packets, in the end resulting in a ABORT_PIPE, and making the Polychrome Sync software querying the device again.

## The Music protocol
In this mode, the Polychrome Sync software reads the sound output of the operating system and adds a byte with the corresponding intensity calculated by the program with an additional byte to the WRITE command. This is sent every 50\~60ms (16.66\~20Hz).

| 10 (WRITE) | 00 | REGION | 06 (MUSIC) | xx | xx | xx | xx | ff | ALL | INTENSITY |
|:----------:|:--:|:------:|:----------:|:--:|:--:|:--:|:--:|:--:|:---:|:---------:|

The device sends a confirmation

| a6 (ANSWER) | 00 | 00 (?) | 07 (OK?) |
|:-----------:|:--:|:------:|:--------:|
