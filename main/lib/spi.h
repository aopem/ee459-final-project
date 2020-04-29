#ifndef SPI_H
#define SPI_H

#include <avr/io.h>

#define SPI_DDR   DDRB
#define SPI_PORT  PORTB
#define SPI_PIN   PINB
#define SPI_MOSI  PB3
#define SPI_MISO  PB4
#define SPI_SS    PB2 // not necessary, we are always master
#define SPI_SCK   PB5

#define ENABLE_CHIP() (SPI_PORT &= (~(1<<SPI_SS)))
#define DISABLE_CHIP() (SPI_PORT |= (1<<SPI_SS))

void spi_init();
uint8_t spi_transmit(uint8_t data);

#endif // SPI_H