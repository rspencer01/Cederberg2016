#ifndef COMPASS_H
#define COMPASS_H

#define COMPASS_VERTICAL 0
#define COMPASS_HORIZONTAL 1

void enableCompass();

void calibrate();

unsigned char readRegister(unsigned char address);

int readCompass(int);
int readCompassX();
int readCompassY();
int readCompassZ();

int readLevel();

char whoami();

void disableCompass();
#endif
