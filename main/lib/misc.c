#include <avr/io.h>
#include <util/delay.h>
#include <const.h>

void button_init(int pin) {
  DDRB  &= ~(1 << pin);   // set pin as input (both buttons on DDRB)
  PORTB |=  (1 << pin);   // turn on internal pull-up resistor
}

int button_pressed(int pin) {
  return !(PINB & pin);   // return opposite of PINB value since button
}                         // not pressed when input high and pressed when
                          // input is low

// do not need function for green LED since
// it is connected to solenoid lock pin
void led_red_init(void) {
  DDRC |= (1 << RED_LED); // configure as output
}

void led_red(int on) {
  if (on)
    PORTC |= (1 << RED_LED);
  else 
    PORTC &= ~(1 << RED_LED);
}

void motion_sensor_init(void) {
  DDRD &= ~(1 << MOTION_SENSOR);      // configure as input
}

int motion_detected(void) {
  return (PIND & MOTION_SENSOR);
}

void lock_init(void) {
  DDRC |= (1 << GREEN_LED_AND_LOCK);  // configure output to MOSFET
  DDRC &= ~(1 << REED_SWITCH);        // configure reed switch input
}

void lock_toggle(int on) {
  if (on)
    PORTC |= (1 << GREEN_LED_AND_LOCK);
  else 
    PORTC &= ~(1 << GREEN_LED_AND_LOCK);
}

int lock_check(void) {
  return !(PINC & REED_SWITCH);       // reed switch is high when no connection
}                                     // then gets pulled to GND on connect


// buzzer code modified from
// https://www.robomart.com/blog/buzzer-io-interfacing-atmega32-microcontroller/
void buzzer_on(void) {
  unsigned char port_restore = 0;
  port_restore = PINC;
  port_restore |= 0x08;
  PORTC = port_restore;
}
 
void buzzer_off(void) {
  unsigned char port_restore = 0;
  port_restore = PINC;
  port_restore &= 0xF7;
  PORTC = port_restore;
}

/* functions for communicating with Raspberry Pi */
void rpi_init(void) {
  DDRC |= (1 << RPI);     // set pin as output
}

void rpi_camera(int on) {
  if (on)
    PORTC |= (1 << RPI);  // RPI will check every 3 sec. to see if
  else                    // the pin is still high, then continue to 
    PORTC &= ~(1 << RPI); // record for additional 3 sec. if pin high
}