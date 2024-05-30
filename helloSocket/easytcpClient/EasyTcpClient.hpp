#define WIN32_LEAN_AND_MEAN   // �궨�����һЩ����������ó�ͻ
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
		_sock = INVALID_SOCKET;  // ��ʼ��Ϊһ����Ч��socket
	}

	// ����������
	virtual ~EasyTcpClient() {
		closeSocket();
	}

	// ��ʼ�������Ӻ͹ر�
	void initSocket() {
		// ����Win sock ����
	#ifdef _WIN32
		WORD ver = MAKEWORD(2, 2);
		WSADATA dat;
		// ����windows��socket���绷��
		WSAStartup(ver, &dat);
	#endif
		if (_sock != INVALID_SOCKET) {
			printf("<socket=%d>�رվ����ӣ�\n",_sock);
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

	}

	// ���ӷ�����
	int connectServer(const char* ip, unsigned short port) {
		if (_sock == INVALID_SOCKET) {
			initSocket();
		}
		sockaddr_in _sin = {};
		_sin.sin_family = AF_INET;
		// ��������ip�Ͷ˿�
		_sin.sin_port = htons(port);
	#ifdef _WIN32
		_sin.sin_addr.S_un.S_addr = inet_addr(ip);
	#else
		_sin.sin_addr.s_addr = inet_addr(ip);
	#endif
		int ret = connect(_sock, (sockaddr*)&_sin, sizeof(sockaddr_in));
		if (ret == SOCKET_ERROR) {
			printf("�������ӷ�����ʧ��...!\n");
		}
		else {
			printf("���ӷ������ɹ�...!\n");
		}
		return ret;
	}

	void closeSocket() {
		if (_sock != INVALID_SOCKET) {
		// �ر�Win sock ����
		#ifdef _WIN32
			closesocket(_sock);
			WSACleanup();
		#else
			close(_sock);
		#endif
			printf("�ر�socket!!!\n");
			_sock = INVALID_SOCKET;
		}
	}




	// ����������Ϣ
	bool onRun() {
		if (isRun()) {
			fd_set fd_read;
			FD_ZERO(&fd_read);
			FD_SET(_sock, &fd_read);
			timeval time = { 1, 0 };
			int ret = select(_sock + 1, &fd_read, nullptr, nullptr, &time);
			if (ret < 0) {
				printf("<socket=%d>select�������\n",_sock) ;
				return false;
			}
			if (FD_ISSET(_sock, &fd_read)) {
				FD_CLR(_sock, &fd_read);
				if (-1 == RecvData(_sock)) { // �����ݿ��Զ�����Ҫ���д���
					printf("<socket=%d>select�������2\n", _sock);
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

	
	// �������ݣ�Ҫ����ճ�� ��ְ�
	int RecvData(SOCKET _cSock) {
		char szRecv[1024] = {};
		int len = recv(_cSock, szRecv, sizeof(DataHeader), 0);
		DataHeader* header = (DataHeader*)szRecv;
		//int len = recv(_cSock, (char *)&header, sizeof(DataHeader), 0);
		if (len <= 0) {
			printf("��������Ͽ�����!\n");
			return -1;
		}
		recv(_cSock, szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);
		OnNetMsg(header);
		return 0;
	}

	// ��Ӧ������Ϣ  -- �����ָ��
	void OnNetMsg(DataHeader* header) {
		// ����һ����������������
		switch (header->cmd) {
		case CMD_LOGIN_RESULT:
		{
			// ��������
			LoginResult* login = (LoginResult*)header;
			printf("�յ����񷵻�:CMD_LOGIN_RESULT,���ݳ��� = %d,result = %d\n", login->dataLength, login->result);
		}
		break;

		case CMD_LOGINOUT_RESULT:
		{
			// ��������
			LoginoutResult* login = (LoginoutResult*)header;
			printf("�յ����񷵻�:CMD_LOGINOUT_RESULT,���ݳ��� = %d,result = %d\n", login->dataLength, login->result);
		}
		break;
		case CMD_NEW_USER_JOIN:
		{
			// ��������
			NewUserJoin* login = (NewUserJoin*)header;
			printf("�յ����񷵻�:CMD_NEW_USER_JOIN,���ݳ��� = %d,sock = %d\n", login->dataLength, login->sock);
		}
		break;
		}
	}


	// ��������
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
