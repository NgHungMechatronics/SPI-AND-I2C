#ifndef LCD_I2C_H
#define	LCD_I2C_H

#include <xc.h>
#include <stdint.h>

#define _XTAL_FREQ 4000000
#define LCD_ADDR 0x7C 

// Khai báo các hàm I2C
void I2C_Master_Init(void);
void I2C_Wait(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_Write_Byte(uint8_t data);

// Khai báo các hàm LCD
void LCD_Init(void);
void LCD_Write(uint8_t mode, uint8_t value);
void LCD_SetCursor(uint8_t row, uint8_t col);
void LCD_Print(const char *str);
void LCD_Clear(void);

#endif