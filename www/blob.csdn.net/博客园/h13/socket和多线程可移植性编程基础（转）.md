# socket和多线程可移植性编程基础（转） - h13 - 博客园
1 前言
2 Socket编程
2.1 Socket通信机制
2.2 socket通信示例图
2.3 Socket在不同平台上的实现
2.3.1 Socket在Windows平台中的实现
2.3.2 Socket在Linux/Unix平台中的实现
2.3.3 可移植的启动和结束调用代码
2.3.4 其它移植问题
3 多线程编程
3.1 线程与进程的不同
3.2 线程冲突与数据保护
3.3 Win32中的线程
3.3.1 线程同步
3.3.2 创建线程
3.4 Linux/Unix中的线程
3.5 可移植的线程代码
4 程序实例
前言
Socket编程特别是多线程编程是一个很大的课题，本文针对公司最近将要实现的下载版和网页版的CPR和CPE两个软件来讲解socket和多线程的可移植编程技术，所涉及的是其中较基础的部分，以尽量满足当前公司需要为准。
Socket编程
Socket通信机制通过socket可实现点对点或广播通信程序，两者之间的区别不大，编程时其程序流程所用代码几乎相同，不同的地方在于目标地址选择的不同。本教材所举实例为点对点的形式，即以客户/服务器形式来实现一个socket通信程序，为描述方便，我们对两点分别命名为Server和Client。Socket实现Server 与Client之间的通信的过程，非常象电信局的普通电话服务，为了更好的理解socket，以下就以电信局的电话服务作为类比来说明socket的通信机制：
    *      电信局提供电话服务类似我们这的Server，普通电话用户类似我们这的Client。
    *      首先电信局必须建立一个电话总机。这就如果我们必须在Server端建立一个Socket(套接字)，这一步通过调用socket()函数实现。
    *      电信局必须给电话总机分配一个号码，以便使用户要拨找该号码得到电话服务，同时接入该电信局的用户必须知道该总机的号码。同样，我也在Server端也要为这一套接字指定一port(端口)，并且要连接该Server的Client必须知道该端口。这一步通过调用bind()函数实现。
    *      接下来电信局必须使总机开通并使总机能够高效地监听用户拨号，如果电信局所提供服务的用户数太多，你会发现拨打电信局总机老是忙音，通常电信局内部会使该总机对应的电话号码连到好几个负责交换的处理中心，在一个处理中心忙于处理当前的某个用户时，新到用户可自动转到一下处理中心得到服务。同样我们的Server端也要使自己的套接口设置成监听状态，这是通用listen()函数实现的，listen()的第二个参数是等待队列数，就如同你可以指定电信局的建立几个负责交换的处理中心。
    *      用户知道了电信局的总机号后就可以进行拨打请求得到服务。在Winsock的世界里做为Client端是要先用socket()函数建立一个套接字，然后调connect()函数进行连接。当然和电话一样，如果等待队列数满了、与Server的线路不通或是Server没有提供此项服务时，连接就不会成功。
    *      电信局的总机接受了这用户拨打的电话后负责接通用户的线路，而总机本身则再回到等待的状态。Server也是一样，调用accept()函数进入监听处理过程，Server端的代码即在中处暂停，一旦Server端接到申请后系统会建立一个新的套接字来对此连接做服务，而原先的套接字则再回到监听等待的状态。
    *      当你电话挂完了，你就可以挂上电话，彼此间也就离线了。Client和Server间的套接字的关闭也是如此；这个关闭离线的动作，可由Client端或Server端任一方先关闭，这也与电话局的服务类似。
