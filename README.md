Cederberg Adventure 2016 Electronics Base Project
=================================================

This repository contains the code for the compass project built at the 2016
Cederberg Adventure.

Operation
---------

The operation of the compass is as follows.

 * Pressing any button will wake the device for 30s/keep it alive for 30s more
 * The left most button toggles between the major modes: compass, spirit
   level and off
 * The second from left button toggles between compass vertical and horizontal
   modes
 * The second from right button toggles between (0,360) and (-180,180) degrees
   mode
 * The rightmost button calibrates the compass

Calibration
-----------

The compass must be calibrated before use.  To calibrate, press the calibrate
button.  The text `CALb` will show, followed by `HZtL`.  As the countdown counts
from 8 to 1, turn a full circle (ie 45 degrees per second), holding the compass
horizontal.  The word `VErt` will then show.  Turn a full circle as before,
now holding the compass out at arm's length, display facing you.

Development
-----------

The project's brain is an Atmel [ATMEGA88PA](http://www.atmel.com/Images/Atmel-8271-8-bit-AVR-Microcontroller-ATmega48A-48PA-88A-88PA-168A-168PA-328-328P_datasheet_Complete.pdf) microchip.  The codebase is in C and compiled with the `Makefile` included
in `src/Compass`.  Installation is performed with a JTAGII.
