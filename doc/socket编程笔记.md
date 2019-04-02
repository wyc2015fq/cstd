# socket编程笔记





# socket头文件



## socket  编程中需要用到的头文件



| 头文件       | 解释                                       |
| ------------ | ------------------------------------------ |
| sys/types.h  | 数据类型定义                               |
| sys/socket.h | 提供socket函数及数据结构                   |
| netinet/in.h | 定义数据结构sockaddr_in                    |
| arpa/inet.h  | 提供IP地址转换函数                         |
| netdb.h      | 提供设置及获取域名的函数                   |
| sys/ioctl.h  | 提供对I/O控制的函数                        |
| sys/poll.h   | 提供socket等待测试机制的函数               |
|              | 其他在网络程序中常见的头文件               |
| unistd.h     | 提供通用的文件、目录、程序及进程操作的函数 |
| errno.h      | 提供错误号errno的定义，用于错误处理        |
| fcntl.h      | 提供对文件控制的函数                       |
| time.h       | 提供有关时间的函数                         |
| crypt.h      | 提供使用DES加密算法的加密函数              |
| pwd.h        | 提供对/etc/passwd文件访问的函数            |
| shadow.h     | 提供对/etc/shadow文件访问的函数            |
| pthread.h    | 提供多线程操作的函数                       |
| signal.h     | 提供对信号操作的函数                       |
| sys/wait.h   | 进程等待                                   |
| sys/ipc.h    | 进程间通讯（IPC）                          |
| sys/shm.h    | 共享内存的函数                             |

 

## 建议：在编写网络程序时，可以直接使用下面这段头文件代码

```c
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <malloc.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <stdarg.h>
#include <fcntl.h>
```

## 涉及到用户权限及密码验证问题时加入如下语句：

```c
#include <shadow.h>
#include <crypt.h>
#include <pwd.h>
```

## 需要注意的是，应该在编译时链接加密算法库，即增加编译选项：

```
-lcrypt
```



## 涉及到文件及时间操作加入如下语句：

```
#include <sys/time.h>
#include <utime.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/file.h>
```

 

## 涉及到多进程操作时加入如下语句：

```c
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
```

 

## 涉及到多线程操作时加入如下语句：

```c
#include <pthread.h>
#include <sys/poll.h>
```

## 需要注意的是，应该在编译时链接线程库，即增加编译选项：

```
-lthread
```

 





# sockaddr 和 sockaddr_in 的区别(转)



  

Sockfd是调用socket函数返回的socket描述符,my_addr是一个指向包含有本机IP地址及端口号等信息的sockaddr类型的指针；addrlen常被设置为sizeof(struct sockaddr)。  
```c
//struct sockaddr结构类型是用来保存socket信息的：  
struct sockaddr {  
 　 unsigned short sa_family; /* 地址族， AF_xxx */  
    char sa_data[14]; /* 14 字节的协议地址 */  
};  
　　//sa_family一般为AF_INET，代表Internet（TCP/IP）地址族；sa_data则包含该socket的IP地址和端口号。  
　　//另外还有一种结构类型：  
struct sockaddr_in {  
    short int sin_family; /* 地址族 */  
　　 unsigned short int sin_port; /* 端口号 */  
　　 struct in_addr sin_addr; /* IP地址 */  
　　 unsigned char sin_zero[8]; /* 填充0 以保持与struct sockaddr同样大小 */  
};  
```
　　这个结构更方便使用。sin_zero用来将sockaddr_in结构填充到与struct sockaddr同样的长度，可以用bzero()或memset()函数将其置为零。指向sockaddr_in 的指针和指向sockaddr的指针可以相互转换，这意味着如果一个函数所需参数类型是sockaddr时，你可以在函数调用的时候将一个指向sockaddr_in的指针转换为指向sockaddr的指针；或者相反。



你只要记住，填值的时候使用sockaddr_in结构，而作为函数的 
参数传入的时候转换成sockaddr结构就行了，毕竟都是16个字符 
长。 

```c
struct in_addr {
    union {
         struct { u_char s_b1,s_b2,s_b3,s_b4; } S_un_b;
         struct { u_short s_w1,s_w2; } S_un_w;
         u_long S_addr;
    } S_un;
};
```









# Tcp 断开连接



## TCP状态转移要点

​    TCP协议规定，对于已经建立的连接，网络双方要进行四次握手才能成功断开连接，如果缺少了其中某个步骤，将会使连接处于假死状态，连接本身占用的资源不会被释放。网络服务器程序要同时管理大量连接，所以很有必要保证无用连接完全断开，否则大量僵死的连接会浪费许多服务器资源。在众多TCP状态中，最值得注意的状态有两个：CLOSE_WAIT和TIME_WAIT。

