#ifndef GPIO_H_
#define GPIO_H_

#include <avr/io.h>

// The buffers for the io ports.  Used for digital
// output only.
extern unsigned char portC;
extern unsigned char portD;

// Initialises the ports
void initPorts();

// Updates all the output ports.
void setPorts();

#endif /* GPIO_H_ */
