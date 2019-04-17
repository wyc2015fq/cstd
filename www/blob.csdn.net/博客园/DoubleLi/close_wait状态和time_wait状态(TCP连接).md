# close_wait状态和time_wait状态(TCP连接) - DoubleLi - 博客园






**1.CLOSE_WAIT的简单解决方案**

不久前，我的Socket Client程序遇到了一个非常尴尬的错误。它本来应该在一个socket长连接上持续不断地向服务器发送数据，如果socket连接断开，那么程序会自动不断地重试建立连接。

有一天发现程序在不断尝试建立连接，但是总是失败。用netstat查看，这个程序竟然有上千个socket连接处于CLOSE_WAIT状态，以至于达到了上限，所以无法建立新的socket连接了。

为什么会这样呢？

它们为什么会都处在CLOSE_WAIT状态呢？

CLOSE_WAIT状态的生成原因

首先我们知道，如果我们的Client程序处于CLOSE_WAIT状态的话，说明套接字是被动关闭的！

因为如果是Server端主动断掉当前连接的话，那么双方关闭这个TCP连接共需要四个packet：

Server ---> FIN ---> Client

Server <--- ACK <--- Client

这时候Server端处于FIN_WAIT_2状态；

而我们的程序Client处于CLOSE_WAIT状态。

如果接下来Client发送FIN给Server:

Server <--- FIN <--- Client

Client就置为LAST_ACK状态。

然后Server回应了ACK:

Server ---> ACK ---> Client

那么Client的套接字才会真正置为CLOSED状态。

