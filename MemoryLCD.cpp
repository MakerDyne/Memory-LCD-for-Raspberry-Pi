#include "MemoryLCD.h"
#include <SPI.h>

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

MemoryLCD::MemoryLCD(byte DISPpin, byte SCSpin, byte SIpin, byte SCLKpin, boolean useEXTCOMIN) {
  // Initialise private vars (constructor args)
  // Set DISPpin = 255, when you call the constructor if you want to save a pin 
  // and not have hardware control of _DISP
  DISP = DISPpin;
  SCS  = SCSpin;
  SI   = SIpin;
  SCLK = SCLKpin;
  EXTCOMIN = 9;
  enablePWM = useEXTCOMIN;
  // initialise private vars (others)
  commandByte = 0b10000000;
  vcomByte    = 0b01000000;
  
  // Delay for MemoryLCD to reach 5V
  delayMicroseconds(800);  // minimum 750us
}


void MemoryLCD::begin(void) {
  // setup timer for EXTCOMIN signal
  if(enablePWM) {
    // (setting these AVR registers can't be done in the constructor)
    // setup 16-bit timer on Arduino Pin 9 for DUEMILANOVE & UNO
    // TODO: set registers correctly for LEONARDO
    TCCR1A = 0;
    TCCR1B = 0;
    TCCR1C = 0;
    TCCR1A |= ((1 << COM1A0)| (0 << COM1A1)); // Set Compare Output to toggle mode
    TCCR1A |= ((0 << WGM10) | (0 << WGM11));  // Set CTC mode with TOP = OCR1A
    TCCR1B |= ((1 << WGM12) | (0 << WGM13)); 
    TCCR1B |= ((1 << CS10) | (1 << CS11) | (0 << CS12)); // Set clock prescaler to 64
    OCR1A = 62500;	// Set output compare value is 62500 for 2Hz (prescaler 64)
  } else {
    // TODO: setup timer driven interrupt instead?
  }
  
  // setup SPI
  // Datasheet says SPI clock must have <1MHz frequency (SPI_CLOCK_DIV16)
  // but it may work up to 4MHz (SPI_CLOCK_DIV4, SPI_CLOCK_DIV8)
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);  // remember to change to LSBFIRST for the address bytes!
  SPI.setClockDivider(SPI_CLOCK_DIV16);	// this is the conservative setting
  SPI.setDataMode(SPI_MODE0);

  // Set pin modes
  pinMode(SCS, OUTPUT);
  pinMode(SI, OUTPUT);
  pinMode(SCLK, OUTPUT);
  digitalWrite(SCS, LOW);
  digitalWrite(SI, LOW);
  digitalWrite(SCLK, LOW);
  if(DISP != 255) {
    pinMode(DISP, OUTPUT);
    digitalWrite(DISP, LOW);
  }
  if(enablePWM) {
    pinMode(EXTCOMIN, OUTPUT);
    digitalWrite(EXTCOMIN, LOW);
  }
  
  // Memory LCD startup sequence  
  digitalWrite(DISP, HIGH);
  delayMicroseconds(PWRUP_DISP_DELAY);

  pinMode(EXTCOMIN, OUTPUT);
  delayMicroseconds(PWRUP_EXTCOMIN_DELAY);
  
  clearLineBuffer();

  // setup LED pin, NB: LED is for testing only
  LED = 7;
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
}


void MemoryLCD::writeLineToDisplay(byte lineNumber, byte *line) {
  writeMultipleLinesToDisplay(lineNumber, 1, line);
}


void MemoryLCD::writeMultipleLinesToDisplay(byte lineNumber, byte numLines, byte *lines) {
  // this implementation writes multiple lines that are CONSECUTIVE (although they don't 
  // have to be, as an address is given for every line, not just the first in the sequence)
  // data for all lines should be stored in a single array
  byte * linePtr = lines;
  SPI.setBitOrder(MSBFIRST);
  digitalWrite(SCS, HIGH);
  delayMicroseconds(SCS_HIGH_DELAY);
  SPI.transfer(0b10000000);
  for(byte x=0; x<numLines; x++) {
    SPI.setBitOrder(LSBFIRST);	// only the line number goes LSBFIRST for some reason
    SPI.transfer(lineNumber++);
    SPI.setBitOrder(MSBFIRST);
   for(byte y=0; y<LCDWIDTH/8; y++) {
     SPI.transfer(*(linePtr++));
   }
    SPI.transfer(0x00);
  }
  SPI.transfer(0x00);  // trailing padding bytes
  delayMicroseconds(SCS_LOW_DELAY);
  digitalWrite(SCS, LOW);
  delayMicroseconds(INTERFRAME_DELAY);
}


