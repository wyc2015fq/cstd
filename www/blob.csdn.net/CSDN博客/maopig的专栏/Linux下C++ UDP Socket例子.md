# Linux下C++ UDP Socket例子 - maopig的专栏 - CSDN博客
2011年08月18日 23:40:35[maopig](https://me.csdn.net/maopig)阅读数：8522
这里我们给出了[linux](http://linux.chinaitlab.com/)下[C++](http://c.chinaitlab.com/)的socket代码如下：
　　#include <iostream>
　　#include <stdio.h>
　　#include <sys/socket.h>
　　#include <unistd.h>
　　#include <sys/types.h>
　　#include <netinet/in.h>
　　#include <stdlib.h>
　　#include <time.h>
　　#include <string.h>
　　#include <stdlib.h>
　　using namespace std;
　　int main (int argc, char *argv[])
　　{
　　cout << "Hello world!" << endl;
　　int localSocket,remoteSocket;
struct sockaddr_in  localAddr,remoteAddr;
　　localSocket = 
socket(AF_INET, SOCK_DGRAM, 0);
　　bzero(&localAddr, sizeof(localAddr));
　　localAddr.sin_family = AF_INET;
　　localAddr.sin_port = htons(53);
　　localAddr.sin_addr.s_addr = INADDR_ANY;
　　cout << "socket returned : " << localSocket << endl;
　　int result = 
bind(localSocket, (struct sockaddr *)&localAddr, sizeof(localAddr));
　　cout << "bind returned : " << result << endl;
　　while(1)
　　{
　　char buffer[1024];
socklen_t remoteAddrLength = sizeof(remoteAddr);
result = 
recvfrom(localSocket, &buffer, sizeof(buffer), 0, (struct sockaddr *)&remoteAddr, &remoteAddrLength);
　　cout << "received UDP data." << endl;
　　//构建一个char[]缓冲区，包含要发送的数据，在这里可以自己定义要发送的数据。
　　char sendBuff[512];
　　memset(sendBuff, 0, sizeof(sendBuff));
　　sendto(localSocket, &sendBuff, sizeof(sendBuff),0, (struct sockaddr *)&remoteAddr ,remoteAddrLength);
　　}
