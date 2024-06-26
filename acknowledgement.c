#include "includes.h"
#include "header.h"
#include "acknowledgement.h"
#include "validation.h"

uint8_t* AckToByteArray(Ack *ack) {
    // Allocate memory for the byte array
    uint8_t *byteArray = (uint8_t *)malloc(34 * sizeof(uint8_t)); // 16 (Header) + 1 (presenceVec) + 5 (timestp) + 5 (orgTimestp) + 2 (orgMsgType) + 1 (ackType) + 4 (optChecksum)
    if (byteArray == NULL) {
        // Memory allocation failed
        return NULL;
    }

    uint8_t *headerArray = HeaderToByteArray(&ack->header);
    if (headerArray == NULL) {
        free(byteArray);
        return NULL;
    }

    // Copy the Header byte array into the Ack byte array
    memcpy(byteArray, headerArray, 16);

    // Copy the rest of the Ack fields into the byte array
    memcpy(byteArray + 16, &ack->presenceVec, sizeof(ack->presenceVec));
    memcpy(byteArray + 17, ack->timestp, sizeof(ack->timestp));
    memcpy(byteArray + 22, ack->orgTimestp, sizeof(ack->orgTimestp));
    memcpy(byteArray + 27, &ack->orgMsgType, sizeof(ack->orgMsgType));
    memcpy(byteArray + 29, &ack->ackType, sizeof(ack->ackType));
    memcpy(byteArray + 30, &ack->optChecksum, sizeof(ack->optChecksum));

    // Free the allocated memory for headerArray
    free(headerArray);

    return byteArray;
}

Ack makeAck(uint16_t orgMsgType, uint8_t ackTypeInt) {
    Header hdr = makeHeader(0x0000, 0x0E, 2345, 5678, 17000 ,0xA0C0);
    uint8_t* harr = HeaderToByteArray(&hdr);
    
    Ack ack;
    ack.header = hdr;
    ack.presenceVec = 0x01;
    memset(ack.timestp, 0, sizeof(ack.timestp)); // Initialize with 0 or set it to the current timestamp
    memset(ack.orgTimestp, 0, sizeof(ack.orgTimestp)); // Initialize with 0 or set it to the original timestamp
    ack.orgMsgType = orgMsgType;
    ack.ackType = (AckType)ackTypeInt; // Cast integer to AckType
    ack.optChecksum = crc32(harr, 16); // Initialize with 0 or compute the checksum if needed

    return ack;
}
