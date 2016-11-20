#include <avr/wdt.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include "compass.h"
#include "gpio.h"
#include "sseg.h"
#include "utils.h"

/// Resets the watchdog timer configuration.
///
/// Resets the watchdog to timeout after 4 seconds, and to fire an interrupt on
/// next timeout.  Thereafter this function must be called again to set up
/// for another interrupt.
///
/// I think it must be possible to only have to call this once, but can't seem
/// to get it to work.  This will have to do.
void resetWatchdogConfig()
{
  // This here is magic.  After hours of stuffing around with registers, I
  // found a macro in <avr/wdt.h> which, after modification, fit my requirements
  // Please do not edit this too much.
  __asm__ __volatile__ (
      "in __tmp_reg__,__SREG__" "\n\t"
      "cli" "\n\t"
      "wdr" "\n\t"
      "sts %0, %1" "\n\t"
      "out __SREG__,__tmp_reg__" "\n\t"
      "sts %0, %2" "\n \t"
      : /* no outputs */
      : "n" (_SFR_MEM_ADDR(_WD_CONTROL_REG)),
      "r" ((uint8_t)(_BV(WDCE) | _BV(WDE))),
      "r" ((uint8_t) (_BV(WDIE) | _BV(WDP3) | _BV(WDP0)))
      : "r0"
  );
}

/// Initialises the microcontroller
///
/// Sets up the watchdog timer and the type of sleep in the registers.
void initMicro()
{
  cli();
  resetWatchdogConfig();
  // Set the sleep mode to "power-down"
  SMCR = _BV(SM1);

  // Disable external interrupts
  EIMSK = 0;
  // Enable pin change interrupts
  PCICR = _BV(PCIE0);
  PCMSK0 = _BV(PCINT6);
  sei();
}

/// Puts the microcontroller in a sleep.
///
/// Prepares the chip to sleep, drawing as little current as
/// possible; sets the push buttons interrupts; and actually
/// goes to sleep.
void sleep()
{
  // Make the screen blank
  writeClear();
  blankDisplay();
  setPorts();
  // Enable sleeping
  SMCR |= _BV(SE);
  // Actually sleep
  __asm__ __volatile__("sleep");
}

/// Returns the linearly interpolated `v[a/b]`
///
/// Parameter `n` is the length of `v`.
int flerp(int a, int b,int* v, int n)
{
  int va = v[min((a/b),n)];
  int vb = v[min((a/b)+1,n)];
  int f = a%b;
  return va + (vb-va) * f / b;
}

/// Calculates the number of digits (in base 10) of the argument.
int digits(int n)
{
  n = abs(n);
  if (n==0) return 1;
  int d = 0;
  while (n > 0)
  {
    d += 1;
    n /= 10;
  }
  return d;
}

/// The values of `atan(x/10)` for `0 <= x <= 10`.  Used for linear interpolating
int atan2v[] = {0, 6, 11, 17, 22, 27, 31, 35, 39, 42, 45};

/// Calculate the angle of a point in xy space from the positive x axis.
///
/// The result is in degrees from 0 to 360.
int iatan2(int x, int y)
{
  if (x==0)
    return y>0 ? 90 : 270;
  int v;
  if (abs(y)<abs(x))
    v = flerp(abs(y)*10,abs(x),atan2v,10);
  else
    v = 90 - flerp(abs(x)*10,abs(y),atan2v,10);
  if (x>=0 && y>=0) return v;
  if (x<=0 && y>=0) return 180-v;
  if (x>=0 && y<=0) return 360-v;
  return 180+v;
}

/// Delays for a given number of milliseconds
///
/// Simply enters a loop for a calculated number of iterations to
/// block for the desired time.
///
/// \note This function is very approximate and is only correct
///       to within an order of magnitude.  Higher precision timing
///       should be done with the `timer0` interrupts.
void delay(int ms)
{
  unsigned int n;

  while (ms!=0)
  {
    // Gotten from some old code for a 4MHz processor and doubled
    n = 495;
    while (--n)
      wdt_reset();
    --ms;
  }
}
