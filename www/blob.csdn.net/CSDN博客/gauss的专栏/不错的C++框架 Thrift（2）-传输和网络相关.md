# 不错的C++框架: Thrift（2）-传输和网络相关 - gauss的专栏 - CSDN博客
2015年05月04日 00:24:10[gauss](https://me.csdn.net/mathlmx)阅读数：301
**1.  协议类**
定义了输出string, struct, int等其数据表达格式
TJSONProtocol->TProtocol
TCompactProtocol->TVirtualProtocol->TProtocol
TDenseProtocol->TBinaryProtocol->TVirtualProtocol->TProtocol
class TDebugProtocol : public TVirtualProtocol<TDebugProtocol> 
TVirtualProtocol增加了xx_virt到TProtocol对应接口xx的转发
**2. 传输类**
负责数据的传输，read，write,peek,flush等IO接口
2.1 实际负责传输
TFDTransport
TSocket->TIOStreamTransport->Transport
TZlibTransport
TFileTransport->TFileReaderTransport
TFileTransport->TFileReaderTransport
                        ->TFileWriterTransport（单独线程异步写）
2.2 UTIL传输相关的，进行了特定的保证
(TTransportUtils) TNullTransport,TPipedFileReaderTransport->TPipedTransport
                                                                                              ->TFileReaderTransport
2.3 缓存相关
TBufferedTransport->TBufferBase->TVirtualTransport  //指定大小的缓存
TFramedTransport->TBufferBase->TVirtualTransport    //只有flush才输出
TMemoryTransport->TBufferBase->TVirtualTransport    //可动态自动增加内存
2.4 业务相关的修饰类，对输出的数据进行修饰。实际传输依赖于2.1中的传输类
THttpServer->THttpTransport->TVirtualTransport
THttpClient->THttpTransport->TVirtualTransport
**3. 服务器类**
负责监听和处理转发
TThreadPoolServer， 通过线程池进行外发处理
TThreadedServer， 启用单线程对业务处理，接入和处理处于两个线程
TSimpleServer， 循环的阻塞处理
TZmqMultiServer-(wrap)>TZmqServer， 依赖Zmp 消息通讯中间件的服务器模型
TNonblockingServer， libevent的反应器处理， 其中业务会丢给外部定义的ThreadManager处理   该模型的消息输出前4个是消息体长度的网络字节（htonl(framesize))
**4. 其它**
socket操作
**设置非堵塞**
if ((flags = fcntl(clientSocket, F_GETFL, 0)) < 0 ||
        fcntl(clientSocket, F_SETFL, flags | O_NONBLOCK) < 0)
在TNonblockingServer中accept产生的socket设置，其它情况设置~O_NONBLOCK
accept监听的socket要设置
定义TSocket连接超时(connTimeout_)，连接前设置O_NONBLOCK，然后轮询等超时（(ret = poll(fds, 1, connTimeout_);）；反之设置~O_NONBLOCK 
**其它超时设置**
接收超时和发送超时
recvTimeval_.tv_sec = (int)(recvTimeout_/1000);
  recvTimeval_.tv_usec = (int)((recvTimeout_%1000)*1000);
  // Copy because poll may modify
  struct timeval r = recvTimeval_;
  int ret = setsockopt(socket_, SOL_SOCKET, SO_RCVTIMEO, &r, sizeof(r));
 struct timeval s = {(int)(sendTimeout_/1000),
                      (int)((sendTimeout_%1000)*1000)};
  int ret = setsockopt(socket_, SOL_SOCKET, SO_SNDTIMEO, &s, sizeof(s));
*代码中针对读情况返回EAGAIN时，增加了是否超时判断，然后重试，但写就直接认为资源消耗完，放弃操作*
**TSocket设置是否DELEY**(禁用nagle算法)
 // Set socket to NODELAY
  int v = noDelay_ ? 1 : 0;
  int ret = setsockopt(socket_, IPPROTO_TCP, TCP_NODELAY, &v, sizeof(v));
 struct linger l = {(lingerOn_ ? 1 : 0), lingerVal_};
  int ret = setsockopt(socket_, SOL_SOCKET, SO_LINGER, &l, sizeof(l));
**其它监听端口的设置**
  // Keepalive to ensure full result flushing
  setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, &one, sizeof(one));
  // Turn linger off to avoid hung sockets （套接口关闭时TCP夭折连接，TCP将丢弃保留在套接口发送缓冲区中的任何数据并发送一个RST给对方，而不是通常的四分组终止序列，这避免了TIME_WAIT状态），在传输过程中，可以配置TSocket是否开启改参数
  setsockopt(s, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling));
  // Set TCP nodelay if available, MAC OS X Hack
  // See http://lists.danga.com/pipermail/memcached/2005-March/001240.html
  #ifndef TCP_NOPUSH
  setsockopt(s, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));
  #endif
  #ifdef TCP_LOW_MIN_RTO
  if (TSocket::getUseLowMinRto()) {
    setsockopt(s, IPPROTO_TCP, TCP_LOW_MIN_RTO, &one, sizeof(one));
  }
  #endif
