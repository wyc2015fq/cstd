# 《Linux程序设计》——套接字 - 一世豁然的专栏 - CSDN博客





2014年10月21日 17:28:25[一世豁然](https://me.csdn.net/Explorer_day)阅读数：739标签：[套接字																[linux](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=套接字&t=blog)
个人分类：[liunx应用编程](https://blog.csdn.net/Explorer_day/article/category/2276817)








一、套接字简介

 1、套接字是一种通信机制，凭借这种机制，客户/服务器系统的开发工作既可以在本地单机上进行，也可以跨网络进行。




 2、套接字的创建和使用与管道有区别，套接字明确地将客户和服务器区分开来。套接字机制可以实现多个客户链接到一个服务器。







二、套接字相关操作

 1、套接字应用程序维持一个链接的方法

   1）、首先，服务器应用程序用系统调用socket来创建一个套接字，它是系统分配给该服务器进程的类似文件描述符的资源，它不能与其他进程共享。

   2）、接下来，服务器进程用bind系统调用来给套接字命名。本地套接字的名字是Linux文件系统中的文件名，一般放在/tmp或/usr/tmp目录中/对于网络套接字，它的名字是

             与客户连接的特定网络相关额服务标识符。此标识符允许Linux将进入的针对特定端口号的连接转到正确的服务器进程。

   3）、然后，服务器进程就开始等待客户连接到这个命名套接字。系统调用listen的作用是，创建一个队列并将其用于存放来自客户的进入连接。服务器通过系统调用accept来


             接受客户的连接。

   4）、基于套接字系统的客户端更加简单。客户首先调用socket创建一个未命名套接字，然后将服务器额命名套接字作为一个地址来调用connect与服务器建立连接。

   5）、相关代码（P514-P516）




 2、套接字属性

   1）、套接字的特性由3个属性确定：域（domain）、类型（type）、协议（protocol）。

   2）、套接字可用地址作为他的名字。地址的格式随域的不同而不同。每个协议族还可以使用一个或多个地址族来定义地址格式。

   3）、套接字的域

       I、域指定套接字通信中使用的网络介质。

       II、AF_INET套接字：指的是Internet。因特网和Linux局域网都是使用该网络。因特网的底层协议——网际协议只有一个地址族，即ip地址。

       III、基于ISO标准协议的网络所使用的AF_ISO和用于施乐网络系统的AF_XNS域。

   4）、套接字类型

       I、因特网提供了两种通信机制：流（stream）和数据报（datagram）。

       II、流套接字提供的是一个有序、可靠、双向字节流的连接。流套接字由类型SOCK_STREAM指定，它们是在AF_INET域中通过TCP/IP链接实现。它们也是AF_UNIX域中常

             用的套接字类型。发送的数据可以确保不会丢失、复制或乱序到达。

       III、数据报套接字由类型SOCK_DCRAM指定的数据报套接字不建立一个连接。它可对发送的数据报的长度有限制。数据报套接字是在AF_DCRAM域中通过UDP/IP连接实

             现的，它提供的是一种无序的不可靠服务。

   5）、套接字协议




 3、创建套接字

**    #include<sys/types.h>**

**     intclude<sys/socket.h>**

**     int  socket(int  domain,  int  type,  int  protocol);**

   I、此系统调用可以用来创建一个描述符，该描述符可以用来访问该套接字。

   II、domain参数指定协议族（P519 表15-1）。最常用的套接字域是AF_UNIX和AF_INET，前者用于通过UNIX和Linux文件系统实现的本地套接字，后者用于UNIX网络套接

         字。

   III、type参数指定这个套接字的通信类型。取值包括SOCK_STREAM和SOCK_DGRAM。

   IV、protocol参数指定使用的协议。




 4、套接字地址

   1）、AF_UNIX域套接字来说，此地址由结构sockaddr_un来描述，该结构定义在sys/un.h头文件中。

**          struct  sockaddr_un{**

**          sa_family_t  sun_family;**

**          char  sun_path[];**

**          };**

        I、sun_family：指定类型成员。

        II、sun_path：套接字地址由此成员中的文件名所指定。

   2）、AF_INET域套接字来说，此地址由结构sockaddr_in来描述，该结构定义在netinet/in.h头文件中。

**      struct  sockaddr_in{**

**          short  int  sin_family;**

**          unsigned  short  int  sin_port;**

**          struct  in_addr;**

**          };**

          IP地址结构定义为：

**      struct  in_addr{**

**         unsigned  long  int  s_addr;**

**         };**




 5、套接字命名

**    #include<sys/socket.h>**

**    int  bind(int  socket,  const  struct  sockaddr  *address,  size_t  address_len);**

    I、此系统调用把参数address中的地址分配给与文件描述符socket关联的命名套接字

    II、address_len参数传递地址结构的长度。

    III、bind调用在成功时返回0，失败时返回-1并设置errno为一个值。（P521  表15-2  表15-3）




 6、创建套接字队列

**     #include<sys/socket.h>**

**     int  listen(int  socket,  int  backlog);**

   I、此函数将队列长度设置为backlog参数的值，常用值为5。

   II、此函数成功时返回0，失败时返回-1。错误代码包括EBADF、EINVAL和ENOTSOCK。




 7、接受链接

**     #include<sys/socket.h>**

**     int  accept(int  socket,  struct  sockadr  *address,  size_t  *address_len);**

    I、参数address_len指定客户结构的长度。如果客户地址的长度超过这个值，它将被切断。

    II、此系统调用只有当有客户程序试图连接到由socket参数指定的套接字上时才返回。此函数将创建一个新套接字来与客户进行通信，并且返回新套接字的描述符。




 8、请求链接

**  #include<sys/socket.h>**

**    int  connet(int  socket,  const  struct  sockaddr  *address,  size_t  address_len);**

   I、参数socket指定的套接字将连接到参数addreaa指定的服务器套接字。

   II、address指向的结构的长度由参数address_len指定。

   III、成功时，此函数返回0，失败时返回-1.可能的错误代码（P522  表15-4）




 9、关闭套接字（close）




 10、套接字通信

   1）、网络客户（P524代码）

   2）、网络服务器（[P524代码）




 11、主机字节序和网络字节序

   1）、为了使不同类型的计算机可以就通过网络传输的多字节整数的值达成一致，需要定义一个网络字节序。

    2）、客户和服务器程序必须在传输之前，将它们的内部整数表示方式转换为网络字节。以下函数可完成这一工作。

**   #include<netinet/in.h>**

**           unsigned  long  htonl(unsigned  long  int  hostlong);**

**           unsigned  short  htons(unsigned  short  int  hostlong);**

**           unsigned  long  ntohl(unsigned  long  int  netlong);**

**           unsigned short  ntohsl(unsigned  short  int  netlong);**


          I、以上函数将16位和32位整数在主机字节序和标准的网络字节序之间进行转换。







三、网络信息

 1、相关函数

   1）、**#include<netdb.h>**

**             struct  hostent  *gethostbyaddr(const  void  *adr,  size_t  len,  int  type);**

**             struct  hostent  *gethostbyname(const  char  *name);**

             I、以上函数至少会包含以下几个成员。

**        struct  hostent{**

**             char  *h_name;**

**             char  **h_aliases;**

**             int  h_addrtype;**

**             int  h_length;**

**            char  **h_addr_list;**

**             };**

             I、如果没有与我们查询的主机或地址相关的数据项，这些函数将返货一个空指针。

   2）、**#include<netdb.h>**

**             struct  servent  *getservbyname(const  char  *name,  const  char  *proto);**

**             struct  servent  *getservbyport(int  port,  const  char  *proto);**

             I、与服务及其关联端口号有关的信息可以通过以上函数来获取。

             II、proto参数指定用于连接该服务的协议，它的两个取值是tcp和udp，前者用于SOCK_STREAM类型的TCP链接，后者用于SOCK_DGRAM数据报。

             III、结构servent至少包含一下几个成员：

**       struct  servent{**

**             char  *s_name;**

**             char  **s_aliases;**

**             int  s_port;**

**             char  *s_proto;**

**             };**

   3）、**#include<arpa/inet.h>**

**             char  *inet_ntoa(struct  in_addr  in);**

             I、此函数的作用是将一个因特网主机地址转换为一个四元组格式的字符串。

             II、失败时返回-1，但POSIX规范并未定义任何错误。

   4）、**#include<unistd.h>**

**             int  gethostname(char  *name,  int  namelength);**

             I、此函数的作用是将当前主机的名字写入name指向的字符串中。主机名以null结尾。

             II、参数namelength指定了字符串name的长度，如果返回的主机名太长，它就会被截断。

             III、此函数在成功时返回0，失败时返回-1.

   5）、网络信息（P528-529代码）、链接到标准服务（P529-531代码）




 2、因特网守护进程（xinetd/inetd）




 3、套接字选项

**  #include<sys.socket.h>**

**     int  setsockopt(int  socket,  int  level,  int  option_name,  const  void  *option_value,  size_t  option_len);**

     I、此函数可以用来控制套接字选项。可以在协议层次的不同级别对选项进行设置。如果想要在套接字级别设置选项，就必须将level参数设置为SOL_SOCKET。如果想要在

          底层协议级别设置选项，就必须将level参数设置为该协议的编号。

     II、option_name参数指定要设置的选项；option_value参数的长度为option_len字节，它用于设置选项额新值，它被传递给底层协议的处理函数，并且不能被修改。

     III、在头文件sys/socket中定义的套接字级别选项（P534  表15-5）。SO_DEBUG和SO_KEEPALIVE用一个整数option_value来设置该选项的开（1）或关（0）。

            SO_LINGER需要使用一个头文件状态sys.socket.h中定义的linger结构，来定义该选项以及套接字关闭之前的拖延时间。

     IV、成功时返回0，失败时返回-1。







四、多客户

 1、可以同时服务多个客户的服务器（P535-536）。




 2、select系统调用

   1）、select系统调用允许程序同时在多个底层文件描述符上等待输入的到达（或输入的完成）。

   2）、select函数对数据结构fd_set进行操作，它是由打开的文件描述符构成的集合。有一组定义好的宏可以用来控制这些集合。

**    #incldue<sys/types.h>**

**           #include<sys/time.h> **

**           void  FD_ZERO(fd_set  *fdset);    **

**           void  FD_CLR(int  fd,  fd_set  *fdset); **

**           void  FD_SET(int  fd,  fd_set  *fdset)**

**           void  FD_ISSET(int  fd,  fd_set  *fdset); **


           I、FD_ZERO将fd_set初始化为空集和。

           II、FD_SET和 FD_CLR分别用于在集合中设置和清除由参数fd传递的文件描述符。

           III、如果FD_ISSET宏中由参数fd指向的文件描述符是由参数fdset指向的fd_set集合中的一个集合，FD_ISSET将返回非0值。

   3）、**#include<sys/type.h>**

**             #include<sys/time.h>**

**             int  select(int  nfds,  fd_set  *readfds,  fd_set  *writefds,  fd_set  *errorfds,  struct  timeval  *timeout);**

             I、此函数可以用超时值来防止无限期的阻塞。此超时值由一个timeval结构给出。这个结构定义在sys/time.h中，组成成员如下：

**         struct  timeval{**

**                  time_t  tv_sec;**

**                  long  tv_usec;**

**                  };**

             II、此函数用于测试的文件描述符集合中，是否有一个文件描述符已处于可读状态或可写状态或错误状态。它将阻塞以等待某个文件描述符进入上述这些状态。

             III、参数nfds指定需要测试的文件描述符数目，测试的文件描述符范围从0到nfds-1.3个描述符集合都可以被设为空指针，这表示不执行相应的测试。

             IV、select函数会发生以下情况时返回：readfds集合中有描述符可读、writefds集合中有描述符可写成或errorfds集合中有描述符遇到错误条件。如果这三种情况都没有

                    发生select将在timeout指定的超时时间经过后返回。如果timeout参数是一个空指针并且套接字上也没有任何活动，这个调用将一直阻塞下去。

             V、select调用返回状态发生变化的描述符总数。失败时返回-1并设置errno来描述错误。可能出现的错误有：EBADF（无效的描述符），EINTR（因终端而返

                    回），EINVAL（nfds或timeout取值错误）。




 3、多客户（P540-P542代码）







五、数据报

 1、**int  sendfrom(int  sockf,  void  *buffer,  size_t  len,  int  flags,  struct  socaddr  *to,  socklen_t  tolen);**

       I、此系统调用从buffer缓存区中给使用指定套接字地址的目标服务器发送一个数据报。

       II、当发生错误时，返回-1并设置errno。（P545  表15-7）





 2、**int recvfrom(int  sockfd,  void  *buffer,  size_t  len,  int  flags,  struct  sockadr  *from,  soclen_t  *fromlen);**

       I、此系统调用在套接字上等待从特定地址到来额数据报，并将它放入buffer缓冲区。

       II、当发生错误时，返回-1并设置errno。（P545  表15-7）





