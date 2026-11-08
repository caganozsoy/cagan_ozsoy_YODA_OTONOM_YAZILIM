#include <stdio.h>
#include <stdlib.h>
#include <pic18f45k22.h>
#include <htc.h>

#define _XTAL_FREQ 8000000

#pragma config FOSC = HSHP
#pragma config WDTEN = OFF

unsigned char seg_char[10] =
    {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
unsigned long counter = 0;
unsigned int value = 0;
unsigned char digits[10] = {0, 0, 0, 0};
unsigned char digit_counter = 0;

unsigned char cont = 0;

void(main)
{
    ANSELA = 0x00;
    TRISA = 0xF0;
    PORTA = 0x00;

    ANSELB = 0x00;
    TRISB = 0xFF;

    ANSELD = 0x00;
    TRISD = 0x00;
    PORTD = 0x00;

    T0CON = 0xC3;
    TMROL = 6;
    RBPU = 1;
    TMROIE = 1;
    TMROIP = 1;

    INT1IE = 1;
    INT1IP = 1;

    GIE = 1;

    while (1)
    {
        value = counter / 50;
        digits[3] = value / 1000;
        digits[2] = (value % 1000) / 100;
        digits[1] = (value % 100) / 10;
        digits[0] = value % 10;
    }
    return;
}

void_interrupt(high_interrupt) isr()
{
    if (TMR0IF)
    {
        TMR0IF = 0;
        TMR0L = 6;
        PORTA = 0x00;
        PORTD = seg_char[digits[digit_counter]];

        PORTA = 0x01 << digit_counter;

        digit_counter++;
        if (digit_counter >= 4)
        {
            digit_counter = 0;
        }
        if (cont)
        {
            counter++
        }
        if (counter > 50 * 10000)
        {
            counter = 0;
        }
    }
    if (INT1IF)
    {
        INT1IF = 0;
        cont = 1 - cont;
    }
}
}