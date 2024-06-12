#include "EasytcpServer.hpp"

int main() {

	EasyTcpServer server;
	server.InitSocket();
	server.BindSocket("172.20.10.6", 4567);
	server.ListrnSocket(5);

	while (server.isRun()) {
		server.OnRun();

	}

	server.closeSocket();
	printf("quit over!!!\n");
	getchar();
	return 0;
}