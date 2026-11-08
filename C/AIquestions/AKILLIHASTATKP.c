#include <xc.h>
#include <stdio.h>
#include <string.h>

// Konfigürasyonlar (MCU modeline göre ayarlanmalıdır)
#pragma config FOSC = HS, WDT = OFF, LVP = OFF

#define _XTAL_FREQ 20000000

// LCD Tanımlamaları
#define RS RB4
#define EN RB5
#define LCD_PORT PORTB

// Global Değişkenler
volatile unsigned int temp_raw, bpm_raw;
volatile int temp_val, bpm_val;
volatile unsigned char alarm_state = 0, display_state = 0;
volatile unsigned int timer0_counter = 0;
char line[16];

void main()
{
    TRISA = 0x01;
    ANSELA = 0x01;
    PORTA = 0x00;

    TRISB = 0x03;
    ANSELB = 0x00;
    PORTB = 0x00;

    TRISC = 0x00;
    ANSELC = 0x00;
    PORTC = 0x00;

    TRISD = 0x00;
    ANSELD = 0x00;
    PORTD = 0x00;

    TRISE = 0x02;
    ANSELE = 0x02;
    PORTE = 0x00;

    T0CON = 0xC3;
    INT1IE = 1;
    INT1IP = 1;
    INT0IE = 1;
    INT0IP = 1;
    GIE = 1;

    ADCON2 = 0xA1;
    ADCON1 = 0x00;
    ADCON0bits.CHS = 0;
    ADCON0bits.ADON = 1;
    GODONE = 1;

    PEIEbits.ADIE = 1;
    IPRbits.ADIP = 1;

    CCP1CON = 0x0C;
    T2CON = 0x04;

    lcd_init();
    lcd_clear();
    lcd_goto(0);

    while (1)
    {
        if (temp_val > 385 || bpm_val < 50 || bpm_val > 150)
        {
            alarm_state = 1;
        }
        if (alarm_state == 1)
        {
            lcd_puts(" ALARM! ");
            lcd_goto(0x40);
            if (temp_val > 385)
            {
                sprintf(line, " HIGH TEMP: %2d.%1d ", temp_val / 10, temp_val % 10);
            }
            else
            {
                sprintf(line, " LOW BPM: %3f ", bpm_val);
            }

            CCPR1L = 0x80;
            CCP1CONbits.DC1B = 0;

            GIE = 0;
            EEADR = 0x00;
            EEDATA = temp_val;
            EECON1bits.EEPGD = 0;
            EECON1bits.CFGS = 0;
            EECON1bits.WREN = 1;
            EECON2 = 0x55;
            EECON2 = 0xAA;
            EECON1bits.WR = 1;
            GIE = 1;
            while (EECON1bits.WR == 1)
                ;
            EECON1.WREN = 0;

            GIE = 0;
            EEADR = 0x01;
            EEDATA = bpm_val;
            EECON1bits.EEPGD = 0;
            EECON1bits.CFGS = 0;
            EECON1bits.WREN = 1;
            EECON2 = 0x55;
            EECON2 = 0xAA;
            EECON1bits.WR = 1;
            GIE = 1;
            while (EECON1bits.WR == 1)
                ;
            EECON1.WREN = 0;
        }

        if (alarm_state == 0)
        {
            sprintf(line, " TEMP: %2d.%1d", temp_val / 10, temp_val % 10);
            lcd_puts(line);
            lcd_goto(0x40);
            sprintf(line, " BPM: %3d", bpm_val);

            CCPR1L = 0x00;
            CCP1CON.DC1B = 0;
        }

        if (display_state == 1)
        {

            GIE = 0;
            EEADR = 0x0A;
            EEDATA = temp_val;
            EECON1bits.EEPGD = 0;
            EECON1bits.CFGS = 0;
            EECON1bits.WREN = 1;
            EECON2 = 0x55;
            EECON2 = 0xAA;
            EECON1bits.WR = 1;
            GIE = 1;
            while (EECON1bits.WR == 1)
                ;
            EECON1bits.WREN = 0;

            GIE = 0;
            EEADR = 0x0B;
            EEDATA = bpm_val;
            EECON1bits.EEPGD = 0;
            EECON1bits.CFGS = 0;
            EECON1bits.WREN = 1;
            EECON2 = 0x55;
            EECON2 = 0xAA;
            EECON1bits.WR = 1;
            GIE = 1;
            while (EECON1bits.WR == 1)
                ;
            EECON1bits.WREN = 0;

            lcd_clear();
            lcd_goto(0x40);
            lcd_puts(" SAVED ");
        }

        if (display_state == 2)
        {
            EEADR = 0x0A;
            EECON1bits.EEPGD = 0;
            EECON1bits.CFGS = 0;
            EECON1bits.RD = 1;

            EEADR = 0x0B;
            EECON1bits.EEPGD = 0;
            EECON1bits.CFGS = 0;
            EECON1bits.RD = 1;

            sprintf(line, " LAST TEMP: %2d.%1d", temp_val / 10, temp_val % 10);
            lcd_goto(0);
            lcd_puts(line);
            lcd_goto(0x40);
            sprintf(line, " LAST BPM: %3d", bpm_val);
            lcd_puts(line);
            delay_ms_(3000);
        }
    }
}

void__interrupt(high_priority) isr()
{
    if (TMR0IF)
    {
        TMR0IF = 0;
        TMR0L = 6;
        timer0_counter++;

        if (timer0_counter >= 125)
        {
            timer0_counter = 0;
        }
    }
    if (ADIF)
    {
        ADIF = 0;
        if (ADCON0bits.CHS == 6)
        {
            temp_raw = ADRESH * 256 + ADRESL;
            temp_val = 300 + (150 * temp_raw) / 1023;

            ADCON0bits.CHS = 0;
        }
        else if (ADCON0bits.CHS == 0)
        {
            ADCON0bits.CHS = 0;
            bpm_raw = ADRESH * 256 + ADRESL;
            bpm_val = 40 + (160 * bpm_raw) / 1023;

            ADCON0bits.CHS = 6;
        }

        ADCON0bits.GODONE = 1;
    }
    if (INT1IF)
    {
        INT1IF = 0;
        display_state = 2;
    }

    if (INT0IF)
    {
        INT0IF = 0;
        display_state = 1;
    }
}