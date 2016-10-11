AVR Howto
=========

Fuses
-----

** _WARNING:_ The `RSTDISBL` fuse is evil.  Do not set it under any circumstances. **

** _WARNING:_ Fuse bits are reversed to what you think they are! **

To find hex values for fuse settings go [here](http://www.engbedded.com/fusecalc/).  Or look it up in the datasheet.  But the calculator is easier.

Debug Wire
----------

Debug wire is a mode that atmegas can be put in to allow certain debugging things.  In this mode, however, fuses cannot be set.

Setting debugwire is as simple as setting the `DWEN` fuse:

    sudo avrdude -c jtag2isp -p m88p -U hfuse:0x9f:m

Unsetting debugWire is more tricky.  I have not succesfully done this yet (semibricked a chip via the `RSTDISBLE` fuse (see above) so am staying in debugWire mode).  However, this command should reset it, if run twice

    sudo avrdude -c jtag2isp -p m88p -U hfuse:0xdf:m

You need to run it twice as the first time the JTAG gets the thing into a temporary no debugWire mode, and the second time it burns the fuse to disable debugWire.

Programming
-----------

As far as I can see there are two ways to program the chip: via debugWire and via ISP

### ISP Programming

To program the flash do

    sudo avrdude -c jtag2isp -p m88p -U flash:w:<FILE>:i

where the file is stored in hex mode, ie like

    :10000000CF93DF9300D000D0CDB7DEB71A8219822C
    :100010001C821B820DC02B813C8189819A818227A1
    :1000200093279C838B8389819A8101969A83898304
    ...

### Debug Wire Programming

To program do

    sudo avarice -2 -jusb -w --program --file <FILE> :4242

Where the file is a compiled `.o` or `.elf` file. This will also begin a debug session on the chip.

Debugging
---------

Install the code via the debug wire programming.  Then start `avr-gdb` on the object file, and as a first command set `target remote localhost:4242`.  Then use `gdb` normally.

USB Issues
----------

TODO: Get this to work (reference)[http://www.homebuilthardware.com/index.php/avr/linux-avrdragon-tutorial-1/]

To stop requiring sudo rights for the USB connection to the JTAG, add a file

    /etc/udev/rules.d/50-avrjtagice.rules

and place in it the line

    SUBSYSTEM=="usb", SYSFS{idVendor}=="03eb", SYSFS{idProduct}=="2103", GROUP="users", MODE="0666"

The values for `idVendor` and `idProduct` can be read of `lsusb`.  Then run `sudo /etc/init.d/udev restart` to flush settings.

Hardware
--------

In this circuit, the programmer header should map as follows:

|Pin Number | Programmer Name | Circuit Name          |
|----------:|-----------------|-----------------------|
|1          | TCK             | SCK                   |
|2          | GND             | n/c                   |
|3          | TDO             | ATmega pin 18 (MISO)  |
|4          | VTref           | VCC (3.3V)            |
|5          | TMS             | n/c                   |
|6          | nSRST           | ATmega pin 1 (/RESET) |
|7          | Vsupply         | n/c                   |
|8          | nTRST           | n/c                   |
|9          | TDI             | ATmega pin 17 (MOSI)  |
|10         | GND             | GND                   |



References
----------
 * [Basic AVRDUDE tutorial](http://www.ladyada.net/learn/avr/avrdude.html)
 * [Fuse calculator](http://www.engbedded.com/fusecalc/)

 * http://winavr.sourceforge.net/AVR-GDB_and_AVaRICE_Guide.pdf
