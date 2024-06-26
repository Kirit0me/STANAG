#ifndef HEARTBEAT_H
#define HEARTBEAT_H
#include "includes.h"
#include "header.h"

typedef struct Heartbeat{
	Header header;
	uint8_t presenceVec;
	uint8_t timestp[5];
	uint32_t optChecksum;
}Heartbeat;

uint8_t* HeartbeatToByteArray(Heartbeat *heartbeat);
Heartbeat makeHeartbeat(Header header, uint8_t presenceVec);
Heartbeat ByteArrayToHeartbeat(uint8_t* byteArray);

#endif
