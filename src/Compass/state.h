#ifndef STATE_H_
#define STATE_H_

#define STATE_COMPASS_360 0x00
#define STATE_COMPASS_180 0x01
#define STATE_COMPASS_VERT_180 0x02
#define STATE_COMPASS_VERT_360 0x03
#define STATE_SPIRIT_LEVEL 0x04

#define STATE_COMPASS_MODE_TOGGLE 0x01
#define STATE_COMPASS_ANGLE_TOGGLE 0x02
#define STATE_SPIRIT_TOGGLE 0x04

extern int state;

extern int displayCountdown;

#endif
