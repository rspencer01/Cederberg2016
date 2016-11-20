#include <avr/wdt.h>
#include <avr/io.h>
#include "utils.h"
#include "gpio.h"
#include "sseg.h"
#include "timers.h"
#include "spi.h"
#include "compass.h"

int pushbuttonPressed = 0;

int main(void)
{
  initMicro();
  initPorts();
  initTimers();
  // Wait for the timer to time out. This is for development, and gives us time
  // to attach the compass module to the board.
  writeHex(0xdead);
  initialWait = 1;
  while(initialWait)
    wdt_reset();
  // Loop forever.  Every second, read the compass and display it.
  while(1)
  {
    enableCompass();
    writeInt(readCompass());
    if (pushbuttonPressed & 0x01)
    {
      calibrate();
      pushbuttonPressed &= ~0x01;
    }
    initialWait = 2;
    while(initialWait)
      wdt_reset();
    disableCompass();
    sleep();
  }
}
