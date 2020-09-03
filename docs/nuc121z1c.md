# polychrome-nuc121ZC2-re
Reverse engineering of the protocol used for Polychrome Sync in ASRock B550 and TRX40 boards

## Informations
The ASRock B550 and TRX40 boards moved the SMBus connected microcontroller to a Nuvoton NUC121ZC2 ARM microcontroller connected to USB.
This device is responsible for controlling the RGB illumination of the motherboard in up to 8 distinct regions, on/off mode plus 13 special effects independent for each region.

## Configurable fields

### Available commands
| Code (hex) |    COMMAND   |
|:----------:|:------------:|
|     10     |     WRITE    |
|     11     |     READ     |
|     14     | UNIDENTIFIED |
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

### Other fields

#### Second command byte
This byte is always 0.

#### READ/WRITE second-least byte
This byte is always ff in READ and WRITE modes.

#### a4 and 14 commands
These commands are sent on Polychrome Sync software initialization. All of it's fiels are unidentified.

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

### Command a4
This is not yet identified. This is the first command to run when the Polychrome Sync software is initialized.

| a4 (?) | 00 | 00 (?) | 01 (?) | 00 (?) | 00 (?) | 00 (?) | 01 (?) |
|:------:|:--:|:------:|:------:|:------:|:------:|:------:|:------:|

The answer received in the ASRock B550 Steel Legend is:

| a6 (ANSWER) | 00 | 00 (?) | 00 (?) | 02 (?) |
|:-----------:|:--:|:------:|:------:|:------:|


### Command 14
These commands are sent in the following order, after the a4 is sent in the Polychrome Sync initialization.

| 14 (?) | 00 | 02 (?) |
|:------:|:--:|:------:|

The answer is 9 bytes that I will not write down here because I think it could be the device serial number.

| a6 (ANSWER) | 00 | xxxxxxxxxxxxxxxxxx (?) |
|:-----------:|:--:|:----------------------:|

The second time the command is sent is:

| 14 (?) | 00 | 03 (?) |
|:------:|:--:|:------:|

The answer is short and could be the device capabilities or the firmware version

| a6 (ANSWER) | 00 | 00 (?) | 03 (?) | 4c (?) |
|:-----------:|:--:|:------:|:------:|:------:|

The third and last time the command is sent is:

| 14 (?) | 00 | 01 (?) |
|:------:|:--:|:------:|

The answer is short and could be the device capabilities or the firmware version

| a6 (ANSWER) | 00 | 00 (?) | 01 (?) | 3f (?) |
|:-----------:|:--:|:------:|:------:|:------:|

## The Music protocol
In this mode, the Polychrome Sync software reads the sound output of the operating system and adds a byte with the corresponding intensity calculated by the program with an additional byte to the WRITE command. This is sent every 50\~60ms (16.66\~20Hz).

| 10 (WRITE) | 00 | REGION | 06 (MUSIC) | xx | xx | xx | xx | ff | ALL | INTENSITY |
|:----------:|:--:|:------:|:----------:|:--:|:--:|:--:|:--:|:--:|:---:|:---------:|

The device sends a confirmation

| a6 (ANSWER) | 00 | 00 (?) | 07 (OK?) |
|:-----------:|:--:|:------:|:--------:|