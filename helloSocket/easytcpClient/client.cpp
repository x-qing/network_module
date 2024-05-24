#define WIN32_LEAN_AND_MEAN   // 宏定义避免一些依赖库的引用冲突
#define _WINSOCK_DEPRECATED_NO_WARNINGS
//#pragma warning(disable:4996）

#include <Windows.h>
#include <WinSock2.h>
#include <stdio.h>
//#pragma comment(lib,"ws2_32.lib")


//struct DataPackage {
//	int age;
//	char name[32];
//};

enum CMD {
	CMD_LOGIN,
	CMD_LOGIN_RESULT,
	CMD_LOGINOUT,
	CMD_LOGINOUT_RESULT,
	CMD_NEW_USER_JOIN,
	CMD_ERROR
};

struct DataHeader {
	short dataLength; // 数据长度
	short cmd; // 命令
};

// 登录以及其返回的结果
// 如果使用结构体来进行，不利于扩展，这里直接使用继承的方式
struct Login : public DataHeader {
	//int age;
	//char name[32];
	Login() {
		dataLength = sizeof(Login);
		cmd = CMD_LOGIN;
	}
	//DataHeader header;
	char userName[32];
	char passWord[32];
};

struct LoginResult : public DataHeader {
	LoginResult() {
		dataLength = sizeof(LoginResult);
		cmd = CMD_LOGIN_RESULT;
		result = 0;   // 为0表示一切正常
	}
	int result;   // 登录的结果
};

// 登出以及其返回的结果
struct Loginout : public DataHeader {
	Loginout() {
		dataLength = sizeof(Loginout);
		cmd = CMD_LOGINOUT;
	}
	char userName[32];
};

struct LoginoutResult : public DataHeader {
	LoginoutResult() {
		dataLength = sizeof(LoginoutResult);
		cmd = CMD_LOGINOUT_RESULT;
		result = 0;
	}
	int result;   // 登录的结果
};

// 新的客户端的加入
struct NewUserJoin : public DataHeader {
	NewUserJoin() {
		dataLength = sizeof(NewUserJoin);
		cmd = CMD_NEW_USER_JOIN;
		sock = 0;
	}
	//int result;   // 登录的结果
	int sock;
};


// 客户端同样使用一个函数来处理进程
int processfd(SOCKET _cSock) {
	// 定义一个缓冲来接收数据
	char szRecv[1024] = {};
	int len = recv(_cSock, szRecv, sizeof(DataHeader), 0);
	DataHeader* header = (DataHeader*)szRecv;
	//int len = recv(_cSock, (char *)&header, sizeof(DataHeader), 0);
	if (len <= 0) {
		printf("与服务器断开连接!\n");
		return -1;
	}
	switch (header->cmd) {
	case CMD_LOGIN_RESULT:
	{
		// 接收数据
		recv(_cSock, szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);
		LoginResult* login = (LoginResult*)szRecv;
		printf("收到服务返回:CMD_LOGIN_RESULT,数据长度 = %d,result = %d\n", login->dataLength,login->result);
		//// 接收登录数据
		////Login* login;
		////recv(_cSock, (char*)&login + sizeof(DataHeader), sizeof(Login) - sizeof(DataHeader), 0);
		//recv(_cSock, szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);
		//Login* login = (Login*)szRecv;
		//printf("收到命令:CMD_LOGIN,数据长度 = %d,username=%s,password=%s\n", login->dataLength, login->userName, login->passWord);
		//// 忽略判断用户名和密码
		//// 定义消息头和返回值
		////DataHeader hd = {CMD_LOGIN,};
		//LoginResult res;

		////send(_cSock, (const char*)&header, sizeof(DataHeader), 0);
		//send(_cSock, (const char*)&res, sizeof(LoginResult), 0);
	}
	break;

	case CMD_LOGINOUT_RESULT:
	{
		// 接收数据
		recv(_cSock, szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);
		LoginoutResult* login = (LoginoutResult*)szRecv;
		printf("收到服务返回:CMD_LOGINOUT_RESULT,数据长度 = %d,result = %d\n", login->dataLength, login->result);
		////Loginout loginout;
		//recv(_cSock, szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);
		//Loginout* loginout = (Loginout*)szRecv;
		//printf("收到命令:CMD_LOGIN,数据长度 = %d,username=%s\n", loginout->dataLength, loginout->userName);
		//// 忽略判断用户名和密码
		//// 定义消息头和返回值
		////DataHeader hd = {CMD_LOGIN,};
		//LoginoutResult res;

		////send(_cSock, (const char*)&header, sizeof(DataHeader), 0);
		//send(_cSock, (const char*)&res, sizeof(LoginoutResult), 0);
	}
	break;
	case CMD_NEW_USER_JOIN:
	{
		// 接收数据
		recv(_cSock, szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);
		NewUserJoin* login = (NewUserJoin*)szRecv;
		printf("收到服务返回:CMD_NEW_USER_JOIN,数据长度 = %d,sock = %d\n", login->dataLength, login->sock);
	}
	break;
	//default:
	//{
	//	DataHeader header = { 0,CMD_ERROR };
	//	//header.cmd = CMD_ERROR;
	//	//header.dataLength = 0;
	//	send(_cSock, (const char*)&header, sizeof(DataHeader), 0);
	//}
	//break;
	//char msgBuf[128] = "???.";
	//send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);
	}
}

