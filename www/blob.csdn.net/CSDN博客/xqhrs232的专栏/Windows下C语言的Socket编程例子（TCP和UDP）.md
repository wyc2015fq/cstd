# Windows下C语言的Socket编程例子（TCP和UDP） - xqhrs232的专栏 - CSDN博客
2016年07月14日 18:17:17[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：651
原文地址::[http://www.cnblogs.com/churi/archive/2013/02/27/2935427.html](http://www.cnblogs.com/churi/archive/2013/02/27/2935427.html)
相关文章
1、[windows
 socket 简单实例](http://www.cnblogs.com/xubin0523/archive/2012/09/11/2679851.html)----[http://www.cnblogs.com/xubin0523/archive/2012/09/11/2679851.html](http://www.cnblogs.com/xubin0523/archive/2012/09/11/2679851.html)
2、[Windows Socket 编程_ 简单的服务器/客户端程序](http://blog.csdn.net/neicole/article/details/7459021)----[http://blog.csdn.net/neicole/article/details/7459021](http://blog.csdn.net/neicole/article/details/7459021)
一。  <TCP>
server端：
```
1 #include "stdafx.h"
 2 #include <stdio.h>
 3 #include <winsock2.h>
 4 
 5 #pragma comment(lib,"ws2_32.lib")
 6 
 7 int main(int argc, char* argv[])
 8 {
 9     //初始化WSA
10     WORD sockVersion = MAKEWORD(2,2);
11     WSADATA wsaData;
12     if(WSAStartup(sockVersion, &wsaData)!=0)
13     {
14         return 0;
15     }
16 
17     //创建套接字
18     SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
19     if(slisten == INVALID_SOCKET)
20     {
21         printf("socket error !");
22         return 0;
23     }
24 
25     //绑定IP和端口
26     sockaddr_in sin;
27     sin.sin_family = AF_INET;
28     sin.sin_port = htons(8888);
29     sin.sin_addr.S_un.S_addr = INADDR_ANY; 
30     if(bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
31     {
32         printf("bind error !");
33     }
34 
35     //开始监听
36     if(listen(slisten, 5) == SOCKET_ERROR)
37     {
38         printf("listen error !");
39         return 0;
40     }
41 
42     //循环接收数据
43     SOCKET sClient;
44     sockaddr_in remoteAddr;
45     int nAddrlen = sizeof(remoteAddr);
46     char revData[255]; 
47     while (true)
48     {
49         printf("等待连接...\n");
50         sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
51         if(sClient == INVALID_SOCKET)
52         {
53             printf("accept error !");
54             continue;
55         }
56         printf("接受到一个连接：%s \r\n", inet_ntoa(remoteAddr.sin_addr));
57         
58         //接收数据
59         int ret = recv(sClient, revData, 255, 0);        
60         if(ret > 0)
61         {
62             revData[ret] = 0x00;
63             printf(revData);
64         }
65 
66         //发送数据
67         char * sendData = "你好，TCP客户端！\n";
68         send(sClient, sendData, strlen(sendData), 0);
69         closesocket(sClient);
70     }
71     
72     closesocket(slisten);
73     WSACleanup();
74     return 0;
75 }
```
client端：
```
1 #include "stdafx.h"
 2 #include <WINSOCK2.H>
 3 #include <STDIO.H>
 4 
 5 #pragma  comment(lib,"ws2_32.lib")
 6 
 7 
 8 int main(int argc, char* argv[])
 9 {
10     WORD sockVersion = MAKEWORD(2,2);
11     WSADATA data; 
12     if(WSAStartup(sockVersion, &data) != 0)
13     {
14         return 0;
15     }
16 
17     SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
18     if(sclient == INVALID_SOCKET)
19     {
20         printf("invalid socket !");
21         return 0;
22     }
23 
24     sockaddr_in serAddr;
25     serAddr.sin_family = AF_INET;
26     serAddr.sin_port = htons(8888);
27     serAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); 
28     if (connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
29     {
30         printf("connect error !");
31         closesocket(sclient);
32         return 0;
33     }
34     char * sendData = "你好，TCP服务端，我是客户端!\n";
35     send(sclient, sendData, strlen(sendData), 0);
36 
37     char recData[255];
38     int ret = recv(sclient, recData, 255, 0);
39     if(ret > 0)
40     {
41         recData[ret] = 0x00;
42         printf(recData);
43     }
44     closesocket(sclient);
45     WSACleanup();
46     return 0;
47 }
```
二. <UDP>
SERVER 端
```
1 #include "stdafx.h"
 2 #include <stdio.h>
 3 #include <winsock2.h>
 4 
 5 #pragma comment(lib, "ws2_32.lib") 
 6 
 7 int main(int argc, char* argv[])
 8 {
 9     WSADATA wsaData;
10     WORD sockVersion = MAKEWORD(2,2);
11     if(WSAStartup(sockVersion, &wsaData) != 0)
12     {
13         return 0;
14     }
15 
16     SOCKET serSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); 
17     if(serSocket == INVALID_SOCKET)
18     {
19         printf("socket error !");
20         return 0;
21     }
22 
23     sockaddr_in serAddr;
24     serAddr.sin_family = AF_INET;
25     serAddr.sin_port = htons(8888);
26     serAddr.sin_addr.S_un.S_addr = INADDR_ANY;
27     if(bind(serSocket, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
28     {
29         printf("bind error !");
30         closesocket(serSocket);
31         return 0;
32     }
33     
34     sockaddr_in remoteAddr;
35     int nAddrLen = sizeof(remoteAddr); 
36     while (true)
37     {
38         char recvData[255];  
39         int ret = recvfrom(serSocket, recvData, 255, 0, (sockaddr *)&remoteAddr, &nAddrLen);
40         if (ret > 0)
41         {
42             recvData[ret] = 0x00;
43             printf("接受到一个连接：%s \r\n", inet_ntoa(remoteAddr.sin_addr));
44             printf(recvData);            
45         }
46 
47         char * sendData = "一个来自服务端的UDP数据包\n";
48         sendto(serSocket, sendData, strlen(sendData), 0, (sockaddr *)&remoteAddr, nAddrLen);    
49 
50     }
51     closesocket(serSocket); 
52     WSACleanup();
53     return 0;
54 }
```
CLIENT 端
```
1 #include "stdafx.h"
 2 #include <stdio.h>
 3 #include <winsock2.h>
 4 
 5 #pragma comment(lib, "ws2_32.lib") 
 6 
 7 int main(int argc, char* argv[])
 8 {
 9     WORD socketVersion = MAKEWORD(2,2);
10     WSADATA wsaData; 
11     if(WSAStartup(socketVersion, &wsaData) != 0)
12     {
13         return 0;
14     }
15     SOCKET sclient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
16     
17     sockaddr_in sin;
18     sin.sin_family = AF_INET;
19     sin.sin_port = htons(8888);
20     sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
21     int len = sizeof(sin);
22     
23     char * sendData = "来自客户端的数据包.\n";
24     sendto(sclient, sendData, strlen(sendData), 0, (sockaddr *)&sin, len);
25 
26     char recvData[255];     
27     int ret = recvfrom(sclient, recvData, 255, 0, (sockaddr *)&sin, &len);
28     if(ret > 0)
29     {
30         recvData[ret] = 0x00;
31         printf(recvData);
32     }
33 
34     closesocket(sclient);
35     WSACleanup();
36     return 0;
37 }
```
本文来至：[http://blog.csdn.net/ssun125/article/details/8525823](http://blog.csdn.net/ssun125/article/details/8525823)
