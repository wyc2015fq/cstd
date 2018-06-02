#ifndef _UTIME_HPP_
#define _UTIME_HPP_
#include <ctime>
namespace std
{
using ::clock_t;
using ::clock;
}
//
// 计时类 (单位为秒, 精确到毫秒)
//
class utime
{
public:
  //开始计时
  utime(): _start_time(std::clock()) {}
  void restart() {
    _start_time = std::clock();
  }
  double elapsed() const {
    return double(std::clock() - _start_time) / CLOCKS_PER_SEC;
  }
  double operator()() const {
    return elapsed();
  }
private:
  std::clock_t _start_time;
}; // end of stopwatch class
#ifdef WIN32
#include <windows.h>
// 高精度计时类 (单位为秒, 精确到微秒)
//
class uutime
{
public:
  typedef __int64 value_type;
  // 获得计数器的时钟频率
  uutime(): freq(frequency()) {
    restart();
  }
  // 获得初始值
  void restart() {
    start_time = counter();
  }
  // 获得对应的时间值，单位为秒
  double elapsed() const {
    return static_cast<double>((double)(counter() - start_time) / freq);
  }
  double operator()() const {
    return elapsed();
  }
public:
  value_type start_time, freq;
  static value_type frequency() {
    value_type frequency;
    ::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&frequency));
    return frequency;
  }
  static value_type counter() {
    value_type counter;
    ::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&counter));
    return counter;
  }
};
class CUtimer : public uutime
{
public:
  CUtimer() {
    restart();
  }
  void Restart(void) {
    restart();
  }
  unsigned long Elapsed(void) {
    return (unsigned long)((counter() - start_time) / 1000);
  }
};
#else
#include <sys/time.h>
class uutime
{
public:
  // 获得计数器的时钟频率
  uutime() {
    restart();
  }
  // 获得初始值
  void restart() {
    _start_time = utime_linux_count();
  }
  // 获得对应的时间值，单位为秒
  double elapsed() const {
    return count() / 1000000.;
  }
  double count() const {
    return (utime_linux_count() - _start_time);
  }
  double operator()() const {
    return elapsed();
  }
private:
  double _start_time;
  static double utime_linux_count() {
    struct timeval tv;
    double total = 0;

    if (gettimeofday(&tv , 0) == -1) {
      return 0;
    }

    total = tv.tv_sec * 1000000 + tv.tv_usec;
    return total;
  }
};
#if 1
// 返回自系统开机以来的毫秒数（tick）
struct CUtimer {
  static unsigned int GetTickCount() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
  }
  static double utime_linux_count() {
    struct timeval tv;
    double total = 0;

    if (gettimeofday(&tv , 0) == -1) {
      return 0;
    }

    total = tv.tv_sec * 1000000 + tv.tv_usec;
    return total;
  }
  unsigned int Restart() {
    return 0;
  }
  unsigned int Elapsed() {
    return GetTickCount();
  }
};
#else
class CUtimer
{
  struct timeval m_start_time;
protected:
  unsigned long utime_count_timeval(struct timeval* _start_time) {
    struct timeval tv;
    time_t s;
    unsigned long us;
    unsigned long total = 0;

    if (gettimeofday(&tv , 0) == -1) {
      return 0;
    }

    s = tv.tv_sec - _start_time->tv_sec;
    us = tv.tv_usec - _start_time->tv_usec;
    total = s * 1000000 + us;
    return total;
  }
public:
  CUtimer() {
    Restart();
  }
  void Restart(void) {
    gettimeofday(&m_start_time, 0);
  }
  unsigned long Elapsed(void) {
    return utime_count_timeval(&m_start_time) / 1000;
  }
};
#endif
#endif
#endif // _UTIME_HPP_

