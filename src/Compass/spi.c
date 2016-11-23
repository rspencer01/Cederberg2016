/// @file spi.c
///
/// Low level SPI interface code to read and write registers on peripheral
/// devices.
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
///
/// Indeed, if the SS port is pulled low, this will fail as the SPI system
/// will be in slave mode (see `ISR(PCINT0_vect)`).  Thus in this case the
/// function will return a dummy response.
///
/// @param data The byte to transmit over SPI
///
/// @returns The reply via SPI, received during transmission of the byte
unsigned char RdWrSPI(unsigned char data)
{
  if (readPushButton(PUSHBUTTON_3))
    return -1;
  // Write the data out
  SPDR = data;
  // Poll a dummy byte
  while (!(SPSR & (1<<SPIF) ));
  data = SPDR;
  return data;
}

/// Reads a register of the SPI device.
///
/// Performs two read/writes to read a single 8bit register of the device.
///
/// @param address The address of the peripheral register to read
///
/// @returns The value held in the peripheral register
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
///
/// @param address The address of the peripheral register to write
/// @param data The value to write to the register
void SPIWriteRegister(unsigned char address, unsigned char data)
{
  enableSPI();
  RdWrSPI(address);
  RdWrSPI(data);
  disableSPI();
}
