#include <stdio.h>
#include <stdlib.h>
#include <pic18f45k22.h>
#include <htc.h>
#include <LCD4.h>

#define _XTAL_FREQ 8000000

#pragma config FOSC = HSHP
#pragma config WDTEN = OFF

unsigned long value1;
unsigned long PR2;
int freq;
unsigned long dutycycleres;
unsigned long dutycycle;
unsigned long dutycycle2;
unsigned long count;
unsigned char power1 = 1;
unsigned char power2 = 1;

unsigned char firstline[17];
unsigned char secondline[17];

void main()
{
    TRISA = 0xFF;
    ANSELA = 0x00;
    PORTA = 0x00;

    TRISB = 0xFF;
    ANSELB = 0x00;
    PORTB = 0x00;

    TRISC = 0xF6;
    ANSELC = 0x00;
    PORTC = 0x00;

    TRISD = 0x80;
    ANSELD = 0x00;
    PORTD = 0x00;

    TRISE = 0x01;
    ANSELE = 0x01;
    PORTE = 0x00;

    T0CON = 0x83;
    TMR0IE = 1;
    TMR0IP = 1;

    TMR0H = 3036 / 256;
    TMR0L = 3036 % 256;

    T2CON = 0x04; // Timer2 is on and prescaler is 1
    PR2 = 0;      // Initially period is 0 you know

    GIE = 1;
    INT0IE = 1;
    INT1IE = 1;

    ADCON2 = 0xA1;
    ADCON1 = 0x00;

    INTCON2bits.RBPU = 0; // INTCON2 içinden PORTB'de bulunan bütün pull up resistorları açıyoruz
                          // RBPU = 0; diye belirtsek bile okey olur aslında
    WPUB = 0x03;          // Sadece Port Bdeki RB0 ve RB1 pinlerdeki pull upları aç

    ADCON0bits.CHS = 5;
    ADCON0bits.ADON = 1;
    GODONE = 1;

    CCP1CON = 0x0C;
    CCP2CON = 0X0C;

    lcd_init();
    lcd_clear();
    lcd_goto(0);

    while (1)
    {
        if (!GODONE)
        {
            value1 = ((ADRESH * 256) + ADRESL);

            GODONE = 1;
        }

        dutycycleres = 4 * (PR2 + 1);

        dutycycle = (value1 * dutycycleres) / 1023;
        dutycycle2 = dutycycleres - dutycycle;

        CCPR1L = dutycycle >> 2;
        CCP1CON = (CCP1CON & 0xCF) + ((dutycycle & 0x0003) << 4);

        CCPR2L = dutycycle2 >> 2;
        CCP2CON = (CCP2CON & 0xCF) + ((dutycycle2 & 0x0003) << 4);

        dutycycle = (100 * dutycycle) / dutycycleres;
        dutycycle2 = (100 * dutycycle2) / dutycycleres;

        sprintf(firstline, "PWM Freq:%4dHz", freq);
        lcd_puts(firstline);
        lcd_goto(0x40); // 2nd line
        sprintf(secondline, "CCP1%3d CCP2%3d", dutycycle, dutycycle2);
        lcd_puts(secondline);
    }
}

void_interrupt(high_priority) isr()
{
    if (TMR0IF)
    {
        TMR0IF = 0;
        TMR0H = 3036 / 256;
        TMR0L = 3036 % 256;

        count++;

        if (count >= 7)
        {
            CCPR1L = 0x00;
            CCP1CONbits.DC1B = 0;
            CCPR2L = 0x00;
            CCP2CONbits.DC1B = 0;
        }
    }
    if (INT0IF)
    {
        INT0IF = 0;
        PR2 = 203;
        freq = 610;
        T2CON = 0x06;
        TRISCbits.TRISC1 = 0;
        TRISCbits.TRISC2 = 0;
        count = 0;
    }
    if (INT1IF)
    {
        INT1IF = 0;
        PR2 = 204;
        freq = 2450;
        T2CON = 0x05;
        TRISCbits.TRISC1 = 0;
        TRISCbits.TRISC2 = 0;
        count = 0;
    }
}