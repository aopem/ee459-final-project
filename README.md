# USC EE 459 Final Project - Spring 2020

#### Title: Google Parcel

#### Due Date: 05/07/20

#### Members: Andrew Opem, David Karapetyan, Natan Benchimol


### Project Outline

For our final project we designed the electrical hardware and software of a smart mailbox. 
The hardware consists of sensors, controllers, and microelectronics likes resistors, capacitors, etc. 
This repository contains the software that is intended to drive the hardware. 
It utilizes the interrupt pins and a main while loop to detect when certain sensors are triggered.



### Program Microcontroller
1. Change `OBJECTS` line in `Makefile` to correct `.o` file.
    * ex. `tests/lab3_test.o`
2. `make flash`



### Adapted Modules:

For each module in our program that adapted code snippets from online sources, the links for each source are listed below.
The full disclaimers for each snippet is at the top of each `.c` or `.cpp` file.

* [I2C](http://ee-classes.usc.edu/ee459/library/plaintext.php?file=samples/AVR/at328-7.c)

* [SPI](https://github.com/asif-mahmud/MIFARE-RFID-with-AVR/blob/master/lib/avr-rfid-library/lib/spi.c)

* [LCD](http://ee-classes.usc.edu/ee459/library/plaintext.php?file=samples/AVR/at328-7.c)

* [HX711](https://github.com/getsiddd/HX711)

* [CC3000](https://github.com/asif-mahmud/MIFARE-RFID-with-AVR/blob/master/lib/avr-rfid-library/lib/mfrc522.c)

* [RFID](https://github.com/asif-mahmud/MIFARE-RFID-with-AVR/blob/master/lib/avr-rfid-library/lib/mfrc522.c)

* [SERVO](https://www.electronicwings.com/avr-atmega/servo-motor-interfacing-with-atmega16)


### Additional Notes:

* General:

	Since we did not have access to the hardware in our design, we were unable to test our code on any of the hardware components.

* CC3000 WiFi Module:

	The code for this module was adapted from an arduino library and has some snippets in C++.
	We did not convert all of the code to C, so the final project will not compile properly. 
	We mentioned this to professor Weber and he said that he understands that given enough time we would be able to convert the code to C so he is not too worried about that. 

* RFID Module:

	When checking for an RFID match, we chose generic test values to compare with. In reality, each module that is shipped to a customer will have to have its own unique RFID associated with the hardware. We did not generate an algorithm or function to distribute these as this is just a protoype. 