#include <xc.h>
#include <stdio.h>

#define _XTAL_FREQ 8000000

// Sistem Durum Değişkenleri
unsigned char system_state = 1;
unsigned int adc_raw = 0;
unsigned int temp = 0;
unsigned char display_digits[4] = {0, 0, 0, 0};
int digit_counter = 0;
unsigned char active_anode = 0;
unsigned int dutycycle;

// 7-Segment Katot Karakter Seti (Ortak Anot yapıya göre ayarlı)
// 0-9 arası sayılar, 10: Derece sembolü (°), 11: 'E' harfi, 12: Tire (-)
const unsigned char seg_char[] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F,
    0x63, // 10: Derece Sembolü (0x63)
    0x79, // 11: E harfi (0x79)
    0x40  // 12: Tire simgesi (0x40)
};

void main()
{

    TRISA = 0xF0;
    ANSELA = 0x00;
    PORTA = 0x00;

    TRISB = 0xFF;
    ANSELB = 0x00;
    PORTB = 0x00;

    TRISC = 0xF9;
    ANSELC = 0x00;
    PORTC = 0x00;

    TRISD = 0x00;
    ANSELD = 0x00;
    PORTD = 0x00;

    TRISE = 0x02;
    ANSELE = 0x02;
    PORTE = 0x00;

    T0CON = 0xC3;
    TMR0L = 6;

    TMR0IE = 1;
    TMR0IP = 1;
    GIE = 1;

    T2CON = 0x04;
    INT0IE = 1;
    INT1IE = 1;
    INT0IP = 1;
    INT1IP = 1;

    ADCON2 = 0xA1;
    ADCON1 = 0x00;
    ADCON0bits.CHS = 6;
    ADCON0bits.ADON = 1;
    GODONE = 1;

    ADIE = 1;
    ADIP = 1;
    PEIE = 1;

    while (1)
    {
        if (system_state == 1)
        {

            display_digits[3] = temp / 100;
            display_digits[2] = (temp / 10) % 10;
            display_digits[1] = (temp % 10);
            display_digits[0] = 10;

            PR2 = 255;

            if (temp > 0 && temp < 150)
            {
                dutycycle = 25;
                CCPR1L = 0x40;
            }
            if (temp >= 150 && temp < 300)
            {
                dutycycle = 50;
                CCPR1L = 0x80;
            }
            else if (temp >= 300 && temp < 450)
            {
                dutycycle = 75;
                CCPR1L = 0xC0;
            }
            else
            {
                dutycycle = 100;
                system_state = 2;
                CCPR1L = 0xFF;
            }

            CCP1CON = (CCP1CON & 0xCF) + ((CCP1CON & 0x0003) << 4);
        }
        if (system_state == 2)
        {
            display_digits[3] = 11;
            display_digits[2] = 11;
            display_digits[1] = 11;
            display_digits[0] = 11;
            CCPR1L = 0xFF;
        }
        if (system_state == 0)
        {
            display_digits[3] = 12;
            display_digits[2] = 12;
            display_digits[1] = 12;
            display_digits[0] = 12;
            CCPR1L = 0x00;
        }
    }
}

void_interrupt(high_priority) isr()
{
    if (TMR0IF)
    {
        TMR0IF = 0;
        TMR0L = 6;

        PORTA = 0x00;
        if (digit_counter == 0)
        {
            PORTD = seg_char[10];
        }
        else
        {
            PORTD = seg_char[display_digits[digit_counter]];
        }
        PORTA = 0x01 << digit_counter;
        digit_counter++;

        if (digit_counter >= 4)
        {
            digit_counter = 0;
        }
    }

    if (ADIF)
    {
        ADIF = 0;
        if (system_state == 1)
        {
            adc_raw = ADRESH * 256 + ADRESL;
            temp = (500 * (adc_raw)) / 1023;
            ADCON0bits.GODONE = 1;
        }
    }

    if (INT0IF)
    {
        INT0IF = 0;
        system_state = 0;
        dutycycle = 0;
    }

    if (system_state == 0)
    {
        if (INT1IF)
        {
            INT1IF = 0;
            system_state = 1;
        }
    }
}