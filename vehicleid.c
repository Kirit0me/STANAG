#include "vehicleid.h"
#include "includes.h"
#include "header.h"
#include "validation.h"

uint8_t* VehicleToByteArray(Vehicle *vehicle) {
    uint8_t *byteArray = (uint8_t *)malloc(114 * sizeof(uint8_t)); // Total size: 16 (Header) + 2 (PresenceVector) + 5 (TimeStamp) + 4 (VsmId) + 4 (VehicleIdUpdate) + 2 (type) + 2 (subtype) + 1 (OwningId) + 16 (TailNo) + 20 (MissionId) + 32 (CallSign) + 2 (ConfigChecksum) + 4 (OptionalChecksum)
    if (byteArray == NULL) {
        return NULL;
    }

    // Convert Header to byte array
    uint8_t *headerArray = HeaderToByteArray(&vehicle->header);
    if (headerArray == NULL) {
        free(byteArray);
        return NULL;
    }

    // Copy the Header byte array into the Vehicle byte array
    memcpy(byteArray, headerArray, sizeof(Header));

    // Copy the rest of the Vehicle fields into the byte array
    memcpy(byteArray + 16, vehicle->PresenceVector, sizeof(vehicle->PresenceVector));
    memcpy(byteArray + 18, vehicle->TimeStamp, sizeof(vehicle->TimeStamp));
    memcpy(byteArray + 23, &vehicle->VsmId, sizeof(vehicle->VsmId));
    memcpy(byteArray + 27, &vehicle->VehicleIdUpdate, sizeof(vehicle->VehicleIdUpdate));
    memcpy(byteArray + 31, &vehicle->type, sizeof(vehicle->type));
    memcpy(byteArray + 33, &vehicle->subtype, sizeof(vehicle->subtype));
    memcpy(byteArray + 35, &vehicle->OwningId, sizeof(vehicle->OwningId));
    memcpy(byteArray + 36, vehicle->TailNo, sizeof(vehicle->TailNo));
    memcpy(byteArray + 52, vehicle->MissionId, sizeof(vehicle->MissionId));
    memcpy(byteArray + 72, vehicle->CallSign, sizeof(vehicle->CallSign));
    memcpy(byteArray + 104, &vehicle->ConfigChecksum, sizeof(vehicle->ConfigChecksum));
    memcpy(byteArray + 106, &vehicle->OptionalChecksum, sizeof(vehicle->OptionalChecksum));

    // Free the allocated memory for headerArray
    free(headerArray);

    return byteArray;
}

void printVehicle(Vehicle *vehicle) {
    printf("Header:\n");
    printf("  Reserved: %04x\n", vehicle->header.reserved);
    printf("  MsgLen: %04x\n", vehicle->header.msgLen);
    printf("  SrcId: %08x\n", vehicle->header.srcId);
    printf("  DestId: %08x\n", vehicle->header.destId);
    printf("  MsgType: %04x\n", vehicle->header.msgType);
    printf("  MsgProp: %04x\n", vehicle->header.mesgProp);

    printf("PresenceVector: ");
    printArrayHex(vehicle->PresenceVector, sizeof(vehicle->PresenceVector));

    printf("TimeStamp: ");
    printArrayHex(vehicle->TimeStamp, sizeof(vehicle->TimeStamp));

    printf("VsmId: %d\n", vehicle->VsmId);
    printf("VehicleIdUpdate: %d\n", vehicle->VehicleIdUpdate);
    printf("Type: %d\n", vehicle->type);
    printf("Subtype: %d\n", vehicle->subtype);
    printf("OwningId: %d\n", vehicle->OwningId);
    printf("TailNo: %s\n", vehicle->TailNo);
    printf("MissionId: %s\n", vehicle->MissionId);
    printf("CallSign: %s\n", vehicle->CallSign);
    printf("ConfigChecksum: %04x\n", vehicle->ConfigChecksum);
    printf("OptionalChecksum: %08x\n", vehicle->OptionalChecksum);
}


Vehicle makeVehicle() {
    Vehicle vehicle;

    Header hdr = makeHeader(0x0001, 0x06, 2345, 5678, 3, 0xA0C0);
    uint8_t* harr = HeaderToByteArray(&hdr);

    vehicle.header = hdr;
    vehicle.PresenceVector[0] = 0x01;
    vehicle.PresenceVector[1] = 0x02;

#ifdef _WIN32
    struct _timeb timebuffer;
    _ftime(&timebuffer);
    long long milliseconds_since_epoch = (long long)timebuffer.time * 1000 + timebuffer.millitm;
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long long milliseconds_since_epoch = (long long)tv.tv_sec * 1000 + tv.tv_usec / 1000;
#endif

    memcpy(vehicle.TimeStamp, &milliseconds_since_epoch, 5);

    vehicle.VsmId = 1234;
    vehicle.VehicleIdUpdate = 5678;
    vehicle.type = 0x0001;
    vehicle.subtype = 0x0002;
    vehicle.OwningId = 0x01;
    strncpy(vehicle.TailNo, "TailNumber", 16);
    strncpy(vehicle.MissionId, "MissionID", 20);
    strncpy(vehicle.CallSign, "CallSign", 32);
    vehicle.ConfigChecksum = 0x1234;
    vehicle.OptionalChecksum = crc32(harr, 16);

    free(harr);

    return vehicle;
}

Vehicle ByteArrayToVehicle(const uint8_t *byteArray) {
    Vehicle vehicle;
    
    if (byteArray == NULL) {
        // Return an empty or default vehicle if input is NULL
        memset(&vehicle, 0, sizeof(Vehicle));
        return vehicle;
    }

    // Extract Header from byte array
    memcpy(&vehicle.header, byteArray, sizeof(Header));

    // Extract the rest of the Vehicle fields from the byte array
    memcpy(vehicle.PresenceVector, byteArray + 16, sizeof(vehicle.PresenceVector));
    memcpy(vehicle.TimeStamp, byteArray + 18, sizeof(vehicle.TimeStamp));
    memcpy(&vehicle.VsmId, byteArray + 23, sizeof(vehicle.VsmId));
    memcpy(&vehicle.VehicleIdUpdate, byteArray + 27, sizeof(vehicle.VehicleIdUpdate));
    memcpy(&vehicle.type, byteArray + 31, sizeof(vehicle.type));
    memcpy(&vehicle.subtype, byteArray + 33, sizeof(vehicle.subtype));
    memcpy(&vehicle.OwningId, byteArray + 35, sizeof(vehicle.OwningId));
    memcpy(vehicle.TailNo, byteArray + 36, sizeof(vehicle.TailNo));
    memcpy(vehicle.MissionId, byteArray + 52, sizeof(vehicle.MissionId));
    memcpy(vehicle.CallSign, byteArray + 72, sizeof(vehicle.CallSign));
    memcpy(&vehicle.ConfigChecksum, byteArray + 104, sizeof(vehicle.ConfigChecksum));
    memcpy(&vehicle.OptionalChecksum, byteArray + 106, sizeof(vehicle.OptionalChecksum));

    return vehicle;
}

