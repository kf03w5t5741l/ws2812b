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
    ws2812_setup_pin(DATA_PIN);
    _delay_us(DELAY_US);

    pixel buf[PIXELS] = { 0 };
    const size_t pixels = sizeof(buf) / sizeof(pixel);

    // clear any pixels by sending empty buffer
    ws2812_send_pixels(DATA_PIN, buf, pixels);

    const pixel red = { .r = 0x50, .g = 0x00, .b = 0x00 };
    buf[0] = red;

    while(1) {
        // shift all pixels one right for visual effect
        pixel last = buf[pixels - 1];
        for (size_t i = pixels - 1; i > 0; i--) {
            buf[i] = buf[i - 1];
        }
        buf[0] = last;

        // transmit our pixel buffer to the LED strip
        ws2812_send_pixels(DATA_PIN, buf, pixels);

        // wait 10ms
        _delay_us(DELAY_US);
    }
}
