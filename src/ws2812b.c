#define __AVR_ATmega32U4__
#include <avr/io.h>

#define F_CPU 16000000UL
#include <util/delay.h>

#define DELAY_ON 100
#define DELAY_OFF 900

int main (void)
{
  DDRB |= 0x20;

  while(1) {

    asm volatile(
      "sbi %[port], %[bit]"   "\n\t"
      : 
      : [port] "I" (_SFR_IO_ADDR(PORTB)), [bit]  "M" (0x5)
    );
    _delay_ms(DELAY_ON);

    asm volatile(
      "cbi %[port], %[bit]"   "\n\t"
      : 
      : [port] "I" (_SFR_IO_ADDR(PORTB)), [bit]  "M" (0x5)
    );
    _delay_ms(DELAY_OFF);

  }
}
