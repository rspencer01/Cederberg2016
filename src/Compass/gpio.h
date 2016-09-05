#ifndef GPIO_H_
#define GPIO_H_

#include <avr/io.h>

// The buffers for the io ports.  Used for digital
// output only.
extern unsigned char portB;
extern unsigned char portC;
extern unsigned char portD;

// Codes for pushbuttons
#define PUSHBUTTON_1 0
#define PUSHBUTTON_2 1
#define PUSHBUTTON_3 2
#define PUSHBUTTON_4 3

// Initialises the ports
void initPorts();

// Updates all the output ports.
void setPorts();

// Which pins are outputs.  See documentaion for 
// initPorts for details.
// Set PB2 output to set up SPI
#define PORTB_OUTPUT_MASK 0b10101000
#define PORTC_OUTPUT_MASK 0b11111111
#define PORTD_OUTPUT_MASK 0b11111111

// Reads a particular pushbutton
char readPushButton(int id);
#endif /* GPIO_H_ */
