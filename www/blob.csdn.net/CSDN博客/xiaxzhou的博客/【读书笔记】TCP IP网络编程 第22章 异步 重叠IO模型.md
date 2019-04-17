# 【读书笔记】TCP/IP网络编程  第22章 异步/重叠IO模型 - xiaxzhou的博客 - CSDN博客





2017年08月13日 15:08:11[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：186








### 第22章 重叠I/O模型

第21章异步处理的并非I/O，而是通知！ 
[http://blog.csdn.net/xiaxzhou/article/details/77116124](http://blog.csdn.net/xiaxzhou/article/details/77116124)
- 重叠I/O

一个线程内部**同时**向多个目标传输数据引起的I/O重叠现象称为重叠I/O。

为了完成这项任务，调用函数应立即返回，只有这样才能发送后续的数据。从结果上看，利用上述模型收发数据时，最重要的前提条件就是**异步I/O**。而且，为了完成异步I/O，调用的I/O函数应以**非阻塞模式**工作。

本章重点不是I/O本身，而是如何确认I/O完成时的状态。不管是输入输出，**只要是非阻塞模式的，就要另外确认执行的结果。**

创建适用于重叠I/O的套接字：WSASocket（）

执行重叠I/O的函数：WSASend（）、WSARecv（）

> 
WSASend函数调用过程中，函数返回时间点和数据传输完成时间点**并非总不一致**。 

  如果输出缓冲是空的，且传输的数据并不大，则函数调用后可立即完成数据传输。此时WSASend函数将返回0，函数第四个参数将保存实际传输的数据大小的信息。 

  反之，WSASend函数返回后仍需要传输数据，则返回SOCKET_ERROR，并将WSA_IO_PENDING注为错误代码，该代码可通过WSAGetLastError函数得到。这是可通过WSAGetOverlappedResult（）获取实际传输的数据大小。
两种方法重叠I/O的I/O完成确认： 

1 利用WSASend、WSARecv第六个参数：基于事件对象 

2 利用WSASend、WSARecv第七个参数：基于Completion Routine- 事件对象 

1 完成I/O时，WSAOVERLAPPED结构体变量引用的事件对象将变为signaled状态。 

2 为了验证I/O的完成和完成结果，需要调用WSAGetOverlappedResult函数。

使用WSASocket时返回值错误：10049

把：server_addr.sin_addr.s_addr=inet_addr(INADDR_ANY); 

改为：server_addr.sin_addr.s_addr=inet_addr(“127.0.0.1”); 

错误解决。不明白为何会这样
#### 基于事件对象：

服务器端：

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
    SOCKET hLisnSock,hRecvSock;
    SOCKADDR_IN lisnAdr,recvAdr;
    int recvAdrSz;

    WSABUF dataBuf;             //缓冲数据地址和大小等信息
    WSAEVENT evObj;             //事件对象
    WSAOVERLAPPED overlapped;   //只需关注：保存的事件对象  多个传输目标需要多个变量

    char buf[BUF_SIZE];
    DWORD recvBytes = 0, flags = 0;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)//加载库并获取库信息填至wsaData
        ErrorHanding("socket start error!");

    //参数：协议族，套接字传输方式，使用的协议，WSA_PROTOCOL_INFO结构体地址/不需要时传null，扩展保留参数，套接字属性信息
    hLisnSock = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

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
    hRecvSock = accept(hLisnSock, (SOCKADDR*)&recvAdr, &recvAdrSz);//建立连接

    evObj = WSACreateEvent();
    memset(&overlapped, 0, sizeof(overlapped));
    overlapped.hEvent = evObj;//填写事件对象

    dataBuf.len = BUF_SIZE;
    dataBuf.buf = buf; //填写接收数据区信息

    if (WSARecv(hRecvSock,&dataBuf,1,&recvBytes,&flags,&overlapped,NULL) == SOCKET_ERROR)
    {
        if (WSAGetLastError()==WSA_IO_PENDING)
        {
            puts("Backgrounnd data receive");
            WSAWaitForMultipleEvents(1, &evObj, TRUE, WSA_INFINITE, FALSE);
            WSAGetOverlappedResult(hRecvSock, &overlapped, &recvBytes, FALSE, NULL);
        }
        else
        {
            ErrorHanding("WSARecv error ");
        }
    }
    printf("received message : &s\n", buf);
    WSACloseEvent(evObj);
    closesocket(hRecvSock);
    closesocket(hLisnSock);
    WSACleanup();
    return;
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

void ErrorHanding(char * message);

void main()
{
    WSADATA wsaData;
    SOCKET hSocket;
    SOCKADDR_IN sendAdr;

    WSABUF dataBuf;                         //保存有待传输数据的地址和大小等信息
    char msg[] = "Network is Computer!";
    DWORD sendBytes = 0;                    //传输完成的数据大小信息

    WSAEVENT evObj;//事件对象
    WSAOVERLAPPED overlapped;//只需关注：保存的事件对象  多个传输目标需要多个变量

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)//加载库并获取库信息填至wsaData
        ErrorHanding("socket start error!");

    //参数：协议族，套接字传输方式，使用的协议，WSA_PROTOCOL_INFO结构体地址/不需要时传null，扩展保留参数，套接字属性信息
    hSocket = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

    //设置目标地址端口
    memset(&sendAdr, 0, sizeof(sendAdr));
    sendAdr.sin_family = AF_INET;
    sendAdr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    sendAdr.sin_port = htons(6000);

    //套接字连接
    if (connect(hSocket, (SOCKADDR*)&sendAdr, sizeof(sendAdr)) == SOCKET_ERROR)
        ErrorHanding("socket connect error!");

    evObj = WSACreateEvent();
    memset(&overlapped, 0, sizeof(overlapped));
    overlapped.hEvent = evObj;          //填写事件对象

    dataBuf.len = strlen(msg) + 1;
    dataBuf.buf = msg;                  //填写发送的数据信息

    if (WSASend(hSocket, &dataBuf, 1, &sendBytes, 0, &overlapped, NULL) == SOCKET_ERROR)
    {
        if (WSAGetLastError()==WSA_IO_PENDING)//数据传输未完成，仍在传输状态
        {
            puts("Background data send");
            WSAWaitForMultipleEvents(1, &evObj, TRUE, WSA_INFINITE, FALSE);//等待传输完成事件发生
            WSAGetOverlappedResult(hSocket, &overlapped, &sendBytes, FALSE, NULL);//获取传输结果
        }
        else
        {
            ErrorHanding("WSASend error!");
        }
    }
    printf("send data size: %d \n", sendBytes);
    WSACloseEvent(evObj);
    closesocket(hSocket);
    WSACleanup();
    return;
}

