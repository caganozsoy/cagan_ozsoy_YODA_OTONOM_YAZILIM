#include <stdio.h>
#include <stdlib.h>
#include <pic18f45k22.h>
#include <htc.h>
#include "LCD.H"

#define _XTAL_FREQ 8000000
#pragma config FOSC = HSHP
#pragma config WDTEN = OFF

unsigned int value = 0;            // ADC sonucu (0-1023)
unsigned char gece = 0;            // 0 = otomatik, 1 = gece modu
unsigned char conversion_done = 0; // ISR -> main haber bayragi
unsigned char line[17] = " ";

void main()
{
    TRISA = 0x04;
    ANSELA = 0x04;
    PORTA = 0x00;

    TRISB = 0x01;
    ANSELB = 0x00;
    PORTB = 0x00;

    TRISC = 0x00;
    ANSELC = 0x00;
    PORTC = 0x00;

    TRISD = 0x00;
    ANSELD = 0x00;
    PORTD = 0x00;

    TRISE = 0x00;
    ANSELE = 0x00;
    PORTE = 0x00;

    INT0IE = 1;
    INT0IP = 1;
    INTCON2bits.INTEDG0 = 0;
    GIE = 1;
    PEIE = 1;

    ADCON2 = 0xA1;
    ADCON1 = 0x00;
    ADCON0bits.CHS = 2;
    ADCON0bits.ADON = 1;
    T2CON = 0x05;
    GODONE = 1;

    PEIEbits.ADIE = 1;
    IPRbits.ADIP = 1;

    CCP2CON = 0x0C;
    PR2 = 249;

    lcd_init();
    lcd_clear();
    lcd_goto(0);

    while (1)
    {
    }
}

void_interrupt(high_priority)
{
    if (ADIF)
    {
        ADIF = 0;
    }
}