Author: Richard Leszczynski
email:  richard@makerdyne.com
web:	www.MakerDyne.com/memorylcds/


Basic class for Sharp Memory LCD control to accompany my Sharp Memory LCD Breakout Boards.

IMPORTANT INFO:

The Sharp Memory LCDs require their screens be refreshed at least once a second. The signal telling
the display to refresh can either be supplied with a software command over SPI (set JP1 LOW on the
rear of your breakout board) or with a square wave signal supplied to the EXTCOMIN pin (set JP1 HIGH).

The class and its demos rely on the hardware method of display refreshing at present as proper
software control has not been implemented. The demo outputs a square wave at 2Hz on  pin "GPIO 25"
of the Raspberry Pi. See the diagram (with the green background) of the GPIO pins for the pin names:

http://elinux.org/RPi_Low-level_peripherals#General_Purpose_Input.2FOutput_.28GPIO.29

A more reliable way of constantly providing a hardware EXTCOMIN signal to the Memory LCD while
writing your code would be to connect the EXTCOMIN pin on the breakout board to the output
of a 555 timer configured in astable mode to output a frequency between 2 Hz and 60 Hz. This will
ensure that an EXTCOMIN signal always gets through to the display regardless of whether you're
currently running your Memory LCD code or not.

See here for how to wire a 555 in astable mode and for guidance on how to choose R & C values.

http://www.ohmslawcalculator.com/555_astable.php

I am using:
C = 20 nF
R1 = 2.7 MOhm
R2 = 10 MOhm

You need a frequency of between 2 Hz and 60 Hz and a duty cycle of between 50 and 70%.
Other combinations of R & C values can provide that if you don't have the parts I've suggested.

If you want to keep the Memory LCD connected to the Raspberry Pi for long periods without providing
a square wave on EXTCOMIN, you should clear the display and turn it off by pulling the breakout
board's DISP pin LOW.

A note on connecting the Memory LCD breakout board to the Pi: I have had a powered-on Pi turn off
upon inserting a breakout board into a breadboard that already had the necessary connections made
with the Pi's GPIOs. 

HARDWARE SETUP:

Again, refer to http://elinux.org/RPi_Low-level_peripherals#General_Purpose_Input.2FOutput_.28GPIO.29

You must make the following connections between the Pi's GPIO pins and the breakout board header to
make the demo run. You can alter the GPIO pins used in your own code for SCS, DIS and, EXTCOMIN by
changing the pins given when the Memory LCD constructor is called. NB: SI and SCLK cannot be changed.

Breakout Board 	--> Raspberry Pi
DISP	 	--> "GPIO 24"  [can be changed]
EXTCOMIN 	--> "GPIO 25"  [can be changed] (or connect  EXCOMIN to the output of a 555 timer)
SCS	 	--> "GPIO 23"  [can be changed]
SI	 	--> "GPIO 10 (MOSI)" [Must be on this pin, is dedicated bcm2835 hardware SPI pin]
SCLK 	 	--> "GPIO 11 (SCLK)" [Must be on this pin, is dedicated bcm2835 hardware SPI pin]
Vin 	 	--> 5V or 3V3 depending on minimum Vin of your Memory LCD model [can be changed]
GND 	 	--> GND [can be changed, multiple GND connections on the GPIO header]

JUMPERS (on rear of breakout board)
JP1 = HIGH
JP2 = Set according to your model of LCD

SOFTWARE INSTALLATION:

Download the zip file from GitHub and extract it a directory of your choice within your HOME directory.

If you use the commands:

cd~
wget https://github.com/MakerDyne/Memory-LCD-for-Raspberry-Pi/archive/master.zip
unzip master.zip

you should end up with the following:

/home/USER/Memory-LCD-for-Raspberry-Pi-master/
/home/USER/Memory-LCD-for-Raspberry-Pi-master/MemoryLCD.h
/home/USER/Memory-LCD-for-Raspberry-Pi-master/MemoryLCD.cpp
/home/USER/Memory-LCD-for-Raspberry-Pi-master/README.txt
/home/USER/Memory-LCD-for-Raspberry-Pi-master/examples/demo/demo.cpp

Now, in addition to the Memory LCD library for Raspberry Pi from my GitHub page, you also need to
install the library for the Pi's Broadcom BCM 2835 chip that allows control of the GPIO pins.

The library can be downloaded from http://www.airspayce.com/mikem/bcm2835/index.html

Use the following commands to download and install it
(NB: version number may change, check website for latest version before entering these commands):

cd ~
wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.25.tar.gz
tar zxvf bcm2835-1.25.tar.gz
cd bcm2835-1.25
./configure
make
sudo make check
sudo make install

Now, before you compile and run the Memory LCD demo program, you must first open the MemoryLCD.h file
and edit the LCDWIDTH and LCDHEIGHT defines such that they match the pixel dimensions of your
particular LCD:

#define LCDWIDTH	96
#define LCDHEIGHT	96

must be changed to match the dimensions of your particular model of Memory LCD in pixels.

(The boards I am selling are either (WxH) 96x96 pixels, 128x128 pixels, or 400x240 pixels.)

If everything has gone to plan, change to the demo directory within the
Memory-LCD-for-Raspberry-Pi-master/ directory and compile, link and run the demo program

cd ~/Memory-LCD-for-Raspberry-Pi-master/examples/demo/
g++ -c -g -Wall -funsigned-char demo.cpp ../../MemoryLCD.cpp
g++ -o demo demo.o MemoryLCD.o  -l bcm2835 -l pthread
sudo ./demo

If there were no compile or linking errors, and the pi is connected to the Memory LCD breakout board
as described above, you should see the demo start with a side scrolling sinewave.

Note the use of sudo to run the demo - any program wanting access to the GPIOs must run as root.