void ErrorHanding(char * message)
{
    auto tmp = WSAGetLastError();
    cout << message << endl;
    exit(1);
}
```

#### 基于CR（Completion Routine）：

注册CR的含义是：Pending的**I/O完成时调用此函数**！ 

I/O完成时调用注册过的函数进行事后处理这就是Completion Routine的运作方式。

如果执行重要任务时突然调用Completion Routine，则可能破坏程序的正常执行流。因此，操作系统通常会预先定义规则：只有请求I/O的线程处于**alertable wait状态**时才能调用Completion Routine函数！

alertable wait状态是等待接收操作系统消息的线程状态。调用下列函数时进入alertable wait状态：
- WaitForSingleObjectEX
- WaitForMultipleObjectEx
- WSAWaitForMultipleEvents
- SleepEx

第1、2、4个函数的功能与WaitForSingleObject、WaitForMultipleObject、Sleep函数相同。上述函数增加了一个参数，若参数为true则线程进入alertable wait状态。

WSAWaitForMultipleEvents最后一个参数设置为TRUE时，线程进入alertable wait状态。

启动I/O任务后，执行完紧急任务后可以调用上述任一函数验证I/O是否完成。此时线程进入alertable wait状态，**如果有已完成的I/O**，则操作系统调用相应的Completion Routine函数。**调用后，上述函数全部返回WAIT_IO_COMPLETION。**

接收端：

```cpp
#include "stdafx.h"
#include <winsock2.h>
#include <stdio.h>
#include <iostream>

using namespace::std;
#define BUF_SIZE 1024

void CALLBACK CompRoutione(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);
void ErrorHanding(char * message);
char buf[BUF_SIZE];
DWORD recvBytes = 0;
void main()
{
    WSADATA wsaData;
    SOCKET hLisnSock,hRecvSock;
    SOCKADDR_IN lisnAdr,recvAdr;
    int recvAdrSz;

    WSABUF dataBuf;             //缓冲数据地址和大小等信息
    WSAOVERLAPPED overlapped;   //只需关注：保存的事件对象  多个传输目标需要多个变量
    WSAEVENT evObj;

    int idx;
    DWORD flags = 0;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)//加载库并获取库信息填至wsaData
        ErrorHanding("socket start error!");

    //参数：协议族，套接字传输方式，使用的协议，WSA_PROTOCOL_INFO结构体地址/不需要时传null，扩展保留参数，套接字属性信息
    hLisnSock = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

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
    hRecvSock = accept(hLisnSock, (SOCKADDR*)&recvAdr, &recvAdrSz);//建立连接

    memset(&overlapped, 0, sizeof(overlapped));
    dataBuf.len = BUF_SIZE;
    dataBuf.buf = buf; //填写接收数据区信息

    if (WSARecv(hRecvSock,&dataBuf,1,&recvBytes,&flags,&overlapped,CompRoutione) == SOCKET_ERROR)
        if (WSAGetLastError() == WSA_IO_PENDING)//验证接收数据的操作是否为pending状态
        {
            puts("Background data recive");
        }

    evObj = WSACreateEvent();//多余的事件对象，使用SleepEx可避免此对象的产生

    //此函数调用是为了使主线程进入alertable wait状态
    idx = WSAWaitForMultipleEvents(1, &evObj, FALSE, WSA_INFINITE, TRUE);
    //线程进入alertable wait状态，
    //如果有已完成的I / O，则操作系统调用相应的Completion Routine函数。
    //调用后，上述函数返回WAIT_IO_COMPLETION。
    if (idx == WAIT_IO_COMPLETION)
    {
        puts("Overlapped I/O Completed!");
    }
    else
    {
        ErrorHanding("WSARecv error");
    }

    WSACloseEvent(evObj);
    closesocket(hRecvSock);
    closesocket(hLisnSock);
    WSACleanup();
    return;
}

//参数：错误信息，实际收发字节数，OVERLAPPED类型对象，调用I/O函数时传入的特性信息
void CALLBACK CompRoutione(DWORD dwError,DWORD szRecvBytes,LPWSAOVERLAPPED lpOverlapped,DWORD flags)
{
    if (dwError != 0)
    {
        ErrorHanding("CompRoutine error");
    }
    else
    {
        recvBytes = szRecvBytes;
        printf("received message :%s \n", buf);
    }

}

void ErrorHanding(char * message)
{
    cout << message << endl;
    exit(1);
}
```







