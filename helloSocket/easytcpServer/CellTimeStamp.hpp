#ifndef _CellTimeStamp_hpp_
#define _CellTimeStamp_hpp_

// 使用c++11带有的计时器
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
		return getElapesedTimeInMicroSec() * 0.000001;  // 转成秒
	}

	double getElapsedTimeInMilliSec() {
		return getElapesedTimeInMicroSec() * 0.001;   // 转成毫秒
	}

	long long getElapesedTimeInMicroSec() {
		// 这里的返回值是long long 类型
		//auto t = duration_cast<microseconds>(high_resolution_clock::now() - _begin).count();

		// 得到的是微妙
		return duration_cast<microseconds>(high_resolution_clock::now() - _begin).count();
	}

protected:
	time_point<high_resolution_clock> _begin;
};

#endif