从以上情况可以看出在服务器端建立一个套接字，进入监听状态到接收到一个客户端的请求的过程如下：
socket()->bind()->listen()->accept()->recv()->send()
在客户端则要简单得多，其调用过程如下：
socket()->connect()->send()->recv()
socket通信示例图
下图显示的通信方式用在许多场合，比如HTTP协议中用的就是这种方式。FTP、TELNET等协议也与此大致相同，不同之外在于FTP、TELNET这类协议会多次重复send()和recv()的调用。
Socket在不同平台上的实现
Socket在Windows平台中的实现
Socket在Windows中的实现称为Winsock，核心文件是winsock.dll(或winsock32.dll)。这个动态链接库负责提供标准的socket调用API和其它几个特定的Windows平台专用API，另外它还实现了适用于Windows消息机制的同步socket通信机制。在Winsock中所有非标准的socket调用均以WSA三个字母开头命名。
由于Win32的各个平台均支持线程，我采用同步socket通信机制来编写Winsock程序不可取。一是多线程的系统中对同步通信有更好的解决办法，二是同步通信机制程序没有很好的兼容性。
Winsock中有两个比较重要的函数就是WSAStartup()和WSACleanup()，它们的作用在于对Winsock动态链接库进行一些初始化或清除工作。在一个进程中，没有调用WSAStartup()之前将无法正确调用任何其它标准的socket函数。
Socket在Linux/Unix平台中的实现
Socket最早是在Unix平台上实现的，socket调用已是当今Unix平台的标准系统调用。Linux的网络部分模块几乎是原封不动的从Unix中迁移过来的，其函数的调用方式基本与Unix相同。
在Linux/Unix平台中，如果你正在向一个套接字发信息，而远端计算机又关闭了该端口，将产生一个SIGPIPE信号，该信号的默认处理过程将中止当前进程，为使我们的socket程序能够健壮的运行，必须接管该信息的处理过程，一般情况使系统忽略该信号即可。
可移植的启动和结束调用代码
根据以上两点所述的socket中不同平台中的实现方式，可以编写出可移植的socket初始化和结束调用代码如下：
```
bool bsocket_init() //初始化socket 调用
{
#ifdef _WIN32
WSADATA wsad;
WORD wVersionReq;
wVersionReq=MAKEWORD(1,1); //清求不低于1.1版本的Winsock
if(WSAStartup(wVersionReq,&wsad))
return false;
return true;
#else
signal(SIGPIPE, SIG_IGN); //忽略SIGPIPE信号
return true;
#endif
}
void bsocket_cleanup() //结束socket 调用
{
#ifdef _WIN32
WSACleanup();
#else
signal(SIGPIPE, SIG_DFL); //恢复SIGPIPE信号
return;
#endif
}
```
其它移植问题
关闭socket
Windows中关闭socket的调用为closesocket(socket sock_in); Linux为close(socket sock_in);
Socket地址长度类型
Socket的许多调用中均用到socket地址长度做为参数，该参数在Windows中为int类型，在Linux中为socklen_t类型。
无效的socket
Windows中调用socket()和accept()时，如果返回的是无效的socket，则该值为INVALID_SOCKET，Linux中为-1。
调用错误返回
Windows中除上述的socket()和accept()以及几个少数的调用外，大部分调用在出错时返回值为SOCKET_ERROR，Linux中为-1。
多线程编程
线程与进程的不同
当把一个可执行程序装载在内存中后，不管你是你是准备它或是已经执行它或是被暂停执行，它都被称为一个进程。你可以称一个进程为一个可执行程序在内存中的实例。虽然一个进程可以复制自己，也可以调用其它进程或与其它进程通信，全基本上进程是由操作系统直接管理的资源。每一个进程均有一个与其它进程独立的操作系统运行环境，比如环境变量、当前目录等。
线程是一种可执行体，它表示对CPU运行时间的占有权，线程也是操作系统中的一种资源，它的创建、中止最终要由操作系统来实现。但线程每一个线程更多表现为由一个进程直接来管理。每一个线程均有一个与其它线程不同的硬件运行环境，比如各CPU中通用寄存器和状态寄存器的值。
一个进程的创建需要操作系统做大量工作，如设置环境变量、装裁可执行代码、分配进程资源等，进程间的切换也需做大量工作。而线程的创建的切换相对来说要简单得多，操作也能够以最快的速度来实现不同执行体之间的切换。
另一方面，由于所有线程共享一个进程空间，线程间的通信变得十分简单；而进程之间由于各自内存相互隔离，进程之间通信只能通过管道、发送系统消息或信号来实现。
线程冲突与数据保护
由于同一进程中的线程由操作系统并发执行。当一个进程中的许多线程要修改某些公用变量时就存在数据保护问题。解决这个问题的通常办法是向操作系统申请一个线程同步对象，操作系统只允许同时有一个少数个线程访问受保护的数据。
Win32中的线程
线程同步
Win32中可用于线程同步的对象有CRITICAL_SECTION, Event, Mutex, Semaphore和Waitable timer等。 其中CRITICAL_SECTION比较适于解决在单一进程多线程的冲突问题。
与CRITICAL_SECTION有关的几个函数为:
    *      InitializeCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
