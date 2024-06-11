#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#pragma comment(lib,"Ws2_32.lib")

typedef struct {
	double width, length;
} Rect;

typedef struct {
	double height, volume;
} Result;

int InitializeWinsock();
SOCKET ConnectToServer(const char *serverIPAddress, u_short port);
void ProcessRectangles(SOCKET socket, FILE *inputFile, FILE *outputFile);
