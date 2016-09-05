#include <avr/io.h>
#include "utils.h"
#include "gpio.h"
#include "sseg.h"
#include "timers.h"
#include "spi.h"

int main(void)
{
  initMicro();
  initPorts();
  initTimers();
  writeHex(0xFFFF);
  while(1)
  {
    while (initialWait)
    {
      __asm__ __volatile__ (" wdr ");
      if (readPushButton(PUSHBUTTON_1))
        writeHex(0x0001);
      else
      if (readPushButton(PUSHBUTTON_2))
        writeHex(0x0010);
      else
      if (readPushButton(PUSHBUTTON_3))
        writeHex(0x0100);
      else
      if (readPushButton(PUSHBUTTON_4))
        writeHex(0x1000);
      else
        writeHex(0x0000);
    }
    writeHex(0xFFFF);
    initSPI();
    // Get the WHOAMI from the compass chip
    writeHex(SPITranceiver( 0b10001111 ));
    initialWait = 5;
  }
}
