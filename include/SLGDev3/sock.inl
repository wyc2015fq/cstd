#ifndef _SOCK_INL_
#define ANYIP(af) (AF_INET6==(af)) ? "::0" : "0.0.0.0"
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
#ifdef _WIN32
#define SOCK_LENTH int
#define T_VOID void
#define T_HANDLE HANDLE
#define SLEEP(Milliseconds) {Sleep(Milliseconds);}
#define LASTERROR WSAGetLastError()
#define MILLISEC 1
#define MEMRESERVER_BLOCKS 10 //内存块大小
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
static int sock_init()
{
    static WSADATA wsaData = {0};
    if (0 == wsaData.wVersion)
    {
        int RetVal = 0;
        if ((RetVal = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
        {
            fprintf(stderr, "WSAStartup failed with error %d\n", RetVal);
            WSACleanup();
            return -1;
        }
    }
    return 0;
}
SOCKET my_socket(int af, int type, int protocol)
{
    sock_init();
    return socket(af, type, protocol);
}
char* GetErrorMsg1(int dw)
{
    // Retrieve the system error message for the last-error code
    static char buf[256];
    LPTSTR lpMsgBuf = NULL;
    if (0 == dw)
    {
        dw = GetLastError();
    }
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                  NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, NULL);
    _snprintf(buf, 256, "err: %d %s", dw, lpMsgBuf);
    return buf;
}
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
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/wait.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#define WSACleanup() (0)
#define WSAGetLastError() (0)
//#define closesocket(x) close(x)
#ifndef NI_MAXHOST
#define NI_MAXHOST   1024
#endif // NI_MAXHOST
typedef int SOCKET;
typedef struct sockaddr_storage SOCKADDR_STORAGE;
#define _snprintf snprintf
#define sock_init()
#define closesocket(x)  close(x)
#endif
#include "inet_ntop.inl"
static int net_addr(const char* ip, int port, void* addr)
{
    struct addrinfo hints, *res, *res0;
    char portStr[32] = {0};
    int ret;
    sock_init();
    _snprintf(portStr, 32, "%d", port);
    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags    = AI_PASSIVE;
    ret = getaddrinfo(ip, portStr, &hints, &res0);
    if (ret != 0)
    {
        DEBUG_INFO("getaddrinfo(ret=%d) err: %s", ret, gai_strerror(ret));
        return 0;
    }
    if (1)
    {
        int i = 0;
        char buf[256];
        DEBUG_INFO("getaddrinfo(ret=%d) ret: %s", ret, gai_strerror(ret));
        for (res = res0; res; res = res->ai_next)
        {
            net_ntop(res->ai_addr, buf, 256);
            DEBUG_INFO("addr=%s flags=%d family=%d type=%d protocol=%d len=%d", buf,
                       res->ai_flags, res->ai_family,
                       res->ai_socktype, res->ai_protocol,
                       res->ai_addrlen);
            ++i;
        }
    }
    for (res = res0; res; res = res->ai_next)
    {
        if (res->ai_family == AF_INET)
        {
            memcpy(addr, res->ai_addr, sizeof(struct sockaddr_in));
            return sizeof(sockaddr_in);
        }
        else if (res->ai_family == AF_INET6)
        {
            memcpy(addr, res->ai_addr, sizeof(struct sockaddr_in6));
            return sizeof(sockaddr_in6);
        }
    }
    return 0;
}
char* getsocknamestr(SOCKET s)
{
    sockaddr_in addrRemote;
    socklen_t nAddrLen = sizeof(addrRemote);
    getsockname(s, (sockaddr*) &addrRemote, &nAddrLen);
    return inet_ntoa(addrRemote.sin_addr);
}
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
#define WRITE_TIME_OUT(sock, time)  \
  fd_set fwds;\
  FD_ZERO(&fwds);\
  FD_SET(sock, &fwds);\
  timeval wtval;\
  wtval.tv_sec = 0;\
  wtval.tv_usec = (time*1000);\
  BOOL bTimeOut = ::select(sock+1, NULL, &fwds, NULL, &wtval) <= 0;
#define READ_TIME_OUT(sock, time)\
  fd_set frds;\
  FD_ZERO(&frds);\
  FD_SET(sock, &frds);\
  timeval rtval;\
  rtval.tv_sec = 0;\
  rtval.tv_usec = (time*1000);\
  BOOL bTimeOut = ::select(sock+1, &frds, NULL, NULL, &rtval) <= 0;
#define WRITE_ARRAY_TIME_OUT(sockArray, sizeArray, time)  \
  fd_set fwds;\
  FD_ZERO(&fwds);\
  SOCKET sock = 0;\
  int32 i = 0;\
  for (; i < sizeArray; ++i) \
  {\
    FD_SET(sockArray[i], &frds);\
    sock = sockArray[i] > sock ? sockArray[i] : sock;\
  }\
  timeval wtval;\
  wtval.tv_sec = 0;\
  wtval.tv_usec = (time*1000);\
  BOOL bTimeOut = ::select(sock+1, NULL, &fwds, NULL, &wtval) <= 0;
#define READ_ARRAY_TIME_OUT(sockArray, sizeArray, time)\
  fd_set frds;\
  FD_ZERO(&frds);\
  SOCKET sock = 0;\
  int32 i = 0;\
  for (; i < sizeArray; ++i) \
  {\
    FD_SET(sockArray[i], &frds);\
    if( sockArray[i] > sock ) sock = sockArray[i];\
  }\
  timeval rtval;\
  rtval.tv_sec = 0;\
  rtval.tv_usec = (time*1000);\
  BOOL bTimeOut = ::select(sock+1, &frds, NULL, NULL, &rtval) <= 0;
#define MACK_ADDR(af, ip, port)\
  int32 err;\
  struct addrinfo hints, *res0;\
  char portstr[32] = {0};\
  _snprintf(portstr, 32, "%d", port);\
  memset(&hints, 0, sizeof(hints));\
  hints.ai_family = af;\
  hints.ai_socktype = SOCK_STREAM;\
  hints.ai_flags = AI_PASSIVE; \
  err = getaddrinfo(ip, portstr, &hints, &res0);\
  if (NULL==res0) {    return FALSE;  }
void sock_close(SOCKET sock)
{
    closesocket(sock);
}
SOCKET sock_create(int af, int32 type)
{
    SOCKET sock = ::socket(af, type, 0);
    int flag = 1, rcvbuf = 64 * 1024;
    //printf("sock_create=%d\n", sock);
    ::setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(flag));
    setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (const int8*)&rcvbuf, sizeof(int32));
    return sock;
}
SOCKET sock_accept(SOCKET sock, addr_in* addr)
{
    SOCK_LENTH len = sizeof(addr_in);
    SOCKET sock_new = ::accept(sock, (struct sockaddr*)addr, &len);
    return sock_new;
}
BOOL sock_listen(SOCKET sock, int32 cns)
{
    return ::listen(sock, cns) == SOCKET_ERROR ? FALSE : TRUE;
}
BOOL sock_bind(SOCKET sock, const addr_in* addr)
{
    int32 on = 1;
    ::setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const int8*)&on, sizeof(on));
    int ret = bind(sock, (const sockaddr*)addr, sizeof(addr_in));
    //printf("%s", strerror(GetLastError()));
    return ret == SOCKET_ERROR ? FALSE : TRUE;
}
BOOL SLGDEV_CALL sock_connect(SOCKET sock, const addr_in* addr)
{
    // sock_ioctl (sock, FIONBIO , true ); //此设置导致SSL握手失败
    connect(sock, (const sockaddr*)addr, sizeof(addr_in));
    WRITE_TIME_OUT(sock, (3 * 1000));
    return !bTimeOut;
}
int32 sock_send(SOCKET sock, const void* data, int32 datalen, const addr_in* addr)
{
    int ret = ::sendto(sock, (const char*)data, (SOCK_LENTH)datalen, 0, (sockaddr*)addr, sizeof(addr_in));
    //printf("\nsock_send %d\n", ret);
    return ret;
}
int32 sock_recv(SOCKET sock, int8* data, int32 len, int32 flag)
{
    int ret = ::recv(sock, data, len, flag);
    //printf("\nsock_recv %d\n", ret);
    return ret;
}
int32 sock_recvfrom(SOCKET sock, int8* data, int32 len, addr_in* addr, int32 flags/* = 0*/)
{
    SOCK_LENTH size = sizeof(addr_in);
    len = ::recvfrom(sock, data, len, flags, (sockaddr*)addr, &size) ;
    return len;
}
BOOL sock_addr(SOCKET sock, addr_in* addr)
{
    SOCK_LENTH nLen = sizeof(addr_in);
    if (::getpeername(sock, (sockaddr*)addr, &nLen) != SOCKET_ERROR)
    {
        return TRUE;
    }
    return FALSE;
}
BOOL sock_set_recvbuf_size(SOCKET sock, int32 size)
{
    return ::setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (const int8*)&size, sizeof(int32)) ? FALSE : TRUE ;
}
BOOL sock_set_sendbuf_size(SOCKET sock, int32 size)
{
    return ::setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (const int8*)&size, sizeof(int32)) ? FALSE : TRUE ;
}
BOOL sock_ioctl(SOCKET sock, int32 cmd, int32 enable)
{
    return ioctlsocket(sock, cmd , (ulong*)&enable) == SOCKET_ERROR ? FALSE : TRUE;
}
int32 sock_type(SOCKET sock)
{
    int32 nType = SOCKET_ERROR ;
    SOCK_LENTH nLen = sizeof(nType);
    getsockopt(sock, SOL_SOCKET, SO_TYPE, (char*)&nType, &nLen) ;
    return nType;
}
BOOL sock_name(SOCKET sock, addr_in* addr)
{
    SOCK_LENTH nLen = sizeof(addr_in);
    if (SOCKET_ERROR == getsockname(sock, (sockaddr*)addr, &nLen))
    {
        return FALSE;
    }
    return TRUE;
}
BOOL sock_readamount(SOCKET sock, int32& amount)
{
    return ioctlsocket(sock, FIONREAD, (ulong*)&amount) == SOCKET_ERROR ? FALSE : TRUE;
}
int32 sock_error()
{
    return LASTERROR;
}
int cmp_cnt_bytes(const void* a, const void* b, int n)
{
    const unsigned char* ua = (const unsigned char*)a;
    const unsigned char* ub = (const unsigned char*)b;
    int i, d = 0;
    for (i = 0; i < n; ++i)
    {
        d += ua[i] != ub[i];
    }
    return d;
}
BOOL sock_getlocalip(const addr_in* addr, int port, addr_in* addr_out)
{
    char hostname[255] = {0}; //主机名
    //const char* addr;
    int ilRc;
    int d = 10000000, ret = 0;
    if (AF_INET == addr->family)
    {
        const sockaddr_in* addr1 = (sockaddr_in*)addr;
        sockaddr_in* addr2 = (sockaddr_in*)addr_out;
#ifdef WIN32
        PHOSTENT hostinfo;
        gethostname(hostname, sizeof(hostname));
        if ((hostinfo = gethostbyname(hostname)) == NULL)   //获得本地ipv4地址
        {
            fprintf(stderr, "gethostbyname Error:%d\n", GetLastError());
            return 1;
        }
        while (*(hostinfo->h_addr_list) != NULL)   //输出ipv4地址
        {
            struct in_addr addr3 = *(struct in_addr*) *hostinfo->h_addr_list;
            int dd = cmp_cnt_bytes(&addr1->sin_addr, &addr3, 4);
            if (dd < d)
            {
                d = dd;
                addr2->sin_family = AF_INET;
                addr2->sin_port = htons(port);
                addr2->sin_addr = addr3;
                ret = sizeof(sockaddr_in);
            }
            hostinfo->h_addr_list++;
        }
#else
        {
            char hname[128];
            struct hostent* hent;
            int i;
            gethostname(hname, sizeof(hname));
            //hent = gethostent();
            hent = gethostbyname(hname);
            DEBUG_INFO("hostname: %s/naddress list: ", hent->h_name);
            for (i = 0; hent->h_addr_list[i]; i++)
            {
                struct in_addr addr3 = *(struct in_addr*)(hent->h_addr_list[i]);
                int dd = cmp_cnt_bytes(&addr1->sin_addr, &addr3, 4);
                if (dd < d)
                {
                    d = dd;
                    addr2->sin_family = AF_INET;
                    addr2->sin_port = htons(port);
                    addr2->sin_addr = addr3;
                    ret = sizeof(sockaddr_in);
                }
            }
        }
#endif
    }
    else if (AF_INET6 == addr->family)
    {
        struct addrinfo* ailist, *aip;
        struct addrinfo hint;
        struct sockaddr_in6* addr1 = (struct sockaddr_in6*)addr;
        sockaddr_in6* addr2 = (sockaddr_in6*)addr_out;
        char portstr[32]; //端口号
        hint.ai_family = AF_INET6; // hint 的限定设置
        hint.ai_socktype = SOCK_STREAM; // 这里可是设置 socket type 比如 SOCK——DGRAM
        hint.ai_flags = AI_PASSIVE; // flags 的标志很多 。常用的有AI_CANONNAME;
        hint.ai_protocol = 0; // 设置协议 一般为0，默认
        hint.ai_addrlen = 0; // 下面不可以设置，为0，或者为NULL
        hint.ai_canonname = NULL;
        hint.ai_addr = NULL;
        hint.ai_next = NULL;
        _snprintf(portstr, 32, "%d", port);
        ilRc = getaddrinfo(hostname, portstr, &hint, &ailist); //通过主机名获得地址信息
        if (ilRc < 0)
        {
            char str_error[100];
            strcpy(str_error, (char*)gai_strerror(GetLastError()));
            DEBUG_INFO("str_error = %s", str_error);
            return 0;
        }
        if (ailist == NULL)
        {
            DEBUG_INFO("%s", "sorry not find the IP address,please try again \n");
        }
        for (aip = ailist; aip != NULL; aip = aip->ai_next)   // 显示获取的信息
        {
            assert(aip->ai_family == AF_INET6);
            struct sockaddr_in6* addr3 = (struct sockaddr_in6*)aip->ai_addr; // 为什么是for 循环 ，先向下看
            int dd = cmp_cnt_bytes(&addr1->sin6_addr, &addr3->sin6_addr, sizeof(in6_addr));
            if (dd < d)
            {
                d = dd;
                *addr2 = *addr3;
                ret = sizeof(sockaddr_in6);
            }
        }
    }
    return ret;
}
uint16 sock_checksum(uint16* buffer, int32 size)
{
    uint32 cksum = 0;
    while (size > 1)
    {
        cksum += *buffer++;
        size -= sizeof(uint16);
    }
    if (size)
    {
        cksum += *(uint8*)buffer;
    }
    cksum = (cksum >> 16) + (cksum & 0xffff);
    cksum += (cksum >> 16);
    return (uint16)(~cksum);
}
/////////////////////////////////////////////////////////////////////////////////////////
#define REALLOC_PUSH1(ptr, poldsz, newsz, retoff)  realloc_push1(&ptr, poldsz, newsz, sizeof(*ptr), retoff)
void* realloc_push1(void* ptr, int* poldsz, int newsz, int elemsz, int retoff)
{
    int oldsz = poldsz ? *poldsz : 0;
    void** pptr = (void**)ptr;
    if (newsz > oldsz)
    {
        *pptr = realloc(*pptr, newsz * elemsz);
        memset((char*)(*pptr) + oldsz * elemsz, 0, (newsz - oldsz)*elemsz);
    }
    if (poldsz)
    {
        *poldsz = newsz;
    }
    return (char*)(*pptr) + (retoff * elemsz);
}
int loadfile(const char* fn, char** buf)
{
    FILE* fp = fopen(fn, "rb");
    int size = 0;
    if (NULL == fp)
    {
        return NULL;
    }
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    if (buf)
    {
        fseek(fp, 0, SEEK_SET);
        *buf = (char*)realloc(*buf, size + 10);
        size = fread(*buf, 1, size, fp);
        (*buf)[size] = 0;
    }
    fclose(fp);
    return size;
}
typedef struct
{
#ifdef _WIN32
    HANDLE m_hEvent;
#else
    pthread_cond_t    m_hEvent;
    pthread_mutex_t   m_sect;
    pthread_mutexattr_t m_attr;
#endif
} CEvent;
typedef struct
{
#ifdef _WIN32
    CRITICAL_SECTION m_sect;
#else
    pthread_mutex_t   m_sect;
    pthread_mutexattr_t m_attr;
#endif
#ifdef _DEBUG
    long m_Count;
#endif
} CriticalSection;
void CriticalSection_Init(CriticalSection* s)
{
#ifdef _DEBUG
    s->m_Count = 0;
#endif
#ifdef _WIN32
    ::InitializeCriticalSection(&s->m_sect);
#else
    pthread_mutexattr_init(&s->m_attr);
    pthread_mutexattr_settype(&s->m_attr, PTHREAD_MUTEX_RECURSIVE_NP);
    pthread_mutex_init(&s->m_sect, &s->m_attr);
#endif
}
void CriticalSection_Free(CriticalSection* s)
{
#ifdef _DEBUG
    if (s->m_Count != 0)
    {
        MessageBoxA(NULL , "锁计数器不为" , "warning!", MB_OK);
    }
#endif
#ifdef _WIN32
    DeleteCriticalSection(&s->m_sect);
#else
    pthread_mutexattr_destroy(&s->m_attr);
    pthread_mutex_destroy(&s->m_sect);
#endif
}
void CriticalSection_Lock(CriticalSection* s)
{
#ifdef _DEBUG
    InterlockedIncrement(&s->m_Count);
#endif
#ifdef _WIN32
    ::EnterCriticalSection(&s->m_sect);
#else
    pthread_mutex_lock(&s->m_sect);
#endif
}
void CriticalSection_Unlock(CriticalSection* s)
{
#ifdef _DEBUG
    InterlockedDecrement(&s->m_Count);
#endif
#ifdef _WIN32
    ::LeaveCriticalSection(&s->m_sect);
#else
    pthread_mutex_unlock(&s->m_sect);
#endif
}
void CEvent_Init(CEvent* s)
{
#ifdef _WIN32
    s->m_hEvent = CreateEvent(NULL, 0, 0, NULL);
#else
    pthread_cond_init(&s->m_hEvent, 0);
    pthread_mutexattr_init(&s->m_attr);
    pthread_mutexattr_settype(&s->m_attr, PTHREAD_MUTEX_TIMED_NP);
    pthread_mutex_init(&s->m_sect, &s->m_attr);
#endif
}
void CEvent_Free(CEvent* s)
{
#ifdef _WIN32
    CloseHandle(s->m_hEvent);
#else
    pthread_cond_destroy(&s->m_hEvent);
    pthread_mutexattr_destroy(&s->m_attr);
    pthread_mutex_destroy(&s->m_sect);
#endif
    memset(s, 0, sizeof(*s));
}
void CEvent_Wait(CEvent* s, unsigned int dwMilliseconds)
{
#ifdef _WIN32
    WaitForSingleObject(s->m_hEvent, dwMilliseconds);
#else
    pthread_mutex_lock(&s->m_sect);
    if (dwMilliseconds == INFINITE)
    {
        pthread_cond_wait(&s->m_hEvent, &s->m_sect);
    }
    else
    {
        timespec timeout;
        timeout.tv_sec = 0;
        timeout. tv_nsec = dwMilliseconds * 10000000;
        pthread_cond_timedwait(&s->m_hEvent, &s->m_sect, &timeout);
    }
    pthread_mutex_unlock(&s->m_sect);
#endif
}
void CEvent_Signal(CEvent* s)
{
#ifdef _WIN32
    SetEvent(s->m_hEvent);
#else
    pthread_mutex_lock(&s->m_sect);
    pthread_cond_signal(&s->m_hEvent);
    pthread_mutex_unlock(&s->m_sect);
#endif
}
T_HANDLE StartThread(thread_cb cb, void* param)
{
    T_HANDLE m_hTread = 0;
#ifdef _WIN32
    m_hTread = (HANDLE)CreateThread(0, 0, (thread_cb)cb, param, 0, 0);
#else
    pthread_create(&m_hTread, NULL, (thread_cb)cb, param) ;
#endif
    return m_hTread;
}
void StopThread(T_HANDLE m_hTread)
{
#ifdef _WIN32
    if (WaitForSingleObject(m_hTread, 3000) == WAIT_TIMEOUT)
    {
        TerminateThread(m_hTread, 100);
    }
#else
    pthread_join(m_hTread, NULL);
#endif
}
typedef struct CThreadPool_ CThreadPool;
typedef struct CWorkerThread_ CWorkerThread;
typedef struct CJob_ CJob;
struct CWorkerThread_
{
    T_HANDLE m_hTread;
    CThreadPool* m_pThreadPool;
    int m_bRunning;
};
struct CThreadPool_
{
    CriticalSection m_synJobList;
    CEvent          m_hJobEvent;
    CJob*           m_JobList;
    int             m_bInit;
    int  m_nWorkThred;
    CWorkerThread*  m_WorkThred;
};
struct CJob_
{
    void* x;
    CJob* next;
    CJob* prev;
    void (*Run)(CJob* s);
};
#define LIST_PUSHBACK(T, list, item) if (list) { T* tmp = list; while(tmp->next) { tmp = tmp->next; } tmp->next = item; } else { list = item; }
#define LIST_POPFRONT(T, list, item) { item = list; if (list) { list = list->next; } }
#define LIST_ADDFRONT(list, item) if (list) { item->prev = NULL; item->next = list; list->prev = item; list = item; } else { item->prev = item->next = NULL; list = item; }
#define LIST_DEL(list, item) if (list==item) { list->prev = NULL; list = item->next; } else { if (item->prev) { item->prev->next = item->next; } if (item->next) { item->next->prev = item->prev; } }
void SetJob(CThreadPool* s, CJob* pJob)
{
    if (!s->m_bInit)
    {
        return;
    }
    CriticalSection_Lock(&s->m_synJobList);
    LIST_PUSHBACK(CJob, s->m_JobList, pJob);
    CEvent_Signal(&s->m_hJobEvent);
    CriticalSection_Unlock(&s->m_synJobList);
}
CJob* GetJob(CThreadPool* s)
{
    CJob* pJob = NULL;
    if (!s->m_bInit)
    {
        return NULL;
    }
    {
        CriticalSection_Lock(&s->m_synJobList);
        LIST_POPFRONT(CJob, s->m_JobList, pJob);
        CriticalSection_Unlock(&s->m_synJobList);
    }
    if (pJob == NULL && s->m_bInit)
    {
        CEvent_Wait(&s->m_hJobEvent, INFINITE);
    }
    return pJob;
}
#ifdef _WIN32
#define THREADFUNDEF(NAME, pParam)  unsigned long __stdcall NAME(void* pParam)
#else
#define THREADFUNDEF(NAME, pParam)  void* NAME(void* pParam)
#endif
THREADFUNDEF(CWorkerThread_Run, pParam)
{
    CWorkerThread* s = (CWorkerThread*)pParam;
    while (s->m_bRunning)
    {
        CJob* pJob = GetJob(s->m_pThreadPool);
        if (pJob)
        {
            pJob->Run(pJob);
            free(pJob);
        }
    }
    return 0;
}
void CThreadPool_Init(CThreadPool* s, int nIniNum)
{
    int i;
    s->m_bInit = (1);
    REALLOC_PUSH1(s->m_WorkThred, &s->m_nWorkThred, nIniNum, 0);
    for (i = 0; i < nIniNum; ++i)
    {
        s->m_WorkThred[i].m_bRunning = 1;
        s->m_WorkThred[i].m_pThreadPool = s;
        s->m_WorkThred[i].m_hTread = StartThread(CWorkerThread_Run, s->m_WorkThred + i);
    }
    CEvent_Init(&s->m_hJobEvent);
    CriticalSection_Init(&s->m_synJobList);
    return ;
}
void CThreadPool_Free(CThreadPool* s)
{
    int i;
    s->m_bInit = 0;
    for (i = 0; i < s->m_nWorkThred; ++i)
    {
        s->m_WorkThred[i].m_bRunning = 0;
        CEvent_Signal(&s->m_hJobEvent);
    }
    CriticalSection_Lock(&s->m_synJobList);
    {
        CJob* job = s->m_JobList;
        while (job)
        {
            CJob* next = job->next;
            free(job);
            job = next;
        }
    }
    CriticalSection_Unlock(&s->m_synJobList);
    free(s->m_WorkThred);
    CEvent_Free(&s->m_hJobEvent);
    CriticalSection_Free(&s->m_synJobList);
    memset(s, 0, sizeof(*s));
}
/////////////////////////////////////////////////////////////////////////////////////////
void CBase64Convert__enBase64Help(unsigned char chasc[3], unsigned char chuue[4])
{
    int i, k = 2;
    unsigned char t = 0;
    for (i = 0; i < 3; i++)
    {
        *(chuue + i) = *(chasc + i) >> k;
        *(chuue + i) |= t;
        t = *(chasc + i) << (8 - k);
        t >>= 2;
        k += 2;
    }
    *(chuue + 3) = *(chasc + 2) & 63;
    for (i = 0; i < 4; i++)
    {
        if ((*(chuue + i) <= 128) && (*(chuue + i) <= 25))
        {
            *(chuue + i) += 65; // 'A'-'Z'
        }
        else if ((*(chuue + i) >= 26) && (*(chuue + i) <= 51))
        {
            *(chuue + i) += 71; // 'a'-'z'
        }
        else if ((*(chuue + i) >= 52) && (*(chuue + i) <= 61))
        {
            *(chuue + i) -= 4; // 0-9
        }
        else if (*(chuue + i) == 62)
        {
            *(chuue + i) = 43; // +
        }
        else if (*(chuue + i) == 63)
        {
            *(chuue + i) = 47; // /
        }
    }
}
void CBase64Convert__deBase64Help(unsigned char chuue[4], unsigned char chasc[3])
{
    int i, k = 2;
    unsigned char t = 0;
    for (i = 0; i < 4; i++)
    {
        if ((*(chuue + i) >= 65) && (*(chuue + i) <= 90))
        {
            *(chuue + i) -= 65; // 'A'-'Z' -> 0-25
        }
        else if ((*(chuue + i) >= 97) && (*(chuue + i) <= 122))
        {
            *(chuue + i) -= 71; // 'a'-'z' -> 26-51
        }
        else if ((*(chuue + i) >= 48) && (*(chuue + i) <= 57))
        {
            *(chuue + i) += 4; // '0'-'9' -> 52-61
        }
        else if (*(chuue + i) == 43)
        {
            *(chuue + i) = 62; // + -> 62
        }
        else if (*(chuue + i) == 47)
        {
            *(chuue + i) = 63; // / -> 63
        }
        else if (*(chuue + i) == 61)
        {
            *(chuue + i) = 0; // = -> 0 Note: 'A'和'='都对应了0
        }
    }
    for (i = 0; i < 3; i++)
    {
        *(chasc + i) = *(chuue + i) << k;
        k += 2;
        t = *(chuue + i + 1) >> (8 - k);
        *(chasc + i) |= t;
    }
}
int CBase64Convert_enBase64(const char* inbuf, size_t inbufLen, char* outStr, int maxOutLen)
{
    unsigned char* outStr1 = (unsigned char*)outStr;
    unsigned char* outStr2 = (unsigned char*)outStr + maxOutLen;
    unsigned char* out = outStr1;
    unsigned char in[8];
    size_t blocks = inbufLen / 3;
    for (size_t i = 0; i < blocks; i++)
    {
        in[0] = inbuf[i * 3];
        in[1] = inbuf[i * 3 + 1];
        in[2] = inbuf[i * 3 + 2];
        CBase64Convert__enBase64Help(in, out);
        out += 4;
    }
    if (inbufLen % 3 == 1)
    {
        in[0] = inbuf[inbufLen - 1];
        in[1] = 0;
        in[2] = 0;
        CBase64Convert__enBase64Help(in, out);
        out += 2;
        *out++ = '=';
        *out++ = '=';
    }
    else if (inbufLen % 3 == 2)
    {
        in[0] = inbuf[inbufLen - 2];
        in[1] = inbuf[inbufLen - 1];
        in[2] = 0;
        CBase64Convert__enBase64Help(in, out);
        out += 3;
        *out++ = '=';
    }
    *out = 0;
    return out - outStr1;
}
int CBase64Convert_deBase64_(const char* src, int src_size, char* outbuf)
{
    // Break when the incoming base64 coding is wrong
    if ((src_size % 4) != 0)
    {
        return 0;
    }
    unsigned char in[4];
    unsigned char out[3];
    size_t blocks = src_size / 4;
    for (size_t i = 0; i < blocks; i++)
    {
        in[0] = src[i * 4];
        in[1] = src[i * 4 + 1];
        in[2] = src[i * 4 + 2];
        in[3] = src[i * 4 + 3];
        CBase64Convert__deBase64Help(in, out);
        outbuf[i * 3] = out[0];
        outbuf[i * 3 + 1] = out[1];
        outbuf[i * 3 + 2] = out[2];
    }
    int length = src_size / 4 * 3;
    if (src[src_size - 1] == '=')
    {
        length--;
        if (src[src_size - 2] == '=')
        {
            length--;
        }
    }
    return length;
}
int CBase64Convert_deBase64(const char* pSrc, int len, char* outbuf)
{
    // Break when the incoming base64 coding is wrong
    if (pSrc == NULL || len == 0 || (len % 4) != 0)
    {
        return 0;
    }
    DEBUG_INFO(("deBase64 len = %d") , len);
    unsigned char inData[4];
    size_t blocks = len / 4;
    //if (blocks*3>=maxoutlen) {      return 0;    }
    for (size_t i = 0; i < blocks; i++)
    {
        memcpy(inData , pSrc + i * 4 , 4);
        CBase64Convert__deBase64Help(inData , (unsigned char*)outbuf + i * 3);
    }
    int length = len / 4 * 3;
    if (pSrc[len - 1] == '=')
    {
        length--;
        if (pSrc[len - 2] == '=')
        {
            length--;
        }
    }
    return length;
}
/////////////////////////////////////////////////////////////////////////////////////////
#define MAX_SEQ 0x7fffffff
typedef int (SLGDEV_CALL* RecvCallBack)(SOCKET sock, const addr_in* Addr, void* user, int socktype);
typedef struct SELECT_DATA_ SELECT_DATA;
struct SELECT_DATA_
{
    SOCKET sock;
    void* lpContext;
    void* user;
    int bListen;
    RecvCallBack m_lpfnRecv;
    SELECT_DATA* next;
    SELECT_DATA* prev;
};
typedef struct
{
    CThreadPool* m_pThreadPool;
    CriticalSection m_synSOCKMap;
    T_HANDLE      m_hWorkTread; //工作线程句柄
    int32         m_bWorkRuning;
    SELECT_DATA* data;
} CSelectRecv;
SELECT_DATA* CSelect_StartRecv(CSelectRecv* s, SELECT_DATA* data, SOCKET sock, int bListen, RecvCallBack m_lpfnRecv, void* user)
{
    data->m_lpfnRecv = m_lpfnRecv;
    data->bListen = bListen;
    data->user = user;
    data->sock = sock;
    LIST_ADDFRONT(s->data, data);
    return data;
}
BOOL CSelect_StopRecv(CSelectRecv* s, SELECT_DATA* data)
{
    LIST_DEL(s->data, data);
    return TRUE;
}
THREADFUNDEF(CSelect_process, pParam)
{
    CSelectRecv* s = (CSelectRecv*)pParam;
    SELECT_DATA* data;
    while (s->m_bWorkRuning)
    {
        SOCKET Array[FD_SETSIZE] = {0};
        int32 sType[FD_SETSIZE] = {1};
        void* users[FD_SETSIZE] = {0};
        int32 nSize = 0;
        SELECT_DATA* datas[FD_SETSIZE] = {0};
        {
            CriticalSection_Lock(&s->m_synSOCKMap);
            for (data = s->data; data; data = data->next)
            {
                datas[nSize] = data;
                Array[nSize] = data->sock;
                sType[nSize] = data->bListen;
                users[nSize] = data->user;
                nSize++;
            }
            CriticalSection_Unlock(&s->m_synSOCKMap);
        }
        if (nSize <= 0)
        {
            SLEEP(MILLISEC * 300);
            continue;
        }
        READ_ARRAY_TIME_OUT(Array, nSize, 300);
        if (bTimeOut)
        {
            SLEEP(MILLISEC * 300);
            continue;
        }
        for (i = 0; i < nSize; ++i)
        {
            if (FD_ISSET(Array[i], &frds))
            {
                SOCKET sock = Array[i];
                void* user = users[i];
                data = datas[i];
                if (sType[i])
                {
                    SOCKET sock = Array[i];
                    addr_in Addr[1] = {0};
                    SOCKET accepted = sock_accept(sock, Addr);
                    if (INVALID_SOCKET != accepted)
                    {
                        data->m_lpfnRecv(accepted, Addr, data->user, 0);
                    }
                }
                else
                {
                    BOOL bClosed = false;
                    CriticalSection_Lock(&s->m_synSOCKMap);
                    {
                        int t = sock_type(sock);
                        if (t == SOCK_STREAM)
                        {
                            if (!data->m_lpfnRecv(sock, NULL, data->user, t))
                            {
                                sock_close(sock);
                                bClosed = true;
                            }
                        }
                        else
                        {
                            data->m_lpfnRecv(sock, NULL, data->user, t);
                        }
                    }
                    CriticalSection_Unlock(&s->m_synSOCKMap);
                    if (bClosed)
                    {
                        //CSelect_StopRecv(s, data);
                    }
                }
            }
        }
    }
    return 0;
}
BOOL CSelect_Init(CSelectRecv* s, BOOL flag)
{
    BOOL ret = false;
    if (flag)
    {
        if (!s->m_bWorkRuning)
        {
            CriticalSection_Init(&s->m_synSOCKMap);
            s->m_bWorkRuning = true;
#ifdef _WIN32
            //m_hWorkTread = (HANDLE)_beginthread(CSelectProcessor::process, 0, this);
            s->m_hWorkTread = (HANDLE)CreateThread(0, 0, CSelect_process, s, 0, 0);
            ret = (s->m_hWorkTread != NULL);
#else
            ret = (pthread_create(&s->m_hWorkTread, NULL, CSelect_process, s) == 0);
#endif
        }
    }
    else
    {
        if (s->m_bWorkRuning)
        {
            s->m_bWorkRuning = false;
#ifdef _WIN32
            if (WaitForSingleObject(s->m_hWorkTread, 1000 * TIME_OUT * 2) == WAIT_TIMEOUT)
            {
                TerminateThread(s->m_hWorkTread, 100);
            }
            s->m_hWorkTread = NULL;
            ret = true;
#else
            pthread_join(s->m_hWorkTread, NULL);
            s->m_hWorkTread = 0;
            ret = true;
#endif
            CriticalSection_Free(&s->m_synSOCKMap);
        }
    }
    return ret;
}
#endif // _SOCK_INL_

