#ifndef CONST_H
#define CONST_H

// misc. constants for use in header files

#define FOSC 7372800                        // Clock frequency
#define BAUD 9600                           // Baud rate
#define MYUBRR FOSC/16/BAUD-1               // Value for UBRR0 register
#define BDIV (FOSC / 100000 - 16) / 2 + 1   // Puts I2C rate just below 100kHz (for i2c_init)

#endif // CONST_H