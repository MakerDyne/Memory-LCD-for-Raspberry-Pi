#ifndef _ARDUINO_TEST_LIB_H
#define _ARDUINO_TEST_LIB_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <SPI.h>

// Memory LCD pixel dimensions - ALTER ACCORDING TO YOUR PARTICULAR LCD MODEL
#define LCDWIDTH		(400)
#define LCDHEIGHT		(240)

// Delay constants for LCD timing   // (Datasheet values)
#define PWRUP_DISP_DELAY	40  // (>30us)
#define PWRUP_EXTCOMIN_DELAY	40  // (>30us)
#define SCS_HIGH_DELAY		3   // (>3us)
#define SCS_LOW_DELAY		1   // (>1us)
#define INTERFRAME_DELAY	1   // (>1us)

class MemoryLCD {
  public:
    MemoryLCD(byte DISPpin, byte SCSpin, byte SIpin, byte SCLKpin, boolean useEXTCOMIN);
    void begin();
    // Write data direct to screen
    void writeLineToDisplay(byte lineNumber, byte *line);
    void writeMultipleLinesToDisplay(byte lineNumber, byte numLines, byte *lines);
    // Write data to line buffer
    void writePixelToLineBuffer(int pixNumber, boolean isWhite);
    void writeByteToLineBuffer(byte byteNumber, byte byteToWrite);
    void copyByteWithinLineBuffer(byte sourceByte, byte destinationByte);
    void setLineBufferBlack(void);
    void setLineBufferWhite(void);
    // write data from line buffer to screen
    void writeLineBufferToDisplay(byte lineNumber);
    void writeLineBufferToDisplayRepeatedly(byte lineNumber, byte numLines);
    // clear functions
    void clearLineBuffer();
    void clearDisplay();
    // turn display on/off
    void turnOff();
    void turnOn();
    // return display parameters
    int getDisplayWidth();
    int getDisplayHeight();
    // software VCOM control - NOT YET PROPERLY IMPLEMENTED
    void softToggleVCOM();
    // LED is for testing only
    void toggleLED(void);
  private:
    byte commandByte;
    byte vcomByte;
    byte DISP;
    byte SCS;
    byte SI;
    byte SCLK;
    byte EXTCOMIN;
    boolean enablePWM;
    byte lineBuffer[LCDWIDTH/8];
    // LED is for testing only
    byte LED;
    boolean ledState;
};

#endif