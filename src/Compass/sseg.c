#include "sseg.h"
#include "gpio.h"
/// An array of the pins that activate each of the four displays
const unsigned char SSEG_SELECT[4] = {0b00000100,0b00001000,0b00010000,0b00100000};

/// An array consisting of the digits represented in seven segment displays
const unsigned char SSEG_NUMBERS[10] = { SSEG_ZERO,
                                         SSEG_ONE,
                                         SSEG_TWO,
                                         SSEG_THREE,
                                         SSEG_FOUR,
                                         SSEG_FIVE,
                                         SSEG_SIX,
                                         SSEG_SEVEN,
                                         SSEG_EIGHT,
                                         SSEG_NINE };

/// The actual values in each display
int displays[4] = {0, 0, 0, 0};

/// The currently strobed display number
int currentDisplay = 0;

/// Strobes the display.
///
/// Activates the next display and writes its value out.
void strobeDisplay()
{
  // Important we switch off the previous display first
  // before switching on the next to prevent leakage from
  // one display to another.
  SSEG_SELECT_PORT &= ~SSEG_SELECT[currentDisplay];
  setPorts();

  currentDisplay++;
  currentDisplay %= 4;

  SSEG_DATA_PORT = displays[currentDisplay];
  SSEG_SELECT_PORT |= SSEG_SELECT[currentDisplay];
  setPorts();
}

/// Writes a decimal number to the display.
///
/// The number passed to this function should be
/// 100 times larger than the number to be displayed,
/// and in the range `(-10000 , 100000)`
///
/// The decimal place will automatically be placed.
void writeNumber(int n)
{
  displays[0] = SSEG_NUMBERS[n%10];
  n/=10;
  displays[1] = SSEG_NUMBERS[n%10];
  n/=10;
  displays[2] = SSEG_NUMBERS[n%10];
  n/=10;
  displays[3] = SSEG_NUMBERS[n%10];
}
