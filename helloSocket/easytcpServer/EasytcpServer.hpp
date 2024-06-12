#ifndef _EasytcpServer_hpp_

#define _EasytcpServer_hpp_

#define WIN32_LEAN_AND_MEAN   // �궨�����һЩ����������ó�ͻ
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

#ifndef RECV_BUFF_SIZE
#define RECV_BUFF_SIZE 10240
#endif

class ClientSocket {
public:
	ClientSocket(SOCKET sockfd = INVALID_SOCKET) {
		_sockfd = sockfd;
		memset(_szMsgBuf, 0, sizeof(_szMsgBuf));
		_lastPos = 0;
	}

	SOCKET getsockfd() {
		return _sockfd;
	}

	char* getmsgbuf() {
		return _szMsgBuf;
	}

	int getlastpos() {
		return _lastPos;
	}

	void setlastpos(int pos) {
		_lastPos = pos;
	}

private:
	SOCKET _sockfd;
	// ��Ϣ������
	char _szMsgBuf[RECV_BUFF_SIZE * 10];
	int _lastPos;   // ���ڶ�λ
	

};

class EasyTcpServer {
private:
	SOCKET _sock;

	// ����ͨ��һ��_sock��˵�ǲ������ˣ�������Ҫ����������µ�һ����װ��
	//std::vector<SOCKET> g_clients;   // ͨ��һ��vector���������Կͻ��˵�����

	// ʹ��ָ�룬��Ҫ��Ϊ�˱�֤�ڽ������ݿ���������ʱ���㣬ͬʱҲ�ܱ�֤ջ�ռ�ı���
	std::vector<ClientSocket*> g_clients;
public:
	EasyTcpServer() {
		_sock = INVALID_SOCKET;
	}

	~EasyTcpServer() {
		closeSocket();
	}

	// ��ʼ��socket
	SOCKET InitSocket() {
		// ����Win sock ����
#ifdef _WIN32
		WORD ver = MAKEWORD(2, 2);
		WSADATA dat;
		// ����windows��socket���绷��
		WSAStartup(ver, &dat);
#endif
		if (_sock != INVALID_SOCKET) {
			printf("<socket=%d>�رվ����ӣ�\n", _sock);
			closeSocket();
		}
		else {
			_sock = socket(AF_INET, SOCK_STREAM, 0);
			if (_sock == SOCKET_ERROR) {
				printf("���󣬴���socketʧ��!\n");
			}
			else {
				printf("����socket�ɹ�!\n");
			}
		}

		return _sock;
	}

	// �󶨶˿ں�
	int BindSocket(const char* ip, unsigned short port) {
		sockaddr_in _sin = {};
		_sin.sin_family = AF_INET;
		_sin.sin_port = htons(port);  // �����������ֽ����ת��

		// ָ��ip��ַ
#ifdef WIN32
	//_sin.sin_addr.S_un.S_addr = inet_addr("172.29.140.202");
		_sin.sin_addr.S_un.S_addr = inet_addr(ip);
#else
		_sin.sin_addr.s_addr = inet_addr("192.168.179.144");
#endif
		//_sin.sin_addr.S_un.S_addr = INADDR_ANY;  // ��ʾ֧�ֱ����ϵ�����ip
		int ret = bind(_sock, (sockaddr*)&_sin, sizeof(sockaddr_in));
		if (ret == SOCKET_ERROR) {
			// �󶨴���
			printf("���󣬰󶨶˿�<%d>ʧ��...!\n", port);
			return 0;
		}
		else {
			printf("�󶨶˿�<%d>�ɹ�...!\n", port);
		}
		return ret;
	}

	// �����˿ں�
	int ListrnSocket(int num) {
		int ret = listen(_sock, num);
		if (ret == SOCKET_ERROR) {
			printf("���󣬼����˿�ʧ��...!\n");
			return 0;
		}
		else {
			printf("��������˿ڳɹ�...!\n");
		}
		return ret;
	}

