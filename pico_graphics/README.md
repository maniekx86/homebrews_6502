# GPU_6502

Graphics interface for my 6502 homebrew PCB prototype board version

Uses Raspberry Pi Pico PIO to hook up on the 6502 bus. To compile use this [Arduino RP2040 core](https://github.com/earlephilhower/arduino-pico) and install PicoDVI library

Registers:
- A0 = 0: Data register 
- A0 = 1: Command select register

Commands: 
- 00 - Write data at current cursor position
- 01 - Write cursor X position
- 02 - Write cursor Y position


