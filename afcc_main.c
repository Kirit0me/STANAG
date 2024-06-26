#include "includes.h"
#include "sock_create.h"
#include "header.h"
#include "heartbeat.h"

#pragma comment(lib, "ws2_32.lib")

void setupServer2() {
    initializeWinsock();
    SOCKET clientSocket = createSocket();

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(5555);

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Connect failed: %d\n", WSAGetLastError());
        closesocket(clientSocket);
        WSACleanup();
        exit(1);
    }

    uint8_t recvArray[26];
    int bytesReceived = recv(clientSocket, (char*)&recvArray, sizeof(recvArray), 0);

    if (bytesReceived == SOCKET_ERROR) {
        printf("Receive failed: %d\n", WSAGetLastError());
    } else {
        printf("Received %d bytes: ", bytesReceived);
        printArrayHex(recvArray, bytesReceived);

        // Conditional response
        if (recvArray[0] == 1) {
            Header hdr = makeHeader(2, 26, 2346, 5679, 1 ,1);
            Heartbeat hbeat = makeHeartbeat(hdr, 0x02);

            uint8_t* responseArray = HeartbeatToByteArray(&hbeat);
            size_t responseSize = 26;  

            int bytesSent = send(clientSocket, (char*)responseArray, responseSize, 0);
            if (bytesSent == SOCKET_ERROR) {
                printf("Send failed: %d\n", WSAGetLastError());
            } else {
                printf("Sent %d bytes: ", bytesSent);
                printArrayHex(responseArray, responseSize);
            }

            free(responseArray); // Don't forget to free the allocated memory
        }
    }

    closesocket(clientSocket);
    WSACleanup();
}

int main() {
    setupServer2();
    return 0;
}
