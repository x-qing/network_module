#pragma warning(disable:4996)

#include "EasyTcpClient.hpp"

//bool g_thread_exit = true;
bool g_bRun = true;
// 创建一个线程函数来获取输入命令
void cmdThread() {
	while (true) {
		char cmdBuf[256] = {};
		scanf("%s", cmdBuf);
		if (0 == strcmp(cmdBuf, "exit")) {
			printf("退出Thread线程！\n");  // 这样退出只是子线程退出了，主线程依旧还在运行
			//g_thread_exit = false;
			//client->closeSocket();
			g_bRun = false;
			return;
		}
		//else if (0 == strcmp(cmdBuf, "login")) {
		//	Login login;
		//	strcpy(login.userName, "xiaoming");
		//	strcpy(login.passWord, "123456");
		//	client->SendData(&login);
		//}
		//else if (0 == strcmp(cmdBuf, "loginout")) {
		//	Loginout loginout;
		//	strcpy(loginout.userName, "xiaoming");
		//	client->SendData(&loginout);
		//}
		else {
			printf("不支持的命令！\n");
		}
	}
}


// 客户端数量
const int cCount = 4000;
const int tCount = 4;     // 线程的数量

EasyTcpClient* client[cCount];


//四个线程：id = 1,2,3,4
void sendThread(int id) {
	int c = (cCount / tCount);
	int begin = (id - 1) * c;
	int end = id * c;




	// 1.创建1000个客户端
	for (int i = begin; i < end; i++) {
		client[i] = new EasyTcpClient();
	}

	// 2.连接服务器
	for (int i = begin; i < end; i++) {
		client[i]->connectServer("192.168.31.146", 4567);
	}

	Login login;
	strcpy(login.userName, "lisi");
	strcpy(login.passWord, "123456");

	// 3.发送数据
	while (g_bRun) {

		for (int i = begin; i < end; i++) {
			client[i]->SendData(&login);   // 只做数据的发送
			//client[i]->onRun();    // 数据接收
		}
	}

	// 5.关闭客户端
	for (int i = begin; i < end; i++) {
		client[i]->closeSocket();
	}
}

int main() {
	// 修改为多线程的情况

	// 1.分离接收用户命令的线程
	std::thread t1(cmdThread);
	t1.detach();
	
	// 2.分离发送数据的线程
	for (int n = 0; n < tCount; n++) {
		std::thread t1(sendThread, n + 1);
		t1.detach();
	}

	printf("已退出！\n");
	getchar();
	return 0;



	// ------------- 创建一个客户端--------------
	/*
	// 两个线程，子线程用来处理命令行输入的数据
	// 主线程用来接收客户端的返回消息
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
	*/


	// -------------- 创建一组客户端----------------
	/*  
	// 两个线程，子线程用来处理命令行输入的数据
	// 主线程用来接收客户端的返回消息
	//const int CCOUNT = 10;
	
	// // select单线程下最大只能支持64个连接
	const int CCOUNT = 100;

	// 这样创建的话,数据量很大，会导致栈空间溢出，所以使用指针的形式
	EasyTcpClient* client[CCOUNT];

	//int a = sizeof(EasyTcpClient);

	for (int i = 0; i < CCOUNT; i++) {
		
		//client[i]->connectServer("192.168.179.144", 4567);
		if (!g_bRun) {
			return 0;
		}
		client[i] = new EasyTcpClient();
	}

	for (int i = 0; i < CCOUNT; i++) {
		if (!g_bRun) {
			return 0;
		}
		client[i]->connectServer("192.168.31.146", 4567);
	}

	std::thread t1(cmdThread);
	t1.detach();
	Login login;
	strcpy(login.userName, "lisi");
	strcpy(login.passWord, "123456");
	while (g_bRun) {

		for (int i = 0; i < CCOUNT; i++) {
			//client[i].connectServer("172.20.10.6", 4567);
			client[i]->SendData(&login);   // 只做数据的发送
			//client[i]->onRun();    // 数据接收
		}

		//client.onRun();
		//client.SendData(&login);
	}
	for (int i = 0; i < CCOUNT; i++) {
		client[i]->closeSocket();
	}
	
	printf("已退出！\n");
	//getchar();
	return 0;
	*/

}