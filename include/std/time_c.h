

#ifndef __TIME_C_H__
#define __TIME_C_H__

#include <time.h>
#include <stdio.h>

static char* strtime2(char* buf, int len)
{
  time_t timep;
  struct tm* p;
  time(&timep);
  p = localtime(&timep); /*取得当地时间*/
  _snprintf(buf, len, "%04d%02d%02d%02d%02d%02d", (1900 + p->tm_year), (1 + p->tm_mon), p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
  return (char*)buf;
}
static char* strtime3(char* buf, int len, const char* fmt)
{
  time_t rawtime;
  struct tm* time1;
  time(&rawtime);
  time1 = localtime(&rawtime);
  strftime(buf, len, fmt, time1);
  return buf;
}
char* timenow()
{
  static char _time_buffer[80];
  return strtime2(_time_buffer, 80);
}


#endif // __TIME_C_H__