### 1、LISTENING状态

　　FTP服务启动后首先处于侦听（LISTENING）状态。

### 2、ESTABLISHED状态

　　ESTABLISHED的意思是建立连接。表示两台机器正在通信。

### 3、CLOSE_WAIT

​    对方主动关闭连接或者网络异常导致连接中断，这时我方的状态会变成CLOSE_WAIT 此时我方要调用close()来使得连接正确关闭

### 4、TIME_WAIT

​    我方主动调用close()断开连接，收到对方确认后状态变为TIME_WAIT。TCP协议规定TIME_WAIT状态会一直持续2MSL(即两倍的分段最大生存期)，以此来确保旧的连接状态不会对新连接产生影响。处于TIME_WAIT状态的连接占用的资源不会被内核释放，所以作为服务器，在可能的情况下，尽量不要主动断开连接，以减少TIME_WAIT状态造成的资源浪费。

​    目前有一种避免TIME_WAIT资源浪费的方法，就是关闭socket的LINGER选项。但这种做法是TCP协议不推荐使用的，在某些情况下这个操作可能会带来错误。



## 在连接撤销过程中，有如下四个过程:   


                          
1.         HOST1上的应用程序关闭己方的连接导致TCP发送一个FIN消息给HOST2。
2.         HOST2发送一个确认消息给HOST1,并且HOST2把FIN作为EOF递交给HOST2上的应用程序。
3.         一段时间过后，HOST2上的应用程序关闭它那边的连接，引发一个FIN消息给HOST1。
4.         HOST1给HOST2发送一个确认消息，然后HOST2关闭连接并释放资源，然而，HOST1却没有关闭连接，而是进入了TIME_WAIT状态，并为两个最大段生存时间(2MSL)保留在此状态.



## 为什么需要time_wait?

\1.       因为在第四步的时候，HOST1发送的ACK可能丢失并导致HOST2重新发送FIN消息，TIME_WAIT维护连接状态.
       如果执行主动关闭的一方HOST1 不进入到TIME_WAIT状态就关闭连接那会发生什么呢？当重传的FIN消息到达时，因为TCP已经不再有连接的信息了，所以就用RST(重新启动)消息应答，导致HOST2进入错误的状态而不是有序终止状态，如果发送最后ACK消息的一方处于TIME_WAIT状态并仍然记录着连接的信息，它就可以正确的响应对等方HOST2的FIN消息了.
\2.       TIME_WAIT为连接中”离群的段”提供从网络中消失的时间.
         考虑一下，如果延迟或者重传段在连接关闭后到达时会发生什么呢？通常情况下，因为TCP仅仅丢弃该数据并响应RST消息，所以这不会造成任何问题。当RST消息到达发出延时段的主机时，因为该主机也没有记录连接的任何信息，所以它也丢弃该段。然而，如果两个相同主机之间又建立了一个具有相同端口号的新连接，那么离群的段就可能被看成是新连接的，如果离群的段中数据的任何序列号恰恰在新连接的当前接收窗口中，数据就会被重新接收，其结果就是破坏新连接。










## 一、TCP连接关闭的几种方式：

1、“正常”关闭：调用close()关闭socket、没close但进程正常结束(当然这是不应该的做法)、进程core掉、在shell命令行中kill掉进程，都可抽象成“正常”关闭。因为即使core掉，内核也会马上帮应用程序回收(close)socket文件描述符。

​     “正常”关闭，默认情况下(非默认即设置Linger下面会介绍)，关闭端即客户端TCP层会发FIN包，对端即服务器TCP层收到后，回ACK，客户端进入FIN_WAIT2状态。此时，TCP终止连接的4个分组中服务器应该发的第3个分组FIN包，其TCP层是不会主动发的，只有服务器端socket“正常”关闭，才会发出这个FIN包。至此，客户端进入TIME_WAIT状态。

2、“非”正常关闭：客户端崩溃了，此时肯定发不出FIN包了(当然啦，内核都没机会帮应用程序回收资源了)。这种情况，服务器端有如下两种情况：

​    A、服务器send数据，因为客户端已经崩溃，服务器收不到ACK自然会不停的重传。源自

​        Berkeley的重传机制，重传8次，相对第一次传的15分钟后仍没收到ACK，则返回

​        ETIMEDOUT或EHOSTUNREAC错误。如果服务器不理会这个错误，再次调用send，则

