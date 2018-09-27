#include"time.h"
#include"stdio.h"
#include"string"
static std::string strtime(const char* fmt) {
  time_t rawtime;
  tm *time1;
  char buf[256];
  time(&rawtime);
  time1 = localtime(&rawtime);
  fmt = fmt ? fmt : "%Y-%m-%d %H:%M:%S";
  strftime(buf, 256, fmt, time1);
  return buf;
}

int test_sftime()
{
  strtime("%Y-%m-%d %H:%M:%S");
  return 0;
}

