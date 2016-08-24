#include <avr/io.h>
#include "utils.h"
#include "gpio.h"
#include "sseg.h"

int main(void)
{
  initMicro();
  initPorts();
  writeNumber(1337);
  while(1)
  {
    strobeDisplay();
    __asm__ __volatile__ (" wdr ");
  }
}
