#define __AVR_ATmega32U4__
#include <avr/io.h>
#include <inttypes.h>
#include <stddef.h>

#define F_CPU 16000000UL
#include <util/delay.h>

#define BYTE_BITS 8

#define DELAY_ON 900
#define DELAY_OFF 900

#define T0H 2
#define T1H 6
#define T0L 6
#define T1L 3

#define TX_PORT PORTB
#define TX_PIN 0x0 

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

        "high:                 "     "\n\t"
        "   dec %[high]        "     "\n\t"
        "   brne high          "     "\n\t"

        "   out %[port], 0"     "\n\t"

        "low:                  "     "\n\t"
        "   dec %[low]         "     "\n\t"
        "   brne low           "     "\n\t"

      :
      : [reg1]  "r" (reg1),
        [reg2]  "r" (reg2),
        [port]  "I" (_SFR_IO_ADDR(TX_PORT)),
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

int main ()
{
    DDRB |= 1 << TX_PIN;

    while(1) {
        _delay_us(500);

        for (size_t i = 0; i < 10; i++) {
            send_byte(TX_PIN, 0x00);
            send_byte(TX_PIN, 0xF0);
            send_byte(TX_PIN, 0x00);
        }

  }
}
