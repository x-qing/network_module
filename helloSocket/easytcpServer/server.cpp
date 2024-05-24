#define WIN32_LEAN_AND_MEAN   // �궨�����һЩ����������ó�ͻ
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <Windows.h>
#include <WinSock2.h>
//#pragma comment(lib,"ws2_32.lib")
#include <stdio.h>

#include <vector>
//#define _WINSOCK_DEPRECATED_NO_WARNINGS
// window�µ�socket����

// ʹ�ýṹ��,��������Ϣ�Ĵ���

enum CMD{
	CMD_LOGIN,
	CMD_LOGIN_RESULT,
	CMD_LOGINOUT,
	CMD_LOGINOUT_RESULT,
	CMD_NEW_USER_JOIN,    // ֪ͬͨ�����ͻ��ˣ����µĿͻ��˵ļ���
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

std::vector<SOCKET> g_clients;

// ʹ��һ���������������
int processfd(SOCKET _cSock) {
	// ����һ����������������
	char szRecv[1024] = {};
	int len = recv(_cSock, szRecv, sizeof(DataHeader), 0);
	DataHeader* header = (DataHeader*)szRecv;
	//int len = recv(_cSock, (char *)&header, sizeof(DataHeader), 0);
	if (len <= 0) {
		printf("server quit! task over!\n");
		return -1;
	}
	switch (header->cmd) {
	case CMD_LOGIN:
	{
		// ���յ�¼����
		//Login* login;
		//recv(_cSock, (char*)&login + sizeof(DataHeader), sizeof(Login) - sizeof(DataHeader), 0);
		recv(_cSock, szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);
		Login* login = (Login*)szRecv;
		printf("�յ��������ԣ�%d��:CMD_LOGIN,���ݳ��� = %d,username=%s,password=%s\n", _cSock,login->dataLength, login->userName, login->passWord);
		// �����ж��û���������
		// ������Ϣͷ�ͷ���ֵ
		//DataHeader hd = {CMD_LOGIN,};
		LoginResult res;

		//send(_cSock, (const char*)&header, sizeof(DataHeader), 0);
		send(_cSock, (const char*)&res, sizeof(LoginResult), 0);
	}
	break;

	case CMD_LOGINOUT:
	{
		//Loginout loginout;
		recv(_cSock, szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);
		Loginout* loginout = (Loginout*)szRecv;
		printf("�յ��������ԣ�%d��:CMD_LOGIN,���ݳ��� = %d,username=%s\n",_cSock, loginout->dataLength, loginout->userName);
		// �����ж��û���������
		// ������Ϣͷ�ͷ���ֵ
		//DataHeader hd = {CMD_LOGIN,};
		LoginoutResult res;

		//send(_cSock, (const char*)&header, sizeof(DataHeader), 0);
		send(_cSock, (const char*)&res, sizeof(LoginoutResult), 0);
	}
	break;
	default:
	{
		DataHeader header = { 0,CMD_ERROR };
		//header.cmd = CMD_ERROR;
		//header.dataLength = 0;
		send(_cSock, (const char*)&header, sizeof(DataHeader), 0);
	}
	break;
	//char msgBuf[128] = "???.";
	//send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);
	}
}

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

	//sockaddr_in clientAddr = {};
	//int naddrLen = sizeof(clientAddr);

	//SOCKET _cSock = INVALID_SOCKET;   // ��ʼ��Ϊһ����Ч��socket
	////char msgBuf[] = "hello client,i am server!\n";
	//_cSock = accept(_sock, (sockaddr*)&clientAddr, &naddrLen);

	//if (_cSock == SOCKET_ERROR) {
	//	printf("accept error!\n");
	//}
	//else {
	//	printf("accept sucess!\n");
	//}
	//printf("new client add:ip = %s,port = %d\n", inet_ntoa(clientAddr.sin_addr),(int)clientAddr.sin_port);

	// ���ջ�����
	/*char recvBuf[128] = { 0 };*/
	//char recvBuf[128] = { 0 };


	// �޸��˷���ˣ����ԺͿͻ��˽��н���
	while (true) {

		// ����select����ģ��
		// select(
		//_In_ int nfds, �����ļ�������,��ʾ�����ļ���������һ����Χ
		//	_Inout_opt_ fd_set FAR* readfds,   �ɶ��¼�
		//	_Inout_opt_ fd_set FAR* writefds,  ��д�¼�
		//	_Inout_opt_ fd_set FAR* exceptfds, �쳣�¼�
		//	_In_opt_ const struct timeval FAR* timeout  �����¼�
		//	);
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
			FD_SET(g_clients[n], &fd_read);
		}

