#pragma warning(disable:4996)

#include "EasyTcpClient.hpp"

//bool g_thread_exit = true;

// 创建一个线程函数来获取输入命令
void cmdThread(EasyTcpClient* client) {
	while (true) {
		char cmdBuf[256] = {};
		scanf("%s", cmdBuf);
		if (0 == strcmp(cmdBuf, "exit")) {
			printf("退出Thread线程！\n");  // 这样退出只是子线程退出了，主线程依旧还在运行
			//g_thread_exit = false;
			client->closeSocket();
			return;
		}
		else if (0 == strcmp(cmdBuf, "login")) {
			Login login;
			strcpy(login.userName, "xiaoming");
			strcpy(login.passWord, "123456");
			client->SendData(&login);
		}
		else if (0 == strcmp(cmdBuf, "loginout")) {
			Loginout loginout;
			strcpy(loginout.userName, "xiaoming");
			client->SendData(&loginout);
		}
		else {
			printf("不支持的命令！\n");
		}
	}
}

int main() {

	EasyTcpClient client;
	client.connectServer("172.20.10.6", 4567);
	std::thread t1(cmdThread, &client);
	t1.detach();
	Login login;
	strcpy(login.userName, "lisi");
	strcpy(login.passWord, "123456");
	while (client.isRun()) {
		client.onRun();
		client.SendData(&login);
		 
	}
	client.closeSocket();
	printf("已退出！\n");
	getchar();
	return 0;
}