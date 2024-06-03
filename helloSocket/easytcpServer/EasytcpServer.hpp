#ifndef _EasytcpServer_hpp_

#define _EasytcpServer_hpp_

#define WIN32_LEAN_AND_MEAN   // 宏定义避免一些依赖库的引用冲突
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#ifdef _WIN32
#include <Windows.h>
#include <WinSock2.h>
#else
#include <arpa/inet.h>
#include <unistd.h>
#include<string.h>
#define SOCKET int
#define INVALID_SOCKET  (SOCKET)(~0)
#define SOCKET_ERROR            (-1)
#endif
//#pragma comment(lib,"ws2_32.lib")
#include <stdio.h>
#include <vector>
#include "messageheader.hpp"

class EasyTcpServer {
private:
	SOCKET _sock;
	std::vector<SOCKET> g_clients;
public:
	EasyTcpServer() {
		_sock = INVALID_SOCKET;
	}

	~EasyTcpServer() {
		closeSocket();
	}

	// 初始化socket
	SOCKET InitSocket() {
		// 启动Win sock 环境
#ifdef _WIN32
		WORD ver = MAKEWORD(2, 2);
		WSADATA dat;
		// 启动windows的socket网络环境
		WSAStartup(ver, &dat);
#endif
		if (_sock != INVALID_SOCKET) {
			printf("<socket=%d>关闭旧连接！\n", _sock);
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

		return _sock;
	}

	// 绑定端口号
	int BindSocket(const char* ip, unsigned short port) {
		sockaddr_in _sin = {};
		_sin.sin_family = AF_INET;
		_sin.sin_port = htons(port);  // 主机到网络字节序的转换

		// 指定ip地址
#ifdef WIN32
	//_sin.sin_addr.S_un.S_addr = inet_addr("172.29.140.202");
		_sin.sin_addr.S_un.S_addr = inet_addr(ip);
#else
		_sin.sin_addr.s_addr = inet_addr("192.168.179.144");
#endif
		//_sin.sin_addr.S_un.S_addr = INADDR_ANY;  // 表示支持本机上的所有ip
		int ret = bind(_sock, (sockaddr*)&_sin, sizeof(sockaddr_in));
		if (ret == SOCKET_ERROR) {
			// 绑定错误
			printf("错误，绑定端口<%d>失败...!\n", port);
			return 0;
		}
		else {
			printf("绑定端口<%d>成功...!\n", port);
		}
		return ret;
	}

	// 监听端口号
	int ListrnSocket(int num) {
		int ret = listen(_sock, num);
		if (ret == SOCKET_ERROR) {
			printf("错误，监听端口失败...!\n");
			return 0;
		}
		else {
			printf("监听网络端口成功...!\n");
		}
		return ret;
	}

	// 接收客户端连接
	SOCKET AcceptSocket() {
		sockaddr_in clientAddr = {};
		int naddrLen = sizeof(clientAddr);

		SOCKET _cSock = INVALID_SOCKET;   // 初始化为一个无效的socket
		//char msgBuf[] = "hello client,i am server!\n";
#ifdef _WIN32

		_cSock = accept(_sock, (sockaddr*)&clientAddr, &naddrLen);
#else
		_cSock = accept(_sock, (sockaddr*)&clientAddr, (socklen_t*)&naddrLen);
#endif
		// 将新加入的客户端保存起来			
		if (_cSock == SOCKET_ERROR) {
			printf("socket=<%d>错误，接收到无效客户端...!\n", _sock);
		}
		else {
			printf("socket=<%d>成功，新客户端加入:socket=%d,ip = %s,port = %d\n", _sock, _cSock, inet_ntoa(clientAddr.sin_addr), (int)clientAddr.sin_port);
			//for (size_t n = 0; n < g_clients.size(); n++) {
			//	NewUserJoin userjoin = {};
			//	userjoin.sock = _cSock;
			//	send(g_clients[n], (const char*)&userjoin, sizeof(NewUserJoin), 0);
			//}
			NewUserJoin userjoin = {};
			SendDataToAll(&userjoin);
			g_clients.push_back(_cSock);
		}
		return _cSock;
	}

	// 关闭socket
	void closeSocket() {
		if (_sock != INVALID_SOCKET) {
			for (size_t n = 0; n < g_clients.size() - 1; n++) {
				//FD_SET(g_clients[n], &fd_read);
#ifdef _WIN32
				closesocket(g_clients[n]);
#else
				close(g_clients[n]);
#endif
			}

#ifdef _WIN32
			closesocket(_sock);
			WSACleanup();
#else
			close(_sock);
#endif
		}
	}

	// 是否在工作中
	bool isRun() {
		return _sock != INVALID_SOCKET;
	}

	// 接收数据  处理粘包  拆分包
	bool OnRun() {
		if (isRun()) {
			fd_set fd_read;
			fd_set fd_write;
			fd_set fd_error;
			// 清空
			FD_ZERO(&fd_read);
			FD_ZERO(&fd_write);
			FD_ZERO(&fd_error);
			//设置把fd放到集合里面
			FD_SET(_sock, &fd_read);
			FD_SET(_sock, &fd_write);
			FD_SET(_sock, &fd_error);

			// 把接收文件描述符加入进去
			for (size_t n = 0; n < g_clients.size(); n++) {
				FD_SET(g_clients[n], &fd_read);
			}
			timeval t = { 1,0 };
			// 非阻塞的情况，可以加一些其它业务处理，然后再查询。
			// int ret = select(_sock + 1, &fd_read, &fd_write, &fd_error, &t);
			int max_fd = _sock;
			for (size_t n = 0; n < g_clients.size(); n++) {
				if (g_clients[n] > max_fd) {
					max_fd = g_clients[n];
				}
			}
			// windows和linux对于select的第一个参数的处理有些不一样。windows对于select的第一个参数，不怎么关注，
			// 而linux对于第一个参数表示的是监听文件描述符的一个范围。
			int ret = select(max_fd + 1, &fd_read, nullptr, nullptr, &t);
			if (ret < 0) {
				printf("select任务结束!\n");
				closeSocket();
				return false;
			}
			if (FD_ISSET(_sock, &fd_read)) {   // 表示有新的连接来了
				FD_CLR(_sock, &fd_read);
				AcceptSocket();
				// printf("--------%zu--------------\n",g_clients.size());
			}

#ifdef _WIN32
			for (size_t n = 0; n < fd_read.fd_count; n++) {
				if (-1 == RecvData(fd_read.fd_array[n])) {
					// 客户端退出
					auto iter = find(g_clients.begin(), g_clients.end(), fd_read.fd_array[n]);
					if (iter != g_clients.end()) {
						g_clients.erase(iter);
					}
				}
			}
#else
			for (size_t n = 0; n < g_clients.size(); n++) {
				if (FD_ISSET(g_clients[n], &fd_read)) {
					// if (processfd(g_clients[n]) == -1) {
					// 	// Client disconnected
					// 	close(g_clients[n]);
					// 	g_clients.erase(g_clients.begin() + n);
					// 	n--; // Adjust the index after erase
					// }
					if (-1 == RecvData(g_clients[n])) {
						// auto iter = find(g_clients.begin(), g_clients.end(), fd_read.fd_array[n]);
						// if (iter != g_clients.end()) {
						//     g_clients.erase(iter);
						// }
						g_clients.erase(g_clients.begin() + n);
						n--;
					}
				}
			}
#endif
			return true;
		}

		return false;
	}

	// 接收数据
	int RecvData(SOCKET _cSock) {
		// 定义一个缓冲来接收数据
		char szRecv[4096] = {};
		int len = recv(_cSock, szRecv, sizeof(DataHeader), 0);
		DataHeader* header = (DataHeader*)szRecv;
		//int len = recv(_cSock, (char *)&header, sizeof(DataHeader), 0);
		if (len <= 0) {
			printf("server quit! task over!\n");
			return -1;
		}
		recv(_cSock, szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);
		OnNetMsg(_cSock, header);
		return 0;
	}

	// 响应网络消息
	// 服务端需要与多个sock来建立连接，这里要传入一个socket参数

	// 这里的DataHeader* header感觉有点问题
	void OnNetMsg(SOCKET _cSock, DataHeader* header) {

		switch (header->cmd) {
		case CMD_LOGIN:
		{
			// 接收登录数据
			Login* login = (Login*)header;
			printf("收到命令来自（%d）:CMD_LOGIN,数据长度 = %d,username=%s,password=%s\n", _cSock, login->dataLength, login->userName, login->passWord);
			LoginResult res;
			//send(_cSock, (const char*)&res, sizeof(LoginResult), 0);
			SendData(_cSock, &res);
		}
		break;

		case CMD_LOGINOUT:
		{
			Loginout* loginout = (Loginout*)header;
			printf("收到命令来自（%d）:CMD_LOGIN,数据长度 = %d,username=%s\n", _cSock, loginout->dataLength, loginout->userName);
			LoginoutResult res;
			//send(_cSock, (const char*)&res, sizeof(LoginoutResult), 0);
			SendData(_cSock, &res);
		}
		break;
		default:
		{
			DataHeader header = { 0,CMD_ERROR };
			//send(_cSock, (const char*)&header, sizeof(DataHeader), 0);
			SendData(_cSock, &header);
		}
		break;
		}
	}

	// 发送给指定socket的数据
	int SendData(SOCKET _cSock, DataHeader* header) {
		if (isRun() && header) {
			send(_cSock, (const char*)header, header->dataLength, 0);
		}
		return SOCKET_ERROR;
	}

	// 还有一个群发的功能
	void SendDataToAll(DataHeader* header) {


		if (isRun() && header) {
			for (size_t n = 0; n < g_clients.size(); n++) {
				SendData(g_clients[n], header);
			}
		}
	}

private:

};


#endif
