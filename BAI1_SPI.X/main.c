/*
 * File:   main.c
 * Author: NGTHANHHUNG
 * Target: PIC16F887
 * Clock: 4MHz Internal
 * UART: 9600 Baud
 * SPI: Master Mode, TC72 Sensor
 */

#include <xc.h>
#include <stdio.h>
#include <stdint.h>

// --- CONFIGURATION BITS ---
#pragma config FOSC = INTRC_NOCLKOUT 
#pragma config WDTE = OFF            // T?t Watchdog Timer
#pragma config PWRTE = ON            // B?t Power-up Timer
#pragma config MCLRE = ON            // B?t chân Reset (MCLR)
#pragma config CP = OFF              // T?t b?o v? code
#pragma config BOREN = ON            // B?t Brown-out Reset
#pragma config LVP = OFF             // T?t n?p ?i?n áp th?p

#define _XTAL_FREQ 4000000

// --- PIN DEFINITIONS ---
#define TC72_CE      PORTAbits.RA1
#define TC72_CE_TRIS TRISAbits.TRISA1

// --- FUNCTION PROTOTYPES ---
void System_Init(void);
void UART_Init(uint32_t baudrate);
void UART_Write_Text(const char* text);
void SPI_Init(void);
uint8_t SPI_Transfer(uint8_t data);
void TC72_Init(void);
int8_t TC72_ReadTemp(void);

// --- MAIN PROGRAM ---
void main(void) {
    System_Init();      // Kh?i t?o h? th?ng (GPIO, OSC)
    UART_Init(9600);    // Kh?i t?o UART 9600
    SPI_Init();         // Kh?i t?o SPI
    
    UART_Write_Text("\r\n==============================");
    UART_Write_Text("\r\n  PIC16F887 TC72 MONITOR  ");
    UART_Write_Text("\r\n==============================\r\n");

    TC72_Init();        // C?u hình ch? ?? cho TC72

    char buffer[32];
    
    while(1) {
        int8_t temp = TC72_ReadTemp();
        
        // ??nh d?ng và g?i d? li?u lên Virtual Terminal
        sprintf(buffer, "Nhiet do: %d C\r\n", temp);
        UART_Write_Text(buffer);
        
        __delay_ms(1000); // ??c m?i giây m?t l?n
    }
}

// --- MODULE FUNCTIONS ---

void System_Init(void) {
    OSCCONbits.IRCF = 0b110; // C?u hình xung n?i 4MHz
    
    ANSEL = 0x00;           
    ANSELH = 0x00;          
    
    TRISA = 0x00;           // ??t PORTA là Output
    PORTA = 0x00;           // Xóa PORTA v? 0
    
    TC72_CE_TRIS = 0;       // ??m b?o chân CE là Output
    TC72_CE = 0;            // Disable TC72 khi m?i b?t ??u
}

void UART_Init(uint32_t baudrate) {
    TRISCbits.TRISC6 = 0;   // TX Pin Output
    TRISCbits.TRISC7 = 1;   // RX Pin Input
    
    TXSTAbits.BRGH = 1;     // High Speed
    BAUDCTLbits.BRG16 = 0;  // 8-bit Baud Rate Generator
    SPBRG = 25;             // 9600 baud @ 4MHz (Sai s? ~0.16%)
    
    TXSTAbits.SYNC = 0;     
    RCSTAbits.SPEN = 1;    
    TXSTAbits.TXEN = 1;     
}

void UART_Write_Text(const char* text) {
    while(*text) {
        while(!TXSTAbits.TRMT); 
        TXREG = *text++;
    }
}

void SPI_Init(void) {
    TRISCbits.TRISC3 = 0;   // SCK Output
    TRISCbits.TRISC4 = 1;   // SDI Input
    TRISCbits.TRISC5 = 0;   // SDO Output
    
    SSPSTAT = 0x00;         // SMP=0, CKE=0
    SSPCON = 0x21;          // SSPEN=1, Master mode, Clock = Fosc/16 (250kHz cho ?n ??nh)
}

uint8_t SPI_Transfer(uint8_t data) {
    SSPBUF = data;          // B?t ??u g?i
    while(!SSPSTATbits.BF); // Ch? nh?n xong
    return SSPBUF;          // Tr? v? d? li?u nh?n ???c
}

void TC72_Init(void) {
    TC72_CE = 1;            // Enable
    __delay_us(50);
    
    SPI_Transfer(0x80);     // Tr? ??n Control Register (Write Mode)
    SPI_Transfer(0x00);     // Ghi 0x00 ?? ch?n Continuous Conversion Mode
    
    __delay_us(50);
    TC72_CE = 0;            // T?t Chip Enable
    __delay_ms(150);        // Ch? c?m bi?n th?c hi?n l?n ?o ??u tiên
}

int8_t TC72_ReadTemp(void) {
    int8_t temp_val;
    
    TC72_CE = 1;            // Enable
    __delay_us(10);
    
    SPI_Transfer(0x02);     // G?i ??a ch? thanh ghi MSB nhi?t ?? (Read Mode)
    temp_val = SPI_Transfer(0x00); // Nh?n byte d? li?u nhi?t ??
    
    __delay_us(10);
    TC72_CE = 0;            //UNEnable
    
    return temp_val;
}