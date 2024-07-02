#include "cucs.h"
#include "includes.h"
#include "header.h"
#include "validation.h"

Cucs makeCucs() {
    Cucs cucs;
    cucs.header = makeHeader(0x0001, 0x23, 2345, 5678, 1, 0xA0C0);
    
    memcpy(cucs.PresenceVector, (uint8_t[]){0x01, 0x02, 0x03}, 3);
    time_t currentTime = time(NULL);
    uint64_t millis = (uint64_t)currentTime * 1000;
    for (int i = 0; i < 5; i++) {
        cucs.TimeStamp[4 - i] = (millis >> (i * 8)) & 0xFF;
    }
    
    cucs.VsmId = 1;
    cucs.DataLinkId = 1;
    cucs.type = 1;
    cucs.subtype = 1;
    cucs.RequestedLoi = 1;
    cucs.RequestedAccess = 1;
    cucs.RequestedMode = 1;
    cucs.ControlledStation1 = 1;
    cucs.ComponentNo = 1;
    cucs.SubcomponentNo = 1;
    cucs.PayloadType = 1;
    cucs.AssetMode = 1;
    cucs.Wait = 1;
    cucs.CucsType = 1;
    cucs.CucsSubtype = 1;
    cucs.PresenceVectorSupport = 1;
    cucs.ControlledStation2 = 1;
    cucs.OptionalChecksum = crc32((uint8_t*)&cucs, sizeof(Cucs) - sizeof(cucs.OptionalChecksum));
    
    return cucs;
}

uint8_t* CucsToByteArray(Cucs* cucs) {
    uint8_t *byteArray = (uint8_t *)malloc(58 * sizeof(uint8_t)); // Total size: 16 (Header) + 3 (PresenceVector) + 5 (TimeStamp) + 4 (VsmId) + 4 (DataLinkId) + 2 (type) + 2 (subtype) + 1 (RequestedLoi) + 1 (RequestedAccess) + 1 (RequestedMode) + 2 (ControlledStation1) + 2 (ComponentNo) + 2 (SubcomponentNo) + 1 (PayloadType) + 1 (AssetMode) + 1 (Wait) + 1 (CucsType) + 2 (CucsSubtype) + 1 (PresenceVectorSupport) + 2 (ControlledStation2) + 4 (OptionalChecksum)
    if (byteArray == NULL) {
        return NULL;
    }

    // Convert Header to byte array
    uint8_t *headerArray = HeaderToByteArray(&cucs->header);
    if (headerArray == NULL) {
        free(byteArray);
        return NULL;
    }

    // Copy the Header byte array into the Cucs byte array
    memcpy(byteArray, headerArray, sizeof(Header));

    // Copy the rest of the Cucs fields into the byte array
    memcpy(byteArray + 16, cucs->PresenceVector, sizeof(cucs->PresenceVector));
    memcpy(byteArray + 19, cucs->TimeStamp, sizeof(cucs->TimeStamp));
    memcpy(byteArray + 24, &cucs->VsmId, sizeof(cucs->VsmId));
    memcpy(byteArray + 28, &cucs->DataLinkId, sizeof(cucs->DataLinkId));
    memcpy(byteArray + 32, &cucs->type, sizeof(cucs->type));
    memcpy(byteArray + 34, &cucs->subtype, sizeof(cucs->subtype));
    memcpy(byteArray + 36, &cucs->RequestedLoi, sizeof(cucs->RequestedLoi));
    memcpy(byteArray + 37, &cucs->RequestedAccess, sizeof(cucs->RequestedAccess));
    memcpy(byteArray + 38, &cucs->RequestedMode, sizeof(cucs->RequestedMode));
    memcpy(byteArray + 39, &cucs->ControlledStation1, sizeof(cucs->ControlledStation1));
    memcpy(byteArray + 41, &cucs->ComponentNo, sizeof(cucs->ComponentNo));
    memcpy(byteArray + 43, &cucs->SubcomponentNo, sizeof(cucs->SubcomponentNo));
    memcpy(byteArray + 45, &cucs->PayloadType, sizeof(cucs->PayloadType));
    memcpy(byteArray + 46, &cucs->AssetMode, sizeof(cucs->AssetMode));
    memcpy(byteArray + 47, &cucs->Wait, sizeof(cucs->Wait));
    memcpy(byteArray + 48, &cucs->CucsType, sizeof(cucs->CucsType));
    memcpy(byteArray + 49, &cucs->CucsSubtype, sizeof(cucs->CucsSubtype));
    memcpy(byteArray + 51, &cucs->PresenceVectorSupport, sizeof(cucs->PresenceVectorSupport));
    memcpy(byteArray + 52, &cucs->ControlledStation2, sizeof(cucs->ControlledStation2));
    memcpy(byteArray + 54, &cucs->OptionalChecksum, sizeof(cucs->OptionalChecksum));

    // Free the allocated memory for headerArray
    free(headerArray);

    return byteArray;
}

