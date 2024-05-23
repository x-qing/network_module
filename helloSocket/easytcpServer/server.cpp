#define WIN32_LEAN_AND_MEAN   // 宏定义避免一些依赖库的引用冲突
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <Windows.h>
#include <WinSock2.h>
//#pragma comment(lib,"ws2_32.lib")
#include <stdio.h>
//#define _WINSOCK_DEPRECATED_NO_WARNINGS
// window下的socket环境

// 使用结构体,来进行消息的传递

enum CMD{
	CMD_LOGIN,
	CMD_LOGIN_RESULT,
	CMD_LOGINOUT,
	CMD_LOGINOUT_RESULT,
	CMD_ERROR
};

struct DataHeader {
	short dataLength; // 数据长度
	short cmd; // 命令
};

// 登录以及其返回的结果
// 如果使用结构体来进行，不利于扩展，这里直接使用继承的方式
struct Login : public DataHeader {
	//int age;
	//char name[32];
	Login() {
		dataLength = sizeof(Login);
		cmd = CMD_LOGIN;
	}
	//DataHeader header;
	char userName[32];
	char passWord[32];
};

struct LoginResult : public DataHeader {
	LoginResult() {
		dataLength = sizeof(LoginResult);
		cmd = CMD_LOGIN_RESULT;
		result = 0;   // 为0表示一切正常
	}
	int result;   // 登录的结果
};

// 登出以及其返回的结果
struct Loginout : public DataHeader {
	Loginout() {
		dataLength = sizeof(Loginout);
		cmd = CMD_LOGINOUT;
	}
	char userName[32];
};

struct LoginoutResult : public DataHeader {
	LoginoutResult() {
		dataLength = sizeof(LoginoutResult);
		cmd = CMD_LOGINOUT_RESULT;
		result = 0;
	}
	int result;   // 登录的结果
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
	printf("new client add:ip = %s,port = %d\n", inet_ntoa(clientAddr.sin_addr),
														(int)clientAddr.sin_port);

	// 接收缓冲区
	/*char recvBuf[128] = { 0 };*/
	//char recvBuf[128] = { 0 };


	// 修改了服务端，可以和客户端进行交互
	while (true) {
		DataHeader header = {};

		// 接收客户端的请求数据
		// 首先接收的是hander的长度，后续已经没有这么长了
		int len = recv(_cSock, (char *)&header, sizeof(DataHeader), 0);
		if (len <= 0) {
			printf("server quit! task over!\n");
			break;
		}
		//printf("recv data:cmd = %d, length = %d\n",header.cmd,header.dataLength);
		switch (header.cmd) {
		case CMD_LOGIN:
			{
				// 接收登录数据
				Login login = {};
				recv(_cSock, (char*)&login + sizeof(DataHeader), sizeof(Login) - sizeof(DataHeader), 0);
				printf("收到命令:CMD_LOGIN,数据长度 = %d,username=%s,password=%s\n", login.dataLength,login.userName,login.passWord);
				// 忽略判断用户名和密码
				// 定义消息头和返回值
				//DataHeader hd = {CMD_LOGIN,};
				LoginResult res;

				//send(_cSock, (const char*)&header, sizeof(DataHeader), 0);
				send(_cSock, (const char*)&res, sizeof(LoginResult), 0);
			}
			break;

		case CMD_LOGINOUT:
			{
				Loginout loginout = {};
				recv(_cSock, (char*)&loginout+sizeof(DataHeader), sizeof(Loginout)-sizeof(DataHeader), 0);
				printf("收到命令:CMD_LOGIN,数据长度 = %d,username=%s\n", loginout.dataLength, loginout.userName);
				// 忽略判断用户名和密码
				// 定义消息头和返回值
				//DataHeader hd = {CMD_LOGIN,};
				LoginoutResult res;

				//send(_cSock, (const char*)&header, sizeof(DataHeader), 0);
				send(_cSock, (const char*)&res, sizeof(LoginoutResult), 0);
			}
			break;
		default:
			{
				header.cmd = CMD_ERROR;
				header.dataLength = 0;
				send(_cSock, (const char*)&header, sizeof(DataHeader), 0);
			}
			break;
			//char msgBuf[128] = "???.";
			//send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);
		}
		// 处理请求
		//if (0 == strcmp(recvBuf, "getInfo")) {
		//	//char msgBuf[128] = "xiao ming";
		//	DataPackage dp = { 80,"xiaohong" };
		//	send(_cSock, (const char*)&dp, sizeof(DataPackage) + 1, 0);   // 把结尾符号0也发送过去
		//}
		//else if (0 == strcmp(recvBuf, "getAge")) {
		//	char msgBuf[128] = "18.";
		//	send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);   // 把结尾符号0也发送过去
		//}
		//else {
		//	char msgBuf[128] = "???.";
		//	send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);
		//}
		
		
		//char msgBuf[] = "hello client,i am server!\n";
		//send(_cSock, msgBuf, strlen(msgBuf)+1,0);   // 把结尾符号0也发送过去
	}


	closesocket(_sock);


	WSACleanup();

	printf("quit over!!!\n");
	getchar();
	return 0;
}