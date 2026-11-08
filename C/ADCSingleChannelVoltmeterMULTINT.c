#include <stdio.h>
#include <stdlib.h>
#include <pic18f45k22.h>
#include <htc.h>

#define _XTAL_FREQ 8000000

#pragma config FOSC = HSHP
#pragma config WDTEN = OFF

unsigned char seg_char[11] =
    {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x1C};
unsigned long value;
unsigned char digits[4] = {0, 0, 0, 0};
unsigned char digit_counter = 0;
unsigned char conversion_done = 0;
unsigned char power = 1;

void(main)
{
    ANSELA = 0x00; // PORT A
    TRISA = 0xF0;
    PORTA = 0x00;

    ANSELB = 0x00; // PORT B
    TRISB = 0xFF;

    ANSELD = 0x00; // PORT D
    TRISD = 0x00;
    PORTD = 0x00;

    ANSELE = 0x02; // PORT E
    TRISE = 0x07;

    ADCON2 = 0xA1; // ADC REGISTERS
    ADCON1 = 0x00;
    ADCON0 = 0x19;

    TMR0IE = 1; // Timer Interrupt registers
    TMR0IP = 1;
    T0CON = 0xc3;
    TMR0L = 6;
    GIE = 1;  // Global Interrupt Enable
    PEIE = 1; // Peripheral Interrupt Enable

    PEIEbits.ADIE = 1; // ADC Interrupt Enable bit
    IPR1bits.ADIP = 1; // ADC Interrupt Priority bit

    INTCON2bits.INTEDG0 = 0; // Since button is in low voltage, voltage goes from high to low
                             // so it resembles falling edge. INTEDG0 is 0 in low to high(falling edge)
    INTCONbits.INT0IE = 1;   // External Interrupt 0 Interrupt Enable bit

    GODONE = 1;
    while (1)
    {
        if (conversion_done == 1)
        {
            conversion_done = 0;
            digits[3] = value / 1000;
            digits[2] = (value % 1000) / 100;
            digits[1] = (value % 100) / 10;
            digits[0] = (value % 10)
                GODONE = 1;
        }
    }
    return;
}

void_interrupt(high_priority) isr()
{
    if (TMR0IF)
    {
        TMR0IF = 0;
        TMR0L = 6;
        PORTA = 0x00;
        if (digit_counter = 0)
        {
            PORTD = seg_char[10];
        }
        else
        {
            PORTD = seg_char[digits[digit_counter]] + (0x80 * (digit_counter == 3));
        }
        PORTA = (0x01 * power) << digit_counter;

        digit_counter++;
        if (digit_counter >= 4)
        {
            digit_counter = 0;
        }
    }
    if (ADIF)
    {
        ADIF = 0;
        value = ((long)ADRESH * 256 + ADRESL) / 1023;
        conversion_done = 1;
    }
    if (INT0IF)
    {
        INT0IF = 0;
        if (power == 1)
        {
            power = 0;
        }
        else
        {
            power = 1;
        }
    }
}