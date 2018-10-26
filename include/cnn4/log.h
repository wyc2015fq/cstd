
#include <stdio.h>
#include <stdarg.h>

int debugUart = 0;

enum DebugType
{
  VERBOSE,
  DEBUG,
  WARNING,
  FATAL,
};

static void setDebugUart(int debug)
{
  debugUart = debug;
  printf("setDebugUart,%d\n", debug);
}
static int logv(const char* format, va_list args_list) {
  return vprintf(format, args_list);
}
static int log(int filter, const char* format, ...)
{
  int ret = 0;
  if (filter >= debugUart) {
    va_list marker;
    va_start(marker, format);
    ret = logv(format, marker);
    va_end(marker);
  }
  return ret;
}
