#include <avr/io.h>
#include "gpio.h"
#include "utils.h"

/// Internal buffer for port C
///
/// Such internal buffers are used so that we can do things like
/// `portC |= 0xF0`.  This involves a read, and reading output
/// pins is a no-no.  Thus we store this internal state and
/// do such calculations against this variable.  Then we write
/// out all the ports (almost atomically, too)
unsigned char portC = 0;
/// Internal buffer for port D
unsigned char portD = 0;

/// Initialises the input/output of all the ports
///
/// Sets the pins to be input or output according to the following
/// port scheme.  By default the directions are set to out, if they
/// are inputs or programmers or no connect.
///
/// __Port C__
/// |Pin Number | Usage              | I/O  |
/// |:---------:|:------------------:|:----:|
/// |`0`        |                    |      |
/// |`1`        |                    |      |
/// |`2`        | `SSEG Control 1`   | `O`  |
/// |`3`        | `SSEG Control 2`   | `O`  |
/// |`4`        | `SSEG Control 3`   | `O`  |
/// |`5`        | `SSEG Control 4`   | `O`  |
/// |`6`        |                    |      |
///
/// __Port D__
/// |Pin Number | Usage              | I/O  |
/// |:---------:|:------------------:|:----:|
/// |`0`        | `SSEG 0`           | `O`  |
/// |`1`        | `SSEG 1`           | `O`  |
/// |`2`        | `SSEG 2`           | `O`  |
/// |`3`        | `SSEG 3`           | `O`  |
/// |`4`        | `SSEG 4`           | `O`  |
/// |`5`        | `SSEG 5`           | `O`  |
/// |`6`        | `SSEG 6`           | `O`  |
/// |`7`        | `SSEG 7`           | `O`  |
void initPorts()
{
  DDRC = 0b11111111;
  DDRD = 0b11111111;

  setPorts();
}

/// Sets all the ports to be the values stored in the buffers.
///
/// We use buffers for the ports as operations such as `PORTD^=0x02` are
/// not well defined (it implies a read of an output pin, which may be
/// sensitive on the circuitry).
///
/// In addition, this allows us to keep the pull up resistors on the
/// push buttons by masking the correct ports.
void setPorts()
{
  PORTD = portD;
  PORTC = portC;
}
