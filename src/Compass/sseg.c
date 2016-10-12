#include "sseg.h"
#include "gpio.h"
#include "utils.h"
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
/// An array consisting of some letters represented in seven segment displays
const unsigned char SSEG_LETTERS[6] = { SSEG_A,
                                        SSEG_B,
                                        SSEG_C,
                                        SSEG_D,
                                        SSEG_E,
                                        SSEG_F};

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

/// Writes a hex number to the display.
void writeHex(unsigned int n)
{
  int i = 0;
  for (;i<4;i++)
  {
    if (n%0x10 < 10)
      displays[i] = SSEG_NUMBERS[n%0x10];
    else
      displays[i] = SSEG_LETTERS[(n%0x10)-10];
    n/=0x10;
  }
}

/// Writes a int number to the display.
///
/// Can handle all integers in the range `-999 <= x <= 9999`
void writeInt(int n)
{
  writeClear();
  int d = digits(n);
  if (d > 4)
  {
    writeHex(0xFFFF);
    return;
  }
  if (n < 0)
  {
    if (d>3)
    {
      writeHex(0xEEEE);
      return;
    }
    displays[d] = SSEG_MINUS;
    n = -n;
  }
  int i = 0;
  for (; i < 4; i++)
  {
    if (i < d)
      displays[i] = SSEG_NUMBERS[n%10];
    n/=10;
  } 
}

/// Sets all the displays to be black
void writeClear()
{
  displays[0] = displays[1] = displays[2] = displays[3] = 0xFF;
}

/// Switches off the displays via the control lines
void blankDisplay()
{
  SSEG_SELECT_PORT &= ~ SSEG_SELECT_MASK;
}
