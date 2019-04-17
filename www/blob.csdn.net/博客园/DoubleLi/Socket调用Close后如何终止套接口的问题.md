# Socket调用Close后如何终止套接口的问题 - DoubleLi - 博客园






setsockopt 设置 SO_LINGER 选项



   此选项指定函数close对面向连接的协议如何操作（如TCP）。内核缺省close操作是立即返回，如果有数据残留在套接口缓冲区中则系统将试着将这些数据发送给对方。



SO_LINGER选项用来改变此缺省设置。使用如下结构：

struct linger {

     int l_onoff; /* 0 = off, nozero = on */

     int l_linger; /* linger time */

};



有下列三种情况：

1、设置 l_onoff为0，则该选项关闭，l_linger的值被忽略，等于内核缺省情况，close调用会立即返回给调用者，如果可能将会传输任何未发送的数据；



2、设置 l_onoff为非0，l_linger为0，则套接口关闭时TCP夭折连接，TCP将丢弃保留在套接口发送缓冲区中的任何数据并发送一个RST给对方，而不是通常的四分组终止序列，这避免了TIME_WAIT状态；



3、设置 l_onoff 为非0，l_linger为非0，当套接口关闭时内核将拖延一段时间（由l_linger决定）。如果套接口缓冲区中仍残留数据，进程将处于睡眠状态，直 到（a）所有数据发送完且被对方确认，之后进行正常的终止序列（描述字访问计数为0）或（b）延迟时间到。此种情况下，应用程序检查close的返回值是非常重要的，如果在数据发送完并被确认前时间到，close将返回EWOULDBLOCK错误且套接口发送缓冲区中的任何数据都丢失。close的成功返回仅告诉我们发送的数据（和FIN）已由对方TCP确认，它并不能告诉我们对方应用进程是否已读了数据。如果套接口设为非阻塞的，它将不等待close完成。



注释：l_linger的单位依赖于实现: 4.4BSD假设其单位是时钟滴答（百分之一秒），但Posix.1g规定单位为秒。



下面的代码是一个使用SO_LINGER选项的例子，使用30秒的超时时限：
#define TRUE     1
#define FALSE    0
int z; /* Status code
*/ int s;       /* Socket s */
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

下面的例子显示了如何设置SO_LINGER的值来中止套接口s上的当前连接：
#define TRUE     1
#define FALSE    0
int z; /* Status code */
int s;       /* Socket s */
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

在上面的这个例子中，当调用close函数时，套接口s会立即中止。中止的语义是通过将超时值设置为0来实现的。









/********** WINDOWS **********/









/* 当连接中断时，需要延迟关闭(linger)以保证所有数据都被传输，所以需要打开SO_LINGER这个选项；  
 * //注：大致意思就是说SO_LINGER选项用来设置当调用closesocket时是否马上关闭socket； 
 * linger的结构在/usr/include/linux/socket.h中定义：//注：这个结构就是SetSocketOpt中的Data的数据结构 
 *　 struct linger 
 *　 { 
 *　　 int l_onoff;　 /* Linger active */       //低字节，0和非0，用来表示是否延时关闭socket
 *　　 int l_linger; /* How long to linger */   //高字节,延时的时间数，单位为秒
 *　 }; 
 *　 如果l_onoff为0，则延迟关闭特性就被取消。

 *   如果非零，则允许套接口延迟关闭； l_linger字段则指明延迟关闭的时间 
 */


更具体的描述如下：
1、若设置了SO_LINGER（亦即linger结构中的l_onoff域设为非零），并设置了零超时间隔，则closesocket()不被阻塞立即执行，不论是否有排队数据未发送或未被确认。这种关闭方式称为“强制”或“失效”关闭，因为套接口的虚电路立即被复位，且丢失了未发送的数据。在远端的recv()调用将以WSAECONNRESET出错。

2、若设置了SO_LINGER并确定了非零的超时间隔，则closesocket()调用阻塞进程，直到所剩数据发送完毕或超时。这种关闭称为“优雅”或“从容”关闭。请注意如果套接口置为非阻塞且SO_LINGER设为非零超时，则closesocket()调用将以WSAEWOULDBLOCK错误返回。

3、若在一个流类套接口上设置了SO_DONTLINGER（也就是说将linger结构的l_onoff域设为零），则closesocket()调用立即返回。但是，如果可能，排队的数据将在套接口关闭前发送。请注意，在这种情况下WINDOWS套接口实现将在一段不确定的时间内保留套接口以及其他资源，这对于想用所以套接口的应用程序来说有一定影响。


SO_DONTLINGER 若为真，则SO_LINGER选项被禁止。
SO_LINGER延迟关闭连接 struct linger上面这两个选项影响close行为；


     选项          间隔    关闭方式  等待关闭与否
  SO_DONTLINGER   不关心     优雅         否
  SO_LINGER        零        强制         否
  SO_LINGER       非零       优雅         是