	// ���տͻ�������
	SOCKET AcceptSocket() {
		sockaddr_in clientAddr = {};
		int naddrLen = sizeof(clientAddr);

		SOCKET cSock = INVALID_SOCKET;   // ��ʼ��Ϊһ����Ч��socket
		//char msgBuf[] = "hello client,i am server!\n";
#ifdef _WIN32

		cSock = accept(_sock, (sockaddr*)&clientAddr, &naddrLen);
#else
		cSock = accept(_sock, (sockaddr*)&clientAddr, (socklen_t*)&naddrLen);
#endif
		// ���¼���Ŀͻ��˱�������			
		if (cSock == SOCKET_ERROR) {
			printf("socket=<%d>���󣬽��յ���Ч�ͻ���...!\n", _sock);
		}
		else {
			printf("socket=<%d>�ɹ����¿ͻ��˼���:socket=%d,ip = %s,port = %d\n", _sock, cSock, inet_ntoa(clientAddr.sin_addr), (int)clientAddr.sin_port);
			//for (size_t n = 0; n < g_clients.size(); n++) {
			//	NewUserJoin userjoin = {};
			//	userjoin.sock = _cSock;
			//	send(g_clients[n], (const char*)&userjoin, sizeof(NewUserJoin), 0);
			//}
			NewUserJoin userjoin = {};
			SendDataToAll(&userjoin);
			
			// ������ʵҪ�ж�new�Ƿ�ɹ�����ʧ��
			g_clients.push_back(new ClientSocket(cSock));
			//g_clients.push_back(_cSock);
		}
		return cSock;
	}

	// �ر�socket
	void closeSocket() {
		if (_sock != INVALID_SOCKET) {
			for (size_t n = 0; n < g_clients.size() - 1; n++) {
				//FD_SET(g_clients[n], &fd_read);
#ifdef _WIN32
				closesocket(g_clients[n]->getsockfd());
#else
				close(g_clients[n]->getsockfd());
#endif
				delete g_clients[n];    // ɾ��
			}  

#ifdef _WIN32
			closesocket(_sock);
			WSACleanup();
#else
			close(_sock);
#endif
			g_clients.clear();
		}
	}

	// �Ƿ��ڹ�����
	bool isRun() {
		return _sock != INVALID_SOCKET;
	}

	// ��������  ����ճ��  ��ְ�
	bool OnRun() {
		if (isRun()) {
			fd_set fd_read;
			fd_set fd_write;
			fd_set fd_error;
			// ���
			FD_ZERO(&fd_read);
			FD_ZERO(&fd_write);
			FD_ZERO(&fd_error);
			//���ð�fd�ŵ���������
			FD_SET(_sock, &fd_read);
			FD_SET(_sock, &fd_write);
			FD_SET(_sock, &fd_error);

			// �ѽ����ļ������������ȥ
			for (size_t n = 0; n < g_clients.size(); n++) {
				FD_SET(g_clients[n]->getsockfd(), &fd_read);
			}
			timeval t = { 1,0 };
			// ����������������Լ�һЩ����ҵ����Ȼ���ٲ�ѯ��
			// int ret = select(_sock + 1, &fd_read, &fd_write, &fd_error, &t);
			int max_fd = _sock;
			for (size_t n = 0; n < g_clients.size(); n++) {
				if (g_clients[n]->getsockfd() > max_fd) {
					max_fd = g_clients[n]->getsockfd();
				}
			}
			// windows��linux����select�ĵ�һ�������Ĵ�����Щ��һ����windows����select�ĵ�һ������������ô��ע��
			// ��linux���ڵ�һ��������ʾ���Ǽ����ļ���������һ����Χ��
			int ret = select(max_fd + 1, &fd_read, nullptr, nullptr, &t);
			if (ret < 0) {
				printf("select�������!\n");
				closeSocket();
				return false;
			}
			if (FD_ISSET(_sock, &fd_read)) {   // ��ʾ���µ���������
				FD_CLR(_sock, &fd_read);
				AcceptSocket();
				// printf("--------%zu--------------\n",g_clients.size());
			}

//#ifdef _WIN32
			//for (size_t n = 0; n < fd_read.fd_count; n++) {
			//	if (-1 == RecvData(fd_read.fd_array[n])) {
			//		// �ͻ����˳�
			//		auto iter = find(g_clients.begin(), g_clients.end(), fd_read.fd_array[n]);
			//		if (iter != g_clients.end()) {
			//			g_clients.erase(iter);
			//		}
			//	}
			//}
//#else
			for (size_t n = 0; n < g_clients.size(); n++) {
				if (FD_ISSET(g_clients[n]->getsockfd(), &fd_read)) {
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

						// ������������Ҫ�Ƴ�����ͬʱҲҪɾ������Ŀռ�
						auto iter = g_clients.begin() + n;
						if (iter != g_clients.end()) {
							delete g_clients[n]; 
							g_clients.erase(iter);
							n--;
						}
					}
				}
			}
//#endif
			return true;
		}

