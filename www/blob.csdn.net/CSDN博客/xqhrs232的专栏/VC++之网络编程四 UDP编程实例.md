# VC++之网络编程四 UDP编程实例 - xqhrs232的专栏 - CSDN博客
2017年04月14日 17:12:38[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：518
原文地址::[http://blog.csdn.net/thinkscape/article/details/4037483](http://blog.csdn.net/thinkscape/article/details/4037483)
 server:
**[cpp]**[view
 plain](http://blog.csdn.net/thinkscape/article/details/4037483#)[copy](http://blog.csdn.net/thinkscape/article/details/4037483#)
- #include <Winsock2.h>
- #include <stdio.h>
- void main(){  
- //mide delete word
- WORD wVersionRequested;  
-     WSADATA wsaData;  
- int err;  
-     wVersionRequested=MAKEWORD(2,2);  
-     err=WSAStartup(wVersionRequested,&wsaData);  
- if(err!=0){  
- return;  
-     }  
- if(LOBYTE(wsaData.wVersion)!=2 || HIBYTE(wsaData.wHighVersion)!=2)  
-     {  
-         WSACleanup();  
- return ;  
-     }  
- 
-     SOCKET sockSrv=socket(AF_INET,SOCK_DGRAM,0);  
- 
-     SOCKADDR_IN addrSrv;  
-     addrSrv.sin_addr.S_un.S_addr =htonl(INADDR_ANY);  
-     addrSrv.sin_family=AF_INET;  
-     addrSrv.sin_port=htons(6000);  
-     bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));  
- 
-     SOCKADDR_IN addrClient;  
- int len=sizeof(SOCKADDR);  
- char recvBuf[100];  
- 
-     recvfrom(sockSrv,recvBuf,100,0,(SOCKADDR*)&addrClient,&len);  
-     printf("%s/n",recvBuf);  
-     closesocket(sockSrv);  
-     WSACleanup();  
- }  
client:
**[cpp]**[view
 plain](http://blog.csdn.net/thinkscape/article/details/4037483#)[copy](http://blog.csdn.net/thinkscape/article/details/4037483#)
- #include <Winsock2.h>
- #include <stdio.h>
- void main(){  
- //mide delete word
- WORD wVersionRequested;  
-     WSADATA wsaData;  
- int err;  
-     wVersionRequested=MAKEWORD(2,2);  
-     err=WSAStartup(wVersionRequested,&wsaData);  
- if(err!=0){  
- return;  
-     }  
- if(LOBYTE(wsaData.wVersion)!=2 || HIBYTE(wsaData.wHighVersion)!=2)  
-     {  
-         WSACleanup();  
- return ;  
-     }  
- 
-     SOCKET sockClient=socket(AF_INET,SOCK_DGRAM,0);  
- 
-     SOCKADDR_IN addrSrv;  
-     addrSrv.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");  
-     addrSrv.sin_family=AF_INET;  
-     addrSrv.sin_port=htons(6000);  
-     sendto(sockClient,"Hello",strlen("Hello")+1,0,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));  
- 
-     closesocket(sockClient);  
-     WSACleanup();  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
注意：
server 和 client 都要"项目属性"--->"配置属性"----> "链接"----> "输入" --->"附加依赖项"中添加"ws2_32.lib"
