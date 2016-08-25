#include <avr/io.h>
#include "utils.h"
#include "gpio.h"
#include "sseg.h"
#include "timers.h"

int main(void)
{
  initMicro();
  initPorts();
  writeNumber(1337);
  initTimers();
  while (initialWait)
  {
    __asm__ __volatile__ (" wdr ");
  }
  while(1)
  {
    __asm__ __volatile__ (" wdr ");
  }
}
