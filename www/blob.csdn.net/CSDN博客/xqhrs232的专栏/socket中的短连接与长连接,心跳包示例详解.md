# socket中的短连接与长连接,心跳包示例详解 - xqhrs232的专栏 - CSDN博客
2016年04月18日 10:08:45[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：339
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://blog.chinaunix.net/uid-26000296-id-3758651.html](http://blog.chinaunix.net/uid-26000296-id-3758651.html)
相关文章
1、
# [Socket 长连接与短连接，心跳](http://blog.csdn.net/zdwzzu2006/article/details/7723738)----[http://blog.csdn.net/zdwzzu2006/article/details/772373](http://blog.csdn.net/zdwzzu2006/article/details/7723738)
# 2、[TCP/IP，http，socket，长连接，短连接](http://blog.csdn.net/zdwzzu2006/article/details/7723745)----[http://blog.csdn.net/zdwzzu2006/article/details/7723745](http://blog.csdn.net/zdwzzu2006/article/details/7723745)
3、[配置开发支持高并发TCP连接的Linux应用程序全攻略](http://blog.csdn.net/zdwzzu2006/article/details/7723724)----[http://blog.csdn.net/zdwzzu2006/article/details/7723724](http://blog.csdn.net/zdwzzu2006/article/details/7723724)
4、[tcp通道关闭时，发生了什么？](http://blog.csdn.net/zdwzzu2006/article/details/7745799)----[http://blog.csdn.net/zdwzzu2006/article/details/7745799](http://blog.csdn.net/zdwzzu2006/article/details/7745799)
5、[linux下的tcp协议栈超时重传机制](http://blog.csdn.net/zdwzzu2006/article/details/7745805)----[http://blog.csdn.net/zdwzzu2006/article/details/7745805](http://blog.csdn.net/zdwzzu2006/article/details/7745805)
6、[解决TCP连接数过多的问题](http://blog.csdn.net/zdwzzu2006/article/details/7745813)----[http://blog.csdn.net/zdwzzu2006/article/details/7745813](http://blog.csdn.net/zdwzzu2006/article/details/7745813)
TCP连接简介
当网络通信时采用TCP协议时，在真正的读写操作之前，server与client之间必须建立一个连接，
当读写操作完成后，双方不再需要这个连接时它们可以释放这个连接，
连接的建立是需要三次握手的，而释放则需要4次握手，
所以说每个连接的建立都是需要资源消耗和时间消耗的
经典的三次握手示意图：
![](http://www.2cto.com/uploadfile/2012/0629/20120629022304765.jpg)
经典的四次握手关闭图：
![](http://www.2cto.com/uploadfile/2012/0629/20120629022304632.jpg)
一、长连接与短连接
长连接: 指在一个TCP连接上可以连续发送多个数据包，
        在TCP连接保持期间，如果没有数据包发送，需要双方发检测包以维持此连接;
        一般需要自己做在线维持。 
短连接: 指通信双方有数据交互时，就建立一个TCP连接，数据发送完成后，则断开此TCP连接;
        一般银行都使用短连接。 
        它的优点是：管理起来比较简单，存在的连接都是有用的连接，不需要额外的控制手段 
比如http的，只是连接、请求、关闭，过程时间较短,服务器若是一段时间内没有收到请求即可关闭连接。 
其实长连接是相对于通常的短连接而说的，也就是长时间保持客户端与服务端的连接状态。
长连接与短连接的操作过程 
通常的短连接操作步骤是： 
  连接→数据传输→关闭连接；
而长连接通常就是： 
  连接→数据传输→保持连接(心跳)→数据传输→保持连接(心跳)→……→关闭连接； 
这就要求长连接在没有数据通信时，定时发送数据包(心跳)，以维持连接状态，
短连接在没有数据传输时直接关闭就行了
什么时候用长连接，短连接？
长连接多用于操作频繁，点对点的通讯，而且连接数不能太多情况。
每个TCP连接都需要三步握手，这需要时间，如果每个操作都是先连接，再操作的话那么处理速度会降低很多，
所以每个操作完后都不断开，下次次处理时直接发送数据包就OK了，不用建立TCP连接。
例如：数据库的连接用长连接， 
如果用短连接频繁的通信会造成socket错误，而且频繁的socket 创建也是对资源的浪费。
二、发送接收方式
1、异步 
报文发送和接收是分开的，相互独立的，互不影响。这种方式又分两种情况： 
(1)异步双工：接收和发送在同一个程序中，由两个不同的子进程分别负责发送和接收 
(2)异步单工：接收和发送是用两个不同的程序来完成。 
2、同步 
报文发送和接收是同步进行，既报文发送后等待接收返回报文。 
同步方式一般需要考虑超时问题，即报文发出去后不能无限等待，需要设定超时时间，
超过该时间发送方不再等待读返回报文，直接通知超时返回。
在长连接中一般是没有条件能够判断读写什么时候结束，所以必须要加长度报文头。
读函数先是读取报文头的长度，再根据这个长度去读相应长度的报文。
三. 单工、半双工和全双工
根据通信双方的分工和信号传输方向可将通信分为三种方式：
单工、
半双工、
全双工。
在计算机网络中主要采用双工方式，其中：
局域网采用半双工方式，
城域网和广域网采用全双年方式。   
1. 单工(Simplex)方式：
通信双方设备中发送器与接收器分工明确，只能在由发送器向接收器的单一固定方向上传送数据。
采用单工通信的典型发送设备如早期计算机的读卡器，典型的接收设备如打印机。   
2. 半双工(Half Duplex)方式：
通信双方设备既是发送器，也是接收器，两台设备可以相互传送数据，但某一时刻则只能向一个方向传送数据。
例如，步话机是半双工设备，因为在一个时刻只能有一方说话。   
3. 全双工(Full Duplex)方式：
通信双方设备既是发送器，也是接收器，两台设备可以同时在两个方向上传送数据。
例如，电话是全双工设备，因为双方可同时说话。
而像WEB网站的http服务一般都用短链接，因为长连接对于服务端来说会耗费一定的资源，
而像WEB网站这么频繁的成千上万甚至上亿客户端的连接用短连接会更省一些资源，
如果用长连接，而且同时有成千上万的用户，如果每个用户都占用一个连接的话，那可想而知吧。
所以并发量大，但每个用户无需频繁操作情况下需用短连好。
总之，长连接和短连接的选择要视情况而定。
四、一个最简单的长连接与心跳保持的示例程序
/*!
 ******************************************************************************
 * \File
 *  
 * \Brief
 *   
 * \Author
 *  Hank
 ******************************************************************************
 */
- #include <stdio.h>
- 
#include <string.h>
- 
#include <errno.h>
- 
#include <sys/socket.h>
- 
#include <resolv.h>
- 
#include <stdlib.h>
- 
#include <netinet/in.h>
- 
#include <arpa/inet.h>
- 
#include <arpa/inet.h>
- 
#include <unistd.h>
- 
#include <sys/time.h>
- 
#include <sys/types.h>
#define MAXBUF 1024
int main(int argc, char **argv)
{
  int sockfd, len;
  struct sockaddr_in dest;
  char buffer[MAXBUF];
  char heartbeat[20] = "hello server";
  fd_set rfds;
  struct timeval tv;
  int retval, maxfd = -1;
  if (argc != 3)
  {
    printf("error! the right format should be : \
          \n\t\t%s IP port\n\t eg:\t%s127.0.0.1 80\n",
          argv[0], argv[0]);
    exit(0);
  }
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("Socket");
    exit(errno);
  }
  bzero(&dest, sizeof(dest));
  dest.sin_family = AF_INET;
  dest.sin_port = htons(atoi(argv[2]));
  memset(&(dest.sin_zero), 0, 8);
  if (inet_aton(argv[1], (struct in_addr*)&dest.sin_addr.s_addr) == 0)
  {
    perror(argv[1]);
    exit(errno);
  }
  if (connect(sockfd, (struct sockaddr*)&dest, sizeof(dest)) != 0)
  {
    perror("Connect");
    exit(errno);
  }
  printf("\nReady to start chatting.\n\
        Direct input messages and \n\
        enter to send messages to the server\n");
  while (1)
  {
    FD_ZERO(&rfds);
    FD_SET(0, &rfds);
    maxfd = 0;
    FD_SET(sockfd, &rfds);
    if (sockfd > maxfd)
      maxfd = sockfd;
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    retval = select(maxfd+1, &rfds, NULL, NULL, &tv);
    if (retval == -1)
    {
      printf("Will exit and the select is error! %s", strerror(errno));
      break;
    }
    else if (retval == 0)
    {
      //printf("No message comes, no buttons, continue to wait ...\n");
      len = send(sockfd, heartbeat, strlen(heartbeat), 0);
      if (len < 0)
      {
        printf("Message '%s' failed to send ! \
              The error code is %d, error message '%s'\n",
              heartbeat, errno, strerror(errno));
        break;
      }
      else
      {
        printf("News: %s \t send, sent a total of %d bytes!\n",
              heartbeat, len);
      }
      continue;
    }
    else
    {
      if (FD_ISSET(sockfd, &rfds))
      {
        bzero(buffer, MAXBUF+1);
        len = recv(sockfd, buffer, MAXBUF, 0);
        if (len > 0)
        {
          printf("Successfully received the message: '%s',%d bytes of data\n",
                  buffer, len);
        }
        else
        {
          if (len < 0)
              printf("Failed to receive the message! \
                    The error code is %d, error message is '%s'\n",
                    errno, strerror(errno));
          else
              printf("Chat to terminate!\n");
          break;
        }
      }
      if (FD_ISSET(0, &rfds))
      {
        bzero(buffer, MAXBUF+1);
        fgets(buffer, MAXBUF, stdin);
        if (!strncasecmp(buffer, "quit", 4))
        {
          printf("Own request to terminate the chat!\n");
          break;
        }
        len = send(sockfd, buffer, strlen(buffer)-1, 0);
        if (len < 0)
        {
          printf("Message '%s' failed to send ! \
                The error code is %d, error message '%s'\n",
                buffer, errno, strerror(errno));
          break;
        }
        else
        {
          printf("News: %s \t send, sent a total of %d bytes!\n",
                buffer, len);
        }
      }
    }
  }
  close(sockfd);
  return 0;
}
第二卦：《坤卦》 
坤：元，亨，利牝马之贞。君子有攸往，先迷，后得主，利。西南得朋，东北丧朋。安贞吉。
【白话】《坤卦》象征地：元始，亨通，如果像雌马那样柔顺，则是吉利的。
君子从事某项事业，虽然开始时不知所从，但结果会是有利的。如往西南方，则会得到朋友的帮助。
如往东南方，则会失去朋友的帮助。如果保持现状，也是吉利的。
《象》曰：地势坤，君子以厚德载物。
【白话】《象辞》说：坤象征大地，君子应效法大地，胸怀宽广，包容万物。
