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
    displayCountdown = 30;
    int lastdisplayCountdown = 31;
    enableCompass();
    while(displayCountdown)
    {
      if (lastdisplayCountdown != displayCountdown)
      {
        if (state & STATE_SPIRIT_TOGGLE)
        {
          int level = readLevel();
          writeInt(level);
        }
        else
        {
          int angle;
          if (state & STATE_COMPASS_ANGLE_TOGGLE)
            angle = readCompass(COMPASS_VERTICAL);
          else
            angle = readCompass(COMPASS_HORIZONTAL);
          if (state & STATE_COMPASS_MODE_TOGGLE)
            if (angle > 180)
              angle -= 360;
          writeInt(angle);
        }
        lastdisplayCountdown = displayCountdown;
      }
      if (pushbuttonPressed & 0x01)
      {
        // This munches displayCountdown
        calibrate();
        pushbuttonPressed &= ~0x01;
        displayCountdown = 30;
      }
      if (pushbuttonPressed & 0x02)
      {
        state ^= STATE_COMPASS_MODE_TOGGLE;
        pushbuttonPressed &= ~0x02;
        displayCountdown = 30;
      }
      if (pushbuttonPressed & 0x04)
      {
        state ^= STATE_COMPASS_ANGLE_TOGGLE;
        pushbuttonPressed &= ~0x04;
        displayCountdown = 30;
      }
      if (pushbuttonPressed & 0x08)
      {
        state ^= STATE_SPIRIT_TOGGLE;
        pushbuttonPressed &= ~0x08;
        displayCountdown = 30;
      }
    }
    disableCompass();
    state &= ~STATE_SPIRIT_TOGGLE;
    sleep();
  }
}
