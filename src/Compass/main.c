#include <avr/wdt.h>
#include <avr/io.h>
#include "utils.h"
#include "gpio.h"
#include "sseg.h"
#include "timers.h"
#include "spi.h"
#include "compass.h"

int main(void)
{
  initMicro();
  initPorts();
  initTimers();
  initSPI();
  // Turn on the compass
  portC |= 0b00000001;
  setPorts();
  // Wait for the timer to time out. This is for development, and gives us time
  // to attach the compass module to the board.
  writeHex(0xdead);
  initialWait = 4;
  while(initialWait)
    wdt_reset();
  // Now that the compass module is attached, we can initialise it.
  initCompass();
  calibrate();
  // Loop forever.  Every second, read the compass and display it.
  while(1)
  {
    writeInt(readCompass());
    initialWait = 1;
    while(initialWait)
      wdt_reset();
  }
}
