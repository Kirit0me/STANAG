#ifndef HEADER_H
#define HEADER_H

#include "includes.h"

typedef struct Header{
	uint16_t reserved;
	uint16_t msgLen;
	uint32_t srcId;
	uint32_t destId;
	uint16_t msgType;
	uint16_t mesgProp;
}Header;

uint8_t* HeaderToByteArray(Header *header);
Header ByteArrayToHeader(uint8_t* byteArray);
Header makeHeader(uint16_t reserved, uint16_t msgLen, uint32_t srcId, uint32_t destId, uint16_t msgType, uint16_t mesgProp);

#endif
