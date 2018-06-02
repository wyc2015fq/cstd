#include <time.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef __ANDROID__
#include <android/log.h>
#endif //__ANDROID__ 
#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif
enum LogLevel {
  EASYCNN_LOG_LEVEL_VERBOSE,
  EASYCNN_LOG_LEVEL_CRITICAL,
  EASYCNN_LOG_LEVEL_FATAL,
  EASYCNN_LOG_LEVEL_NONE
};
#if MSC_VER<1300
#ifndef vsnprintf
#define vsnprintf _vsnprintf
#endif // vsnprintf
#endif
//default : console
#if 0
static char* formatString(const char* fmt, va_list args)
{
  char* content;
  const int size = vsnprintf(NULL, 0, fmt, args);
  if (size > 0) {
    content.resize(size);
    vsprintf(const_cast<char*>(content.data()), fmt, args);
  }
  return content;
}
#endif
static const char* level2str(enum LogLevel level)
{
  switch (level) {
  case EASYCNN_LOG_LEVEL_VERBOSE:
    return "verbose";
  case EASYCNN_LOG_LEVEL_CRITICAL:
    return "critical";
  case EASYCNN_LOG_LEVEL_FATAL:
    return "fatal";
  default:
    break;
  }
  return "unknown";
}
static char* buildInnerContent(char* buf, int len, enum LogLevel level, const char* content)
{
  const time_t t = time(NULL);
  const struct tm* local = localtime(&t);
  //_snprintf(buf, len, "[%04d/%02d/%02d %02d:%02d:%02d][%s]%s\n", local->tm_year + 1990, local->tm_mon + 1, local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec, level2str(level), content);
  _snprintf(buf, len, "[%02d:%02d:%02d]%s\n", local->tm_hour, local->tm_min, local->tm_sec, content);
  return buf;
}
static void defaultLogRoute(enum LogLevel level, const char* content)
{
  char buf[1024];
  const char* innerContent = buildInnerContent(buf, 1024, level, content);
#ifdef __ANDROID__
  __android_log_print(ANDROID_LOG_INFO, "digit", "log : %s", innerContent.c_str());
#else
  printf(innerContent);
#endif //__ANDROID__
}
//////////////////////////////////////////////////////////////////////////
static enum LogLevel globalLogLevel = EASYCNN_LOG_LEVEL_VERBOSE;
typedef void(*LogCb_t)(enum LogLevel, const char* content);
static LogCb_t globalLogCb = defaultLogRoute;
//log level setting
static void setLogLevel(enum LogLevel level)
{
  globalLogLevel = level;
}
static enum LogLevel getLogLevel()
{
  return globalLogLevel;
}
//log route setting
void setLogRedirect(LogCb_t logCb)
{
  globalLogCb = logCb;
}
//log function
void logVerbose(const char* fmt, ...)
{
  if (globalLogCb) {
    enum LogLevel level = EASYCNN_LOG_LEVEL_VERBOSE;
    char buf[1024];
    va_list args;
    if (globalLogLevel > level) {
      return;
    }
    va_start(args, fmt);
    vsnprintf(buf, 1024, fmt, args);
    va_end(args);
    globalLogCb(level, buf);
  }
}
void logCritical(const char* fmt, ...)
{
  if (globalLogCb){
    enum LogLevel level = EASYCNN_LOG_LEVEL_CRITICAL;
    char buf[1024];
    va_list args;
    if (globalLogLevel > level) {
      return;
    }
    va_start(args, fmt);
    vsnprintf(buf, 1024, fmt, args);
    va_end(args);
    globalLogCb(level, buf);
  }
}
void logFatal(const char* fmt, ...)
{
  enum LogLevel level = EASYCNN_LOG_LEVEL_FATAL;
  char buf[1024];
  va_list args;
  if (globalLogLevel > level) {
    return;
  }
  va_start(args, fmt);
  vsnprintf(buf, 1024, fmt, args);
  va_end(args);
  globalLogCb(level, buf);
}
#include <assert.h>
static char* __FILE = NULL;
static char* __FUNCTION = NULL;
static int __LINE = 0;
#if 0
#define easyAssert (__FILE = __FILE__, __FUNCTION = __FUNCTION__, __LINE = __LINE__),easyAssertCore
#else
#define easyAssert (__FILE = __FILE__, __FUNCTION = "", __LINE = __LINE__),easyAssertCore
#endif
static void* globalAssertFatalUserData = NULL;
static void(*globalAssertFatalCB)(void* userData, const char* errorStr) = NULL;
void setAssertFatalCallback(void(*cb)(void* userData, const char* errorStr), void* userData)
{
  globalAssertFatalCB = cb;
  globalAssertFatalUserData = userData;
}
int easyAssertCore(int condition, const char* fmt, ...)
{
  const char* file = __FILE;
  const char* function = __FUNCTION;
  long line = __LINE;
  if (!condition) {
    char errorStr[256];
    va_list args;
    va_start(args, fmt);
    vsnprintf(errorStr, 256, fmt, args);
    logFatal("FILE:%s,FUNCTION:%s,LINE:%d", file, function, line);
    logFatal(fmt, args);
    va_end(args);
    if (globalAssertFatalCB) {
      globalAssertFatalCB(globalAssertFatalUserData, errorStr);
    }
    else {
      assert(0);
      exit(0);
    }
  }
  return 0;
}

