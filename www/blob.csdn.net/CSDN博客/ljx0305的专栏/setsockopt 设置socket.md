# setsockopt 设置socket - ljx0305的专栏 - CSDN博客
2008年07月30日 11:17:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：622标签：[socket																[windows																[tcp																[算法																[struct																[pascal](https://so.csdn.net/so/search/s.do?q=pascal&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=算法&t=blog)](https://so.csdn.net/so/search/s.do?q=tcp&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=socket&t=blog)
个人分类：[网络编程](https://blog.csdn.net/ljx0305/article/category/403915)
**1.**closesocket（一般不会立即关闭而经历TIME_WAIT的过程）后想继续重用该socket：
BOOL bReuseaddr=TRUE;
**setsockopt**(s,SOL_SOCKET ,SO_REUSEADDR,(const char*)&bReuseaddr,sizeof(BOOL));
**2.** 如果要已经处于连接状态的soket在调用closesocket后强制关闭，不经历
TIME_WAIT的过程：
BOOL bDontLinger = FALSE;
**setsockopt**(s,SOL_SOCKET,SO_DONTLINGER,(const char*)&bDontLinger,sizeof(BOOL));
**3.**在send(),recv()过程中有时由于网络状况等原因，发收不能预期进行,而设置收发时限：
int nNetTimeout=1000;//1秒
//发送时限
**setsockopt**(socket，SOL_S0CKET,SO_SNDTIMEO，(char *)&nNetTimeout,sizeof(int));
//接收时限
**setsockopt**(socket，SOL_S0CKET,SO_RCVTIMEO，(char *)&nNetTimeout,sizeof(int));
**4.**在send()的时候，返回的是实际发送出去的字节(同步)或发送到socket缓冲区的字节
(异步);系统默认的状态发送和接收一次为8688字节(约为8.5K)；在实际的过程中发送数据
和接收数据量比较大，可以设置socket缓冲区，而避免了send(),recv()不断的循环收发：
// 接收缓冲区
int nRecvBuf=32*1024;//设置为32K
**setsockopt**(s,SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(int));
//发送缓冲区
int nSendBuf=32*1024;//设置为32K
**setsockopt**(s,SOL_SOCKET,SO_SNDBUF,(const char*)&nSendBuf,sizeof(int));
**5.** 如果在发送数据的时，希望不经历由系统缓冲区到socket缓冲区的拷贝而影响
程序的性能：
int nZero=0;
**setsockopt**(socket，SOL_S0CKET,SO_SNDBUF，(char *)&nZero,sizeof(nZero));
**6.**同上在recv()完成上述功能(默认情况是将socket缓冲区的内容拷贝到系统缓冲区)：
int nZero=0;
**setsockopt**(socket，SOL_S0CKET,SO_RCVBUF，(char *)&nZero,sizeof(int));
**7.**一般在发送UDP数据报的时候，希望该socket发送的数据具有广播特性：
BOOL bBroadcast=TRUE;
**setsockopt**(s,SOL_SOCKET,SO_BROADCAST,(const char*)&bBroadcast,sizeof(BOOL));
**8.**在client连接服务器过程中，如果处于非阻塞模式下的socket在connect()的过程中可
以设置connect()延时,直到accpet()被呼叫(本函数设置只有在非阻塞的过程中有显著的
作用，在阻塞的函数调用中作用不大)
BOOL bConditionalAccept=TRUE;
**setsockopt**(s,SOL_SOCKET,SO_CONDITIONAL_ACCEPT,(const char*)&bConditionalAccept,sizeof(BOOL));
**9**.如果在发送数据的过程中(send()没有完成，还有数据没发送)而调用了closesocket(),以前我们
一般采取的措施是"从容关闭"shutdown(s,SD_BOTH),但是数据是肯定丢失了，如何设置让程序满足具体
应用的要求(即让没发完的数据发送出去后在关闭socket)？
struct linger {
u_short l_onoff;
u_short l_linger;
};
linger m_sLinger;
m_sLinger.l_onoff=1;//(在closesocket()调用,但是还有数据没发送完毕的时候容许逗留)
// 如果m_sLinger.l_onoff=0;则功能和2.)作用相同;
m_sLinger.l_linger=5;//(容许逗留的时间为5秒)
**setsockopt**(s,SOL_SOCKET,SO_LINGER,(const char*)&m_sLinger,sizeof(linger)); 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
设置套接口的选项。
  #include <winsock.h>
  int PASCAL FAR **setsockopt**( SOCKET s, int level, int optname,
  const char FAR* optval, int optlen);
  s：标识一个套接口的描述字。
  level：选项定义的层次；目前仅支持SOL_SOCKET和IPPROTO_TCP层次。
  optname：需设置的选项。
  optval：指针，指向存放选项值的缓冲区。
  optlen：optval缓冲区的长度。
注释：
**setsockopt**()函数用于任意类型、任意状态套接口的设置选项值。尽管在不同协议层上存在选项，但本函数仅定义了最高的“套接口”层次上的选项。选项影响套接口的操作，诸如加急数据是否在普通数据流中接收，广播数据是否可以从套接口发送等等。
  有两种套接口的选项：一种是布尔型选项，允许或禁止一种特性；另一种是整形或结构选项。允许一个布尔型选项，则将optval指向非零整形数；禁止一个选项optval指向一个等于零的整形数。对于布尔型选项，optlen应等于sizeof(int)；对其他选项，optval指向包含所需选项的整形数或结构，而optlen则为整形数或结构的长度。SO_LINGER选项用于控制下述情况的行动：套接口上有排队的待发送数据，且closesocket()调用已执行。参见closesocket()函数中关于SO_LINGER选项对closesocket()语义的影响。应用程序通过创建一个linger结构来设置相应的操作特性：
  struct linger {
        int l_onoff;
        int l_linger;
  };
  为了允许SO_LINGER，应用程序应将l_onoff设为非零，将l_linger设为零或需要的超时值（以秒为单位），然后调用**setsockopt**()。为了允许SO_DONTLINGER（亦即禁止SO_LINGER），l_onoff应设为零，然后调用**setsockopt**()。
  缺省条件下，一个套接口不能与一个已在使用中的本地地址捆绑（参见bind()）。但有时会需要“重用”地址。因为每一个连接都由本地地址和远端地址的组合唯一确定，所以只要远端地址不同，两个套接口与一个地址捆绑并无大碍。为了通知WINDOWS套接口实现不要因为一个地址已被一个套接口使用就不让它与另一个套接口捆绑，应用程序可在bind()调用前先设置SO_REUSEADDR选项。请注意仅在bind()调用时该选项才被解释；故此无需（但也无害）将一个不会共用地址的套接口设置该选项，或者在bind()对这个或其他套接口无影响情况下设置或清除这一选项。
  一个应用程序可以通过打开SO_KEEPALIVE选项，使得WINDOWS套接口实现在TCP连接情况下允许使用“保持活动”包。一个WINDOWS套接口实现并不是必需支持“保持活动”，但是如果支持的话，具体的语义将与实现有关，应遵守RFC1122“Internet主机要求－通讯层”中第4.2.3.6节的规范。如果有关连接由于“保持活动”而失效，则进行中的任何对该套接口的调用都将以WSAENETRESET错误返回，后续的任何调用将以WSAENOTCONN错误返回。
  TCP_NODELAY选项禁止Nagle算法。Nagle算法通过将未确认的数据存入缓冲区直到蓄足一个包一起发送的方法，来减少主机发送的零碎小数据包的数目。但对于某些应用来说，这种算法将降低系统性能。所以TCP_NODELAY可用来将此算法关闭。应用程序编写者只有在确切了解它的效果并确实需要的情况下，才设置TCP_NODELAY选项，因为设置后对网络性能有明显的负面影响。TCP_NODELAY是唯一使用IPPROTO_TCP层的选项，其他所有选项都使用SOL_SOCKET层。
  如果设置了SO_DEBUG选项，WINDOWS套接口供应商被鼓励（但不是必需）提供输出相应的调试信息。但产生调试信息的机制以及调试信息的形式已超出本规范的讨论范围。
**setsockopt**()支持下列选项。其中“类型”表明optval所指数据的类型。
选项        类型  意义
SO_BROADCAST    BOOL    允许套接口传送广播信息。
SO_DEBUG    BOOL    记录调试信息。
SO_DONTLINER    BOOL    不要因为数据未发送就阻塞关闭操作。设置本选项相当于将SO_LINGER的l_onoff元素置为零。
SO_DONTROUTE    BOOL    禁止选径；直接传送。
SO_KEEPALIVE    BOOL    发送“保持活动”包。
SO_LINGER   struct linger FAR*  如关闭时有未发送数据，则逗留。
SO_OOBINLINE    BOOL    在常规数据流中接收带外数据。
SO_RCVBUF   int 为接收确定缓冲区大小。
SO_REUSEADDR    BOOL    允许套接口和一个已在使用中的地址捆绑（参见bind()）。
SO_SNDBUF   int 指定发送缓冲区大小。
TCP_NODELAY BOOL    禁止发送合并的Nagle算法。
**setsockopt**()不支持的BSD选项有：
选项名      类型    意义
SO_ACCEPTCONN   BOOL    套接口在监听。
SO_ERROR    int 获取错误状态并清除。
SO_RCVLOWAT int 接收低级水印。
SO_RCVTIMEO int 接收超时。
SO_SNDLOWAT int 发送低级水印。
SO_SNDTIMEO int 发送超时。
SO_TYPE     int 套接口类型。
IP_OPTIONS      在IP头中设置选项。
返回值：
  若无错误发生，**setsockopt**()返回0。否则的话，返回SOCKET_ERROR错误，应用程序可通过WSAGetLastError()获取相应错误代码。
错误代码：
  WSANOTINITIALISED：在使用此API之前应首先成功地调用WSAStartup()。
  WSAENETDOWN：WINDOWS套接口实现检测到网络子系统失效。
  WSAEFAULT：optval不是进程地址空间中的一个有效部分。
  WSAEINPROGRESS：一个阻塞的WINDOWS套接口调用正在运行中。
  WSAEINVAL：level值非法，或optval中的信息非法。
  WSAENETRESET：当SO_KEEPALIVE设置后连接超时。
  WSAENOPROTOOPT：未知或不支持选项。其中，SOCK_STREAM类型的套接口不支持SO_BROADCAST选项，SOCK_DGRAM类型的套接口不支持SO_DONTLINGER 、SO_KEEPALIVE、SO_LINGER和SO_OOBINLINE选项。
  WSAENOTCONN：当设置SO_KEEPALIVE后连接被复位。
  WSAENOTSOCK：描述字不是一个套接口。
参见：
  bind(), getsockopt(), ioctlsocket(), socket(), WSAAsyncSelect().
引用：[http://blog.csdn.net/qinmi/archive/2007/03/07/1523081.aspx](http://blog.csdn.net/qinmi/archive/2007/03/07/1523081.aspx)
