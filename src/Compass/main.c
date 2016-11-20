#include <avr/wdt.h>
#include <avr/interrupt.h>
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
int displayCountdown = 0;

int main(void)
{
  initMicro();
  initPorts();
  initTimers();
  // Loop forever
  while(1)
  {
    enableCompass();
    displayCountdown = 10;
    int lastdisplayCountdown = 11;
    while(displayCountdown)
    {
      if (lastdisplayCountdown != displayCountdown)
      {
        int angle = readCompass();
        if (state == STATE_COMPASS_180)
          if (angle > 180)
            angle -= 360;
        writeInt(angle);
        lastdisplayCountdown = displayCountdown;
      }
      if (pushbuttonPressed & 0x01)
      {
        // This munches displayCountdown
        calibrate();
        pushbuttonPressed &= ~0x01;
        displayCountdown = 10;
      }
      if (pushbuttonPressed & 0x02)
      {
        state ^= STATE_COMPASS_ANGLE_TOGGLE;
        pushbuttonPressed &= ~0x02;
        displayCountdown = 10;
      }
      wdt_reset();
    }
    disableCompass();
    sleep();
  }
}
