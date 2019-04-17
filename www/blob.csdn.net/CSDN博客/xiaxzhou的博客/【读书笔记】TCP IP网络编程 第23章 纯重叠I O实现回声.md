# 【读书笔记】TCP/IP网络编程 第23章 纯重叠I/O实现回声 - xiaxzhou的博客 - CSDN博客





2017年08月13日 21:47:17[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：212








### 纯重叠I/O实现回声服务器

#### 非阻塞模式的套接字：

ioctlsocket()函数可修改套接字属性为非阻塞模式
- 在没有客户端连接时调用accept函数，将直接返回INVALID_SOCKET。调用WSAGetLastError函数返回WSAEWOULDBLOCK。
- 调用accept函数时创建的套接字同样有非阻塞属性。

#### 工作原理：

1 有新客户端连接时，调用WSARecv函数，以非阻塞模式接收数据，接收完成后调用ReadCompRoutine函数 

2 调用的ReadCompRoutine函数中，调用WSASend函数，以非阻塞模式发送数据，发送完成后调用WriteCompRoutine函数 

3 调用的WriteCompRoutine函数中，调用WSARecv函数，以非阻塞模式接收数据，接收完成后调用ReadCompRoutine函数
通过交替调用ReadCompRoutine函数和WriteCompRoutine函数，反复执行数据的接收和发送。

### 纯重叠I/O回声服务器的缺点：

重复调用非阻塞模式的accept函数和以进入alertable wait状态为目的SleepEx函数将影响性能。

服务器模型IOCP：创建专用的I/O线程，该线程负责与所有的客户端进行I/O。

服务器端：

```cpp
#include "stdafx.h"
#include <winsock2.h>
#include <stdio.h>
#include <iostream>

using namespace::std;
#define BUF_SIZE 1024

void CALLBACK ReadCompRoutine(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);
void CALLBACK WriteCompRoutine(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);
void ErrorHanding(char * message);

typedef struct
{
    SOCKET hClntSock;   //套接字句柄
    char buf[BUF_SIZE]; //缓冲
    WSABUF wsaBuf;      //缓冲相关信息
}PER_IO_DATA,*LPPER_IO_DATA;


void main()
{
    WSADATA wsaData;
    SOCKET hLisnSock, hRecvSock;
    SOCKADDR_IN lisnAdr, recvAdr;
    LPWSAOVERLAPPED lpOvlap;
    DWORD recvBytes;
    LPPER_IO_DATA hbInfo;
    int recvAdrSz;
    DWORD flagInfo = 0;
    u_long mode = 1;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)//加载库并获取库信息填至wsaData
        ErrorHanding("socket start error!");

    //参数：协议族，套接字传输方式，使用的协议，WSA_PROTOCOL_INFO结构体地址/不需要时传null，扩展保留参数，套接字属性信息
    hLisnSock = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
    //将hLisnSock句柄的套接字I/O模式(FIONBIO)改为mode中指定的形式:非阻塞模式
    ioctlsocket(hLisnSock, FIONBIO, &mode);

    //设置目标地址端口
    memset(&lisnAdr, 0, sizeof(lisnAdr));
    lisnAdr.sin_family = AF_INET;
    lisnAdr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    lisnAdr.sin_port = htons(6000);

    //套接字绑定
    if (bind(hLisnSock, (SOCKADDR*)&lisnAdr, sizeof(lisnAdr)) == SOCKET_ERROR)
        ErrorHanding("socket bind error!");

    //设置为监听模式
    if (listen(hLisnSock, 5) == SOCKET_ERROR)
        ErrorHanding("socket listen error!");

    recvAdrSz = sizeof(recvAdr);
    while (1)
    {
        //进入短暂alertable wait 模式，运行ReadCompRoutine、WriteCompRoutine函数
        SleepEx(100, TRUE);

        //非阻塞套接字，需要处理INVALID_SOCKET
        //返回的新的套接字也是非阻塞的
        hRecvSock = accept(hLisnSock, (SOCKADDR*)&recvAdr, &recvAdrSz);
        if (hRecvSock == INVALID_SOCKET)
        {
            //无客户端连接时，accept返回INVALID_SOCKET，WSAGetLastError()返回WSAEWOULDBLOCK
            if (WSAGetLastError() == WSAEWOULDBLOCK)
                continue;
            else
                ErrorHanding("accept() error");
        }

        puts("Client connected");

        //申请重叠I/O需要使用的结构体变量的内存空间并初始化
        //在循环内部申请：每个客户端需要独立的WSAOVERLAPPED结构体变量
        lpOvlap = (LPWSAOVERLAPPED)malloc(sizeof(WSAOVERLAPPED));
        memset(lpOvlap, 0, sizeof(WSAOVERLAPPED));

        hbInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
        hbInfo->hClntSock = (DWORD)hRecvSock;

        (hbInfo->wsaBuf).buf = hbInfo->buf;
        (hbInfo->wsaBuf).len = BUF_SIZE;

        //基于CR的重叠I/O不需要事件对象，故可以用来传递其他信息
        lpOvlap->hEvent = (HANDLE)hbInfo;
        //接收第一条信息
        WSARecv(hRecvSock, &(hbInfo->wsaBuf), 1, &recvBytes, &flagInfo, lpOvlap, ReadCompRoutine);
    }
    closesocket(hRecvSock);
    closesocket(hLisnSock);
    WSACleanup();
    return;
}
//参数：错误信息，实际收发字节数，OVERLAPPED类型对象，调用I/O函数时传入的特性信息
void CALLBACK ReadCompRoutine(DWORD dwError, DWORD szRecvBytes, LPWSAOVERLAPPED lpOverlapped, DWORD flags)
{
    //从lpoverlapped中恢复传递的信息
    LPPER_IO_DATA hbInfo = (LPPER_IO_DATA)(lpOverlapped->hEvent);
    SOCKET hSock = hbInfo->hClntSock;
    LPWSABUF bufInfo = &(hbInfo->wsaBuf);
    DWORD sentBytes;

    //接收到EOF，断开连接
    if (szRecvBytes == 0)
    {
        closesocket(hSock);
        free(hbInfo);
        free(lpOverlapped);
        puts("Client disconnected");
    }
    else
    {
        bufInfo->len = szRecvBytes;
        //将接收到的信息回传回去，传递完毕执行WriteCompRoutine(): 接收信息
        WSASend(hSock, bufInfo, 1, &sentBytes, 0, lpOverlapped, WriteCompRoutine);
    }
}

//参数：错误信息，实际收发字节数，OVERLAPPED类型对象，调用I/O函数时传入的特性信息
void CALLBACK WriteCompRoutine(DWORD dwError, DWORD szRecvBytes, LPWSAOVERLAPPED lpOverlapped, DWORD flags)
{
    //从lpoverlapped中恢复传递的信息
    LPPER_IO_DATA hbInfo = (LPPER_IO_DATA)(lpOverlapped->hEvent);
    SOCKET hSock = hbInfo->hClntSock;
    LPWSABUF bufInfo = &(hbInfo->wsaBuf);
    DWORD recvBytes;
    DWORD flagInfo = 0;
    //接收数据，接收完毕执行ReadCompRoutine：发送数据
    WSARecv(hSock, bufInfo, 1, &recvBytes, &flagInfo, lpOverlapped, ReadCompRoutine);
}

void ErrorHanding(char * message)
{
    cout << message << endl;
    exit(1);
}
```

客户端：

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
    int readLen;
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
            break;

        strLen = strlen(message);
        send(hSocket, message, strlen(message), 0);
        readLen= 0;

        //考虑到TCP传输特性，重复调用recv函数
        while (readLen < strLen)
        {
            readLen += recv(hSocket, message, BUF_SIZE - 1, 0);
        }
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




