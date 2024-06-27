#include "includes.h"
#include "sock_create.h"
#include "header.h"
#include "heartbeat.h"
#include "acknowledgement.h"
#include "validation.h"

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

        Heartbeat hbeat = ByteArrayToHeartbeat(recvArray);
        printHeartbeat(&hbeat);

        size_t recvLength = sizeof(recvArray);
        uint32_t checksum = *(uint32_t*)(recvArray + recvLength - 4);
        
        printf("Checksum of received message: ");
        printArrayHex((uint8_t*)&checksum, sizeof(checksum));

        // Conditional response
        if (check_msg(recvArray)) {
            Ack ack = makeAck(16002, check_msg(recvArray));

            uint8_t* responseArray = AckToByteArray(&ack);
            size_t responseSize = 34;  

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
