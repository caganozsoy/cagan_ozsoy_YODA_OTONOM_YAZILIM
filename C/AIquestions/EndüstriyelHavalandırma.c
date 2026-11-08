#include <stdio.h>
#include <stdlib.h>
#include <pic18f45k22.h>
#include <htc.h>

// Konfigürasyon Ayarları
#pragma config FOSC = HSHP // Yüksek Hızlı Kristal Modu
#pragma config WDTEN = OFF // Watchdog Timer Kapalı

#define _XTAL_FREQ 8000000 // 8 MHz Sistem Saati

// LCD Pin Tanımlamaları (Ders notlarındaki harici kütüphane varsayımı)
// Notlarda kullanılan standart prototipler: lcd_init(), lcd_goto(), lcd_puts(), lcd_clear()
extern void lcd_init(void);
extern void lcd_goto(unsigned char pos);
extern void lcd_puts(const char *s);
extern void lcd_clear(void);

// Global Değişkenler
unsigned char seg_char[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F}; // Common Cathode Lookup Table
unsigned char digits[4] = {0, 0, 0, 0};
unsigned char digit_counter = 0;
unsigned int adc_value = 0;
volatile float temperature = 0.0;
volatile unsigned char emergency_count = 0;
volatile unsigned char emergency_flag = 0;
unsigned char line[17] = " ";

void main()
{
    TRISA = 0x01;
    ANSELA = 0x01;
    PORTA = 0x00;

    TRISB = 0x01;
    ANSELB = 0x00;
    PORTB = 0x00;

    TRISC = 0xF9;
    ANSELC = 0x00;
    PORTC = 0x00;

    TRISD = 0x00;
    ANSELD = 0x00;
    PORTD = 0x00;

    TRISE = 0x00;
    ANSELE = 0x00;
    ;
    PORTE = 0x00;

    T0CON = 0xC3;
    TMR0L = 6;
    TMR0IE = 1;
    TMR0IP = 1;

    INT0IE = 1;
    INT0IP = 1;

    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;

    INTCON2bits.INTEDG0 = 0;

    ADCON2 = 0xA1;
    ADCON1 = 0x00;
    ADCON0bits.CHS = 0;
    ADCON0bits.ADON = 1;
    GODONE = 1;

    PR2 = 255;
    CCP1CON = 0x0C;
    T2CON = 0x05;

    lcd_init();
    lcd_clear();
    lcd_goto(0);

    while (1)
    {
        if (emergency_flag == 1)
        {
            GIE = 0;
            EEADR = 0x10;
            EEDATA = emergency_count;
            EECON1bits.EEPGD = 0;
            EECON1bits.CFGS = 0;
            EECON1bits.WREN = 1;
            EECON2 = 0x55;
            EECON2 = 0xAA;
            EECON1bits.WR = 1;
            GIE = 1;
            while (EECON1bits.WR)
                ;
            EECON1bits.WREN = 0;

            lcd_clear();
            lcd_goto(0);
            lcd_puts(" EMERGENCY STOP! ");

            while (1)
            {
            }
        }
        if (emergency_flag == 0)
        {
            if (!GODONE)
            {

                adc_value = ADRESH * 256 + ADRESL;
                temperature = (500.0 * adc_value) / 1023.0;

                sprintf(line, " TEMP %0.2f C ", temperature);

                int temp = temperature * 10;

                digits[3] = temp / 1000;
                digits[2] = (temp % 1000) / 100;
                digits[1] = (temp % 100) / 10;
                digits[0] = (temp % 10);

                if (temperature < 25.0)
                {
                    CCPR1L = 0x00;
                    CCP1CONbits.DC1B = 0;
                    lcd_goto(0x40);
                    lcd_puts(" FAN: OFF ");
                }
                else if (temperature >= 25 && temperature < 40)
                {
                    CCPR1L = 0x80; //
                    CCP1CONbits.DC1B = 0;
                    lcd_goto(0x40);
                    lcd_puts(" FAN: MID ");
                }
                else
                {
                    CCPR1L = 0xFF;
                    CCP1CONbits.DC1B = 3;
                    lcd_goto(0x40);
                    lcd_puts(" FAN: MAX ");
                }
                GODONE = 1;
            }
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
        PORTD = seg_char[digits[digit_counter]] + 0x80 * (digit_counter == 1);
        PORTA = 0x02 << digit_counter;

        digit_counter++;
        if (digit_counter >= 4)
        {
            digit_counter = 0;
        }
    }
    if (INT0IF)
    {
        INT0IF = 0;
        emergency_count++;
        emergency_flag = 1;

        CCPR1L = 0x00;
        CCP1CONbits.DC1B = 0;
        TRISCbits.TRISC2 = 1;
    }
}