# 【读书笔记】TCP/IP网络编程 第18章 多线程并发 - xiaxzhou的博客 - CSDN博客





2017年08月10日 20:51:18[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：158








### 多线程并发  聊天程序

基于Windows VS2013、pthread
- 服务器端：

主线程监视新连接： 

当新连接到来时，新建线程，传入连接返回的新套接字给两个新线程，并将新套接字记录到数组中

新线程： 

从传入的套接字中接收消息，并通过套接字数组中的套接字转发出去 

当前套接字断开连接时，将其从数组中删除，退出线程`pthread_detach(thread_id);  `
与pthread_join() 不同，detach函数不会引起线程终止或进入阻塞状态，可以引导销毁线程创建的内存

```cpp
#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <winsock2.h>
#include <pthread.h>

#pragma comment(lib,"pthreadVC2.lib")

using namespace::std;
#define BUF_SIZE 100
#define MAX_CLNT 256//最大可连接客户端数

void * handle_clnt(void*arg);//线程入口函数：新建线程处理新连接的客户端
void send_msg(char* msg, int len);
void ErrorHanding(char * message);

int clnt_cnt = 0;
SOCKET clnt_socks[MAX_CLNT];//连接套接字数组
pthread_mutex_t mutex;//互斥锁

void main()
{
    WSADATA wsaData;
    SOCKET serv_sock, clnt_sock;
    SOCKADDR_IN serv_adr, clnt_adr;
    int clnt_adr_sz;
    pthread_t thread_id;//线程标识

    pthread_mutex_init(&mutex,NULL);//初始化互斥锁

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)//加载库并获取库信息填至wsaData
        ErrorHanding("socket start error!");

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);//流式套接字,IPV4:TCP
    if (serv_sock == INVALID_SOCKET)
        ErrorHanding("socket create error!");

    //设置地址端口
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.S_un.S_addr = INADDR_ANY;
    serv_adr.sin_port = htons(6000);

    //套接字绑定地址端口
    if (bind(serv_sock, (SOCKADDR*)&serv_adr, sizeof(serv_adr)) == SOCKET_ERROR)
        ErrorHanding("socket bind error!");

    //设置为监听模式
    if (listen(serv_sock, 5) == SOCKET_ERROR)
        ErrorHanding("socket listen error!");

    while (1)
    {
        clnt_adr_sz = sizeof(serv_adr);
        clnt_sock = accept(serv_sock, (SOCKADDR*)&clnt_adr, &clnt_adr_sz);//accept函数单独创建新套接字连接到客户端，服务端套接字继续监听

        pthread_mutex_lock(&mutex);
        clnt_socks[clnt_cnt++] = clnt_sock;
        pthread_mutex_unlock(&mutex);


        pthread_create(&thread_id, NULL, handle_clnt, (void*)&clnt_sock);//创建新线程负责新客户端，并将新套接字传入新线程中
        pthread_detach(thread_id);      //detach函数不会引起线程终止或进入阻塞状态，可以引导销毁线程创建的内存
        printf("connect client IP: %s\n", inet_ntoa(clnt_adr.sin_addr));
    }
    closesocket(serv_sock);
    WSACleanup();
    return;
}

void * handle_clnt(void* arg)
{
    SOCKET clnt_sock = *((SOCKET*)arg);
    int str_len = 0;
    char msg[BUF_SIZE];

    while ((str_len=recv(clnt_sock,msg,sizeof(msg),0))!=0)//recv 成功返回字节数，失败返回SOCET_ERRO  ,收到EOF返回0
    {
        send_msg(msg, str_len);
    }

    pthread_mutex_lock(&mutex);
    for (auto i = 0; i < clnt_cnt;++i)
    {
        if (clnt_sock == clnt_socks[i])
        {
            while (i++ < clnt_cnt-1)
            {
                clnt_socks[i] = clnt_socks[i + 1];
            }
            break;
        }
    }
    clnt_cnt--;
    pthread_mutex_unlock(&mutex);
    closesocket(clnt_sock);
    return NULL;
}

void send_msg(char* msg, int len)
{
    pthread_mutex_lock(&mutex);
    for (auto i = 0; i < clnt_cnt;++i)
    {
        send(clnt_socks[i], msg, len,0);//向所有客户端转发收到的消息
    }
    pthread_mutex_unlock(&mutex);
}

void ErrorHanding(char * message)
{
    cout << message << endl;
    exit(1);
}
```
- 客户端：

两个新线程：接收线程，发送线程 

主线程阻塞等待新线程的结束

```cpp
#include "stdafx.h"
#include <winsock2.h>
#include <stdio.h>
#include <pthread.h>
#include <iostream>
#pragma comment(lib,"pthreadVC2.lib")

using namespace::std;
#define BUF_SIZE 100
#define NAME_SIZE 20

void* send_msg(void* arg);
void* recv_msg(void * arg);
void ErrorHanding(char * message);

char name[NAME_SIZE] = "[DEFALUT]";
char msg[BUF_SIZE];

void main()
{
    //加载套接字库
    WSADATA wsaData;//保存套接字信息
    SOCKET sock;//套接字句柄
    int strLen;
    SOCKADDR_IN serv_adr;//保存服务器的地址信息
    void * thread_return;
    pthread_t snd_thread, rcv_thread;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)//加载库并获取库信息填至wsaData
        ErrorHanding("socket start error!");

    //创建套接字
    sock = socket(PF_INET, SOCK_STREAM, 0);//流式套接字,IPV4:TCP
    if (sock == INVALID_SOCKET)
        ErrorHanding("socket create error!");

    //设置地址端口
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;//地址族
    serv_adr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//本地回路IP地址  字节顺序转换
    serv_adr.sin_port = htons(6000);//与服务器端口一致   字节顺序转换

    //向服务器发送连接请求
    if (connect(sock, (SOCKADDR*)&serv_adr, sizeof(serv_adr)) == SOCKET_ERROR)
        ErrorHanding("socket connect error!");
    else
        puts("connected...........");

    pthread_create(&snd_thread, NULL, send_msg, (void*)&sock);
    pthread_create(&rcv_thread, NULL, recv_msg, (void*)&sock);
    pthread_join(snd_thread, &thread_return);
    pthread_join(rcv_thread, &thread_return);

    closesocket(sock);
    WSACleanup();
    return;
}

void* send_msg(void* arg)
{
    SOCKET sock = *((SOCKET*)arg);

    char name_msg[NAME_SIZE + BUF_SIZE];
    while (1)
    {
        fgets(msg, BUF_SIZE, stdin);
        if (strlen(msg)==1&&(msg[0]=='q'||msg[0]=='Q'))
        {
            closesocket(sock);
            exit(0);
        }
        sprintf(name_msg, "%s %s", name, msg);
        send(sock, name_msg, strlen(name_msg), 0);
    }
    return NULL;
}

void* recv_msg(void* arg)
{
    SOCKET sock = *((SOCKET*)arg);
    char name_msg[NAME_SIZE + BUF_SIZE];
    int str_len;
    while (1)
    {
        str_len = recv(sock, name_msg, NAME_SIZE + BUF_SIZE, 0);
        if (str_len == SOCKET_ERROR)
        {
            return (void*)-1;
        }
        name_msg[str_len] = 0;
        fputs(name_msg, stdout);
    }
    return NULL;
}

void ErrorHanding(char * message)
{
    cout << message << endl;
    exit(1);
}
```




