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
	//client.initSocket();
	// 	server.BindSocket("192.168.31.146", 4567);
	client.connectServer("192.168.31.146", 4567);
	std::thread t1(cmdThread, &client);
	t1.detach();

	// 再创建一个客户端
	//EasyTcpClient client2;
	////client.initSocket();
	//client2.connectServer("172.29.140.202", 4567);
	//std::thread t2(cmdThread, &client2);
	//t2.detach(); 

	while (client.isRun()) {
		client.onRun();
		//client2.onRun();
	}
	client.closeSocket();
	//client2.closeSocket();
	printf("已退出！\n");
	getchar();
	return 0;
}