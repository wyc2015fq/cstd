#ifndef _SFTIME_H_
#define _SFTIME_H_
/*
sf_get_time();
_snprintf(buf, 256, ".\\cap\\cap_%02d%02d%02d%03d.avi",
sfhour(), sfminute() , sfsecond() , sfmillis() );
_snprintf(buf, 256, "./bmp/cap%04d%02d%02d%02d%02d%02d%03d.bmp",
            sfyear(), sfmonth(), sfday(), sfhour(), sfminute() , sfsecond() , sfmillis());
*/
#if 0
#include <windows.h>
#define set_time( system_time ) SetLocalTime(system_time)
#define sfyear()         sftime_init()->system_time.wYear
#define sfmonth()        sftime_init()->system_time.wMonth
#define sfdayofweek()    sftime_init()->system_time.wDayOfWeek
#define sfday()          sftime_init()->system_time.wDay
#define sfhour()         sftime_init()->system_time.wHour
//子午线标准时间 +8==北京时间
#define sfminute()       sftime_init()->system_time.wMinute
#define sfsecond()       sftime_init()->system_time.wSecond
#define sfmillis()       sftime_init()->system_time.wMilliseconds
#define sfto_double()    (((((sfyear(sf)*12. + sfmonth(sf)) * 30 + sfday(sf)) * 24 + sfhour(sf))* 60 + sfminute(sf))* 60 + sfsecond(sf))*1000 + sfmillis(sf)
#define sftotalMillis()  (double)((sfyear(sf)*365.+sfmonth(sf)*30+sfday(sf) )*24*60*60*1000.0+sfsecond(sf)*1000+sfmillis(sf))
//////////////////////////////////////////////////////////////////////////
// 定时器：为 Windows 实现一个连续更新，高精度的时间供应器
typedef struct reference_point {
  FILETIME        file_time;
  LARGE_INTEGER   counter;
} reference_point;
typedef struct SFTIME {
  reference_point ref_point;
  LARGE_INTEGER   frequency;
  FILETIME        file_time;
  SYSTEMTIME      system_time;
  TIME_ZONE_INFORMATION zone_info;
} SFTIME;
//同步
static void simplistic_synchronize(reference_point* ref_point)
{
  long           i   = 0;
  FILETIME       ft0 = { 0, 0 };
  FILETIME*      ft1 = &(ref_point->file_time);
  LARGE_INTEGER* li  = &(ref_point->counter);
  // Spin waiting for a change in system time. Get the matching
  // performace counter value for that time.
  GetSystemTimeAsFileTime(&ft0);
  do {
    GetSystemTimeAsFileTime(ft1);
    QueryPerformanceCounter(li);
  }
  while ((ft0.dwHighDateTime == ft1->dwHighDateTime) && (ft0.dwLowDateTime == ft1->dwLowDateTime));
}
static SFTIME* sftime_init()
{
  static SFTIME sf[1];
  static int sfinited = 0;
  if (0 == sfinited) {
    sfinited = 1;
    QueryPerformanceFrequency(&sf->frequency);
    simplistic_synchronize(&sf->ref_point);
    GetTimeZoneInformation(&sf->zone_info);
  }
  return (sf);
}
static SFTIME* sf_get_time()
{
  SFTIME* sf = sftime_init();
  {
    LARGE_INTEGER* frequency = &sf->frequency;
    reference_point* reference = &sf->ref_point;
    FILETIME* current_time = &sf->file_time;
    LARGE_INTEGER li;
    LARGE_INTEGER ticks_elapsed;
    ULARGE_INTEGER filetime_ticks, filetime_ref_as_ul;
    QueryPerformanceCounter(&li);
    // Calculate performance counter ticks elapsed
    ticks_elapsed.QuadPart = li.QuadPart - reference->counter.QuadPart;
    // Translate to 100-nanosecondsintervals (FILETIME
    // resolution) and add to
    // reference FILETIME to get current FILETIME.
    filetime_ticks.QuadPart = (ULONGLONG)
        ((((double) ticks_elapsed.QuadPart / (double) frequency->QuadPart) * 10000000.0) + 0.5);
    filetime_ref_as_ul.HighPart = reference->file_time.dwHighDateTime;
    filetime_ref_as_ul.LowPart = reference->file_time.dwLowDateTime;
    filetime_ref_as_ul.QuadPart += filetime_ticks.QuadPart;
    // Copy to result
    current_time->dwHighDateTime = filetime_ref_as_ul.HighPart;
    current_time->dwLowDateTime = filetime_ref_as_ul.LowPart;
  }
  FileTimeToSystemTime(&sf->file_time, &sf->system_time);
  SystemTimeToTzSpecificLocalTime(&sf->zone_info, &sf->system_time, &sf->system_time);
  return (sf);
}
// time1 - time2
static double systime_diff(const SYSTEMTIME* sTime1, const SYSTEMTIME* sTime2)
{
  ULARGE_INTEGER fTime1;/*FILETIME*/
  ULARGE_INTEGER fTime2;/*FILETIME*/
  __int64 counter = 10 * 1000 * 1000;
  double dft;
  SystemTimeToFileTime(sTime1, (FILETIME*)&fTime1);
  SystemTimeToFileTime(sTime2, (FILETIME*)&fTime2);
  dft = (fTime1.QuadPart - fTime2.QuadPart) * 1. / counter;
  return dft;
}
#else
#include <time.h>
#define set_time( system_time ) SetLocalTime(system_time)
#define sfyear()         sftime_init()->tm_year
#define sfmonth()        sftime_init()->tm_mon
#define sfdayofweek()    sftime_init()->tm_wday
#define sfday()          sftime_init()->tm_mday
#define sfhour()         sftime_init()->tm_hour
//子午线标准时间 +8==北京时间
#define sfminute()       sftime_init()->tm_min
#define sfsecond()       sftime_init()->tm_sec
#define sfmillis()       0
#define sfto_double()    (((((sfyear(sf)*12. + sfmonth(sf)) * 30 + sfday(sf)) * 24 + sfhour(sf))* 60 + sfminute(sf))* 60 + sfsecond(sf))*1000 + sfmillis(sf)
#define sftotalMillis()  (double)((sfyear(sf)*365.+sfmonth(sf)*30+sfday(sf) )*24*60*60*1000.0+sfsecond(sf)*1000+sfmillis(sf))
static struct tm* sftime_init() {
  static struct tm sf[1];
  return (sf);
}
static struct tm* sf_get_time() {
  time_t timep;
  struct tm* p;
  time(&timep);
  p = gmtime(&timep);
  *sftime_init() = * p;
  return sftime_init();
}
#endif
#endif // _SFTIME_H_

