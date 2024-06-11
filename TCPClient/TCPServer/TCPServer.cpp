#include "common.h"
#include "hinhchunhat.h"

SOCKET SocketBind(u_short port)
{
	SOCKET s;
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Không thể tạo Socket : %d", WSAGetLastError());
	}
	printf("Socket create.\n");
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);
	if (bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind bị lỗi không thành công : %d", WSAGetLastError());
	}
	puts("Bind xong");
	listen(s, 50);
	return s;
}

void XuLyClient(SOCKET clisocket)
{
	tinhhinhchunhat cn;
	inketquahinhchunhat kq;

	while (true) {
		int bytesReceived = recv(clisocket, (char *)&cn, sizeof(cn), 0);
		if (bytesReceived <= 0) {
			printf("Client ngat ket noi.\n");
			break;
		}
		Ketqua(cn.width, cn.length, kq.x, kq.Vmax);
		int bytesSent = send(clisocket, (char *)&kq, sizeof(kq), 0);
		if (bytesSent <= 0) {
			printf("Gui du lieu den CLient that bai.\n");
			break;
		}
	}
	closesocket(clisocket);
}
void main()
{
	Initializewinsock();
	u_short port = 58901;
	SOCKET svrsocket = SocketBind(port);
	SOCKET clisocket;
	struct sockaddr_in client;
	int c = sizeof(client);
	while ((clisocket = accept(svrsocket, (struct sockaddr *)&client, &c)) != INVALID_SOCKET)
	{
		printf("Dia chi IP Client: %s\n", inet_ntoa(client.sin_addr));
		printf("Client Port: %d\n", ntohs(client.sin_port));
		XuLyClient(clisocket);
	}
	closesocket(svrsocket);
	WSACleanup();
}
