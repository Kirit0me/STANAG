#include "includes.h"
#include "sock_create.h"
#include "header.h"
#include "heartbeat.h"
#include "validation.h"
#include "acknowledgement.h"
#include "cucs.h"
#include "vehicleid.h"
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

#define MSG_TYPE_HEARTBEAT 16002
#define MSG_TYPE_ACK 17000
#define MSG_TYPE_CUCS 1
#define MSG_TYPE_VID 3

HANDLE hUserEvent;

typedef struct {
    SOCKET clientSocket;
} ThreadParam;

DWORD WINAPI heartbeatThread(LPVOID param);
DWORD WINAPI recvThread(LPVOID param);
DWORD WINAPI cucsThread(LPVOID param);
DWORD WINAPI userInterruptThread(LPVOID param);

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

    hUserEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

    ThreadParam param;
    param.clientSocket = clientSocket;

    HANDLE hHeartbeatThread = CreateThread(NULL, 0, heartbeatThread, &param, 0, NULL);
    HANDLE hRecvThread = CreateThread(NULL, 0, recvThread, &param, 0, NULL);
    HANDLE hCucsThread = CreateThread(NULL, 0, cucsThread, &param, 0, NULL);
    HANDLE hUserInterruptThread = CreateThread(NULL, 0, userInterruptThread, NULL, 0, NULL);

    WaitForSingleObject(hHeartbeatThread, INFINITE);
    WaitForSingleObject(hRecvThread, INFINITE);
    WaitForSingleObject(hCucsThread, INFINITE);
    WaitForSingleObject(hUserInterruptThread, INFINITE);

    CloseHandle(hHeartbeatThread);
    CloseHandle(hRecvThread);
    CloseHandle(hCucsThread);
    CloseHandle(hUserInterruptThread);
    CloseHandle(hUserEvent);

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
}

DWORD WINAPI heartbeatThread(LPVOID param) {
    ThreadParam* pParam = (ThreadParam*)param;
    SOCKET clientSocket = pParam->clientSocket;

    while (1) {
        Heartbeat hbeat = makeHeartbeat();
        uint8_t* array = HeartbeatToByteArray(&hbeat);
        size_t arraySize = 26;

        int bytesSent = send(clientSocket, (char*)array, arraySize, 0);
        if (bytesSent == SOCKET_ERROR) {
            printf("Send failed: %d\n", WSAGetLastError());
        } else {
            printf("Sent %d bytes: ", bytesSent);
            printArrayHex(array, arraySize);
        }

        free(array);
        Sleep(2000); // Sleep for 2 seconds
    }

    return 0;
}

DWORD WINAPI recvThread(LPVOID param) {
    ThreadParam* pParam = (ThreadParam*)param;
    SOCKET clientSocket = pParam->clientSocket;
    uint8_t recvArray[500];
    size_t recvLength = sizeof(recvArray);

    while (1) {
        int bytesReceived = recv(clientSocket, (char*)&recvArray, recvLength, 0);

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
                case MSG_TYPE_HEARTBEAT: {
                    Heartbeat hbeat = byteArrayToHeartbeat(recvArray);
                    printHeartbeat(&hbeat);
                    break;
                }
                case MSG_TYPE_ACK: {
                    Ack ack = byteArrayToAck(recvArray);
                    printAck(&ack);
                    break;
                }
                case MSG_TYPE_CUCS: {
                    Cucs cucs = byteArrayToCucs(recvArray);
                    printCucs(&cucs);
                    break;
                }
                case MSG_TYPE_VID: {
                    Vehicle vid = ByteArrayToVehicle(recvArray);
                    printVehicle(&vid);
                    break;
                }
                default:
                    printf("Unknown message type: %d\n", msgType);
                    break;
            }

            uint32_t checksum = *(uint32_t*)(recvArray + bytesReceived - 4);
            printf("Checksum of received message: ");
            printArrayHex((uint8_t*)&checksum, sizeof(checksum));
        }
    }

    return 0;
}

DWORD WINAPI cucsThread(LPVOID param) {
    ThreadParam* pParam = (ThreadParam*)param;
    SOCKET clientSocket = pParam->clientSocket;

    while (1) {
        WaitForSingleObject(hUserEvent, INFINITE);

        Cucs cucs = makeCucs();
        uint8_t* array = CucsToByteArray(&cucs);
        size_t arraySize = sizeof(Cucs);

        int bytesSent = send(clientSocket, (char*)array, arraySize, 0);
        if (bytesSent == SOCKET_ERROR) {
            printf("Send failed: %d\n", WSAGetLastError());
        } else {
            printf("Sent %d bytes: ", bytesSent);
            printArrayHex(array, arraySize);
        }

        free(array);
    }

    return 0;
}

DWORD WINAPI userInterruptThread(LPVOID param) {
    while (1) {
        printf("Press Enter to send CUCS packet...\n");
        getchar();
        SetEvent(hUserEvent);
    }

    return 0;
}

int main() {
    setupServer1();
    return 0;
}