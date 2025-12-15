#ifndef LCD_I2C_H
#define LCD_I2C_H

#include "driver/i2c_master.h"

#define LCD_ADDR 0x27
#define LCD_COLS 16
#define LCD_ROWS 2

void lcd_init();
void lcd_clear();
void lcd_set_cursor(int col, int row);
void lcd_print(const char *str);

#endif
