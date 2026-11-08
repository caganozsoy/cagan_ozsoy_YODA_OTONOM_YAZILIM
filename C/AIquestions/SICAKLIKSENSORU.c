#include <stdio.h>
#include <stdlib.h>
#include <pic18f45k22.h>
#include <htc.h>
#include "LCD.H"

#define _XTAL_FREQ 8000000
#pragma config FOSC = HSHP
#pragma config WDTEN = OFF

unsigned int value = 0;            // ADC sonucu (0-1023)
unsigned char dur = 0;             // 0 = calisiyor, 1 = acil stop
unsigned char conversion_done = 0; // ISR -> main bayragi
unsigned char line[17] = " ";

void main()
{

    TRISA = 0x02;
    ANSELA = 0x02;
    PORTA = 0x00;

    TRISB = 0xC1;
    ANSELB = 0x00;
    PORTB = 0x00;

    TRISCbits.TRISC2 = 0;
    ANSELC = 0x00;
    PORTC = 0x00;

    TRISD = 0x00;
    ANSELD = 0x00;
    PORTD = 0x00;

    TRISE = 0x00;
    ANSELE = 0x00;
    PORTE = 0x00;

    ADCON2 = 0xA1;
    ADCON1 = 0x00;
    ADCON0bits.CHS = 1;
    ADCON0bits.ADON = 1;

    INT0IE = 1;
    INT0IP = 1;
    PEIE = 1;
    GIE = 1;
    PEIEbits.ADIE = 1;
    IPRbits.ADIP = 1;

    PR2 = 199;
    CCP1CON = 0x0C;
    T2CON = 0x05;

    lcd_init();
    lcd_clear();
    lcd_goto(0);

    while (1)
    {
        if (conversion_done)
        {
            conversion_done = 0;
            if (dur == 1)
            {
                lcd_puts(" ACIL STOP ");
                lcd_goto(0x40);
                sprintf(line, "SCKLK: %4d ", value);
                lcd_puts(line);
            }
            else
            {
                lcd_puts(" KULUCKA ");
                lcd_goto(0x40);
                sprintf(line, "SCKLK: %4d ", value);
                lcd_puts(line);
            }
        }
    }
}

void_interrupt(high_interrupt) isr()
{
    if (ADIF)
    {

        value = ADRESH * 256 + ADRESL;
        if (dur == 1)
        {
            CCPR1L = 0x00;
            CCP1CONbits.DC1B = 00;
        }

        if (value >= 0 && value < 340)
        {
            CCPR1L = value >> 2;
            CCP1CON = (CCP1CON & 0xCF) + ((CCP1CON & 0x0003) << 4);
        }
        else if (value < 680)
        {
            CCPR1L = value >> 2;
            CCP1CON = (CCP1CON & 0xCF) + ((CCP1CON & 0x0003) << 4);
        }
        else
        {
            CCPR1L = value >> 2;
            CCP1CON = (CCP1CON & 0xCF) + ((CCP1CON & 0x0003) << 4);
        }
        conversion_done = 1;
        INTCON2bits.GODONE = 1;
    }
    if (INT0IF)
    {
        dur = 1 - dur;
    }
}