# Select模型的使用常识 - 三少GG - CSDN博客
2012年08月02日 22:44:16[三少GG](https://me.csdn.net/scut1135)阅读数：1099

## Select模型的使用常识
分类:[技术交流](http://itman2006.bokee.com/cat.10192934.html)
2006.7.1118:02作者：[itman2006](http://id.bokee.com/showInfo.b?username=itman2006.bokee.com)
 | [评论：0](http://itman2006.bokee.com/viewdiary.11382347.html#remark) | 阅读：1765
　　在网络编程中,有五种模型可以使用,它们分别是select、WSAAsyncSelect、WSAEventSelect、Overlapped、Completion。在这里五种模型中，除了select模型以外，其他模型不能移植到Linux中，而我们通常情况所写的程序都要适应于windows和linux，所以我们选择了select模型。
　　它的核心函数是int select( int nfds , fd_set FAR* readfds , fd_set FAR* writefds , fd_set FAR* exceptfds ,const struct timeval FAR * timeout ) ；该函数来检查你要调用的socket套接字是否已经有了需要处理的数据。
　　select包含三个socket队列,分别代表:readfds ,检查可读性,writefds,检查可写性,exceptfds,例外数据。
timeout是select函数的返回时间。例如,我们想要检查一个套接字是否有数据需要接收,我们可以把套接字句柄加入可读性检查队列中,然后调用select,如果,该套接字没有数据需要接收,select函数会把该套接字从可读性检查队列中删除掉,所以我们只要检查该套接字句柄是否还存在于可读性队列中,就可以知道到底有没有数据需要接收了。
　　另外Winsock提供了一些宏用来操作套接字队列fd_set。
　　FD_CLR( s,*set) 从队列set删除句柄s.
　　FD_ISSET( s, *set) 检查句柄s是否存在与队列set中.
　　FD_SET( s,*set )把句柄s添加到队列set中.
　　FD_ZERO( *set ) 把set队列初始化成空队列.
　　使用select模型的大体结构如下：
　　第一步：初始化(WSAStartup->socket->bind->listen)。
　　第二步：ioctlsocket改变套接制的控制模式。
　　第三步：设计线程，其线程体内部结构为：
　　　　FD_ZERO(&ReadSet);
      　FD_ZERO(&WriteSet);
        FD_SET(ListenSocket, &ReadSet);
        if ((Total = select(0, &ReadSet, &WriteSet, NULL, NULL)) == SOCKET_ERROR
　　　　　......
        if (FD_ISSET(ListenSocket, &ReadSet))
      {
          if ((AcceptSocket = accept(ListenSocket, NULL, NULL)) != INVALID_SOCKET)
         {
            NonBlock = 1;
            if (ioctlsocket(AcceptSocket, FIONBIO, &NonBlock) == SOCKET_ERROR)
            {
　　　　　　......
            }
        }
         else
         {  
　　　　　　......
         }
      }
      for (i = 0; Total > 0 && i < TotalSockets; i++)
      {
         if (FD_ISSET(SocketInfo->Socket, &ReadSet))
         {
             if (WSARecv(SocketInfo->Socket, &(SocketInfo->DataBuf), 1, &RecvBytes,&Flags, NULL, NULL) == SOCKET_ERROR)
            {
               ......            
            } 
            else
            {
              ......
            }
         }
         if (FD_ISSET(SocketInfo->Socket, &WriteSet))
         {
           ......
           if (WSASend(SocketInfo->Socket, &(SocketInfo->DataBuf), 1, &SendBytes, 0,NULL, NULL) == SOCKET_ERROR)
            {
              ......
            }
            else
            {
              ......
            }
         }
      }
   }
}
### [Select模型原理](http://blog.csdn.net/yanheifeng715000/article/details/6592447)
2011-07-08 12:25
460人阅读[评论](http://blog.csdn.net/yanheifeng715000/article/details/6592447#comments)(0)收藏[举报](http://blog.csdn.net/yanheifeng715000/article/details/6592447#report)
Select模型原理
利用select函数，判断套接字上是否存在数据，或者能否向一个套接字写入数据。目的是防止应用程序在套接字处于锁定模式时，调用recv（或send）从没有数据的套接字上接收数据，被迫进入阻塞状态。
select参数和返回值意义如下：
int **select** (
 IN **int** nfds,                           //0,无意义
 IN OUT **fd_set*** readfds,      //检查可读性
 IN OUT** fd_set*** writefds,     //检查可写性
 IN OUT f**d_set* **exceptfds,  //例外数据
 IN const** struct timeval*** timeout);    //函数的返回时间
struct  **timeval** {
        long    tv_sec;        //秒 
        long    tv_usec;     //毫秒
};
select返回fd_set中可用的套接字个数。
fd_set是一个SOCKET队列，以下宏可以对该队列进行操作：
FD_CLR( s, *set) 从队列set删除句柄s;
FD_ISSET( s, *set) 检查句柄s是否存在与队列set中;
FD_SET( s, *set )把句柄s添加到队列set中;
FD_ZERO( *set ) 把set队列初始化成空队列.
**Select工作流程**
1：用FD_ZERO宏来初始化我们感兴趣的fd_set。
也就是select函数的第二三四个参数。
2：用FD_SET宏来将套接字句柄分配给相应的fd_set。
如果想要检查一个套接字是否有数据需要接收，可以用FD_SET宏把套接接字句柄加入可读性检查队列中
3：调用select函数。
如果该套接字没有数据需要接收，select函数会把该套接字从可读性检查队列中删除掉，
4：用FD_ISSET对套接字句柄进行检查。
如果我们所关注的那个套接字句柄仍然在开始分配的那个fd_set里，那么说明马上可以进行相应的IO操 作。比如一个分配给select第一个参数的套接字句柄在select返回后仍然在select第一个参数的fd_set里，那么说明当前数据已经来了， 马上可以读取成功而不会被阻塞。
**[cpp]**[view plain](http://blog.csdn.net/yanheifeng715000/article/details/6592447#)[copy](http://blog.csdn.net/yanheifeng715000/article/details/6592447#)[print](http://blog.csdn.net/yanheifeng715000/article/details/6592447#)[?](http://blog.csdn.net/yanheifeng715000/article/details/6592447#)
- #include <winsock2.h>   
- #include <stdio.h>   
- 
- #define PORT  5150   
- 
- #define MSGSIZE  1024   
- 
- #pragma comment(lib, "ws2_32.lib")   
- 
- int g_iTotalConn1 = 0;    
- int g_iTotalConn2 = 0;     
- 
- SOCKET g_CliSocketArr1[FD_SETSIZE];    
- SOCKET g_CliSocketArr2[FD_SETSIZE];    
- 
- DWORD WINAPI WorkerThread1(LPVOID lpParam);     
- int CALLBACK ConditionFunc1(LPWSABUF lpCallerId,LPWSABUF lpCallerData, LPQOS lpSQOS,LPQOS lpGQOS,LPWSABUF lpCalleeId, LPWSABUF lpCalleeData,GROUP FAR * g,DWORD dwCallbackData);  
- 
- DWORD WINAPI WorkerThread2(LPVOID lpParam);     
- int CALLBACK ConditionFunc2(LPWSABUF lpCallerId,LPWSABUF lpCallerData, LPQOS lpSQOS,LPQOS lpGQOS,LPWSABUF lpCalleeId, LPWSABUF lpCalleeData,GROUP FAR * g,DWORD dwCallbackData);  
- 
- 
- int main(int argc, char* argv[])     
- {     
-  WSADATA wsaData;     
-  SOCKET sListen, sClient;     
-  SOCKADDR_IN local, client;     
- int iAddrSize = sizeof(SOCKADDR_IN);     
- DWORD dwThreadId;     
- // Initialize windows socket library   
-  WSAStartup(0x0202, &wsaData);     
- // Create listening socket   
-  sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);     
- // Bind   
- 
-  local.sin_family = AF_INET;     
-  local.sin_addr.S_un.S_addr = htonl(INADDR_ANY);     
-  local.sin_port = htons(PORT);     
-  bind(sListen, (sockaddr*)&local, sizeof(SOCKADDR_IN));     
- 
- // Listen   
- 
-  listen(sListen, 3);     
- 
- // Create worker thread   
- 
-  CreateThread(NULL, 0, WorkerThread1, NULL, 0, &dwThreadId);    
- // CreateThread(NULL, 0, WorkerThread2, NULL, 0, &dwThreadId);  
- 
- while (TRUE)      
-  {       
-   sClient = WSAAccept(sListen, (sockaddr*)&client, &iAddrSize, ConditionFunc1, 0);  
-   printf("1:Accepted client:%s:%d:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port), g_iTotalConn1);     
-   g_CliSocketArr1[g_iTotalConn1++] = sClient;    
- /*         
-   sClient = WSAAccept(sListen, (sockaddr*)&client, &iAddrSize, ConditionFunc2, 0);
-   printf("2:Accepted client:%s:%d:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port), g_iTotalConn2);   
-   g_CliSocketArr2[g_iTotalConn2++] = sClient;   */
- 
-  }     
- return 0;     
- }     
- 
- DWORD WINAPI WorkerThread1(LPVOID lpParam)     
- {     
- int i;     
-  fd_set fdread;     
- int ret;     
- struct timeval tv = {1, 0};     
- char szMessage[MSGSIZE];     
- while (TRUE)      
-  {     
-   FD_ZERO(&fdread);   //1清空队列
- for (i = 0; i < g_iTotalConn1; i++)      
-   {     
-    FD_SET(g_CliSocketArr1[i], &fdread);   //2将要检查的套接口加入队列
-   }     
- 
- // We only care read event   
-   ret = select(0, &fdread, NULL, NULL, &tv);   //3查询满足要求的套接字，不满足要求，出队
- if (ret == 0)      
-   {     
- // Time expired   
- continue;     
-   }     
- 
- for (i = 0; i < g_iTotalConn1; i++)      
-   {     
- if (FD_ISSET(g_CliSocketArr1[i], &fdread))    //4.是否依然在队列
-    {     
- // A read event happened on g_CliSocketArr   
- 
-     ret = recv(g_CliSocketArr1[i], szMessage, MSGSIZE, 0);     
- if (ret == 0 || (ret == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET))      
-     {     
- // Client socket closed   
-      printf("1:Client socket %d closed.\n", g_CliSocketArr1[i]);     
-      closesocket(g_CliSocketArr1[i]);     
- if (i < g_iTotalConn1-1)      
-      {     
-       g_CliSocketArr1[i--] = g_CliSocketArr1[--g_iTotalConn1];     
-      }     
-     }      
- else
-     {     
- // We reveived a message from client   
-      szMessage[ret] = '\0';     
-      send(g_CliSocketArr1[i], szMessage, strlen(szMessage), 0);     
-     }     
-    }     
-   }    
-  }     
- }   
- 
- int CALLBACK ConditionFunc1(LPWSABUF lpCallerId,LPWSABUF lpCallerData, LPQOS lpSQOS,LPQOS lpGQOS,LPWSABUF lpCalleeId, LPWSABUF lpCalleeData,GROUP FAR * g,DWORD dwCallbackData)  
- {  
- if (g_iTotalConn1 < FD_SETSIZE)  
- return CF_ACCEPT;  
- else
- return CF_REJECT;  
- }  
- 
- DWORD WINAPI WorkerThread2(LPVOID lpParam)     
- {     
- int i;     
-  fd_set fdread;     
- int ret;     
- struct timeval tv = {1, 0};     
- char szMessage[MSGSIZE];     
- while (TRUE)      
-  {     
-   FD_ZERO(&fdread);   //1清空队列
- for (i = 0; i < g_iTotalConn2; i++)      
-   {     
-    FD_SET(g_CliSocketArr2[i], &fdread);   //2将要检查的套接口加入队列
-   }     
- 
- // We only care read event   
-   ret = select(0, &fdread, NULL, NULL, &tv);   //3查询满足要求的套接字，不满足要求，出队
- if (ret == 0)      
-   {     
- // Time expired   
- continue;     
-   }     
- 
- for (i = 0; i < g_iTotalConn2; i++)      
-   {     
- if (FD_ISSET(g_CliSocketArr2[i], &fdread))    //4.是否依然在队列
-    {     
- // A read event happened on g_CliSocketArr   
- 
-     ret = recv(g_CliSocketArr2[i], szMessage, MSGSIZE, 0);     
- if (ret == 0 || (ret == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET))      
-     {     
- // Client socket closed   
-      printf("2:Client socket %d closed.\n", g_CliSocketArr1[i]);     
-      closesocket(g_CliSocketArr2[i]);     
- if (i < g_iTotalConn2-1)      
-      {     
-       g_CliSocketArr2[i--] = g_CliSocketArr2[--g_iTotalConn2];     
-      }     
-     }      
- else
-     {     
- // We reveived a message from client   
-      szMessage[ret] = '\0';     
-      send(g_CliSocketArr2[i], szMessage, strlen(szMessage), 0);     
-     }     
-    }     
-   }    
-  }     
- }   
- 
- int CALLBACK ConditionFunc2(LPWSABUF lpCallerId,LPWSABUF lpCallerData, LPQOS lpSQOS,LPQOS lpGQOS,LPWSABUF lpCalleeId, LPWSABUF lpCalleeData,GROUP FAR * g,DWORD dwCallbackData)  
- {  
- if (g_iTotalConn2 < FD_SETSIZE)  
- return CF_ACCEPT;  
- else
- return CF_REJECT;  
- }  
服务器的几个主要动作如下：
1.创建监听套接字，绑定，监听；
2.创建工作者线程；
3.创建一个套接字数组，用来存放当前所有活动的客户端套接字，每accept一个连接就更新一次数组；
4.接受客户端的连接。
这里有一点需要注意的，就是我没有重新定义FD_SETSIZE宏，所以服务器最多支持的并发连接数为64。而且，这里决不能无条件的accept,服务器应该根据当前的连接数来决定是否接受来自某个客户端的连接。 
工作者线程里面是一个死循环，一次循环完成的动作是：
1.将当前所有的客户端套接字加入到读集fdread中；
2.调用select函数；
3.查看某个套接字是否仍然处于读集中，如果是，则接收数据。如果接收的数据长度为0，或者发生WSAECONNRESET错误，则表示客户端套接字主动关闭，这时需要将服务器中对应的套接字所绑定的资源释放掉，然后调整我们的套接字数组（将数组中最后一个套接字挪到当前的位置上）
除了需要有条件接受客户端的连接外，还需要在连接数为0的情形下做特殊处理，因为如果读集中没有任何套接字，select函数会立刻返回，这将导致工作者线程成为一个毫无停顿的死循环，CPU的占用率马上达到100%。
关系到套接字列表的操作都需要使用循环,在轮询的时候,需要遍历一次,再新的一轮开始时,将列表加入队列又需要遍历一次.也就是说,Select在工作一次时,需要至少遍历2次列表,这是它效率较低的原因之一.在大规模的网络连接方面,还是推荐使用IOCP或EPOLL模型.但是Select模型可以使用在诸如对战类游戏上,比如类似星际这种,因为它小巧易于实现,而且对战类游戏的网络连接量并不大.
对于Select模型想要突破Windows 64个限制的话,可以采取分段轮询,一次轮询64个.例如套接字列表为128个,在第一次轮询时,将前64个放入队列中用Select进行状态查询,待本次操作全部结束后.将后64个再加入轮询队列中进行轮询处理.这样处理需要在非阻塞式下工作.以此类推,Select也能支持无限多个.
注意：
1.那个最大的连接数是指每一个线程可以处理的连接数，当你有多个线程时，连接数是可以无限增长的，不过此时的效率就比较低。
2.关于发送操作writefds的问题，当套接字成功连接或者一个套接字刚刚成功接收信息时都会调用。
3.我们通常会创建一个套接字来进行监听，之后用accept返回的套接字进行通信。这里要注意一点，用于监听的套接字在没有新连接时也会进行writefds的操作。
WinSocket模型的探讨——select模型(1)
2010-10-14 17:04
查找了很多资料都找不到select模型的详细用法，《Windows网络编程》这本书上也只是写了一个简单的回应服务器，就连writefds的用法都没讲，也不知道什么时候利用“可写”来发文件。这些都是我的疑问，相信很多研究网络编程的同路人也碰到了我的这些问题。这些疑问在这篇文章中都解决了！耗费了偶很多的精力去猜测去思考！
感觉一些已经得道的高人都不肯把这些问题说透彻点，唉，只能靠自己去摸索了，希望这篇文章能对你有用，也希望一些高人能出来指点指点！
SOCKET模型的出现是为了解决“一个客户端一线程”的问题，为了WINDOWS的线程切换不要太频繁，我们可以使用WINDOWS的 SOCKET模型。但在论坛里又看到了一些文章说现在的计算机硬件相当发达，成万的线程切换根本不是什么问题。无论怎么样，既然这些模型被MS发明了出来，就有它的道理，我们还是好好来学习一下吧。
对于select模型，大家都知道用select函数，然后判断读集、写集。但如何使用select，最终写好的Server又是一个怎么样的结构呢？
select可以这样用，写成两个函数，SelectSend和SelectRecv，这两个函数和一般的sendrecv不同的地方在于它是有超时值的，这样就不会把程序完全阻塞了。这两个函数大概如下（参考《PC网络游戏编程》）：
SelectRecv(...)
int SelectRecv(SOCKET hSocket,char*pszBuffer,
int nBufferSize,
         DWORD dwTimeout)
{
     ASSERT(hSocket!= NULL);
if(hSocket==NULL)
return ( SOCKET_ERROR );
     FD_SET fd={1, hSocket};
     TIMEVAL tv={dwTimeout, 0};
int nBytesReceived=0;
if(select(0,&fd,
 NULL, NULL,&tv)==0)
goto CLEAR;
if((nBytesReceived= recv(hSocket,
 pszBuffer, nBufferSize,0))== SOCKET_ERROR)
goto CLEAR;
return nBytesReceived;
CLEAR:
     SetLastError(WSAGetLastError());//超时
return(SOCKET_ERROR);
}
SelectSend(...)
int SelectSend(SOCKET hSocket,charconst* pszBuffer,
int nBufferSize, DWORD dwTimeout)
{
if(hSocket==NULL)
return(SOCKET_ERROR);
int nBytesSent=0;
int nBytesThisTime;
constchar* pszTemp=
 pszBuffer;
do{
         nBytesThisTime= Send_Block(hSocket,pszTemp, nBufferSize-nBytesSent,
 dwTimeout);
if(nBytesThisTime<0)            return(SOCKET_ERROR);
//如果一次没有发送成功
         nBytesSent+= nBytesThisTime;
//改变当前字符指针
         pszTemp+= nBytesThisTime;
     }while(nBytesSent< nBufferSize);
return nBytesSent;
}
这样就可以利用上面两个函数写发送和接收程序了！我们下面来看看如何使用select模型建立我们的Server，下面分了4个文件，大家可以拷贝下来实际编译一下。首先有两个公用的文件：CommonSocket.h、CommonCmd.h。他们都是放在 Common Include 目录中的！然后就是SelectServer.cpp和SocketClient.cpp这两个文件，可以分别建立两个工程把他们加进去编译！有些关键的地方我都写在文件注释里了，可以自己看看。为了方便大家拷贝，我就不用“代码插入”的方式了。直接贴到下面！
/**********************************************************************************************************************/
第一个文件
/*/
文件：SelectServer.cpp
说明：
此文件演示了如何使用select模型来建立服务器，难点是select的writefds在什么时候使用。
好好看看代码就能很明白的了，可以说我写这些代码就是为了探索这个问题的！找了很多资料都找不到！！
在这里我怀疑是否可以同时读写同一个SOCKET，结果发现是可以的，但是最好别这样做。因为会导致包的顺序不一致。
    这里说一下SELECT模型的逻辑：
我们如果不使用select模型，在调用recv或者send时候会导致程序阻塞。如果使用了select
就给我们增加了一层保护，就是说在调用了select函数之后，对处于读集合的socket进行recv操作
是一定会成功的（这是操作系统给我们保证的）。对于判断SOCKET是否可写时也一样。
而且就算不可读或不可写，使用了select也不会锁 死！因为 select 函数提供了超时！利用这个特性还可以
做异步connect，也就是可以扫描主机，看哪个主机开了服务（远程控制软件经常这样干哦！）
我们如何利用这种逻辑来设计我们的server呢？
这里用的方法是建立一个SocketInfo，这个SocketInfo包括了对Socket当前进行的操作，我把它分为：
{RecvCmd, RecvData, ExecCmd} 一开始socket是处于一个RecvCmd的状态，
然后取到了CMD（也就是取到了指令，可想象一下CPU得到了指令后干什么），然后就要取数据了，取得指令
知道要干什么，取得了数据就可以实际开始干了。实际开始干就是ExecCmd，在这个状态之后都是需要
发送数据的了，所以把他们都放在判断SOCKET可写下面<就是 if(FD_ISSET(vecSocketInfo[i].sock, &fdWrite)) >，
即当Socket可写就可以发送信息给客户端了。
发送的根本协议是这样的：先发一个SCommand的结构体过去，这个结构体说明了指令和数据的长度。
然后就根据这个长度接收数据。最后再给客户端做出相应的响应！
    根据这种代码结构，可以很方便的添加新的功能。
错误处理做得不太好，以后再补充了。
其他的如注释，结构，命名等的编码规范都用了个人比较喜欢的方式。
输出：
..BinSelectServer.exe
用法：
直接启动就可以了
Todo:
下一步首先完成各个SOCKET的模型，然后公开自己的研究代码。
功能方面就是：
1、服务器可以指定共享文件夹
2、客户端可以列出服务器共享了哪些文件
3、客户端可以列出哪些用户在线，并可以发命令和其他用户聊天
4、加上界面
