#ifndef UTILS_H_
#define UTILS_H_

#define abs(_x) (((_x) > 0) ? (_x) : -(_x))
#define min(_x,_y) (((_x) > (_y)) ? (_y) : (_x))

// Initialises all the flags for the watchdog and interrupts etc
void initMicro();

// Linear interpolation of a function
int flerp(int a, int b,int* v, int n);

// Caluclate number of digits
int digits(int n);

// An integer atan2 function (in degrees)
int iatan2(int x, int y);

#endif /* UTILS_H_ */
