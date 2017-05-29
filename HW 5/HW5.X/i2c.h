#ifndef I2C__H__
#define I2C__H__

void i2c_init(void);
void i2c_start(void);
void i2c_restart(void);
void i2c_send(unsigned char value);
unsigned char i2c_receive(void);
void i2c_ack(int value);
void i2c_stop(void);

void i2c_expander_init();
void i2c_expander_set(char address, char value);
char i2c_expander_get(char address);

#endif