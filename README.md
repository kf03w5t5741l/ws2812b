# WS2812B

ATmega328P driver library for WS2812/WS2812B LED strips. Features: not many. Includes tasty AVR assembly. Tested on an Arduino Uno with the WS2812B chip.

## Usage

```c
#define __AVR_ATmega32U4__
#include <avr/io.h>
#include <inttypes.h>
#include <stddef.h>

#define F_CPU 16000000UL
#include <util/delay.h>

#include "ws2812b.h"

#define DELAY_US 10000
#define DATA_PIN 0
#define PIXELS 30

int main ()
{
    ws2812b_setup_pin(DATA_PIN);
    _delay_us(DELAY_US);

    pixel buf[PIXELS] = { 0 };
    const size_t pixels = sizeof(buf) / sizeof(pixel);

    // clear any pixels by sending empty buffer
    ws2812b_send_pixels(DATA_PIN, buf, pixels);

    const pixel red = { .r = 0xFF, .g = 0x00, .b = 0x00 };
    buf[0] = red;

    ws2812b_initialize_pixel_buf(buf, pixels, red);

    while(1) {
        // shift all pixels one right for visual effect
        pixel last = buf[pixels - 1];
        for (size_t i = pixels - 1; i > 0; i--) {
            buf[i] = buf[i - 1];
        }
        buf[0] = last;

        // transmit our pixel buffer to the LED strip
        ws2812b_send_pixels(DATA_PIN, buf, pixels);

        // wait 10ms
        _delay_us(DELAY_US);
    }
}
```

## Example build instructions

### Build requirements

🐧: `gcc-avr`, `avr-libc`, `binutils`, `avrdude`, `cmake`

🍎: `avr-gcc`, `avr-binutils`, `avrdude`, `cmake` (install via Homebrew)

### Build instructions

> **_All:_** Check and adjust the `avrdude` port and programmer arguments before executing `run.sh`.

> **_Linux users:_** You may need to modify `build.sh` and `run.sh` so that the `-DCMAKE_TOOLCHAIN_FILE` argument reflects the full path to the `avr.cmake` file.

Run `build.sh` to build. Run `run.sh` to build and upload with `avrdude`.

## Resources

For more on how to transmit data to the WS2812(B) LED strips, see cpldcpu's excellent [analysis](https://cpldcpu.wordpress.com/2014/01/14/light_ws2812-library-v2-0-part-i-understanding-the-ws2812/) and his [light_ws2812](https://github.com/cpldcpu/light_ws2812) library, which applies broadly the same approach as the above and works for many more platforms.

The datasheets for the WS2812 and WS2812B chips are available here:

- [WS2812 Datasheet](https://cdn-shop.adafruit.com/datasheets/WS2812.pdf)
- [WS2812B Datasheet](https://cdn-shop.adafruit.com/datasheets/WS2812B.pdf)


For more on the AVR assembly language and how to incorporate it into C with the GNU C compiler for AVR:

- [Beginners Programming in AVR Assembler](http://www.avr-asm-tutorial.net/avr_en/beginner/index.html)
- [AVR-GCC Inline Assembler Cookbook](https://www.nongnu.org/avr-libc/user-manual/inline_asm.html)

