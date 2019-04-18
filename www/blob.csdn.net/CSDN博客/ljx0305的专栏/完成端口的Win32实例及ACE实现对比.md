# 完成端口的Win32实例及ACE实现对比 - ljx0305的专栏 - CSDN博客
2008年05月15日 20:03:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：602标签：[socket																[file																[null																[delete																[buffer																[io](https://so.csdn.net/so/search/s.do?q=io&t=blog)](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)](https://so.csdn.net/so/search/s.do?q=delete&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=socket&t=blog)
个人分类：[C++](https://blog.csdn.net/ljx0305/article/category/380566)
完成端口（IOC）是用来实现高性能服务的一种常用方法，主要是通过操作系统提供的异步调用功能实现IO操作，可以通过很少的线程数实现高性能的并发服务。[ACE的前摄器（Proactor）模式在Windows下就是通过完成端口实现的](http://blog.csdn.net/stephenxu111/archive/2008/05/15/2449397.aspx#ace)。
1．下面是网上一个**利用完成端口技术实现的高性能文件下载服务程序的源码：**
（用Visual C++ 6.0 SP5+2003 Platform SDK编译通过）
/******************************************************************** 
     created:     2005/12/24 
     created:     24:12:2005   20:25 
     modified:     2005/12/24 
     filename:     d:/vcwork/iocomp/iocomp.cpp 
     file path:     d:/vcwork/iocomp 
     file base:     iocomp 
     file ext:     cpp 
     author:         kruglinski(kruglinski_at_gmail_dot_com) 
     purpose:     利用完成端口技术实现的高性能文件下载服务程序
*********************************************************************/
#define _WIN32_WINNT     0x0500 
#include <cstdlib> 
#include <clocale> 
#include <ctime> 
#include <iostream>//一使用输入输出流程序顿时增大70K 
#include <vector> 
#include <algorithm> 
#include <winsock2.h> 
#include <mswsock.h> 
using namespace std; 
#pragma comment(lib,"ws2_32.lib") 
#pragma comment(lib,"mswsock.lib") 
const intMAX_BUFFER_SIZE=1024; 
const intPRE_SEND_SIZE=1024; 
const intQUIT_TIME_OUT=3000; 
const intPRE_DOT_TIMER=QUIT_TIME_OUT/80; 
typedef enum{IoTransFile,IoSend,IoRecv,IoQuit} IO_TYPE; 
typedef struct 
{ 
SOCKEThSocket; 
     SOCKADDR_IN ClientAddr; 
}PRE_SOCKET_DATA,*PPRE_SOCKET_DATA; 
typedef struct 
{ 
     OVERLAPPED     oa; 
     WSABUF         DataBuf; 
charBuffer[MAX_BUFFER_SIZE]; 
     IO_TYPE         IoType; 
}PRE_IO_DATA,*PPRE_IO_DATA; 
typedef vector<PPRE_SOCKET_DATA>     SocketDataVector; 
typedef vector<PPRE_IO_DATA>         IoDataVector; 
SocketDataVectorgSockDataVec; 
IoDataVectorgIoDataVec; 
CRITICAL_SECTIONcsProtection; 
char* TimeNow(void) 
{ 
time_tt=time(NULL); 
     tm *localtm=localtime(&t); 
     static chartimemsg[512]={0}; 
strftime(timemsg,512,"%Z: %B %d %X,%Y",localtm); 
     return timemsg; 
} 
BOOL TransFile(PPRE_IO_DATApIoData,PPRE_SOCKET_DATApSocketData,DWORDdwNameLen) 
{ 
//这一句是为nc做的,你可以修改它
pIoData->Buffer[dwNameLen-1]='/0'; 
HANDLEhFile=CreateFile(pIoData->Buffer,GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL); 
BOOLbRet=FALSE; 
     if(hFile!=INVALID_HANDLE_VALUE) 
     { 
cout<<"Transmit File "<<pIoData->Buffer<<" to client"<<endl; 
pIoData->IoType=IoTransFile; 
memset(&pIoData->oa,0,sizeof(OVERLAPPED)); 
         *reinterpret_cast<HANDLE*>(pIoData->Buffer)=hFile; 
TransmitFile(pSocketData->hSocket,hFile,GetFileSize(hFile,NULL),PRE_SEND_SIZE,reinterpret_cast<LPOVERLAPPED>(pIoData),NULL,TF_USE_SYSTEM_THREAD); 
bRet=WSAGetLastError()==WSA_IO_PENDING; 
     } 
     else 
cout<<"Transmit File "<<"Error:"<<GetLastError()<<endl; 
     return bRet; 
} 
DWORDWINAPI ThreadProc(LPVOIDIocpHandle) 
{ 
DWORDdwRecv=0; 
DWORDdwFlags=0; 
HANDLEhIocp=reinterpret_cast<HANDLE>(IocpHandle); 
DWORDdwTransCount=0; 
PPRE_IO_DATApPreIoData=NULL; 
PPRE_SOCKET_DATApPreHandleData=NULL; 
     while(TRUE) 
     { 
         if(GetQueuedCompletionStatus(hIocp,&dwTransCount, 
             reinterpret_cast<LPDWORD>(&pPreHandleData), 
             reinterpret_cast<LPOVERLAPPED*>(&pPreIoData),INFINITE)) 
        { 
             if(0==dwTransCount&&IoQuit!=pPreIoData->IoType) 
             { 
cout<<"Client:"
                     <<inet_ntoa(pPreHandleData->ClientAddr.sin_addr) 
                     <<":"<<ntohs(pPreHandleData->ClientAddr.sin_port) 
                     <<" is closed"<<endl; 
closesocket(pPreHandleData->hSocket); 
EnterCriticalSection(&csProtection); 
IoDataVector::iteratoritrIoDelete=find(gIoDataVec.begin(),gIoDataVec.end(),pPreIoData); 
SocketDataVector::iteratoritrSockDelete=find(gSockDataVec.begin(),gSockDataVec.end(),pPreHandleData); 
                   delete *itrIoDelete; 
                   delete *itrSockDelete; 
gIoDataVec.erase(itrIoDelete); 
gSockDataVec.erase(itrSockDelete); 
LeaveCriticalSection(&csProtection); 
                 continue; 
             } 
             switch(pPreIoData->IoType){ 
             case IoTransFile: 
cout<<"Client:"
                     <<inet_ntoa(pPreHandleData->ClientAddr.sin_addr) 
                     <<":"<<ntohs(pPreHandleData->ClientAddr.sin_port) 
                     <<" Transmit finished"<<endl; 
CloseHandle(*reinterpret_cast<HANDLE*>(pPreIoData->Buffer)); 
                 goto LRERECV; 
             case IoSend: 
cout<<"Client:"
                     <<inet_ntoa(pPreHandleData->ClientAddr.sin_addr) 
                     <<":"<<ntohs(pPreHandleData->ClientAddr.sin_port) 
                     <<" Send finished"<<endl; 
LRERECV: 
pPreIoData->IoType=IoRecv; 
pPreIoData->DataBuf.len=MAX_BUFFER_SIZE; 
memset(&pPreIoData->oa,0,sizeof(OVERLAPPED)); 
WSARecv(pPreHandleData->hSocket,&pPreIoData->DataBuf,1, 
                     &dwRecv,&dwFlags, 
                     reinterpret_cast<LPWSAOVERLAPPED>(pPreIoData),NULL); 
                 break; 
             case IoRecv: 
cout<<"Client:"
                     <<inet_ntoa(pPreHandleData->ClientAddr.sin_addr) 
                     <<":"<<ntohs(pPreHandleData->ClientAddr.sin_port) 
                     <<" recv finished"<<endl; 
pPreIoData->IoType=IoSend; 
                 if(!TransFile(pPreIoData,pPreHandleData,dwTransCount)) 
                 { 
memset(&pPreIoData->oa,0,sizeof(OVERLAPPED)); 
strcpy(pPreIoData->DataBuf.buf,"File transmit error!/r/n"); 
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
HANDLEhIocp=NULL; 
SOCKEThListen=NULL; 
BOOLWINAPI ShutdownHandler(DWORDdwCtrlType) 
{ 
PRE_SOCKET_DATAPreSockData={0}; 
PRE_IO_DATAPreIoData={0}; 
PreIoData.IoType=IoQuit; 
     if(hIocp) 
     { 
PostQueuedCompletionStatus(hIocp,1, 
             reinterpret_cast<ULONG_PTR>(&PreSockData), 
             reinterpret_cast<LPOVERLAPPED>(&PreIoData)); 
cout<<"Shutdown at "<<TimeNow()<<endl<<"wait for a moment please"<<endl; 
//让出CPU时间,让线程退出
         for(intt=0;t<80;t+=1) 
         { 
Sleep(PRE_DOT_TIMER); 
cout<<"."; 
         } 
CloseHandle(hIocp); 
     } 
inti=0; 
     for(;i<gSockDataVec.size();i++) 
     { 
PPRE_SOCKET_DATApSockData=gSockDataVec[i]; 
closesocket(pSockData->hSocket); 
         delete pSockData; 
     } 
     for(i=0;i<gIoDataVec.size();i++) 
     { 
PPRE_IO_DATApIoData=gIoDataVec[i]; 
         delete pIoData; 
     } 
DeleteCriticalSection(&csProtection); 
     if(hListen) 
closesocket(hListen); 
WSACleanup(); 
exit(0); 
     return TRUE; 
} 
LONGWINAPI MyExceptionFilter(struct _EXCEPTION_POINTERS *ExceptionInfo) 
{ 
     ShutdownHandler(0); 
     return EXCEPTION_EXECUTE_HANDLER; 
} 
u_shortDefPort=8182; 
intmain(intargc,char **argv) 
{ 
     if(argc==2) 
DefPort=atoi(argv[1]); 
InitializeCriticalSection(&csProtection); 
SetUnhandledExceptionFilter(MyExceptionFilter); 
SetConsoleCtrlHandler(ShutdownHandler,TRUE); 
hIocp=CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,0); 
     WSADATA data={0}; 
WSAStartup(0x0202,&data); 
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
     for(inti=0;i<si.dwNumberOfProcessors;i++) 
     { 
         QueueUserWorkItem(ThreadProc,hIocp,WT_EXECUTELONGFUNCTION); 
     } 
cout<<"Startup at "<<TimeNow()<<endl
         <<"work on port "<<DefPort<<endl
         <<"press CTRL+C to shutdown"<<endl<<endl<<endl; 
     while(TRUE) 
     { 
intnamelen=sizeof(addr); 
memset(&addr,0,sizeof(addr)); 
SOCKEThAccept=accept(hListen,reinterpret_cast<PSOCKADDR>(&addr),&namelen); 
         if(hAccept!=INVALID_SOCKET) 
         { 
cout<<"accept a client:"<<inet_ntoa(addr.sin_addr)<<":"<<ntohs(addr.sin_port)<<endl; 
PPRE_SOCKET_DATApPreHandleData=newPRE_SOCKET_DATA; 
pPreHandleData->hSocket=hAccept; 
memcpy(&pPreHandleData->ClientAddr,&addr,sizeof(addr)); 
CreateIoCompletionPort(reinterpret_cast<HANDLE>(hAccept),hIocp,reinterpret_cast<DWORD>(pPreHandleData),0); 
PPRE_IO_DATApPreIoData=new(nothrow) PRE_IO_DATA; 
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
DWORDdwRecv=0; 
DWORDdwFlags=0; 
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
2． 而使用ACE前摄器模式的话，IOC架构更加清晰，扩展性更强，而且是跨平台的！在ACE包里面ACE_wrappers/examples/Reactor/Proactor下面有几个例子，可以说明这个应用：
l**test_udp_proactor**基于Proactor的UDP服务器以及客户端的例子。服务端先打开一个udp服务端口，然后在ACE_Asynch_Read_Dgram上异步读数据。运行这个应用时带-h参数时做服务器，不带时做客户端。
l**test_timeout**基于Proactor的计时器事件处理例子。首先在前摄器框架上注册计时器ACE_Proactor::instance ()->schedule_timer()，然后激活Worker线程池，每个工作线程的svc()函数中，调用ACE_Proactor::run_event_loop()处理计时器到期事件。这和反应器框架的主要不同在于，前摄器的线程池中的每个线程可以处理不同计时器的到时事件。
l**test_proactor**基于Proactor的TCP文件服务器以及客户端的例子，功能类似于前面的“[利用完成端口技术实现的高性能文件下载服务程序](http://blog.csdn.net/stephenxu111/archive/2008/05/15/2449397.aspx#IOC_WIN32_SAMPLE)”。其中客户端Sender读取一个本地文件，然后写到服务端去（512字节一组），读取和发送都是异步的。服务端每来一个连接，就创建一个Receiver，这个Receiver接收客户端发来的数据，并把它存到文件（默认的名字叫output），接收和保存两者都是异步的。
Main函数功能：
              服务端首先创建一个异步连接器：
                               ACE_Asynch_Acceptor<Receiver> acceptor;
              然后打开异步连接器，进入服务状态：
                               acceptor.open()
              再进入前摄器主事件分派循环：
                               ACE_Proactor::instance ()->handle_events ()
连接到来时会创建一个Receiver，并由OS调用Receiver::open()，并且打开<ACE_Asynch_Write_File>和<ACE_Asynch_Read_Stream>，准备进行读取TCP连接发来的数据和将数据写文件的工作，然后调用initiate_read_stream()进行第一次的数据接收工作。
如果这两项工作被OS完成，则回调handle_write_file()和handle_read_stream()。
这个实例是单线程的。在多CPU环境下，需要改成工作者线程模式。
另外测试时发现test_proactor实例有个错误的地方：
1） Receiver::open()里面，默认的文件打开函数打不开文件
/*Stephen：下面的直接执行会导致打不开，报 ACE_OS::open: 参数错误。
 this->dump_file_ = ACE_OS::open (dump_file,
                                   O_CREAT | O_RDWR | O_TRUNC | FILE_FLAG_OVERLAPPED,
                                   0644);
 */
 this->dump_file_ = ACE_OS::open (dump_file, O_CREAT | O_RDWR | O_TRUNC | FILE_FLAG_OVERLAPPED);
// Stephen：曾经试图把上面第二个参数改为GENERIC_READ | GENERIC_WRITE | FILE_FLAG_OVERLAPPED，结果不行，不是打不开文件，就是input/output error
 // Stephen：失败是由于0644参数造成的，这个是干什么用的呢？
还有每次计划从文件读回来1024字节，但显示读出只有512字节，何故？
//Stephen：后来发现stdio.h里面BUFSIZ确实是512，而且它在include路径的前面，导致每次都读512字节，在ace里面一些地方才是1024（for WinCE）
int
Sender::initiate_read_file (void)
{
// Create a new <Message_Block>. Note that this message block will
// be used both to <read> data asynchronously from the file and to
// <write> data asynchronously to the socket.
ACE_Message_Block *mb = 0;
ACE_NEW_RETURN (mb,
ACE_Message_Block (BUFSIZ + 1),
                  -1);
// Inititiate an asynchronous read from the file
 // 由于是异步的，所以下面的read()函数很快返回，当OS读完后，回调Sender::handle_read_file()函数去处理发送，并在这个函数中激发下一次512字节的读
 if (this->rf_.read (*mb,
mb->size () - 1,
                      this->file_offset_) == -1)
ACE_ERROR_RETURN ((LM_ERROR,
"%p/n",
"ACE_Asynch_Read_File::read"),
                      -1);
 return 0;
}
引用:http://blog.csdn.net/stephenxu111/archive/2008/05/15/2449397.aspx
