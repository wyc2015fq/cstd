# 利用Windows sockets实现TCP通信示例程序 - xqhrs232的专栏 - CSDN博客
2013年06月28日 10:21:17[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：782
原文地址::[http://java-mans.iteye.com/blog/1648808](http://java-mans.iteye.com/blog/1648808)
参考《精通Windows Sockets网络开发--基于Visual C++实现》
[源代码下载链接](http://download.csdn.net/detail/whucv/4514307)
该程序由客户端和服务器端组成，实现的功能是客户端向服务器发送“MyTcp”字符串，服务器接收到该字符串，并显示出来。
客户端发送字符串后退出，服务器显示字符串后退出。
服务器实现，也即当前的TcpExample
# 1、新建console程序TcpExample。A simple Application，
添加库文件ws2_32.lib为2.2版，1.1版socket为wsock32.lib，
![](https://img-my.csdn.net/uploads/201208/21/1345486071_8749.png)
Cpp代码 ![复制代码](http://java-mans.iteye.com/images/icon_copy.gif)![收藏代码](http://java-mans.iteye.com/images/icon_star.png)![](http://java-mans.iteye.com/images/spinner.gif)
- #include <stdio.h> 
- #include <WINSOCK2.H> 
- #define BUF_SIZE 64          //缓冲区大小 
- **char** buf[BUF_SIZE];           //接收数据缓冲区
- SOCKET sClient;  
```cpp
#include <stdio.h>
#include <WINSOCK2.H>
#define BUF_SIZE 64          //缓冲区大小
char buf[BUF_SIZE];           //接收数据缓冲区
SOCKET sClient;
```
# 2、WSAStartUp()初始化套接字动态库
Cpp代码 ![复制代码](http://java-mans.iteye.com/images/icon_copy.gif)![收藏代码](http://java-mans.iteye.com/images/icon_star.png)![](http://java-mans.iteye.com/images/spinner.gif)
- WSADATA wsd;    //WSADATA变量 
- 
- **int** retVal;     //返回值
- if (WSAStartup(MAKEWORD(2,2),&wsd) != 0)   
-     {   
-         printf("WSAStartup 失败！\n");   
- return 1;   
-     }  
```cpp
WSADATA wsd;	//WSADATA变量
	
	int retVal;		//返回值
	if (WSAStartup(MAKEWORD(2,2),&wsd) != 0)
	{
		printf("WSAStartup 失败！\n");
		return 1;
	}
```
# 3、socket()创建套接字
//创建套接字
Cpp代码 ![复制代码](http://java-mans.iteye.com/images/icon_copy.gif)![收藏代码](http://java-mans.iteye.com/images/icon_star.png)![](http://java-mans.iteye.com/images/spinner.gif)
- //创建套接字 
-     SOCKET sServer; //服务器套接字 
-     sServer=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);   
- if (INVALID_SOCKET == sServer)   
-     {   
-         printf("创建套接字失败！\n");   
-         WSACleanup();//释放套接字资源 
- return -1;   
-     }  
```cpp
//创建套接字
	SOCKET sServer;	//服务器套接字
	sServer=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (INVALID_SOCKET == sServer)
	{
		printf("创建套接字失败！\n");
		WSACleanup();//释放套接字资源
		return -1;
	}
```
# 4、绑定套接字
Cpp代码 ![复制代码](http://java-mans.iteye.com/images/icon_copy.gif)![收藏代码](http://java-mans.iteye.com/images/icon_star.png)![](http://java-mans.iteye.com/images/spinner.gif)
- //首先定义服务地址 
- SOCKADDR_IN addrServ;   
- //服务器套接字地址 
-        addrServ.sin_family=AF_INET;   
-        addrServ.sin_port=htons(4999);   
- /*    addrServ.sin_addr.S_addr=INADDR_ANY;
-        addrServ.sin_addr.S_addr=INADDR_ANY; 
- 原书错误见winsock2.h，in_addr只有一个联合体类型的S_un成员变量 
-  *    struct in_addr { 
-        union { 
-                      struct{ u_char s_b1,s_b2,s_b3,s_b4; } S_un_b;
-                      struct{ u_short s_w1,s_w2; } S_un_w;
-                      u_longS_addr; 
-        } S_un; 
-        */
-        addrServ.sin_addr.S_un.S_addr=INADDR_ANY;   
- //绑定套接字 
-        retVal=bind(sServer,(LPSOCKADDR)&addrServ,sizeof(SOCKADDR_IN));   
- if(SOCKET_ERROR == retVal)   
-        {   
-               printf("绑定失败！\n");   
-               closesocket(sServer);//关闭套接字
-               WSACleanup();//释放套接字资源 
- return-1;   
-        }  
```cpp
//首先定义服务地址
SOCKADDR_IN addrServ;
       //服务器套接字地址
       addrServ.sin_family=AF_INET;
       addrServ.sin_port=htons(4999);
/*    addrServ.sin_addr.S_addr=INADDR_ANY;
       addrServ.sin_addr.S_addr=INADDR_ANY;
原书错误见winsock2.h，in_addr只有一个联合体类型的S_un成员变量
 *    struct in_addr {
       union {
                     struct{ u_char s_b1,s_b2,s_b3,s_b4; } S_un_b;
                     struct{ u_short s_w1,s_w2; } S_un_w;
                     u_longS_addr;
       } S_un;
       */
       addrServ.sin_addr.S_un.S_addr=INADDR_ANY;
       //绑定套接字
       retVal=bind(sServer,(LPSOCKADDR)&addrServ,sizeof(SOCKADDR_IN));
       if(SOCKET_ERROR == retVal)
       {
              printf("绑定失败！\n");
              closesocket(sServer);//关闭套接字
              WSACleanup();//释放套接字资源
              return-1;
       }
```
# 5、监听
Cpp代码 ![复制代码](http://java-mans.iteye.com/images/icon_copy.gif)![收藏代码](http://java-mans.iteye.com/images/icon_star.png)![](http://java-mans.iteye.com/images/spinner.gif)
- //开始监听,设置等待接受连接的队列长度为1 
-        retVal=listen(sServer,1);   
- if(SOCKET_ERROR == retVal)   
-        {   
-               printf("监听失败！\n");   
-               closesocket(sServer);   
-               WSACleanup();   
- return-1;   
-        }  
```cpp
//开始监听,设置等待接受连接的队列长度为1
       retVal=listen(sServer,1);
       if(SOCKET_ERROR == retVal)
       {
              printf("监听失败！\n");
              closesocket(sServer);
              WSACleanup();
              return-1;
       }
```
# 6、接受客户端连接
Cpp代码 ![复制代码](http://java-mans.iteye.com/images/icon_copy.gif)![收藏代码](http://java-mans.iteye.com/images/icon_star.png)![](http://java-mans.iteye.com/images/spinner.gif)
- //accept函数处于阻塞状态，直到接受了一个客户端请求。当该函数返回时，新建一个套接字，同时返回该客户端的地址
- //如果调用失败，则返回INVALID_SOCKET 
-       sockaddr_inaddrClient;   
-       intaddrClientlen =sizeof(addrClient);   
-       sClient=accept(sServer,(sockaddrFAR*)&addrClient,&addrClientlen);   
- if(INVALID_SOCKET == sClient)   
-       {   
-              printf("接受客户端连接失败！\n");   
-              closesocket(sServer);   
-              WSACleanup();   
- return-1;   
-       }  
```cpp
//accept函数处于阻塞状态，直到接受了一个客户端请求。当该函数返回时，新建一个套接字，同时返回该客户端的地址
       //如果调用失败，则返回INVALID_SOCKET
       sockaddr_inaddrClient;
       intaddrClientlen =sizeof(addrClient);
       sClient=accept(sServer,(sockaddrFAR*)&addrClient,&addrClientlen);
       if(INVALID_SOCKET == sClient)
       {
              printf("接受客户端连接失败！\n");
              closesocket(sServer);
              WSACleanup();
              return-1;
       }
```
# 7、接受客户端数据
Cpp代码 ![复制代码](http://java-mans.iteye.com/images/icon_copy.gif)![收藏代码](http://java-mans.iteye.com/images/icon_star.png)![](http://java-mans.iteye.com/images/spinner.gif)
- //接收并显示数据 
- //调用recv() 函数接收客户端发送的数据。在调用该函数前首先调用ZeroMemory()函数将接收数据缓冲区设置为0，最后调用printf显示数据
-        charbuf[128];   
-        intBUF_SIZE=128;   
-        ZeroMemory(buf,BUF_SIZE);   
-        retVal=recv(sClient,buf,BUF_SIZE,0);   
- if(SOCKET_ERROR ==retVal)   
-        {   
-               printf("接收数据失败！\n");   
-               closesocket(sServer);   
-               closesocket(sClient);   
-               WSACleanup();   
- return-1;   
-        }   
-        printf("%s\n",buf);//输出数据
- 
```cpp
//接收并显示数据
       //调用recv() 函数接收客户端发送的数据。在调用该函数前首先调用ZeroMemory()函数将接收数据缓冲区设置为0，最后调用printf显示数据
       charbuf[128];
       intBUF_SIZE=128;
       ZeroMemory(buf,BUF_SIZE);
       retVal=recv(sClient,buf,BUF_SIZE,0);
       if(SOCKET_ERROR ==retVal)
       {
              printf("接收数据失败！\n");
              closesocket(sServer);
              closesocket(sClient);
              WSACleanup();
              return-1;
       }
       printf("%s\n",buf);//输出数据
```
# 8、退出
Cpp代码 ![复制代码](http://java-mans.iteye.com/images/icon_copy.gif)![收藏代码](http://java-mans.iteye.com/images/icon_star.png)![](http://java-mans.iteye.com/images/spinner.gif)
- closesocket(sServer);   
-      closesocket(sClient);   
-      WSACleanup();  
```cpp
closesocket(sServer);
       closesocket(sClient);
       WSACleanup();
```
实现客户端
客户端主要功能是连接服务器，向服务器发送“MyTcp”字符串。客户端的实现包括
初始化套接字动态库、创建套接字、连接服务器、发送数据和退出。
1、console程序TcpClient，选中Add to current workspace，A simple Application ,
![](https://img-my.csdn.net/uploads/201208/21/1345486422_3205.png)
2、#include <stdio.h>,添加库文件
3、定义全局变量
Cpp代码 ![复制代码](http://java-mans.iteye.com/images/icon_copy.gif)![收藏代码](http://java-mans.iteye.com/images/icon_star.png)![](http://java-mans.iteye.com/images/spinner.gif)
- #include <WINSOCK2.H> 
- #define BUF_SIZE 64          //缓冲区大小 
- WSADATA wsd;                  //WSADATA变量 
- SOCKET sHost;                   //服务器套接字 
- SOCKADDR_IN servAddr;   //服务器地址 
- **char** buf[BUF_SIZE];           //接收数据缓冲区
- **int** retval;                            //返回值
```cpp
#include <WINSOCK2.H>
#define BUF_SIZE 64          //缓冲区大小
WSADATA wsd;                  //WSADATA变量
SOCKET sHost;                   //服务器套接字
SOCKADDR_IN servAddr;   //服务器地址
char buf[BUF_SIZE];           //接收数据缓冲区
int retval;                            //返回值
```
4、初始化套接字动态库
Cpp代码 ![复制代码](http://java-mans.iteye.com/images/icon_copy.gif)![收藏代码](http://java-mans.iteye.com/images/icon_star.png)![](http://java-mans.iteye.com/images/spinner.gif)
- //初始化套接字动态库 
- if(WSAStartup(MAKEWORD(2,2),(LPWSADATA)&wsd)!=0)   
-        {   
-               printf("WSAStartup失败！\n");   
- return-1;   
-        }  
```cpp
//初始化套接字动态库
       if(WSAStartup(MAKEWORD(2,2),(LPWSADATA)&wsd)!=0)
       {
              printf("WSAStartup失败！\n");
              return-1;
       }
```
5、创建套接字
Cpp代码 ![复制代码](http://java-mans.iteye.com/images/icon_copy.gif)![收藏代码](http://java-mans.iteye.com/images/icon_star.png)![](http://java-mans.iteye.com/images/spinner.gif)
- //创建套接字 
-        sHost=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);   
- if(INVALID_SOCKET == sHost)   
-        {   
-               printf("创建套接字失败！\n");   
-               WSACleanup();   
- return-1;   
-        }  
```cpp
//创建套接字
       sHost=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
       if(INVALID_SOCKET == sHost)
       {
              printf("创建套接字失败！\n");
              WSACleanup();
              return-1;
       }
```
6、连接服务器
设置IP地址为127.0.0.1，服务器端口为4999.调用connect() 函数连接服务器。
Cpp代码 ![复制代码](http://java-mans.iteye.com/images/icon_copy.gif)![收藏代码](http://java-mans.iteye.com/images/icon_star.png)![](http://java-mans.iteye.com/images/spinner.gif)
- //设置服务器地址 
-        servAddr.sin_family=AF_INET;   
-        servAddr.sin_port=htons((**short**)4999);   
-        servAddr.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");   
-        intnServAddlen=sizeof(servAddr);   
- //连接服务器 
-        retval=connect(sHost,(LPSOCKADDR)&servAddr,sizeof(servAddr));   
- if(SOCKET_ERROR == retval)   
-        {   
-               printf("连接服务器失败！ \n");   
-               closesocket(sHost);   
-               WSACleanup();   
-        }  
```cpp
//设置服务器地址
       servAddr.sin_family=AF_INET;
       servAddr.sin_port=htons((short)4999);
       servAddr.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");
       intnServAddlen=sizeof(servAddr);
       //连接服务器
       retval=connect(sHost,(LPSOCKADDR)&servAddr,sizeof(servAddr));
       if(SOCKET_ERROR == retval)
       {
              printf("连接服务器失败！ \n");
              closesocket(sHost);
              WSACleanup();
       }
```
7、发送数据
Cpp代码 ![复制代码](http://java-mans.iteye.com/images/icon_copy.gif)![收藏代码](http://java-mans.iteye.com/images/icon_star.png)![](http://java-mans.iteye.com/images/spinner.gif)
- //首先利用ZeroMemory设置发送数据缓冲区为0 
-       ZeroMemory(buf,BUF_SIZE);   
-       strcpy(buf,"MyTcp");   
-       retval=send(sHost,buf,strlen(buf),0);   
- if(SOCKET_ERROR == retval)   
-       {   
-              printf("向服务器发送数据失败！ \n");   
-              closesocket(sHost);   
-              WSACleanup();   
- return-1;   
-       }  
```cpp
//首先利用ZeroMemory设置发送数据缓冲区为0
       ZeroMemory(buf,BUF_SIZE);
       strcpy(buf,"MyTcp");
       retval=send(sHost,buf,strlen(buf),0);
       if(SOCKET_ERROR == retval)
       {
              printf("向服务器发送数据失败！ \n");
              closesocket(sHost);
              WSACleanup();
              return-1;
       }
```
8、退出
Cpp代码 ![复制代码](http://java-mans.iteye.com/images/icon_copy.gif)![收藏代码](http://java-mans.iteye.com/images/icon_star.png)![](http://java-mans.iteye.com/images/spinner.gif)
- closesocket(sHost);   
-       WSACleanup();  
```cpp
closesocket(sHost);
       WSACleanup();
```
先运行TcpExample，再运行TcpClient。
![](https://img-my.csdn.net/uploads/201208/21/1345486539_9944.png)
