#include <stdint.h>
#include <moisture.h>

#define COUNT_NUMBER 1000

#define CAP_DRIVE_PIN 10


void setup(){
  Serial.begin(9600);
  while(!Serial);
  init_AC(CAP_DRIVE_PIN);

}

void loop(){

 uint32_t reading = takeSoilReading(COUNT_NUMBER);

 Serial.println(reading);

 delay(10);
}

