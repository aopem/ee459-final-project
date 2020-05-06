#include <avr/io.h>
#include <util/delay.h>
#include <servo.h>

void servo_init(unsigned char servo_pin) {
  DDRD |= (1 << servo_pin);  // Make OC1A pin as output 
  TCNT1 = 0;                 // Set timer1 count zero
  ICR1 = 2499;               // Set TOP count for timer1 in ICR1 register

  // Set Fast PWM, TOP in ICR1, Clear OC1A on compare match, clk/64
  TCCR1A = (1<<WGM11)|(1<<COM1A1);
  TCCR1B = (1<<WGM12)|(1<<WGM13)|(1<<CS10)|(1<<CS11);
}

/* servo_rotate_to - rotates servo to angle between -90 to +90 degrees
 * OC1RA register values correspond to servo angle between -90 to +90 degrees
 * OC1RA = 65  -> -90 degrees
 * OC1RA = 175 ->   0 degrees
 * OC1RA = 300 -> +90 degrees
 */
int servo_rotate_to(int angle) {
  if (angle >= -90 && angle <= 90) {
    OC1RA = angle + 175;  // add 175 offset to angle to get OC1RA value
    _delay_ms(1500);      // give servo time to rotate
    return 1;
  }
  return 0;
}