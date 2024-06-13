#include <iostream>
#include "CellTimeStamp.hpp"

// c++11��׼���߳̿�
#include <thread>

// ���ĸ���
#include <mutex>

// ԭ�Ӳ���
#include <atomic>


using namespace std;

mutex m;

const int tCount = 4;

atomic_int sum = 0;

//int sum = 0;

void workFun(int n ) {

	//cout << n << endl;

	for (int i = 0; i < 10000; i++) {
		// ��������������ʱ�������
		//m.lock();
		// �ٽ��������
		//cout << "hello,sub thread<"<<n << ">" << endl;

		//lock_guard<mutex> lg(m);    // �����ջ�Զ��������ص�
		
		sum++;

		

		//m.unlock();
	}

	//m.unlock();
	
}

int main() {

	// �ṩһ���̵߳���ں���
	//thread t(workFun,10);

	// �߳�����
	thread t[tCount];

	for (int i = 0; i < tCount; i++) {
		t[i] = thread(workFun, i);
		//t[i].detach();
	}

	CellTimeStamp tTime;

	for (int i = 0; i < tCount; i++) {
		//t[i] = thread(workFun, 10);
		//t[i].detach();
		t[i].join();
	}




	//t.detach();
	//t.join();

	//for (int n = 0; n < 4; n++) {
	//	cout << "hello,main thread" << endl;
	//}

	cout << "hello,main thread" << endl;
	cout << tTime.getElapsedTimeInMilliSec() << "sum=" << sum << endl;
	//cout << sum << endl;


	sum = 0;
	tTime.update();
	for (int i = 0; i < 40000; i++) {
		sum++;
	}
	cout << tTime.getElapsedTimeInMilliSec() << "sum=" << sum << endl;

	// �ȴ�һ������
	getchar();

	return 0;
}