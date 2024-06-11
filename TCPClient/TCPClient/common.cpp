#include "common.h"

int InitializeWinsock() {
	WSADATA wsa;
	printf("\nInitializing Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		printf("That bai. Loi code: %d", WSAGetLastError());
		return 0;
	}
	printf("Da khoi tao.");
	return 1;
}

SOCKET ConnectToServer(const char *serverIPAddress, u_short port) {
	SOCKET s;
	struct sockaddr_in server;

	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		printf("Tao Socket that bai: %d", WSAGetLastError());
		return INVALID_SOCKET;
	}
	printf("Da tao Socket.\n");

	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	if (inet_pton(AF_INET, serverIPAddress, &server.sin_addr) != 1) {
		perror("inet_pton");
		return INVALID_SOCKET;
	}

	if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0) {
		perror("Ket noi that bai");
		return INVALID_SOCKET;
	}
	printf("Da ket noi den Server.\n");
	return s;
}

void ProcessRectangles(SOCKET socket, FILE *inputFile, FILE *outputFile) {
	char line[256];
	Rect rect; 
	Result result;

	fgets(line, sizeof(line), inputFile); 

	while (fgets(line, sizeof(line), inputFile) != NULL) {
		if (sscanf(line, "%lf %lf", &rect.width, &rect.length) == 2) { 
			send(socket, (char *)&rect, sizeof(Rect), 0); 
			recv(socket, (char *)&result, sizeof(Result), 0);

			printf("Chieu rong va chieu dai: %.11lf %.11lf\n", rect.width, rect.length);
			printf("Chieu cao va Vmax: %.11lf %.11lf\n", result.height, result.volume);

			fprintf(outputFile, "%.11lf,%.11lf,%.11lf,%.11lf\n", rect.width, rect.length, result.height, result.volume);
		}
		else {
			printf("Error reading line: %s\n", line);
		}
	}
}

