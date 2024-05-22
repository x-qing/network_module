#define WIN32_LEAN_AND_MEAN   // 宏定义避免一些依赖库的引用冲突
#include <Windows.h>
#include <WinSock2.h>
//#pragma comment(lib,"ws2_32.lib")

// window下的socket环境
int main() {
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	// 启动windows的socket网络环境
	WSAStartup(ver, &dat);

	/*

	*/


	WSACleanup();
	return 0;
}