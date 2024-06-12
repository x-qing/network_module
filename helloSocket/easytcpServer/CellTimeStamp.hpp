#ifndef _CellTimeStamp_hpp_
#define _CellTimeStamp_hpp_

// ʹ��c++11���еļ�ʱ��
//#include <windows.h>
#include <chrono>
using namespace std::chrono;

class CellTimeStamp {
public:
	CellTimeStamp() {
		update();
	}
	~CellTimeStamp() {

	}

	void update() {
		_begin = high_resolution_clock::now();

	}

	double getElapsedSecond() {
		return getElapesedTimeInMicroSec() * 0.000001;  // ת����
	}

	double getElapsedTimeInMilliSec() {
		return getElapesedTimeInMicroSec() * 0.001;   // ת�ɺ���
	}

	long long getElapesedTimeInMicroSec() {
		// ����ķ���ֵ��long long ����
		//auto t = duration_cast<microseconds>(high_resolution_clock::now() - _begin).count();

		// �õ�����΢��
		return duration_cast<microseconds>(high_resolution_clock::now() - _begin).count();
	}

protected:
	time_point<high_resolution_clock> _begin;
};

#endif