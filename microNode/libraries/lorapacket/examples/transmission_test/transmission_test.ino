#include <LoRa.h>
#include "lorapacket.h"

int UpLinkCounter = 1;

uint8_t AppSKey[] = { 0xF2, 0x06, 0x9B, 0x04, 0x12, 0x20, 0xD7, 0x88, 0xB5, 0x6E, 0x38, 0xF7, 0xD2, 0xAE, 0x68, 0x9A };
uint8_t NwkSKey[] = { 0xFD, 0x46, 0xEC, 0xBD, 0x21, 0x17, 0x4D, 0x04, 0x72, 0x10, 0x1A, 0x6A, 0x2C, 0x06, 0x4E, 0x56 };
uint32_t DevAddr = 0x26011400;

void setup() {

  //LoRa.setPins(ss, reset, dio0)
  //dio0 must be interrupt capable via Arduinos attachInterrupt
  LoRa.setPins(25, 30, 26);

  if (!LoRa.begin(868500E3)) {
        digitalWrite(13,HIGH);
    while (1);
  }

  LoRa.setSpreadingFactor(7);
  LoRa.setCodingRate4(5);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setSyncWord(0x34);
  LoRa.enableCrc();
}

void loop() {

  //Create Payload
  uint8_t payload[] = "Hello World :) ";
  
  uint8_t payloadLength = 14;
  
  uint8_t buffer1[MAXPAYLOAD];
  uint8_t len = generatePacket(payload, payloadLength, DevAddr, NwkSKey, AppSKey, UpLinkCounter, buffer1);

  //Send packet
  LoRa.beginPacket();
  
  for(int i = 0; i < len; i++) {
      LoRa.write(buffer1[i]);
  }

  LoRa.endPacket();

  //Incriment frame counter
  UpLinkCounter++;

  //Blink the onboard LED
  digitalWrite(12,HIGH);
  delay(200);
  digitalWrite(12, LOW);

  delay(5000);
}
