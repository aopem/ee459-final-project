#include <lcd.h>
#include <i2c.h>
#include <avr/io.h>
#include <util/delay.h>
#include <const.h>

#define LCDAddressI2C 0x50
#define Prefix        0xFE
#define ClearScreen   0x51
#define MoveCursor    0x47  // follow this with 2 hex digits for row then col pos
#define DisplayOn     0x41

/*
  lcd_init - Initialize the LCD
*/
void lcd_init() {
  i2c_init(BDIV);

  _delay_ms(250);               // Wait 500msec for the LCD to start up
  _delay_ms(250);

  uint8_t cmd[6];               // 6 bytes to hold 2 byte prefix (0xFE)                     
                                // last 4 bytes for actual commands 

  cmd[0] = Prefix >> 8;         // Separate into 2 bytes, where MSB first
  cmd[1] = Prefix & 0xFF;       // and LSB second   
  cmd[2] = ClearScreen >> 8;    // repeat for clear screen command
  cmd[3] = ClearScreen & 0xFF;
  cmd[4] = DisplayOn >> 8;      // repeat for display on command
  cmd[5] = DisplayOn & 0xFF;

  i2c_io(LCDAddressI2C, NULL, 0, cmd, 6, NULL, 0);           
}

/*
  moveto - Move the cursor to the row and column given by the arguments.
  Row is 0 or 1, column is 0 - 15.
*/
void lcd_moveto(unsigned char row, unsigned char col) {
  uint8_t cmd[6];               // 6 bytes to hold 2 byte prefix (0xFE)                     
                                // 2 bytes to hold command
                                // 2 bytes to hold row, col

  cmd[0] = Prefix >> 8;         // Separate into 2 bytes, where MSB first
  cmd[1] = Prefix & 0xFF;       // and LSB second   
  cmd[2] = MoveCursor >> 8;     // repeat for move cursor command
  cmd[3] = MoveCursor & 0xFF;
  cmd[4] = row + 1 & 0xFF;      // repeat for row, col 
  cmd[5] = col + 1 & 0xFF;

  i2c_io(LCDAddressI2C, NULL, 0, cmd, 6, NULL, 0);
}


/*
  lcd_stringout - Print the contents of the character string "str"
  at the current cursor position.
*/
void lcd_stringout(char* str, unsigned char len) {
  int i = 0;

  while (i < len) {
    i2c_io(LCDAddressI2C, NULL, 0, str[i], len, NULL, 0);
    i++;
  }
}

void lcd_clear_screen(void) {
  uint8_t cmd[4];               // 4 bytes to hold 2 byte prefix (0xFE)                     
                                // last 2 bytes for actual command 

  cmd[0] = Prefix >> 8;         // Separate into 2 bytes, where MSB first
  cmd[1] = Prefix & 0xFF;       // and LSB second   
  cmd[2] = ClearScreen >> 8;    // repeat for clear screen command
  cmd[3] = ClearScreen & 0xFF;

  i2c_io(LCDAddressI2C, NULL, 0, cmd, 4, NULL, 0); 
}

