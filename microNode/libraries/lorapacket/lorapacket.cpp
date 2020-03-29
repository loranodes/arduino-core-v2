#include "LoRaMacCrypto.h"
#include "lorapacket.h"
#include "Arduino.h"

uint8_t generatePacket(
    uint8_t payload[],
    uint8_t payloadLength,
    uint32_t DevAddr,
    uint8_t NwkSKey[],
    uint8_t AppSKey[],
    uint16_t UpLinkCounter,
    uint8_t LoRaMacBuffer[]
){
    
    uint8_t MHDR = 0x40;
    
    uint8_t fCtrl = 0;

    uint8_t pktHeaderLen = 0;
    uint8_t LoRaMacBufferPktLen;
    uint32_t mic = 0;
    uint8_t framePort = 3;
    
    LoRaMacBufferPktLen = 0;
    
    LoRaMacBuffer[pktHeaderLen++] = MHDR;
    
    LoRaMacBuffer[pktHeaderLen++] = ( DevAddr ) & 0xFF;
    LoRaMacBuffer[pktHeaderLen++] = ( DevAddr >> 8 ) & 0xFF;
    LoRaMacBuffer[pktHeaderLen++] = ( DevAddr >> 16 ) & 0xFF;
    LoRaMacBuffer[pktHeaderLen++] = ( DevAddr >> 24 ) & 0xFF;
    
    LoRaMacBuffer[pktHeaderLen++] = fCtrl;
    
    LoRaMacBuffer[pktHeaderLen++] = UpLinkCounter & 0xFF;
    LoRaMacBuffer[pktHeaderLen++] = ( UpLinkCounter >> 8 ) & 0xFF;
    LoRaMacBuffer[pktHeaderLen++] = framePort;
        
    LoRaMacPayloadEncrypt( payload, payloadLength, AppSKey, DevAddr, UP_LINK, UpLinkCounter, &LoRaMacBuffer[pktHeaderLen] );
    
    LoRaMacBufferPktLen = pktHeaderLen + payloadLength;
    
    LoRaMacComputeMic( LoRaMacBuffer, LoRaMacBufferPktLen, NwkSKey, DevAddr, UP_LINK, UpLinkCounter, &mic );
    
    LoRaMacBuffer[LoRaMacBufferPktLen + 0] = mic & 0xFF;
    LoRaMacBuffer[LoRaMacBufferPktLen + 1] = ( mic >> 8 ) & 0xFF;
    LoRaMacBuffer[LoRaMacBufferPktLen + 2] = ( mic >> 16 ) & 0xFF;
    LoRaMacBuffer[LoRaMacBufferPktLen + 3] = ( mic >> 24 ) & 0xFF;
    
    LoRaMacBufferPktLen += 4; //LORAMAC_MFR_LEN

    return LoRaMacBufferPktLen;
}
