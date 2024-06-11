#include "common.h"

int main() {
	if (!InitializeWinsock()) {
		printf("khong khoi tao duoc Winsock. dang thoat.\n");
		return EXIT_FAILURE;
	}
	const char *serverIPAddress = "172.17.13.71";
	u_short port = 58901;
	FILE *inputFile = fopen("DanhHCN_31012024.txt", "r");
	FILE *outputFile = fopen("ketqua.csv", "w");

	if (!inputFile || !outputFile) {
		perror("khong mo duoc file");
		return EXIT_FAILURE;
	}
	SOCKET serverSocket = ConnectToServer(serverIPAddress, port);
	if (serverSocket == INVALID_SOCKET) {
		printf("Ket noi den Server that bai. dang thoat.\n");
		return EXIT_FAILURE;
	}
	ProcessRectangles(serverSocket, inputFile, outputFile);
	fclose(inputFile);
	fclose(outputFile);
	closesocket(serverSocket);
	WSACleanup();
	system("7-Zip\\7z.exe a ketqua.7z ketqua.csv");

	return EXIT_SUCCESS;
}
