#ifndef CUCS_H
#define CUCS_H

#include "includes.h"
#include "header.h"
#include "validation.h"

typedef struct Cucs {
    Header header;
    uint8_t PresenceVector[3];
    uint8_t TimeStamp[5];
    int32_t VsmId;
    int32_t DataLinkId;
    uint16_t type;
    uint16_t subtype;
    uint8_t RequestedLoi;
    uint8_t RequestedAccess;
    uint8_t RequestedMode;
    uint16_t ControlledStation1;
    uint16_t ComponentNo;
    uint16_t SubcomponentNo;
    uint8_t PayloadType;
    uint8_t AssetMode;
    uint8_t Wait;
    uint8_t CucsType;
    uint16_t CucsSubtype;
    uint8_t PresenceVectorSupport;
    uint16_t ControlledStation2;
    uint32_t OptionalChecksum;
} Cucs;

Cucs makeCucs();
uint8_t* CucsToByteArray(Cucs* cucs);
void printCucs(Cucs* cucs);
Cucs byteArrayToCucs(uint8_t* byteArray);

#endif