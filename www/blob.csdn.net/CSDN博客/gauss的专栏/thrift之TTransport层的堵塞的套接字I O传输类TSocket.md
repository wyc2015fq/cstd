# thrift之TTransport层的堵塞的套接字I/O传输类TSocket - gauss的专栏 - CSDN博客
2015年11月03日 23:31:21[gauss](https://me.csdn.net/mathlmx)阅读数：449
                
本节将介绍第一个实现具体传输功能的类TSocket，这个类是基于TCP socket实现TTransport的接口。下面具体介绍这个类的相关函数功能实现。
　　1.构造函数
　　分析一个类的功能首先看它的定义和构造函数实现，先看看它的定义：
**[cpp]**[view plain](http://blog.csdn.net/qiangweiloveforever/article/details/9474941#)[copy](http://blog.csdn.net/qiangweiloveforever/article/details/9474941#)[print](http://blog.csdn.net/qiangweiloveforever/article/details/9474941#)[?](http://blog.csdn.net/qiangweiloveforever/article/details/9474941#)
- class TSocket : public TVirtualTransport<TSocket> { ......}  
　　由定义可以看书TSocket继承至虚拟传输类，并且把自己当做模板参数传递过去，所以从虚拟传输类继承下来的虚拟函数（如read_virt）调用非虚拟函数（如read）就是TSocket自己实现的。
　　TSocket类的构造函数有4个，当然还有一个析构函数。四个构造函数就是根据不同的参数来构造，它们的声明如下：
**[cpp]**[view plain](http://blog.csdn.net/qiangweiloveforever/article/details/9474941#)[copy](http://blog.csdn.net/qiangweiloveforever/article/details/9474941#)[print](http://blog.csdn.net/qiangweiloveforever/article/details/9474941#)[?](http://blog.csdn.net/qiangweiloveforever/article/details/9474941#)
- TSocket();//所有参数都默认
- TSocket(std::string host, int port);//根据主机名和端口构造一个socket
- TSocket(std::string path);//构造unix域的一个socket
- TSocket(int socket);//构造一个原始的unix句柄socket
　　四个构造函数分别用于不同的情况下来产生不同的TSocket对象，不过这些构造函数都只是简单的初始化一些最基本的成员变量，而没有真正的连接socket。它们初始化的变量基本如下：
**[cpp]**[view plain](http://blog.csdn.net/qiangweiloveforever/article/details/9474941#)[copy](http://blog.csdn.net/qiangweiloveforever/article/details/9474941#)[print](http://blog.csdn.net/qiangweiloveforever/article/details/9474941#)[?](http://blog.csdn.net/qiangweiloveforever/article/details/9474941#)
- TSocket::TSocket() :  
-   host_(""),  
-   port_(0),  
-   path_(""),  
-   socket_(-1),  
-   connTimeout_(0),  
-   sendTimeout_(0),  
-   recvTimeout_(0),  
-   lingerOn_(1),  
-   lingerVal_(0),  
-   noDelay_(1),  
-   maxRecvRetries_(5) {  
-   recvTimeval_.tv_sec = (int)(recvTimeout_/1000);  
-   recvTimeval_.tv_usec = (int)((recvTimeout_%1000)*1000);  
-   cachedPeerAddr_.ipv4.sin_family = AF_UNSPEC;  
- }  
　　大部分简单的参数都采用初始化列表初始化了，需要简单计算的就放在函数体内初始化，其他几个都是这种情况。下面需要单独介绍一下的是unix domain socket。
　　socket API原本是为网络通讯设计的，但后来在socket的框架上发展出一种IPC机制，就是UNIX Domain Socket。虽然网络socket也可用于同一台主机的进程间通讯（通过loopback地址127.0.0.1），但是UNIX Domain Socket用于IPC更有效率：不需要经过网络协议栈，不需要打包拆包、计算校验和、维护序号和应答等，只是将应用层数据从一个进程拷贝到另一个进程。这是因为，IPC机制本质上是可靠的通讯，而网络协议是为不可靠的通讯设计的。UNIX Domain Socket也提供面向流和面向数据包两种API接口，类似于TCP和UDP，但是面向消息的UNIX
 Domain Socket也是可靠的，消息既不会丢失也不会顺序错乱。
　　UNIX Domain Socket是全双工的，API接口语义丰富，相比其它IPC机制有明显的优越性，目前已成为使用最广泛的IPC机制，比如X Window服务器和GUI程序之间就是通过UNIX Domain Socket通讯的。
　　使用UNIX Domain Socket的过程和网络socket十分相似，也要先调用socket()创建一个socket文件描述符，address family指定为AF_UNIX，type可以选择SOCK_DGRAM或SOCK_STREAM，protocol参数仍然指定为0即可。
　　UNIX Domain Socket与网络socket编程最明显的不同在于地址格式不同，用结构体sockaddr_un表示，网络编程的socket地址是IP地址加端口号，而UNIX Domain Socket的地址是一个socket类型的文件在文件系统中的路径，这个socket文件由bind()调用创建，如果调用bind()时该文件已存在，则bind()错误返回。
　　打开连接函数open
　　首先看这个函数的代码实现，如下：
**[cpp]**[view plain](http://blog.csdn.net/qiangweiloveforever/article/details/9474941#)[copy](http://blog.csdn.net/qiangweiloveforever/article/details/9474941#)[print](http://blog.csdn.net/qiangweiloveforever/article/details/9474941#)[?](http://blog.csdn.net/qiangweiloveforever/article/details/9474941#)
- void TSocket::open() {  
- if (isOpen()) {//如果已经打开就直接返回
- return;  
-   }  
- if (! path_.empty()) {//如果unix路径不为空就打开unix domian socket
-     unix_open();  
-   } else {  
-     local_open();//打开通用socket
-   }  
- }  
　　Open函数又根据路径为不为空（不为空就是unix domain socket）调用相应的函数来继续打开连接，首先看看打开unix domain socket，代码如下：
**[cpp]**[view plain](http://blog.csdn.net/qiangweiloveforever/article/details/9474941#)[copy](http://blog.csdn.net/qiangweiloveforever/article/details/9474941#)[print](http://blog.csdn.net/qiangweiloveforever/article/details/9474941#)[?](http://blog.csdn.net/qiangweiloveforever/article/details/9474941#)
- void TSocket::unix_open(){  
- if (! path_.empty()) {//保证path_不为空
- // Unix Domain SOcket does not need addrinfo struct, so we pass NULL
-     openConnection(NULL);//调用真正的打开连接函数
-   }  
- }  
　　由代码可以看出，真正实现打开连接的函数是openConnection，这个函数根据传递的参数来决定是否是打开unix domain socket，实现代码如下（这个函数代码比较多，其中除了错误部分代码省略）：
**[cpp]**[view plain](http://blog.csdn.net/qiangweiloveforever/article/details/9474941#)[copy](http://blog.csdn.net/qiangweiloveforever/article/details/9474941#)[print](http://blog.csdn.net/qiangweiloveforever/article/details/9474941#)[?](http://blog.csdn.net/qiangweiloveforever/article/details/9474941#)
- void TSocket::openConnection(struct addrinfo *res) {  
- if (isOpen()) {  
- return;//如果已经打开了直接返回
-   }  
- if (! path_.empty()) {//根据路径是否为空创建不同的socket
-     socket_ = socket(PF_UNIX, SOCK_STREAM, IPPROTO_IP);//创建unix domain socket
-   } else {  
-     socket_ = socket(res->ai_family, res->ai_socktype, res->ai_protocol);//创建通用的网络通信socket
-   }  
- if (sendTimeout_ > 0) {//如果发生超时设置大于0就调用设置发送超时函数设置发送超时
-     setSendTimeout(sendTimeout_);  
-   }  
- if (recvTimeout_ > 0) {//如果接收超时设置大于0就调用设置接收超时函数设置接收超时
-     setRecvTimeout(recvTimeout_);  
-   }  
-   setLinger(lingerOn_, lingerVal_);//设置优雅断开连接或关闭连接参数
-   setNoDelay(noDelay_);//设置无延时
- #ifdef TCP_LOW_MIN_RTO
- if (getUseLowMinRto()) {//设置是否使用较低的最低TCP重传超时 
- int one = 1;  
-     setsockopt(socket_, IPPROTO_TCP, TCP_LOW_MIN_RTO, &one, sizeof(one));  
-   }  
- #endif
- //如果超时已经存在设置连接为非阻塞
- int flags = fcntl(socket_, F_GETFL, 0);//得到socket_的标识
- if (connTimeout_ > 0) {//超时已经存在
- if (-1 == fcntl(socket_, F_SETFL, flags | O_NONBLOCK)) {//设置为非阻塞
-     }  
-   } else {  
- if (-1 == fcntl(socket_, F_SETFL, flags & ~O_NONBLOCK)) {//设置为阻塞
-     }  
-   }  
- // 连接socket
- int ret;  
- if (! path_.empty()) {//unix domain socket
- #ifndef _WIN32 //window不支持
- struct sockaddr_un address;  
-     socklen_t len;  
- if (path_.length() > sizeof(address.sun_path)) {//path_长度不能超过最长限制
-     }  
-     address.sun_family = AF_UNIX;  
-     snprintf(address.sun_path, sizeof(address.sun_path), "%s", path_.c_str());  
-     len = sizeof(address);  
-     ret = connect(socket_, (struct sockaddr *) &address, len);//连接unix domain socket
- #else
- //window不支持unix domain socket
- #endif
- 
-   } else {  
-     ret = connect(socket_, res->ai_addr, res->ai_addrlen);//连接通用的非unix domain socket
-   }  
- if (ret == 0) {//失败了就会执行后面的代码，用poll来监听写事件
- goto done;//成功了就直接跳转到完成处
-   }  
- struct pollfd fds[1];//定于用于poll的描述符
-   std::memset(fds, 0 , sizeof(fds));//初始化为0
-   fds[0].fd = socket_;//描述符为socket
-   fds[0].events = POLLOUT;//接收写事件
-   ret = poll(fds, 1, connTimeout_);//调用poll，有一个超时值
- if (ret > 0) {  
- // 确保socket已经被连接并且没有错误被设置
- int val;  
-     socklen_t lon;  
-     lon = sizeof(int);  
- int ret2 = getsockopt(socket_, SOL_SOCKET, SO_ERROR, cast_sockopt(&val), &lon);//得到错误选项参数
- if (val == 0) {// socket没有错误也直接到完成处了
- goto done;  
-     }  
-   } elseif (ret == 0) {// socket 超时
- //相应处理代码省略
-   } else {  
- // poll()出错了，相应处理代码省略
-   }  
- 
-  done:  
-   fcntl(socket_, F_SETFL, flags);//设置socket到原来的模式了（阻塞）
- if (path_.empty()) {//如果是unix domain socket就设置缓存地址
-     setCachedAddress(res->ai_addr, res->ai_addrlen);  
-   }  
- }  
　　上面这个函数代码确实比较长，不过还好都是比较简单的代码实现，没有什么很绕的代码，整个流程也很清晰，在代码中也有比较详细的注释了。下面继续看通用socket打开函数local_open（它也真正的执行打开功能也是调用上面刚才介绍的那个函数，只是传递了具体的地址信息）：
**[cpp]**[view plain](http://blog.csdn.net/qiangweiloveforever/article/details/9474941#)[copy](http://blog.csdn.net/qiangweiloveforever/article/details/9474941#)[print](http://blog.csdn.net/qiangweiloveforever/article/details/9474941#)[?](http://blog.csdn.net/qiangweiloveforever/article/details/9474941#)
- void TSocket::local_open(){  
- #ifdef _WIN32
-     TWinsockSingleton::create();//兼容window平台
- #endif // _WIN32
- if (isOpen()) {//打开了就直接返回
- return;  
-   }  
- if (port_ < 0 || port_ > 0xFFFF) {//验证端口是否为有效值
- throw TTransportException(TTransportException::NOT_OPEN, "Specified port is invalid");  
-   }  
- struct addrinfo hints, *res, *res0;  
-   res = NULL;  
-   res0 = NULL;  
- int error;  
- char port[sizeof("65535")];  
-   std::memset(&hints, 0, sizeof(hints));//内存设置为0
-   hints.ai_family = PF_UNSPEC;  
-   hints.ai_socktype = SOCK_STREAM;  
-   hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG;  
-   sprintf(port, "%d", port_);  
-   error = getaddrinfo(host_.c_str(), port, &hints, &res0);//根据主机名得到所有网卡地址信息
- // 循环遍历所有的网卡地址信息，直到有一个成功打开
- for (res = res0; res; res = res->ai_next) {  
- try {  
-       openConnection(res);//调用打开函数
- break;//成功就退出循环
-     } catch (TTransportException& ttx) {  
- if (res->ai_next) {//异常处理，是否还有下一个地址，有就继续
-         close();  
-       } else {  
-         close();  
-         freeaddrinfo(res0); // 清除地址信息内存和资源
- throw;//抛出异常
-       }  
-     }  
-   }  
-   freeaddrinfo(res0);//释放地址结构内存
- }  
　　整个local_open函数就是根据主机名得到所有的网卡信息，然后依次尝试打开，直到打开一个为止就退出循环，如果所有都不成功就抛出一个异常信息。
　　读函数read
　　在实现读函数的时候需要注意区分返回错误为EAGAIN的情况，因为当超时和系统资源耗尽都会产生这个错误（没有明显的特征可以区分它们），所以Thrift在实现的时候设置一个最大的尝试次数，如果超过这个了这个次数就认为是系统资源耗尽了。下面具体看看read函数的实现，代码如下（省略一些参数检查和错误处理的代码）：
**[cpp]**[view plain](http://blog.csdn.net/qiangweiloveforever/article/details/9474941#)[copy](http://blog.csdn.net/qiangweiloveforever/article/details/9474941#)[print](http://blog.csdn.net/qiangweiloveforever/article/details/9474941#)[?](http://blog.csdn.net/qiangweiloveforever/article/details/9474941#)
- uint32_t TSocket::read(uint8_t* buf, uint32_t len) {  
-   int32_t retries = 0;//重试的次数
-   uint32_t eagainThresholdMicros = 0;  
- if (recvTimeout_) {//如果设置了接收超时时间，那么计算最大时间间隔来判断是否系统资源耗尽
-     eagainThresholdMicros = (recvTimeout_*1000)/ ((maxRecvRetries_>0) ? maxRecvRetries_ : 2);  
-   }  
-  try_again:  
- struct timeval begin;  
- if (recvTimeout_ > 0) {  
-     gettimeofday(&begin, NULL);//得到开始时间
-   } else {  
-     begin.tv_sec = begin.tv_usec = 0;//默认为0，不需要时间来判断是超时了
-   }  
- int got = recv(socket_, cast_sockopt(buf), len, 0);//从socket接收数据
- int errno_copy = errno; //保存错误代码
-   ++g_socket_syscalls;//系统调用次数统计加1
- if (got < 0) {//如果读取错误
- if (errno_copy == EAGAIN) {//是否为EAGAIN
- if (recvTimeout_ == 0) {//如果没有设置超时时间，那么就是资源耗尽错误了！抛出异常
- throw TTransportException(TTransportException::TIMED_OUT, "EAGAIN (unavailable resources)");  
-       }  
- struct timeval end;  
-       gettimeofday(&end, NULL);//得到结束时间，会改变errno，所以前面需要保存就是这个原因
-       uint32_t readElapsedMicros =  (((end.tv_sec - begin.tv_sec) * 1000 * 1000)//计算消耗的时间
-                                      + (((uint64_t)(end.tv_usec - begin.tv_usec))));  
- if (!eagainThresholdMicros || (readElapsedMicros < eagainThresholdMicros)) {  
- if (retries++ < maxRecvRetries_) {//重试次数还小于最大重试次数
-           usleep(50);//睡眠50毫秒
- goto try_again;//再次尝试从socket读取数据
-         } else {//否则就认为是资源不足了
- throw TTransportException(TTransportException::TIMED_OUT, "EAGAIN (unavailable resources)");  
-         }  
-       } else {//推测为超时了
- throw TTransportException(TTransportException::TIMED_OUT, "EAGAIN (timed out)");  
-       }  
-     }  
- if (errno_copy == EINTR && retries++ < maxRecvRetries_) {//如果是中断并且重试次数没有超过
- goto try_again;//那么重试
-     }  
-     #if defined __FreeBSD__ || defined __MACH__
- if (errno_copy == ECONNRESET) {//FreeBSD和MACH特殊处理错误代码
- return 0;  
-     }  
-     #endif
- #ifdef _WIN32
- if(errno_copy == WSAECONNRESET) {//win32平台处理错误代码
- return 0; // EOF
-     }  
- #endif
- return got;  
- }  
　　整个读函数其实没有什么特别的，主要的任务就是错误情况的处理，从这里可以看出其实实现一个功能是很容易的，但是要做到稳定和容错性确实需要发很大功夫。
　　写函数write
　　写函数和读函数实现差不多，主要的代码还是在处理错误上面，还有一点不同的是写函数写的内容可能一次没有发送完毕，所以是在一个while循环中一直发送直到指定的内容全部发送完毕。代码实现如下：
**[cpp]**[view plain](http://blog.csdn.net/qiangweiloveforever/article/details/9474941#)[copy](http://blog.csdn.net/qiangweiloveforever/article/details/9474941#)[print](http://blog.csdn.net/qiangweiloveforever/article/details/9474941#)[?](http://blog.csdn.net/qiangweiloveforever/article/details/9474941#)
- void TSocket::write(const uint8_t* buf, uint32_t len) {  
-   uint32_t sent = 0;//记录已经发送了的字节数
- while (sent < len) {//是否已经发送了指定的字节长度
-     uint32_t b = write_partial(buf + sent, len - sent);//调部分写入函数
- if (b == 0) {//发送超时过期了
- throw TTransportException(TTransportException::TIMED_OUT, "send timeout expired");  
-     }  
-     sent += b;//已经发送的字节数
-   }  
- }  
- 上面的函数还没有这种的调用send函数发送写入的内容，而是调用部分写入函数write_partial写入，这个函数实现如下：  
- uint32_t TSocket::write_partial(const uint8_t* buf, uint32_t len) {  
-   uint32_t sent = 0;  
- int flags = 0;  
- #ifdef MSG_NOSIGNAL 
- //使用这个代替SIGPIPE 错误，代替我们检查返回EPIPE错误条件和关闭socket的情况
-   flags |= MSG_NOSIGNAL;//设置这个标志位
- #endif 
- int b = send(socket_, const_cast_sockopt(buf + sent), len - sent, flags);//发送数据
-   ++g_socket_syscalls;//系统调用计数加1
- if (b < 0) { //错误处理
- if (errno == EWOULDBLOCK || errno == EAGAIN) {  
- return 0;//应该阻塞错误直接返回
-     }  
- int errno_copy = errno;//保存错误代码
- if (errno_copy == EPIPE || errno_copy == ECONNRESET || errno_copy == ENOTCONN) {  
-       close();//连接错误关闭掉socket
-     }  
-   }  
- return b;//返回写入的字节数
- }  
　　这个写入的实现逻辑和过程也是非常简单的，只是需要考虑到各种错误的情况并且相应的处理之。
　　其他函数
　　TSocket类还有一些其他函数，不过功能都比较简单，比如设置一些超时和得到一些成员变量值的函数，哪些函数一般都是几句代码完成了。
            
