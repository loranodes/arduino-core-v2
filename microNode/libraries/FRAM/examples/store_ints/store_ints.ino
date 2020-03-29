#include <fram.h>

/*
 * FRAM can be used to perminantly store values through power cycles
 * The fram fitted can store 2048 bytes
 * Addresses 0 -> 2047
 */

void setup() {
  FRAMinit();

  Serial.begin(9600);
}

void loop() {
  uint16_t i = 0;

  for(i = 0; i < 2048; i++){
    // FRAMwrite(address, value tostore)
    FRAMwrite(i, i);
  }

  Serial.println("Values written to FRAM");
  delay(2000);

  for(i = 0; i < 2047; i++){
    // FRAMread(addess)
    uint8_t value = FRAMread(i);

    Serial.print("Address ");
    Serial.print(i);
    Serial.print(" = ");
    Serial.println(value);
  }

  Serial.println("All addresses read");

  delay(5000);
  

}
