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
    }
    writeHex(0xFFFF);
    initSPI();
    // Get the WHOAMI from the compass chip
    writeHex(SPITranceiver( 0b10001111 ));
    initialWait = 5;
  }
}
