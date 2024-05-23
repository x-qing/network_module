#define WIN32_LEAN_AND_MEAN   // 宏定义避免一些依赖库的引用冲突
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <Windows.h>
#include <WinSock2.h>
//#pragma comment(lib,"ws2_32.lib")
#include <stdio.h>
//#define _WINSOCK_DEPRECATED_NO_WARNINGS
// window下的socket环境

// 使用结构体,来进行消息的传递
struct DataPackage {
	int age;
	char name[32];
};

int main() {
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	// 启动windows的socket网络环境
	WSAStartup(ver, &dat);
	/*
	* socket简易tcp服务端
	* socket -> bind -> listen -> accept -> read -> recv ->close
	*
	* socket建议tcp服务端
	* socket -> connect -> read -> recv -> close
	*/

	// 协议类型 ipv4
	// 类型：数据流
	// 什么协议：tcp
	SOCKET _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(4567);  // 主机到网络字节序的转换
	_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	//_sin.sin_addr.S_un.S_addr = INADDR_ANY;  // 表示支持本机上的所有ip
	if (bind(_sock, (sockaddr*)&_sin, sizeof(sockaddr_in)) == SOCKET_ERROR) {
		// 绑定错误
		printf("bind error!\n");
		return 0;
	}
	else {
		printf("bind sucess!\n");
	}

	if (listen(_sock, 5) == SOCKET_ERROR) {
		printf("listen error!\n");
		return 0;
	}
	else {
		printf("listen sucess!\n");
	}

	sockaddr_in clientAddr = {};
	int naddrLen = sizeof(clientAddr);

	SOCKET _cSock = INVALID_SOCKET;   // 初始化为一个无效的socket
	//char msgBuf[] = "hello client,i am server!\n";
	_cSock = accept(_sock, (sockaddr*)&clientAddr, &naddrLen);

	if (_cSock == SOCKET_ERROR) {
		printf("accept error!\n");
	}
	else {
		printf("accept sucess!\n");
	}
	printf("new client add:ip = %s,port = %d\n", inet_ntoa(clientAddr.sin_addr), (int)clientAddr.sin_port);

	// 接收缓冲区
	/*char recvBuf[128] = { 0 };*/
	char recvBuf[128] = { 0 };

	// 修改了服务端，可以和客户端进行交互
	while (true) {
		// 接收客户端的请求数据
		int len = recv(_cSock, recvBuf, 128, 0);
		if (len <= 0) {
			printf("server quit! task over!\n");
			break;
		}
		printf("recv data from client:%s\n",recvBuf);
		// 处理请求
		if (0 == strcmp(recvBuf, "getInfo")) {
			//char msgBuf[128] = "xiao ming";
			DataPackage dp = { 80,"xiaohong" };
			send(_cSock, (const char*)&dp, sizeof(DataPackage) + 1, 0);   // 把结尾符号0也发送过去
		}
		//else if (0 == strcmp(recvBuf, "getAge")) {
		//	char msgBuf[128] = "18.";
		//	send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);   // 把结尾符号0也发送过去
		//}
		else {
			char msgBuf[128] = "???.";
			send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);
		}
		
		
		//char msgBuf[] = "hello client,i am server!\n";
		//send(_cSock, msgBuf, strlen(msgBuf)+1,0);   // 把结尾符号0也发送过去
	}


	closesocket(_sock);


	WSACleanup();

	printf("quit over!!!\n");
	getchar();
	return 0;
}