#include <avr/io.h>
#include <util/delay.h>
#include <lib/headers.h>

volatile int motion_sensor_flag = 0;	// flag for checking if RFID is triggered
volatile int rfid_flag = 0;						// flag for checking if the motion sensor is triggered
volatile int wifi_flag = 1;

#define USER_RFID 0xAA
#define POST_RFID 0xBB

void init(void);

int main(){
	int ms_count = 0;		// motion sensor count
	int door_count = 0;	// door count
	int ts_flag = 0;		// telescopic flag

	uint8_t str[16];		// for storing RFID data
	uint8_t tag_id = 0; // for reading RFID tag data

	init();							// initialize all modules
	sei();							// global interrupt enable

	// this is a c++ library!! (for wifi module)
	Adafruit_CC3000_Client client(); 	/* C++ CODE */
	client.connect();									/* C++ CODE */

	int package_inside = 0  // use to see if there is a package inside

	// used with weight sensor
	double tare_point_128     = HX711_get_offset();
	double current_weight_128 = HX711_read_average(10) - tare_point_128;

	while(1) {					// main loop

		if (motion_sensor_flag) {
			motion_sensor_flag = 0;

			// send signal to pi to turn on camera
			rpi_camera(1);

			// set count back to zero
			ms_count = 0;

			// prepare user notification
			client.write("Motion detected at mailbox"); /* C++ CODE */
		}

		if (ms_count > 1000  && !motion_sensor_flag) {

			// turn off camera
			rpi_camera(0);
			ms_count = 0;
		}

		if (rfid_flag) {
			rfid_flag = 0;

			// read rfid tag
			tag_id = rfid_request(PICC_REQALL, str);

			if (tag_id == CARD_FOUND) {
			
				// check rfid
				if (tag_id == USER_RFID) { 			// user

					// unlocks
					lock_unlock(1);

					// wait 30 secs	
					_delay_ms(30000);			
				}
				else if (tag_id == POST_RFID) {	// postman

					// unlocks
					lock_unlock(1);
					lcd_clear_screen();
					lcd_moveto(0,0);
					lcd_stringout("Successful Delivery?");

					// wait for a button press
					while ( !(button_pressed(SUCCESSFUL_BUTTON) || button_pressed(UNSUCCESSFUL_BUTTON)) ) {}

					if (button_pressed(SUCCESSFUL_BUTTON)) {
						lcd_clear_screen();
						lcd_moveto(0,0);
						lcd_stringout("Thank you!");
						_delay_ms(2000);
						lcd_clear_screen();

						// prepare user notification
						client.write("Package delivered successfully"); /* C++ CODE */
					}
					else if (button_pressed(UNSUCCESSFUL_BUTTON)) {

						// check if already expanded or not
						if (!ts_flag) {
							lcd_clear_screen();
							lcd_moveto(0,0);
							lcd_stringout("Creating Space");
							servo_rotate_to(90);
							ts_flag = 1;

							// prepare user notification
							client.write("Package delivered successfully"); /* C++ CODE */
						}
						else {
							lcd_clear_screen();
							lcd_moveto(0,0);
							lcd_stringout("No Extra Space");

							// prepare user notification
							client.write("Package delivery unsuccessful"); /* C++ CODE */
						}
					}

					// wait 30 secs	
					_delay_ms(30000);					
					}
					else {	// thief
						// prepare user notification
						client.write("SECURITY ALERT: Unauthorized attempt to unlock"); /* C++ CODE */

						buzzer_on();
						rpi_camera(1);
						_delay_ms(1000);
						buzzer_off();
						rpi_camera(0);
					}
				}
				else {	// no RFID tag found or error
					lcd_clear_screen();
					lcd_moveto(0,0);
					lcd_stringout("No Card Found");
				}

			// check if door is left open
			if (!lock_locked()) {
				buzzer_on();
			}
			else {
				buzzer_off();
			}
		} // rfid flag if statement

		if (door_count > 5000 && !lock_locked()) {	// if 5 mins has passed
			
			// alert user 
			client.write("SECURITY ALERT: Door left open for at least 5 minutes"); /* C++ CODE */
		}
		else {
			buzzer_off();
		}

		// check if current weight shows there is a package present
		if (current_weight_128 > 1)
			package_inside = 1;
		else
			package_inside = 0

		// update weight sensor, counts
		current_weight_128 = HX711_read_average(10) - tare_point_128;

		ms_count++;
		door_count++;

	}	// end main while

	return 0;
}


void init() {
	i2c_init(BDIV);
	spi_init();
	button_init(SUCCESSFUL_BUTTON);
	button_init(UNSUCCESSFUL_BUTTON);
	motion_sensor_init();
	buzzer_init();
	lock_init();
	rpi_init();
	lcd_init();
	servo_init();
	rfid_init();
	HX711_init(128);
}


ISR(PCINT2_vect) {
  // check which module
	if (motion_detected()) {		// motion sensor interrupt is triggered
		motion_sensor_flag = 1;
	}

	if (PIND & RFID == 1) {			// RFID interrupt is triggered
		rfid_flag = 1;
	}
}

