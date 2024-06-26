#include "includes.h"
#include "sock_create.h"
#include "header.h"
#include "heartbeat.h"

#pragma comment(lib, "ws2_32.lib")

void setupServer1() {
    initializeWinsock();
    SOCKET serverSocket = createSocket();

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(5555);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Bind failed: %d\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        exit(1);
    }

    if (listen(serverSocket, 5) == SOCKET_ERROR) {
        printf("Listen failed: %d\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        exit(1);
    }

    printf("Server 1 waiting for connections...\n");

    SOCKET clientSocket = accept(serverSocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET) {
        printf("Accept failed: %d\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        exit(1);
    }

    Header hdr = makeHeader(1, 26, 2345, 5678, 1 ,1);
    Heartbeat hbeat = makeHeartbeat(hdr, 0x01);

    uint8_t* array = HeartbeatToByteArray(&hbeat);
    size_t arraySize = 26; 

    int bytesSent = send(clientSocket, (char*)array, arraySize, 0);

    if (bytesSent == SOCKET_ERROR) {
        printf("Send failed: %d\n", WSAGetLastError());
    } else {
        printf("Sent %d bytes: ", bytesSent);
        printArrayHex(array, arraySize);
    }

    uint8_t recvArray[26];
    int bytesReceived = recv(clientSocket, (char*)&recvArray, sizeof(recvArray), 0);

    if (bytesReceived == SOCKET_ERROR) {
        printf("Receive failed: %d\n", WSAGetLastError());
    } else {
        printf("Received %d bytes: ", bytesReceived);
        printArrayHex(recvArray, bytesReceived);
    }

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
}

int main() {
    setupServer1();
    return 0;
}
