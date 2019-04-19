# Select模型原理 - xqhrs232的专栏 - CSDN博客
2016年04月20日 18:09:09[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：341
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://blog.csdn.net/yanheifeng715000/article/details/6592447](http://blog.csdn.net/yanheifeng715000/article/details/6592447)
相关文章
1、[Select模型](http://blog.chinaunix.net/uid-21783276-id-2689035.html) ----[http://blog.chinaunix.net/uid-21783276-id-2689035.html](http://blog.chinaunix.net/uid-21783276-id-2689035.html)
2、[select 模型详解](http://blog.csdn.net/wangjieest/article/details/7043472)----[http://blog.csdn.net/wangjieest/article/details/7043472](http://blog.csdn.net/wangjieest/article/details/7043472)
Select模型原理
利用select函数，判断套接字上是否存在数据，或者能否向一个套接字写入数据。目的是防止应用程序在套接字处于锁定模式时，调用recv（或send）从没有数据的套接字上接收数据，被迫进入阻塞状态。
select参数和返回值意义如下：
int **select** (
 IN **int** nfds,                           //0,无意义
 IN OUT **fd_set*** readfds,      //检查可读性
 IN OUT** fd_set*** writefds,     //检查可写性
 IN OUT f**d_set* **exceptfds,  //例外数据
 IN const** struct timeval*** timeout);    //函数的返回时间
struct  **timeval** {
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
**[cpp]**[view
 plain](http://blog.csdn.net/yanheifeng715000/article/details/6592447#)[copy](http://blog.csdn.net/yanheifeng715000/article/details/6592447#)
[print](http://blog.csdn.net/yanheifeng715000/article/details/6592447#)[?](http://blog.csdn.net/yanheifeng715000/article/details/6592447#)
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
