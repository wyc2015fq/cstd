
#include <inttypes.h>
#include <ctime>
#include "os.hpp"
//#include <sys/time.h>

namespace wstd {

  typedef uint64_t ptime;

  class timer
  {
  public:
    timer() { _start_time = std::clock(); } // postcondition: elapsed()==0
                                            //         timer( const timer& src );      // post: elapsed()==src.elapsed()
                                            //        ~timer(){}
                                            //  timer& operator=( const timer& src );  // post: elapsed()==src.elapsed()
    void   restart() { _start_time = std::clock(); } // post: elapsed()==0
    double elapsed() const                  // return elapsed time in seconds
    {
      return  double(std::clock() - _start_time) / CLOCKS_PER_SEC;
    }

    double elapsed_max() const   // return estimated maximum value for elapsed()
                                 // Portability warning: elapsed_max() may return too high a value on systems
                                 // where std::clock_t overflows or resets at surprising values.
    {
      return (double((std::numeric_limits<std::clock_t>::max)())
        - double(_start_time)) / double(CLOCKS_PER_SEC);
    }

    double elapsed_min() const            // return minimum value for elapsed()
    {
      return double(1) / double(CLOCKS_PER_SEC);
    }

  private:
    std::clock_t _start_time;
  }; // timer

  class utime
  {
  public:
    typedef int64_t value_type;
	// 获得初始值
	void restart() { start_time = counter(); }
    // 获得计数器的时钟频率
    utime() : freq(frequency()) { start_time = counter(); }
    // 获得对应的时间值，单位为秒
    double elapsed() const { return static_cast<double>((double)(counter() - start_time) / freq); }
    double operator()() const { return elapsed(); }

    value_type  start_time, freq;
    static value_type frequency()
    {
      value_type  freq1;
#if defined(OS_WINDOWS)
      ::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&freq1));
#elif defined(OS_LINUX)
      struct timeval tvStop;
      gettimeofday(&tvStop, NULL);
	  freq1 = ((value_type)tvStop.tv_sec) * 1000000 + (tvStop.tv_usec);
#endif
      return freq1;
    }
    static value_type counter()
    {
#if defined(OS_WINDOWS)
      value_type  counter;
      ::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&counter));
      return counter;
#elif defined(OS_LINUX)
      return 1000000;
#endif
    }
    static value_type total_microseconds(value_type time) {
      return time*1000/ counter();
    }
  };


}