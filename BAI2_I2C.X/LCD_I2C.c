#include "LCD_I2C.h"

// ---  I2C ---
void I2C_Master_Init(void) {
    TRISC3 = 1; 
    TRISC4 = 1;
    SSPCON = 0x28;      // Master mode, SSPEN = 1
    SSPSTAT = 0x80;
    SSPADD = 9;         // 100kHz v?i Fosc 4MHz
}

void I2C_Wait(void) {
    while (!PIR1bits.SSPIF);
    PIR1bits.SSPIF = 0;
}

void I2C_Start(void) {
    SSPCON2bits.SEN = 1;
    I2C_Wait();
}

void I2C_Stop(void) {
    SSPCON2bits.PEN = 1;
    I2C_Wait();
}

void I2C_Write_Byte(uint8_t data) {
    SSPBUF = data;
    I2C_Wait();
}

// --- LCD ---
void LCD_Write(uint8_t mode, uint8_t value) {
    I2C_Start();
    I2C_Write_Byte(LCD_ADDR);
    I2C_Write_Byte(mode);   // 0x00: L?nh, 0x40: D? li?u
    I2C_Write_Byte(value);
    I2C_Stop();
    __delay_ms(2);
}

void LCD_Init(void) {
    __delay_ms(50);
    LCD_Write(0x00, 0x38);  // 8-bit, 2 dòng
    LCD_Write(0x00, 0x0C);  // Hi?n màn hình, t?t con tr?
    LCD_Write(0x00, 0x06);  // T? t?ng con tr?
    LCD_Clear();
}

void LCD_Clear(void) {
    LCD_Write(0x00, 0x01);
    __delay_ms(5);
}

void LCD_SetCursor(uint8_t row, uint8_t col) {
    uint8_t cmd = (row == 1) ? (0x80 + col - 1) : (0xC0 + col - 1);
    LCD_Write(0x00, cmd);
}

void LCD_Print(const char *str) {
    while (*str) {
        LCD_Write(0x40, *str++);
    }
}