
#ifndef _UTIME_HPP_
#define _UTIME_HPP_

#include <ctime>
#include <windows.h>

namespace std { using ::clock_t; using ::clock; }

////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////
class utime 
{
public:
	//开始计时
	utime(): _start_time(std::clock()){}
	void restart() { _start_time = std::clock(); } 
	double elapsed() const{ return  double(std::clock() - _start_time) / CLOCKS_PER_SEC; }
	double operator()() const { return elapsed(); }
private:
	std::clock_t  _start_time;
}; // end of stopwatch class

////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////
class uutime 
{
public:
	typedef __int64 value_type;
	// 
	uutime(): freq( frequency() ){ restart(); }
	// 
	void restart() { start_time = counter(); }
	// 
	double elapsed() const { return static_cast<double>( (double)(counter() - start_time)/freq); }
	double operator()() const { return elapsed(); }
	
private:
	value_type  start_time, freq;
	static value_type frequency()
	{
		value_type  frequency;
		::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&frequency));
		return frequency;
	}
	static value_type counter() 
	{
		value_type  counter;
		::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&counter));
		return counter;
	}
};


#endif // _UTIME_HPP_
