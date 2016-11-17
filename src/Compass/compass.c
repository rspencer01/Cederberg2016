#include <avr/wdt.h>
#include "compass.h"
#include "spi.h"
#include "utils.h"
#include "timers.h"
#include "sseg.h"

/// The x component of the magnetometer offset
int xcalib;
/// The y component of the magnetometer offset
int ycalib;

/// Set all the registers in the compass as desired
void initCompass()
{
  SPIWriteRegister(0x20,0x37);
  SPIWriteRegister(0x24,0x68);
  SPIWriteRegister(0x25,0x00);
  SPIWriteRegister(0x26,0x00);
}

/// Calibrates the compass
///
/// The user, in 8 seconds, must make a full revolution.  Both X and Y values
/// are agrigated every second over that period and the sum is stored as the
/// offset.
///
/// \note This function blocks.  It controls variable `initialWait` and resets
/// the watchdog of its own accord.
void calibrate()
{
  writeMessage(SSEG_CAL);

  initialWait = 1;
  while(initialWait)
    wdt_reset();

  int lastInitialWait = 9;
  xcalib = 0;
  ycalib = 0;
  initialWait = 8;
  while(initialWait)
  {
    if (lastInitialWait != initialWait)
    {
      lastInitialWait = initialWait;
      xcalib += readCompassX()/8;
      ycalib += readCompassY()/8;
    }
    writeInt(initialWait);
    delay(10);
    wdt_reset();
  }

  writeMessage(SSEG_DONE);

  initialWait = 1;
  while(initialWait)
    wdt_reset();
}

/// Reads the x value from the magnetometer
int readCompassX()
{
  int x = 0;
  x = SPIReadRegister(0x09 | 0x80);
  x <<= 8;
  x += SPIReadRegister(0x08 | 0x80);
  return x;
}

/// Reads the y value from the magnetometer
int readCompassY()
{
  int y = 0;
  y = SPIReadRegister(0x0b | 0x80);
  y <<= 8;
  y += SPIReadRegister(0x0a | 0x80);
  return y;
}

/// Returns the heading in degrees clockwise from North.
int readCompass()
{
  int x = readCompassX()-xcalib;
  int y = readCompassY()-ycalib;
  // Reverse things due to the layout of the package.
  return iatan2(-y,x);
}
