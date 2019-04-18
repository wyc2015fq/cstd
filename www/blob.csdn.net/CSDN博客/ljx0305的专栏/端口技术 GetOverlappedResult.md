# 端口技术 GetOverlappedResult - ljx0305的专栏 - CSDN博客
2008年07月02日 16:39:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：1260标签：[socket																[io																[服务器																[winapi																[windows																[web服务](https://so.csdn.net/so/search/s.do?q=web服务&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=winapi&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=io&t=blog)](https://so.csdn.net/so/search/s.do?q=socket&t=blog)
个人分类：[网络编程](https://blog.csdn.net/ljx0305/article/category/403915)
早在两年前我就已经能很熟练的运用完成端口这种技术了,只是一直没有机会将它用在什么项目中,这段时间见到这种技术被过分炒作,过分的神秘化,就想写一篇解释它如何工作的文章.想告诉大家它没有传说中的那么高深难懂!有什么错误的地方还请高人指正.转载请注明出处及作者,谢谢!
以一个文件传输服务端为例,在我的机器上它只起两个线程就可以为很多个个客户端同时提供文件下载服务,程序的性能会随机器内CPU个数的增加而线性增长,我尽可能做到使它清晰易懂,虽然程序很小却用到了NT 5的一些新特性,重叠IO,完成端口以及线程池,基于这种模型的服务端程序应该是NT系统上性能最好的了.
首先.做为完成端口的基础,我们应该理解重叠IO,这需要你已经理解了内核对象及操作系统的一些概念概念,什么是信号/非信号态,什么是等待函数,什么是成功等待的副作用,什么是线程挂起等,如果这些概令还没有理解,你应该先看一下Windows 核心编程中的相关内容.如果已经理解这些,那么重叠IO对你来说并不难.
你可以这样认为重叠IO,现在你已经进入一个服务器/客户机环境,请不要混淆概念,这里的服务器是指操作系统,而客户机是指你的程序(它进行IO操作),是当你进行IO操作(send,recv,writefile,readfile….)时你发送一个IO请求给服务器(操作系统),由服务器来完成你需要的操作,然后你什么事都没有了,当服务器完成IO请求时它会通知你,当然在这期间你可以做任何事,一个常用的技巧是在发送重叠IO请求后,程序在一个循环中一边调用PeekMessage,TranslateMessage和DispatchMessage更新界面,同时调用 **GetOverlappedResult**等待服务器完成IO操作,更高效一点的做法是使用IO完成例程来处理服务器(操作系统)返回的结果,但并不是每个支持重叠IO操作的函数都支持完成例程如TransmitFile函数.
例1.一次重叠写操作过程(**GetOverlappedResult**方法):
1.填写一个OVERLAPPED结构
2.进行一次写操作,并指定重叠操作参数(上面的OVERLAPPED结构变量的指针)
3.做其它事(如更新界面)
4.**GetOverlappedResult**取操作结果
5.如果IO请求没有完成,并且没有出错则回到３
6.处理IO操作结果
例2.一次重叠写操作过程(完成例程方法):
1.填写一个OVERLAPPED结构
2.进行一次写操作,并指定重叠操作参数(上面的OVERLAPPED结构变量的指针),并指定完成例程
3.做其它事(如更新界面)
4.当完成例程被调用说明IO操作已经完成或出错,现在可以对操作结果进行处理了
如果你已经理解上面的概念,就已经很接近IO完成端口了,当然这只是很常规的重叠操作它已经非常高效,但如果再结合多线程对一个File或是Socket进行重叠IO操作就会非常复杂,通常程序员很难把握这种复杂度.完成端口可以说就是为了充分发挥多线程和重叠IO操作相结合的性能而设计的.很多人都说它复杂,其实如果你自己实现一个多线程的对一个File或是Socket进行重叠IO操作的程序(注意是多个线程对一个HANDLE或SOCKET进行重叠 IO操作,而不是启一个线程对一个HANDLE进行重叠IO操作)就会发现完成端口实际上简化了多线程里使用重叠IO的复杂度,并且性能更高,性能高在哪?下面进行说明.
我们可能写过这样的服务端程序:
例3.主程序:
1.监听一个端口
2.等待连接
3.当有连接来时
4.启一个线程对这个客户端进行处理
5.回到2
服务线程:
1.读客户端请求
2.如果客户端不再有请求,执行6
3.处理请求
4.返回操作结果
5.回到1
6.退出线程
这是一种最简单的网络服务器模型,我们把它优化一下
例4.主程序:
1.开一个线程池,里面有机器能承受的最大线程数个线程,线程都处于挂起(suspend)状态
1.监听一个端口
2.等待连接
3.当有连接来时
4.从线程池里Resume一个线程对这个客户端进行处理
5.回到2
服务线程与例3模型里的相同,只是当线程处理完客户端所有请求后,不是退出而是回到线程池,再次挂起让出CPU时间,并等待为下一个客户机服务.当然在此期间线程会因为IO操作(服务线程的第1,5操作,也许还有其它阻塞操作)挂起自己,但不会回到线程池,也就是说它一次只能为一个客户端服务.
这可能是你能想到的最高效的服务端模型了吧!它与第一个服务端模型相比少了很多个用户态到内核态的CONTEXT Switch,反映也更加快速,也许你可能觉得这很微不足道,这说明你缺少对大规模高性能服务器程序(比如网游服务端)的认识,如果你的服务端程序要对几千万个客户端进行服务呢?这也是微软Windows NT开发组在NT 5以上的系统中添加线程池的原因.
思考一下什么样的模型可以让一个线程为多个客户端服务呢!那就要跳出每来一个连接启线程为其服务的固定思维模式,我们把线程服务的最小单元分割为单独的读或写操作(注意是读或写不是读和写),而不是一个客户端从连接到断开期间的所有读写操作.每个线程都使用重叠IO进行读写操作,投递了读写请求后线程回到线程池,等待为其它客户机服务,当操作完成或出错时再回来处理操作结果,然后再回到线程池.
看看这样的服务器模型:
例5.主程序:
1.开一个线程池,里面有机器内CPU个数两倍的线程,线程都处于挂起(suspend)状态,它们在都等处理一次重叠IO操作的完成结果
1.监听一个端口
2.等待连接
3.当有连接来时
4.投递一个重叠读操作读取命令
5.回到2
服务线程:
1.如果读完成,则处理读取的内容(如HTTP GET命令),否则执行3
2.投递一个重叠写操作(如返回HTTP GET命令需要的网页)
3.如果是一个写操作完成,可以再投递一个重叠读操作,读取客户机的下一个请求,或者是关闭连接(如HTTP协议里每发完一个网页就断开)
4.取得下一个重叠IO操作结果,如果IO操作没有完成或没有IO操作则回到线程池
假设这是一个WEB服务器程序,可以看到工作者线程是以读或写为最小的工作单元运行的,在主程序里面进行了一次重叠读操作
当读操作完成时一个线程池中的一个工作者线程被激活取得了操作结果,处理GET或POST命令,然后发送一个网页内容,发送也是一个重叠操作,然后处理对其它客户机的IO操作结果,如果没有其它的东西需要处理时回到线程池等待.可以看到使用这种模型发送和接收可以是也可以不是一个线程.
当发送操作完成时,线程池中的一个工作者线程池激活,它关闭连接(HTTP协议),然后处理其它的IO操作结果,如果没有其它的东西需要处理时回到线程池等待.
看看在这样的模型中一个线程怎么为多个客户端服务,同样是模拟一个WEB服务器例子:
假如现在系统中有两个线程,ThreadA,ThreadB它们在都等处理一次重叠IO操作的完成结果
当一个客户机ClientA连接来时主程序投递一个重叠读操作,然后等待下一个客户机连接,当读操作完成时ThreadA被激活,它收到一个HTTP GET命令,然后ThreadA使用重叠写操作发送一个网页给ClientA,然后立即回到线程池等待处理下一个IO操作结果,这时发送操作还没有完成, 又有一个客户机ClientB连接来,主程序再投递一个重叠读操作,当读操作完成时ThreadA(当然也可能是ThreadB)再次被激活,它重复同样步骤,收到一个GET命令,使用重叠写操作发送一个网页给ClientB,这次它没有来得及回到线程池时,又有一个连接ClientC连入,主程序再投递一个重叠读操作,读操作完成时ThreadB被激活(因为ThreadA还没有回到线程池)它收到一个HTTP GET命令,然后ThreadB使用重叠写操作发送一个网页给ClientC,然后ThreadB回到线程池,这时ThreadA也回到了线程池.
可以想象现在有三个挂起的发送操作分别是ThreadA发送给ClientA和ClientB的网页,以及ThreadB发送给ClientC的网页,它们由操作系统内核来处理.ThreadA和ThreadB现在已经回到线程池,可以继续为其它任何客户端服务.
当对ClientA的重叠写操作已经完成,ThreadA(也可以是ThreadB)又被激活,它关闭与ClientA连接,但还没有回到线程池,与此同时发送给ClientB的重叠写操作也完成,ThreadB被激活(因为ThreadA还没有回到线程池)它关闭与ClientB的连接,然后回到线程池, 这时ClientC的写操作也完成,ThreadB再次被激活(因为ThreadA还是没有回到线程池),它再关闭与ClientC的连接,这时 ThreadA回到线程池,ThreadB也回到线程池.这时对三个客户端的服务全部完成.可以看到在整个服务过程中,”建立连接”,”读数据”,”写数据”和”关闭连接”等操作是逻辑上连续而实际上分开的.
到现在为止两个线程处理了三次读操作和三次写操作,在这些读写操作过程中所出现的状态机(state machine)是比较复杂的,我们模拟的是经过我简化过的,实际上的状态要比这个还要复杂很多,然而这样的服务端模型在客户端请求越多时与前两个模型相比的性能越高.而使用完成端口我们可以很容易实现这样的服务器模型.
微软的IIS WEB服务器就是使用这样的服务端模型,很多人说什么阿帕奇服务器比IIS的性能好什么什么的我表示怀疑,除非阿帕奇服务器可以将线程分割成,为更小的单元服务,我觉得不太可能!这种完成端口模型已经将单个读或写操作作为最小的服务单元,我觉得在相同机器配置的情况下IIS的性能要远远高于其它WEB服务器,这也是从实现机理上来分析的,如果出现性能上的差别可能是在不同的操作系统上,也许Linux的内核比Windows的要好,有人真的研究过吗?还是大家一起在炒作啊.
对于状态机概念,在很多方面都用到,TCPIP中有,编译原理中有,OpengGL中有等等,我的离散数学不好 (我是会计专业不学这个),不过还是搞懂了些,我想如果你多花些时间看,还是可以搞懂的.最后是一个简单的文件传输服务器程序代码,只用了两个线程(我的机器里只有一块CPU)就可以服务多个客户端.我调试时用它同时为6个nc客户端提供文件下载服务都没有问题,当然更多也不会有问题,只是略为使用了一下 NT 5的线程池和完成端口技术就可以有这样高的性能,更不用说IIS的性能咯!
希望大家不要陷在这个程序的框架中,Ctrl+C, Ctrl+V没有什么意义,要理解它的实质.程序使用Visual C++ 6.0 SP5+2003 Platform SDK编译通过,在Windows XP Professional下调试运行通过.程序运行的最低要求是Windows 2000操作系统.
/********************************************************************
created:??? 2005/12/24
created:??? 24:12:2005?? 20:25
modified:??? 2005/12/24
filename:???? d:/vcwork/iocomp/iocomp.cpp
file path:??? d:/vcwork/iocomp
file base:??? iocomp
file ext:??? cpp
author:??????? kruglinski(kruglinski_at_gmail_dot_com)
purpose:??? 利用完成端口技术实现的高性能文件下载服务程序
*********************************************************************/
#define _WIN32_WINNT??? 0×0500
#include <cstdlib>
#include <clocale>
#include <ctime>
#include <iostream>//一使用输入输出流程序顿时增大70K
#include <vector>
#include <algorithm>
#include <winsock2.h>
#include <mswsock.h>
using namespace std;
#pragma comment(lib,”ws2_32.lib”)
#pragma comment(lib,”mswsock.lib”)
const int MAX_BUFFER_SIZE=1024;
const int PRE_SEND_SIZE=1024;
const int QUIT_TIME_OUT=3000;
const int PRE_DOT_TIMER=QUIT_TIME_OUT/80;
typedef enum{IoTransFile,IoSend,IoRecv,IoQuit} IO_TYPE;
typedef struct
{
SOCKET hSocket;
SOCKADDR_IN ClientAddr;
}PRE_SOCKET_DATA,*PPRE_SOCKET_DATA;
typedef struct
{
OVERLAPPED??? oa;
WSABUF??????? DataBuf;
char??????? Buffer[MAX_BUFFER_SIZE];
IO_TYPE??????? IoType;
}PRE_IO_DATA,*PPRE_IO_DATA;
typedef vector<PPRE_SOCKET_DATA>??? SocketDataVector;
typedef vector<PPRE_IO_DATA>??????? IoDataVector;
SocketDataVector??? gSockDataVec;
IoDataVector??????? gIoDataVec;
CRITICAL_SECTION??? csProtection;
char* TimeNow(void)
{
time_t t=time(NULL);
tm *localtm=localtime(&t);
static char timemsg[512]={0};
strftime(timemsg,512,”%Z: %B %d %X,%Y”,localtm);
return timemsg;
}
BOOL TransFile(PPRE_IO_DATA pIoData,PPRE_SOCKET_DATA pSocketData,DWORD dwNameLen)
{
//这一句是为nc做的,你可以修改它
pIoData->Buffer[dwNameLen-1]=”;
HANDLE hFile=CreateFile(pIoData->Buffer,GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);
BOOL bRet=FALSE;
if(hFile!=INVALID_HANDLE_VALUE)
{
cout<<”Transmit File “<<pIoData->Buffer<<” to client”<<endl;
pIoData->IoType=IoTransFile;
memset(&pIoData->oa,0,sizeof(OVERLAPPED));
*reinterpret_cast<HANDLE*>(pIoData->Buffer)=hFile;
TransmitFile(pSocketData->hSocket,hFile,GetFileSize(hFile,NULL),PRE_SEND_SIZE,reinterpret_cast<LPOVERLAPPED>(pIoData),NULL,TF_USE_SYSTEM_THREAD);
bRet=WSAGetLastError()==WSA_IO_PENDING;
}
else
cout<<”Transmit File “<<”Error:”<<GetLastError()<<endl;
return bRet;
}
DWORD WINAPI ThreadProc(LPVOID IocpHandle)
{
DWORD dwRecv=0;
DWORD dwFlags=0;
HANDLE hIocp=reinterpret_cast<HANDLE>(IocpHandle);
DWORD dwTransCount=0;
PPRE_IO_DATA pPreIoData=NULL;
PPRE_SOCKET_DATA pPreHandleData=NULL;
while(TRUE)
{
if(GetQueuedCompletionStatus(hIocp,&dwTransCount,
reinterpret_cast<LPDWORD>(&pPreHandleData),
reinterpret_cast<LPOVERLAPPED*>(&pPreIoData),INFINITE))
{
if(0==dwTransCount&&IoQuit!=pPreIoData->IoType)
{
cout<<”Client:”
<<inet_ntoa(pPreHandleData->ClientAddr.sin_addr)
<<”:”<<ntohs(pPreHandleData->ClientAddr.sin_port)
<<” is closed”<<endl;
closesocket(pPreHandleData->hSocket);
EnterCriticalSection(&csProtection);
IoDataVector::iterator itrIoDelete=find(gIoDataVec.begin(),gIoDataVec.end(),pPreIoData);
SocketDataVector::iterator itrSockDelete=find(gSockDataVec.begin(),gSockDataVec.end(),pPreHandleData);
delete *itrIoDelete;
delete *itrSockDelete;
gIoDataVec.erase(itrIoDelete);
gSockDataVec.erase(itrSockDelete);
LeaveCriticalSection(&csProtection);
continue;
}
switch(pPreIoData->IoType){
case IoTransFile:
cout<<”Client:”
<<inet_ntoa(pPreHandleData->ClientAddr.sin_addr)
<<”:”<<ntohs(pPreHandleData->ClientAddr.sin_port)
<<” Transmit finished”<<endl;
CloseHandle(*reinterpret_cast<HANDLE*>(pPreIoData->Buffer));
goto LRERECV;
case IoSend:
cout<<”Client:”
<<inet_ntoa(pPreHandleData->ClientAddr.sin_addr)
<<”:”<<ntohs(pPreHandleData->ClientAddr.sin_port)
<<” Send finished”<<endl;
LRERECV:
pPreIoData->IoType=IoRecv;
pPreIoData->DataBuf.len=MAX_BUFFER_SIZE;
memset(&pPreIoData->oa,0,sizeof(OVERLAPPED));
WSARecv(pPreHandleData->hSocket,&pPreIoData->DataBuf,1,
&dwRecv,&dwFlags,
reinterpret_cast<LPWSAOVERLAPPED>(pPreIoData),NULL);
break;
case IoRecv:
cout<<”Client:”
<<inet_ntoa(pPreHandleData->ClientAddr.sin_addr)
<<”:”<<ntohs(pPreHandleData->ClientAddr.sin_port)
<<” recv finished”<<endl;
pPreIoData->IoType=IoSend;
if(!TransFile(pPreIoData,pPreHandleData,dwTransCount))
{
memset(&pPreIoData->oa,0,sizeof(OVERLAPPED));
strcpy(pPreIoData->DataBuf.buf,”File transmit error!/r/n”);
pPreIoData->DataBuf.len=strlen(pPreIoData->DataBuf.buf);
WSASend(pPreHandleData->hSocket,&pPreIoData->DataBuf,1,
&dwRecv,dwFlags,
reinterpret_cast<LPWSAOVERLAPPED>(pPreIoData),NULL);
}
break;
case IoQuit:
goto LQUIT;
default:
;
}
}
}
LQUIT:
return 0;
}
HANDLE hIocp=NULL;
SOCKET hListen=NULL;
BOOL WINAPI ShutdownHandler(DWORD dwCtrlType)
{
PRE_SOCKET_DATA PreSockData={0};
PRE_IO_DATA PreIoData={0};
PreIoData.IoType=IoQuit;
if(hIocp)
{
PostQueuedCompletionStatus(hIocp,1,
reinterpret_cast<ULONG_PTR>(&PreSockData),
reinterpret_cast<LPOVERLAPPED>(&PreIoData));
cout<<”Shutdown at “<<TimeNow()<<endl<<”wait for a moment please”<<endl;
//让出CPU时间,让线程退出
for(int t=0;t<80;t+=1)
{
Sleep(PRE_DOT_TIMER);
cout<<”.”;
}
CloseHandle(hIocp);
}
int i=0;
for(;i<gSockDataVec.size();i++)
{
PPRE_SOCKET_DATA pSockData=gSockDataVec[i];
closesocket(pSockData->hSocket);
delete pSockData;
}
for(i=0;i<gIoDataVec.size();i++)
{
PPRE_IO_DATA pIoData=gIoDataVec[i];
delete pIoData;
}
DeleteCriticalSection(&csProtection);
if(hListen)
closesocket(hListen);
WSACleanup();
exit(0);
return TRUE;
}
LONG WINAPI MyExceptionFilter(struct _EXCEPTION_POINTERS *ExceptionInfo)
{
ShutdownHandler(0);
return EXCEPTION_EXECUTE_HANDLER;
}
u_short DefPort=8182;
int main(int argc,char **argv)
{
if(argc==2)
DefPort=atoi(argv[1]);
InitializeCriticalSection(&csProtection);
SetUnhandledExceptionFilter(MyExceptionFilter);
SetConsoleCtrlHandler(ShutdownHandler,TRUE);
hIocp=CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,0);
WSADATA data={0};
WSAStartup(0×0202,&data);
hListen=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
if(INVALID_SOCKET==hListen)
{
ShutdownHandler(0);
}
SOCKADDR_IN addr={0};
addr.sin_family=AF_INET;
addr.sin_port=htons(DefPort);
if(bind(hListen,reinterpret_cast<PSOCKADDR>(&addr),
sizeof(addr))==SOCKET_ERROR)
{
ShutdownHandler(0);
}
if(listen(hListen,256)==SOCKET_ERROR)
ShutdownHandler(0);
SYSTEM_INFO si={0};
GetSystemInfo(&si);
si.dwNumberOfProcessors<<=1;
for(int i=0;i<si.dwNumberOfProcessors;i++)
{
QueueUserWorkItem(ThreadProc,hIocp,WT_EXECUTELONGFUNCTION);
}
cout<<”Startup at “<<TimeNow()<<endl
<<”work on port “<<DefPort<<endl
<<”press CTRL+C to shutdown”<<endl<<endl<<endl;
while(TRUE)
{
int namelen=sizeof(addr);
memset(&addr,0,sizeof(addr));
SOCKET hAccept=accept(hListen,reinterpret_cast<PSOCKADDR>(&addr),&namelen);
if(hAccept!=INVALID_SOCKET)
{
cout<<”accept a client:”<<inet_ntoa(addr.sin_addr)<<”:”<<ntohs(addr.sin_port)<<endl;
PPRE_SOCKET_DATA pPreHandleData=new PRE_SOCKET_DATA;
pPreHandleData->hSocket=hAccept;
memcpy(&pPreHandleData->ClientAddr,&addr,sizeof(addr));
CreateIoCompletionPort(reinterpret_cast<HANDLE>(hAccept),hIocp,reinterpret_cast<DWORD>(pPreHandleData),0);
PPRE_IO_DATA pPreIoData=new(nothrow) PRE_IO_DATA;
if(pPreIoData)
{
EnterCriticalSection(&csProtection);
gSockDataVec.push_back(pPreHandleData);
gIoDataVec.push_back(pPreIoData);
LeaveCriticalSection(&csProtection);
memset(pPreIoData,0,sizeof(PRE_IO_DATA));
pPreIoData->IoType=IoRecv;
pPreIoData->DataBuf.len=MAX_BUFFER_SIZE;
pPreIoData->DataBuf.buf=pPreIoData->Buffer;
DWORD dwRecv=0;
DWORD dwFlags=0;
WSARecv(hAccept,&pPreIoData->DataBuf,1,&dwRecv,&dwFlags,reinterpret_cast<WSAOVERLAPPED*>(pPreIoData),NULL);
}
else
{
delete pPreHandleData;
closesocket(hAccept);
}
}
}
return 0;
}
参考资料:
《MSDN 2001》
《Windows 网络编程》
《Windows 核心编程》
《TCP/IP详解》
]]> 4google http://my.donews.com/4google/
http://my.donews.com/4google/2007/05/07/post-070507-234520-3/ 2007-05-07T15:48:44Z
2007-05-07T15:48:44Z Luonix Luo [Luonix@21cn.com](mailto:Luonix@21cn.com)
转自:http://blog.csdn.net/thimin/archive/2007/10/30/1856217.aspx
