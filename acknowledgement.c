#include "includes.h"
#include "header.h"
#include "validation.h"
#include "acknowledgement.h"
#include "heartbeat.h"

void getCurrentTimeMillis(uint8_t *timestamp, size_t timestampSize) {
    struct timeb tb;
    ftime(&tb);
    uint64_t currentTimeMillis = (uint64_t)tb.time * 1000 + (uint64_t)tb.millitm;

    // Fill the timestamp field with the least significant bytes of currentTimeMillis
    for (size_t i = 0; i < timestampSize; ++i) {
        timestamp[i] = (currentTimeMillis >> (8 * (timestampSize - 1 - i))) & 0xFF;
    }
}

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

Ack makeAck(uint16_t orgMsgType, uint8_t ackTypeInt, Heartbeat* hbeat) {
    // Create the header for the Ack message
    Header hdr = makeHeader(0x0000, 0x0E, 2345, 5678, 17000 ,0xA0C0);
    uint8_t* harr = HeaderToByteArray(&hdr);

    // Create and initialize the Ack message
    Ack ack;
    ack.header = hdr;
    ack.presenceVec = 0x01;

   // Get the current timestamp and fill the ack.timestp field
    getCurrentTimeMillis(ack.timestp, sizeof(ack.timestp));

    // Use the timestamp from the heartbeat message for ack.orgTimestp
    if (hbeat != NULL) {
        memcpy(ack.orgTimestp, hbeat->timestp, sizeof(ack.orgTimestp));
    } else {
        memset(ack.orgTimestp, 0, sizeof(ack.orgTimestp)); // Initialize with 0 if hbeat is NULL
    }

    ack.orgMsgType = orgMsgType;
    ack.ackType = (AckType)ackTypeInt; // Cast integer to AckType
    ack.optChecksum = crc32(harr, 16); // Compute the checksum

    return ack;
}
void printAck(Ack* ack) {
    printf("Ack:\n");
    printHeader(&ack->header);
    printf("  presenceVec: 0x%02x\n", ack->presenceVec);
    printf("  timestp: ");
    printArrayHex(ack->timestp, sizeof(ack->timestp));
    printf("  orgTimestp: ");
    printArrayHex(ack->orgTimestp, sizeof(ack->orgTimestp));
    printf("  orgMsgType: 0x%04x\n", ack->orgMsgType);
    printf("  ackType: %u\n", ack->ackType);
    printf("  optChecksum: 0x%08x\n", ack->optChecksum);
}

Ack byteArrayToAck(uint8_t* byteArray) {
    Ack ack;

    memcpy(&ack.header, byteArray, sizeof(Header));
    memcpy(&ack.presenceVec, byteArray + 16, sizeof(ack.presenceVec));
    memcpy(ack.timestp, byteArray + 17, sizeof(ack.timestp));
    memcpy(&ack.orgTimestp, byteArray + 22, sizeof(ack.orgTimestp));
    memcpy(&ack.orgMsgType, byteArray + 27, sizeof(ack.orgMsgType));
    memcpy(&ack.ackType, byteArray + 29, sizeof(ack.ackType));
    memcpy(&ack.optChecksum, byteArray + 30, sizeof(ack.optChecksum));

    return ack;
}
