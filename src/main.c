#define __AVR_ATmega32U4__
#include <avr/io.h>
#include <inttypes.h>
#include <stddef.h>

#define F_CPU 16000000UL
#include <util/delay.h>

#include "ws2812b.h"

#define DELAY_US 8000 

#define DATA_PIN 0x0 

#define PIXELS 30

int main ()
{
    DDRB |= 1 << DATA_PIN;

    pixel black = { .r = 0x00, .g = 0x00, .b = 0x00 };
    pixel red = { .r = 0x50, .g = 0x00, .b = 0x00 };

    pixel buf[PIXELS] = { 0 };
    buf[0] = red;
    const size_t pixels = sizeof(buf) / sizeof(pixel);

    for (size_t i = 0; i < pixels; i++) {
        send_pixel(DATA_PIN, black);
    }

    while(1) {

        pixel last = buf[pixels - 1];
        for (size_t i = pixels - 1; i > 0; i--) {
            buf[i] = buf[i - 1];
        }
        buf[0] = last;

        send_pixel_buf(DATA_PIN, buf, pixels);

        _delay_us(DELAY_US);

    }
}
