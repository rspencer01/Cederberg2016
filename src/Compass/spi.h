#ifndef SPI_H_
#define SPI_H_

void initSPI();

unsigned char SPIReadRegister(unsigned char address);
unsigned char SPIWriteRegister(unsigned char address, unsigned char data);

#endif
