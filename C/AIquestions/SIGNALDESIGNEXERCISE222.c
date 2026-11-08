#include <stdio.h>
#include <xc.h>
#include "LCD.H"

#define _XTAL_FREQ 8000000
#pragma config FOSC = HSHP
#pragma config WDTEN = OFF

unsigned char line1[17] = " ";
unsigned char line2[17] = " ";

float duty_cycle = 0, prev_duty = 0;
int counter = 0, frequency = 0, w1 = 0, w2 = 0, is_pwm1 = 0, is_pwm2 = 0;

int main()
{
    TRISA = 0x00;
    ANSELA = 0x00;
    PORTA = 0x00;

    TRISB = 0x02;
    ANSELB = 0x00;
    PORTB = 0x00;

    TRISCbits.TRISC1 = 1;
    TRISCbits.TRISC2 = 1;
    ANSELC = 0x00;

    TRISD = 0x00;
    ANSELD = 0x00;
    PORTD = 0x00;

    TRISE = 0x01;
    ANSELE = 0x01;
    PORTE = 0x00;

    INTCON2bits.RBPU = 0; // dahili dirençleri aktif etme
    WPUB = 0x03;          // PORT B deki pull upları açıyoz

    T0CON = 0x07;
    TMR0L = 0;
    TMR0IE = 1;
    TMR0IP = 1;

    INT0IE = 1;
    INT1IE = 1;
    INT0IF = 0;
    INT1IF = 0;

    INTCON2bits.INTEDG0 = 0;
    INTCON2bits.INTEDG1 = 0;

    ADCON2 = 0xA1;
    ADCON1 = 0x00;
    ADCON0bits.CHS = 5;
    ADCON0bits.ADON = 1;
    GODONE = 1;

    CCP1CON = 0x0C;
    CCPR1L = 0x00;
    CCP2CON = 0x0C;
    CCPR2L = 0x00;
    PR2 = 0;
    T2CON = 0x04;

    PEIE = 1;
    GIE = 1;

    lcd_init();
    lcd_clear();
    lcd_goto(0);

    while (1)
    {
        if (!GODONE)
        {
            duty_cycle = ((ADRESH * 256) + ADRESL) / 1023;
            w1 = duty_cycle * 4.0 * (PR2 + 1);
            w2 = (1.0 - duty_cycle) * 4.0 * (PR2 + 1);
            GODONE = 1;
        }
        if (counter >= 7)
        {
            CCPR1L = 0x00;
            CCP1CONbits.DC1B = 0;

            CCPR2L = 0x00;
            CCP1CONbits.DC1B = 0;
        }
        else
        {
            CCPR1L = w1 >> 2;
            CCP1CON = (CCP1CON & 0xCF) + ((w1 & 0x0003) << 4);
            CCPR2L = w2 >> 2;
            CCP2CON = (CCP2CON & 0xCF) + ((w2 & 0x0003) << 4);
        }

        w1 = 100 * duty_cycle;
        w2 = (1 - duty_cycle) * 100;

        sprintf(line1, "FREQ: %4d", frequency);
        sprintf(line2, "CCP1:%3d,CCP2:%3d", w1, w2);

        lcd_goto(0);
        lcd_puts(line1);
        lcd_goto(0x40);
        lcd_puts(line2);

        PORTEbits.RE1 = is_pwm1;
        PORTEbits.RE2 = is_pwm2;

        if (prev_duty != duty_cycle)
        {
            counter = 0;
            prev_duty = duty_cycle;
        }
    }
}

void_interrupt(low_priority) isr_low()
{
    if (TMR0IF)
    {
        TMR0IF = 0;
        TMR0L = 0;

        counter++;
    }
}

void_interrupt(high_priority) isr_high()
{
    if (INT0IF)
    {
        INT0IF = 0;
        is_pwm1 = 1;
        is_pwm2 = 0;
        frequency = 610;
        T2CON = 0x06;
        PR2 = 204;
        TRISCbits.TRISC1 = 0;
        TRISCbits.TRISC2 = 0;
    }
    if (INT1IF)
    {
        INT1IF = 0;
        is_pwm1 = 0;
        is_pwm2 = 1;
        frequency = 2450;
        T2CON = 0x05;
        PR2 = 203;
        TRISCbits.TRISC1 = 0;
        TRISCbits.TRISC2 = 0;
    }
}