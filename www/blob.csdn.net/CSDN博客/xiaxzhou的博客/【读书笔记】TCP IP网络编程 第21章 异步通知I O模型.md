# 【读书笔记】TCP/IP网络编程  第21章 异步通知I/O模型 - xiaxzhou的博客 - CSDN博客





2017年08月12日 14:32:52[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：193标签：[网络编程](https://so.csdn.net/so/search/s.do?q=网络编程&t=blog)








### 第21章 异步通知I/O模型
- 异步通知I/O：

通过**WSACreateEvent**（）创建manual-reset模式non-signaled状态的事件对象。

使用**WSAEventSelect**（）函数向操作系统注册套接字及其关联的事件对象，当发生指定事件时，将相应的事件对象改为signaled状态。

使用**WSAWaitForMultipleEvent**（）验证多个套接字是否发生事件（阻塞式）

使用**WSAEnumNetworkEvents**（）获取发生事件的类型信息，此函数会将事件对象的signaled状态改为non-signaled状态。

服务器端：

```cpp
#include "stdafx.h"
#include <winsock2.h>
#include <stdio.h>
#include <iostream>
using namespace::std;
#define BUF_SIZE 104

void CompressSockets(SOCKET hSockArr[], int idx, int total);
void CompressEvents(WSAEVENT hEventArr[], int idx, int total);
void ErrorHanding(char * message);

void main()
{
    WSADATA wsaData;
    SOCKET hServSock, hClntSock;
    SOCKADDR_IN servAdr, clntAdr;

    SOCKET hSockArr[WSA_MAXIMUM_WAIT_EVENTS];
    WSAEVENT hEventArr[WSA_MAXIMUM_WAIT_EVENTS];//两个数组是为了维护套接字和事件的关联关系
    WSAEVENT newEvent;          //事件对象：发生事件时，事件对象被改为signaled状态
    WSANETWORKEVENTS netEvents; //保存发生的事件信息 : FD_READ FD_ACCEPT DF_WRITE

    int numOfClntSock = 0;
    int strLen, i;
    int posInfo, startIdx;
    int clntAdrLen;
    char msg[BUF_SIZE];

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)//加载库并获取库信息填至wsaData
        ErrorHanding("socket start error!");

    hServSock = socket(PF_INET, SOCK_STREAM, 0);//流式套接字,IPV4:TCP

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

    newEvent = WSACreateEvent();//创建事件对象：manual_reset模式
    if (WSAEventSelect(hServSock,newEvent,FD_ACCEPT)==SOCKET_ERROR )//异步通知：向操作系统注册。参数：监视的对象，通知的事件对象，监视的事件类型
        ErrorHanding("WSAEventSelect error!");

    hSockArr[numOfClntSock] = hServSock;
    hEventArr[numOfClntSock] = newEvent;//维持套接字与事件对象的对应关系
    numOfClntSock++;

    while (1)//等待事件发生
    {
        //参数：事件对象个数，数组指针，一个/全部signaled返回，超时设置，可警告等待
        posInfo = WSAWaitForMultipleEvents(numOfClntSock, hEventArr, FALSE, WSA_INFINITE, FALSE);//返回值：signaled状态的事件对象数组索引信息
        startIdx = posInfo - WSA_WAIT_EVENT_0;//signaled事件对象在数组的下标最小值

        for (i = startIdx; i < numOfClntSock; ++i)
        {
            //因为事件对象是manual-reset模式,可以反复验证
            int sigEventIdx = WSAWaitForMultipleEvents(1, &hEventArr[i], TRUE, 0, FALSE);//循环从第一个signaled事件对象到最后一个对象，验证是否转化为signaled状态
            if ((sigEventIdx == WSA_WAIT_FAILED || sigEventIdx == WSA_WAIT_TIMEOUT))
            {
                continue;
            }
            else
            {
                sigEventIdx = i;
                //区分事件类型。参数：发生事件的套接字，关联的事件对象，保存事件类型的结构体
                //此函数会将manual_reset模式的事件对象改为non_signaled状态
                WSAEnumNetworkEvents(hSockArr[sigEventIdx], hEventArr[sigEventIdx], &netEvents);

                if (netEvents.lNetworkEvents & FD_ACCEPT)//请求连接
                {
                    if (netEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
                    {
                        puts("Accept error!");
                        return;
                    }
                    clntAdrLen = sizeof(clntAdr);
                    hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &clntAdrLen);
                    newEvent = WSACreateEvent();//为新套接字创建新事件对象
                    WSAEventSelect(hClntSock, newEvent, FD_READ | FD_CLOSE);//将新事件对象注册到操作系统中

                    hEventArr[numOfClntSock] = newEvent;
                    hSockArr[numOfClntSock] = hClntSock;
                    numOfClntSock++;
                    puts("connected new client");
                }
                if (netEvents.lNetworkEvents & FD_READ)//接收数据
                {
                    if (netEvents.iErrorCode[FD_READ_BIT] !=0)
                    {
                        puts("read error!");
                        return;
                    }

                    strLen = recv(hSockArr[sigEventIdx], msg, sizeof(msg), 0);
                    msg[strLen] = 0;
                    send(hSockArr[sigEventIdx], msg, strlen(msg), 0);
                }
                if (netEvents.lNetworkEvents & FD_CLOSE)//断开连接
                {
                    if (netEvents.iErrorCode[FD_CLOSE_BIT]!= 0)
                    {
                        puts("read error!");
                        return;
                    }
                    WSACloseEvent(hEventArr[sigEventIdx]);
                    closesocket(hSockArr[sigEventIdx]);

                    numOfClntSock--;
                    CompressEvents(hEventArr, sigEventIdx, numOfClntSock);
                    CompressSockets(hSockArr, sigEventIdx, numOfClntSock);
                }
            }
        }
    }
    WSACleanup();
    return;
}

void CompressSockets(SOCKET hSockArr[], int idx, int total)
{
    int i;
    for (i = idx; i < total;++i)
        hSockArr[i] = hSockArr[i + 1];
}

void CompressEvents(WSAEVENT hEventArr[], int idx, int total)
{
    int i;
    for (i = idx; i < total; ++i)
        hEventArr[i] = hEventArr[i + 1];
}

void ErrorHanding(char * message)
{
    cout << message << endl;
    exit(1);
}
```

客户端不变：

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



