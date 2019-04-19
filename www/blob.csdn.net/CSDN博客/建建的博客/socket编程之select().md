# socket编程之select() - 建建的博客 - CSDN博客
2017年01月24日 11:15:31[纪建](https://me.csdn.net/u013898698)阅读数：189
个人分类：[Socket学习](https://blog.csdn.net/u013898698/article/category/6694075)

Select在Socket编程中还是比较重要的，它能够监视我们需要监视的文件描述符的变化情况——读写或是异常。 
int select(int maxfdp,fd_set *readfds,fd_set *writefds,fd_set *errorfds,struct timeval *timeout);
/*参数列表
int maxfdp是一个整数值，是指集合中所有文件描述符的范围，即所有文件描述符的最大值加1，不能错！在Windows中这个参数的值无所谓，
可以设置不正确。 
fd_set *readfds是指向fd_set结构的指针，这个集合中应该包括文件描述符，我们是要监视这些文件描述符的读变化的，即我们关心是否可
以从这些文件中读取数据了，如果这个集合中有一个文件可读，select就会返回一个大于0的值，表示有文件可读，如果没有可读的文件，则
根据timeout参数再判断是否超时，若超出timeout的时间，select返回0，若发生错误返回负值。可以传入NULL值，表示不关心任何文件的读
变化。 
fd_set *writefds是指向fd_set结构的指针，这个集合中应该包括文件描述符，我们是要监视这些文件描述符的写变化的，即我们关心是否可
以向这些文件中写入数据了，如果这个集合中有一个文件可写，select就会返回一个大于0的值，表示有文件可写，如果没有可写的文件，则
根据timeout参数再判断是否超时，若超出timeout的时间，select返回0，若发生错误返回负值。可以传入NULL值，表示不关心任何文件的写
变化。 
fd_set *errorfds同上面两个参数的意图，用来监视文件错误异常。 
struct timeval* timeout是select的超时时间，这个参数至关重要，它可以使select处于三种状态：
第一，若将NULL以形参传入，即不传入时间结构，就是将select置于阻塞状态，一定等到监视文件描述符集合中某个文件描述符发生变化为止
；
第二，若将时间值设为0秒0毫秒，就变成一个纯粹的非阻塞函数，不管文件描述符是否有变化，都立刻返回继续执行，文件无变化返回0，有
变化返回一个正值；
第三，timeout的值大于0，这就是等待的超时时间，即 select在timeout时间内阻塞，超时时间之内有事件到来就返回了，否则在超时后不管
怎样一定返回，返回值同上述。
*/
/*
返回值： 
负值：select错误
正值：某些文件可读写或出错
0：等待超时，没有可读写或错误的文件
*/
清空集合 FD_ZERO(fd_set *)
将一个给定的文件描述符加入集合之中FD_SET(int ,fd_set *)
将一个给定的文件描述符从集合中删除FD_CLR(int ,fd_set*)
检查集合中指定的文件描述符是否可以读写FD_ISSET(int ,fd_set* )
#include <winsock.h> 
     #include <stdio.h> 
     #define PORT       5150 
     #define MSGSIZE     1024 
     #pragma comment(lib, "ws2_32.lib") 
     int     g_iTotalConn = 0; 
     SOCKET g_CliSocketArr[FD_SETSIZE]; 
     DWORD WINAPI WorkerThread(LPVOID lpParameter); 
     int main() 
     {   
         WSADATA     wsaData;   
         SOCKET       sListen, sClient;   
         SOCKADDR_IN local, client;   
         int         iaddrSize = sizeof(SOCKADDR_IN);   
         DWORD       dwThreadId;   
         // Initialize Windows socket library   
         WSAStartup(0x0202, &wsaData);   
         // Create listening socket   
         sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);   
         // Bind           
         local.sin_addr.S_un.S_addr = htonl(INADDR_ANY); 
         local.sin_family = AF_INET; 
         local.sin_port = htons(PORT);   
         bind(sListen, (struct sockaddr *)&local, sizeof(SOCKADDR_IN));   
         // Listen   listen(sListen, 3);   
         // Create worker thread   
         CreateThread(NULL, 0, WorkerThread, NULL, 0, &dwThreadId);     
         while (TRUE)   
         {               // Accept a connection     
             sClient = accept(sListen, (struct sockaddr *)&client, &iaddrSize);     
             printf("Accepted client:%s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));     
             // Add socket to g_CliSocketArr     
             g_CliSocketArr[g_iTotalConn++] = sClient;   
         }     
         return 0; 
     } 
     DWORD WINAPI WorkerThread(LPVOID lpParam) 
     {   
         int             i;   
         fd_set         fdread;   
         int             ret;   
         struct timeval tv = {1, 0};   
         char           szMessage[MSGSIZE];     
         while (TRUE)   
         {     
             FD_ZERO(&fdread);     
             for (i = 0; i < g_iTotalConn; i++) 
             { 
                 FD_SET(g_CliSocketArr, &fdread); 
             }                     // We only care read event 
             ret = select(0, &fdread, NULL, NULL, &tv); 
             if (ret == 0) 
             {       // Time expired 
                 continue; 
             } 
             for (i = 0; i < g_iTotalConn; i++) 
             { 
                 if (FD_ISSET(g_CliSocketArr, &fdread)) 
                   {         // A read event happened on g_CliSocketArr 
                       ret = recv(g_CliSocketArr, szMessage, MSGSIZE, 0); 
                       if (ret == 0 || (ret == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET)) 
                         { 
                             // Client socket closed           
                             printf("Client socket %d closed.\n", g_CliSocketArr); 
                             closesocket(g_CliSocketArr); 
                             if (i < g_iTotalConn - 1) 
                             { 
                                 g_CliSocketArr[i--] = g_CliSocketArr[--g_iTotalConn]; 
                             } 
                         } 
                         else 
                         { 
                               // We received a message from client 
                               szMessage[ret] = '\0'; 
                               send(g_CliSocketArr, szMessage, strlen(szMessage), 0); 
                         } 
                   } //if 
             }//for 
         }//while     
         return 0; 
     } 
服务器的几个主要动作如下： 
     1.创建监听套接字，绑定，监听； 
     2.创建工作者线程； 
     3.创建一个套接字数组，用来存放当前所有活动的客户端套接字，每accept一个连接就更新一次数组； 
     4.接受客户端的连接。 
     这里有一点需要注意的，就是我没有重新定义FD_SETSIZE宏，所以服务器最多支持的并发连接数为64。而且，这里决不能无条件的
accept,服务器应该根据当前的连接数来决定是否接受来自某个客户端的连接。一种比较好的实现方案就是采用WSAAccept函数，而且让
WSAAccept回调自己实现的Condition Function。
int CALLBACK ConditionFunc(LPWSABUF lpCallerId,LPWSABUF lpCallerData, LPQOS lpSQOS,LPQOS lpGQOS,LPWSABUF lpCalleeId, 
LPWSABUF lpCalleeData,GROUP FAR * 
g,DWORD dwCallbackData) 
{ 
　　if (当前连接数 < FD_SETSIZE) 
　　　　return CF_ACCEPT; 
　　else   
　　　　return CF_REJECT; 
} 
根据当前的客户端数量判断是不是允许建立连接
除了需要有条件接受客户端的连接外，还需要在连接数为0的情形下做特殊处理，因为如果读集中没有任何套接字，select函数会立刻返
回，这将导致工作者线程成为一个毫无停顿的死循环，CPU的占用率马上达到100%
以上参考网上资料编写