​        立马返回Broken Pipe错误。    

​       注：15分钟超时可以在 /proc/sys/net/ipv4/tcp_retries2 中修改

   B、 服务器不发任何数据了，那只有靠应用层心跳检测机制或Keepalive，来发觉TCP断连了。

## 二、SO_LINGER套接口选项

​           A、l_onoff设置为0，这也是默认情况，函数close()是立即返回的，然后TCP连接双方是通过

​                FIN、ACK4分组来终止TCP连接的。当然,发送缓冲区还有数据的话，系统将试着将这些数据

​                发送到对方。

​           B、l_onoff非0，l_linger设置0，函数close()立即返回，并发送RST终止连接，发送缓冲区的数据丢弃。

​           C、l_onoff非0，l_linger非0，函数close()不立即返回,而是在(a)发送缓冲区数据发送完并得到确认

​                 (b)l_linger延迟时间到,l_linger时间单位为微妙。两者之一成立时返回。如果在发送缓冲区数据发送

​                完并被确认前延迟时间到的话，close返回EWOULDBLOCK(或EAGAIN)错误。

## 三、客户端TCP连接“正常”关闭，服务器的几种情况：

​          情形 客户端l_onoff设置为0， 
“正常”关闭 客户端l_onoff非0，l_linger设置0,“正常”关闭 
服务器阻塞模式send，正阻塞在send函数未返回 客户端TCP发送FIN，服务器send函数返回成功(返回字节数是实际拷贝到发送缓冲区的字节数)。客户端发送RST。如果服务器再次调用send,将返回errno[32]:Broken pipe 客户端TCP发送RST，服务器函数返回成功(返回字节数是实际拷贝到发送缓冲区的字节数)。若服务器再次调用send，则返回-1，errno[104]:Connection reset by peer。若再次调用send，则返回-1，errno[32]:Broken pipe 
服务器空闲 客户端TCP发送FIN，若服务器没理会而调用send，客户端发送RST，send返回-1，errno[32]:Broken pipe 客户端TCP发送RST,若服务器没理会而调用send，send返回-1，errno[104]:Connection reset by peer。若再次调用send，则返回-1，errno[32]:Broken pipe

总之，1、收到对端RST后，仍然调入send()，则返回Connection reset by peer,再次调用send()，则返回Broken pipe

​         2、收到对端FIN后，仍然调研哪个send(),直接返回Broken pipe

 

 

## setsockopt 设置 SO_LINGER 选项

 

   此选项指定函数close对面向连接的协议如何操作（如TCP）。内核缺省close操作是立即返回，如果有数据残留在套接口缓冲区中则系统将试着将这些数据发送给对方。

 

SO_LINGER选项用来改变此缺省设置。使用如下结构：

```c
struct linger {
     int l_onoff; /* 0 = off, nozero = on */
     int l_linger; /* linger time */
};
```

 

有下列三种情况：

1、设置 l_onoff为0，则该选项关闭，l_linger的值被忽略，等于内核缺省情况，close调用会立即返回给调用者，如果可能将会传输任何未发送的数据；

 

2、设置 l_onoff为非0，l_linger为0，则套接口关闭时TCP夭折连接，TCP将丢弃保留在套接口发送缓冲区中的任何数据并发送一个RST给对方，而不是通常的四分组终止序列，这避免了TIME_WAIT状态；

 

3、设置 l_onoff 为非0，l_linger为非0，当套接口关闭时内核将拖延一段时间（由l_linger决定）。如果套接口缓冲区中仍残留数据，进程将处于睡眠状态，直 到（a）所有数据发送完且被对方确认，之后进行正常的终止序列（描述字访问计数为0）或（b）延迟时间到。此种情况下，应用程序检查close的返回值是非常重要的，如果在数据发送完并被确认前时间到，close将返回EWOULDBLOCK错误且套接口发送缓冲区中的任何数据都丢失。close的成功返回仅告诉我们发送的数据（和FIN）已由对方TCP确认，它并不能告诉我们对方应用进程是否已读了数据。如果套接口设为非阻塞的，它将不等待close完成。

 

注释：l_linger的单位依赖于实现: 4.4BSD假设其单位是时钟滴答（百分之一秒），但Posix.1g规定单位为秒。

 

下面的代码是一个使用SO_LINGER选项的例子，使用30秒的超时时限：

