#include <xc.h>
#include <stdio.h>

#define _XTAL_FREQ 8000000

// LCD Pin Tanımlamaları (HD44780 4-Bit Modu)
#define RS PORTBbits.RB4
#define EN PORTBbits.RB5

// Sistem Durum Değişkenleri
volatile unsigned int adc_raw = 0;
volatile unsigned long millivolts = 0;
volatile unsigned int pwm_duty = 0;
volatile unsigned char alarm_state = 0;

unsigned char display_digits[4] = {0, 0, 0, 0};
unsigned char digit_counter = 0;
unsigned char line1[17] = "";
unsigned char line2[17] = "";

const unsigned char seg_char[] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

// --- LCD SÜRÜCÜ FONKSİYONLARI ---
void lcd_strobe(void)
{
    EN = 1;
    __delay_us(2);
    EN = 0;
    __delay_us(2);
}

void lcd_write_4bit(unsigned char nibble)
{
    PORTB = (PORTB & 0xF0) | (nibble & 0x0F);
    lcd_strobe();
}

// 1. LCD Komut Gönderme Fonksiyonu
void lcd_cmd(unsigned char cmd)
{
    RS = 0;
    lcd_write_4bit(cmd >> 4); // Üst 4 bit
    lcd_write_4bit(cmd);      // Alt 4 bit
    __delay_ms(2);
}

// 2. LCD Ekrana Yazı (String) Basma Fonksiyonu (lcd_puts)
void lcd_puts(const char *str)
{
    RS = 1; // Veri modu
    while (*str)
    {
        lcd_write_4bit((*str) >> 4);
        lcd_write_4bit(*str);
        __delay_us(50);
        str++;
    }
}

// 3. LCD Başlatma Fonksiyonu (lcd_init)
void lcd_init(void)
{
    __delay_ms(20);
    RS = 0;
    EN = 0;
    lcd_write_4bit(0x03);
    __delay_ms(5);
    lcd_write_4bit(0x03);
    __delay_us(200);
    lcd_write_4bit(0x03);
    lcd_write_4bit(0x02); // 4-bit moduna geçiş komutu

    lcd_cmd(0x28); // 2 Satır, 5x8 Font
    lcd_cmd(0x0C); // Ekran açık, imleç kapalı
    lcd_cmd(0x06); // Giriş modu (Artan imleç)
}

// 4. LCD Ekran Temizleme Fonksiyonu (lcd_clear)
void lcd_clear(void)
{
    lcd_cmd(0x01);
    __delay_ms(2);
}

// 5. LCD Belirli Adrese Gitme Fonksiyonu (lcd_goto)
void lcd_goto(unsigned char pos)
{
    lcd_cmd(0x80 + pos); // 0x00 ilk satır başı, 0x40 ikinci satır başı
}

void main()
{

    ANSELA = 0x00;
    TRISA = 0x00;
    PORTA = 0x00;

    ANSELB = 0x00;
    TRISB = 0xCF;
    PORTB = 0x00;

    ANSELC = 0x00;
    TRISC = 0x00;
    PORTC = 0x00;

    ANSELD = 0x00;
    TRISD = 0x00;
    PORTD = 0x00;

    ANSELE = 0x02;
    TRISE = 0x07;
    PORTE = 0x00;

    T0CON = 0xC1;
    TMR0L = 6;
    TMR0IE = 1;
    TMR0IP = 1;

    INT1IE = 1;
    INT1IP = 1;

    GIE = 1;
    GODONE = 1;
    INTCON2.INTEDG1 = 0;

    T2CON = 0x06;
    PR2 = 124;

    CCP1CON = 0x0C;

    ADCON2 = 0xA1;
    ADCON1 = 0x00;
    ADCON0bits.ADON = 1;
    ADCON0bits.CHS = 6;

    lcd_init();
    lcd_clear();
    lcd_goto(0);

    while (1)
    {
        if (!GODONE && alarm_state == 0)
        {

            adc_raw = (ADRESH * 256 + ADRESL);
            millivolts = (5000 * adc_raw) / 1023;

            display_digits[3] = millivolts / 1000;
            display_digits[2] = (millivolts % 1000) / 100;
            display_digits[1] = (millivolts / 100) % 10;
            display_digits[0] = millivolts % 10;

            pwm_duty = adc_raw / ((PR2 + 1) * 4);

            CCPR1L = adc_raw >> 2;
            CCP1CON = (CCP1CON & 0xCF) + ((CCP1CON & 0x0003) << 4);

            sprintf(line1, "TEMP: %lu mV", millivolts);
            lcd_puts(line1);
            lcd_goto(0x40);
            sprintf(line2, "FAN: %d ", pwm_duty);
            lcd_puts(line2);

            GODONE = 1;
        }
    }
}

void_interrupt(high_priority) isr2()
{
    if (TMR0IF)
    {
        TMR0IF = 0;
        TMR0L = 6;

        PORTA = 0x00;
        PORTD = seg_char[display_digits[digit_counter]] + 0x80 * (digit_counter == 2);
        PORTA = 0x01 << digit_counter;

        digit_counter++;
        if (digit_counter >= 4)
        {
            digit_counter = 0;
        }
    }
    if (INT1IF)
    {
        INT1IF = 0;

        pwm_duty = 0;
        TRISbits.TRISC2 = 1;

        lcd_init();
        lcd_clear();
        lcd_goto(0);
        lcd_puts("ALARM");
        alarm_state = 1;
    }
}
