#include <led.h>

/* 
 * Available colours
 * OFF
 * BLUE     RED
 * GREEN    PINK
 * CYAN     WHITE
 * 
 */

void setup() {
  //Initilise the RGB LED pins
  RGBinit();
}

void loop() {
  //RGBset , used to set the colour of the LED
  RGBset(OFF);
  delay(500);
  RGBset(BLUE);
  delay(500);
  RGBset(GREEN);
  delay(500);
  RGBset(CYAN);
  delay(500);
  RGBset(RED);
  delay(500);
  RGBset(PINK);
  delay(500);
  RGBset(WHITE);
  delay(500);
  
}
