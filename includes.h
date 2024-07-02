#ifndef INCLUDES_H
#define INCLUDES_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stddef.h>
#include <sys/timeb.h>

void printArrayHex(uint8_t* arr, size_t length);

#endif