该函数初始化一个CRITICAL_SECTION。
    *      DeleteCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
该函数清除一个CRITICAL_SECTION。
    *      EnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
该函数使本线程取得CRITICAL_SECTION控制权，代码进入保护状态。
    *      LeaveCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
该函数使本线程放弃CRITICAL_SECTION控制权，代码退出保护状态。
创建线程
Win32中最常用的创建线程的函数为CreateThread()，该函数的格式为：
```
HANDLE CreateThread(
LPSECURITY_ATTRIBUTES lpThreadAttributes,
DWORD dwStackSize,
LPTHREAD_START_ROUTINE lpStartAddress,
LPVOID lpParameter,
DWORD dwCreationFlags,
LPDWORD lpThreadId);
```
    *      lpThreadAttributes: 输入参数。指定是否本次函数返回的线程句柄可以被子进程继承，可以为NULL。在CPR/CPE两个产品下载版和网页版的开发过程代码使用NULL应可满足需要。
    *      dwStackSize : 输入参数。指定指定的线程堆栈大小，可以为0，如果为0，使用默认堆栈大小。在CPR/CPE两个产品下载版和网页版的开发过程代码使用0应可满足需要。
    *      lpStartAddress : 输入参数。指向线程函数，该函数包含线程的执行代码。
    *      lpParameter : 输入参数。指定要传给线程的参数块内存地址。
    *      lpThreadId : 输出参数。该参数在调用结束后将包含被创建线程的ID。
Linux/Unix中的线程
当前各版本的Linux和Unix本身并不支持线程，但在Linux/Unix各版本均自带pthread线程包及它们的开发运行头文件和库文件。
与Win32类似，pthread包中用于线程同步的对象也有多种，在我们的开发应用中，使用pthread_mutex_t即可满足需求。与pthread_mutex_t相关的几个函数为：
    *
      pthread_mutex_init(pthread_mutex_t * p, pthread_mutexattr_t * pa);
该函数初始化一个pthread_mutex_t，其中pa参数可以设置pthread_mutex_t的属性。与Win32中多线程的机制稍有不同，在默认情况下，pthread中对同一线程的多次保护请求会造成互锁，以我们产品开发的情况来看，要修改pthread的属性方可满足实际需要，但pthread中设置多线程同步属性的方法的可移植性不高。对同一线程多次申请保护的问题可以记录线程ID的办法来解决。
    *      pthread_mutex_destroy(pthread_mutex_t * p);
清除一个已初始化的pthread_mutex_t。
    *      pthread_mutex_lock(pthread_mutex_t * p);
请求取得pthread_mutex_t控制权，进入代码保护。
    *      pthread_mutex_unlock(pthread_mutex_t * p);
