#include "spi.h"
#include "gpio.h"

/// We trust the gpio setup to set the MOSI and SCK as outputs and
/// the MISO as input
void initSPI()
{
  // Enable SPI, master and clockrate
  SPCR = (1<<SPE) | (1<<MSTR) | (1<<CPOL) | (1<<CPHA) | (1<<SPR1) | (1<<SPR0);
}


/// Send a single byte to the chip and get one byte back
unsigned char SPITranceiver(unsigned char data)
{
  // Enable the chip select
  portB &= ~0x80;
  setPorts();

  // Write the data out
  SPDR = data;
  // Poll a dummy byte, but don't watchdog out
  while (!(SPSR & (1<<SPIF) ))
  {
    __asm__ __volatile__ (" wdr ");
  }

  // Write a dummy byte
  SPDR = 0;
  // Poll, but don't watchdog out
  while (!(SPSR & (1<<SPIF) ))
  {
    __asm__ __volatile__ (" wdr ");
  }
  data = SPDR;

  // Disable the chip select (good manners)
  portB |= 0x80;
  setPorts();

  return data;
}
