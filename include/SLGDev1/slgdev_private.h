#ifndef _SLGDEV_PRIVATE_H_
#define _SLGDEV_PRIVATE_H_
#include "slgdev.h"

#define SYN_REQUES_CONNECT 0x01          //同步连接请求
#define SYN_ACK_CONNECT    0x10          //同步连接确认
#define SYN_REQUES_CONNECT 0x01          //同步连接请求
#define SYN_ACK_CONNECT    0x10          //同步连接确认
#define MULTYGROUP_PORT    13332         //多播组端口
#define MULTYGROUP_IP      "224.0.1.100" //多播组地址
#define MAX_MEMRESERVER_SIZE (1024*1024)
#define SERVER_PORT 13333
#define VIDEO_PORT 23334
#define BINDE_TIMES 20
#define SEND_TIMEOUT 1
#define SEND_BUF (64*1024)
#define RECV_BUF (64*1024)

struct slgdev_config_t
{
    int m_debug;
};
slgdev_config_t* get_config();

#if _MSC_VER < 1400
#define snprintf     _snprintf
#define vsnprintf     _vsnprintf
#endif

#ifdef _WIN32
//#define USE_SLGDB
#else
#define ZeroMemory(p, n) memset(p, 0, n)
#define Sleep usleep
#endif

#ifdef _WIN32

#ifdef WINCE
#define USE_NET_MODEL NET_SELECTED
#else
#define USE_NET_MODEL NET_OVERLAPPED
#endif

#else
#define USE_NET_MODEL NET_SELECTED
#endif


#define DEBUG_INFO OutputInfo

int OutputInfo(const char* format , ...);


#ifdef _WIN32
#pragma warning( disable : 4786 )
// Insert your headers here
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#else
#include <stdint.h>
#endif

#include <vector>
#include <map>
#include <list>
#include <deque>
#include <assert.h>
#include <string.h>
using std::map;
#ifdef WIN32
#pragma warning( disable : 4786 )
#include <winsock2.h>
#include <Ws2tcpip.h>
//#include <process.h>

#else
//#include "cutimer.h"
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#endif
#ifdef WIN32
#define SOCK_LENTH int
#define T_VOID void
#define T_HANDLE HANDLE
#define SLEEP(Milliseconds) {Sleep(Milliseconds);}

#define LASTERROR WSAGetLastError()
#define MILLISEC 1
#define MEMRESERVER_BLOCKS 10 //内存块大小
#else
#define SOCK_LENTH socklen_t
#define T_HANDLE pthread_t
#define T_VOID void*
#define INVALID_SOCKET 0
#define SOCKET_ERROR -1
#define ioctlsocket ioctl
#define SLEEP(Milliseconds) {struct timeval tval;tval.tv_sec= 0;tval.tv_usec=(Milliseconds); select( 0 , NULL , NULL , NULL , &tval ); }

#define LASTERROR errno
#define WSAECONNRESET ECONNRESET
#define WSAECONNABORTED ECONNABORTED
#define WSAEWOULDBLOCK EWOULDBLOCK
#define WSAEINTR EINTR
#define MILLISEC 1000
#define SD_BOTH SHUT_RDWR
#define MEMRESERVER_BLOCKS 2 //内存块大小
#define INFINITE 0xFFFFFFFF
//typedef unsigned long DWORD;
#define debugPrint(...) { printf(__VA_ARGS__); printf(" \r\n"); }

#endif
#define TIME_OUT 5 //10s
typedef sockaddr SOCKADDR, *PSOCKADDR;
typedef sockaddr_in SOCKADDR_IN;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long ulong;
typedef char int8;
typedef int int32;
typedef short int16;
#define IN
#define OUT
#define MAX_SEND_COUNTS 5 //包最大连续发送次数
#define MAX_CONNECTS 5 //一次侦听最大连接数
#define RECV_TIME_OUT (10*1000) //30s
#define PACK_SIZE (60*1024)
#define MEM_UNIT (8*1024)
#undef ALIGN
#define ALIGN(bytes, alignments) (((bytes)+((alignments)-1))&~((alignments)-1))
typedef struct
{
    uint32 msg_identify; //标识
    uint32 msg_offset ; //偏移
    uint32 msg_len; //数据长度
    uint32 msg_total; //数据总长度
    uint32 msg_checksum; //检验和
    // int8 msg_data[4]; //数据
} SOCK_DATA_HEADER;


