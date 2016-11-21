#ifndef SPI_H_
#define SPI_H_

void initSPI();

unsigned char SPIReadRegister(unsigned char address);
void SPIWriteRegister(unsigned char address, unsigned char data);
void disableSPI();
void enableSPI();
#endif
