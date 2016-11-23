/// @file timers.c
///
/// All the timer set up code and ISR vectors.
#include <avr/interrupt.h>
#include "timers.h"
#include "sseg.h"
#include "spi.h"
#include "gpio.h"
#include "state.h"
#include "compass.h"
#include "reaction.h"

extern int count;
extern int pushbuttonPressed;
/// A countdown to divide the 4ms timer into 40ms
int timer_4ms_20ms;
/// A countdown to divide the 40ms timer into 1s
int timer_20ms_1s;
/// Number of seconds to wait before doing anything (see main loop)
int initialWait = 3;

/// The pushbutton debounce series
int buttonDebounce[4] = {0,0,0,0};

/// Initialises the timer module.
///
/// Sets up the timers to operate on a 4ms and a 1s basis.
void initTimers()
{
  // Enable Interrupt TimerCounter0 Compare Match A (TIMER0_COMPA_vect)
  TIMSK0 = _BV(OCIE0A);
  // Mode = CTC (Clear timer on compare match)
  TCCR0A = _BV(WGM01);
  // The CKDIV8 fuse should be set.  On an internal clock of 8MHz, that
  // gives (with a prescaler of 64)
  // Clock/8 = 1MHz/64 = 64 micro s per tick
  TCCR0B = _BV(CS01) | _BV(CS00);
  // Set compare match to be 128 (64micros*128 = 8ms)
  OCR0A = 64;
  // Set up the scalars
  timer_4ms_20ms = INI_4MS_20MS;
  timer_20ms_1s = INI_20MS_1S;
}


/// The interrupt service routine for the timer0
///
/// Updates the display strobe on a 4ms basis.
ISR(TIMER0_COMPA_vect)
{
  // Update the display strobing
  strobeDisplay();

  timer_4ms_20ms--;
  if (timer_4ms_20ms==0)
  {
    // Occurs every 20ms
    timer_4ms_20ms = INI_4MS_20MS;

    if (reactCountdown > 0)
      reactCountdown--;

    int i;
    for (i = 0; i < 4; ++i)
    {
      buttonDebounce[i] <<= 1;
      if (readPushButton(i))
        buttonDebounce[i] |= 1;
      if (buttonDebounce[i] == 0b0001111)
        pushbuttonPressed |= 1<<i;
    }

    timer_20ms_1s--;
    if (timer_20ms_1s==0)
    {
      // Occurs every second
      timer_20ms_1s = INI_20MS_1S;
      if (displayCountdown> 0) displayCountdown--;
    }
  }
}

/// The PCINT0 vector
///
/// Called by the depression of pushbutton 3 or 4.  Since pushbutton
/// 3 is on SS port, depressing this pin kills the SPI mode (see
/// section 19.3 of the datasheet).  We thus immediately reset SPI
/// configuration and port direction.
ISR(PCINT0_vect)
{
  initSPI();
  initPorts();
}
