#include <avr/io.h>
#include <stdio.h>

void serial_init(unsigned short);
void serial_out(char);
void serial_outs(char*);
char serial_in(void);

#define FOSC 7372800            // Clock frequency
#define BAUD 9600               // Baud rate
#define MYUBRR FOSC/16/BAUD-1   // Value for UBRR0 register

char output[30];

int main(void) {

  // voltage level test
  serial_init(MYUBRR);

  char in_byte;

  while (1) {

    // loopback test
    in_byte = serial_in();

    // string test
    if (in_byte >= '0' && in_byte <= '9') {
      sprintf(output, "you put in a number: %c\r\n", in_byte);
      serial_outs(output);
    }
    else {
      sprintf(output, "you sinned this way: %c\r\n", in_byte);
      serial_outs(output);
    }
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
  UCSR0C = (3 << UCSZ00);    // Set for async. operation, no parity,
                             // one stop bit, 8 data bits
}


void serial_outs(char *s)
{
    char ch;

    while ((ch = *s++) != '\0')
        serial_out(ch);
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

