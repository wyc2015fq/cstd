# 【读书笔记】TCP/IP网络编程 第6章UDP数据传输 - xiaxzhou的博客 - CSDN博客





2017年08月08日 15:36:58[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：197








### 第6章 UDP数据传输
- UDP数据边界：

TCP数据传输不存在边界，数据传输过程中调用send recv函数的次数不具任何意义

相反，UDP是具有数据边界的协议，传输中调用send recv函数的次数非常重要。因此，输入函数的调用次数应和输出函数的调用次数完全一致，这样才能保证接收全部已发送数据。
- 连接UDP和未连接UDP：

**未连接UDP套接字**：每次调用**sendto**时，都向UDP套接字注册目标IP和端口。(若没有bind，则自动分配ip和端口，直到程序结束为止)

每次都可变更目标地址，因此可以重复利用一个UDP向不同的目标传输数据

**连接的套接字**：对UDP套接字调用**connect**函数，用于对同一主机长时间的通信。并不意味着要和对方UDP套接字连接，只是向UDP套接字注册目标的IP和端口信息。因为已经指定的收发对象，不仅可以使用sendto、recvfrom ，**还可以使用send、recv函数进行通信。**

UDP接收端：

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
    SOCKET servSock;
    char message[BUF_SIZE];
    int strLen;
    int clntAdrSz;
    SOCKADDR_IN servAdr, clntAdr;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)//加载库并获取库信息填至wsaData
        ErrorHanding("socket start error!");

    servSock = socket(PF_INET, SOCK_DGRAM, 0);//流式套接字,IPV4:UDP
    if (servSock == INVALID_SOCKET)
        ErrorHanding("socket create error!");

    //设置地址端口
    memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family = AF_INET;
    servAdr.sin_addr.S_un.S_addr = INADDR_ANY;
    servAdr.sin_port = htons(6000);

    //套接字绑定地址端口
    if (bind(servSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
        ErrorHanding("socket bind error!");

    while (1)
    {
        clntAdrSz = sizeof(clntAdr);
        //recvfrom参数：套接字，缓冲区，缓冲区大小，可选参数，存发送端地址信息的结构体，前一参数大小值变量地址
        strLen = recvfrom(servSock, message, BUF_SIZE, 0,(SOCKADDR*)&clntAdr,&clntAdrSz);

        sendto(servSock, message, strLen, 0, (SOCKADDR*)&clntAdr, sizeof(clntAdr));
    }

    closesocket(servSock);
    WSACleanup();
    return ;
}

void ErrorHanding(char * message)
{
    cout << message << endl;
    exit(1);
}
```

UDP发送端：

```cpp
// TcpClient.cpp : 定义控制台应用程序的入口点。
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
    SOCKET sock;//套接字句柄
    char message[BUF_SIZE];
    int str_Len;
    SOCKADDR_IN servAdr ;//保存服务器的地址信息

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)//加载库并获取库信息填至wsaData
        ErrorHanding("socket start error!");

    //创建套接字
    sock = socket(PF_INET, SOCK_DGRAM, 0);//流式套接字,IPV4:TCP
    if (sock == INVALID_SOCKET)
        ErrorHanding("socket create error!"); 

    //设置地址端口
    memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family = AF_INET;//地址族
    servAdr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//本地回路IP地址  字节顺序转换
    servAdr.sin_port = htons(6000);//与服务器端口一致   字节顺序转换

    //向套接字注册目标主机信息，并不是真的建立连接
    connect(sock, (SOCKADDR*)&servAdr, sizeof(servAdr));

    while (1)
    {
        fputs("Input message(Q to quit):", stdout);
        fgets(message, BUF_SIZE, stdin);
        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
            break;

        //sendto(sock, message, strlen(message), 0, (SOCKADDR*)&servAdr, sizeof(servAdr));
        //SOCKADDR_IN clntAdr;
        //int clntAdrSz = sizeof(clntAdr);
        //str_Len = recvfrom(sock, message, BUF_SIZE, 0, (SOCKADDR*)&clntAdr, &clntAdrSz);

        send(sock, message, strlen(message), 0);
        str_Len = recv(sock, message, BUF_SIZE - 1, 0);

        message[str_Len] = 0;
        printf("Message from server: %s ", message);
    }

    closesocket(sock);
    WSACleanup();
    return;
}

void ErrorHanding(char * message)
{
    cout << message << endl;
    exit(1);
}
```



