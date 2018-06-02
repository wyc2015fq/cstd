
#ifndef _UTIME_HPP_
#define _UTIME_HPP_

#include <ctime>

namespace std { using ::clock_t; using ::clock; }

////////////////////////////////////////////////////////////////////////
// 计时类 (单位为秒, 精确到毫秒)
////////////////////////////////////////////////////////////////////////
class utime 
{
public:
  typedef std::clock_t value_type;
	//开始计时
	utime(): _start_time(counter()){}
	void restart() { _start_time = counter(); } 
	double elapsed() const{ return  double(counter() - _start_time) / CLOCKS_PER_SEC; }
  double operator()() const { return elapsed(); }
  static value_type counter() {
    return std::clock();
  }
private:
	value_type  _start_time;
}; // end of stopwatch class


#if 0
////////////////////////////////////////////////////////////////////////
// 高精度计时类 (单位为秒, 精确到微秒)
////////////////////////////////////////////////////////////////////////
#ifdef WIN32
#include <windows.h>
class uutime
{
public:
	typedef __int64 value_type;
	// 获得计数器的时钟频率
	uutime(): freq( frequency() ){ restart(); }
	// 获得初始值
	void restart() { start_time = counter(); }
	// 获得对应的时间值，单位为秒
	double elapsed() const { return static_cast<double>( (double)(counter() - start_time)/freq); }
  double operator()() const { return elapsed(); }
  
  static value_type counter();
  {
    value_type  counter;
    ::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&counter));
    return counter;
	}
private:
	value_type  start_time, freq;
	static value_type frequency()
	{
		value_type  frequency;
		::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&frequency));
		return frequency;
	}
};
#else
#include <sys/time.h>
class uutime
{
public:
	typedef double value_type;
  // 获得计数器的时钟频率
  uutime() : freq(1000000) { restart(); }
  // 获得初始值
  void restart() { start_time = counter(); }
  // 获得对应的时间值，单位为秒
  double elapsed() const { return ( (double)(counter() - start_time)/freq); }
  double operator()() const { return elapsed(); }
  
  static value_type counter()
  {
    struct timeval tv;
    time_t s;
    value_type total = 0;
    
    if( gettimeofday( &tv , 0 ) == -1 ) {
      return 0;
    }
    total = tv.tv_sec * 1000000 + tv.tv_usec;
    return total;
  }
private:
  value_type freq, start_time;

};
#endif
#else
typedef utime uutime;
#endif


#endif // _UTIME_HPP_
