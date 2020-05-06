

volatile int motion_sensor_flag = 0;	// flag for checking if RFID is triggered
	
volatile int rfid_flag = 0;				// flag for checking if the motion sensor is triggered

volatile int wifi_flag = 1;


void init(){

	i2c_init();

	spi_init();

	button_init(SUCCESSFUL_BUTTON);

	button_init(UNSUCCESSFUL_BUTTON);

	motion_sensor_init();

	buzzer_init();

	lock_init();

	rpi_init();

	hx711_init();

	lcd_init();

	servo_init();

	rfid_init();

	// wifi_init();

}

int main(){

	int ms_count = 0;	// motion sensor count

	int door_count = 0;	// door count

	int ts_flag = 0;	// telescopic flag

	init();				// initialize all modules


	while(1){			// main loop


		if(motion_sensor_flag){

			motion_sensor_flag = 0;

			// send signal to pi
			rpi_camera(1);

			// set count back to zero
			ms_count = 0;

		}

		if(ms_count > 1000  && !motion_sensor_flag){

			rpi_camera(0);

			ms_count = 0;

		}

		if(rfid_flag){

			// check rfid

			if(rfid_read() == USER_RFID){ 		// user

				// unlocks
				lock_toggle(1);

				// wait 30 secs	
				_delay_ms(30000);			

			}
			else if(rfid_read() == POSTMAN_RFID){	// postman

				// unlocks
				lock_toggle(1);

				lcd_clear_screen();


				lcd_moveto(0,0);


				lcd_stringout("Successful Delivery?");

				while(!(button_pressed(SUCCESSFUL_BUTTON) || button_pressed(UNSUCCESSFUL_BUTTON))){

				}

				if(button_pressed(SUCCESSFUL_BUTTON)){

					lcd_clear_screen();

					lcd_moveto(0,0);

					lcd_stringout("Thank you!");

					_delay_ms(2000);

					lcd_clear_screen();

				}
				else if(button_pressed(UNSUCCESSFUL_BUTTON)){

					if(!ts_flag){

						lcd_clear_screen();

						lcd_moveto(0,0);

						lcd_stringout("Creating Space");

						servo_rotate_to(90);

						ts_flag = 1;

					}
					else{

						lcd_clear_screen();

						lcd_moveto(0,0);

						lcd_stringout("No Extra Space");
					}

				}

				// wait 30 secs	
				_delay_ms(30000);					
				

			}
			else{		// thief

				buzzer_on();

				rpi_camera(1);

				_delay_ms(1000);

				buzzer_off();

				rpi_camera(0);

			}

			// check if door is left open
			if(lock_check()!= 1){

				//turn buzzer on
				buzzer_on();


			}
			else{

				buzzer_off();

			}

		}

		if(door_count > 5000 && !lock_check()){	// if 5 mins has passed

			// alert user

		}	
		else {

			buzzer_off();
		}	

		if(wifi_flag){


		}

		ms_count++;

		door_count++;


	}// end main while

	return 0;
}

ISR(PCINT2_vect)
	{
	    // ISR code

	    // check which module


		if(motion_detected()){		// motion sensor interrupt is triggered

			motion_sensor_flag = 1;

		}

		if(PIND & RFID == 1){		// RFID interrupt is triggered


			rfid_flag = 1;

		}




	}

ISR(INT0_vect)
	{
	    // ISR code 

	    // wifi module

	    wifi_flag = 1;

	}

