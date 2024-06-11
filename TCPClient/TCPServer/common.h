#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#pragma comment(lib,"Ws2_32.lib")

typedef struct tinhhinhchunhat
{
	double width, length;
}tinhhinhchunhat;

typedef struct inketquahinhchunhat
{
	double x, Vmax;
}inketquahinhchunhat;

int Initializewinsock();