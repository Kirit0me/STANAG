#ifndef ACK_H
#define ACK_H
#include "includes.h"
#include "header.h"

typedef enum : uint8_t {
    MSG_RECEIVED = 0,
    MSG_PARTIALLY_RECEIVED = 1,
    MSG_RECEIVED_COMPLETED = 2,
    MSG_RECEIVED_REJECTED = 3,
    MSG_RECEIVE_FAILED = 4
} AckType;

typedef struct Ack {
    Header header;
    uint8_t presenceVec;
    uint8_t timestp[5];
    uint8_t orgTimestp[5];
    uint16_t orgMsgType;
    AckType ackType;   
    uint32_t optChecksum;
} Ack;

uint8_t* AckToByteArray(Ack *ack);

#endif
