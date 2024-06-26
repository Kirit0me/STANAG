#ifndef SOCK_CREATE
#define SOCK_CREATE

#include <winsock2.h>
#include <stdint.h>

void initializeWinsock();
SOCKET createSocket();

#endif