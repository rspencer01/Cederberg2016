/// @file compass.c
///
/// Code controlling the interface to the compass/accelerometer chip LSM303D

#include "compass.h"
#include "gpio.h"
#include "state.h"
#include "spi.h"
#include "utils.h"
#include "timers.h"
#include "sseg.h"

/// The x component of the magnetometer offset
int x_mag_calib;
/// The y component of the magnetometer offset
int y_mag_calib;
/// The y component of the magnetometer offset
int z_mag_calib;

/// Set all the registers in the compass as desired
void enableCompass()
{
  portC |= 0b00000001;
  setPorts();
  initSPI();
  enableSPI();
  delay(50);
  SPIWriteRegister(0x20,0x17);
  SPIWriteRegister(0x21,0x00);
  SPIWriteRegister(0x24,0x68);
  SPIWriteRegister(0x25,0x00);
  SPIWriteRegister(0x26,0x00);
  delay(50);
}

/// Calibrates the compass offset
///
/// The user, in 8 seconds, must make a full revolution.  Both X and Y values
/// are agrigated every second over that period and the sum is stored as the
/// offset.
///
/// \note This function blocks.  It controls variable `displayCountdown`
void calibrate()
{
  // Do this so we get a full second of the first message
  displayCountdown = 1;
  while(displayCountdown);

  writeMessage(SSEG_CAL);
  displayCountdown = 2;
  while(displayCountdown);

  writeMessage(SSEG_HZTL);
  displayCountdown = 4;
  while(displayCountdown);

  int lastdisplayCountdown = 9;
  x_mag_calib = 0;
  y_mag_calib = 0;
  displayCountdown = 8;
  while(displayCountdown)
  {
    if (lastdisplayCountdown != displayCountdown)
    {
      lastdisplayCountdown = displayCountdown;
      x_mag_calib += readCompassX()/8;
      y_mag_calib += readCompassY()/8;
    }
    writeInt(displayCountdown);
    delay(10);
  }

  writeMessage(SSEG_VERT);
  displayCountdown = 4;
  while(displayCountdown);

  lastdisplayCountdown = 9;
  z_mag_calib = 0;
  displayCountdown = 8;
  while(displayCountdown)
  {
    if (lastdisplayCountdown != displayCountdown)
    {
      lastdisplayCountdown = displayCountdown;
      z_mag_calib += readCompassZ()/8;
    }
    writeInt(displayCountdown);
    delay(10);
  }

  writeMessage(SSEG_DONE);

  displayCountdown = 1;
  while(displayCountdown);
}

/// Reads the x value from the magnetometer
///
int readCompassX()
{
  int x = 0;
  x = SPIReadRegister(0x09 | 0x80);
  x <<= 8;
  x += SPIReadRegister(0x08 | 0x80);
  return x;
}

/// Reads the y value from the magnetometer
///
int readCompassY()
{
  int y = 0;
  y = SPIReadRegister(0x0b | 0x80);
  y <<= 8;
  y += SPIReadRegister(0x0a | 0x80);
  return y;
}

/// Reads the z value from the magnetometer
///
int readCompassZ()
{
  int z = 0;
  z = SPIReadRegister(0x0d | 0x80);
  z <<= 8;
  z += SPIReadRegister(0x0c | 0x80);
  return z;
}

/// Reads the x value from the magnetometer
///
int readAccelerometerY()
{
  int x = 0;
  x = SPIReadRegister(0x2b | 0x80);
  x <<= 8;
  x += SPIReadRegister(0x2a | 0x80);
  return x;
}

int readLevel()
{
  return readAccelerometerY()/10;
}

/// Returns the heading in degrees clockwise from North.
///
int readCompass(int mode)
{
  int x = readCompassX()-x_mag_calib;
  int y;
  if (mode == COMPASS_HORIZONTAL)
    y = readCompassY()-y_mag_calib;
  else
    y = readCompassZ()-z_mag_calib;
  // Reverse things due to the layout of the package.
  return iatan2(-y,x);
}

/// Disable the compass, turning of its power
///
/// To maintain a lower power footprint, the compass module can be hard.
/// deactivated by removing its power line (which is controlled by pin C0)
/// and the power to its surrounding infrastructure.
void disableCompass()
{
  // Remove SPI control of the B port
  SPCR &= ~_BV(SPE);
  // Disable the compass power line
  portC &= ~0b00000001;
  // Set all the SPI ports to low manually
  portB &= ~0b00101000;
  setPorts();
  // SPI enable is high on disable.  Disable it to pull down all
  // the pins
  enableSPI();
}
