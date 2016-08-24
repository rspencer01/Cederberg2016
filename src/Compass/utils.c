#include <avr/io.h>

/// Initialises the microcontroller
///
/// Sets up the watchdog timer, and the pushbutton interrupts
/// and the type of sleep in the registers.
void initMicro()
{
  // Disable all interrupts
  SREG &= ~_BV(SREG_I);

  // Clear these bits, in this order, on startup/restart, as advised in datasheet (pg51)
  MCUSR &= ~_BV(WDRF);
  // Set the timer bits that let us change things
  WDTCSR |= _BV(WDCE) | _BV(WDE);
  // Quickly!  Within 4 clock cycles we must set up the watchdog
  WDTCSR = _BV(WDIE) | _BV(WDP3) | _BV(WDP0);
  // Set the watchdog to give an interrupt
  WDTCSR |= _BV(WDIE);

  // Set the sleep mode to "power-down"
  SMCR = _BV(SM1);

  // Enable the interrupts again
  SREG |= _BV(SREG_I);
}
