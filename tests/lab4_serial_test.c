#include <avr/io.h>

void serial_init(unsigned short);
void serial_out(char);
char serial_in(void);

#define FOSC 9830400            // Clock frequency
#define BAUD 19200              // Baud rate used by the LCD
#define MYUBRR FOSC/16/BAUD-1   // Value for UBRR0 register

char str1[] = "12345678901234567890";
char str2[] = "abcdefghijklmnop";
char str3[] = "a1b2c3e4f5g6h7i8j9";

int main(void) {

  // voltage level test
  serial_init(MYUBRR);

  while (1) {

    // loopback test
    // serial_out("a");
  }

  return 0;
}

// Serial Communication Functions
/*
 * serial_init - Initialize the USART port
 */
void serial_init(unsigned short ubrr) {
  UBRR0 = ubrr;              // Set baud rate
  UCSR0B |= (1 << TXEN0);    // Turn on transmitter
  UCSR0B |= (1 << RXEN0);    // Turn on receiver
  UCSR0C = (3 << UCSZ00);    // Set for async . operation , no parity ,
                             // one stop bit , 8 data bits
}

/*
 * serial_out - Output a byte to the USART0 port
 */
void serial_out(char ch)
{
  while ((UCSR0A & (1 << UDRE0)) == 0);
  UDR0 = ch;
}

/*
 * serial_in - Read a byte from the USART0 and return it
 */
char serial_in()
{
  while (!(UCSR0A & (1 << RXC0)));
  return UDR0;
}

