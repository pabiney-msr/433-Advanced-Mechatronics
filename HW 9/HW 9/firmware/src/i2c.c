#include <proc/p32mx250f128b.h>
#include "i2c.h"

void i2c_init(void)
{
    ANSELBbits.ANSB2 = 0;
    ANSELBbits.ANSB3 = 0;
    I2C2BRG = 233;
    I2C2CONbits.ON = 1;
}

void i2c_start(void)
{
    I2C2CONbits.SEN = 1;
    while (I2C2CONbits.SEN){;}
}

void i2c_restart(void)
{
    I2C2CONbits.RSEN = 1;
    while (I2C2CONbits.RSEN){;}
}

void i2c_send(unsigned char value)
{
    I2C2TRN = value;
    while (I2C2STATbits.TRSTAT){;}
}

unsigned char i2c_receive(void)
{
    I2C2CONbits.RCEN = 1;
    while (!I2C2STATbits.RBF){;}
    return I2C2RCV;
}


void i2c_receive_multiple(unsigned char reg, unsigned char *data, int length)
{
    int i = 0;
    i2c_start();
    i2c_send(SHIFTED_SLAVE_ADDRESS_OR_0);
    i2c_send(reg);
    i2c_restart();
    i2c_send(SHIFTED_SLAVE_ADDRESS_OR_1);
    for(; i<length;++i)
    {
        data[i] = i2c_receive();
        i2c_ack((i != length - 1) ? 0 : 1);
    }
    i2c_stop();
}

void i2c_ack(int value)
{
    I2C2CONbits.ACKDT = value;
    I2C2CONbits.ACKEN = 1;
    while (I2C2CONbits.ACKEN){;}
}

void i2c_stop(void)
{
    I2C2CONbits.PEN = 1;
    while (I2C2CONbits.PEN){;}
}

void i2c_expander_init()
{
    i2c_expander_set(CTRL1_XL, 0x82);
    i2c_expander_set(CTRL2_G, 0x88);
    i2c_expander_set(CTRL3_C, 0x04);
}

void i2c_expander_set(char address, char value)
{
    i2c_start();
    i2c_send(SHIFTED_SLAVE_ADDRESS_OR_0);
    i2c_send(address);
    i2c_send(value);
    i2c_stop();
}

char i2c_expander_get(char address)
{
    char output;
    i2c_start();
    i2c_send(SHIFTED_SLAVE_ADDRESS_OR_0);
    i2c_send(address);
    i2c_restart();
    i2c_send(SHIFTED_SLAVE_ADDRESS_OR_1);
    output = i2c_receive();
    i2c_ack(1);
    i2c_stop();
    return output;
}