void printCucs(Cucs* cucs) {
    printf("CUCS Packet:\n");
    printHeader(&cucs->header);
    printf("PresenceVector: ");
    printArrayHex(cucs->PresenceVector, 3);
    printf("TimeStamp: ");
    printArrayHex(cucs->TimeStamp, 5);
    printf("VsmId: %d\n", cucs->VsmId);
    printf("DataLinkId: %d\n", cucs->DataLinkId);
    printf("Type: %d\n", cucs->type);
    printf("Subtype: %d\n", cucs->subtype);
    printf("RequestedLoi: %d\n", cucs->RequestedLoi);
    printf("RequestedAccess: %d\n", cucs->RequestedAccess);
    printf("RequestedMode: %d\n", cucs->RequestedMode);
    printf("ControlledStation1: %d\n", cucs->ControlledStation1);
    printf("ComponentNo: %d\n", cucs->ComponentNo);
    printf("SubcomponentNo: %d\n", cucs->SubcomponentNo);
    printf("PayloadType: %d\n", cucs->PayloadType);
    printf("AssetMode: %d\n", cucs->AssetMode);
    printf("Wait: %d\n", cucs->Wait);
    printf("CucsType: %d\n", cucs->CucsType);
    printf("CucsSubtype: %d\n", cucs->CucsSubtype);
    printf("PresenceVectorSupport: %d\n", cucs->PresenceVectorSupport);
    printf("ControlledStation2: %d\n", cucs->ControlledStation2);
    printf("OptionalChecksum: %08x\n", cucs->OptionalChecksum);
}

Cucs byteArrayToCucs(uint8_t* byteArray) {
    Cucs cucs;

    // Extract Header from the byte array
    memcpy(&cucs.header, byteArray, sizeof(Header));

    // Extract PresenceVector (3 bytes)
    memcpy(cucs.PresenceVector, byteArray + 16, sizeof(cucs.PresenceVector));

    // Extract TimeStamp (5 bytes)
    memcpy(cucs.TimeStamp, byteArray + 19, sizeof(cucs.TimeStamp));

    // Extract VsmId (4 bytes)
    memcpy(&cucs.VsmId, byteArray + 24, sizeof(cucs.VsmId));

    // Extract DataLinkId (4 bytes)
    memcpy(&cucs.DataLinkId, byteArray + 28, sizeof(cucs.DataLinkId));

    // Extract type (2 bytes)
    memcpy(&cucs.type, byteArray + 32, sizeof(cucs.type));

    // Extract subtype (2 bytes)
    memcpy(&cucs.subtype, byteArray + 34, sizeof(cucs.subtype));

    // Extract RequestedLoi (1 byte)
    memcpy(&cucs.RequestedLoi, byteArray + 36, sizeof(cucs.RequestedLoi));

    // Extract RequestedAccess (1 byte)
    memcpy(&cucs.RequestedAccess, byteArray + 37, sizeof(cucs.RequestedAccess));

    // Extract RequestedMode (1 byte)
    memcpy(&cucs.RequestedMode, byteArray + 38, sizeof(cucs.RequestedMode));

    // Extract ControlledStation1 (2 bytes)
    memcpy(&cucs.ControlledStation1, byteArray + 39, sizeof(cucs.ControlledStation1));

    // Extract ComponentNo (2 bytes)
    memcpy(&cucs.ComponentNo, byteArray + 41, sizeof(cucs.ComponentNo));

    // Extract SubcomponentNo (2 bytes)
    memcpy(&cucs.SubcomponentNo, byteArray + 43, sizeof(cucs.SubcomponentNo));

    // Extract PayloadType (1 byte)
    memcpy(&cucs.PayloadType, byteArray + 45, sizeof(cucs.PayloadType));

    // Extract AssetMode (1 byte)
    memcpy(&cucs.AssetMode, byteArray + 46, sizeof(cucs.AssetMode));

    // Extract Wait (1 byte)
    memcpy(&cucs.Wait, byteArray + 47, sizeof(cucs.Wait));

    // Extract CucsType (1 byte)
    memcpy(&cucs.CucsType, byteArray + 48, sizeof(cucs.CucsType));

    // Extract CucsSubtype (2 bytes)
    memcpy(&cucs.CucsSubtype, byteArray + 49, sizeof(cucs.CucsSubtype));

    // Extract PresenceVectorSupport (1 byte)
    memcpy(&cucs.PresenceVectorSupport, byteArray + 51, sizeof(cucs.PresenceVectorSupport));

    // Extract ControlledStation2 (2 bytes)
    memcpy(&cucs.ControlledStation2, byteArray + 52, sizeof(cucs.ControlledStation2));

    // Extract OptionalChecksum (4 bytes)
    memcpy(&cucs.OptionalChecksum, byteArray + 54, sizeof(cucs.OptionalChecksum));

    return cucs;
}
