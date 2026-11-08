#include <stdio.h>
#include <stdlib.h>
#include <pic18f45k22.h>
#include <htc.h>

#define _XTAL_FREQ 8000000

#pragma config FOSC = HSHP
#pragma config WDTEN = OFF

unsigned int value[2] = {0, 0};
unsigned char an_cnt = 0;
unsigned char an_list[2] = {0, 1}; // list for deciding which AN registers are going to be used

void(main)
{
    ANSELA = 0x03;
    TRISA = 0xFF;

    ANSELC = 0x00;
    TRISC = 0xF9;

    ADCON2 = 0xA1 ADCON1 = 0x00;
    ADCON0 = 0x19;

    ADCON0bits.CHS = an_list[0];
    ADCON0bits.ADON = 1;

    GODONE = 1;

    T2C0N = 0x04;
    PR2 = 255;
    CCP1CON = 0x0C CCP2CON = 0x0C

        while (1)
    {
        if (!GODONE)
        {
            value[an_cnt] = ADRESH * 256 + ADRESL;
            an_cnt++;
            if (an_cnt >= 2)
            {
                an_cnt = 0;
            }
            ADCONbits.CHS = an_list[an_cnt];
            GODONE = 1;
        }
        CCPR1L = value[0] >> 2;
        CCP1CON = (CCP1CON & OxCF) + ((value[0] & 0x0003) << 4);

        CCPR2L = value[1] >> 2;
        CCP2CON = (CCP2CON & OxCF) + ((value[1] & 0x0003) << 4;)
    }
}
