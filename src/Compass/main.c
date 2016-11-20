#include <avr/wdt.h>
#include <avr/io.h>
#include "utils.h"
#include "gpio.h"
#include "sseg.h"
#include "state.h"
#include "timers.h"
#include "spi.h"
#include "compass.h"

int pushbuttonPressed = 0;
int state = STATE_COMPASS_360;

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
    int angle = readCompass();
    if (state == STATE_COMPASS_180)
      if (angle > 180)
        angle -= 360;
    writeInt(angle);
    if (pushbuttonPressed & 0x01)
    {
      calibrate();
      pushbuttonPressed &= ~0x01;
    }
    if (pushbuttonPressed & 0x02)
    {
      state ^= STATE_COMPASS_ANGLE_TOGGLE;
      pushbuttonPressed &= ~0x02;
    }
    initialWait = 2;
    while(initialWait)
      wdt_reset();
    disableCompass();
    sleep();
  }
}
