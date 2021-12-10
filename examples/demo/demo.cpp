#include "../../MemoryLCD.h"
#include <unistd.h>
#include <cmath>
#include <iostream>

using namespace std;

  /*
   * The Raspberry Pi GPIO pins used for SPI are:
   * P1-19 (MOSI)
   * P1-21 (MISO)
   * P1-23 (CLK)
   * P1-24 (CE0)
   * P1-26 (CE1)
   */
  
// use the pin name/number, not the number based on physical header position
char SCS       = 23;      // Use any pin except the dedicated SPI SS pins?
char DISP      = 24;      // Use any non-specialised GPIO pin
char EXTCOMIN  = 25;	     // Use any non-specialised GPIO pin

MemoryLCD memLcd(SCS, DISP, EXTCOMIN, true);

unsigned int lcdWidth;
unsigned int lcdHeight;
char numRepetitions = 4;
bool toggle = true;
 
int main() {
  memLcd.clearDisplay();
  lcdWidth = memLcd.getDisplayWidth();
  lcdHeight = memLcd.getDisplayHeight();
  sleep(2);

  while(1) {
    // print sinewave
    //float increment = 360.00/lcdWidth;  // one number MUST have a decimal point here!
    float increment = 6.2831/lcdWidth;
    unsigned int sinValue = 0;
    for(float theta=0; theta<50.26; theta += 0.1256) {
    //for(unsigned int theta=0; theta<211; theta +=30) {
      for(unsigned int y=1; y<=lcdHeight; y++) {
	  for(unsigned int x=1; x<=lcdWidth; x++) {
	  sinValue = ( sin( theta+(x*increment) ) * lcdHeight/2 ) + lcdHeight/2;
	  if(sinValue >= y && y > lcdHeight/2) {
	    memLcd.writePixelToLineBuffer(x, 0);
	  }
	  if(sinValue <= y && y <= lcdHeight/2) {
	    memLcd.writePixelToLineBuffer(x, 0);
	  }
	}
	memLcd.writeLineBufferToDisplay(y);
	memLcd.clearLineBuffer();
      }
      usleep(20000);
      memLcd.clearDisplay();
    }
    
    
    // print expanding and contracting circles
    unsigned int originX = lcdWidth/2;
    unsigned int originY = lcdHeight/2;
    unsigned int expandingCircleRadius = (lcdHeight/2)*0.9;
    for(unsigned int repeat = 0; repeat < 2; repeat++) {
      for(unsigned int radius = 5; radius < expandingCircleRadius; radius++) {
	for(unsigned int y = originY - radius; y <= originY; y++) {
	  // need to calculate left and right limits of the circle
	  float theta = acos(float(abs(int(originY-y)))/float(radius));
	  theta -= 1.5708;
	  unsigned int xLength = cos(theta)*float(radius);
	  for(unsigned int x = originX - xLength; x <= originX; x++) {
	    memLcd.writePixelToLineBuffer(x, 0);
	    memLcd.writePixelToLineBuffer(originX + (originX - x), 0);
	  }
	  memLcd.writeLineBufferToDisplay(y);
	  memLcd.writeLineBufferToDisplay(originY + (originY - y));
	  memLcd.clearLineBuffer();
	}
	usleep(20000);
      }
      for(unsigned int radius = expandingCircleRadius; radius > 2; radius--) {
	for(unsigned int y = originY - radius; y <= originY; y++) {
	  // need to calculate left and right limits of the circle
	  float theta = acos(float(abs(int(originY-y)))/float(radius));
	  theta -= 1.5708;
	  unsigned int xLength = cos(theta)*float(radius);
	  for(unsigned int x = originX - xLength; x <= originX ; x++) {
	    memLcd.writePixelToLineBuffer(x, 0);
	    memLcd.writePixelToLineBuffer(originX + (originX - x), 0);
	  }
	  memLcd.writeLineBufferToDisplay(y);
	  memLcd.writeLineBufferToDisplay(originY + (originY - y));
	  memLcd.clearLineBuffer();
	}
	memLcd.writeLineBufferToDisplay(originY+radius);
	memLcd.writeLineBufferToDisplay(originY-radius);
	usleep(20000);
      }
      memLcd.clearDisplay();
      memLcd.clearLineBuffer();
    }
    
    
    // print circling circle
    numRepetitions = 4;
    unsigned int sweepRadius = (lcdHeight/2)*0.8;
    unsigned int sweepOriginX = lcdWidth/2;
    unsigned int sweepOriginY = lcdHeight/2;
    unsigned int circleRadius = 0.7*((lcdHeight/2)-sweepRadius);
    for(float rads=0; rads<6.2824*numRepetitions; rads += 0.04) {
      // calculate circle centre
      unsigned int circleOriginX = sweepOriginX + cos(rads)*sweepRadius;
      unsigned int circleOriginY = sweepOriginY + sin(rads)*sweepRadius;
      // draw circle about the centre
      for(unsigned int y = circleOriginY - circleRadius; y <= circleOriginY; y++) {
	// need to calculate left and right limits of the circle
	float theta = acos(float(std::abs(int(circleOriginY-y)))/float(circleRadius));
	theta -= 1.5708;
	unsigned int xLength = cos(theta)*float(circleRadius);
	for(unsigned int x = circleOriginX - xLength; x <= circleOriginX; x++) {
	  memLcd.writePixelToLineBuffer(x, 0);
	  memLcd.writePixelToLineBuffer(circleOriginX + (circleOriginX - x), 0);
	}
	memLcd.writeLineBufferToDisplay(y);
	memLcd.writeLineBufferToDisplay(circleOriginY + (circleOriginY - y));
	memLcd.clearLineBuffer();
      }
      usleep(15000);
      memLcd.clearDisplay();
    }
    memLcd.clearDisplay();
    memLcd.clearLineBuffer();
    
    
    // print triangles
    numRepetitions = 4;
    toggle = false;
    for(char i=0; i< numRepetitions; i++) {
      for(unsigned int y=1; y<=lcdHeight; y++) {
	for(unsigned int x=1; x<y+((lcdWidth-lcdHeight)/2); x++) {
	  memLcd.writePixelToLineBuffer(x, toggle);
	}
	memLcd.writeLineBufferToDisplay(y);
	usleep(5000);
      }
      for(unsigned int y=lcdHeight; y>0; y--) {
	for(unsigned int x=lcdWidth; x>y+((lcdWidth-lcdHeight)/2); x--) {
	  memLcd.writePixelToLineBuffer(x, toggle);
	}
	memLcd.writeLineBufferToDisplay(y);
	usleep(5000);
      }
      if(toggle)
	toggle = false;
      else
	toggle =  true;
    }
    memLcd.clearDisplay();
    memLcd.clearLineBuffer();
    
    
    // Print chequerboard patterns
    numRepetitions = 8;
    for(char i=0; i<numRepetitions; i++) {
      for(unsigned int y=1; y<=lcdHeight; y++) {
	for(unsigned int x=1; x <=lcdWidth/8; x++) {
	  if(toggle) {
	    memLcd.writeByteToLineBuffer(x, 0xFF);
	    toggle = false;
	  } else {
	    memLcd.writeByteToLineBuffer(x, 0x00);
	    toggle = true;
	  }
	}
	memLcd.writeLineBufferToDisplay(y);
	memLcd.clearLineBuffer();
	if(y%8 == 0)
	  if(toggle)
	    toggle = false;
	  else
	    toggle = true;
      }
      usleep(500000);
      if(toggle)
	toggle = false;
      else
	toggle = true;
    }
    memLcd.clearDisplay();
    memLcd.clearLineBuffer();
    
    
//     // print scrolling diagonal lines
//     numRepetitions = 32;
//     toggle = true;
//     const char lineWidth = 8;
//     for(unsigned int i=0; i<numRepetitions; i++) {
//       // work only on the mimumum screen area for a complete repeating pattern
//       for(unsigned int y=1; y<(lineWidth*2)+1; y++) {
// 	for(unsigned int x=1; x<(lineWidth*2)+1; x++) {
// 	  memLcd.writePixelToLineBuffer(x, toggle);
// 	  if((x-y+1+i)%lineWidth == 0 && x != 2*lineWidth) {
// 	    if(toggle)
// 	      toggle = false;
// 	    else
// 	      toggle = true;
// 	  }
// 	// now horizontally copy the repeating pattern to the rest of the screen
// 	}for(char j=3; i<=lcdWidth/8; j++) {
// 	  if(j%2 == 0) 
// 	    memLcd.copyByteWithinLineBuffer(2,j);
// 	  else
// 	    memLcd.copyByteWithinLineBuffer(1,j);
// 	}
// 	// now horizontally copy the repeating pattern to the rest of the screen
// 	for(unsigned int k=y; i<=lcdHeight; k+=(2*lineWidth))
// 	  memLcd.writeLineBufferToDisplay(k);
// 	memLcd.clearLineBuffer();
// 	//if((lcdWidth-y+1+i)%8 == 0) {
// 	if((y+i)%8 == 0) {
// 	  if(toggle)
// 	    toggle = false;
// 	  else
// 	    toggle = true;
// 	}
//       }
//       usleep(200000);
//       memLcd.clearDisplay();
//     }
//     memLcd.clearLineBuffer();
//     memLcd.clearDisplay();
    
    
    // print black and white alternating screens
    // demonstrates use of writeByteToLineBuffer and writeLineBufferToDisplay
    numRepetitions = 4;
    for(char i=0; i<numRepetitions; i++) {
      char colour = 0x00;
      if(i%2 == 0)
	memLcd.setLineBufferBlack();
      else
	memLcd.setLineBufferWhite();
      for(char y=1; y<lcdHeight+1; y++) {
	memLcd.writeLineBufferToDisplay(y);
	usleep(5000);
      }
    }
    memLcd.clearLineBuffer();
    memLcd.clearDisplay();
    
    
    // print horizontal line descending down the screen
    char lineThickness = 10;
    for(unsigned int i=0; i<numRepetitions; i++) {
      for(char y=1; y<lcdHeight+lineThickness+1; y++) {  // lcdHeight+10 to give the line some thickness
	int blackLine = y;
	int whiteLine = y - lineThickness;
	if(whiteLine > 0 && whiteLine < lcdHeight+1) {
	  memLcd.setLineBufferWhite();
	  memLcd.writeLineBufferToDisplay(whiteLine);
	}
	if(blackLine > 0 && blackLine < lcdHeight+1) {
	  memLcd.setLineBufferBlack();
	  memLcd.writeLineBufferToDisplay(blackLine);
	}
	usleep(5000);
      }
    }
    // make sure you clear both the buffer and display before moving on to a new sequence
    memLcd.clearLineBuffer();
    memLcd.clearDisplay();
  }
  sleep(1);
  return 0;
}