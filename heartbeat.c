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
    Header hdr = makeHeader(0x0001, 0x06, 2345, 5678, 16002 ,0xA0C0);
    uint8_t* harr = HeaderToByteArray(&hdr);

    hbeat.header = hdr;
    hbeat.presenceVec = 0x01;
    memcpy(hbeat.timestp, (uint8_t[]){0x01, 0x02, 0x03, 0x04, 0x05}, 5);
    hbeat.optChecksum = crc32(harr, 16);

    return hbeat;
}

Heartbeat ByteArrayToHeartbeat(uint8_t* byteArray) {
    Heartbeat heartbeat;

    // Convert the first 16 bytes to Header
    heartbeat.header = ByteArrayToHeader(byteArray);

    // Copy the rest of the Heartbeat fields from the byte array
    memcpy(&heartbeat.presenceVec, byteArray + 16, sizeof(heartbeat.presenceVec));
    memcpy(heartbeat.timestp, byteArray + 17, sizeof(heartbeat.timestp));
    memcpy(&heartbeat.optChecksum, byteArray + 22, sizeof(heartbeat.optChecksum));

    return heartbeat;
}

void printHeartbeat(Heartbeat* heartbeat) {
    printf("Heartbeat:\n");
    printHeader(&heartbeat->header);
    printf("  presenceVec: 0x%02x\n", heartbeat->presenceVec);
    printf("  timestp: ");
    printArrayHex(heartbeat->timestp, sizeof(heartbeat->timestp));
    printf("  optChecksum: 0x%08x\n", heartbeat->optChecksum);
}