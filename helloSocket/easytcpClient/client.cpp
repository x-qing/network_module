#pragma warning(disable:4996)

#include "EasyTcpClient.hpp"

//bool g_thread_exit = true;
bool g_bRun = true;
// ����һ���̺߳�������ȡ��������
void cmdThread() {
	while (true) {
		char cmdBuf[256] = {};
		scanf("%s", cmdBuf);
		if (0 == strcmp(cmdBuf, "exit")) {
			printf("�˳�Thread�̣߳�\n");  // �����˳�ֻ�����߳��˳��ˣ����߳����ɻ�������
			//g_thread_exit = false;
			//client->closeSocket();
			g_bRun = false;
			return;
		}
		//else if (0 == strcmp(cmdBuf, "login")) {
		//	Login login;
		//	strcpy(login.userName, "xiaoming");
		//	strcpy(login.passWord, "123456");
		//	client->SendData(&login);
		//}
		//else if (0 == strcmp(cmdBuf, "loginout")) {
		//	Loginout loginout;
		//	strcpy(loginout.userName, "xiaoming");
		//	client->SendData(&loginout);
		//}
		else {
			printf("��֧�ֵ����\n");
		}
	}
}

int main() {
	
	// 




	/*   ------------- ����һ���ͻ���--------------
	// �����̣߳����߳������������������������
	// ���߳��������տͻ��˵ķ�����Ϣ
	EasyTcpClient client;
	client.connectServer("172.20.10.6", 4567);
	std::thread t1(cmdThread, &client);
	t1.detach();
	Login login;
	strcpy(login.userName, "lisi");
	strcpy(login.passWord, "123456");
	while (client.isRun()) {
		client.onRun();
		client.SendData(&login);
	}
	client.closeSocket();
	printf("���˳���\n");
	getchar();
	return 0;
	*/


	/*  ------  ����һ��ͻ���*/
		// �����̣߳����߳������������������������
	// ���߳��������տͻ��˵ķ�����Ϣ
	//const int CCOUNT = 10;
	
	// // select���߳������ֻ��֧��64������
	const int CCOUNT = 100;

	// ���������Ļ�,�������ܴ󣬻ᵼ��ջ�ռ����������ʹ��ָ�����ʽ
	EasyTcpClient* client[CCOUNT];

	//int a = sizeof(EasyTcpClient);

	for (int i = 0; i < CCOUNT; i++) {
		
		//client[i]->connectServer("192.168.179.144", 4567);
		if (!g_bRun) {
			return 0;
		}
		client[i] = new EasyTcpClient();
	}

	for (int i = 0; i < CCOUNT; i++) {
		if (!g_bRun) {
			return 0;
		}
		client[i]->connectServer("192.168.31.146", 4567);
	}

	std::thread t1(cmdThread);
	t1.detach();
	Login login;
	strcpy(login.userName, "lisi");
	strcpy(login.passWord, "123456");
	while (g_bRun) {

		for (int i = 0; i < CCOUNT; i++) {
			//client[i].connectServer("172.20.10.6", 4567);
			client[i]->SendData(&login);   // ֻ�����ݵķ���
			//client[i]->onRun();    // ���ݽ���
		}

		//client.onRun();
		//client.SendData(&login);
	}
	for (int i = 0; i < CCOUNT; i++) {
		client[i]->closeSocket();
	}
	
	printf("���˳���\n");
	//getchar();
	return 0;

}