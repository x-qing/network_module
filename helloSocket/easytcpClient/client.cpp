#define WIN32_LEAN_AND_MEAN   // �궨�����һЩ����������ó�ͻ
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <Windows.h>
#include <WinSock2.h>
#include <stdio.h>
//#pragma comment(lib,"ws2_32.lib")

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
	// ������ʾ������IPPROTO_TCPЭ�飬Ҳ���Ը���ǰ����������ʹ��Ĭ�ϵ�Э�顣
	SOCKET _sock = socket(AF_INET, SOCK_STREAM, 0);
	if (_sock == SOCKET_ERROR) {
		printf("socket error!\n");
	}
	else {
		printf("socket sucess!\n");
	}

	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	// ��������ip�Ͷ˿�
	_sin.sin_port = htons(4567);
	_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	int _csock = connect(_sock, (sockaddr*)&_sin, sizeof(sockaddr_in));
	if (_csock == SOCKET_ERROR) {
		printf("connect error!\n");
	}
	else {
		printf("connect sucess!\n");
	}
	char recvbuf[1024] = {0};

	int len = recv(_sock, recvbuf, 1024, 0);
	if (len > 0) {
		printf("recv data is :%d-%s\n",len, recvbuf);
	}
	

	closesocket(_sock);

	WSACleanup();
	getchar();
	return 0;
}