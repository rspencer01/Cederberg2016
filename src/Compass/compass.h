#ifndef COMPASS_H
#define COMPASS_H

void enableCompass();

void calibrate();

unsigned char readRegister(unsigned char address);

int readCompass();
int readCompassX();
int readCompassY();

void disableCompass();
#endif
