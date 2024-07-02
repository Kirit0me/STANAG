#include "includes.h"
#include "header.h"
#include "validation.h"
#include "heartbeat.h"

uint8_t* HeartbeatToByteArray(Heartbeat *heartbeat) {
    uint8_t *byteArray = (uint8_t *)malloc(26 * sizeof(uint8_t)); // 16 (Header) + 1 (presenceVec) + 5 (timestp) + 4 (optChecksum)
    if (byteArray == NULL) {
        return NULL;
    }

    // Convert Header to byte array
    uint8_t *headerArray = HeaderToByteArray(&heartbeat->header);
    if (headerArray == NULL) {
        free(byteArray);
        return NULL;
    }

    // Copy the Header byte array into the Heartbeat byte array
    memcpy(byteArray, headerArray, 16);

    // Copy the rest of the Heartbeat fields into the byte array
    memcpy(byteArray + 16, &heartbeat->presenceVec, sizeof(heartbeat->presenceVec));
    memcpy(byteArray + 17, heartbeat->timestp, sizeof(heartbeat->timestp));
    memcpy(byteArray + 22, &heartbeat->optChecksum, sizeof(heartbeat->optChecksum));

    // Free the allocated memory for headerArray
    free(headerArray);

    return byteArray;
}

Heartbeat makeHeartbeat() {
    Heartbeat hbeat;
    Header hdr = makeHeader(0x01, 0x06, 2345, 5678, 16002 ,0xA0C0);
    uint8_t* harr = HeaderToByteArray(&hdr);

    hbeat.header = hdr;
    hbeat.presenceVec = 0x01;
    struct timeb tb;
    ftime(&tb);
    uint64_t currentTimeMillis = (uint64_t)tb.time * 1000 + (uint64_t)tb.millitm;

    // Fill the timestamp field with the least significant 5 bytes of currentTimeMillis
    for (int i = 0; i < 5; ++i) {
        hbeat.timestp[i] = (currentTimeMillis >> (8 * (4 - i))) & 0xFF;
    }
    hbeat.optChecksum = crc32(harr, 16);

    return hbeat;
}

void printHeartbeat(Heartbeat* heartbeat) {
    printf("Heartbeat:\n");
    printHeader(&heartbeat->header);
    printf("  presenceVec: 0x%02x\n", heartbeat->presenceVec);
    printf("  timestp: ");
    printArrayHex(heartbeat->timestp, sizeof(heartbeat->timestp));
    printf("  optChecksum: 0x%08x\n", heartbeat->optChecksum);
}

Heartbeat byteArrayToHeartbeat(uint8_t* byteArray) {
    Heartbeat heartbeat;

    memcpy(&heartbeat.header, byteArray, sizeof(Header));
    memcpy(&heartbeat.presenceVec, byteArray + 16, sizeof(heartbeat.presenceVec));
    memcpy(heartbeat.timestp, byteArray + 17, sizeof(heartbeat.timestp));
    memcpy(&heartbeat.optChecksum, byteArray + 22, sizeof(heartbeat.optChecksum));

    return heartbeat;
}