// window下的socket环境
int main() {
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	// 启动windows的socket网络环境
	WSAStartup(ver, &dat);
	/*
	* socket简易tcp服务端
	* socket -> bind -> listen -> accept -> read -> recv ->close
	*
	* socket建议tcp服务端
	* socket -> connect -> read -> recv -> close
	*/
	// 可以显示声明用IPPROTO_TCP协议，也可以根据前面两个参数使用默认的协议。
	SOCKET _sock = socket(AF_INET, SOCK_STREAM, 0);
	if (_sock == SOCKET_ERROR) {
		printf("socket error!\n");
	}
	else {
		printf("socket sucess!\n");
	}

	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	// 服务器的ip和端口
	_sin.sin_port = htons(4567);
	_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	int _csock = connect(_sock, (sockaddr*)&_sin, sizeof(sockaddr_in));
	if (_csock == SOCKET_ERROR) {
		printf("connect error!\n");
	}
	else {
		printf("connect sucess!\n");
	}
	
	// 都是128，长度匹配
	
	while (true) {

		// 添加select网络模型
		fd_set fd_read;
		FD_ZERO(&fd_read);
		FD_SET(_sock, &fd_read);
		timeval time = { 0, 0 };
		int ret = select(_sock, &fd_read, nullptr, nullptr, &time);
		if (ret < 0) {
			printf("select任务结束\n");
			break;
		}
		if (FD_ISSET(_sock, &fd_read)) {
			FD_CLR(_sock, &fd_read);
			if (-1 == processfd(_sock)) { // 有数据可以读，需要进行处理
				printf("select任务结束2\n");
				break;
			}
		}
		printf("空闲时间处理其他业务！\n");
		Login login;
		strcpy(login.userName, "xiaoming");
		strcpy(login.passWord, "123456");
		send(_sock, (const char*)&login, sizeof(Login), 0);
		//Sleep(1000);
		//char cmdbuf[128] = { 0 };
		//scanf("%s", cmdbuf);

		// 处理请求
		//if (0 == strcmp(cmdbuf, "exit")) {
		//	//printf("client quit")
		//	printf("client quit! task over!\n");
		//	break;
		//}
		//else if(0 == strcmp(cmdbuf,"login")) {
		//	// 登录命令
		//	Login login;
		//	strcpy(login.userName, "username");
		//	strcpy(login.passWord, "password");

		//	//DataHeader header = { sizeof(Login) ,CMD_LOGIN};
		//	//send(_sock, (const char *)&header, sizeof(DataHeader), 0);
		//	send(_sock, (const char*)&login, sizeof(Login), 0);

		//	// 接收服务器的消息
		//	//DataHeader retheader = {};
		//	LoginResult retlogin = {};
		//	//recv(_sock, (char*)&retheader, sizeof(DataHeader), 0);
		//	recv(_sock, (char*)&retlogin, sizeof(LoginResult), 0);
		//	printf("Loginresult = %d\n", retlogin.result);
		//}
		//else if (0 == strcmp(cmdbuf, "logout")) {
		//	// 退出命令
		//	Loginout loginout;
		//	strcpy(loginout.userName, "username");
		//	//DataHeader header = { sizeof(Loginout),CMD_LOGINOUT };
		//	//send(_sock, (const char*)&header, sizeof(DataHeader), 0);
		//	send(_sock, (const char*)&loginout, sizeof(Loginout), 0);

		//	// 接收服务器的消息
		//	//DataHeader retheader = {};
		//	LoginoutResult retloginout = {};
		//	//recv(_sock, (char*)&retheader, sizeof(DataHeader), 0);
		//	recv(_sock, (char*)&retloginout, sizeof(LoginoutResult), 0);
		//	printf("Loginresult = %d\n", retloginout.result);
		//}
		//else {
		//	printf("不支持此类命令，请重新输入\n");
		//}

		//char recvbuf[128] = { 0 };

		//int len = recv(_sock, recvbuf, 1024, 0);

		//if (len > 0) {
		//	// 对接收到的数据进行处理
		//	DataPackage* dp = (DataPackage*)recvbuf;

		//	printf("recv data is: age=%d,name=%s\n", dp->age, dp->name);
		//}
	}
	

	closesocket(_sock);

	WSACleanup();
	printf("quit over!!!\n");
	//getchar();
	getchar();
	return 0;
}