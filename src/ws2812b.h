#pragma once
#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <stddef.h>

#define BYTE_BITS 8

#define DATA_DIRECTION_REGISTER DDRB
#define DATA_PORT PORTB

#define T0H 2   // evaluates to 1 * 3 + 2 + 1  =  6 cycles  (0.375 μs)
#define T1H 4   // evaluates to 3 * 3 + 2 + 1  = 12 cycles  (0.75  μs)
#define T0L 3   // evaluates to 2 * 3 + 2 + 10 = 18 cycles  (1.125 μs)
#define T1L 1   // evaluates to 0 * 3 + 2 + 10 = 12 cycles  (0.75  μs)

#define RESET_DELAY_US 150 

typedef struct pixel {
    uint8_t g;
    uint8_t r;
    uint8_t b;
} pixel;

void ws2812_setup_pin(uint8_t pin) {
    DATA_DIRECTION_REGISTER |= 1 << pin;
}

void ws2812_send_bytes(uint8_t pin, uint8_t buf[], uint8_t nmemb)
{
    volatile uint8_t reg1, reg2, val, bit, high, low;

    cli();                          // disable interrupts

    asm volatile(
        "   in  %[reg1], %[port]    "   "\n\t"    // make sure we preserve the
        "   or  %[reg1],  %[pin]    "   "\n\t"    // rest of the DATA_PORT state
        "   ldi %[reg2],    0xFF    "   "\n\t"    // and only set and clear the
        "   eor %[reg2],  %[pin]    "   "\n\t"    // DATA_PIN bit
        "   and %[reg2], %[reg1]    "   "\n\t"

        "byte:                      "   "\n\t"    // byte loop start
        "   ldi  %[bit], %[bits]    "   "\n\t"      
        "   ld   %[val], %a[buf]+   "   "\n\t"

        "bit:                       "    "\n\t"   // bit loop start
        "   lsl %[val]              "    "\n\t"
        "   brcs one                "    "\n\t"

        "zero:                      "    "\n\t"
        "   ldi %[high], %[t0h]     "    "\n\t"
        "   ldi %[low],  %[t0l]     "    "\n\t"
        "   rjmp high               "    "\n\t"

        "one:                       "    "\n\t"
        "   ldi %[high], %[t1h]     "    "\n\t"
        "   ldi %[low],  %[t1l]     "    "\n\t"

        "high:                      "    "\n\t"
        "   out %[port], %[reg1]    "    "\n\t"    // set DATA_PIN high

        "keephigh:                  "    "\n\t"    // voltage high loop
        "   dec %[high]             "    "\n\t"
        "   brne keephigh           "    "\n\t"

        "low:                       "    "\n\t"
        "   out %[port], %[reg2]    "    "\n\t"    // set bit low

        "keeplow:                   "    "\n\t"    // voltage low loop
        "   dec %[low]              "    "\n\t"
        "   brne keeplow            "    "\n\t"
        
        "   dec %[bit]              "    "\n\t"
        "   brne bit                "    "\n\t"     // bit loop end

        "   dec %[nmemb]            "    "\n\t"
        "   brne byte               "    "\n\t"     // byte loop end

      : [reg1]  "=&r" (reg1),
        [reg2]  "=&r" (reg2),
        [bit]   "+&r" (bit),
        [val]   "+&r" (val),
        [buf]   "+&e" (buf),
        [high]  "+&r" (high),
        [low]   "+&r" (low),
        [nmemb] "+&r" (nmemb)
      : [bits]  "I" (BYTE_BITS),
        [t0h]   "I" (T0H),
        [t0l]   "I" (T0L),
        [t1h]   "I" (T1H),
        [t1l]   "I" (T1L),
        [port]  "I" (_SFR_IO_ADDR(DATA_PORT)),
        [pin]   "r" (1 << pin)
    );

    sei();                          // enable interrupts

}

void ws2812_send_pixels(uint8_t pin, pixel buf[], size_t nmemb)
{
    ws2812_send_bytes(pin, (uint8_t*) buf, nmemb * sizeof(pixel));
    _delay_us(RESET_DELAY_US);      // pause to signal end of transmission
}

void ws2812_initialize_pixel_buf(pixel buf[], size_t nmemb, pixel p) {
    for (size_t i = 0; i < nmemb; i++) {
        buf[i] = p;
    }
}
