#include "includes.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

void printArrayHex(uint8_t* arr, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        printf("%02x ", arr[i]);
    }
    printf("\n");
}