```c
#define TRUE     1
#define FALSE    0
int z; /* Status code*/
int s;       /* Socket s */
struct linger so_linger;
...
so_linger.l_onoff = TRUE;
so_linger.l_linger = 30;
z = setsockopt(s,
    SOL_SOCKET,
    SO_LINGER,
    &so_linger,
    sizeof so_linger);
if ( z )
   perror("setsockopt(2)");

//下面的例子显示了如何设置SO_LINGER的值来中止套接口s上的当前连接：
#define TRUE     1
#define FALSE    0
int z; /* Status code */
int s;       /* Socket s */
struct linger so_linger;
...
so_linger.l_onoff = TRUE;
so_linger.l_linger = 0;
z = setsockopt(s,
    SOL_SOCKET,
    SO_LINGER,
    &so_linger,
    sizeof so_linger);
if ( z )
    perror("setsockopt(2)");
    close(s); /* Abort connection */
```

在上面的这个例子中，当调用close函数时，套接口s会立即中止。中止的语义是通过将超时值设置为0来实现的。


```c
/** WINDOWS **/

 

/* 当连接中断时，需要延迟关闭(linger)以保证所有数据都被传输，所以需要打开SO_LINGER这个选项；  
 * //注：大致意思就是说SO_LINGER选项用来设置当调用closesocket时是否马上关闭socket； 
 * linger的结构在/usr/include/linux/socket.h中定义：//注：这个结构就是SetSocketOpt中的Data的数据结构 
 *　 struct linger 
 *　 { 
 *　　 int l_onoff;　 /* Linger active */       //低字节，0和非0，用来表示是否延时关闭socket
 *　　 int l_linger; /* How long to linger */   //高字节,延时的时间数，单位为秒
 *　 }; 
 *　 如果l_onoff为0，则延迟关闭特性就被取消。
 *   如果非零，则允许套接口延迟关闭； l_linger字段则指明延迟关闭的时间 
 */
```

更具体的描述如下：
1、若设置了SO_LINGER（亦即linger结构中的l_onoff域设为非零），并设置了零超时间隔，则closesocket()不被阻塞立即执行，不论是否有排队数据未发送或未被确认。这种关闭方式称为“强制”或“失效”关闭，因为套接口的虚电路立即被复位，且丢失了未发送的数据。在远端的recv()调用将以WSAECONNRESET出错。

2、若设置了SO_LINGER并确定了非零的超时间隔，则closesocket()调用阻塞进程，直到所剩数据发送完毕或超时。这种关闭称为“优雅”或“从容”关闭。请注意如果套接口置为非阻塞且SO_LINGER设为非零超时，则closesocket()调用将以WSAEWOULDBLOCK错误返回。

3、若在一个流类套接口上设置了SO_DONTLINGER（也就是说将linger结构的l_onoff域设为零），则closesocket()调用立即返回。但是，如果可能，排队的数据将在套接口关闭前发送。请注意，在这种情况下WINDOWS套接口实现将在一段不确定的时间内保留套接口以及其他资源，这对于想用所以套接口的应用程序来说有一定影响。


SO_DONTLINGER 若为真，则SO_LINGER选项被禁止。
SO_LINGER延迟关闭连接 struct linger上面这两个选项影响close行为；


|选项|          间隔|    关闭方式|  等待关闭与否|
|----|----|----|----|
|SO_DONTLINGER|   不关心|     优雅|         否|
|SO_LINGER |       零 |       强制|         否|
|SO_LINGER|       非零 |      优雅|         是|

 











# EINTER信号的详解

 



再次分析非阻塞性sock的读取

read（）返回值有三：
\1. 大于0（简单）
   继续读

\2. 等于0（简单）
   对方关闭，自己也需要返回错误，然后关闭。

\3. 小于0（）
   小于零的分析见下面的一些摘录。

 


1.
这个程序在多个linux服务器上运行，都没有问题，但最近在一台Red   Hat   Linux   release   8.0   (Psyche)   Kernel   2.4.18-14   上总是出现通过select调用read（）时返回值-1,errno值为11（EWOULDBLOCK），
出错信息是Resource   temporarily   unavailable。   
  出现该错误，如果多次重读，偶尔会出现读取到异常包的现象。   
  请问这是什么原因，是不是和操作系统有关？
2.
4 楼x86（大雪）
回复于 2005-11-08 10:27:08 得分 0

EAGAIN=EWOULDBLOCK(BSD风格)   
    
  此错误由在非阻塞套接字上不能立即完成的操作返回，例如，当套接字上没有排队数据可读时调用了recv()函数。此错误不是严重错误，相应操作应该稍后重试。对于在非阻塞   SOCK_STREAM套接字上调用connect()函数来说，报告EWOULDBLOCK是正常的，因为建立一个连接必须花费一些时间。

