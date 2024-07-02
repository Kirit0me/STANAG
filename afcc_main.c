#include "includes.h"
#include "sock_create.h"
#include "header.h"
#include "heartbeat.h"
#include "acknowledgement.h"
#include "validation.h"
#include "cucs.h"
#include "vehicleid.h"

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

    uint8_t recvArray[500];

    while (1) {
        int bytesReceived = recv(clientSocket, (char*)&recvArray, sizeof(recvArray), 0);

        if (bytesReceived == SOCKET_ERROR) {
            printf("Receive failed: %d\n", WSAGetLastError());
            break;
        } else if (bytesReceived > 0) {
            printf("Received %d bytes: ", bytesReceived);
            printArrayHex(recvArray, bytesReceived);

            Header header;
            memcpy(&header, recvArray, sizeof(Header));
            uint16_t msgType = header.msgType;

            switch (msgType) {
                case 16002: { // Heartbeat
                    Heartbeat hbeat = byteArrayToHeartbeat(recvArray);
                    printHeartbeat(&hbeat);

                    Ack ack = makeAck(16002, check_msg(HeartbeatToByteArray(&hbeat), 26), &hbeat );
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
                    break;
                }
                case 1: { // CUCS
                    Vehicle vehicle = makeVehicle();
                    uint8_t* vehicleArray = VehicleToByteArray(&vehicle);
                    size_t vehicleSize = sizeof(Vehicle);  

                    int bytesSent = send(clientSocket, (char*)vehicleArray, vehicleSize, 0);
                    if (bytesSent == SOCKET_ERROR) {
                        printf("Send failed: %d\n", WSAGetLastError());
                    } else {
                        printf("Sent %d bytes: ", bytesSent);
                        printArrayHex(vehicleArray, vehicleSize);
                    }

                    free(vehicleArray); // Don't forget to free the allocated memory
                    break;
                }
                default:
                    printf("Unknown message type: %d\n", msgType);
                    break;
            }
        }
    }

    closesocket(clientSocket);
    WSACleanup();
}

int main() {
    setupServer2();
    return 0;
}