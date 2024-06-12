#pragma warning(disable:4996)

#include "EasytcpServer.hpp"

bool g_bRun = true;
// 创建一个线程函数来获取输入命令
void cmdThread() {
	while (true) {
		char cmdBuf[256] = {};
		scanf("%s", cmdBuf);
		if (0 == strcmp(cmdBuf, "exit")) {
			printf("退出Thread线程！\n");
			g_bRun = false;
			//server->closeSocket();
			return;
		}
		else {
			printf("不支持的命令！\n");
		}
	}
}

int main() {

	EasyTcpServer server;
	server.InitSocket();
	server.BindSocket("192.168.31.146", 4567);
	server.ListrnSocket(5);

	std::thread t1(cmdThread);
	t1.detach();

	while (g_bRun) {
		server.OnRun();
	}

	server.closeSocket();
	printf("quit over!!!\n");
	getchar();
	return 0;
}