3.
7 楼do_do（do_do）
回复于 2005-11-08 15:22:59 得分 0

大雪是对的，EWOULDBLOCK的意思是如果你不把socket设成非阻塞(即阻塞)模式时，这个读操作将阻塞，也就是说数据还未准备好(但系统知道数据来了，所以select告诉你那个socket可读)。使用非阻塞模式做I/O操作的细心的人会检查errno是不是EAGAIN、EWOULDBLOCK、EINTR，如果是就应该重读，一般是用循环。如果你不是一定要用非阻塞就不要设成这样，这就是为什么系统的默认模式是阻塞。 

4.
在Linux进行非阻塞的socket接收数据时经常出现Resource temporarily unavailable，errno代码为11(EAGAIN)，这是什么意思？

　　这表明你在非阻塞模式下调用了阻塞操作，在该操作没有完成就返回这个错误，这个错误不会破坏socket的同步，不用管它，下次循环接着recv就可以。对非阻塞socket而言，EAGAIN不是一种错误。在VxWorks和Windows上，EAGAIN的名字叫做EWOULDBLOCK。

另外，如果出现EINTR即errno为4，错误描述Interrupted system call，操作也应该继续。

　　最后，如果recv的返回值为0，那表明连接已经断开，我们的接收操作也应该结束

 

 

 

 

 

10.
宏: int EAGAIN
    资源暂时不可用。你再次调用函数有可能成功。宏EWOULDBLOCK是
EAGAIN的另一个名字。它们在GNU库中总是相同的。
    这个错还有可能在一些其他情况发生。
    *对非阻塞模式的对象进行某个会阻塞的操作。再次做同样的操作
就会阻塞直到某种条件使它可以读，写，连(不管是什么操作)。你可以使用
selece来找出哪些操作是可能的，参见等待输入输出(Waiting for Input or
Output).移植性考虑,在许多老的Unix系统上，这种错误被指示为与EAGAIN
不同的EWOULDBLOCK。为了便于移植，你可以同时检验两种错误并做同样处理。
    *某个资源上的故障使操作不能进行。例如fork有可能返回着个错误，
这也表示这个故障可以被克服，所以你的程序可以以后尝试这个操作。停几秒
让其他进程释放资源然后再试也许是个好主意。这种故障可能很严重并会影响
整个系统，所以通常交互式的程序会报告用户这个错误并返回命令循环。

宏: int EWOULDBLOCK
    对 GNU C库来说，这只是EAGAIN的另一个名字，在每一个操作系统上
这两个值都是一样的。
    有些老的UNIX系统上这两个值不一样。

宏: int EINPROGRESS
    对某个设置成非阻塞模式的对象的操作不能马上结束。某些函数可能
永远是阻塞的（例如connect, 参见建立连接Making a connection）,它们不会
返回EAGAIN。它们返回EINPROGRESS表示操作已经开始并需要一些时间来完成。
在操作完成以前对对象的操作会返回EALREADY。你可以使用select来检测什么时
候等待的操作被完成。参见等待输入输出(Waiting for Input or Output).

 

11.
\#define SERV_PORT    5000
\#define MAXLINE        1000

 


/*从sock读N个字节,成功返回n>=0 不成功-1*/
static ssize_t _tcp_readn(int fd, void *vptr, size_t n)
{
    size_t nleft;
    ssize_t nread;
    char *ptr;


    ptr = (char*)vptr;
    nleft = n;
    while (nleft > 0)
    {
        nread = read(fd, ptr, nleft);
        if (nread < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }
            else if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                continue;
            }
            else
            {
                return  - 1;
            }
        }
        else if (nread == 0)
        {
            //closed
            break;
        }

​        nleft -= nread;
​        ptr += nread;
​    }

​    return (n - nleft);
}

/*写sock N个字节,成功返回n>=0 不成功-1*/
static ssize_t _tcp_writen(int fd, const void *vptr, size_t n)
{
    size_t nleft;
    ssize_t nwritten;
    const char *ptr;

​    ptr = (char*)vptr;
​    nleft = n;
​    while (nleft > 0)
​    {
​        nwritten = write(fd, ptr, nleft);
​        if (nwritten <= 0)
​        {
​            if (errno == EINTR)
​            {
​                continue;
​            }
​            else if (errno == EAGAIN || errno == EWOULDBLOCK)
​            {
​                continue;
​            }
​            else
​            {
​                return  - 1;
​            }
​        }

​        nleft -= nwritten;
​        ptr += nwritten;
​    }

​    return n;
}

