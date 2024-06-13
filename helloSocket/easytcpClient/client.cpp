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


// �ͻ�������
const int cCount = 4000;
const int tCount = 4;     // �̵߳�����

EasyTcpClient* client[cCount];


//�ĸ��̣߳�id = 1,2,3,4
void sendThread(int id) {
	int c = (cCount / tCount);
	int begin = (id - 1) * c;
	int end = id * c;




	// 1.����1000���ͻ���
	for (int i = begin; i < end; i++) {
		client[i] = new EasyTcpClient();
	}

	// 2.���ӷ�����
	for (int i = begin; i < end; i++) {
		client[i]->connectServer("192.168.31.146", 4567);
	}

	Login login;
	strcpy(login.userName, "lisi");
	strcpy(login.passWord, "123456");

	// 3.��������
	while (g_bRun) {

		for (int i = begin; i < end; i++) {
			client[i]->SendData(&login);   // ֻ�����ݵķ���
			//client[i]->onRun();    // ���ݽ���
		}
	}

	// 5.�رտͻ���
	for (int i = begin; i < end; i++) {
		client[i]->closeSocket();
	}
}

int main() {
	// �޸�Ϊ���̵߳����

	// 1.��������û�������߳�
	std::thread t1(cmdThread);
	t1.detach();
	
	// 2.���뷢�����ݵ��߳�
	for (int n = 0; n < tCount; n++) {
		std::thread t1(sendThread, n + 1);
		t1.detach();
	}

	printf("���˳���\n");
	getchar();
	return 0;



	// ------------- ����һ���ͻ���--------------
	/*
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


	// -------------- ����һ��ͻ���----------------
	/*  
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
	*/

}