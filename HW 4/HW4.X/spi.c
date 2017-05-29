#include <proc/p32mx250f128b.h>
#include "spi.h"

#define CS LATBbits.LATB7

void setVoltage(unsigned int channel, unsigned int voltage)
{
    char bits[] = {0, 0};
    bits[0] = (((channel << 3) | 0b0111) << 4) | (voltage >> 4);
    bits[1] = voltage << 4;
    
    CS = 0;
    SPI1_IO(bits[0]);
    SPI1_IO(bits[1]);
    CS = 1;
}

void initSPI1()
{
    SPI1CON = 0;
    SPI1BUF;
    SPI1BRG = 0x1000; // 0x1000 to see on N-scope
    SPI1STATbits.SPIROV = 0;
    SPI1CONbits.CKP = 0;
    SPI1CONbits.CKE = 1;
    SPI1CONbits.MSTEN = 1;
    SPI1CONbits.ON = 1;
    
    ANSELBbits.ANSB13 = 0;
    RPB13Rbits.RPB13R = 0b0011; //B13, pin 24, is SDO1
    SDI1Rbits.SDI1R = 0b0100;   //B8,  pin 17, is SDI1
    TRISBbits.TRISB7 = 0;       //B7,  pin 16, is CS   
}

char SPI1_IO(unsigned char write)
{
    SPI1BUF = write;
    while(!SPI1STATbits.SPIRBF){;}
    return SPI1BUF;   
}