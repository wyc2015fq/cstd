# UDP实现linux与Windows之间数据传输 - xqhrs232的专栏 - CSDN博客
2017年10月31日 14:58:22[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：595
原文地址::[http://blog.csdn.net/heavy_dream/article/details/72460899](http://blog.csdn.net/heavy_dream/article/details/72460899)
相关文章
1、[TCP实现Linux与Windows之间数据传输](http://blog.csdn.net/heavy_dream/article/details/72461971)----[http://blog.csdn.net/heavy_dream/article/details/72461971](http://blog.csdn.net/heavy_dream/article/details/72461971)
2、Socket网络程序设计（4） ———— 实现linux和windows的文件传输----[http://blog.csdn.net/kevinbetterq/article/details/77036547](http://blog.csdn.net/kevinbetterq/article/details/77036547)
实验要求：用两台计算机，一台是服务器另一台是客户端，运行结果要求在客户端把键入的字符串发送给服务器，服务器显示收到的字符串，并将字符串发回给客户端，客户端显示服务器发回的字符串。要求客户端能够多次发送字符串，服务器能够不断的接收并显示和发送。
**Linux是服务器：**
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>/*包含socket()/bind()*/
#include <netinet/in.h>/*包含struct sockaddr_in*/
#include <string.h>/*包含memset()*/
#define PORT_SERV 8888/*服务器端口*/
#define BUFF_LEN 256/*缓冲区大小*/
void static udpserv_echo(int s, struct sockaddr*client)
{
int n;
 /*接收数据长度*/
char buff[BUFF_LEN];/*接收发送缓冲区*/
//char senbuff[1024];
socklen_t len;/*地址长度*/
//memset(&senbuff,0,sizeof(senbuff));
//memset(&buff,0,sizeof(buff));
while(1)
 /*循环等待*/
{
len = sizeof(*client);
n = recvfrom(s, buff, BUFF_LEN, 0, client, &len);
/*接收数据放到buff中，并获得客户端地址*/
write(1,buff,BUFF_LEN);
printf("\n");
sprintf(buff,"%s  this server return",buff);
sendto(s, buff, n, 0, client, len);/*将接收到的n个字节发送回客户端*/
memset(&buff,0,sizeof(buff));
}
}
int main(int argc, char*argv[])
{
int s;
 /*套接字文件描述符*/
struct sockaddr_in addr_serv,addr_clie;/*地址结构*/
s = socket(AF_INET, SOCK_DGRAM, 0);/*建立数据报套接字*/
memset(&addr_serv, 0, sizeof(addr_serv));/*清空地址结构*/
addr_serv.sin_family = AF_INET;/*地址类型为AF_INET*/
addr_serv.sin_addr.s_addr = htonl(INADDR_ANY);/*任意本地地址*/
addr_serv.sin_port = htons(PORT_SERV);/*服务器端口*/
bind(s, (struct sockaddr*)&addr_serv, sizeof(addr_serv));
/*绑定地址*/
udpserv_echo(s, (struct sockaddr*)&addr_clie);/*回显处理程序*/
return 0;
}
**Windows是客户端：**
// UDPClient.cpp : 定义控制台应用程序的入口点。
//
//用UDP协议使用sendto( )函数发送数据报
#include "stdafx.h"
#include <WINSOCK2.H> // 用于管理Windows Sockets版本2函数的头文件
#pragma comment(lib,"WS2_32.lib") 
#include "stdlib.h"
#include "iostream"
using namespace std;
int _tmain(int argc, _TCHAR* argv[])
{
WSADATA wsaData; // WSADATA变量，用于初始化Windows Socket
SOCKET SendSocket; // 发送消息的SOCKET
sockaddr_in RecvAddr; // 服务器端地址
int Port = 8888; // 服务器端侦听端口
char SendBuf[1024]; // 发送数据的缓冲区
int BufLen = 1024; // 发送缓冲区大小
int RecvAddrSize = sizeof(RecvAddr); //jieshouzhe者地址长度
//---------------------------------------------
// 初始化SOCKET
WSAStartup(MAKEWORD(2,2), &wsaData);//MAKEWORD(2,2)生成版本号；指向WSADATA结构体变量指针
//---------------------------------------------
// 创建SOCKET对象
SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
//---------------------------------------------
// 设置服务器地址
RecvAddr.sin_family = AF_INET;  //协议族
RecvAddr.sin_port = htons(Port); //端口
RecvAddr.sin_addr.s_addr = inet_addr("192.168.19.88"); //服务器IP地址
//---------------------------------------------
// 向服务器发送数据报
printf("Sending a datagram to the receiver...\n");
while (1){
//清空缓冲区
memset(&SendBuf, 0, sizeof(SendBuf));
//使用cin向缓冲区输入的语句
//cin>>SendBuf;
cin.getline(SendBuf,1024);
//使用sendto发送数据
sendto(SendSocket, SendBuf, BufLen, 0, (SOCKADDR *) &RecvAddr, sizeof(RecvAddr));
//使用recvfrom接收数据
recvfrom(SendSocket, SendBuf, BufLen, 0, (SOCKADDR *)&RecvAddr, &RecvAddrSize);
//使用cout输出缓冲区的语句
cout << SendBuf << endl;
}
//---------------------------------------------
// 发送完成，关闭SOCKET
printf("Finished sending. Closing socket.\n");
closesocket(SendSocket);
//---------------------------------------------
// 释放资源，并退出
printf("Exiting.\n");
WSACleanup();
system("pause");
return 0;
}
**运行结果：**
**Linux服务器：**
**Windows客户端：**
****************************************************************************************************************************************************************************************
**我们可以尝试下Linux做客户端，Windows做服务器测试下，结果在怎么样。**
**Linux客户端：**
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>/*包含socket()/bind()*/
#include <netinet/in.h>/*包含struct sockaddr_in*/
#include <string.h>/*包含memset()*/
#define PORT_SERV 8888/*服务器端口*/
#define BUFF_LEN 256/*缓冲区大小*/
static void udpclie_echo(int s, struct sockaddr*to)
{
char buff[BUFF_LEN];/*发送给服务器的测试数据05*/
char recvbuff[1024];
struct sockaddr_in from;/*服务器地址*/
socklen_t len = sizeof(*to);/*地址长度*/
memset(&buff, 0, sizeof(buff));
while(1){
scanf("%s",buff);
sendto(s, buff, BUFF_LEN, 0, to, len);/*发送给服务器*/
recvfrom(s, buff, BUFF_LEN, 0, (struct sockaddr*)&from, &len);
//sprintf(recvbuff,"%s this server return",buff);/*从服务器接收数据*/
printf("recved:%s\n",buff);/*打印数据*/
}
}
int main(int argc, char*argv[])
{
int s;
 /*套接字文件描述符*/
struct sockaddr_in addr_serv;/*地址结构*/
s = socket(AF_INET, SOCK_DGRAM, 0);/*建立数据报套接字*/
memset(&addr_serv, 0, sizeof(addr_serv));/*清空地址结构*/
addr_serv.sin_family = AF_INET;/*地址类型为AF_INET*/
/*addr_serv.sin_addr.s_addr = htonl(INADDR_ANY);任意本地地址*/
//addr_serv.sin_addr.s_addr = inet_pton("192.168.19.152");
 addr_serv.sin_addr.s_addr = inet_addr("192.168.19.152");
addr_serv.sin_port = htons(PORT_SERV);/*服务器端口*/
udpclie_echo(s, (struct sockaddr*)&addr_serv);/*客户端回显程序*/
close(s);
return 0;
}
**Windows服务器：**
// UDPServer.cpp : 定义控制台应用程序的入口点。
//
//用UDP协议使用recvfrom( )函数接收数据报
#include "stdafx.h"
#include <stdio.h>
#include "winsock2.h"
#include "stdlib.h"
#pragma comment(lib,"WS2_32.lib") 
#include "iostream"
using namespace std;
int _tmain(int argc, _TCHAR* argv[])
{
WSADATA wsaData; // WSADATA变量，用于初始化Windows Socket
SOCKET RecvSocket; // 接收消息的SOCKET 
SOCKET SendSocket; // 发送消息的SOCKET 
sockaddr_in RecvAddr; // 服务器端地址
int Port = 8888; // 服务器端侦听地址
char RecvBuf[1024]; // 接收数据的缓冲区
int BufLen = 1024; // 接收缓冲区大小
sockaddr_in SenderAddr; // 发送者的地址
int SenderAddrSize = sizeof(SenderAddr); //发送者地址长度
//-----------------------------------------------
// 初始化Winsock
WSAStartup(MAKEWORD(2,2), &wsaData);
//-----------------------------------------------
// 创建接收数据报的SOCKET
RecvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
//创建发送数据报的套接字
SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
// 将SOCKET与指定端口和0.0.0.0绑定
bind(SendSocket, (SOCKADDR *)&RecvAddr, sizeof(RecvAddr));
//-----------------------------------------------
// 将SOCKET与指定端口和0.0.0.0绑定
RecvAddr.sin_family = AF_INET;
RecvAddr.sin_port = htons(Port);
RecvAddr.sin_addr.s_addr = htonl(INADDR_ANY); 
bind(RecvSocket, (SOCKADDR *) &RecvAddr, sizeof(RecvAddr));
//-----------------------------------------------
// 调用recvfrom()函数在绑定的SOCKET上接收数据
printf("Receiving datagrams...\n");
while (1)
{
//接收客户端发过来的数据
recvfrom(RecvSocket, RecvBuf, BufLen, 0, (SOCKADDR *)&SenderAddr, &SenderAddrSize);
//sprintf(RecvBuf, "%s this server return", RecvBuf);
//把数据返回给客服端
sendto(SendSocket, RecvBuf, BufLen, 0, (SOCKADDR *)&SenderAddr, SenderAddrSize);
//使用cout输出缓冲区
cout<<RecvBuf<<endl;
//清空缓冲区
memset(&RecvBuf,0,sizeof(RecvBuf));
}
//-----------------------------------------------
// 关闭SOCKET，结束接收数据
printf("Finished receiving. Closing socket.\n");
closesocket(RecvSocket);
//-----------------------------------------------
// 释放资源，退出
printf("Exiting.\n");
WSACleanup();
system("pause");
return 0;
}
**运行结果：**
**Linux客户端：**
**Windows服务器：**