void MemoryLCD::writePixelToLineBuffer(int pixel, boolean isWhite) {
// pixel location expected in the fn args follows the scheme defined in the datasheet.
// NB: the datasheet defines pixel addresses starting from 1, NOT 0
  if((pixel <= LCDWIDTH) && (pixel != 0)) {
    pixel = pixel - 1;
    bitWrite(lineBuffer[pixel/8], 7 - pixel%8, isWhite);
  }  
}


void MemoryLCD::writeByteToLineBuffer(byte byteNumber, byte byteToWrite) {
// byte location expected in the fn args has been extrapolated from the pixel location
// format (see above), so bytes go from 1 to LCDWIDTH/8, not from 0
  if(byteNumber <= LCDWIDTH/8) {
    byteNumber -= 1;
    lineBuffer[byteNumber] = byteToWrite;
  } 
}


void MemoryLCD::copyByteWithinLineBuffer(byte sourceByte, byte destinationByte) {
  if(sourceByte <= LCDWIDTH/8 && destinationByte <= LCDWIDTH/8) {
    sourceByte -= 1;
    destinationByte -= 1;
    lineBuffer[destinationByte] = lineBuffer[sourceByte];
  } 
}

void MemoryLCD::setLineBufferBlack(void) {
  for(byte i=0; i<LCDWIDTH/8; i++) {
    lineBuffer[i] = 0x00;
  }
}
void MemoryLCD::setLineBufferWhite(void) {
  for(byte i=0; i<LCDWIDTH/8; i++) {
    lineBuffer[i] = 0xFF;
  }
}

void MemoryLCD::writeLineBufferToDisplay(byte lineNumber) {
  writeMultipleLinesToDisplay(lineNumber, 1, lineBuffer);
}


void MemoryLCD::writeLineBufferToDisplayRepeatedly(byte lineNumber, byte numLines) {
  writeMultipleLinesToDisplay(lineNumber, numLines, lineBuffer);
}


void MemoryLCD::clearLineBuffer() {
  setLineBufferWhite();
}


void MemoryLCD::clearDisplay() {
  SPI.setBitOrder(MSBFIRST);
  digitalWrite(SCS, HIGH);
  delayMicroseconds(SCS_HIGH_DELAY);
  SPI.transfer(0b00100000);
  SPI.transfer(0x00);
  delayMicroseconds(SCS_LOW_DELAY);
  digitalWrite(SCS, LOW);
  delayMicroseconds(INTERFRAME_DELAY);
}


// won't work if DISP pin is not used
void MemoryLCD::turnOff() {
  if(DISP != 255)
    digitalWrite(DISP, HIGH);
}


// won't work if DISP pin is not used
void MemoryLCD::turnOn() {
  if(DISP != 255)
    digitalWrite(DISP, LOW);
}


int MemoryLCD::getDisplayWidth() {
  return LCDWIDTH;
}


int MemoryLCD::getDisplayHeight() {
  return LCDHEIGHT;
}
    

void MemoryLCD::softToggleVCOM() {
  SPI.setBitOrder(MSBFIRST);
  vcomByte ^= (0b01000000);
  digitalWrite(SCS, HIGH);
  delayMicroseconds(SCS_HIGH_DELAY);
  SPI.transfer(vcomByte);
  SPI.transfer(0x00);
  delayMicroseconds(SCS_LOW_DELAY);
  digitalWrite(SCS, LOW);
  delayMicroseconds(10);
}


void MemoryLCD::toggleLED(void) {
  if(ledState) {
    digitalWrite(LED, LOW);
    ledState = false;
  } else {
    digitalWrite(LED, HIGH);
    ledState = true;
  }
}