对listen的端口要设置可重用
 // Set reuseaddr to avoid 2MSL delay on server restart
setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
// Defer accept
  #ifdef TCP_DEFER_ACCEPT（内核不等待最后一个ACK包，而且在第一个真正有数据的包到达后才初始化侦听过程，可以减少一些上下文切换，三次握手后，如果没有数据到来，服务器socket处于SYN_RECV状态；使用*TCP_DEFER_ACCEPT*可以减少用户程序hold的连接数，也可以减少用户调用epoll_ctl和epoll_wait的次数，从而提高了程序的性能）
  if (-1 == setsockopt(serverSocket_, SOL_SOCKET, TCP_DEFER_ACCEPT,
                       &one, sizeof(one))) {
**IPV6ONLY设置**
 #ifdef IPV6_V6ONLY
  if (res->ai_family == AF_INET6) {
    int zero = 0;
    if (-1 == setsockopt(s, IPPROTO_IPV6, IPV6_V6ONLY, &zero, sizeof(zero))) {
      GlobalOutput("TServerSocket::listen() IPV6_V6ONLY");
    }
  }
  #endif // #ifdef IPV6_V6ONLY
**BUF设置**
 // Set TCP buffer sizes
  if (tcpSendBuffer_ > 0) {
    if (-1 == setsockopt(serverSocket_, SOL_SOCKET, SO_SNDBUF,
                         &tcpSendBuffer_, sizeof(tcpSendBuffer_)))
if (tcpRecvBuffer_ > 0) {
    if (-1 == setsockopt(serverSocket_, SOL_SOCKET, SO_RCVBUF,
                         &tcpRecvBuffer_, sizeof(tcpRecvBuffer_))) {
注备：
MSL是Maximum Segment Lifetime英文的缩写，中文可以译为“报文最大生存时间”，他是任何报文在网络上存在的最长时间，超过这个时间报文将被丢弃。
2MSL即两倍的MSL，TCP的TIME_WAIT状态也称为2MSL等待状态，当TCP的一端发起主动关闭，在发出最后一个ACK包后，即第3次握手完成后发送了第四次握手的ACK包后就进入了TIME_WAIT状态，必须在此状态上停留两倍的MSL时间，等待2MSL时间主要目的是怕最后一个ACK包对方没收到，那么对方在超时后将重发第三次握手的FIN包，主动关闭端接到重发的FIN包后可以再发一个ACK应答包。在TIME_WAIT状态时两端的端口不能使用，要等到2MSL时间结束才可继续使用。当连接处于2MSL等待阶段时任何迟到的报文段都将被丢弃。不过在实际应用中可以通过设置SO_REUSEADDR选项达到不必等待2MSL时间结束再使用此端口。



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
（1）支持的传输格式
TBinaryProtocol – 二进制格式.
TCompactProtocol – 压缩格式
TJSONProtocol – JSON格式
TSimpleJSONProtocol –提供JSON只写协议, 生成的文件很容易通过脚本语言解析。
TDebugProtocol – 使用易懂的可读的文本格式，以便于debug
（2） 支持的数据传输方式
TSocket -阻塞式socker
TFramedTransport – 以frame为单位进行传输，非阻塞式服务中使用。
TFileTransport – 以文件形式进行传输。
TMemoryTransport – 将内存用于I/O. java实现时内部实际使用了简单的ByteArrayOutputStream。
TZlibTransport – 使用zlib进行压缩， 与其他传输方式联合使用。当前无java实现。
（3）支持的服务模型
TSimpleServer – 简单的单线程服务模型，常用于测试
TThreadPoolServer – 多线程服务模型，使用标准的阻塞式IO。
TNonblockingServer – 多线程服务模型，使用非阻塞式IO（需使用TFramedTransport数据传输方式）
负责数据传输，有以下几个可用类：
TFileTransport：文件（日志）传输类，允许client将文件传给server，允许server将收到的数据写到文件中。
THttpTransport：采用Http传输协议进行数据传输
TSocket：采用TCP Socket进行数据传输
TZlibTransport：压缩后对数据进行传输，或者将收到的数据解压
下面几个类主要是对上面几个类地装饰（采用了装饰模式），以提高传输效率。
TBufferedTransport：对某个Transport对象操作的数据进行buffer，即从buffer中读取数据进行传输，或者将数据直接写入buffer
TFramedTransport：同TBufferedTransport类似，也会对相关数据进行buffer，同时，它支持定长数据发送和接收。
TMemoryBuffer：从一个缓冲区中读写数据
负责数据编码，主要有以下几个可用类：
TBinaryProtocol：二进制编码
TJSONProtocol：JSON编码
TCompactProtocol：密集二进制编码
TDebugProtocol：以用户易读的方式组织数据
TSimpleServer：简单的单线程服务器，主要用于测试
TThreadPoolServer：使用标准阻塞式IO的多线程服务器
TNonblockingServer：使用非阻塞式IO的多线程服务器，TFramedTransport必须使用该类型的server
            
