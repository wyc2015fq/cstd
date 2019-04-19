# 跨平台Socket 代码测试 - 建建的博客 - CSDN博客
2017年03月11日 22:21:29[纪建](https://me.csdn.net/u013898698)阅读数：128
#ifdef WIN32
#include <Windows.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#define closesocket close
#endif
#include <stdio.h>
#include <iostream>
int main()
{
#ifdef WIN32
WSADATA ws;//加载动态库
WSAStartup(MAKEWORD(2,2),&ws);//引用加载动态库
#endif
    int sock=socket(AF_INET,SOCK_STREAM,0);
printf("%d",sock);
if (sock<0)
{
std::cout<<"create socket error"<<std::endl;
return -1;
}
    getchar();
std::cout<<"create socket success"<<std::endl;
closesocket(sock);
return 1;
}
其实所有的操作都是基于文件操作来的