![](http://www.cppstore.com/upload/20160512/20160512171821_29683.png)



我们的程序处于CLOSE_WAIT状态，而不是LAST_ACK状态，说明还没有发FIN给Server，那么可能是在关闭连接之前还有许多数据要发送或者其他事要做，导致没有发这个FIN packet。



原因知道了，那么为什么不发FIN包呢，难道会在关闭己方连接前有那么多事情要做吗？

还有一个问题，为什么有数千个连接都处于这个状态呢？难道那段时间内，服务器端总是主动拆除我们的连接吗？



不管怎么样，我们必须防止类似情况再度发生！

首先，我们要防止不断开辟新的端口，这可以通过设置SO_REUSEADDR套接字选项做到：

重用本地地址和端口

以前我总是一个端口不行，就换一个新的使用，所以导致让数千个端口进入CLOSE_WAIT状态。如果下次还发生这种尴尬状况，我希望加一个限定，只是当前这个端口处于CLOSE_WAIT状态！

在调用

sockConnected = socket(AF_INET, SOCK_STREAM, 0);

之后，我们要设置该套接字的选项来重用：

/// 允许重用本地地址和端口:

/// 这样的好处是，即使socket断了，调用前面的socket函数也不会占用另一个，而是始终就是一个端口

/// 这样防止socket始终连接不上，那么按照原来的做法，会不断地换端口。

int nREUSEADDR = 1;

setsockopt(sockConnected,

SOL_SOCKET,

SO_REUSEADDR,

(const char*)&nREUSEADDR,

sizeof(int));

教科书上是这么说的：这样，假如服务器关闭或者退出，造成本地地址和端口都处于TIME_WAIT状态，那么SO_REUSEADDR就显得非常有用。

也许我们无法避免被冻结在CLOSE_WAIT状态永远不出现，但起码可以保证不会占用新的端口。

其次，我们要设置SO_LINGER套接字选项：

从容关闭还是强行关闭？

LINGER是“拖延”的意思。

默认情况下(Win2k)，SO_DONTLINGER套接字选项的是1；SO_LINGER选项是，linger为{l_onoff：0，l_linger：0}。

如果在发送数据的过程中(send()没有完成，还有数据没发送)而调用了closesocket()，以前我们一般采取的措施是“从容关闭”：

因为在退出服务或者每次重新建立socket之前，我都会先调用

/// 先将双向的通讯关闭

shutdown(sockConnected, SD_BOTH);

/// 安全起见，每次建立Socket连接前，先把这个旧连接关闭

closesocket(sockConnected);



我们这次要这么做：

设置SO_LINGER为零（亦即linger结构中的l_onoff域设为非零，但l_linger为0），便不用担心closesocket调用进入“锁定”状态（等待完成），不论是否有排队数据未发送或未被确认。这种关闭方式称为“强行关闭”，因为套接字的虚电路立即被复位，尚未发出的所有数据都会丢失。在远端的recv()调用都会失败，并返回WSAECONNRESET错误。

在connect成功建立连接之后设置该选项：

linger m_sLinger;

m_sLinger.l_onoff = 1; // (在closesocket()调用,但是还有数据没发送完毕的时候容许逗留)

m_sLinger.l_linger = 0; // (容许逗留的时间为0秒)

setsockopt(sockConnected,

SOL_SOCKET,

SO_LINGER,

(const char*)&m_sLinger,

sizeof(linger));



总结

也许我们避免不了CLOSE_WAIT状态冻结的再次出现，但我们会使影响降到最小，希望那个重用套接字选项能够使得下一次重新建立连接时可以把CLOSE_WAIT状态踢掉。



当一方关闭连接后，另外一方没有检测到，就会导致了CLOSE_WAIT的出现，上次我的一个朋友也是这样，他写了一个客户端和 APACHE连接，当APACHE把连接断掉后，他没检测到，出现了CLOSE_WAIT，后来我叫他检测了这个地方，他添加了调用 closesocket的代码后，这个问题就消除了。

如果你在关闭连接前还是出现CLOSE_WAIT,建议你取消shutdown的调用，直接两边closesocket试试。

另外一个问题:

比如这样的一个例子：

当客户端登录上服务器后，发送身份验证的请求，服务器收到了数据，对客户端身份进行验证，发现密码错误，这时候服务器的一般做法应该是先发送一个密码错误的信息给客户端，然后把连接断掉。

如果把

m_sLinger.l_onoff = 1;

m_sLinger.l_linger = 0;

这样设置后，很多情况下，客户端根本就收不到密码错误的消息，连接就被断了。



**2. CLOSE_WAIT的真正原因及解决方案**

很简单，就是某一方在网络连接断开后，没有检测到这个错误，没有执行closesocket，导致了这个状态的实现，这在TCP/IP协议的状态变迁图上可以清楚看到。同时和这个相对应的还有一种叫TIME_WAIT的。

另外，把SOCKET的SO_LINGER设置为0秒拖延（也就是立即关闭）在很多时候是有害处的。

还有，把端口设置为可复用是一种不安全的网络编程方法。

断开连接的时候，

当发起主动关闭的左边这方发送一个FIN过去后，被动关闭的这一方要会回应一个ACK，这个ACK是TCP回应的，而不是应用程序发送的，此时，被动关闭的一方就处于CLOSE_WAIT状态了。如果此时被动关闭的这一方不再继续调用closesocket,那么他就不会 发送接下来的FIN，导致被动关闭的一方一直处于CLOSE_WAIT。只有被动关闭的这一方调用了closesocket,才会发送一个FIN给主动关闭的这一 方，同时也使得主动关闭的一方的状态变迁为LAST_ACK。

比如被动关闭的是客户端。。。

当对方调用closesocket的时候，你的程序正在

int nRet = recv(s,....);

if (nRet == SOCKET_ERROR)

{

// closesocket(s);

return FALSE;

}

很多人就是忘记了那句closesocket，这种代码太常见了。



下面为一张TCP连接的状态转换图：

![](http://www.cppstore.com/upload/20160512/20160512171847_60558.jpg)

说明：虚线和实线分别对应服务器端(被连接端)和客户端端(主动连接端)。

结合上图使用netstat -na命令即可知道到当前的TCP连接状态。一般LISTEN、ESTABLISHED、TIME_WAIT是比较常见。

分析：

上面我碰到的这个问题主要因为TCP的结束流程未走完，造成连接未释放。现设客户端主动断开连接，流程如下

Client 消息 Server

close()

------ FIN ------->

FIN_WAIT1 CLOSE_WAIT

<----- ACK -------

FIN_WAIT2

close()

<------ FIN ------

TIME_WAIT LAST_ACK

------ ACK ------->

CLOSED

CLOSED

如上图所示，由于Server的Socket在客户端已经关闭时而没有调用关闭，造成服务器端的连接处在“挂起”状态，而客户端则处在等待应答的状态上。此问题的典型特征是：一端处于FIN_WAIT2 ，而另一端处于CLOSE_WAIT. 不过，根本问题还是程序写的不好，有待提高。



**3.TIME_WAIT状态**

根据TCP协议，主动发起关闭的一方，会进入TIME_WAIT状态，持续2*MSL(Max Segment Lifetime)，缺省为240秒，在这个post中简洁的介绍了为什么需要这个状态。

值得一说的是，对于基于TCP的HTTP协议，关闭TCP连接的是Server端，这样，Server端会进入TIME_WAIT状态，可 想而知，对于访问量大的Web Server，会存在大量的TIME_WAIT状态，假如server一秒钟接收1000个请求，那么就会积压240*1000=240，000个 TIME_WAIT的记录，维护这些状态给Server带来负担。当然现代操作系统都会用快速的查找算法来管理这些TIME_WAIT，所以对于新的 TCP连接请求，判断是否hit中一个TIME_WAIT不会太费时间，但是有这么多状态要维护总是不好。



HTTP协议1.1版规定default行为是Keep-Alive，也就是会重用TCP连接传输多个 request/response，一个主要原因就是发现了这个问题。还有一个方法减缓TIME_WAIT压力就是把系统的2*MSL时间减少，因为 240秒的时间实在是忒长了点，对于Windows，修改注册表，在HKEY_LOCAL_MACHINE\ SYSTEM\CurrentControlSet\Services\ Tcpip\Parameters上添加一个DWORD类型的值TcpTimedWaitDelay，一般认为不要少于60，不然可能会有麻烦。

对于大型的服务，一台server搞不定，需要一个LB(Load Balancer)把流量分配到若干后端服务器上，如果这个LB是以NAT方式工作的话，可能会带来问题。假如所有从LB到后端Server的IP包的 source address都是一样的(LB的对内地址），那么LB到后端Server的TCP连接会受限制，因为频繁的TCP连接建立和关闭，会在server上留 下TIME_WAIT状态，而且这些状态对应的remote address都是LB的，LB的source port撑死也就60000多个(2^16=65536,1~1023是保留端口，还有一些其他端口缺省也不会用），每个LB上的端口一旦进入 Server的TIME_WAIT黑名单，就有240秒不能再用来建立和Server的连接，这样LB和Server最多也就能支持300个左右的连接。 如果没有LB，不会有这个问题，因为这样server看到的remote address是internet上广阔无垠的集合，对每个address，60000多个port实在是够用了。



一开始我觉得用上LB会很大程度上限制TCP的连接数，但是实验表明没这回事，LB后面的一台Windows Server 2003每秒处理请求数照样达到了600个，难道TIME_WAIT状态没起作用？用Net Monitor和netstat观察后发现，Server和LB的XXXX端口之间的连接进入TIME_WAIT状态后，再来一个LB的XXXX端口的 SYN包，Server照样接收处理了，而是想像的那样被drop掉了。翻书，从书堆里面找出覆满尘土的大学时代的《UNIX Network Programming, Volume 1, Second Edition: Networking APIs: Sockets and XTI》，中间提到一句，对于BSD-derived实现，只要SYN的sequence number比上一次关闭时的最大sequence number还要大，那么TIME_WAIT状态一样接受这个SYN，难不成Windows也算BSD-derived?有了这点线索和关键字 (BSD)，找到这个post，在NT4.0的时候，还是和BSD-derived不一样的，不过Windows Server 2003已经是NT5.2了，也许有点差别了。



做个试验，用Socket API编一个Client端，每次都Bind到本地一个端口比如2345，重复的建立TCP连接往一个Server发送Keep-Alive=false 的HTTP请求，Windows的实现让sequence number不断的增长，所以虽然Server对于Client的2345端口连接保持TIME_WAIT状态，但是总是能够接受新的请求，不会拒绝。那 如果SYN的Sequence Number变小会怎么样呢？同样用Socket API，不过这次用Raw IP，发送一个小sequence number的SYN包过去，Net Monitor里面看到，这个SYN被Server接收后如泥牛如海，一点反应没有，被drop掉了。



按照书上的说法，BSD-derived和Windows Server 2003的做法有安全隐患，不过至少这样至少不会出现TIME_WAIT阻止TCP请求的问题，当然，客户端要配合，保证不同TCP连接的sequence number要上涨不要下降。

**4. socket api: close() 和 shutdown()**

由前面内容可知，对一条TCP连接而言，首先调用close()的一方会进入TIME_WAIT状态，除此之外，关于close()还有一些细节需要说明。

对一个tcp socket调用close()的默认动作是将该socket标记为已关闭并立即返回到调用该api进程中。此时，从应用层来看，该socket fd不能再被进程使用，即不能再作为read或write的参数。而从传输层来看，TCP会尝试将目前send buffer中积压的数据发到链路上，然后才会发起TCP的4次挥手以彻底关闭TCP连接。

调用close()是关闭TCP连接的正常方式，但这种方式存在两个限制，而这正是引入shutdown()的原因：

1）close()其实只是将socket fd的引用计数减1，只有当该socket fd的引用计数减至0时，TCP传输层才会发起4次握手从而真正关闭连接。而shutdown则可以直接发起关闭连接所需的4次握手，而不用受到引用计数的限制；

2）close()会终止TCP的双工链路。由于TCP连接的全双工特性，可能会存在这样的应用场景：local peer不会再向remote peer发送数据，而remote peer可能还有数据需要发送过来，在这种情况下，如果local peer想要通知remote peer自己不会再发送数据但还会继续收数据这个事实，用close()是不行的，而shutdown()可以完成这个任务。

**5. 如果发现大量的TIME_WAIT时的处理方案**

发现系统存在大量TIME_WAIT状态的连接，通过调整内核参数解决，

vi /etc/sysctl.conf

编辑文件，加入以下内容：

net.ipv4.tcp_syncookies = 1

net.ipv4.tcp_tw_reuse = 1

net.ipv4.tcp_tw_recycle = 1

net.ipv4.tcp_fin_timeout = 30



然后执行 /sbin/sysctl -p 让参数生效。

net.ipv4.tcp_syncookies = 1 表示开启SYN Cookies。当出现SYN等待队列溢出时，启用cookies来处理，可防范少量SYN攻击，默认为0，表示关闭；

net.ipv4.tcp_tw_reuse = 1 表示开启重用。允许将TIME-WAIT sockets重新用于新的TCP连接，默认为0，表示关闭；

net.ipv4.tcp_tw_recycle = 1 表示开启TCP连接中TIME-WAIT sockets的快速回收，默认为0，表示关闭。

net.ipv4.tcp_fin_timeout 修改系統默认的 TIMEOUT 时间