11.
关于EINTR的问题

我想问的是，，什么时候会产生这个EINTR信号呢？我的程序代码里并没有产生该信号。 
我以前碰到一个程序，跟这个结构差不多，，是一个while循环，总是报错说，产生了EINTR信号。所以

我一直不明白，，到底是什么产生了这个信号？？？请大家赐教。

回答：
自己系统中，可能非有意而为产生信号的原因：

进程调试 
控制台切换 
任务切换 
伪终端断开 
异步IO 
父进程死亡导致孤儿进程 
子进程退出 
系统重启或关闭

只要保证你的程序不受上述原因影响可能就不需要再检查信号打断错误了。

等等，这些事件发生，会产生打断（打扰）EINTR信号，忽略继续处理就可以了。

感悟：
read（或者write）时,应从对方Ewouldblock/Eagain,还有自己EINTer信号，
两个角度考虑。
有整体性的逻辑。

 

为什么会有Ewouldblock/Eagain，上面的某个已经说明了。好像是默认是阻塞的，该
非阻塞时，调用阻塞函数而又没数据时，等等。


15.
如何设置socket的Connect超时(linux)

1.首先将标志位设为Non-blocking模式，准备在非阻塞模式下调用connect函数
2.调用connect，正常情况下，因为TCP三次握手需要一些时间；而非阻塞调用只要不能立即完成就会返回错误，所以这里会返回EINPROGRESS，表示在建立连接但还没有完成。

已经说得很明白了，判断EINprocess错误。
man：
If the connection cannot be established  immediately  and  O_NONBLOCK  is  set  for  the  file
       descriptor  for  the socket, connect() shall fail and set errno to [EINPROGRESS], but the con-
       nection request shall not be aborted, and the connection shall be established  asynchronously.











# setsockopt()改善程序的健壮性



不断的收到coolmei25 (梅生)的答谢，我都不好意思了（我都没帮到他),下面
写出我在网络编程中的一点心得体会，希望对他(^_^也对大家)有帮助:

