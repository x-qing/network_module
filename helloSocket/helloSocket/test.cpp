#define WIN32_LEAN_AND_MEAN   // �궨�����һЩ����������ó�ͻ
#include <Windows.h>
#include <WinSock2.h>
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


	WSACleanup();
	return 0;
}