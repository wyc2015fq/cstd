# 【读书笔记】TCP/IP 网络编程 第23章 IOCP模型 - xiaxzhou的博客 - CSDN博客





2017年08月14日 14:25:37[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：215








### **IOCP模型**

IOCP中已完成的I/O信息将注册到**完成端口对象（Completion Port，简称CP对象）**。但这个过程并非简单的注册，需要如下两个工作：

1 创建完成端口对象

2 建立完成端口对象和套接字之间的联系

> 
此时的套接字必须被赋予重叠属性。（异步I/O）


#### **创建CP对象：CreateIoCompletoinPort()**

> 
前三个参数是无意义的，只有最后一个参数有意义：表示分配给CP对象的处理I/O的线程数，如果为0，表示分配CPU核数个线程数。 

  成功返回CP对象句柄，失败返回NULL


#### **连接完成端口对象和套接字：reateIoCompletoinPort()**

> 
参数：套接字句柄、CP对象句柄、**传递已完成I/O相关信息**、CP对象非null时自动忽略此参数


#### **传输数据：WSASend（）、 WSARecv（）**

#### **确认完成端口已完成的I/O和线程的I/O处理：GetQueuedCompletionStatus()**

> 
参数：CP对象句柄、传输数据大小变量地址、**CreateIoCompletoinPort（）第三个参数、WSASend/Recv（）的倒数第二个参数**、超时信息。

- **重点理解：**

> 
通过GetQueuedCompletionStatus（）函数的第三个参数得到的是以连接套接字和CP对象为目的而调用的CreateIoCompletoinPort（）函数的第三个参数值 


  通过GetQueuedCompletionStatus（）函数的第四个参数得到的是调用WSASend/Recv（）函数时传入的WSAOVERLAPPED结构体变量地址值
#### 是否自动创建线程？


否，需要程序员自行创建线程，该线程为了确认I/O的完成会调用GetQueuedCompletionStatus（）函数。

任何线程都能调用GetQueuedCompletionStatus（）函数，但实际得到I/O完成信息的线程数不会超过创建CP对象时指定的最大线程数。



### 理解IOCP模型：




**并发模型**：




> 
一个线程等待一个客户请求，并创建一个新的线程来处理请求。当新线程正在处理客户请求时，原来的线程会进入下一次循环并等待另一个客户请求。处理客户请求的线程完成处理过程后，会终止。



**并发模型的优点：**




> 
等待请求的线程只有很少的工作需要做，大多数事件它处于睡眠状态，当客户请求到来时，**能对客户的请求进行快捷的处理**。每个客户请求都有自己的线程，服务器具备非常好的**伸缩性**，能发挥多处理器的优势。CPU硬件升级时服务器性能会相应的提高。



**并发模型的问题：**

- **问题1：**

> 
线程过多时，所有的线程都处于可运行状态，内核在各个线程之间进行切换花费的时间太多。为了解决这个问题，IOCP的做法是并发运行的**线程的数量必须有一个上限**。


可运行线程的数量是多少合适？

> 
答案是处理器核数个线程是最理想的。

- **问题2：**

> 
需要为每个客户的请求创建一个新的线程。线程创建销毁开销过大。


IOCP使用**线程池**解决这个问题。

**IOCP模型的内部结构：**
- **设备列表：**

> 
保存与该CP端口关联的设备，此处为套接字

- **I/O完成队列：**

> 
先入先出，当设备的一个异步I/O请求完成时，系统检查是否与一个CP端口关联，如果设备与CP端口关联，系统将该项已完成的I/O请求追加到CP端口的I/O完成队列的末尾。

- **等待线程队列：**

> 
先进后出

- 
**已释放/忙碌线程列表：**

- 
**已暂停线程列表：**


> 
被释放的线程调用任何函数使得该线程切换到了等待状态，CP端口会检测到这一情况，并将该线程标识符从已释放线程队列移除，添加到已暂停线程列表中。


#### CP端口如何管理线程池：

**等待线程队列为什么要先进后出？**

> 
如果I/O请求完成的足够慢，使得一个线程能把它们全部处理完，那么**系统会不断唤醒同一个线程**，使其他线程继续睡眠。通过使用后入先出的算法，系统可以将那些未被调度的线程的内存资源换出到磁盘，并将它们从处理器的高速缓存中清除。


线程个数？

> 
当创建CP端口时，需要指定运行多少个线程并发。通常这个值设为主机CPU核的个数。


线程池中应该有多少线程？

> 
经验法则是：CPU核的个数乘以2

如果CPU有两个核，线程池应该有4个线程。则，如果有4个I/O请求已完成，有4个线程在等待，CP端口**只会唤醒两个线程**，而让其他两个线程继续睡眠。 

  当每个线程处理完一个已完成的I/O项时，会再次进入等待线程队列，系统发现I/O完成队列还有其他的项，于是会唤醒**同一个线程**对剩余的项进行处理。


**为什么要让更多的线程在线程池中等待呢？**

> 
当CP端口唤醒一个线程时，会将该线程标识放入已释放线程列表中。如果一个已释放线程调用的任何函数将该线程切换到了等待状态，那么CP端口会检测到这一情况，回将该线程的标识从已释放线程列表移除，并添加到已暂停线程列表中。

CP端口的目标是将尽可能多的线程保持在已释放线程列表中（但必须小于指定的并发线程数），如果一个线程进入等待状态，则已释放线程列表会减少，CP端口将释放另一个正在等待的线程。

如果暂停的线程被唤醒，那么它会离开已暂停线程列表，重新进入已释放线程列表。**此时已释放线程列表会在短时间内大于最大允许的并发线程数**。一旦线程进入下一次循环，并进入等待线程列表，可运行线程的数量就会迅速下降。


以上解释了为什么线程池中的线程数应该大于在完成端口中设置的并发线程数量。

IOCP模型回声服务器：

```cpp
#include "stdafx.h"
#include <winsock2.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <pthread.h>
#include <process.h>
#pragma comment(lib,"pthreadVC2.lib") 
using namespace::std;
#define BUF_SIZE 102
#define READ 3
#define WRITE 5

typedef struct
{
    SOCKET hClntSock;
    SOCKADDR_IN clntAdr;
}PER_HANDLE_DATA, *LPPER_HANDLE_DATA; // 套接字信息

typedef struct
{
    OVERLAPPED overlapped;      //重叠对象
    WSABUF wsaBuf;
    char buffer[BUF_SIZE];
    int rwMode;                 //读写标志
}PER_IO_DATA, *LPPER_IO_DATA;   //IO信息

void* EchoThreadMain(void*);    //线程函数
void ErrorHanding(char * message);

void main()
{
    WSADATA wsaData;
    HANDLE hComPort;
    SYSTEM_INFO sysInfo;
    LPPER_IO_DATA ioInfo;
    LPPER_HANDLE_DATA handleInfo;// 套接字信息

    SOCKET hServSock;
    SOCKADDR_IN servAdr;
    DWORD recvBytes;
    int i;
    DWORD flags = 0;
    vector<pthread_t> pthread_t_vec;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHanding("socket start error!");

    //创建CP对象
    hComPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);//最大线程数

    //创建线程，传入CP对象
    GetSystemInfo(&sysInfo);
    for (auto i = 0; i < sysInfo.dwNumberOfProcessors*2; ++i)
    {
        pthread_t_vec.push_back(pthread_t());
        pthread_create(&pthread_t_vec[i], NULL, EchoThreadMain, (void*)hComPort);
        pthread_detach(pthread_t_vec[i]);

    }

    hServSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
    memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family = AF_INET;
    servAdr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    servAdr.sin_port = htons(6000);
    bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr));
    listen(hServSock, 5);

    while (1)
    {
        SOCKET hClntSock;
        SOCKADDR_IN clntAdr;
        int addrLen = sizeof(clntAdr);

        hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &addrLen);
        // 填写套接字信息
        handleInfo = (LPPER_HANDLE_DATA)malloc(sizeof(PER_HANDLE_DATA));
        handleInfo->hClntSock = hClntSock;
        memcpy(&(handleInfo->clntAdr), &clntAdr, addrLen);

        //将套接字与CP对象建立连接，并传入套接字信息结构体地址
        CreateIoCompletionPort((HANDLE)hClntSock, hComPort, (DWORD)handleInfo, 0);//利用handleInfo向线程传递套接字信息

        ioInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
        memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
        ioInfo->wsaBuf.len = BUF_SIZE;
        ioInfo->wsaBuf.buf = ioInfo->buffer;
        ioInfo->rwMode = READ;

        //第一次接收数据,将唤醒阻塞在GetQueuedCompletionStatus的线程
        WSARecv(handleInfo->hClntSock, &(ioInfo->wsaBuf), 1, &recvBytes, &flags, &(ioInfo->overlapped), NULL);//利用overlapped向线程传递IO信息结构体地址
    }
    return;
}
void *EchoThreadMain(void* pComport)    //线程函数
{
    HANDLE hComPort = (HANDLE)pComport;
    SOCKET sock;
    DWORD bytesTrans;
    LPPER_HANDLE_DATA handleInfo;   //调用GetQueuedCompletionStatus后获得
    LPPER_IO_DATA ioInfo;           //调用GetQueuedCompletionStatus后获得
    DWORD flags = 0;
    while (1)
    {
        //确认完成端口已完成的I/O
        GetQueuedCompletionStatus(hComPort, &bytesTrans, (LPDWORD)&handleInfo, (LPOVERLAPPED*)&ioInfo, INFINITE);

        //线程每次处理的I/O完成的套接字不一定是同一个，由CP对象决定
        sock = handleInfo->hClntSock;

        if (ioInfo->rwMode == READ)//完成读入数据时执行
        {
            puts("message received!");
            if (bytesTrans == 0)//接收到EOF
            {
                closesocket(sock);
                free(handleInfo);
                free(ioInfo);
                continue;
            }

            memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
            ioInfo->wsaBuf.len = bytesTrans;
            ioInfo->rwMode = WRITE;             //标注当前套接字处于发送模式
            WSASend(sock, &(ioInfo->wsaBuf), 1, NULL, 0, &(ioInfo->overlapped), NULL);

            ioInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
            memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
            ioInfo->wsaBuf.len = BUF_SIZE;
            ioInfo->wsaBuf.buf = ioInfo->buffer;
            ioInfo->rwMode = READ;

            WSARecv(sock, &(ioInfo->wsaBuf), 1, NULL, &flags, &(ioInfo->overlapped), NULL);
        }
        else
        {
            puts("message sent!");
            free(ioInfo);
        }
    }
}

void ErrorHanding(char * message)
{
    cout << message << endl;
    exit(1);
}
```

客户端代码不变：

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






