#ifndef VEHICLE_H
#define VEHICLE_H

#include "includes.h"
#include "header.h"
#include "validation.h"

typedef struct Vehicle {
    Header header;
    uint8_t PresenceVector[2];
    uint8_t TimeStamp[5];
    int32_t VsmId;
    int32_t VehicleIdUpdate;
    uint16_t type;
    uint16_t subtype;
    uint8_t OwningId;
    char TailNo[16];
    char MissionId[20];
    char CallSign[32];
    uint16_t ConfigChecksum;
    uint32_t OptionalChecksum;
} Vehicle;

uint8_t* VehicleToByteArray(Vehicle *vehicle);
void printVehicle(Vehicle *vehicle);
Vehicle makeVehicle();
Vehicle ByteArrayToVehicle(const uint8_t *byteArray);

#endif