放弃一个pthread_mutex_t控制权，退出代码保护。
可移植的线程代码
(参见程序实例)
程序实例
```
#include <stdio.h>
#ifdef _WIN32
#include <winsock.h>
#define socklen_t int
#else
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#define closesocket(_x) close(_x)
#endif
#ifdef _WIN32
#define THREAD_PROC WINAPI
#define THREAD_RETURN DWORD
#define THREAD_PARAM void *
#define THREAD_ID DWORD
#define CPO CRITICAL_SECTION
#define CPO_Init(_x) InitializeCriticalSection(&_x)
#define CPO_Dele(_x) DeleteCriticalSection(&_x)
#define CPO_Enter(_x) EnterCriticalSection(&_x)
#define CPO_Leave(_x) LeaveCriticalSection(&_x)
#else
struct CPO { pthread_mutex_t m; pthread_t t;};
#define THREAD_PROC
#define THREAD_RETURN void *
#define THREAD_PARAM void *
#define THREAD_ID pthread_t
#define CPO_Init(_x) { _x.t=0; pthread_mutex_init(&_x.m, NULL); }
#define CPO_Dele(_x) { pthread_mutex_destroy(&_x.m); }
#define CPO_Enter(_x) while(true) \
{ \
if(_x.t==0) \
{ \
pthread_mutex_lock(&_x.m); \
_x.t=pthread_self(); \
break;\
}\
if(pthread_self()==_x.t)\
break; \
pthread_mutex_lock(&_x.m); \
_x.t=pthread_self();\
break; \
}
#define CPO_Leave(_x) { pthread_mutex_unlock(&_x.m); _x.t=0; }
#endif
typedef THREAD_RETURN THREAD_PROC THREAD_FUNCTION(THREAD_PARAM thread_param);
#ifdef _WIN32
THREAD_ID bcreate_thread(THREAD_FUNCTION pfun, void * pparam)
{
THREAD_ID tid;
if(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)pfun, (LPVOID)pparam, 0, &tid)==NULL)
return (THREAD_ID)-1;
return tid;
}
#else
THREAD_ID bcreate_thread(THREAD_FUNCTION pfun, void * pparam)
{
THREAD_ID tid;
if(pthread_create(&tid, NULL, (void * (*)(void *))pfun, pparam)<1)
return (THREAD_ID)-1;
pthread_detach(tid);
return tid;
}
#endif
#ifndef SOCKET
#define SOCKET int
#endif
#ifndef SOCKET_ERROR
#define SOCKET_ERROR -1
#endif
#ifndef INVALID_SOCKET
#define INVALID_SOCKET -1
#endif
bool bsocket_init()
{
#ifdef _WIN32
WSADATA wsad;
WORD wVersionReq;
wVersionReq=MAKEWORD(1,1);
if(WSAStartup(wVersionReq,&wsad))
return false;
return true;
#else
signal(SIGPIPE, SIG_IGN);
return true;
#endif
}
void bsocket_cleanup()
{
#ifdef _WIN32
WSACleanup();
#else
signal(SIGPIPE, SIG_DFL);
return;
#endif
}
#define WEB_SERVER_PORT 90
#define WEB_SERVER_IP "127.0.0.1"
#define HTTP_HEAD "HTTP/1.0 200 OK\x0d\x0a""Content-type: text/html\x0d\x0a\x0d\x0a"
#define WELCOME_HTML "<html>\n<body>\n<B>Welcome to my Website!</B><hr>\n<center>g_hits: %d</center>\n</body>\n</html>\n"
CPO g_cpo;
int g_hits;
THREAD_RETURN THREAD_PROC do_accept(THREAD_PARAM param)
{
SOCKET sock=((SOCKET *)param)[0];
char ptmp[2048];
recv(sock, ptmp, 2048, 0);
CPO_Enter(g_cpo);
int a=g_hits;
sprintf(ptmp, WELCOME_HTML, a++);
g_hits=a;
CPO_Leave(g_cpo);
send(sock, HTTP_HEAD, strlen(HTTP_HEAD), 0);
send(sock, ptmp, strlen(ptmp), 0);
closesocket(sock);
return 0;
}
main(int argc,char ** argv)
{
char perror[1024];
SOCKET s,rs;
sockaddr_in sin,rsin;
socklen_t slen;
bool result=false;
perror[0]=0;
CPO_Init(g_cpo);
g_hits=0;
if(!bsocket_init())
{
strcpy(perror, "Can't init socket!");
goto error_out;
}
s=socket(PF_INET,SOCK_STREAM,0);
if(s==INVALID_SOCKET)
{
strcpy(perror, "Can't create socket!");
goto error_out;
}
sin.sin_family=AF_INET;
sin.sin_port=htons(WEB_SERVER_PORT);
sin.sin_addr.s_addr=inet_addr(WEB_SERVER_IP);
slen=sizeof(sin);
if(bind(s,(sockaddr *)&sin,slen)==SOCKET_ERROR)
{
strcpy(perror, "Can't bind socket!");
goto error_out;
}
if(listen(s,5)==SOCKET_ERROR)
{
strcpy(perror, "Can't listen on this socket!");
goto error_out;
}
printf("web server running......\n");
slen=sizeof(rsin);
while(true)
{
rs=accept(s,(sockaddr *)&rsin,&slen);
if(rs==INVALID_SOCKET)
{
strcpy(perror, "accept() a INVALID_SOCKET!");
break;
}
bcreate_thread(do_accept, &rs);
}
result=true;
error_out:
if(s!=INVALID_SOCKET) closesocket(s);
if(rs!=INVALID_SOCKET) closesocket(rs);
if(!result) { printf(perror); printf("\n"); }
CPO_Dele(g_cpo);
bsocket_cleanup();
return 0;
}
```

