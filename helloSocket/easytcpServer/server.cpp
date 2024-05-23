#define WIN32_LEAN_AND_MEAN   // �궨�����һЩ����������ó�ͻ
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <Windows.h>
#include <WinSock2.h>
//#pragma comment(lib,"ws2_32.lib")
#include <stdio.h>
//#define _WINSOCK_DEPRECATED_NO_WARNINGS
// window�µ�socket����

// ʹ�ýṹ��,��������Ϣ�Ĵ���

enum CMD{CMD_LOGIN,CMD_LOGINOUT,CMD_ERROR};

struct DataHeader {
	short dataLength; // ���ݳ���
	short cmd; // ����
};

// ��¼�Լ��䷵�صĽ��
struct Login {
	//int age;
	//char name[32];
	char userName[32];
	char passWord[32];
};

struct LoginResult {
	int result;   // ��¼�Ľ��
};

// �ǳ��Լ��䷵�صĽ��
struct Loginout {
	char userName[32];
};

struct LoginoutResult {
	int result;   // ��¼�Ľ��
};

int main() {
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	// ����windows��socket���绷��
	WSAStartup(ver, &dat);
	/*
	* socket����tcp�����
	* socket -> bind -> listen -> accept -> read -> recv ->close
	*
	* socket����tcp�����
	* socket -> connect -> read -> recv -> close
	*/

	// Э������ ipv4
	// ���ͣ�������
	// ʲôЭ�飺tcp
	SOCKET _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(4567);  // �����������ֽ����ת��
	_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	//_sin.sin_addr.S_un.S_addr = INADDR_ANY;  // ��ʾ֧�ֱ����ϵ�����ip
	if (bind(_sock, (sockaddr*)&_sin, sizeof(sockaddr_in)) == SOCKET_ERROR) {
		// �󶨴���
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

	SOCKET _cSock = INVALID_SOCKET;   // ��ʼ��Ϊһ����Ч��socket
	//char msgBuf[] = "hello client,i am server!\n";
	_cSock = accept(_sock, (sockaddr*)&clientAddr, &naddrLen);

	if (_cSock == SOCKET_ERROR) {
		printf("accept error!\n");
	}
	else {
		printf("accept sucess!\n");
	}
	printf("new client add:ip = %s,port = %d\n", inet_ntoa(clientAddr.sin_addr), (int)clientAddr.sin_port);

	// ���ջ�����
	/*char recvBuf[128] = { 0 };*/
	//char recvBuf[128] = { 0 };


	// �޸��˷���ˣ����ԺͿͻ��˽��н���
	while (true) {
		DataHeader header = {};

		// ���տͻ��˵���������
		int len = recv(_cSock, (char *)&header, sizeof(DataHeader), 0);
		if (len <= 0) {
			printf("server quit! task over!\n");
			break;
		}
		printf("recv data:cmd = %d, length = %d\n",header.cmd,header.dataLength);
		switch (header.cmd) {
		case CMD_LOGIN:
			{
				Login login = {};
				recv(_cSock, (char*)&login, sizeof(Login), 0);
				// �����ж��û���������
				// ������Ϣͷ�ͷ���ֵ
				//DataHeader hd = {CMD_LOGIN,};
				LoginResult res = { 1 };

				send(_cSock, (const char*)&header, sizeof(DataHeader), 0);
				send(_cSock, (const char*)&res, sizeof(LoginResult), 0);
			}
			break;

		case CMD_LOGINOUT:
			{
				Loginout loginout = {};
				recv(_cSock, (char*)&loginout, sizeof(Loginout), 0);
				// �����ж��û���������
				// ������Ϣͷ�ͷ���ֵ
				//DataHeader hd = {CMD_LOGIN,};
				LoginoutResult res = { 1 };

				send(_cSock, (const char*)&header, sizeof(DataHeader), 0);
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
		// ��������
		//if (0 == strcmp(recvBuf, "getInfo")) {
		//	//char msgBuf[128] = "xiao ming";
		//	DataPackage dp = { 80,"xiaohong" };
		//	send(_cSock, (const char*)&dp, sizeof(DataPackage) + 1, 0);   // �ѽ�β����0Ҳ���͹�ȥ
		//}
		//else if (0 == strcmp(recvBuf, "getAge")) {
		//	char msgBuf[128] = "18.";
		//	send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);   // �ѽ�β����0Ҳ���͹�ȥ
		//}
		//else {
		//	char msgBuf[128] = "???.";
		//	send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);
		//}
		
		
		//char msgBuf[] = "hello client,i am server!\n";
		//send(_cSock, msgBuf, strlen(msgBuf)+1,0);   // �ѽ�β����0Ҳ���͹�ȥ
	}


	closesocket(_sock);


	WSACleanup();

	printf("quit over!!!\n");
	getchar();
	return 0;
}