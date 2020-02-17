#include <avr/io.h>

int main(void) {
  // set all I/O pins to zeros
  PORTB = 0x00;
  PORTC = 0x00;
  PORTD = 0x00;

  // set all I/O pins to outputs
  DDRB |= 0xFF;
  // DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3) |
  //         (1 << PB4) | (1 << PB5) | (1 << PB7);
  // DDRB |= 0xBF;
  DDRC |= 0xFF;
  DDRD |= 0xFF;

  // loop through pins
  while (1) {
    PORTB |=  (1 << PB1); // 15, trigger pin
    PORTB &= ~(1 << PB1); 
    PORTB |=  (1 << PB2); // 16
    PORTB &= ~(1 << PB2);
    PORTB |=  (1 << PB3); // 17
    PORTB &= ~(1 << PB3); 
    PORTB |=  (1 << PB4); // 18
    PORTB &= ~(1 << PB4);
    PORTB |=  (1 << PB5); // 19
    PORTB &= ~(1 << PB5);
    PORTB |=  (1 << PB7); // 10 
    PORTB &= ~(1 << PB7);
    PORTB |=  (1 << PB0); // 14
    PORTB &= ~(1 << PB0);

    PORTC |=  (1 << PC0); // 23
    PORTC &= ~(1 << PC0);
    PORTC |=  (1 << PC1); // 24
    PORTC &= ~(1 << PC1);
    PORTC |=  (1 << PC2); // 25
    PORTC &= ~(1 << PC2);
    PORTC |=  (1 << PC3); // 26
    PORTC &= ~(1 << PC3);
    PORTC |=  (1 << PC4); // 27
    PORTC &= ~(1 << PC4);
    PORTC |=  (1 << PC5); // 28
    PORTC &= ~(1 << PC5);

    PORTD |=  (1 << PD0); // 2
    PORTD &= ~(1 << PD0);
    PORTD |=  (1 << PD1); // 3
    PORTD &= ~(1 << PD1);
    PORTD |=  (1 << PD2); // 4
    PORTD &= ~(1 << PD2);
    PORTD |=  (1 << PD3); // 5
    PORTD &= ~(1 << PD3);
    PORTD |=  (1 << PD4); // 6
    PORTD &= ~(1 << PD4);
    PORTD |=  (1 << PD5); // 11
    PORTD &= ~(1 << PD5);
    PORTD |=  (1 << PD6); // 12
    PORTD &= ~(1 << PD6);
    PORTD |=  (1 << PD7); // 13
    PORTD &= ~(1 << PD7);
  }

  return 0;
}