1. 如果在已经处于 ESTABLISHED状态下的socket(一般由端口号和标志符区分）调用
closesocket（一般不会立即关闭而经历TIME_WAIT的过程）后想继续重用该socket：
```c
BOOL bReuseaddr=TRUE;
setsockopt(s,SOL_SOCKET ,SO_REUSEADDR,(const char*)&bReuseaddr,sizeof(BOOL));
```
2. 如果要已经处于连接状态的soket在调用closesocket后强制关闭，不经历
TIME_WAIT的过程：
```c
BOOL bDontLinger = FALSE; 
setsockopt(s,SOL_SOCKET,SO_DONTLINGER,(const char*)&bDontLinger,sizeof(BOOL));
```
3.在send(),recv()过程中有时由于网络状况等原因，发收不能预期进行,而设置收发时限：
```c
int nNetTimeout=1000;//1秒
//发送时限
setsockopt(socket，SOL_S0CKET,SO_SNDTIMEO，(char *)&nNetTimeout,sizeof(int));
//接收时限
setsockopt(socket，SOL_S0CKET,SO_RCVTIMEO，(char *)&nNetTimeout,sizeof(int));
```
4.在send()的时候，返回的是实际发送出去的字节(同步)或发送到socket缓冲区的字节
(异步);系统默认的状态发送和接收一次为8688字节(约为8.5K)；在实际的过程中发送数据
和接收数据量比较大，可以设置socket缓冲区，而避免了send(),recv()不断的循环收发：
```c
// 接收缓冲区
int nRecvBuf=32*1024;//设置为32K
setsockopt(s,SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(int));
//发送缓冲区
int nSendBuf=32*1024;//设置为32K
setsockopt(s,SOL_SOCKET,SO_SNDBUF,(const char*)&nSendBuf,sizeof(int));
```
5. 如果在发送数据的时，希望不经历由系统缓冲区到socket缓冲区的拷贝而影响
程序的性能：
```c
int nZero=0;
setsockopt(socket，SOL_S0CKET,SO_SNDBUF，(char *)&nZero,sizeof(nZero));
```
6.同上在recv()完成上述功能(默认情况是将socket缓冲区的内容拷贝到系统缓冲区)：
```c
int nZero=0;
setsockopt(socket，SOL_S0CKET,SO_RCVBUF，(char *)&nZero,sizeof(int));
```
7.一般在发送UDP数据报的时候，希望该socket发送的数据具有广播特性：
```c
BOOL bBroadcast=TRUE; 
setsockopt(s,SOL_SOCKET,SO_BROADCAST,(const char*)&bBroadcast,sizeof(BOOL));
```
8.在client连接服务器过程中，如果处于非阻塞模式下的socket在connect()的过程中可
以设置connect()延时,直到accpet()被呼叫(本函数设置只有在非阻塞的过程中有显著的
作用，在阻塞的函数调用中作用不大)
```c
BOOL bConditionalAccept=TRUE;
setsockopt(s,SOL_SOCKET,SO_CONDITIONAL_ACCEPT,(const char*)&bConditionalAccept,sizeof(BOOL));
```
9.如果在发送数据的过程中(send()没有完成，还有数据没发送)而调用了closesocket(),以前我们
一般采取的措施是"从容关闭"shutdown(s,SD_BOTH),但是数据是肯定丢失了，如何设置让程序满足具体
应用的要求(即让没发完的数据发送出去后在关闭socket)？
```c
struct linger {
u_short l_onoff;
u_short l_linger;
};
linger m_sLinger;
m_sLinger.l_onoff=1;//(在closesocket()调用,但是还有数据没发送完毕的时候容许逗留)
// 如果m_sLinger.l_onoff=0;则功能和2.)作用相同;
m_sLinger.l_linger=5;//(容许逗留的时间为5秒)
setsockopt(s,SOL_SOCKET,SO_LINGER,(const char*)&m_sLinger,sizeof(linger));
```
Note:1.在设置了逗留延时，用于一个非阻塞的socket是作用不大的，最好不用;
2.如果想要程序不经历SO_LINGER需要设置SO_DONTLINGER，或者设置l_onoff=0；
10.还一个用的比较少的是在SDI或者是Dialog的程序中，可以记录socket的调试信息：
(前不久做过这个函数的测试，调式信息可以保存，包括socket建立时候的参数,采用的
具体协议，以及出错的代码都可以记录下来）
```cpp
BOOL bDebug=TRUE;
setsockopt(s,SOL_SOCKET,SO_DEBUG,(const char*)&bDebug,sizeof(BOOL));
```
11.附加：往往通过setsockopt()设置了缓冲区大小，但还不能满足数据的传输需求，
我的习惯是自己写个处理网络缓冲的类，动态分配内存;下面我将这个类写出，希望对
初学者有所帮助：
```cpp
//仿照String 改写而成
//==============================================================================
// 二进制数据，主要用于收发网络缓冲区的数据
// CNetIOBuffer 以 MFC 类 CString 的源代码作为蓝本改写而成，用法与 CString 类似，
// 但是 CNetIOBuffer 中存放的是纯粹的二进制数据，'\0' 并不作为它的结束标志。
// 其数据长度可以通过 GetLength() 获得，缓冲区地址可以通过运算符 LPBYTE 获得。


//==============================================================================
// Copyright (c) All-Vision Corporation. All rights reserved.
// Module: NetObject
// File: SimpleIOBuffer.h
// Author: gdy119
// Email : 8751webmaster@126.com
// Date: 2004.11.26
//==============================================================================
// NetIOBuffer.h
#ifndef _NETIOBUFFER_H
#define _NETIOBUFFER_H
//=============================================================================
#define MAX_BUFFER_LENGTH 1024*1024
//=============================================================================
//主要用来处理网络缓冲的数据
class CNetIOBuffer 
{
protected:
LPBYTE m_pbinData;
int m_nLength;
int m_nTotalLength;
CRITICAL_SECTIONm_cs;
void Initvalibers();
public:
CNetIOBuffer();
CNetIOBuffer(const LPBYTE lbbyte, int nLength);
CNetIOBuffer(const CNetIOBuffer&binarySrc);
virtual ~CNetIOBuffer();
//=============================================================================
BOOL CopyData(const LPBYTE lbbyte, int nLength);
BOOL ConcatData(const LPBYTE lbbyte, int nLength);
void ResetIoBuffer();
int GetLength() const;
BOOL SetLength(int nLen);
LPBYTE GetCurPos();
int GetRemainLen();
BOOL IsEmpty() const;
operator LPBYTE() const;
static GetMaxLength() { return MAX_BUFFER_LENGTH; }
const CNetIOBuffer& operator=(const CNetIOBuffer& buffSrc);
};
#endif // 
// NetOBuffer.cpp: implementation of the CNetIOBuffer class.
//======================================================================
#include "stdafx.h"
#include "NetIOBuffer.h"
//======================================================================
//=======================================================================
// Construction/Destruction
CNetIOBuffer::CNetIOBuffer()
{
Initvalibers();

}
CNetIOBuffer::CNetIOBuffer(const LPBYTE lbbyte, int nLength)
{
Initvalibers();
CopyData(lbbyte, nLength);
}
CNetIOBuffer::~CNetIOBuffer()
{
delete []m_pbinData;
m_pbinData=NULL;
DeleteCriticalSection(&m_cs);

}
CNetIOBuffer::CNetIOBuffer(const CNetIOBuffer&binarySrc)
{

Initvalibers();
CopyData(binarySrc,binarySrc.GetLength());

}
void CNetIOBuffer::Initvalibers()
{

m_pbinData = NULL;
m_nLength = 0;
m_nTotalLength = MAX_BUFFER_LENGTH;
if(m_pbinData==NULL)
{
m_pbinData=new BYTE[m_nTotalLength];
ASSERT(m_pbinData!=NULL);
}
InitializeCriticalSection(&m_cs);
}
void CNetIOBuffer::ResetIoBuffer()
{
EnterCriticalSection(&m_cs);
m_nLength = 0;
memset(m_pbinData,0,m_nTotalLength);
LeaveCriticalSection(&m_cs);
}

BOOL CNetIOBuffer::CopyData(const LPBYTE lbbyte, int nLength)
{
if( nLength > MAX_BUFFER_LENGTH )
return FALSE;

ResetIoBuffer();
EnterCriticalSection(&m_cs);
memcpy(m_pbinData, lbbyte, nLength );
m_nLength = nLength;
LeaveCriticalSection(&m_cs);

return TRUE;
}

BOOL CNetIOBuffer::ConcatData(const LPBYTE lbbyte, int nLength)
{
if( m_nLength + nLength > MAX_BUFFER_LENGTH )
return FALSE;

EnterCriticalSection(&m_cs);
memcpy(m_pbinData+m_nLength, lbbyte, nLength );
m_nLength += nLength;
LeaveCriticalSection(&m_cs);

return TRUE;
}

int CNetIOBuffer::GetLength() const
{
return m_nLength;
}

BOOL CNetIOBuffer::SetLength(int nLen)
{
if( nLen > MAX_BUFFER_LENGTH )
return FALSE;

EnterCriticalSection(&m_cs);
m_nLength = nLen;
LeaveCriticalSection(&m_cs);

return TRUE;
}

LPBYTE CNetIOBuffer::GetCurPos()
{

if( m_nLength < MAX_BUFFER_LENGTH )

return (m_pbinData+m_nLength);

else
return NULL;
}

CNetIOBuffer:: operator LPBYTE() const
{
return m_pbinData;
}

int CNetIOBuffer::GetRemainLen()
{

return MAX_BUFFER_LENGTH - m_nLength;

}
BOOL CNetIOBuffer::IsEmpty() const
{
return m_nLength == 0;
}

const CNetIOBuffer& CNetIOBuffer:: operator=(const CNetIOBuffer& buffSrc)
{
if(&buffSrc!=this)
{
CopyData(buffSrc, buffSrc.GetLength());

}
return *this;

}
```


回复人： PiggyXP(【小猪】●至爱VC，至爱网络版●) ( ) 信誉：204 



其实我觉得第5条很应该值得注意
```
int nZero=0;
setsockopt(socket，SOL_S0CKET,SO_SNDBUF，(char *)&nZero,sizeof(nZero));
```
记得以前有些朋友讨论过，socket虽然send成功了，但是其实只是发送到数据缓冲区里面了，而并没有真正的在物理设备上发送出去；而通过这条语句，将发送缓冲区设置为0，即屏蔽掉发送缓冲以后，一旦send返回（当然是就阻塞套结字来说），就可以肯定数据已经在发送的途中了^_^，但是这样做也许会影响系统的性能


to:Sander() 
UDP也有拷贝过程，但是UDP包有最大限制为64K；
TCP_NODELAY 一般用在the normal data stream 上；
12.发送数据时候一般是系统缓冲区满以后才发送，现在设置为只要系统
缓冲区有数据就立刻发送： 
```
BOOL bNodelay=TRUE;
SetSockOpt(s,IPPROTO_TCP,TCP_NODELAY,(const char*)&bNodelayt,sizeof(BOOL));
```
回复人： od4ys(风风) ( ) 信誉：100 

setoptsock（）这个函数 设置成端口复用的时候，很容易对一些没有进行单独bind模式的程序造成危害。
比如old的 ping icmp door，简单的sniffer后，收到包，然后设置setoptsock bind web服务，然后建立个cmd进程 bind再80端口。


