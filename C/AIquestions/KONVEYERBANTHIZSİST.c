#include <xc.h>
#include <stdio.h>

#define _XTAL_FREQ 8000000

// Sistem Durum Değişkenleri
unsigned char mode = 0;      // 0: Otomatik, 1: Manuel
unsigned char emergency = 0; // 0: Normal, 1: Acil Durum Kilitlenmesi
unsigned char an_cnt = 0;    // ADC Kanal seçici (0: AN0, 1: AN1)
int mode_changed = 0;

unsigned int an_list[] = {0, 1};
unsigned char adc_value1;
unsigned char adc_value2;
unsigned int value;
unsigned int weight = 0;      // 10 katı alınmış ağırlık (örn: 47.3 için 473)
unsigned int manual_duty = 0; // Manuel mod duty cycle değeri (0-255 arası)

unsigned char display_digits[4] = {0, 0, 0, 0};
unsigned char digit_counter = 0;

// Karakter Seti
// 0-9: Sayılar, 10: A(0x77), 11: U(0x1C), 12: T(0x0F), 13: O(0x3F), 14: M(0x37), 15: n(0x54)
const unsigned char seg_char[] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F,
    0x77, // 10: A
    0x1C, // 11: U
    0x0F, // 12: T
    0x3F, // 13: O (0 ile aynı)
    0x37, // 14: M
    0x54  // 15: n
};

void main()
{
    ANSELA = 0x00;
    TRISA = 0x02;
    PORTA = 0x00;

    ANSELB = 0x00;
    TRISB = 0xFF;
    PORTB = 0x00;

    ANSELC = 0x00;
    TRISC = 0xF9;
    PORTC = 0x00;

    ANSELD = 0x00;
    TRISD = 0x00;
    PORTD = 0x00;

    TOCON = 0xC3;
    TMR0L = 6;
    TMR0IE = 1;
    TMR0IP = 1;

    INT0IE = 1;
    INT2IE = 1;
    INT0IP = 1;
    INT2IP = 1;

    T2CON = 0x04;
    PR2 = 255;

    ADCON2 = 0xA1;
    ADCON1 = 0x00;
    ADCONbits.CHS = 0;
    ADCON0bits.ADON = 1;

    PEIEbits.ADIE = 1;
    IPRbits.ADIP = 1;
    GIE = 1;
    GODONE = 1;

    while (1)
    {
        if (mode == 0)
        {

            display_digits[3] = weight / 100;
            display_digits[2] = (weight % 100) / 10;
            display_digits[1] = weight % 10;
            display_digits[0] = 0;

            ADCON0bits.CHS = 0;
            value = 4 * (PR2 + 1);
            value = value--;

            if (weight >= 0 && weight <= 250)
            {
                value = value * 8 / 10;
            }
            if (weight > 250 && weight <= 500)
            {
                value = value * 6 / 10;
            }
            if (weight > 500 && weight <= 750)
            {
                value = value * 4 / 10;
            }
            if (weight > 750 && weight <= 1000)
            {
                value = value * 2 / 10;
            }

            CCPR1L = value >> 2;
            CCP1CON = (CCP1CON & 0xCF) + ((CCP1CON & 0x0003) << 4);

            CCPR2L = value >> 2;
            CCP2CON = (CCP2CON & 0xCF) + ((CCP2CON & 0x0003) << 4);
        }
        if (mode == 1)
        {
            ADCONbits.CHS = 1;
            manual_duty = adc_value2 / 10;
            if (manual_duty <= 100)
            {
                display_digits[3] = 0;
                display_digits[2] = manual_duty / 100;
                display_digits[1] = manual_duty / 10;
                display_digits[0] = manual_duty % 10;
            }
        }
        if (emergency == 1)
        {
            GIE = 0;
            CCPR1L = 0;
            CCPR2L = 0;
        }
        if (mode_changed == 1)
        {
            if (mode == 0)
            {
                display_digits[3] = 10;
                display_digits[2] = 11;
                display_digits[1] = 12;
                display_digits[0] = 13;
                _delay_ms(1000);
            }
            if (mode == 1)
            {
                display_digits[3] = 14;
                display_digits[2] = 10;
                display_digits[1] = 15;
                display_digits[0] = 11;
                _delay_ms(1000);
            }
            mode_changed = 0;
        }
    }
}

void_interrupt(high_priority)
{
    if (TMR0IF)
    {
        TMR0IF = 0;
        TMR0L = 6;
        if (emergency == 1)
        {
            PORTA = 0x00;
            PORTD = seg_char[0];
            PORTA = 0x01 << digit_counter;
        }
        else if (mode_changed == 1)
        {
            PORTA = 0x00;
            PORTD = seg_char[display_digits[digit_counter]];
            PORTA = 0x01 << digit_counter;
        }
        else if (mode == 1)
        {
            PORTA = 0x00;
            PORTD = seg_char[digit_digits[digit_counter]];
            PORTA = 0x01 << digit_counter;
        }
        else if (mode == 0)
        {
            PORTA = 0x00;
            PORTD = seg_char[display_digits[digit_counter]] + 0x80(digit_counter = 2);
            PORTA = 0x01 << digit_counter;
        }

        digit_counter++;
        if (digit_counter > 3)
        {
            digit_counter = 0;
        }
    }
    if (ADIF)
    {
        ADIF = 0;
        ADCON0bits.CHS = an_list[an_cnt];
        if (ADCON0bits.CHS == 0)
        {
            adc_value1 = ADRESH * 256 + ADRESL;
            weight = ((100 * adc_value1) / 1023) * 10;
        }
        if (ADCON0bits.CHS == 1)
        {
            adc_value2 = ADRESH * 256 + ADRESL;
        }
        an_cnt++;
        if (an_cnt >= 2)
        {
            an_cnt = 0;
        }
        INTCON2bits.GODONE = 1;
    }

    if (INTOIF)
    {
        INT0IF = 0;
        emergency = 1;
    }

    if (INT2IF)
    {
        INT2IF = 0;
        mode_changed = 1;
        if (emergency == 0)
        {
            if (mode == 0)
            {
                mode = 1;
            }
            else
            {
                mode = 0;
            }
        }
    }
}