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
  initSPI();
  while(1)
  {
    while (initialWait)
    {
      __asm__ __volatile__ (" wdr ");
    }
  }
}
