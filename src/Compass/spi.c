#include "spi.h"
#include "gpio.h"

/// Set up the builtin registers to run SPI
///
/// We trust the gpio setup to set the MOSI and SCK as outputs and
/// the MISO as input.
///
/// We enable spi, set the ATMega as the master, set the correct
/// clock polarity (high when idle) and phase and set the speed select
/// to `f_asc/16`.
void initSPI()
{
  // Enable SPI, master and clockrate
  SPCR = (1<<SPE) | (1<<MSTR) | (1<<CPOL) | (1<<CPHA) | (1<<SPR1) | (1<<SPR0);
}

/// Enable the SPI device
///
/// It is good practice to do this just before reading/writing, and to
/// disable just after.
void enableSPI()
{
  portB &= ~0x80;
  setPorts();
}

/// Disable the SPI device
///
/// It is good practice to do this just after reading/writing.
void disableSPI()
{
  portB |= 0x80;
  setPorts();
}

/// Write a byte to SPI and get the MISO reply.
///
/// This is the "raw" SPI transaction.  A single byte is written out
/// and whatever is returned over the line at the same time is returned
/// by this funciton.
///
/// Instead of using interrupts, we poll the status register until there
/// is data to return.  This might not be the best thing to do...
unsigned char RdWrSPI(unsigned char data)
{
  // Write the data out
  SPDR = data;
  // Poll a dummy byte, but don't watchdog out
  while (!(SPSR & (1<<SPIF) ))
  {
    __asm__ __volatile__ (" wdr ");
  }
  data = SPDR;
  return data;
}

/// Reads a register of the SPI device.
///
/// Performs two read/writes to read a single 8bit register of the device.
unsigned char SPIReadRegister(unsigned char address)
{
  enableSPI();
  RdWrSPI(address);
  unsigned char data = RdWrSPI(0);
  disableSPI();
  return data;
}

/// Writes a register of the SPI device
///
/// Performs a two read/writes to set a single 8bit register of the device.
unsigned char SPIWriteRegister(unsigned char address, unsigned char data)
{
  enableSPI();
  RdWrSPI(address);
  RdWrSPI(data);
  disableSPI();
  return data;
}
