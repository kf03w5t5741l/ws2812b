#pragma once
#include <avr/io.h>
#include <inttypes.h>
#include <stddef.h>

#define BYTE_BITS 8

#define DATA_PORT PORTB

#define T0H 1   // evaluates to 1 * 3 + 2 + 1 = 6 cycles    (0.375  μs)
#define T1H 3   // evaluates to 3 * 3 + 2 = 11 cycles       (0.6875 μs)
#define T0L 4   // evaluates to 4 * 3 + 2 = 14 cycles       (0.875  μs)
#define T1L 2   // evaluates to 2 * 3 + 2 + 1 = 9 cycles    (0.5625 μs)

#define RESET_DELAY_US 300

typedef struct pixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} pixel;

void send_bit(uint8_t pin, uint8_t value)
{
    volatile uint8_t high = 0;
    volatile uint8_t low = 0;

    if (value) {
      high = T1H;
      low = T1L;
    } else {
      high = T0H;
      low = T0L;
    }

    volatile uint8_t reg1;
    volatile uint8_t reg2;

    asm volatile(
        "   in  %[reg1], %[port]"     "\n\t"    // this implementation 
        "   or  %[reg1],  %[pin]"     "\n\t"    // preserves the DATA_PORT
        "   ldi %[reg2],    0xFF"     "\n\t"    // state and only sets and
        "   eor %[reg2],  %[pin]"     "\n\t"    // clears the the DATA_PIN bit
        "   and %[reg2], %[reg1]"     "\n\t"
        "   out %[port], %[reg1]"     "\n\t"    // set DATA_PIN high

        "high:                  "     "\n\t"    // voltage high loop:
        "   dec %[high]         "     "\n\t"    // 1 cycle
        "   brne high           "     "\n\t"    // 2 cycles on branch, else 1

        "   out %[port], %[reg2]"     "\n\t"    // set bit low, 1 cycle

        "low:                   "     "\n\t"    // voltage low loop:
        "   dec %[low]          "     "\n\t"    // 1 cycle
        "   brne low            "     "\n\t"    // 2 cycle on brandch, else 1

      :
      : [reg1]  "r" (reg1),
        [reg2]  "r" (reg2),
        [port]  "I" (_SFR_IO_ADDR(DATA_PORT)),
        [pin]   "r" (1 << pin),
        [high]  "r" (high),
        [low]   "r" (low)
    );
}

void send_byte(uint8_t pin, uint8_t byte)
{
    for (volatile uint8_t i = 0; i < BYTE_BITS; i++) {
        volatile uint8_t bit = byte & 1 << (BYTE_BITS - i - 1);
        send_bit(pin, bit);
    }
}

void send_pixel(uint8_t pin, pixel p)
{
    send_byte(pin, p.g);
    send_byte(pin, p.r);
    send_byte(pin, p.b);
}

void send_pixel_buf(uint8_t pin, pixel buf[], size_t nmemb)
{
    for (size_t i = 0; i < nmemb; i++) {
        send_pixel(pin, buf[i]);
    }
    _delay_us(RESET_DELAY_US); // pause to signal our transmission has ended
}

void initialize_pixel_buf(pixel buf[], size_t nmemb, pixel p) {
    for (size_t i = 0; i < nmemb; i++) {
        buf[i] = p;
    }
}