		// ����select��timeout����
		// Ϊnullptr --->��ʾ����Ϊ����״̬��һ��Ҫ�ȵ����ӵ��ļ��������б仯�ŷ��أ�
		// Ϊ0 --->��ʾ������
		// Ϊһ��������ʱ�� --->��ʾҪ�ȴ����ʱ���ٷ���
		timeval t = { 0,0 };
		// ����������������Լ�һЩ����ҵ����Ȼ���ٲ�ѯ��
		int ret = select(_sock + 1, &fd_read, &fd_write, &fd_error, &t);
		if (ret < 0) {
			printf("select�������!\n");
			break;
		}
		if (FD_ISSET(_sock, &fd_read)) {   // ��ʾ���µ���������
			FD_CLR(_sock, &fd_read);
			sockaddr_in clientAddr = {};
			int naddrLen = sizeof(clientAddr);

			SOCKET _cSock = INVALID_SOCKET;   // ��ʼ��Ϊһ����Ч��socket
			//char msgBuf[] = "hello client,i am server!\n";
			_cSock = accept(_sock, (sockaddr*)&clientAddr, &naddrLen);

			// ���¼���Ŀͻ��˱�������			
			if (_cSock == SOCKET_ERROR) {
				printf("accept error!\n");
			}
			else {
				printf("accept sucess!\n");
			}
			printf("new client add:ip = %s,port = %d\n", inet_ntoa(clientAddr.sin_addr), (int)clientAddr.sin_port);
			for (size_t n = 0; n < g_clients.size(); n++) {
				NewUserJoin userjoin = {};
				userjoin.sock = _cSock;
				send(g_clients[n], (const char*) & userjoin, sizeof(NewUserJoin), 0);
			}
			g_clients.push_back(_cSock);
		}
		//DataHeader header = {};

		// ���տͻ��˵���������
		// ���Ƚ��յ���hander�ĳ��ȣ������Ѿ�û����ô����

		//// ����һ����������������
		//char szRecv[1024] = {};
		//int len = recv(_cSock, szRecv, sizeof(DataHeader), 0);
		//DataHeader* header = (DataHeader*)szRecv;
		////int len = recv(_cSock, (char *)&header, sizeof(DataHeader), 0);
		//if (len <= 0) {
		//	printf("server quit! task over!\n");
		//	break;
		//}
		// ����fd_array�Ĵ�С��64
		for (size_t n = 0; n < fd_read.fd_count ; n++) {
			if (-1 == processfd(fd_read.fd_array[n])) {
				// �ͻ����˳�
				auto iter = find(g_clients.begin(), g_clients.end(), fd_read.fd_array[n]);
				if (iter != g_clients.end()) {
					g_clients.erase(iter);
				}
			}
		}

		// ��������ҵ��
		printf("����ʱ�䴦������ҵ��!\n");
		//printf("recv data:cmd = %d, length = %d\n",header.cmd,header.dataLength);
		//switch (header->cmd) {
		//case CMD_LOGIN:
		//	{
		//		// ���յ�¼����
		//		//Login* login;
		//		//recv(_cSock, (char*)&login + sizeof(DataHeader), sizeof(Login) - sizeof(DataHeader), 0);
		//		recv(_cSock, szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader),0);
		//		Login* login = (Login*)szRecv;
		//		printf("�յ�����:CMD_LOGIN,���ݳ��� = %d,username=%s,password=%s\n", login->dataLength,login->userName,login->passWord);
		//		// �����ж��û���������
		//		// ������Ϣͷ�ͷ���ֵ
		//		//DataHeader hd = {CMD_LOGIN,};
		//		LoginResult res;

		//		//send(_cSock, (const char*)&header, sizeof(DataHeader), 0);
		//		send(_cSock, (const char*)&res, sizeof(LoginResult), 0);
		//	}
		//	break;

		//case CMD_LOGINOUT:
		//	{
		//		//Loginout loginout;
		//		recv(_cSock, szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);
		//		Loginout* loginout = (Loginout*)szRecv;
		//		printf("�յ�����:CMD_LOGIN,���ݳ��� = %d,username=%s\n", loginout->dataLength, loginout->userName);
		//		// �����ж��û���������
		//		// ������Ϣͷ�ͷ���ֵ
		//		//DataHeader hd = {CMD_LOGIN,};
		//		LoginoutResult res;

		//		//send(_cSock, (const char*)&header, sizeof(DataHeader), 0);
		//		send(_cSock, (const char*)&res, sizeof(LoginoutResult), 0);
		//	}
		//	break;
		//default:
		//	{
		//		DataHeader header = {0,CMD_ERROR};
		//		//header.cmd = CMD_ERROR;
		//		//header.dataLength = 0;
		//		send(_cSock, (const char*)&header, sizeof(DataHeader), 0);
		//	}
		//	break;
		//	//char msgBuf[128] = "???.";
		//	//send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);
		//}
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

	for (size_t n = 0;n < g_clients.size() - 1; n++) {
		//FD_SET(g_clients[n], &fd_read);
		closesocket(g_clients[n]);
	}

	closesocket(_sock);


	WSACleanup();

	printf("quit over!!!\n");
	getchar();
	return 0;
}