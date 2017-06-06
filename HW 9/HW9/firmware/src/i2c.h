#ifndef I2C_H__
#define I2C_H__
// Header file for i2c_master_noint.c
// helps implement use I2C1 as a master without using interrupts

#define SLAVE_ADDRESS  0b1101011
#define SHIFTED_SLAVE_ADDRESS SLAVE_ADDRESS << 1
#define SHIFTED_SLAVE_ADDRESS_OR_0 SHIFTED_SLAVE_ADDRESS | 0
#define SHIFTED_SLAVE_ADDRESS_OR_1 SHIFTED_SLAVE_ADDRESS | 1
#define CTRL1_XL    0x10
#define CTRL2_G     0x11
#define CTRL3_C     0x12
#define OUT_TEMP_L  0x20
#define OUTX_L_XL   0x28
#define WHO_AM_I    0x0F

void i2c_init(void);
void i2c_start(void);
void i2c_restart(void);
void i2c_send(unsigned char value);
unsigned char i2c_receive(void);
void i2c_receive_multiple(unsigned char reg, unsigned char *data, int length);
void i2c_ack(int value);
void i2c_stop(void);

void i2c_expander_init();
void i2c_expander_set(char address, char value);
char i2c_expander_get(char address);
#endif