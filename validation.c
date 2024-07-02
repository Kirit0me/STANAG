#include "includes.h"
#include "validation.h"

int extractHeaderAndCRC(uint8_t* byteArray, size_t length, Header* header, uint32_t* crc) {
    if (length < 20) { // 16 bytes for header + 4 bytes for CRC
        return -1; // Error: insufficient data
    }

    // Extract the first 16 bytes as Header
    memcpy(header, byteArray, sizeof(Header));

    // Extract the last 4 bytes as CRC
    memcpy(crc, byteArray + length - sizeof(uint32_t), sizeof(uint32_t));

    return 0; // Success
}

int check_msg(uint8_t* byteArr, size_t arr_size) {
    Header header;
    uint32_t extractedCRC;

    if (extractHeaderAndCRC(byteArr, arr_size, &header, &extractedCRC) != 0) {
        printf("Error: Insufficient data to extract Header and CRC.\n");
        return 1;
    }

	uint32_t computedCRC = crc32((uint8_t*)&header, sizeof(Header));

    // Compare the computed CRC with the extracted CRC
    if (computedCRC == extractedCRC) {
        printf("CRC check passed.\n");
        if (header.mesgProp == 0x20C0) {
        	return 0; // msgProp is 0x20C0
		} else if(header.mesgProp == 0xA0C0) {
			return 2; // msgProp is 0xA0C0
		}
    } else {
        printf("CRC check failed.\n");
        return 4; // CRC does not match
    }
}

uint32_t crc32(uint8_t* data, size_t length)
{
	uint32_t crc = 0xFFFFFFFF;
	uint32_t i, j;
	for (i=0; i <length; i++) {
		crc ^= data [ i ];
		for (j=0; j<8; j++) {
			if (crc & 1) {
				crc = (crc >>1) ^ 0xEDB88320;
			} else {
			crc >>= 1;
			}
		}
	}
	return crc ^ 0xFFFFFFFF;
}