#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif
class CriticalSection
{
#ifdef WIN32
    CRITICAL_SECTION m_sect;
#else
    pthread_mutex_t   m_sect;
    pthread_mutexattr_t m_attr;
#endif
#ifdef _DEBUG
    long m_Count;
#endif
public:
    CriticalSection()
    {
#ifdef _DEBUG
        m_Count = 0;
#endif
#ifdef WIN32
        ::InitializeCriticalSection(&m_sect);
#else
        pthread_mutexattr_init(&m_attr);
        pthread_mutexattr_settype(&m_attr, PTHREAD_MUTEX_RECURSIVE_NP);
        pthread_mutex_init(&m_sect, &m_attr);
#endif
    }
    ~CriticalSection()
    {
#ifdef _DEBUG

        if (m_Count != 0)
        {
            ::MessageBoxW(NULL , L"锁计数器不为" , L"warning!", MB_OK);
        }

#endif
#ifdef WIN32
        ::DeleteCriticalSection(&m_sect);
#else
        pthread_mutexattr_destroy(&m_attr);
        pthread_mutex_destroy(&m_sect);
#endif
    }
    void Lock(void)
    {
#ifdef _DEBUG
        InterlockedIncrement(&m_Count);
#endif
#ifdef WIN32
        ::EnterCriticalSection(&m_sect);
#else
        pthread_mutex_lock(&m_sect);
#endif
    }
    void Unlock(void)
    {
#ifdef _DEBUG
        InterlockedDecrement(&m_Count);
#endif
#ifdef WIN32
        ::LeaveCriticalSection(&m_sect);
#else
        pthread_mutex_unlock(&m_sect);
#endif
    }
};
class AoutLock
{
    CriticalSection* m_pCritalSection;
public:
    AoutLock(CriticalSection* pCritalSection)
    {
        if (pCritalSection)
        {
            pCritalSection->Lock();
            m_pCritalSection = pCritalSection;
        }
    }
    ~AoutLock()
    {
        if (m_pCritalSection)
        {
            m_pCritalSection->Unlock();
        }
    }
};

#if 0
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

    void restart()
    {
        _start_time = std::clock();
    }

    double elapsed() const
    {
        return double(std::clock() - _start_time) / CLOCKS_PER_SEC;
    }

    double operator()() const
    {
        return elapsed();
    }

private:
    std::clock_t _start_time;
}; // end of stopwatch class
#endif
#ifdef WIN32
#include <windows.h>
// 高精度计时类 (单位为秒, 精确到微秒)
//
class uutime
{
public:
    typedef __int64 value_type;
    // 获得计数器的时钟频率
    uutime(): freq(frequency())
    {
        restart();
    }

    // 获得初始值
    void restart()
    {
        start_time = counter();
    }

    // 获得对应的时间值，单位为秒
    double elapsed() const
    {
        return static_cast<double>((double)(counter() - start_time) / freq);
    }

    double operator()() const
    {
        return elapsed();
    }

public:
    value_type start_time, freq;
    static value_type frequency()
    {
        value_type frequency;
        ::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&frequency));
        return frequency;
    }

    static value_type counter()
    {
        value_type counter;
        ::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&counter));
        return counter;
    }

};
class CUtimer : public uutime
{
public:
    CUtimer()
    {
        restart();
    }

    void Restart(void)
    {
        restart();
    }

