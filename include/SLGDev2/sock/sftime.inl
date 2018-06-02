#ifndef _MY_TIMER_
#define _MY_TIMER_
#ifdef WIN32
#include <windows.h>
#include <iostream>
#include <iomanip>
inline std::string num2str(long i)
{
  char buf[32];
  sprintf(buf, "%d", i);
  return buf;
}
// 定时器：为 Windows 实现一个连续更新，高精度的时间供应器
class sftime
{
public:
  typedef WORD word_t;
  typedef sftime self;
private:
  struct reference_point {
    FILETIME file_time;
    LARGE_INTEGER counter;
  };
  reference_point ref_point;
  LARGE_INTEGER frequency;
  FILETIME file_time;
  SYSTEMTIME system_time;
  TIME_ZONE_INFORMATION zone_info;
  // 同步
  static void simplistic_synchronize(reference_point& ref_point) {
    long i;
    FILETIME ft0 = { 0, 0 };
    FILETIME& ft1 = ref_point.file_time;
    LARGE_INTEGER& li = ref_point.counter;
    // Spin waiting for a change in system time. Get the matching
    // performace counter value for that time.
    ::GetSystemTimeAsFileTime(&ft0);
    i = 0;

    do {
      ::GetSystemTimeAsFileTime(&ft1);
      ::QueryPerformanceCounter(&li);
    }
    while ((ft0.dwHighDateTime == ft1.dwHighDateTime) && (ft0.dwLowDateTime == ft1.dwLowDateTime));
  }
  static void get_file_time(LARGE_INTEGER& frequency, reference_point& reference, FILETIME& current_time) {
    LARGE_INTEGER li;
    ::QueryPerformanceCounter(&li);
    // Calculate performance counter ticks elapsed
    LARGE_INTEGER ticks_elapsed;
    ticks_elapsed.QuadPart = li.QuadPart - reference.counter.QuadPart;
    // Translate to 100-nanosecondsintervals (FILETIME
    // resolution) and add to
    // reference FILETIME to get current FILETIME.
    ULARGE_INTEGER filetime_ticks, filetime_ref_as_ul;
    filetime_ticks.QuadPart = (ULONGLONG)
        ((((double) ticks_elapsed.QuadPart / (double) frequency.QuadPart) * 10000000.0) + 0.5);
    filetime_ref_as_ul.HighPart = reference.file_time.dwHighDateTime;
    filetime_ref_as_ul.LowPart = reference.file_time.dwLowDateTime;
    filetime_ref_as_ul.QuadPart += filetime_ticks.QuadPart;
    // Copy to result
    current_time.dwHighDateTime = filetime_ref_as_ul.HighPart;
    current_time.dwLowDateTime = filetime_ref_as_ul.LowPart;
  }
public:
  sftime() {
    ::QueryPerformanceFrequency(&frequency);
    simplistic_synchronize(ref_point);
    GetTimeZoneInformation(&zone_info);
    get_time();
  }
  self& get_time() {
    get_file_time(frequency, ref_point, file_time);
    ::FileTimeToSystemTime(&file_time, &system_time);
    SystemTimeToTzSpecificLocalTime(&zone_info, &system_time, &system_time);
    return *this;
  }
  static void set_time(const SYSTEMTIME& system_time) {
    SetLocalTime(&system_time);
  }
  // friend double operator-(const self& first_time, const self& second_time) {
  // enum {
  // LLL = sizeof(DWORD) * 8
  // };
  // const FILETIME& ft1 = first_time.file_time;
  // const FILETIME& ft2 = second_time.file_time;
  // double dh = ft1.dwHighDateTime - ft2.dwHighDateTime;
  // double dl = ft1.dwLowDateTime - ft2.dwLowDateTime;
  // return dh * (1 << LLL) + dl;
  // }
public:
  word_t& year() {
    return system_time.wYear;
  }
  word_t& month() {
    return system_time.wMonth;
  }
  word_t& dayofweek() {
    return system_time.wDayOfWeek;
  }
  word_t& day() {
    return system_time.wDay;
  }
  word_t& hour() {
    return system_time.wHour;
  }
  //子午线标准时间 +8==北京时间
  word_t minute() {
    return system_time.wMinute;
  }
  word_t second() {
    return system_time.wSecond;
  }
  word_t millis() {
    return system_time.wMilliseconds;
  }
  self& operator++() {
    get_time();
    return *this;
  }
  double to_double() {
    double dd = year() * 100 + month();
    dd = dd * 100 + day();
    dd = dd * 100 + hour();
    dd = dd * 100 + minute();
    dd = dd * 100 + second();
    dd += 0.001 * millis();
    return dd;
  }
  std::string str() {
    char buf[64];
    _snprintf(buf, 64, "%04d%02d%02d%02d%2d%02d.%3d", (year()), (month()), (day()), (hour()), (minute()), (second()), (millis()));
    return buf;
  }
};
#else
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <string>
// 定时器：为 Windows 实现一个连续更新，高精度的时间供应器
class sftime
{
public:
  typedef unsigned short word_t;
  typedef sftime self;
private:
  struct tm system_time;
public:
  sftime() {
    get_time();
  }
  self& get_time() {
    time_t now; //实例化time_t结构
    time(&now);
    system_time = *localtime(&now);
    return *this;
  }
public:
  word_t year() {
    return system_time.tm_year + 1900;
  }
  word_t month() {
    return system_time.tm_mon;
  }
  word_t dayofweek() {
    return system_time.tm_wday;
  }
  word_t day() {
    return system_time.tm_mday;
  }
  word_t hour() {
    return system_time.tm_hour;
  }
  //子午线标准时间 +8==北京时间
  word_t minute() {
    return system_time.tm_min;
  }
  word_t second() {
    return system_time.tm_sec;
  }
  word_t millis() {
    return 0;
  }
  self& operator++() {
    get_time();
    return *this;
  }
  double to_double() {
    double dd = year() * 100 + month();
    dd = dd * 100 + day();
    dd = dd * 100 + hour();
    dd = dd * 100 + minute();
    dd = dd * 100 + second();
    dd += 0.001 * millis();
    return dd;
  }
  std::string str() {
#ifndef WIN32
#define _snprintf snprintf
#endif
    char buf[64];
    _snprintf(buf, 64, "%04d%02d%02d%02d%2d%02d.%3d", (year()), (month()), (day()), (hour()), (minute()), (second()), (millis()));
    return buf;
  }
};
#endif
#if 0
#include <iostream>
inline long test_sftime()
{
  sftime sf;
  long i;

  for (i = 100; i--;) {
    ++sf;
    std::cout << std::setw(2) << sf.hour() << ':' << std::setw(2) << sf.minute() << ':' << std::setw(2) << sf.second() << ':' << std::setw(3) << sf.millis() << '\n';
  }

  return 0;
}
#endif
#endif

