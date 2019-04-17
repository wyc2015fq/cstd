# 【读书笔记】TCP/IP网络编程 第14章 多播、广播 - xiaxzhou的博客 - CSDN博客





2017年08月09日 10:19:44[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：151








### 第14章 多播、广播

#### 多播数据传输方式及流量方面的特点：
- 多播服务器端针对特定的多播组，**只发送一次数据**
- 即使只发送一次数据，该组内的所有客户端**都会受到数据**（路由器负责复制文件并传递到主机）
- 多播组数可在IP地址范围内任意增加
- 加入特定的多播组，就可接收发往该组的数据

#### TTL（time to live）：

其值大小决定了数据包传递的距离，每经过一个路由器值就减一，TTL变为0则无法被传递，只能销毁了。

#### 多播与广播的区别：

多播即使在跨越不同网络的情况下，只要加入多播组就能接收数据。相反，广播只能**向同一网络**中的主机传输数据。
- 直接广播与本地广播区别：

直接广播(direct broadcast)是指Host部分全为1的广播地址。如：192.168.224.31。当某机器发出目的地址为直接广播（如：192.168.224.31）时，**路由器通过查找路由表可以转发，直到该网段**。当然，这需要在路由器接口上enable direct broadcast。直接广播在跨路由DHCP,TFTP等应用有着重要的作用。 本地广播(local broadcast)是指255.255.255.255。当某机器发出目的地址为本地广播时，**路由器不会转发该包。所以该包只能限制在本网段**，达到隔离广播，消除广播风暴的目的，所以称之为本地广播。

广播发送端：

```cpp
#include "stdafx.h"
#include <winsock2.h>
#include <stdio.h>
#include <iostream>
#include <ws2tcpip.h>

using namespace::std;
#define BUF_SIZE 3000
void ErrorHanding(char * message);

void main()
{
    WSADATA wsaData;
    SOCKET send_sock;
    SOCKADDR_IN broad_adr;
    FILE * fp;
    char buf[BUF_SIZE];
    int so_brd = 1;


    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)//加载库并获取库信息填至wsaData
        ErrorHanding("socket start error!");

    send_sock = socket(PF_INET, SOCK_DGRAM, 0);//流式套接字,IPV4:TCP

    //设置地址端口
    memset(&broad_adr, 0, sizeof(broad_adr));
    broad_adr.sin_family = AF_INET;
    broad_adr.sin_addr.S_un.S_addr = inet_addr("255.255.255.255");
    broad_adr.sin_port = htons(6000);

    //利用套接字IP_MULTICAST_TTL设置TTL
    setsockopt(send_sock, SOL_SOCKET, SO_BROADCAST, (char *)&so_brd, sizeof(so_brd));

    //打开传输数据文件
    if ((fp=fopen("TCPSrv.cpp","r"))==NULL)
        ErrorHanding("file open error!");

    while (!feof(fp))
    {
        fgets(buf, BUF_SIZE, fp);
        sendto(send_sock, buf, strlen(buf), 0, (SOCKADDR*)&broad_adr, sizeof(broad_adr));//向地址发送数据

        Sleep(1000);
    }
    closesocket(send_sock);
    WSACleanup();
    return ;
}

void ErrorHanding(char * message)
{
    cout << message << endl;
    exit(1);
}
```

广播接收端：

```cpp
#include "stdafx.h"
#include <winsock2.h>
#include <stdio.h>
#include <iostream>
#include <ws2tcpip.h>

using namespace::std;
#define BUF_SIZE 3000 
void ErrorHanding(char * message);

void main()
{
    WSADATA wsaData;
    SOCKET recv_sock;
    char buf[BUF_SIZE];
    int str_len;
    SOCKADDR_IN adr;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)//加载库并获取库信息填至wsaData
        ErrorHanding("socket start error!");

    recv_sock = socket(PF_INET, SOCK_DGRAM, 0);//流式套接字,IPV4:UDP

    //设置地址端口
    memset(&adr, 0, sizeof(adr));
    adr.sin_family = AF_INET;
    adr.sin_addr.S_un.S_addr = INADDR_ANY;
    adr.sin_port = htons(6000);

    //套接字绑定地址端口
    if (bind(recv_sock, (SOCKADDR*)&adr, sizeof(adr)) == SOCKET_ERROR)
        ErrorHanding("socket bind error!");

    while (1)
    {
        //recvfrom参数：套接字，缓冲区，缓冲区大小，可选参数，存发送端地址信息的结构体，前一参数大小值变量地址
        str_len = recvfrom(recv_sock, buf, BUF_SIZE-1, 0, NULL, 0);//不需要知道传输数据的主机地址信息 :第5/6个参数设为null 0

        if (str_len<0)
            break;
        buf[str_len] = 0;

        fputs(buf, stdout);
    }

    closesocket(recv_sock);
    WSACleanup();
    return;
}

void ErrorHanding(char * message)
{
    cout << message << endl;
    exit(1);
}
```

广播效果：

![这里写图片描述](https://img-blog.csdn.net/20170809103732240?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlheHpob3U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

多播发送者：

```cpp
#include "stdafx.h"
#include <winsock2.h>
#include <stdio.h>
#include <iostream>
#include <ws2tcpip.h>

using namespace::std;
#define BUF_SIZE 1024
void ErrorHanding(char * message);
#define TTL 64

void main()
{
    WSADATA wsaData;
    SOCKET hSendSock;
    SOCKADDR_IN mulAdr;
    int timeLive = TTL;
    FILE * fp;
    char buf[BUF_SIZE];

    int adrSz;
    int strLen, fdNum, i;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)//加载库并获取库信息填至wsaData
        ErrorHanding("socket start error!");

    hSendSock = socket(PF_INET, SOCK_DGRAM, 0);//流式套接字,IPV4:TCP

    //设置地址端口
    memset(&mulAdr, 0, sizeof(mulAdr));
    mulAdr.sin_family = AF_INET;
    mulAdr.sin_addr.S_un.S_addr = inet_addr("244.1.1.2");
    mulAdr.sin_port = htons(6000);

    //利用套接字IP_MULTICAST_TTL设置TTL
    setsockopt(hSendSock, IPPROTO_IP, IP_MULTICAST_TTL, (char *)&timeLive, sizeof(timeLive));

    //打开传输数据文件
    if ((fp=fopen("TCPSrv.cpp","r"))==NULL)
        ErrorHanding("file open error!");

    while (!feof(fp))
    {
        fgets(buf, BUF_SIZE, fp);
        sendto(hSendSock, buf, strlen(buf),0,(SOCKADDR*)&mulAdr,sizeof(mulAdr));//向多播地址发送数据

        Sleep(2000);
    }
    closesocket(hSendSock);
    WSACleanup();
    return ;
}

void ErrorHanding(char * message)
{
    cout << message << endl;
    exit(1);
}
```

多播接收端：

```cpp
#include "stdafx.h"
#include <winsock2.h>
#include <stdio.h>
#include <iostream>
#include <ws2tcpip.h>

using namespace::std;
#define BUF_SIZE 1024
void ErrorHanding(char * message);

void main()
{
    WSADATA wsaData;
    SOCKET hRecvSock;
    char buf[BUF_SIZE];
    int strLen;
    SOCKADDR_IN adr;
    ip_mreq joinAdr;//多播组地址信息

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)//加载库并获取库信息填至wsaData
        ErrorHanding("socket start error!");

    hRecvSock = socket(PF_INET, SOCK_DGRAM, 0);//流式套接字,IPV4:UDP

    //设置地址端口
    memset(&adr, 0, sizeof(adr));
    adr.sin_family = AF_INET;
    adr.sin_addr.S_un.S_addr = INADDR_ANY;
    adr.sin_port = htons(6000);

    //套接字绑定地址端口
    if (bind(hRecvSock, (SOCKADDR*)&adr, sizeof(adr)) == SOCKET_ERROR)
        ErrorHanding("socket bind error!");

    //设置多播组信息
    joinAdr.imr_multiaddr.S_un.S_addr = inet_addr("244.1.1.2");
    joinAdr.imr_interface.S_un.S_addr = INADDR_ANY;

    //利用套接字ip_add_membership加入多播组
    if (setsockopt(hRecvSock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&joinAdr, sizeof(joinAdr)) == SOCKET_ERROR)
        ErrorHanding("setsockopt() error !");

    while (1)
    {
        //recvfrom参数：套接字，缓冲区，缓冲区大小，可选参数，存发送端地址信息的结构体，前一参数大小值变量地址
        strLen = recvfrom(hRecvSock, buf, BUF_SIZE, 0, NULL,0);//不需要知道传输数据的主机地址信息 :第5/6个参数设为null 0

        if (strLen<0)
            break;
        fputs(buf, stdout);
    }

    closesocket(hRecvSock);
    WSACleanup();
    return;
}

void ErrorHanding(char * message)
{
    cout << message << endl;
    exit(1);
}
```