    unsigned long Elapsed(void)
    {
        return (unsigned long)((counter() - start_time) / 1000);
    }

};
#else
#include <sys/time.h>
class uutime
{
public:
    // 获得计数器的时钟频率
    uutime()
    {
        restart();
    }

    // 获得初始值
    void restart()
    {
        _start_time = utime_linux_count();
    }

    // 获得对应的时间值，单位为秒
    double elapsed() const
    {
        return count() / 1000000.;
    }

    double count() const
    {
        return (utime_linux_count() - _start_time);
    }

    double operator()() const
    {
        return elapsed();
    }

private:
    double _start_time;
    static double utime_linux_count()
    {
        struct timeval tv;
        double total = 0;

        if (gettimeofday(&tv , 0) == -1)
        {
            return 0;
        }

        total = tv.tv_sec * 1000000 + tv.tv_usec;
        return total;
    }

};
#if 0
// 返回自系统开机以来的毫秒数（tick）
struct CUtimer
{
    static unsigned int GetTickCount()
    {
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
    }

    static double utime_linux_count()
    {
        struct timeval tv;
        double total = 0;

        if (gettimeofday(&tv , 0) == -1)
        {
            return 0;
        }

        total = tv.tv_sec * 1000000 + tv.tv_usec;
        return total;
    }

    unsigned int Restart()
    {
        return 0;
    }

    unsigned int Elapsed()
    {
        return GetTickCount();
    }

};
#else
class CUtimer
{
    struct timeval m_start_time;
protected:
    unsigned long utime_count_timeval(struct timeval* _start_time)
    {
        struct timeval tv;
        time_t s;
        unsigned long us;
        unsigned long total = 0;

        if (gettimeofday(&tv , 0) == -1)
        {
            return 0;
        }

        s = tv.tv_sec - _start_time->tv_sec;
        us = tv.tv_usec - _start_time->tv_usec;
        total = s * 1000000 + us;
        return total;
    }

public:
    CUtimer()
    {
        Restart();
    }

    void Restart(void)
    {
        gettimeofday(&m_start_time, 0);
    }

    unsigned long Elapsed(void)
    {
        return utime_count_timeval(&m_start_time) / 1000;
    }

};
#endif
#endif



void CNetManager_OnRecv(SOCKET sock, const int8* pbData, int32 nLen, const addr_in* lpAddr, CSockBase* user);

typedef struct structMEMBLOCK
{
    int8* m_pbBlock;
    int32 m_nSize;
    int32 m_nUsed;
    int32 m_bFirst;
    structMEMBLOCK* m_pNext;
} MEMBLOCK, *PMEMBLOCK;

#define CMemPool CMemPool111


class CMemPool
{
public:
    static CMemPool& GetInstance();
public:
    virtual ~CMemPool();
public:
    void CreateMemPool(uint32 nPoolSize, uint32 nUnitSize);
    int8* GetMem(uint32 nSize, BOOL bCreate = true);
    void FreeMem(int8* pMem);
    void FreeAllMem();
private:
    PMEMBLOCK FindSuitbleBlock(uint32 nSize);
    PMEMBLOCK SkipBlock(PMEMBLOCK pCur);
    void AssignMemToBlock(PMEMBLOCK pFirst, uint32 nChunks, int8* pRawMem);
    int32 CalcueMemBlock(uint32 size);
private:
    CMemPool();
private:
    PMEMBLOCK m_pHead;
    PMEMBLOCK m_pCur;
    PMEMBLOCK m_pLast;
    int8* m_pbMemPool;
    uint32 m_nUnitsize;
    uint32 m_nPoolSize;
private:
    CriticalSection m_syn;
};

void CNetManager_OnListen(SOCKET sock, const addr_in* Addr, CSockBase* user);

#include "sock/sock.h"
#include "sock/ThreadPool.h"
#include "sock/SockHandle.h"
#include "sock/Socket.h"

int CBase64Convert_enBase64(const char* inbuf, size_t inbufLen, char* outStr, int maxOutLen);

#endif //  _CCC_H_