		return false;
	}

	char _szRecv[RECV_BUFF_SIZE] = {};
	// ��������
	int RecvData(ClientSocket* pClient) {
		 
		// ���տͻ��˵�����
		int nlen = recv(pClient->getsockfd(), _szRecv, RECV_BUFF_SIZE, 0);
		DataHeader* header = (DataHeader*)_szRecv;
		//int len = recv(_cSock, (char *)&header, sizeof(DataHeader), 0);
		if (nlen <= 0) {
			//printf("server quit! task over!\n");
			printf("�ͻ���<socket=%d>���˳������������\n", pClient->getsockfd());
			return -1;
		}
		//recv(_cSock, szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);
		//OnNetMsg(_cSock, header);
		//return 0;

		memcpy(pClient->getmsgbuf() + pClient->getlastpos(), _szRecv, nlen);
		//pClient->getlastpos() += nlen;
		pClient->setlastpos(pClient->getlastpos() + nlen);

		// ��ǰ�����ݳ��ȴ�����Ϣͷ���ˣ�����֪��Ҫ�������Ϣ�ĳ���
		while (pClient->getlastpos() >= sizeof(DataHeader)) {
			DataHeader* header = (DataHeader*)pClient->getmsgbuf();
			if (pClient->getlastpos() >= header->dataLength) {

				// ʣ��δ�������Ϣ���������ݵĳ���
				int nSize = pClient->getlastpos() - header->dataLength;
				// ����������Ϣ
				OnNetMsg(pClient->getsockfd(),header);
				// ����ǰ��
				//memcpy(_szMsgBuf, _szMsgBuf + header->dataLength, _lastPos - header->dataLength);
				//_lastPos = nSize;
				memcpy(pClient->getmsgbuf(), pClient->getmsgbuf() + header->dataLength, nSize);
				pClient->setlastpos(nSize);
			}
			else {
				//ʣ������ݲ���һ����������Ϣ
				break;
			}
		}

		return 0;
	}

	// ��Ӧ������Ϣ
	// �������Ҫ����sock���������ӣ�����Ҫ����һ��socket����

	// �����DataHeader* header�о��е�����
	void OnNetMsg(SOCKET cSock, DataHeader* header) {

		switch (header->cmd) {
		case CMD_LOGIN:
		{
			// ���յ�¼����
			Login* login = (Login*)header;
			printf("�յ��������ԣ�%d��:CMD_LOGIN,���ݳ��� = %d,username=%s,password=%s\n", cSock, login->dataLength, login->userName, login->passWord);
			LoginResult res;
			//send(_cSock, (const char*)&res, sizeof(LoginResult), 0);
			SendData(cSock, &res);
		}
		break;

		case CMD_LOGINOUT:
		{
			Loginout* loginout = (Loginout*)header;
			printf("�յ��������ԣ�%d��:CMD_LOGIN,���ݳ��� = %d,username=%s\n", cSock, loginout->dataLength, loginout->userName);
			LoginoutResult res;
			//send(_cSock, (const char*)&res, sizeof(LoginoutResult), 0);
			SendData(cSock, &res);
		}
		break;
		default:
		{
			//DataHeader header = { 0,CMD_ERROR };
			//send(_cSock, (const char*)&header, sizeof(DataHeader), 0);
			//SendData(_cSock, &header);
			printf("<socket=%d>�յ�δ������Ϣ,���ݳ��� = %d\n", cSock, header->dataLength);
			DataHeader ret;
			SendData(cSock, &ret);


		}
		break;
		}
	}

	// ���͸�ָ��socket������
	int SendData(SOCKET cSock, DataHeader* header) {
		if (isRun() && header) {
			send(cSock, (const char*)header, header->dataLength, 0);
		}
		return SOCKET_ERROR;
	}

	// ����һ��Ⱥ���Ĺ���
	void SendDataToAll(DataHeader* header) {


		if (isRun() && header) {
			for (size_t n = 0; n < g_clients.size(); n++) {
				SendData(g_clients[n]->getsockfd(), header);
			}
		}
	}

private:

};


#endif
