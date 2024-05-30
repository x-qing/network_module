#include "EasytcpServer.hpp"

int main() {

	EasyTcpServer server;
	server.InitSocket();
	server.BindSocket("192.168.31.146", 4567);
	server.ListrnSocket(5);

	while (server.isRun()) {
		server.OnRun();

	}

	server.closeSocket();
	printf("quit over!!!\n");
	getchar();
	return 0;
}