#include <avr/interrupt.h>
#include "timers.h"
#include "sseg.h"

/// A countdown to divide the 4ms timer into 40ms
int timer_4ms_20ms;
/// A countdown to divide the 40ms timer into 1s
int timer_20ms_1s;
/// Number of seconds to wait before doing anything (see main loop)
int initialWait = 3;

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
  timer_4ms_20ms--;
  if (timer_4ms_20ms==0)
  {
    // Occurs every 240ms
    timer_4ms_20ms = INI_4MS_20MS;

    // Update the display strobing
    strobeDisplay();

    timer_20ms_1s--;
    if (timer_20ms_1s==0)
    {
      // Occurs every second
      timer_20ms_1s = INI_20MS_1S;
      if (initialWait > 0) initialWait--;
    }
  }
}

