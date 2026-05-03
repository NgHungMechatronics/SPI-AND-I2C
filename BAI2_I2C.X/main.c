/*
 * File:   main.c
 * Author: NGTHANHHUNG
 * Target: PIC16F887
 */
#include <xc.h>
#include <stdint.h>
#include "LCD_I2C.h"

// ==========================================
// CONFIGURATION BITS
// ==========================================
#pragma config FOSC = INTRC_NOCLKOUT 
#pragma config WDTE = OFF           
#pragma config PWRTE = ON            
#pragma config MCLRE = ON            
#pragma config CP = OFF             
#pragma config CPD = OFF             
#pragma config BOREN = ON           
#pragma config IESO = OFF          
#pragma config FCMEN = OFF        
#pragma config LVP = OFF           

void main(void) {
   
    OSCCON = 0x61;      
    ANSEL = 0;         
    ANSELH = 0;        
    
   
    I2C_Master_Init();
    LCD_Init();
    
    // Hi?n th? n?i dung
    LCD_SetCursor(1, 6);
    LCD_Print("SORRY :((");
    
    LCD_SetCursor(2, 3);
    LCD_Print("MECHATRONIC");

    while (1) {
        // Ch??ng trình chính l?p ? ?ây
    }
}