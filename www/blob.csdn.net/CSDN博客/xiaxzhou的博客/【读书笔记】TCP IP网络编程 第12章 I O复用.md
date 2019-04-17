# 【读书笔记】TCP/IP网络编程 第12章 I/O复用 - xiaxzhou的博客 - CSDN博客





2017年08月08日 22:48:09[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：205








### 第12章 I/O复用
- 用法

使用select函数，将多个套接字统一监视（接收，传输，异常）。

调用select后，并不是把发生变化的套接字集中到一起，而是通过观察作为监视对象的fd_set变量的变化，找出发生变化的套接字。

作为监视对象的fd_set变量会发生变化，所以调用select函数前应复制并保存原有的信息。并在每次调用select函数时传递新的监视对象信息。
- 为何要把监视对象传递给操作系统：

套接字是由操作系统管理的，需要操作系统才能完成对套接字的监视。
- 性能瓶颈：

无论如何优化都无法同时连接上百个客户端。原因在于 每次都要传递监视对象信息，应用程序向操作系统传递数据将对程序造成很大的负担。
- 改进方式：

仅向操作系统传递1次监视对象，监视范围或者内容变化时只通知发生变化的事项。这样就无需每次调用select函数时都向操作系统传递监视对象信息，但前提是操作系统支持这种处理方式。Linux支持的方式是epoll，Windows支持的方式是IOCP。

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
    SOCKET hServSock,hClntSock;
    SOCKADDR_IN servAdr, clntAdr;
    TIMEVAL timeout;
    fd_set reads, cpyReads;//file_detect_sets

    int adrSz;
    int strLen, fdNum, i;
    char buf[BUF_SIZE];

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)//加载库并获取库信息填至wsaData
        ErrorHanding("socket start error!");

    hServSock = socket(PF_INET, SOCK_STREAM, 0);//流式套接字,IPV4:TCP

    //设置地址端口
    memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family = AF_INET;
    servAdr.sin_addr.S_un.S_addr = INADDR_ANY;
    servAdr.sin_port = htons(6000);

    //套接字绑定地址端口
    if(bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
        ErrorHanding("socket bind error!");

    //监听模式
    listen(hServSock, 5);

    FD_ZERO(&reads);//fd_set 初始化为0
    FD_SET(hServSock, &reads);//向 fd_set 注册套接字句柄

    while (1)
    {
        cpyReads = reads;//select函数会改变传入的fd_set
        timeout.tv_sec = 5;//超时设置
        timeout.tv_usec = 5000;

        //select函数参数：无意义，关注是否待读的fd_set，关注是否可写的fd_set，关注是否异常的fd_set，超时设置
        if ((fdNum = select(0, &cpyReads, 0, 0, &timeout)) == SOCKET_ERROR)
            break;

        if (fdNum == 0)//超时没有套接字发生了改变
            continue;

        for (i = 0; i < reads.fd_count; i++)
        {
            //FD_ISSSET ：若第二个参数包含第一个参数（文件描述符/套接字）返回 真
            if (FD_ISSET(reads.fd_array[i], &cpyReads))//若是发生了变化的套接字
            {
                if (reads.fd_array[i] == hServSock)//如果是服务器套接字发生改变:说明有连接请求
                {
                    adrSz = sizeof(clntAdr);
                    hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &adrSz);//建立连接
                    FD_SET(hClntSock, &reads);                                  //将建立连接新建的套接字注册进fd_set
                    printf("connnect client %d\n", hClntSock);
                }
                else
                {
                    strLen = recv(reads.fd_array[i], buf, BUF_SIZE - 1, 0);//若是非服务器端套接字发生改变，则说明是新建的套接字发生了变化
                    //直接根据套接字读取数据
                    if (strLen == 0)//如果读入的是EOF，结束此连接
                    {
                        FD_CLR(reads.fd_array[i], &reads);//取消关注此套接字
                        closesocket(cpyReads.fd_array[i]);//关闭此套接字
                        printf("closed client :%d\n", cpyReads.fd_array[i]);
                    }
                    else//如果接收了数据，则发送回去
                    {
                        send(reads.fd_array[i], buf, strLen, 0);
                    }
                }
            }
        }
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



