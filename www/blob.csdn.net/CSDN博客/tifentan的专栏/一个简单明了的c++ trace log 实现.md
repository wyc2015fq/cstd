# 一个简单明了的c++ trace log 实现 - tifentan的专栏 - CSDN博客

2017年10月31日 15:26:48[露蛇](https://me.csdn.net/tifentan)阅读数：1018


近段时间被旧的trace log 类折磨，实在太多bug，着手在windows下实现了个，延续一向我的风格，简单明了能不写多一行废代码绝对不写，然后要容易读懂。

基本要求是 

1.线程安全 

2.支持分级过滤 

3.记录时间，线程id，开始记录头等等 

4.跨平台 

5.支持回调处理

代码是考虑过跨平台的，主要是锁跟几个函数，有需要改动的地方应该很少，但目前还没做，因为要浪费时间测试呢，往后有需要再改改就是了。

废话不多说上代码

头文件

```
/*
* CREATE: tifentan
* DATE: 2017/10/30
* MODIFY: 2017/10/30
* NOTE:
*   trace.h 
*   有疑问我qq 522935050
*
*/

#ifndef  TTF_TRACE_H_
#define TTF_TRACE_H_

#include <string>

enum TtfTraceLevel
{
    ttfTraceNone = 0x0000,    // no trace
    ttfTraceStateInfo = 0x0001,
    ttfTraceWarning = 0x0002,
    ttfTraceError = 0x0004,
    ttfTraceCritical = 0x0008,
    ttfTraceApiCall = 0x0010,
    ttfTraceModuleCall = 0x0020,

    ttfTraceInfo = 0x0040,
    ttfTraceDebugFlag = 0x007f,
    ttfTraceDebug = 0x0080,

    ttfTraceDefault = 0x00ff,

    ttfTraceMemory = 0x0100,   // memory info
    ttfTraceTimer = 0x0200,   // timing info
    ttfTraceStream = 0x0400,   // "continuous" stream of data

    // Non-verbose level used by LS_INFO of logging.h. Do not use directly.
    ttfTraceTerseInfo = 0x2000,

    ttfTraceAll = 0xffff
};

enum TtfTraceMode {
    ttfTraceModeNone,
    ttfTraceModeEnd = 0x0ffff,
};

// External Trace API
class TtfTraceCallback {
public:
    virtual void Print(TtfTraceLevel level, const char* message, int length) = 0;

protected:
    virtual ~TtfTraceCallback() {}
    TtfTraceCallback() {}
};

#define TTF_TRACE_MAX_MESSAGE_SIZE 1024
#define TTF_TRACE_MAX_FILE_SIZE 100000000 //100m

class TtfTrace {
public:
    TtfTrace();
    ~TtfTrace();

    int SetFile(char* path);
    int SetCallBack(TtfTraceCallback* cb);
    int SetFilter(unsigned int filter);
    unsigned int GetFilter();

    int doTrace(const TtfTraceLevel level, 
        const char msg[TTF_TRACE_MAX_MESSAGE_SIZE],
        const TtfTraceMode module = ttfTraceModeNone,
        const int32_t id = 0);
    int doTraceEx(const TtfTraceLevel level,
        const TtfTraceMode module,
        const int32_t id,
        char* str,
        ...);

    int Flush();
    int Rewind();
    unsigned int GetSize();

protected:
    int AddThreadId(char* ptr);
    int AddTime(char* ptr);
    int AddHead(char* ptr);
    int AddLevel(char* ptr, TtfTraceLevel level);
    int AddModuleAndId(char* ptr, TtfTraceMode module,int32_t id);
    int WriteToFile();

private:
    TtfTraceCallback *cb_;
    CRITICAL_SECTION crit_;
    std::string trace_file_path_;
    FILE* file_ = nullptr;
    size_t position_ = 0;
    unsigned int level_filter_ = ttfTraceDefault;
    //unsigned int prev_tick_count_ = 0;
    char* msg_ = 0;
    int msg_len_ = 0;
    size_t row_ = 0;
};

//使用这个宏就可以用了
#define DEFINE_GLOBAL_TTF_TRACE extern TtfTrace* g_ttf_trace;

//跨模块时使用下面两个来使用同一个trace
#define DECLARE_GLOBAL_TTF_TRACE TtfTrace* g_ttf_trace = 0;
#define INIT_GLOBAL_TTF_TRACE(trace) g_ttf_trace = trace;

//在某个文件中定义一个
#define CREATE_GLOBAL_TTF_TRACE INIT_GLOBAL_TTF_TRACE(new TtfTrace())

//使用其中之一来创建一个全局的trace file
void g_create_ttf_trace_file(char* str);
#define CREATE_GLOBAL_TTF_TRACE_FILE g_create_ttf_trace_file

//释放,跨模块时自己注意一下释放问题，一定保证在所有调用后释放,不放心自己加个全局锁吧
void g_release_ttf_trace_file();
#define RELEASE_GLOBAL_TTF_TRACE_FILE g_release_ttf_trace_file

//void g_do_ttf_trace(const TtfTraceLevel level,
//  const TtfTraceMode module,
//  const int32_t id,
//  const char* msg, ...);
#define TTF_TRACE_EX(level,module,id,msg,...) if(g_ttf_trace) \
                                                g_ttf_trace->doTraceEx(level,module,id,msg,##__VA_ARGS__);

#define TTF_TRACE(level,msg,...) if(g_ttf_trace) \
                                                g_ttf_trace->doTraceEx(level,ttfTraceModeNone,0,msg,##__VA_ARGS__);

#endif // ! TTF_TRACE_H_
```

实现cpp文件

```cpp
#include "windows.h"
#include "ttf_trace.h"

/*
EnterCriticalSection(&crit_);
LeaveCriticalSection(&crit_);

*/

TtfTrace::TtfTrace()
    :cb_(nullptr)
{
    InitializeCriticalSection(&crit_);
    msg_ = new char[TTF_TRACE_MAX_MESSAGE_SIZE];
}

TtfTrace::~TtfTrace()
{
    if (file_) {
        fclose(file_);
    }
    if (msg_) {
        delete[] msg_;
    }
    DeleteCriticalSection(&crit_);
}

int TtfTrace::SetFile(char * path)
{
    if (path == nullptr) {
        return -1;
    }
    int ret = 0;

    EnterCriticalSection(&crit_);
    if (file_) {
        fclose(file_);
    }
    trace_file_path_.clear();
    file_ = fopen(path, "ab");
    if (file_) {
        fseek(file_, 0, SEEK_END);
        position_ = ftell(file_);
        fseek(file_, 0, SEEK_SET);
    }
    else {
        ret = -1;
    }
    trace_file_path_ = path;
    LeaveCriticalSection(&crit_);

    return ret;
}

int TtfTrace::SetCallBack(TtfTraceCallback* cb)
{
    EnterCriticalSection(&crit_);
    cb_ = cb;
    LeaveCriticalSection(&crit_);

    return 0;
}

int TtfTrace::SetFilter(unsigned int filter)
{
    EnterCriticalSection(&crit_);
    level_filter_ = filter;
    LeaveCriticalSection(&crit_);
    return 0;
}

unsigned int TtfTrace::GetFilter()
{
    return level_filter_;
}

int TtfTrace::doTrace(const TtfTraceLevel level, const char msg[TTF_TRACE_MAX_MESSAGE_SIZE], const TtfTraceMode module, const int32_t id)
{
    if((level & level_filter_) == 0){
        return 0;
    }
    int ret = 0;
    char * p = msg_;
    EnterCriticalSection(&crit_);
    if ( file_) { //能过滤且文件存在
        if (position_ > TTF_TRACE_MAX_FILE_SIZE) {
            fflush(file_);
            position_ = 0;
            fseek(file_, 0, SEEK_SET);
            row_ = 0;
        }
        if (row_ == 0) {
            msg_len_ = AddHead(msg_);
            ret = WriteToFile();
            if (ret>0) {
                row_++;
            }
        }
        p += AddTime(p);
        p += AddThreadId(p);
        p += AddLevel(p,level);
        p += AddModuleAndId(p, module, id);
        msg_len_ = p - msg_;
        int rest = TTF_TRACE_MAX_MESSAGE_SIZE - msg_len_;
        ret = snprintf(p, rest, "%s", msg_); //最后一个0 将代替成 \n
        //_snprintf 这两个处理跟返回值都不一样，考虑到跨平台，统一一个算了
        //https://msdn.microsoft.com/en-us/library/2ts7cx93.aspx
        if (ret<0 || ret >= rest) {
            ret = rest - 1;
        }
        msg_len_ += ret;
        ret = WriteToFile();
        if (ret>0) {
            row_++;
        }
    }
    LeaveCriticalSection(&crit_);
    return ret;
}

int TtfTrace::doTraceEx(const TtfTraceLevel level, const TtfTraceMode module, const int32_t id, char * str, ...)
{
    if ((level & level_filter_) == 0) {
        return 0;
    }
    int ret = 0;
    char * p = msg_;
    EnterCriticalSection(&crit_);
    if (file_) { //能过滤且文件存在
        if (position_ > TTF_TRACE_MAX_FILE_SIZE) {
            fflush(file_);
            position_ = 0;
            fseek(file_, 0, SEEK_SET);
            row_ = 0;
        }
        if (row_ == 0) {
            msg_len_ = AddHead(msg_);
            ret = WriteToFile();
            if (ret>0) {
                row_++;
            }
        }
        p += AddTime(p);
        p += AddThreadId(p);
        p += AddLevel(p, level);
        p += AddModuleAndId(p, module, id);
        msg_len_ = p - msg_;
        int rest = TTF_TRACE_MAX_MESSAGE_SIZE - msg_len_;

        va_list args;
        va_start(args, str);
        //ret = _vsnprintf(p, TTF_TRACE_MAX_MESSAGE_SIZE - 1, msg, args);
        ret = vsnprintf(p, rest, str, args);
        va_end(args);

        if (ret<0 || ret >= rest) {
            ret = rest - 1;
        }
        msg_len_ += ret;
        ret = WriteToFile();
        if (ret>0) {
            row_++;
        }
    }
    LeaveCriticalSection(&crit_);
    return ret;
}

int TtfTrace::Flush()
{
    EnterCriticalSection(&crit_);
    if (file_) {
        fflush(file_);
    }
    LeaveCriticalSection(&crit_);
    return 0;
}

int TtfTrace::Rewind()
{
    EnterCriticalSection(&crit_);
    if (file_) {
        position_ = 0;
        fseek(file_, 0, SEEK_SET);
    }
    LeaveCriticalSection(&crit_);
    return 0;
}

unsigned int TtfTrace::GetSize()
{
    return position_;
}

int TtfTrace::AddThreadId(char * ptr)
{
    return sprintf(ptr, "[PID:%u]|", GetCurrentThreadId());
}

int TtfTrace::AddTime(char * ptr)
{
    //uint32_t dw_current_time = timeGetTime();
    SYSTEMTIME system_time;
    GetLocalTime(&system_time);

    /*uint32_t dw_delta_time = dw_current_time - prev_tick_count_;
    if (prev_tick_count_ == 0) {
        dw_delta_time = 0;
    }
    prev_tick_count_ = dw_current_time;*/

    //if (dw_delta_time > 0x0fffffff) {
    //  // Either wraparound or data race.
    //  dw_delta_time = 0;
    //}
    //if (dw_delta_time > 99999) {
    //  dw_delta_time = 99999;
    //}
    int ret = 0;
    ret = sprintf(ptr, "[%02u:%02u:%02u:%03u]|", system_time.wHour,
        system_time.wMinute, system_time.wSecond,
        system_time.wMilliseconds);
    if (ret < 0) ret = 0;
    return ret;
}

static int Unicode2Utf8(const wchar_t * unicode, char * utf8, int nBuffSize)
{
    if (!unicode || !wcslen(unicode))
    {
        return 0;
    }
    int len;
    len = WideCharToMultiByte(CP_UTF8, 0, unicode, -1, NULL, 0, NULL, NULL);
    if (len > nBuffSize)
    {
        return 0;
    }
    WideCharToMultiByte(CP_UTF8, 0, unicode, -1, utf8, len, NULL, NULL);
    return len;
}

int TtfTrace::AddHead(char * ptr)
{
    //prev_tick_count_ = timeGetTime();

    SYSTEMTIME sys_time;
    GetLocalTime(&sys_time);

    TCHAR sz_date_str[20];
    TCHAR sz_time_str[20];
    char utf8_data[128];
    char utf8_time[128];

    // Create date string (e.g. Apr 04 2002)
    GetDateFormat(LOCALE_SYSTEM_DEFAULT, 0, &sys_time, TEXT("MMM dd yyyy"),
        sz_date_str, 20);

    // Create time string (e.g. 15:32:08)
    GetTimeFormat(LOCALE_SYSTEM_DEFAULT, 0, &sys_time, TEXT("HH':'mm':'ss"),
        sz_time_str, 20);
    Unicode2Utf8(sz_date_str, utf8_data, 128);
    Unicode2Utf8(sz_time_str, utf8_time, 128);

    //sprintf(trace_message, "Local Date: %ls Local Time: %ls", sz_date_str,
    //  sz_time_str);
    int ret = 0;
    ret = sprintf(ptr, "Local Date: [%s] Local Time: [%s]", utf8_data,
        utf8_time);
    if (ret < 0) ret = 0;
    return ret;
}

int TtfTrace::AddLevel(char * ptr, TtfTraceLevel level)
{
    int ret = 0;;
    switch (level) {
    case ttfTraceTerseInfo:
        // Add the appropriate amount of whitespace.
        ret = sprintf(ptr, "[]|");
        break;
    case ttfTraceStateInfo:
        ret =sprintf(ptr, "[STATEINFO]|");
        break;
    case ttfTraceWarning:
        ret = sprintf(ptr, "[WARNING]|");
        break;
    case ttfTraceError:
        ret = sprintf(ptr, "[ERROR]|");
        break;
    case ttfTraceCritical:
        ret = sprintf(ptr, "[CRITICAL]|");
        break;
    case ttfTraceInfo:
        ret = sprintf(ptr, "[INFO]|");
        break;
    case ttfTraceModuleCall:
        ret = sprintf(ptr, "[MODULECALL]|");
        break;
    case ttfTraceMemory:
        ret = sprintf(ptr, "[MEMORY]|");
        break;
    case ttfTraceTimer:
        ret = sprintf(ptr, "[TIMER]|");
        break;
    case ttfTraceStream:
        ret = sprintf(ptr, "[STREAM]|");
        break;
    case ttfTraceApiCall:
        ret = sprintf(ptr, "[APICALL]|");
        break;
    case ttfTraceDebug:
        ret = sprintf(ptr, "[DEBUG]|");
        break;
    default:
        return 0;
    }
    if (ret < 0) ret = 0;
    // All messages are 12 characters.
    return ret;
}

int TtfTrace::AddModuleAndId(char * ptr, TtfTraceMode module, int32_t id)
{
    const unsigned long int id_engine = id >> 16;
    const unsigned long int id_channel = id & 0xffff;
    return 0;
}

int TtfTrace::WriteToFile()
{
    //if (file_ == nullptr) return 0;
    msg_[msg_len_] = '\n';
    int totol = msg_len_ + 1;
    int ret = fwrite(msg_,1, totol,file_);
    if (ret == totol) {
        position_ += ret;
    }else { //write error
        OutputDebugStringA("Trace error write..................\n");
        ret = 0;
    }

    return ret;
}

DECLARE_GLOBAL_TTF_TRACE

void g_create_ttf_trace_file(char * str)
{
    CREATE_GLOBAL_TTF_TRACE
    g_ttf_trace->SetFile(str);
}

void g_release_ttf_trace_file()
{
    if (g_ttf_trace) {
        delete g_ttf_trace;
        g_ttf_trace = 0;
    }       
}

//void g_do_ttf_trace(const TtfTraceLevel level, const TtfTraceMode module, const int32_t id, const char * msg, ...)
//{
//  if (g_ttf_trace) {
//      
//  }
//}
```

最后写个简单的使用例子：

```
#include "ttf_trace.h"
DEFINE_GLOBAL_TTF_TRACE
void main(){
CREATE_GLOBAL_TTF_TRACE_FILE("test.log");

    TTF_TRACE_EX(ttfTraceInfo, ttfTraceModeNone,0,"what the fuck");
    TTF_TRACE(ttfTraceDebug,"cao nids%d",1);
    TTF_TRACE(ttfTraceDebug, "cao nids%d", 2);
    TTF_TRACE(ttfTraceDebug, "cao nids%d", 3);
    TTF_TRACE(ttfTraceDebug, "cao nids%d", 3);
    TTF_TRACE(ttfTraceDebug, "cao nids%d", 4);

    RELEASE_GLOBAL_TTF_TRACE_FILE();
}
```

再上个notepad++上看到的log文件效果： 
![这里写图片描述](https://img-blog.csdn.net/20171031153304486?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlmZW50YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

妈蛋，再也不用原来那个垃圾代码了。

