Fuses
=====

** _WARNING:_ The `RSTDISBL` fuse is evil.  Do not set it under any circumstances. **
** _WARNING:_ Fuse bits are reversed! **

To find hex values for fuse settings go [here](http://www.engbedded.com/fusecalc/).  Or look it up in the datasheet.  But the calculator is easier.

Debug Wire
==========

Debug wire is a mode that atmegas can be put in to allow certain debugging things.  In this mode, fuses cannot be set.

Setting debugwire is as simple as setting the `DWEN` fuse:

    sudo avrdude -c jtag2isp -p m88p -U hfuse:0x9f:m

Unsetting debugWire is more tricky.  I have not succesfully done this yet (semibricked a chip via the `RSTDISBLE` fuse (see above) so am staying in debugWire mode).  However, this command should reset it, if run twice

    sudo avrdude -c jtag2isp -p m88p -U hfuse:0xdf:m

You need to run it twice as the first time the JTAG gets the thing into a temporary no debugWire mode, and the second time it burns the fuse to disable debugWire.

Programming
===========

As far as I can see there are two ways to program the chip: via debugWire and via ISP

ISP Programming
---------------

To program the flash do

    sudo avrdude -c jtag2isp -p m88p -U flash:w:<FILE>:i

where the file is stored in hex mode, ie like

    :10000000CF93DF9300D000D0CDB7DEB71A8219822C
    :100010001C821B820DC02B813C8189819A818227A1
    :1000200093279C838B8389819A8101969A83898304
    ...

Debug Wire Programming
----------------------

To program do

    sudo avarice -2 -jusb -w --program --file <FILE> :4242

Where the file is a compiled `.o` or `.elf` file. This will also begin a debug session on the chip.

Debugging
=========

Install the code via the debug wire programming.  Then start `avr-gdb` on the object file, and as a first command set `target remote localhost:4242`.  Then use `gdb` normally.

USB Issues
==========

TODO: Get this to work (reference)[http://www.homebuilthardware.com/index.php/avr/linux-avrdragon-tutorial-1/]

To stop requiring sudo rights for the USB connection to the JTAG, add a file

    /etc/udev/rules.d/50-avrjtagice.rules

and place in it the line

    SUBSYSTEM=="usb", SYSFS{idVendor}=="03eb", SYSFS{idProduct}=="2103", GROUP="users", MODE="0666"

The values for `idVendor` and `idProduct` can be read of `lsusb`.  Then run `sudo /etc/init.d/udev restart` to flush settings.

References
=========
 * http://winavr.sourceforge.net/AVR-GDB_and_AVaRICE_Guide.pdf
