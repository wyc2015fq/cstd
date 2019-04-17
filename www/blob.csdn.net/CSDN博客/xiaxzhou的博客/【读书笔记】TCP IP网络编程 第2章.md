# 【读书笔记】TCP/IP网络编程 第2章 - xiaxzhou的博客 - CSDN博客





2017年08月07日 22:50:06[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：133








第2.3节：windows平台下实现

函数缩写解释：

inet_addr() 

inet_ntoa() 

inet_aton()
> 
**a : ASCII**
**n : network**


htons() 

htonl() 

ntohs() 

ntohl()
> 
**h : host **
**n :  net   **
**s : short   **
**l : long**
**INADDR_ANY** ：自动获取IP地址，若同一计算机有多个IP，只要端口号一致，可以从不同IP地址接收数据

服务端代码：

```cpp
#include "stdafx.h"
#include <winsock2.h>
#include <stdio.h>
#include <iostream>
using namespace::std;

void main()
{
    //加载套接字库
    WSADATA wsaData;
    SOCKET hServSock, hClntSock;
    SOCKADDR_IN servAddr, clntAddr;
    int szClntAddr;
    char message[] = "Hello World!";

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)//加载库并获取库信息填至wsaData
    {
        cout << "socket start error!" << endl;
        return;
    }

    hServSock = socket(PF_INET, SOCK_STREAM, 0);//流式套接字,IPV4:TCP

    if (hServSock == INVALID_SOCKET)
    {
        cout << "socket create error!" << endl;
        return;
    }

    //设置地址端口
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;//Address Family 地址族
    //servAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); //"inet": internet ; "n" : network   ;"a" : ASCII    本地回路IP地址  字节顺序转换
    servAddr.sin_addr.S_un.S_addr = INADDR_ANY;
    //INADDR_ANY 自动获取IP地址，若同一计算机有多个IP，只要端口号一致，可以从不同IP地址接收数据
    servAddr.sin_port = htons(6000);//h:host   n: net   s:short   l:long >>  host to net short  与服务器端口一致   字节顺序转换

    //套接字绑定地址端口
    //第二个参数期望得到：SOCKADDR  但直接填写SOCKADDR比较麻烦,通过SOCKADDR_IN保存IPv4地址信息
    if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
    {
        cout << "socket bind error!" << endl;
        return;
    }

    if (listen(hServSock, 5) == SOCKET_ERROR)//设置为监听模式，准备接收客户请求，等待连接队列的最大长度为5
    {
        cout << "socket listen error!" << endl;
        return;
    }
    szClntAddr = sizeof(clntAddr);
    hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);
    if (hClntSock==INVALID_SOCKET)
    {
        cout << "socket accept error!" << endl;
        return;
    }

    send(hClntSock, message, sizeof(message), 0);
    closesocket(hClntSock);
    closesocket(hServSock);
    WSACleanup();
    return ;
}
```

客户端代码：

```cpp
// TcpClient.cpp : 定义控制台应用程序的入口点。
#include "stdafx.h"
#include <winsock2.h>
#include <stdio.h>
#include <iostream>
using namespace::std;


void main()
{
    //加载套接字库
    WSADATA wsaData;//保存套接字信息
    SOCKET hSocket;//套接字句柄
    SOCKADDR_IN servAddr;//保存服务器的地址信息

    char message[30];
    int strlen = 0;
    int idx = 0, readLen = 0;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)//加载库并获取库信息填至wsaData
    {
        cout << "socket start error!" << endl;
        return;
    }

    //创建套接字
    hSocket = socket(PF_INET, SOCK_STREAM, 0);//流式套接字,IPV4:TCP

    if (hSocket == INVALID_SOCKET)
    {
        cout << "socket create error!" << endl;
        return;
    }

    //设置地址端口
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;//地址族
    servAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//本地回路IP地址  字节顺序转换
    servAddr.sin_port = htons(6000);//与服务器端口一致   字节顺序转换

    //向服务器发送连接请求
    if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
    {
        cout << "socket connect error!" << endl;
        return;
    }

    while (readLen=recv(hSocket,&message[idx++],1,0))
    {
        if (readLen == -1)
        {
            cout << "socket recive error!" << endl;
            return;
        }
        strlen += readLen;
    }

    printf("Message from server :%s \n", message);
    printf("Function read call count:%d\n", strlen);

    closesocket(hSocket);
    WSACleanup();
    return;
}
```







