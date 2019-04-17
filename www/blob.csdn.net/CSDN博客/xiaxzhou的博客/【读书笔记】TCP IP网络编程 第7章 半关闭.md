# 【读书笔记】TCP/IP网络编程 第7章 半关闭 - xiaxzhou的博客 - CSDN博客





2017年08月08日 19:25:31[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：187








### 第七章：优雅地断开连接

**半关闭**：断开连接的一部分，可以传输数据但是无法接收，或可以接收数据但无法传输。

closesocket()函数会同时断开输入及输出流。
- 半关闭函数：**shutdown（）**

SD_RECEIVE：断开输入流 

SD_SEND：断开输出流 

SD_BOTH：同时断开I/O流
shutdown（）函数，关闭输出流时，既发送了**EOF**，还保留了输入流。

recv（）函数在收到EOF时，返回值为0。

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
    FILE * fp;
    char buf[BUF_SIZE];
    int clntAdrSz;
    int readCnt;
    SOCKADDR_IN servAdr, clntAdr;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)//加载库并获取库信息填至wsaData
        ErrorHanding("socket start error!");

    hServSock = socket(PF_INET, SOCK_STREAM, 0);//流式套接字,IPV4:TCP

    //设置地址端口
    memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family = AF_INET;
    servAdr.sin_addr.S_un.S_addr = INADDR_ANY;
    servAdr.sin_port = htons(6000);

    //套接字绑定地址端口
    bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr));
    //监听模式
    listen(hServSock, 5);

    //打开文件
    fp = fopen("TCPSrv.cpp", "rb");

    clntAdrSz = sizeof(clntAdr);
    //等待连接
    hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &clntAdrSz);

    //传输文件
    while (1)
    {
        readCnt = fread((void*)buf, 1, BUF_SIZE, fp);

        if (readCnt < BUF_SIZE)
        {
            send(hClntSock, (char*)&buf, readCnt, 0);//文件分多次send，最后一次send大小一定小于BUF_SIZE
            break;
        }
        send(hClntSock, (char*)&buf, BUF_SIZE, 0);//文件大小大于BUF_SIZE
    }
    //半关闭
    shutdown(hClntSock, SD_SEND);
    recv(hClntSock, (char*)buf, BUF_SIZE, 0);
    printf("Message from client : %s\n", buf);

    //完全关闭
    fclose(fp);
    closesocket(hClntSock);
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
    SOCKET hsock;//套接字句柄
    FILE *fp;

    char buf[BUF_SIZE];
    int readCnt;
    SOCKADDR_IN servAdr ;//保存服务器的地址信息

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)//加载库并获取库信息填至wsaData
        ErrorHanding("socket start error!");

    //创建套接字
    hsock = socket(PF_INET, SOCK_STREAM, 0);//流式套接字,IPV4:UDP

    //设置地址端口
    memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family = AF_INET;//地址族
    servAdr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//本地回路IP地址  字节顺序转换
    servAdr.sin_port = htons(6000);//与服务器端口一致   字节顺序转换

    connect(hsock, (SOCKADDR*)&servAdr, sizeof(servAdr));

    fp = fopen("receive.dat", "wb");

    while ((readCnt = recv(hsock, buf, BUF_SIZE, 0)) != 0)
    {
        fwrite((void*)buf, 1, readCnt, fp);
    }
    puts("Received file data");
    send(hsock, "Thank you", 10, 0);
    fclose(fp);
    closesocket(hsock);
    WSACleanup();
    return;
}

void ErrorHanding(char * message)
{
    cout << message << endl;
    exit(1);
}
```




