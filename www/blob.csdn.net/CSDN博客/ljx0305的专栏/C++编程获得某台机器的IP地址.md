# C++编程获得某台机器的IP地址 - ljx0305的专栏 - CSDN博客
2008年04月07日 13:36:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：515
**如何用C++编程获得某台机器的IP地址？**
编译：[Northtibet](mailto:northtibet@sohu.com)
[下载源代码](http://www.vckbase.com/code/downcode.asp?id=1651)
   对于这个古老的问题，VC知识库的精华区以及以往的在线杂志中都有详尽不一的描述。本文提供的方法是一个比较完整的解决方案，并附有详细的实现细节。希望大家对这个问题有彻底的认识。其实，如果你熟悉 Windows 的Sockets API，并了解一些底层的Winsock知识。那么要获得某台机器的IP地址并不是什么难事。一台机器可以装多块网卡，因此它就可能有多个IP地址。目前很多发烧友的PC机都装有多块网卡。其中一块网卡与调制解调器（MODEM）或者ADSL适配器相连，另一块与家里的局域网（LAN）相连。对于有宽带连接条件的家庭，这更是一种典型的配置。任何事情，一旦你知道了解决的方法，一切都会变得如此简单。下面是本文提供的一个简单的控制台程序（程序名为getip1），其功能就是显示本机的IP地址。如图一所示： 
![](http://www.vckbase.com/document/journal/vckbase15/images/cfig020604_1.gif)
图一 getip1的运行画面 
下面是 getip1 程序的代码，很简单：
////////////////////////////////////////////////////////////////
// getip1.cpp
//
// 本程序报告本机上每一块网卡的IP地址
// 命令行编译命令为:
//
// cl getip1.cpp wsock32.lib
//
// 请一定要在环境变量中正确指定LIB库的路径;可以运行vcvars32.bat
//
#include <winsock.h>
#include <wsipx.h>
#include <wsnwlink.h>
#include <stdio.h>
int main()
{
   ////////////////
   // 初始化 Windows sockets API. 要求版本为 version 1.1
   //
   WORD wVersionRequested = MAKEWORD(1, 1);
   WSADATA wsaData;
   if (WSAStartup(wVersionRequested, &wsaData)) {
      printf("WSAStartup failed %s/n", WSAGetLastError());
      return -1;
   }
   //////////////////
   // 获得主机名.
   //
   char hostname[256];
   int res = gethostname(hostname, sizeof(hostname));
   if (res != 0) {
      printf("Error: %u/n", WSAGetLastError());
      return -1;
   }
   printf("hostname=%s/n", hostname);
   ////////////////
   // 根据主机名获取主机信息. 
   //
   hostent* pHostent = gethostbyname(hostname);
   if (pHostent==NULL) {
      printf("Error: %u/n", WSAGetLastError());
      return -1;
   }
   //////////////////
   // 解析返回的hostent信息.
   //
   hostent& he = *pHostent;
   printf("name=%s/naliases=%s/naddrtype=%d/nlength=%d/n",
      he.h_name, he.h_aliases, he.h_addrtype, he.h_length);
   
   sockaddr_in sa;
   for (int nAdapter=0; he.h_addr_list[nAdapter]; nAdapter++) {
      memcpy ( &sa.sin_addr.s_addr, he.h_addr_list[nAdapter],he.h_length);
      // 输出机器的IP地址.
      printf("Address: %s/n", inet_ntoa(sa.sin_addr)); // 显示地址串
   }
   //////////////////
   // 终止 Windows sockets API
   //
   WSACleanup();
   return 0;
}     
    要使用Winsock，首先必须调用WSAStartup，最后结束时不要忘了调用WSACleanup。要获取IP地址，首先必须得到机器的主机名（host name），调用gethostname就可以实现，有了主机名，接下来调用gethostbyname来获取包括IP地址在内的更多的主机信息。gethostbyname返回一个指向hostent数据结构的指针，这个结构在<winsock.h>文件中是这样定义的：
// 来自winsock.h
struct  hostent {
  char    FAR * h_name;            /* 正式的主机名*/
  char    FAR * FAR * h_aliases;   /* 别名列表*/
  short   h_addrtype;              /* 主机地址类型*/
  short   h_length;                /* 地址长度*/
  char    FAR * FAR * h_addr_list; /* 地址清单*/
};
    这是个典型的底层APIs使用的数据结构，很多人都不是很熟悉它。实际上，hostent是一个变长的数据结构，h_name是主机名，在例子程序中的值为 "zxn.hangwire.sdb"。没有别名（h_aliases）。h_addrtype是地址类型（或者也叫地址家族），在例子程序中的值为2（AF_INET = internet，其它内容参见winsock.h）。h_length是每一个地址的长度，以字节为单位。因为IP地址的长度是4个字节，所以在例子程序中的值为4，h_addr_list是地址数组的开始点，它们一个接着一个存放，结尾是一个null。每一个x.y.z.w数字占一个字节。为了将IP地址格式化为x.y.z.w的形式，必须将地址数组先拷贝到一个叫sockaddr的数据结构中，然后调用一个特殊的函数inet_ntoa。 图二是hostent 结构在内存中存放示意图： 
![](http://www.vckbase.com/document/journal/vckbase15/images/cfig020604_2.gif)
图二 hostent 结构在内存中的存放示意图 
相信以上的解释再辅以阅读代码，你以后不再会对hostent结构感到陌生。 
现在.NET风靡一时，用C#语言编程也成为一种时尚，如果有谁想知道如何用C#来解决本文的问题，请参考精华区的相关内容： 
“[C#编程如何获取某台机器的IP地址？](http://www.vckbase.com/bbs/prime/viewprime.asp?id=423)” 
最后，顺祝编程愉快！ 
转自[http://www.vckbase.com/document/viewdoc/?id=435](http://www.vckbase.com/document/viewdoc/?id=435)
