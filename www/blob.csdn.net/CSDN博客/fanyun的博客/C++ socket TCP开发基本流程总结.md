# C++ socket TCP开发基本流程总结 - fanyun的博客 - CSDN博客
2016年09月30日 09:04:30[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：4196
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
Sockets编程有三种：
(1).流步套接字（SOCK_STREAM）；
(2).数据报套接字（SOCK_DGRAM）；
(3).原始套接字（SOCK_RAW）；
而基于TCP的socket编程是采用的流步套接字，具体编程步骤如下：
**服务器端编程的步骤：**
1：加载套接字库，创建套接字(WSAStartup()/socket())；
2：绑定套接字到一个IP地址和一个端口上(bind())；
3：将套接字设置为监听模步等待连接请求(listen())；
4：请求到来后，接受连接请求，返回一个新的对应于此次连接的套接字(accept())；
5：用返回的套接字和客户端进行通信(send()/recv())；
6：返回，等待另一连接请求；
7：关闭套接字，关闭加载的套接字库(closesocket()/WSACleanup())。
**客户端编程的步骤**：
1：加载套接字库，创建套接字(WSAStartup()/socket())；
2：向服务器发出连接请求(connect())；
3：和服务器端进行通信(send()/recv())；
4：关闭套接字，关闭加载的套接字库(closesocket()/WSACleanup())。
**第一步: 加载/释放Winsock库:**
1.加载方法:
WSADATA wsa;
/*初始化socket资源*/
if(WSAStartup(MAKEWORD(1,1),&wsa) != 0)
{
   return;   //代表失败
}
2.释放方法:
WSACleanup();
**第二步: 构造SOCKET:**
1.服务端:构造监听SOCKET,流步SOCKET.
   SOCKET** Listen_Sock **=socket(AF_INET, SOCK_STREAM, 0)
2.客户端:构造通讯SOCKET,流步SOCKET.
   SOCKET** Client_Sock **= socket(AF_INET, SOCK_STREAM, 0)
**第三步: 配置监听地址和端口:**
1.服务端: SOCKADDR_IN **serverAddr**ZeroMemory((char *)&**serverAddr**,sizeof(**serverAddr**));
**serverAddr**.sin_family = **AF_INET**;
**serverAddr**.sin_port = htons(**1234**);          /*本地监听端口:1234*/
**serverAddr**.sin_addr.s_addr = htonl(**INADDR_ANY**);/*有IP*/
**第四步:   绑定SOCKET:**
1.服务端:绑定监听SOCKET.
bind(**Listen_Sock,**(struct sockaddr *)&**serverAddr,**sizeof(**serverAddr**))
**第五步: 服务端/客户端连接:**
1.服务端:等待客户端接入.
  SOCKET** Command_Sock **= accept(**Listen_Sock**,...)
2.客户端:请求与服务端连接.
int ret = connect(**Client_Sock**, ...)
**第六步: 收/发数据:**
1.服务端:等待客户端接入.charbuf[1024].
    接收数据:recv(**Command_Sock**,buf,...)
或
    发送数据:send(**Command_Sock**,buf,...)
2.客户端:请求与服务端连接.charbuf[1024].
    发送数据:send(**Client_Sock**,buf,...)
或
    接收数据:recv(**Client_Sock**,buf,...)
**第七步: 关闭SOCKET:**
1.服务端:关闭SOCKET.
   closesocket(**Listen_Sock**)
   closesocket(**Command_Sock**)
2.客户端:关闭SOCKET.
   closesocket(**Client_Sock**)
**服务端：**
```cpp
#include <WINSOCK2.H>
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib")
void main()
{
 //创建套接字
 WORD myVersionRequest;
 WSADATA wsaData;
 myVersionRequest=MAKEWORD(1,1);
 int err;
 err=WSAStartup(myVersionRequest,&wsaData);
 if (!err)
 {
  printf("已打开套接字\n");
 } 
 else
 {
  //进一步绑定套接字
  printf("嵌套字未打开!");
  return;
 }
 SOCKET serSocket=socket(AF_INET,SOCK_STREAM,0);//创建了可识别套接字
 //需要绑定的参数
 SOCKADDR_IN addr;
 addr.sin_family=AF_INET;
 //addr.sin_addr.S_un.S_addr=htonl(INADDR_ANY);//ip地址
  addrSrv.sin_addr.S_un.S_addr=inet_addr("192.168.245.221");
 addr.sin_port=htons(6000);//绑定端口
 bind(serSocket,(SOCKADDR*)&addr,sizeof(SOCKADDR));//绑定完成
 listen(serSocket,5);//其中第二个参数代表能够接收的最多的连接数
 //开始进行监听
 SOCKADDR_IN clientsocket;
 int len=sizeof(SOCKADDR);
 while (1)
 {
  SOCKET serConn=accept(serSocket,(SOCKADDR*)&clientsocket,&len);//如果这里不是accept而是conection的话，就会不断的监听
  char sendBuf[100];
   
  sprintf(sendBuf,"welcome %s to bejing",inet_ntoa(clientsocket.sin_addr));//找对对应的IP并且将这行字打印到那里
  send(serConn,sendBuf,strlen(sendBuf)+1,0);
  char receiveBuf[100];//接收
  recv(serConn,receiveBuf,strlen(receiveBuf)+1,0);
  printf("%s\n",receiveBuf);
  closesocket(serConn);//关闭
 WSACleanup();//释放资源的操作
 }
}
```
**客户端：**
```cpp
#include <WINSOCK2.H>
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib")
void main()
{
 int err;
 WORD versionRequired;
 WSADATA wsaData;
 versionRequired=MAKEWORD(1,1);
 err=WSAStartup(versionRequired,&wsaData);//协议库的版本信息
 if (!err)
 {
  printf("客户端嵌套字已经打开!\n");
 }
 else
 {
  printf("客户端的嵌套字打开失败!\n");
  return;//结束
 }
 SOCKET clientSocket=socket(AF_INET,SOCK_STREAM,0);
 SOCKADDR_IN clientsock_in;
 clientsock_in.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");
 clientsock_in.sin_family=AF_INET;
 clientsock_in.sin_port=htons(6000);
 //bind(clientSocket,(SOCKADDR*)&clientsock_in,strlen(SOCKADDR));//注意第三个参数
 //listen(clientSocket,5);
 connect(clientSocket,(SOCKADDR*)&clientsock_in,sizeof(SOCKADDR));//开始连接
 char receiveBuf[100];
 recv(clientSocket,receiveBuf,101,0);
 printf("%s\n",receiveBuf);
 send(clientSocket,"hello,this is client",strlen("hello,this is client")+1,0);
 closesocket(clientSocket);
 WSACleanup();
}
```

