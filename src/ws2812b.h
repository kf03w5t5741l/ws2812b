#pragma once
#include <avr/io.h>
#include <inttypes.h>
#include <stddef.h>

#define BYTE_BITS 8

#define DATA_PORT PORTB

#define T0H 2
#define T1H 4
#define T0L 4
#define T1L 2

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
        "   in  %[reg1], %[port]"     "\n\t"
        "   or  %[reg1],  %[pin]"     "\n\t"
        "   ldi %[reg2],    0xFF"     "\n\t"
        "   eor %[reg2],  %[pin]"     "\n\t"
        "   and %[reg2], %[reg1]"     "\n\t"
        "   out %[port], %[reg1]"     "\n\t"

        "high:                  "     "\n\t"
        "   dec %[high]         "     "\n\t"
        "   brne high           "     "\n\t"

        "   out %[port], %[reg2]"     "\n\t"

        "low:                   "     "\n\t"
        "   dec %[low]          "     "\n\t"
        "   brne low            "     "\n\t"

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

void send_pixel(uint8_t pin, pixel c)
{
    send_byte(pin, c.g);
    send_byte(pin, c.r);
    send_byte(pin, c.b);
}

void send_pixel_buf(uint8_t pin, pixel buf[], size_t nmemb)
{
    for (size_t i = 0; i < nmemb; i++) {
        send_pixel(pin, buf[i]);
    }
    _delay_us(RESET_DELAY_US);
}

void initialize_pixel_buf(pixel buf[], size_t nmemb, pixel c) {
    for (size_t i = 0; i < nmemb; i++) {
        buf[i] = c;
    }
}
