# Linux下简单的socket编程 - weixin_33985507的博客 - CSDN博客
2011年09月13日 09:40:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1

### [Linux下简单的socket编程](http://blog.csdn.net/woods2001/article/details/4554816)
### [http://blog.csdn.net/woods2001/article/details/4554816](http://blog.csdn.net/woods2001/article/details/4554816)分类： [网络](http://blog.csdn.net/woods2001/article/category/571484)2009-09-15 14:5098人阅读[评论](http://blog.csdn.net/woods2001/article/details/4554816#comments)(0)[收藏](http://www.cnblogs.com/moonvan/admin/javascript:void(0);)[举报](http://blog.csdn.net/woods2001/article/details/4554816#report)
  其实，有过Window网络编程经验的人，会很快的熟悉简单的Linux网络编程，其中大部分有关socket的命令都是相似的，其中存在的区别有二点：
   1.windows有关socket编程需要加载相关的链接库，比如：ws2_32.lib，其中头文件是<winsock.h>,而Linux下却不需要加载这样的连接库，只要包含相关的头文件就行。
   2.Linux中的设备无关性，Linux把一切的设备，文件，套接字都当成是文件来读写，用户只要知道他们的文件描述符就可以对他们尽享相关的读写操作。
  了解了这两点后，我们就可以着手开始Linux网络编程了。
  首先，还有一点需要了解，一般一个网络程序模型都具有服务端和客户端，所以我们需要编程一个客户端和一个服务端才能进行相互的通信。
  头文件
  linux网络编程需要包含比较都的头文件，很多人都不知道哪些数据结构被包含在哪些头文件下
  但是我们一般包含这些头文件就基本可以满足要求了。
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
  这次我实验的是TCP连接循环服务器
  算法大致如下：
  socket();
  bind();
  listen();
  while(1)
  {
     accept();
     while(1)
     {
        read();
        printf();
        write(); 
     }
  }
  具体的每个函数的用法我在博客中的另一篇文章（转载）中有详细介绍
  客户端
  socket();
  conect()
  while(1)
  {
     scanf();
     write();
     read();
     printf();
  }
  我只有一台装有Linux的机器，还有一台只装了Windows，所以我利用windows telnet登陆到linux，在同一台机器上做完了这次实验
  实验结果大致如下：
  gcc server.c -o server
  gcc client.c -o client
  windows机器DOS中：./client
  connect successfully to 172.18.137.157
  please input communicate data: hello
  172.18.137.157 say:hello too
  Linux:./server 6000(端口号)
  connect successfully to 172.18.137.157
  172.18.137.157 say:hello
  please input communicate data:hello too
  如果有朋友需要在这方面交流，可以QQ我:276362907
