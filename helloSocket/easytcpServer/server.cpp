#define WIN32_LEAN_AND_MEAN   // �궨�����һЩ����������ó�ͻ
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <Windows.h>
#include <WinSock2.h>
//#pragma comment(lib,"ws2_32.lib")
#include <stdio.h>
//#define _WINSOCK_DEPRECATED_NO_WARNINGS
// window�µ�socket����
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
	//_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	_sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(_sock, (sockaddr*)&_sin, sizeof(_sin)) == SOCKET_ERROR) {
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

	sockaddr_in clientAddr = {};
	int naddrLen = sizeof(clientAddr);

	SOCKET _cSock = INVALID_SOCKET;   // ��ʼ��Ϊһ����Ч��socket
	char msgBuf[] = "hello client,i am server!\n";
	while (true) {
		_cSock = accept(_sock, (sockaddr*)&clientAddr, &naddrLen);

		if (_cSock == INVALID_SOCKET) {
			printf("accept error!\n");
		}
		printf("new client add:ip = %s,port = %d\n",inet_ntoa(clientAddr.sin_addr),(int)clientAddr.sin_port);
		//char msgBuf[] = "hello client,i am server!\n";
		send(_cSock, msgBuf, strlen(msgBuf)+1,0);   // �ѽ�β����0Ҳ���͹�ȥ
	}


	closesocket(_sock);


	WSACleanup();
	return 0;
}