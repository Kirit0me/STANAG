#ifndef VALIDATION_H 
#define VALIDATION_H

#include "header.h"

uint32_t crc32(uint8_t* data, size_t length);
int check_msg(uint8_t* byteArr);

#endif