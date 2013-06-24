Author: Richard Leszczynski
email:  richard@makerdyne.com
web:	www.MakerDyne.com/memorylcds/


Basic class for Sharp Memory LCD control to accompany my Sharp Memory LCD Breakout Boards.

The class relies on refreshing the LCD with hardware PWM output on pin 9 of an Arduino Uno or Duemilanove.

HARDWARE SETUP:

Breakout Board 	--> Arduino
DISP	 	--> 8
EXTCOMIN 	--> 9
SCS	 	--> 10
SI	 	--> 11
SCLK 	 	--> 13
Vin 	 	--> 5V
GND 	 	--> GND

JUMPERS (on rear of breakout board)
JP1 = HIGH
JP2 = Set according to your model of LCD

SOFTWARE INSTALLATION:

Download the zip file from GitHub and extract it to your Arduino libraries folder.
you should end up with the following:

/path/to/ArduinoSketchbook/libraries/MemoryLCD/
/path/to/ArduinoSketchbook/libraries/MemoryLCD/MemoryLCD.h
/path/to/ArduinoSketchbook/libraries/MemoryLCD/MemoryLCD.cpp
/path/to/ArduinoSketchbook/libraries/MemoryLCD/keywords.txt
/path/to/ArduinoSketchbook/libraries/MemoryLCD/README.txt
/path/to/ArduinoSketchbook/libraries/MemoryLCD/examples/demo/demo.ino

SOFTWARE USE:

Run the demo sketch to check your breakout board is working.
To use the MemoryLCD class in your own sketch, you must include the following at the top of your sketch:

#include "MemoryLCD.h"
#include <SPI.h>