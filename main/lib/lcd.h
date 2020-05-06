#ifndef LCD_H
#define LCD_H

void lcd_init(void);
void lcd_moveto(unsigned char row, unsigned char col);
void lcd_stringout(char* str);

#endif // LCD_H