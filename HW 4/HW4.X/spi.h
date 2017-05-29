#ifndef SPI_H_
#define SPI_H_

void initSPI1();
void setVoltage(unsigned int channel, unsigned int voltage);
char SPI1_IO(unsigned char write);

#endif