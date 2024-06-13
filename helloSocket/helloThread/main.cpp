#include <iostream>
#include "CellTimeStamp.hpp"

// c++11标准的线程库
#include <thread>

// 锁的概念
#include <mutex>

// 原子操作
#include <atomic>


using namespace std;

mutex m;

const int tCount = 4;

atomic_int sum = 0;

//int sum = 0;

void workFun(int n ) {

	//cout << n << endl;

	for (int i = 0; i < 10000; i++) {
		// 加锁解锁导致了时间的消耗
		//m.lock();
		// 临界区代码段
		//cout << "hello,sub thread<"<<n << ">" << endl;

		//lock_guard<mutex> lg(m);    // 对象出栈自动析构的特点
		
		sum++;

		

		//m.unlock();
	}

	//m.unlock();
	
}

int main() {

	// 提供一个线程的入口函数
	//thread t(workFun,10);

	// 线程数组
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

	// 等待一个输入
	getchar();

	return 0;
}