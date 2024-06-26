#include "includes.h"
#include "validation.h"

int check_msg(Header hdr, uint32_t chksum) {
    return 1;
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

int check_srcdest() {

}

int MM_doc_ver() {

}


