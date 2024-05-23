#define WIN32_LEAN_AND_MEAN   // �궨�����һЩ����������ó�ͻ
#define _WINSOCK_DEPRECATED_NO_WARNINGS
//#pragma warning(disable:4996��

#include <Windows.h>
#include <WinSock2.h>
#include <stdio.h>
//#pragma comment(lib,"ws2_32.lib")


struct DataPackage {
	int age;
	char name[32];
};

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
	
	// ����128������ƥ��
	
	while (true) {
		char cmdbuf[128] = { 0 };
		scanf("%s", cmdbuf);

		// ��������
		if (0 == strcmp(cmdbuf, "exit")) {
			//printf("client quit")
			printf("client quit! task over!\n");
			break;
		}
		else {
			// ��������
			send(_sock, cmdbuf, strlen(cmdbuf)+1, 0);
		}

		char recvbuf[128] = { 0 };

		int len = recv(_sock, recvbuf, 1024, 0);

		if (len > 0) {
			// �Խ��յ������ݽ��д���
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