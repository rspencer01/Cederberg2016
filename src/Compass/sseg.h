#ifndef SSEG_H_
#define SSEG_H_

// The seven segment displays are set out as follows:
//
//        0
//      5   1
//        6
//      4   2
//        3   7
//
// We only show one at a time.  The rest are strobed
// one after another fast enough to appear static to
// the human eye.


// The port on which the sseg select pins reside
#define SSEG_SELECT_PORT portC

// The port on which the sseg drain pins reside
#define SSEG_DATA_PORT portD

// A mask for which pins are sseg select pins
#define SSEG_SELECT_MASK 0b00111100


// Representation of the numbers in the above sseg display
#define SSEG_ZERO    0b11000000
#define SSEG_ONE     0b11111001
#define SSEG_TWO     0b10100100
#define SSEG_THREE   0b10110000
#define SSEG_FOUR    0b10011001
#define SSEG_FIVE    0b10010010
#define SSEG_SIX     0b10000010
#define SSEG_SEVEN   0b11111000
#define SSEG_EIGHT   0b10000000
#define SSEG_NINE    0b10010000

// Symbols
#define SSEG_DECIMAL 0b01111111
#define SSEG_MINUS   0b10111111

// Representation of some letters
#define SSEG_A 0b10001000
#define SSEG_B 0b10000011
#define SSEG_C 0b11000110
#define SSEG_c 0b10100111
#define SSEG_D 0b10100001
#define SSEG_E 0b10000110
#define SSEG_F 0b10001110
#define SSEG_G 0b11000010
#define SSEG_H 0b10001001
#define SSEG_I 0b11001111
#define SSEG_L 0b11000111
#define SSEG_N 0b10101011
#define SSEG_O 0b11000000
#define SSEG_Q 0b10011000
#define SSEG_R 0b10101111
#define SSEG_S 0b10010010
#define SSEG_T 0b10000111
#define SSEG_U 0b11000001
#define SSEG_V 0b11000001
#define SSEG_Z 0b10100100

// Useful phrases
extern const int SSEG_CALB [];
extern const int SSEG_DONE [];
extern const int SSEG_HZTL [];
extern const int SSEG_VERT [];
extern const int SSEG_BE [];
extern const int SSEG_FAST [];
extern const int SSEG_GOOD [];
extern const int SSEG_NICE [];
extern const int SSEG_EH [];
extern const int SSEG_BAD [];


// Performs the strobing
void strobeDisplay();

// Write a numeral
void writeHex(unsigned int n);
void writeInt(int n);
void writeMessage(const int message[]);
void writePrefix(int character);

// Clear the display
void writeClear();
void blankDisplay();

#endif
