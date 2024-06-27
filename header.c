#include "header.h"

Header makeHeader(uint16_t reserved, uint16_t msgLen, uint32_t srcId, uint32_t destId, uint16_t msgType, uint16_t mesgProp) {
    Header temp;
    temp.reserved = reserved;
    temp.msgLen = msgLen;
    temp.srcId = srcId;
    temp.destId = destId;
    temp.msgType = msgType;
    temp.mesgProp = mesgProp;
    return temp;
}

uint8_t* HeaderToByteArray(Header *header) {
    uint8_t *byteArray = (uint8_t *)malloc(16 * sizeof(uint8_t));
    if (byteArray == NULL) {
        return NULL;
    }
    
    memcpy(byteArray, &header->reserved, sizeof(header->reserved));
    memcpy(byteArray + 2, &header->msgLen, sizeof(header->msgLen));
    memcpy(byteArray + 4, &header->srcId, sizeof(header->srcId));
    memcpy(byteArray + 8, &header->destId, sizeof(header->destId));
    memcpy(byteArray + 12, &header->msgType, sizeof(header->msgType));
    memcpy(byteArray + 14, &header->mesgProp, sizeof(header->mesgProp));

    return byteArray;
}

Header ByteArrayToHeader(uint8_t* byteArray) {
    Header header;

    memcpy(&header.reserved, byteArray, sizeof(header.reserved));
    memcpy(&header.msgLen, byteArray + 2, sizeof(header.msgLen));
    memcpy(&header.srcId, byteArray + 4, sizeof(header.srcId));
    memcpy(&header.destId, byteArray + 8, sizeof(header.destId));
    memcpy(&header.msgType, byteArray + 12, sizeof(header.msgType));
    memcpy(&header.mesgProp, byteArray + 14, sizeof(header.mesgProp));

    return header;
}

void printHeader(Header* header) {
    printf("Header:\n");
    printf("  reserved: 0x%04x\n", header->reserved);
    printf("  msgLen: %u\n", header->msgLen);
    printf("  srcId: 0x%08x\n", header->srcId);
    printf("  destId: 0x%08x\n", header->destId);
    printf("  msgType: 0x%04x\n", header->msgType);
    printf("  mesgProp: 0x%04x\n", header->mesgProp);
}