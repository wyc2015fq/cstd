# vc根据域名获取IP地址 gethostbyname()函数 - xqhrs232的专栏 - CSDN博客
2017年08月17日 23:06:47[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：209
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://www.cnblogs.com/guanjie20/archive/2010/02/09/1666744.html](http://www.cnblogs.com/guanjie20/archive/2010/02/09/1666744.html)
以下是VC Socket初始化时用到的两个函数
一、WSAStartup函数
                int WSAStartup
                       (
                          WORD wVersionRequested,
                          LPWSADATA lpWSAData
                         );
使用Socket的程序在使用Socket之前必须调用WSAStartup函数。该函数的第一个参数指明程序请求使用的Socket版本，其中高位字节指明副版本、低位字节指明主版本；操作系统利用第二个参数返回请求的Socket的版本信息。当一个应用程序调用WSAStartup函数时，操作系统根据请求的Socket版本来搜索相应的Socket库，然后绑定找到的Socket库到该应用程序中。以后应用程序就可以调用所请求的Socket库中的其它Socket函数了。该函数执行成功后返回0。
例：假如一个程序要使用2.1版本的Socket,那么程序代码如下
wVersionRequested = MAKEWORD( 2, 1 );
err = WSAStartup( wVersionRequested, &wsaData );
二、WSACleanup函数
int WSACleanup (void);
应用程序在完成对请求的Socket库的使用后，要调用WSACleanup函数来解除与Socket库的绑定并且释放Socket库所占用的系统资源。
三 Socket接口的检索有关域名、通信服务和协议等Internet信息的数据库函数，如
gethostbyaddr、gethostbyname、gethostname、getprotolbyname
getprotolbynumber、getserverbyname、getservbyport。
　1．gethostname()
　　【函数原型】
int PASCAL FAR gethostname (char FAR * name, int namelen);
　　【使用说明】
　　该函数可以获取本地主机的主机名，其中：
　　name：<输出>用于指向所获取的主机名的缓冲区的指针。
　　Namelen：<输入>缓冲区的大小，以字节为单位。
　　返回值：若无错误，返回0；否则，返回错误代吗。
2．gethostbyname()
　　【函数原型】
struct hostent FAR * PASCAL FAR gethostbyname(const char FAR * name);
　　【使用说明】
　　该函数可以从主机名数据库中得到对应的“主机”。
　　该函数唯一的参数name就是前面调用函数gethostname()得到的主机名。若无错误，刚返回一个指向hostent结构的批针，它可以标识一个“主机”列表。
===============================================
根据域名 获取IP地址 gethostbyname()函数
可以运行的源代码
很容易, 但让一个没用过的人就不好写, 那些高手都很懒写BLOG
:
//getip.cpp
#include <windows.h>
#include <iostream>
#include <winsock.h>
#pragma comment(lib, "ws2_32")
using namespace std;
int main()
{
 int WSA_return;
 WSADATA WSAData;
 /*******************************************************************
 使用Socket的程序在使用Socket之前必须调用WSAStartup函数。
 该函数的第一个参数指明程序请求使用的Socket版本，
 其中高位字节指明副版本、低位字节指明主版本；
 操作系统利用第二个参数返回请求的Socket的版本信息。
 当一个应用程序调用WSAStartup函数时，操作系统根据请求的Socket版本来搜索相应的Socket库，
 然后绑定找到的Socket库到该应用程序中。
 以后应用程序就可以调用所请求的Socket库中的其它Socket函数了。
 该函数执行成功后返回0。
 *****************************************************************/
 WSA_return=WSAStartup(0x0101,&WSAData);
 /* 结构指针 */ 
 HOSTENT *host_entry;
 /* 网址: [http://www.google.cn/](http://www.google.cn/) */
 char host_name[256] ="[http://www.google.cn/](http://www.google.cn/)";
 if(WSA_return==0)
 {
       /* 即要解析的域名或主机名 */
  host_entry=gethostbyname(host_name);
  printf("%s\n", host_name);
  if(host_entry!=0)
  {
   printf("解析IP地址: ");
    printf("%d.%d.%d.%d",
        (host_entry->h_addr_list[0][0]&0x00ff),
        (host_entry->h_addr_list[0][1]&0x00ff),
        (host_entry->h_addr_list[0][2]&0x00ff),
        (host_entry->h_addr_list[0][3]&0x00ff));
  }
 }
 WSACleanup();
 }
