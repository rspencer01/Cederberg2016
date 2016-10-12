#include "compass.h"
#include "spi.h"
#include "utils.h"

/// Set all the registers in the compass as desired
void initCompass()
{
  SPIWriteRegister(0x20,0x37);
  SPIWriteRegister(0x24,0x68);
  SPIWriteRegister(0x25,0x00);
  SPIWriteRegister(0x26,0x00);
}

/// Returns the heading in degrees clockwise from North.
int readCompass()
{
  int x = 0;
  x = SPIReadRegister(0x09 | 0x80);
  x <<= 8;
  x += SPIReadRegister(0x08 | 0x80);
  int y = 0;
  y = SPIReadRegister(0x0b | 0x80);
  y <<= 8;
  y += SPIReadRegister(0x0a | 0x80);
  // Reverse things due to the layout of the package.
  return iatan2(-y,x);
}
