#ifndef _MEMORY_LCD_LIB_H
#define _MEMORY_LCD_LIB_H

#include <bcm2835.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>

// Memory LCD pixel dimensions - ALTER ACCORDING TO YOUR PARTICULAR LCD MODEL
#define LCDWIDTH		(96)
#define LCDHEIGHT		(96)

// Delay constants for LCD timing   // (Datasheet values)
#define PWRUP_DISP_DELAY	40  // (>30us)
#define PWRUP_EXTCOMIN_DELAY	40  // (>30us)
#define SCS_HIGH_DELAY		3   // (>3us)
#define SCS_LOW_DELAY		1   // (>1us)
#define INTERFRAME_DELAY	1   // (>1us)

class MemoryLCD {
  public:
    MemoryLCD(char SCSpin, char DISPpin, char EXTCOMINpin, bool useEXTCOMIN);
    ~MemoryLCD();
    // Write data direct to display
    void writeLineToDisplay(char lineNumber, char *line);
    void writeMultipleLinesToDisplay(char lineNumber, char numLines, char *lines);
    // Write data to line buffer
    void writePixelToLineBuffer(unsigned int pixel, bool isWhite);
    void writeByteToLineBuffer(char byteNumber, char byteToWrite);
    void copyByteWithinLineBuffer(char sourceByte, char destinationByte);
    void setLineBufferBlack();
    void setLineBufferWhite();
    // write data from line buffer to display
    void writeLineBufferToDisplay(char lineNumber);
    void writeLineBufferToDisplayRepeatedly(char lineNumber, char numLines);
    // Write data to frame buffer
    void writePixelToFrameBuffer(unsigned int pixel, char lineNumber, bool isWhite);
    void writeByteToFrameBuffer(char byteNumber, char lineNumber, char byteToWrite);
    void setFrameBufferBlack();
    void setFrameBufferWhite();
    // write data from frame buffer to display
    void writeFrameBufferToDisplay();
    // clear functions
    void clearLineBuffer();
    void clearFrameBuffer();
    void clearDisplay();
    // turn display on/off
    void turnOff();
    void turnOn();
    // return display parameters
    unsigned int getDisplayWidth();
    unsigned int getDisplayHeight();
    // software VCOM control - NOT YET PROPERLY IMPLEMENTED
    void softToggleVCOM();
  private:
    char commandByte;
    char vcomByte;
    char clearByte;
    char paddingByte;
    char DISP;
    char SCS;
    char SI;
    char SCLK;
    char EXTCOMIN;
    bool enablePWM;
    char lineBuffer[LCDWIDTH/8];
    char frameBuffer[LCDWIDTH*LCDHEIGHT/8];
    static void *hardToggleVCOM(void *arg);
    char reverseByte(char b);
};

#endif