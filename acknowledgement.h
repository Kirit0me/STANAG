#ifndef ACK_H
#define ACK_H
#include "includes.h"
#include "header.h"
#include "validation.h"

typedef enum {
    MSG_RECEIVED = 0,
    MSG_PARTIALLY_RECEIVED = 1,
    MSG_RECEIVED_COMPLETED = 2,
    MSG_RECEIVED_REJECTED = 3,
    MSG_RECEIVE_FAILED = 4
} AckType;

typedef struct {
    Header header;
    uint8_t presenceVec;
    uint8_t timestp[5];
    uint8_t orgTimestp[5];
    uint16_t orgMsgType;
    uint8_t ackType;  // Changed to uint8_t to ensure it's 1 byte
    uint32_t optChecksum;
} Ack;

uint8_t* AckToByteArray(Ack *ack);
Ack makeAck(uint16_t orgMsgType, uint8_t ackTypeInt);

#endif
