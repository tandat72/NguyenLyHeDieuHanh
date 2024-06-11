#include "common.h"
int Initializewinsock()
{
	WSADATA wsa;
	printf("\nInitializeWinsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed.Error Code : %d", WSAGetLastError());
		exit(-1)
			;
		return 0;
	}
	printf("Initialized.");
	return 1;
}
