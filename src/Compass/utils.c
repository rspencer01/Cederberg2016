/// @file utils.c
///
/// Various utilities and initialisations.
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include "compass.h"
#include "gpio.h"
#include "sseg.h"
#include "utils.h"

/// Initialises the microcontroller
///
/// Sets up the watchdog timer and the type of sleep in the registers.
void initMicro()
{
  cli();
  // Stop the watchdog timer
  wdt_disable();
  // Set the sleep mode to "power-down"
  SMCR = _BV(SM1);

  // Disable external interrupts
  EIMSK = 0;
  // Enable pin change interrupts
  PCICR = _BV(PCIE0);
  PCMSK0 = _BV(PCINT6) | _BV(PCINT2) | _BV(PCINT1) | _BV(PCINT0);
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
/// @param a The numerator of the fractional index
/// @param b The denominator of the fractional index
/// @param v The array to interpolate
/// @param n The lenght of `v`
int flerp(int a, int b,int* v, int n)
{
  int va = v[min((a/b),n)];
  int vb = v[min((a/b)+1,n)];
  int f = a%b;
  return va + (vb-va) * f / b;
}

/// Calculates the number of digits (in base 10) of the argument.
///
/// @param n A positive or negative number
/// @returns The number of digits in `n`
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
///
/// @param x The x coordinate of the point
/// @param y The y coordinate of the point
/// @returns The angle in degrees from 0 to 360
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
///
/// @param ms The approximate number of milliseconds to block
void delay(int ms)
{
  unsigned int n;

  while (ms!=0)
  {
    // Gotten from some old code for a 4MHz processor and doubled
    n = 495;
    while (--n);
    --ms;
  }
}
