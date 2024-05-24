#define WIN32_LEAN_AND_MEAN   // �궨�����һЩ����������ó�ͻ
#define _WINSOCK_DEPRECATED_NO_WARNINGS
//#pragma warning(disable:4996��

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
	short dataLength; // ���ݳ���
	short cmd; // ����
};

// ��¼�Լ��䷵�صĽ��
// ���ʹ�ýṹ�������У���������չ������ֱ��ʹ�ü̳еķ�ʽ
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
		result = 0;   // Ϊ0��ʾһ������
	}
	int result;   // ��¼�Ľ��
};

// �ǳ��Լ��䷵�صĽ��
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
	int result;   // ��¼�Ľ��
};

// �µĿͻ��˵ļ���
struct NewUserJoin : public DataHeader {
	NewUserJoin() {
		dataLength = sizeof(NewUserJoin);
		cmd = CMD_NEW_USER_JOIN;
		sock = 0;
	}
	//int result;   // ��¼�Ľ��
	int sock;
};


// �ͻ���ͬ��ʹ��һ���������������
int processfd(SOCKET _cSock) {
	// ����һ����������������
	char szRecv[1024] = {};
	int len = recv(_cSock, szRecv, sizeof(DataHeader), 0);
	DataHeader* header = (DataHeader*)szRecv;
	//int len = recv(_cSock, (char *)&header, sizeof(DataHeader), 0);
	if (len <= 0) {
		printf("��������Ͽ�����!\n");
		return -1;
	}
	switch (header->cmd) {
	case CMD_LOGIN_RESULT:
	{
		// ��������
		recv(_cSock, szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);
		LoginResult* login = (LoginResult*)szRecv;
		printf("�յ����񷵻�:CMD_LOGIN_RESULT,���ݳ��� = %d,result = %d\n", login->dataLength,login->result);
		//// ���յ�¼����
		////Login* login;
		////recv(_cSock, (char*)&login + sizeof(DataHeader), sizeof(Login) - sizeof(DataHeader), 0);
		//recv(_cSock, szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);
		//Login* login = (Login*)szRecv;
		//printf("�յ�����:CMD_LOGIN,���ݳ��� = %d,username=%s,password=%s\n", login->dataLength, login->userName, login->passWord);
		//// �����ж��û���������
		//// ������Ϣͷ�ͷ���ֵ
		////DataHeader hd = {CMD_LOGIN,};
		//LoginResult res;

		////send(_cSock, (const char*)&header, sizeof(DataHeader), 0);
		//send(_cSock, (const char*)&res, sizeof(LoginResult), 0);
	}
	break;

	case CMD_LOGINOUT_RESULT:
	{
		// ��������
		recv(_cSock, szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);
		LoginoutResult* login = (LoginoutResult*)szRecv;
		printf("�յ����񷵻�:CMD_LOGINOUT_RESULT,���ݳ��� = %d,result = %d\n", login->dataLength, login->result);
		////Loginout loginout;
		//recv(_cSock, szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);
		//Loginout* loginout = (Loginout*)szRecv;
		//printf("�յ�����:CMD_LOGIN,���ݳ��� = %d,username=%s\n", loginout->dataLength, loginout->userName);
		//// �����ж��û���������
		//// ������Ϣͷ�ͷ���ֵ
		////DataHeader hd = {CMD_LOGIN,};
		//LoginoutResult res;

		////send(_cSock, (const char*)&header, sizeof(DataHeader), 0);
		//send(_cSock, (const char*)&res, sizeof(LoginoutResult), 0);
	}
	break;
	case CMD_NEW_USER_JOIN:
	{
		// ��������
		recv(_cSock, szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);
		NewUserJoin* login = (NewUserJoin*)szRecv;
		printf("�յ����񷵻�:CMD_NEW_USER_JOIN,���ݳ��� = %d,sock = %d\n", login->dataLength, login->sock);
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

		// ���select����ģ��
		fd_set fd_read;
		FD_ZERO(&fd_read);
		FD_SET(_sock, &fd_read);
		timeval time = { 0, 0 };
		int ret = select(_sock, &fd_read, nullptr, nullptr, &time);
		if (ret < 0) {
			printf("select�������\n");
			break;
		}
		if (FD_ISSET(_sock, &fd_read)) {
			FD_CLR(_sock, &fd_read);
			if (-1 == processfd(_sock)) { // �����ݿ��Զ�����Ҫ���д���
				printf("select�������2\n");
				break;
			}
		}
		printf("����ʱ�䴦������ҵ��\n");
		Login login;
		strcpy(login.userName, "xiaoming");
		strcpy(login.passWord, "123456");
		send(_sock, (const char*)&login, sizeof(Login), 0);
		//Sleep(1000);
		//char cmdbuf[128] = { 0 };
		//scanf("%s", cmdbuf);

		// ��������
		//if (0 == strcmp(cmdbuf, "exit")) {
		//	//printf("client quit")
		//	printf("client quit! task over!\n");
		//	break;
		//}
		//else if(0 == strcmp(cmdbuf,"login")) {
		//	// ��¼����
		//	Login login;
		//	strcpy(login.userName, "username");
		//	strcpy(login.passWord, "password");

		//	//DataHeader header = { sizeof(Login) ,CMD_LOGIN};
		//	//send(_sock, (const char *)&header, sizeof(DataHeader), 0);
		//	send(_sock, (const char*)&login, sizeof(Login), 0);

		//	// ���շ���������Ϣ
		//	//DataHeader retheader = {};
		//	LoginResult retlogin = {};
		//	//recv(_sock, (char*)&retheader, sizeof(DataHeader), 0);
		//	recv(_sock, (char*)&retlogin, sizeof(LoginResult), 0);
		//	printf("Loginresult = %d\n", retlogin.result);
		//}
		//else if (0 == strcmp(cmdbuf, "logout")) {
		//	// �˳�����
		//	Loginout loginout;
		//	strcpy(loginout.userName, "username");
		//	//DataHeader header = { sizeof(Loginout),CMD_LOGINOUT };
		//	//send(_sock, (const char*)&header, sizeof(DataHeader), 0);
		//	send(_sock, (const char*)&loginout, sizeof(Loginout), 0);

		//	// ���շ���������Ϣ
		//	//DataHeader retheader = {};
		//	LoginoutResult retloginout = {};
		//	//recv(_sock, (char*)&retheader, sizeof(DataHeader), 0);
		//	recv(_sock, (char*)&retloginout, sizeof(LoginoutResult), 0);
		//	printf("Loginresult = %d\n", retloginout.result);
		//}
		//else {
		//	printf("��֧�ִ����������������\n");
		//}

		//char recvbuf[128] = { 0 };

		//int len = recv(_sock, recvbuf, 1024, 0);

		//if (len > 0) {
		//	// �Խ��յ������ݽ��д���
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