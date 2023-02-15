# ST7565R library for RP2040

This is a library I created for my [personal project](https://github.com/skylordants/Pico_receiver). I'm sharing it, in case it is helpful to somebody and to make my code public, but I don't intend on regularily maintaining it, because it works for me, unless something big happens.

## Usage
The simplest way to use the library is to include it in a platformio project using [wizio-pico baremetal](https://github.com/Wiz-IO/wizio-pico) platform, but other projects using the RP2040 SDK should work as well.

An ST7565R object can be created by calling it's constructor and giving it spi pins. After that *write_chars* can be used to write characters. Functions *ram_select_address* and *ram_write* can be used to display pixels directly.

## Sources
Most of the stuff is based on the [datasheet](https://www.lcd-module.de/eng/pdf/zubehoer/st7565r.pdf). In the beginning, to get sense of things, Adafruit's Arduino and AVR [ST7565-LCD](https://github.com/adafruit/ST7565-LCD) was very useful. "glcdfont.h" is mostly copied from there, only the degree symbol is my contribution. Also, the initialization sequence is mostly taken from there. But I wrote all the code myself, directly only copied a font, the initialization sequence is also described in the datasheet and Adafruit's library is unlicensed, so I don't think I'm committing any license infringement. If I am and I need to add something or it needs to be taken down, you can contact me. Please don't come after me with lawyers.
