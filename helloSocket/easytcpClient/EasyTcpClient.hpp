#define WIN32_LEAN_AND_MEAN   // 宏定义避免一些依赖库的引用冲突
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#ifndef _EasyTcpClient_hpp_
#define _EasyTcpClient_hpp_

#ifdef _WIN32
	#include <Windows.h>
	#include <WinSock2.h>
#else
	#include<unistd.h>
	#include<arpa/inet.h>
	#include<string.h>
	#define SOCKET int
	#define INVALID_SOCKET  (SOCKET)(~0)
	#define SOCKET_ERROR            (-1)
#endif

#include <stdio.h>
#include <thread>
#include "MessageHeader.hpp"

class EasyTcpClient {
public:
	EasyTcpClient() {
		_sock = INVALID_SOCKET;  // 初始化为一个无效的socket
	}

	// 虚析构函数
	virtual ~EasyTcpClient() {
		closeSocket();
	}

	// 初始化，连接和关闭
	void initSocket() {
		// 启动Win sock 环境
	#ifdef _WIN32
		WORD ver = MAKEWORD(2, 2);
		WSADATA dat;
		// 启动windows的socket网络环境
		WSAStartup(ver, &dat);
	#endif
		if (_sock != INVALID_SOCKET) {
			printf("<socket=%d>关闭旧连接！\n",_sock);
			closeSocket();
		}
		else {
			_sock = socket(AF_INET, SOCK_STREAM, 0);
			if (_sock == SOCKET_ERROR) {
				printf("错误，创建socket失败!\n");
			}
			else {
				printf("创建socket成功!\n");
			}
		}

	}

	// 连接服务器
	int connectServer(const char* ip, unsigned short port) {
		if (_sock == INVALID_SOCKET) {
			initSocket();
		}
		sockaddr_in _sin = {};
		_sin.sin_family = AF_INET;
		// 服务器的ip和端口
		_sin.sin_port = htons(port);
	#ifdef _WIN32
		_sin.sin_addr.S_un.S_addr = inet_addr(ip);
	#else
		_sin.sin_addr.s_addr = inet_addr(ip);
	#endif
		int ret = connect(_sock, (sockaddr*)&_sin, sizeof(sockaddr_in));
		if (ret == SOCKET_ERROR) {
			printf("错误，连接服务器失败...!\n");
		}
		else {
			printf("连接服务器成功...!\n");
		}
		return ret;
	}

	void closeSocket() {
		if (_sock != INVALID_SOCKET) {
		// 关闭Win sock 环境
		#ifdef _WIN32
			closesocket(_sock);
			WSACleanup();
		#else
			close(_sock);
		#endif
			printf("关闭socket!!!\n");
			_sock = INVALID_SOCKET;
		}
	}




	// 处理网络消息
	bool onRun() {
		if (isRun()) {
			fd_set fd_read;
			FD_ZERO(&fd_read);
			FD_SET(_sock, &fd_read);
			timeval time = { 1, 0 };
			int ret = select(_sock + 1, &fd_read, nullptr, nullptr, &time);
			if (ret < 0) {
				printf("<socket=%d>select任务结束\n",_sock) ;
				return false;
			}
			if (FD_ISSET(_sock, &fd_read)) {
				FD_CLR(_sock, &fd_read);
				if (-1 == RecvData(_sock)) { // 有数据可以读，需要进行处理
					printf("<socket=%d>select任务结束2\n", _sock);
					return false;
				}
			}
			return true;
		}
		return false;
	}
		
	bool isRun() {
		return _sock != INVALID_SOCKET;
	}

	
	// 接收数据，要处理粘包 拆分包
	int RecvData(SOCKET _cSock) {
		char szRecv[1024] = {};
		int len = recv(_cSock, szRecv, sizeof(DataHeader), 0);
		DataHeader* header = (DataHeader*)szRecv;
		//int len = recv(_cSock, (char *)&header, sizeof(DataHeader), 0);
		if (len <= 0) {
			printf("与服务器断开连接!\n");
			return -1;
		}
		recv(_cSock, szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);
		OnNetMsg(header);
		return 0;
	}

	// 响应网络消息  -- 基类的指针
	void OnNetMsg(DataHeader* header) {
		// 定义一个缓冲来接收数据
		switch (header->cmd) {
		case CMD_LOGIN_RESULT:
		{
			// 接收数据
			LoginResult* login = (LoginResult*)header;
			printf("收到服务返回:CMD_LOGIN_RESULT,数据长度 = %d,result = %d\n", login->dataLength, login->result);
		}
		break;

		case CMD_LOGINOUT_RESULT:
		{
			// 接收数据
			LoginoutResult* login = (LoginoutResult*)header;
			printf("收到服务返回:CMD_LOGINOUT_RESULT,数据长度 = %d,result = %d\n", login->dataLength, login->result);
		}
		break;
		case CMD_NEW_USER_JOIN:
		{
			// 接收数据
			NewUserJoin* login = (NewUserJoin*)header;
			printf("收到服务返回:CMD_NEW_USER_JOIN,数据长度 = %d,sock = %d\n", login->dataLength, login->sock);
		}
		break;
		}
	}


	// 发送数据
	int SendData(DataHeader* header) {
		if (isRun() && header) {
			send(_sock, (const char*)header, header->dataLength, 0);
		}
		return SOCKET_ERROR;
	}

private:
	SOCKET _sock;
};

#endif
