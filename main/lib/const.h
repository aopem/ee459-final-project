#ifndef CONST_H
#define CONST_H

// misc. constants for use in header files
#define FOSC 7372800                        // Clock frequency
#define BAUD 9600                           // Baud rate
#define MYUBRR FOSC/16/BAUD-1               // Value for UBRR0 register
#define BDIV (FOSC / 100000 - 16) / 2 + 1   // Puts I2C rate just below 100kHz (for i2c_init)

// pin connections
#define SUCCESSFUL_BUTTON   PB0
#define UNSUCCESSFUL_BUTTON PB1
#define BUZZER              PB2
#define RPI                 PC0
#define RED_LED             PC1
#define REED_SWITCH         PC2
#define GREEN_LED_AND_LOCK  PC3
#define SERVO               PD6
#define MOTION_SENSOR       PD7

#endif // CONST_H