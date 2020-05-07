#ifndef MISC_H
#define MISC_H

void button_init(int pin);
int  button_pressed(int pin);

void led_red_init(void);
void led_red(int on);

void motion_sensor_init(void);
int  motion_detected(void);

void lock_init(void);
void lock_unlock(int on);
int  lock_locked(void);

void buzzer_init(void);
void buzzer_on(void);
void buzzer_off(void);

void rpi_init(void);
void rpi_camera(int on);

#endif // MISC_H