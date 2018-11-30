
#ifndef _LOGGING_HPP_
#define _LOGGING_HPP_

#include <iostream>
#ifdef _WIN32
#include <windows.h>
#undef max
#undef min
#endif
#include "flags.hpp"

#ifndef OutputDebugString
#define OutputDebugString printf
#endif

DEFINE_bool(logtostderr, true, "Set whether log messages go to stderr instead of logfiles");
DEFINE_bool(alsologtostderr, true, "Set whether log messages go to stderr in addition to logfiles.");
DEFINE_bool(colorlogtostderr, true, "Set color messages logged to stderr (if supported by terminal).");
DEFINE_int32(stderrthreshold, 0, "Log messages at a level >= this flag are automatically sent to stderr in addition to log files.");
DEFINE_bool(log_prefix, true, "Set whether the log prefix should be prepended to each line of output.");
DEFINE_int32(logbuflevel, 0, "Log messages at a level <= this flag are buffered. Log messages at a higher level are flushed immediately.");
DEFINE_int32(logbufsecs, 0, "Sets the maximum number of seconds which logs may be buffered for.");
DEFINE_int32(minloglevel, 0, "Log suppression level: messages logged at a lower level than this are suppressed.");
DEFINE_string(log_dir, "./", "If specified, logfiles are written into this directory instead of the default logging directory.");
DEFINE_int32(logfile_mode, 0, "Set the log file mode.");
DEFINE_string(log_link, "", "Sets the path of the directory into which to put additional links to the log files.");
DEFINE_int32(v, 0, "in vlog_is_on.cc");
DEFINE_int32(max_log_size, 100, "Sets the maximum log file size (in MB).");
DEFINE_bool(stop_logging_if_full_disk, true, "Sets whether to avoid logging to the disk if the disk is full.");

namespace wstd
{
  static int fappend(const char* filename, const void* buf, int len) {
    FILE* pf = fopen(filename, "a");
    if (pf) {
      fwrite(buf, len, 1, pf);
      fclose(pf);
    }
    return 0;
  }
class LogHelp {
public:

  LogHelp(const char* options, const char* file, int line) {
    string stime = strtime(NULL);
    string fnext = path_split_filenameext(file);
    //stream_ << wstd::format("%s %s(%d) %s:", stime.c_str(), fnext.c_str(), line, options);
    stream_ << wstd::format("%s(%d): %s:", file, line, options);
  }
  ~LogHelp() {
    stream_ << "\n";
    string s = stream_.str();
#ifdef _WIN32
    OutputDebugString(s.c_str());
#else
    puts(s.c_str());
#endif
    fappend("log.txt", s.c_str(), s.length());
  }
  std::stringstream& get() {
    return stream_;
  }
  std::stringstream stream_;
};
struct LogMessageVoidify {
  void operator&(std::ostream &) {}
};



#define LOGNULL  (void)0
#define LOG(severity)  wstd::LogHelp( #severity ,__FILE__,__LINE__ ).get()


#define LOG_IF(severity, condition)   !(condition) ? (void)0 : wstd::LogMessageVoidify() & LOG(severity)
#define GOOGLE_PREDICT_BRANCH_NOT_TAKEN(x)  (x)
#define CHECK(condition)        LOG_IF(FATAL, GOOGLE_PREDICT_BRANCH_NOT_TAKEN(!(condition))) << "Check failed: " #condition " "
#define CHECK_OP(name, op, val1, val2)  CHECK((val1) op (val2))
#define CHECK_EQ(val1, val2) CHECK_OP(_EQ, ==, val1, val2)
#define CHECK_NE(val1, val2) CHECK_OP(_NE, !=, val1, val2)
#define CHECK_LE(val1, val2) CHECK_OP(_LE, <=, val1, val2)
#define CHECK_LT(val1, val2) CHECK_OP(_LT, < , val1, val2)
#define CHECK_GE(val1, val2) CHECK_OP(_GE, >=, val1, val2)
#define CHECK_GT(val1, val2) CHECK_OP(_GT, > , val1, val2)
#define CHECK_NOTNULL(val) (CHECK(NULL!=(val)) << wstd::format("'%s'Must be non NULL", #val), val)
//LOG_EVERY_N:此代码每执行过N此后,打印一次,注:第一次也会打印
#define LOG_EVERY_N(severity, n)   static int LOG_EVERY_ ## __LINE__ = 0; LOG_IF(severity, (LOG_EVERY_ ## __LINE__)++%n==0)
//LOG_FIRST_N:此代码执行的前N此都打印,超过N次后不打印.
#define LOG_FIRST_N(severity, n)   static int LOG_EVERY_ ## __LINE__ = 0; LOG_IF(severity, (LOG_EVERY_ ## __LINE__)++<n)

#ifdef _DEBUG
#define DLOG LOG
#define DCHECK CHECK
#define DCHECK_EQ CHECK_EQ
#define DCHECK_NE CHECK_NE
#define DCHECK_LE CHECK_LE
#define DCHECK_LT CHECK_LT
#define DCHECK_GE CHECK_GE
#define DCHECK_GT CHECK_GT
#else
#define DLOG(severity)  LOG_IF(severity, false)
#define DCHECK(condition) LOG_IF(FATAL, false)
#define DCHECK_EQ(val1, val2) DCHECK(false)
#define DCHECK_NE(val1, val2) DCHECK(false)
#define DCHECK_LE(val1, val2) DCHECK(false)
#define DCHECK_LT(val1, val2) DCHECK(false)
#define DCHECK_GE(val1, val2) DCHECK(false)
#define DCHECK_GT(val1, val2) DCHECK(false)
#endif

  enum GLOGTYPE {
    GLOG_INFO = 0, GLOG_WARNING = 1, GLOG_ERROR = 2, GLOG_FATAL = 3,
    NUM_SEVERITIES = 4
  };
  static void SetLogDestination(enum GLOGTYPE type, const char* str) {}
  static void InitGoogleLogging(const char* str) {}
}

#endif // _LOGGING_HPP_
