#ifndef TIMERS_H_
#define TIMERS_H_

// Scalars to change 4ms to 1 second via 40ms
#define INI_4MS_20MS 5
#define INI_20MS_1S 50

// Sets up all the timer stuff
void initTimers();

extern int initialWait;

#endif /* TIMERS_H_ */
