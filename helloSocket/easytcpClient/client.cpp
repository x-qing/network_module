#include "EasyTcpClient.hpp"

//bool g_thread_exit = true;

// ����һ���̺߳�������ȡ��������
void cmdThread(EasyTcpClient* client) {
	while (true) {
		char cmdBuf[256] = {};
		scanf("%s", cmdBuf);
		if (0 == strcmp(cmdBuf, "exit")) {
			printf("�˳�Thread�̣߳�\n");  // �����˳�ֻ�����߳��˳��ˣ����߳����ɻ�������
			//g_thread_exit = false;
			client->closeSocket();
			return;
		}
		else if (0 == strcmp(cmdBuf, "login")) {
			Login login;
			strcpy(login.userName, "xiaoming");
			strcpy(login.passWord, "123456");
			client->SendData(&login);
		}
		else if (0 == strcmp(cmdBuf, "loginout")) {
			Loginout loginout;
			strcpy(loginout.userName, "xiaoming");
			client->SendData(&loginout);
		}
		else {
			printf("��֧�ֵ����\n");
		}
	}
}

int main() {

	EasyTcpClient client;
	//client.initSocket();
	// 	server.BindSocket("192.168.31.146", 4567);
	client.connectServer("192.168.31.146", 4567);
	std::thread t1(cmdThread, &client);
	t1.detach();

	// �ٴ���һ���ͻ���
	//EasyTcpClient client2;
	////client.initSocket();
	//client2.connectServer("172.29.140.202", 4567);
	//std::thread t2(cmdThread, &client2);
	//t2.detach(); 

	while (client.isRun()) {
		client.onRun();
		//client2.onRun();
	}
	client.closeSocket();
	//client2.closeSocket();
	printf("���˳���\n");
	getchar();
	return 0;
}