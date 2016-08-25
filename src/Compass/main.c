#include <avr/io.h>
#include "utils.h"
#include "gpio.h"
#include "sseg.h"
#include "timers.h"

int main(void)
{
  initMicro();
  initPorts();
  initTimers();
  int i;
  writeHex(sizeof(i));
  while (initialWait)
  {
    __asm__ __volatile__ (" wdr ");
  }
  writeHex(0xed12);
  while(1)
  {
    __asm__ __volatile__ (" wdr ");
  }
}
