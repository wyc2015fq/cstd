# 【读书笔记】TCP/IP网络编程 第4章 回声 - xiaxzhou的博客 - CSDN博客





2017年08月08日 11:27:16[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：152








第4.5节

windows实现回声

回声服务端：

```cpp
#include "stdafx.h"
#include <winsock2.h>
#include <stdio.h>
#include <iostream>
using namespace::std;
#define BUF_SIZE 1024
void ErrorHanding(char * message);
void main()
{
    WSADATA wsaData;
    SOCKET hServSock, hClntSock;
    char message[BUF_SIZE];
    int strLen, i;
    SOCKADDR_IN servAdr, clntAdr;
    int clntAdrSize;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)//加载库并获取库信息填至wsaData
        ErrorHanding("socket start error!");

    hServSock = socket(PF_INET, SOCK_STREAM, 0);//流式套接字,IPV4:TCP
    if (hServSock == INVALID_SOCKET)
        ErrorHanding("socket create error!");

    //设置地址端口
    memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family = AF_INET;
    servAdr.sin_addr.S_un.S_addr = INADDR_ANY;
    servAdr.sin_port = htons(6000);

    //套接字绑定地址端口
    if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
        ErrorHanding("socket bind error!");

    //调用listen后，设置为监听模式，准备接收客户请求，才成为服务端套接字。等待连接队列的最大长度为5
    if (listen(hServSock, 5) == SOCKET_ERROR)
        ErrorHanding("socket listen error!");

    clntAdrSize = sizeof(clntAdr);

    for (i = 0; i < 5; ++i)
    {
        //clntAdrSize 为clntAdr 的长度，函数调用完后，该变量被填充入客户端地址长度
        hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &clntAdrSize);//accept函数单独创建新套接字连接到客户端，服务端套接字继续监听
        if (hClntSock == -1)
        {
            ErrorHanding("socket accept error!");
        }
        else
        {
            printf("Connect client %d\n", i + 1);
        }
        //recv 参数：socket，缓存地址，缓存最大字节数，接收数据的多种选项信息
        //send 参数：socket，缓存地址，发送的字节数  ，接收数据的多种选项信息
        while ((strLen = recv(hClntSock, message, BUF_SIZE, 0)) != 0)
            send(hClntSock, message, strLen, 0);

        closesocket(hClntSock);
    }
    closesocket(hServSock);
    WSACleanup();
    return ;
}

void ErrorHanding(char * message)
{
    cout << message << endl;
    exit(1);
}
```

回声客户端：

```cpp
#include "stdafx.h"
#include <winsock2.h>
#include <stdio.h>
#include <iostream>
using namespace::std;
#define BUF_SIZE 1024
void ErrorHanding(char * message);
void main()
{
    //加载套接字库
    WSADATA wsaData;//保存套接字信息
    SOCKET hSocket;//套接字句柄
    char message[BUF_SIZE];
    int strLen;
    SOCKADDR_IN servAdr;//保存服务器的地址信息

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)//加载库并获取库信息填至wsaData
        ErrorHanding("socket start error!");

    //创建套接字
    hSocket = socket(PF_INET, SOCK_STREAM, 0);//流式套接字,IPV4:TCP
    if (hSocket == INVALID_SOCKET)
        ErrorHanding("socket create error!"); 

    //设置地址端口
    memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family = AF_INET;//地址族
    servAdr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//本地回路IP地址  字节顺序转换
    servAdr.sin_port = htons(6000);//与服务器端口一致   字节顺序转换

    //向服务器发送连接请求
    if (connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
        ErrorHanding("socket connect error!"); 
    else
        puts("connected...........");

    while (1)
    {
        fputs("Input message(Q to quit):", stdout);
        fgets(message, BUF_SIZE, stdin);

        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
        {
            break;
        }
        send(hSocket, message, strlen(message), 0);
        strLen = recv(hSocket, message, BUF_SIZE - 1, 0); 
        message[strLen] = 0;
        printf("Message from server: %s ", message);
    }

    closesocket(hSocket);
    WSACleanup();
    return;
}

void ErrorHanding(char * message)
{
    cout << message << endl;
    exit(1);
}
```



