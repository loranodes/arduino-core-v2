#include <stdint.h>


#define UP_LINK 0
#define MAXPAYLOAD  255


uint8_t generatePacket(
    uint8_t* payload,
    uint8_t payloadLength,
    uint32_t DevAddr,
    uint8_t* NwkSKey,
    uint8_t* AppSKey,
    uint16_t UpLinkCounter,
    uint8_t LoRaMacBuffer[]
);

