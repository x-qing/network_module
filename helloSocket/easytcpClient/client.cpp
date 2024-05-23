#define WIN32_LEAN_AND_MEAN   // 宏定义避免一些依赖库的引用冲突
#define _WINSOCK_DEPRECATED_NO_WARNINGS
//#pragma warning(disable:4996）

#include <Windows.h>
#include <WinSock2.h>
#include <stdio.h>
//#pragma comment(lib,"ws2_32.lib")


struct DataPackage {
	int age;
	char name[32];
};

// window下的socket环境
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
	// 可以显示声明用IPPROTO_TCP协议，也可以根据前面两个参数使用默认的协议。
	SOCKET _sock = socket(AF_INET, SOCK_STREAM, 0);
	if (_sock == SOCKET_ERROR) {
		printf("socket error!\n");
	}
	else {
		printf("socket sucess!\n");
	}

	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	// 服务器的ip和端口
	_sin.sin_port = htons(4567);
	_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	int _csock = connect(_sock, (sockaddr*)&_sin, sizeof(sockaddr_in));
	if (_csock == SOCKET_ERROR) {
		printf("connect error!\n");
	}
	else {
		printf("connect sucess!\n");
	}
	
	// 都是128，长度匹配
	
	while (true) {
		char cmdbuf[128] = { 0 };
		scanf("%s", cmdbuf);

		// 处理请求
		if (0 == strcmp(cmdbuf, "exit")) {
			//printf("client quit")
			printf("client quit! task over!\n");
			break;
		}
		else {
			// 发送命令
			send(_sock, cmdbuf, strlen(cmdbuf)+1, 0);
		}

		char recvbuf[128] = { 0 };

		int len = recv(_sock, recvbuf, 1024, 0);

		if (len > 0) {
			// 对接收到的数据进行处理
			DataPackage* dp = (DataPackage*)recvbuf;

			printf("recv data is: age=%d,name=%s\n", dp->age, dp->name);
		}
	}
	

	closesocket(_sock);

	WSACleanup();
	printf("quit over!!!\n");
	//getchar();
	getchar();
	return 0;
}