# iOS经典讲解之网络编程 TCP/UDP HTTP - xqhrs232的专栏 - CSDN博客
2019年02月27日 16:49:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：73
个人分类：[Mac OS/i OS技术](https://blog.csdn.net/xqhrs232/article/category/1252162)
原文地址::[https://blog.csdn.net/loving_ios/article/details/50421698](https://blog.csdn.net/loving_ios/article/details/50421698)
相关文章
1、[iOS 网络编程 TCP/UDP HTTP](https://www.cnblogs.com/fengmin/p/5006417.html)----[https://www.cnblogs.com/fengmin/p/5006417.html](https://www.cnblogs.com/fengmin/p/5006417.html)
2、iOS TCP协议的理解(基于Socket【套接字】)----[https://blog.csdn.net/qq_18505715/article/details/51043046](https://blog.csdn.net/qq_18505715/article/details/51043046)
3、iOS-Socket-UDP/TCP:客户端点对点和客户端对服务器数据收发----[https://www.jianshu.com/p/2e408c3eaaa8](https://www.jianshu.com/p/2e408c3eaaa8)
4、[iOS Socket/Tcp编程 GCDAsyncSocket的实战(带回调)](https://www.cnblogs.com/xiaoxiaoyublogs/p/5855852.html)----[https://www.cnblogs.com/xiaoxiaoyublogs/p/5855852.html](https://www.cnblogs.com/xiaoxiaoyublogs/p/5855852.html)
5、[iOS Socket编程-C语言版（TCP）](https://www.cnblogs.com/Free-Thinker/p/10442390.html)----[https://www.cnblogs.com/Free-Thinker/p/10442390.html](https://www.cnblogs.com/Free-Thinker/p/10442390.html)
6、IOS实例 实现基于Socket TCP/IP通讯----[http://mobile.51cto.com/iphone-278129.htm](http://mobile.51cto.com/iphone-278129.htm)
一、HTTP协议的主要特点：（摘自 [仰望星空](https://www.baidu.com/s?wd=%E4%BB%B0%E6%9C%9B%E6%98%9F%E7%A9%BA&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd) 的博客）**重点内容**
1. CS模式 
2. 简单快速：只需要传送请求方法和路径。（常用方法有GET,HEAD,POST） 
3. 灵活：任意对象都可以，类型由Content-Type加以标记 
4. 无连接、无状态 即每次连接只处理一个请求，对于事务处理没有记忆能力 
http表示要通过HTTP协议来定位网络资源；host表示合法的Internet主机域名或者IP地址；port制定一个端口号，为空时使用缺省端口号80；abs_path指定请求资源的URI;如果URL中没有给出abs_path，那么当它作为请求URI时，必须以“/”的形式给出（此过程由浏览器完成）。
二、TCP/UDP区别和联系 
1.TCP是面向连接的可靠的传输控制协议，UDP是面向非连接的用户数据报协议. 
2.TCP（三次握手保证相对可靠性）可传大量数据，速度相对比较慢，UDP一次性传输少量对可靠性要求不高的数据，速度比较快 
tcp一般用于音频、视频等数据的传输，资源能耗比较小，对可靠性要求不高，即使丢失一两条数据也不会产生太大影响。
三、Socket连接和Http连接的区别 
1.http是基于socket之上的，socket是一套完成tcp和udp协议的接口 
2.HTTP协议：简单对象访问协议，对应于应用层 ，HTTP协议是基于TCP连接的 
3.tcp协议： 对应于传输层 
4.ip协议： 对应于网络层 
TCP/IP是传输层协议，主要解决数据如何在网络中传输；而HTTP是应用层协议，主要解决如何包装数据。 
Socket是对TCP/IP协议的封装，Socket本身并不是协议，而是一个调用接口（API），通过Socket，我们才能使用TCP/IP协议。 
http是短连接，客户端向服务端发送一次请求，服务端响应后连接即断掉；socket是长连接，一般情况下，如果[服务器](https://www.baidu.com/s?wd=%E6%9C%8D%E5%8A%A1%E5%99%A8&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)端或者客户端主机down了，网络故障，或者两者长时间没有数据传输，连接可能会断。所以当以个socket连接中没有数据的传输，为了维持连接需要发送心跳消息。
四、三次握手的过程不再赘述，主要来了解下socket建立网络连接的步骤 
建立socket连接至少需要一堆套接字，其中一个运行于客户端，另一个运行于服务端（ClientSocket、ServerSocket） 
套接字建立连接的过程分为三步：服务器监听、客户端请求、连接确认 
1。服务器监听：服务器端套接字并不定位具体的客户端套接字，而是处于等待连接的状态，[实时监控](https://www.baidu.com/s?wd=%E5%AE%9E%E6%97%B6%E7%9B%91%E6%8E%A7&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)网络状态，等待客户端的连接请求。 
2。客户端请求：指客户端的套接字提出连接请求，要连接的目标是服务器端的套接字。为此，客户端的套接字必须首先描述它要连接的服务器的套接字，指出服务器端套接字的**地址**和**端口号**，然后就向服务器端套接字提出连接请求。 
3。连接确认：当服务器端套接字监听到或者说接收到客户端套接字的连接请求时，就响应客户端套接字的请求，建立一个新的线程，把服务器端套接字的描述发给客户端，一旦客户端确认了此描述，双方就正式建立连接。而服务器端套接字继续处于监听状态，继续接收其他客户端套接字的连接请求。
五、HTTP连接最显著的特点是客户端发送的每次请求都需要服务器回送响应，在请求结束后，会主动释放连接。从建立连接到关闭连接的过程称为“一次连接”。
## /*在进行编程前，先了解下AsyncSocket*/
AsyncSocket是IOS下专门用于socket套接字开发的开源库，它封装了CFNetwork/BSD Socket.提供了异步的开发模型和简便的开发接口。它支持TCP/UDP,支持UDP组播 
AsyncSocket支持GCD/Runloop模式 支持ARC 使用时需要增加两个库 CFNetwork.frame和Security.frame
六、UDP编程 
server端流程： 
1. socket创建套接字 
2. bind绑定port 
3. recv接收、send发送数据 
4. close关闭socket套接字 
client端流程 
1.socket创建套接字 
2.bind绑定port端口（可选） 
3. recv接收 send发送数据 
4. close关闭socket套机制
- 
`UDP编程涉及到 ip和字符串的转化如下`
- 
`/*绑定ip地址 */`
- 
`inet_pton(AF_INET,"192.168.101.2",&addr.sin_addr);`
- 
`/*把地址sin_addr转化成字符串ipBuf*/`
- 
`inet_ntop(AF_INET,&srcaddr.sin_addr,ipBuf,16);`
UDP基本API 
1. 创建UDP的套接字`int sd = socket(AF_INET,SOCK_DGRAM,0);`
2. 设置端口可以重用 
```
int nOptval = 1; 
ret = setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,(const void*)&nOptval,sizeof(int));
```
3. 绑定端口 `ret = bind(sd,(struct sockaddr *)&addr,addrlen);`
4. 给某个地址发送数据`ssize_t sendLen = sendto(sd,res,strlen(res),0,(struct sockaddr *)&srcaddr,sizeof(srcaddr));`
5. 接收来自sd的数据`ssize_t recvLen = recvfrom(sd,&info,sizeof(info),0,(struct sockaddr *)&srcaddr,&addrlen);`
6. 关闭套接字 `Close(sd);`
UDP 编程示例 
server端 ： 新建工程 Cococa Application 引入AsyncSocket 
引入头文件 #import “AsyncUdpSocket.h” 以及代理 AsyncUdpSocketDelegate
- 
`_udpSocketServer = [[AsyncUdpSocket alloc] initWithDelegate:self];`
- 
`//绑定端口 用于标识socket对象`
- 
`if(![_udpSocketServer bindToPort:5678 error:nil]){`
- 
- 
`NSLog(@"绑定失败");`
- 
`}`
- 
- 
`//监听状态 是否有客户端发送来的消息`
- 
`[_udpSocketServer receiveWithTimeout:-1 tag:100];`
代理回调
- 
`#pragma maek- socketDelegate`
- 
`//收到消息时的回调`
- 
`-(BOOL)onUdpSocket:(AsyncUdpSocket *)sock didReceiveData:(NSData *)data withTag:(long)tag fromHost:(NSString *)host port:(UInt16)port{`
- 
- 
`NSLog(@"port:%d",port);`
- 
`NSString* messege = [[NSString alloc]initWithData:data encoding:NSUTF8StringEncoding];`
- 
`NSLog(@"recieve:%@",messege);`
- 
`[sock receiveWithTimeout:-1 tag:200];`
- 
`return YES;`
- 
- 
`}`
上述代码即可实现简单的UDPServer端
UDPClient 代码（简单示例） 
同样的要声明UDP对象 然后发送消息
- 
`NSString* messege = @"UDPClient 消息";`
- 
`NSData* data = [messege dataUsingEncoding:NSUTF8StringEncoding];`
- 
`/*`
- 
`*发送消息`
- 
`host: 制定服务器的ip地址`
- 
`port: 服务器的端口`
- 
`-1 不限时发送`
- 
`tag 对此次操作的标记`
- 
`*/`
- 
`[_clientSocket sendData:data toHost:@"127.0.0.1" port:5678 withTimeout:-1 tag:0];`
- 
`/*发送成功的回调方法是*/`
- 
`- (void)onUdpSocket:(AsyncUdpSocket *)sock didSendDataWithTag:(long)tag{`
- 
`NSLog(@"发送成功!");`
- 
`}`
TCP编程 
server端流程 
1. socket创建socket套接字 
2. bind绑定port端口 
3. listen监听端口 
4. close关闭socket套机制 
client端流程 
1.socket创建套接字 
2.bind绑定port端口（可选） 
3.connect服务器端口 
4.close关闭socket套机制
了解下 tcp的重传策略： TCP用于控制数据段是否需要重传的依据是设立重发定时器。在发送一个数据段的同时启动一个重发定时器，AC(Ackonowlegement)就关闭该定时器，如果在定时器超时前没有收到确认，则重传该数据段。在选择重发时间的过程中，TCP必须具有自适应性。它需要根据互联网当时的通信情况，给出合适的数据重发。
TCP编程实例 
server 
1.引入头文件 #import “AsyncSocket.h” AsyncSocketDelegate
- 
`_tcpServer = [[AsyncSocket alloc]initWithDelegate:self];`
- 
`//服务端对应的ip地址和端口，_serverSocket负责监听是否有客户端接入`
- 
`//[_tcpServer acceptOnInterface:@"127.0.0.1" port:5678 error:nil];`
- 
`//服务端负责监听是否有客户端接入此端口，host可以缺省不写`
- 
- 
`[_tcpServer acceptOnPort:5678 error:nil];`
2.发现有客户端接入时 响应
- 
`-(void)onSocket:(AsyncSocket *)sock didAcceptNewSocket:(AsyncSocket *)newSocket{`
- 
- 
`NSLog(@"new socket host:%@ port:%d",newSocket.connectedHost,newSocket.connectedPort);`
- 
`//将新生成的socket对象加入数组中`
- 
`[_array addObject:newSocket];`
- 
`//负责不限时的监听客户端是否发送消息过来`
- 
`[newSocket readDataWithTimeout:-1 tag:1];`
- 
- 
`}`
3 收到客户端发送过来的消息时
- 
`-(void)onSocket:(AsyncSocket *)sock didReadData:(NSData *)data withTag:(long)tag{`
- 
- 
`NSString *message = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];`
- 
`NSLog(@"receive:%@",message);`
- 
`//告诉sock，继续监听客户端`
- 
`[sock readDataWithTimeout:-1 tag:2];`
- 
`}`
4连接的客户端长时间不活跃时 触发下面的方法
- 
`- (void)onSocket:(AsyncSocket *)sock willDisconnectWithError:(NSError *)err{`
- 
`NSLog(@"willDisconnect!");`
- 
`}`
5 断开连接时
- 
`//已经断开连接`
- 
`- (void)onSocketDidDisconnect:(AsyncSocket *)sock{`
- 
`NSLog(@"%s",__FUNCTION__);//__FUNCTION__ 能够打印出当前函数的名称，一般用于对程序进行暴力调试时`
- 
`}`
client编程 
1。初始化一个AsyncSocket对象 
_clientSocket = [[AsyncSocket alloc] initWithDelegate:self]; 
2. 与指定的服务器进行连接
- 
`if (!_clientSocket) {`
- 
`//创建一个客户端对象，并设置delegate`
- 
`_clientSocket = [[AsyncSocket alloc] initWithDelegate:self];`
- 
`}`
- 
`//先判断是否与指定服务器连接`
- 
`if ([_clientSocket isConnected]) {`
- 
`//断开连接`
- 
`[_clientSocket disconnect];`
- 
`}`
- 
`//与指定服务器连接`
- 
`//connectToHost 服务端的ip地址`
- 
`//port 服务端的端口：端口的值可随意约定`
- 
`[_clientSocket connectToHost:@"127.0.0.1" onPort:5678 error:nil];`
3.发送消息
- 
`NSString *message = @"hello server!";`
- 
`//将数据转换成data`
- 
`NSData *data = [message dataUsingEncoding:NSUTF8StringEncoding];`
- 
`//将data发给服务器`
- 
`//data 发送的数据， timeout:-1 不限时发送, tag，对此次交互的标记`
- 
`[_clientSocket writeData:data withTimeout:-1 tag:0];`
4.回调方法
- 
`//当客户端与服务端连接成功时，调用此方法`
- 
`- (void)onSocket:(AsyncSocket *)sock didConnectToHost:(NSString *)host port:(UInt16)port{`
- 
`NSLog(@"与服务器:%@ %d 相连接",host,port);`
- 
`}`
- 
`//消息发送成功后，调用此方法`
- 
`- (void)onSocket:(AsyncSocket *)sock didWriteDataWithTag:(long)tag{`
- 
`NSLog(@"send!");`
- 
`}`
