# JAVA NIO non-blocking模式实现高并发服务器 - z69183787的专栏 - CSDN博客
2014年04月14日 11:12:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：15554
Java自1.4以后,加入了新IO特性,NIO. 号称new IO. NIO带来了non-blocking特性. 这篇文章主要讲的是如何使用NIO的网络新特性,来构建高性能非阻塞并发服务器.
文章基于个人理解,我也来搞搞NIO.,求指正.
## 在NIO之前
服务器还是在使用阻塞式的java socket. 以Tomcat最新版本没有开启NIO模式的源码为例, tomcat会accept出来一个socket连接,然后调用processSocket方法来处理socket.
|`01`|`while``(``true``) {`|
|`02`|`....`|
|`03`|`    ``Socket socket = ``null``;`|
|`04`|`    ``try``{`|
|`05`|`        ``// Accept the next incoming connection from the server`|
|`06`|`        ``// socket`|
|`07`|`        ``socket = serverSocketFactory.acceptSocket(serverSocket);`|
|`08`|`    ``}`|
|`09`|`...`|
|`10`|`...`|
|`11`|`    ``// Configure the socket`|
|`12`|`    ``if``(running && !paused && setSocketOptions(socket)) {`|
|`13`|`        ``// Hand this socket off to an appropriate processor`|
|`14`|`        ``if``(!processSocket(socket)) {`|
|`15`|`            ``countDownConnection();`|
|`16`|`            ``// Close socket right away(socket);`|
|`17`|`            ``closeSocket(socket);`|
|`18`|`        ``}`|
|`19`|`    ``}`|
|`20`|`....`|
|`21`|`}`|
使用ServerSocket.accept()方法来创建一个连接. accept方法是阻塞方法,在下一个connection进来之前,accept会阻塞.
在一个socket进来之后,Tomcat会在thread pool里面拿出一个thread来处理连接的socket. 然后自己快速的脱身去接受下一个socket连接. 代码如下:
|`01`|`    ``protected``boolean``processSocket(Socket socket) {`|
|`02`|`        ``// Process the request from this socket`|
|`03`|`        ``try``{`|
|`04`|`            ``SocketWrapper<Socket> wrapper = ``new``SocketWrapper<Socket>(socket);`|
|`05`|`            ``wrapper.setKeepAliveLeft(getMaxKeepAliveRequests());`|
|`06`|`            ``// During shutdown, executor may be null - avoid NPE`|
|`07`|`            ``if``(!running) {`|
|`08`|`                ``return``false``;`|
|`09`|`            ``}`|
|`10`|`            ``getExecutor().execute(``new``SocketProcessor(wrapper));`|
|`11`|`        ``} ``catch``(RejectedExecutionException x) {`|
|`12`|`            ``log.warn(``"Socket processing request was rejected for:"``+socket,x);`|
|`13`|`            ``return``false``;`|
|`14`|`        ``} ``catch``(Throwable t) {`|
|`15`|`            ``ExceptionUtils.handleThrowable(t);`|
|`16`|`            ``// This means we got an OOM or similar creating a thread, or that`|
|`17`|`            ``// the pool and its queue are full`|
|`18`|`            ``log.error(sm.getString(``"endpoint.process.fail"``), t);`|
|`19`|`            ``return``false``;`|
|`20`|`        ``}`|
|`21`|`        ``return``true``;`|
|`22`|`    ``}`|
而每个处理socket的线程,也总是会阻塞在while(true) sockek.getInputStream().read() 方法上. 
总结就是, 一个socket必须使用一个线程来处理. 致使服务器需要维护比较多的线程. 线程本身就是一个消耗资源的东西,并且每个处理socket的线程都会阻塞在read方法上,使得系统大量资源被浪费.
以上这种socket的服务方式适用于HTTP服务器,每个http请求都是短期的,无状态的,并且http后台的业务逻辑也一般比较复杂. 使用多线程和阻塞方式是合适的.
倘若是做游戏服务器,尤其是CS架构的游戏.这种传统模式服务器毫无胜算.游戏有以下几个特点是传统服务器不能胜任的:
1, 持久TCP连接. 每一个client和server之间都存在一个持久的连接.当CCU(并发用户数量)上升,阻塞式服务器无法为每一个连接运行一个线程.
2, 自己开发的二进制流传输协议. 游戏服务器讲究响应快.那网络传输也要节省时间. HTTP协议的冗余内容太多,一个好的游戏服务器传输协议,可以使得message压缩到3-6倍甚至以上.这就使得游戏服务器要开发自己的协议解析器.
3, 传输双向,且消息传输频率高.假设一个游戏服务器instance连接了2000个client,每个client平均每秒钟传输1-10个message,一个message大约几百字节或者几千字节.而server也需要向client广播其他玩家的当前信息.这使得服务器需要有高速处理消息的能力.
4, CS架构的游戏服务器端的逻辑并不像APP服务器端的逻辑那么复杂. 网络游戏在client端处理了大部分逻辑,server端负责简单逻辑,甚至只是传递消息.
## 在Java NIO出现以后
出现了使用NIO写的非阻塞网络引擎,比如Apache Mina, JBoss Netty, Smartfoxserver BitSwarm. 比较起来, Mina的性能不如后两者.Tomcat也存在NIO模式,不过需要人工开启.
首先要说明一下, 与App Server的servlet开发模式不一样, 在Mina, Netty和BitSwarm上开发应用程序都是Event Driven的设计模式.Server端会收到Client端的event,Client也会收到Server端的event,Server端与Client端的都要注册各种event的EventHandler来handle event.
用大白话来解释NIO:
1, Buffers, 网络传输字节存放的地方.无论是从channel中取,还是向channel中写,都必须以Buffers作为中间存贮格式.
2, Socket Channels. Channel是网络连接和buffer之间的数据通道.每个连接一个channel.就像之前的socket的stream一样.
3, Selector. 像一个巡警,在一个片区里面不停的巡逻. 一旦发现事件发生,立刻将事件select出来.不过这些事件必须是提前注册在selector上的. select出来的事件打包成SelectionKey.里面包含了事件的发生事件,地点,人物. 如果警察不巡逻,每个街道(socket)分配一个警察(thread),那么一个片区有几条街道,就需要几个警察.但现在警察巡逻了,一个巡警(selector)可以管理所有的片区里面的街道(socketchannel).
以上把警察比作线程,街道比作socket或socketchannel,街道上发生的一切比作stream.把巡警比作selector,引起巡警注意的事件比作selectionKey.
从上可以看出,使用NIO可以使用一个线程,就能维护多个持久TCP连接.
## NIO实例
下面给出NIO编写的EchoServer和Client. Client连接server以后,将发送一条消息给server. Server会原封不懂的把消息发送回来.Client再把消息发送回去.Server再发回来.用不休止. 在性能的允许下,Client可以启动任意多.
以下Code涵盖了NIO里面最常用的方法和连接断开诊断.注释也全.
首先是Server的实现. Server端启动了2个线程,connectionBell线程用于巡逻新的连接事件. readBell线程用于读取所有channel的数据. 注解: Mina采取了同样的做法,只是readBell线程启动的个数等于处理器个数+1. 由此可见,NIO只需要少量的几个线程就可以维持非常多的并发持久连接.
每当事件发生,会调用dispatch方法去处理event. 一般情况,会使用一个ThreadPool来处理event. ThreadPool的大小可以自定义.但不是越大越好.如果处理event的逻辑比较复杂,比如需要额外网络连接或者复杂数据库查询,那ThreadPool就需要稍微大些.(猜测)Smartfoxserver处理上万的并发,也只用到了3-4个线程来dispatch
 event.
EchoServer
|`001`|`public``class``EchoServer {`|
|`002`|`    ``public``static``SelectorLoop connectionBell;`|
|`003`|`    ``public``static``SelectorLoop readBell;`|
|`004`|`    ``public``boolean``isReadBellRunning=``false``;`|
|`005`||
|`006`|`    ``public``static``void``main(String[] args) ``throws``IOException {`|
|`007`|`        ``new``EchoServer().startServer();`|
|`008`|`    ``}`|
|`009`|`    `|
|`010`|`    ``// 启动服务器`|
|`011`|`    ``public``void``startServer() ``throws``IOException {`|
|`012`|`        ``// 准备好一个闹钟.当有链接进来的时候响.`|
|`013`|`        ``connectionBell = ``new``SelectorLoop();`|
|`014`|`        `|
|`015`|`        ``// 准备好一个闹装,当有read事件进来的时候响.`|
|`016`|`        ``readBell = ``new``SelectorLoop();`|
|`017`|`        `|
|`018`|`        ``// 开启一个server channel来监听`|
|`019`|`        ``ServerSocketChannel ssc = ServerSocketChannel.open();`|
|`020`|`        ``// 开启非阻塞模式`|
|`021`|`        ``ssc.configureBlocking(``false``);`|
|`022`|`        `|
|`023`|`        ``ServerSocket socket = ssc.socket();`|
|`024`|`        ``socket.bind(``new``InetSocketAddress(``"localhost"``,``7878``));`|
|`025`|`        `|
|`026`|`        ``// 给闹钟规定好要监听报告的事件,这个闹钟只监听新连接事件.`|
|`027`|`        ``ssc.register(connectionBell.getSelector(), SelectionKey.OP_ACCEPT);`|
|`028`|`        ``new``Thread(connectionBell).start();`|
|`029`|`    ``}`|
|`030`|`    `|
|`031`|`    ``// Selector轮询线程类`|
|`032`|`    ``public``class``SelectorLoop ``implements``Runnable {`|
|`033`|`        ``private``Selector selector;`|
|`034`|`        ``private``ByteBuffer temp = ByteBuffer.allocate(``1024``);`|
|`035`|`        `|
|`036`|`        ``public``SelectorLoop() ``throws``IOException {`|
|`037`|`            ``this``.selector = Selector.open();`|
|`038`|`        ``}`|
|`039`|`        `|
|`040`|`        ``public``Selector getSelector() {`|
|`041`|`            ``return``this``.selector;`|
|`042`|`        ``}`|
|`043`||
|`044`|`        ``@Override`|
|`045`|`        ``public``void``run() {`|
|`046`|`            ``while``(``true``) {`|
|`047`|`                ``try``{`|
|`048`|`                    ``// 阻塞,只有当至少一个注册的事件发生的时候才会继续.`|
|`049`|`                    ``this``.selector.select();`|
|`050`|`                    `|
|`051`|`                    ``Set<SelectionKey> selectKeys = ``this``.selector.selectedKeys();`|
|`052`|`                    ``Iterator<SelectionKey> it = selectKeys.iterator();`|
|`053`|`                    ``while``(it.hasNext()) {`|
|`054`|`                        ``SelectionKey key = it.next();`|
|`055`|`                        ``it.remove();`|
|`056`|`                        ``// 处理事件. 可以用多线程来处理.`|
|`057`|`                        ``this``.dispatch(key);`|
|`058`|`                    ``}`|
|`059`|`                ``} ``catch``(IOException e) {`|
|`060`|`                    ``e.printStackTrace();`|
|`061`|`                ``} ``catch``(InterruptedException e) {`|
|`062`|`                    ``e.printStackTrace();`|
|`063`|`                ``}`|
|`064`|`            ``}`|
|`065`|`        ``}`|
|`066`|`        `|
|`067`|`        ``public``void``dispatch(SelectionKey key) ``throws``IOException, InterruptedException {`|
|`068`|`            ``if``(key.isAcceptable()) {`|
|`069`|`                ``// 这是一个connection accept事件, 并且这个事件是注册在serversocketchannel上的.`|
|`070`|`                ``ServerSocketChannel ssc = (ServerSocketChannel) key.channel();`|
|`071`|`                ``// 接受一个连接.`|
|`072`|`                ``SocketChannel sc = ssc.accept();`|
|`073`|`                `|
|`074`|`                ``// 对新的连接的channel注册read事件. 使用readBell闹钟.`|
|`075`|`                ``sc.configureBlocking(``false``);`|
|`076`|`                ``sc.register(readBell.getSelector(), SelectionKey.OP_READ);`|
|`077`|`                `|
|`078`|`                ``// 如果读取线程还没有启动,那就启动一个读取线程.`|
|`079`|`                ``synchronized``(EchoServer.``this``) {`|
|`080`|`                    ``if``(!EchoServer.``this``.isReadBellRunning) {`|
|`081`|`                        ``EchoServer.``this``.isReadBellRunning = ``true``;`|
|`082`|`                        ``new``Thread(readBell).start();`|
|`083`|`                    ``}`|
|`084`|`                ``}`|
|`085`|`                `|
|`086`|`            ``} ``else``if``(key.isReadable()) {`|
|`087`|`                ``// 这是一个read事件,并且这个事件是注册在socketchannel上的.`|
|`088`|`                ``SocketChannel sc = (SocketChannel) key.channel();`|
|`089`|`                ``// 写数据到buffer`|
|`090`|`                ``int``count = sc.read(temp);`|
|`091`|`                ``if``(count < ``0``) {`|
|`092`|`                    ``// 客户端已经断开连接.`|
|`093`|`                    ``key.cancel();`|
|`094`|`                    ``sc.close();`|
|`095`|`                    ``return``;`|
|`096`|`                ``}`|
|`097`|`                ``// 切换buffer到读状态,内部指针归位.`|
|`098`|`                ``temp.flip();`|
|`099`|`                ``String msg = Charset.forName(``"UTF-8"``).decode(temp).toString();`|
|`100`|`                ``System.out.println(``"Server received ["``+msg+``"] from client address:"``+ sc.getRemoteAddress());`|
|`101`|`                `|
|`102`|`                ``Thread.sleep(``1000``);`|
|`103`|`                ``// echo back.`|
|`104`|`                ``sc.write(ByteBuffer.wrap(msg.getBytes(Charset.forName(``"UTF-8"``))));`|
|`105`|`                `|
|`106`|`                ``// 清空buffer`|
|`107`|`                ``temp.clear();`|
|`108`|`            ``}`|
|`109`|`        ``}`|
|`110`|`        `|
|`111`|`    ``}`|
|`112`||
|`113`|`}`|
接下来就是Client的实现.Client可以用传统IO,也可以使用NIO.这个例子使用的NIO,单线程.
|`001`|`public``class``Client ``implements``Runnable {`|
|`002`|`    ``// 空闲计数器,如果空闲超过10次,将检测server是否中断连接.`|
|`003`|`    ``private``static``int``idleCounter = ``0``;`|
|`004`|`    ``private``Selector selector;`|
|`005`|`    ``private``SocketChannel socketChannel;`|
|`006`|`    ``private``ByteBuffer temp = ByteBuffer.allocate(``1024``);`|
|`007`||
|`008`|`    ``public``static``void``main(String[] args) ``throws``IOException {`|
|`009`|`        ``Client client= ``new``Client();`|
|`010`|`        ``new``Thread(client).start();`|
|`011`|`        ``//client.sendFirstMsg();`|
|`012`|`    ``}`|
|`013`|`    `|
|`014`|`    ``public``Client() ``throws``IOException {`|
|`015`|`        ``// 同样的,注册闹钟.`|
|`016`|`        ``this``.selector = Selector.open();`|
|`017`|`        `|
|`018`|`        ``// 连接远程server`|
|`019`|`        ``socketChannel = SocketChannel.open();`|
|`020`|`        ``// 如果快速的建立了连接,返回true.如果没有建立,则返回false,并在连接后出发Connect事件.`|
|`021`|`        ``Boolean isConnected = socketChannel.connect(``new``InetSocketAddress(``"localhost"``, ``7878``));`|
|`022`|`        ``socketChannel.configureBlocking(``false``);`|
|`023`|`        ``SelectionKey key = socketChannel.register(selector, SelectionKey.OP_READ);`|
|`024`|`        `|
|`025`|`        ``if``(isConnected) {`|
|`026`|`            ``this``.sendFirstMsg();`|
|`027`|`        ``} ``else``{`|
|`028`|`            ``// 如果连接还在尝试中,则注册connect事件的监听. connect成功以后会出发connect事件.`|
|`029`|`            ``key.interestOps(SelectionKey.OP_CONNECT);`|
|`030`|`        ``}`|
|`031`|`    ``}`|
|`032`|`    `|
|`033`|`    ``public``void``sendFirstMsg() ``throws``IOException {`|
|`034`|`        ``String msg = ``"Hello NIO."``;`|
|`035`|`        ``socketChannel.write(ByteBuffer.wrap(msg.getBytes(Charset.forName(``"UTF-8"``))));`|
|`036`|`    ``}`|
|`037`||
|`038`|`    ``@Override`|
|`039`|`    ``public``void``run() {`|
|`040`|`        ``while``(``true``) {`|
|`041`|`            ``try``{`|
|`042`|`                ``// 阻塞,等待事件发生,或者1秒超时. num为发生事件的数量.`|
|`043`|`                ``int``num = ``this``.selector.select(``1000``);`|
|`044`|`                ``if``(num ==``0``) {`|
|`045`|`                    ``idleCounter ++;`|
|`046`|`                    ``if``(idleCounter >``10``) {`|
|`047`|`                        ``// 如果server断开了连接,发送消息将失败.`|
|`048`|`                        ``try``{`|
|`049`|`                            ``this``.sendFirstMsg();`|
|`050`|`                        ``} ``catch``(ClosedChannelException e) {`|
|`051`|`                            ``e.printStackTrace();`|
|`052`|`                            ``this``.socketChannel.close();`|
|`053`|`                            ``return``;`|
|`054`|`                        ``}`|
|`055`|`                    ``}`|
|`056`|`                    ``continue``;`|
|`057`|`                ``} ``else``{`|
|`058`|`                    ``idleCounter = ``0``;`|
|`059`|`                ``}`|
|`060`|`                ``Set<SelectionKey> keys = ``this``.selector.selectedKeys();`|
|`061`|`                ``Iterator<SelectionKey> it = keys.iterator();`|
|`062`|`                ``while``(it.hasNext()) {`|
|`063`|`                    ``SelectionKey key = it.next();`|
|`064`|`                    ``it.remove();`|
|`065`|`                    ``if``(key.isConnectable()) {`|
|`066`|`                        ``// socket connected`|
|`067`|`                        ``SocketChannel sc = (SocketChannel)key.channel();`|
|`068`|`                        ``if``(sc.isConnectionPending()) {`|
|`069`|`                            ``sc.finishConnect();`|
|`070`|`                        ``}`|
|`071`|`                        ``// send first message;`|
|`072`|`                        ``this``.sendFirstMsg();`|
|`073`|`                    ``}`|
|`074`|`                    ``if``(key.isReadable()) {`|
|`075`|`                        ``// msg received.`|
|`076`|`                        ``SocketChannel sc = (SocketChannel)key.channel();`|
|`077`|`                        ``this``.temp = ByteBuffer.allocate(``1024``);`|
|`078`|`                        ``int``count = sc.read(temp);`|
|`079`|`                        ``if``(count<``0``) {`|
|`080`|`                            ``sc.close();`|
|`081`|`                            ``continue``;`|
|`082`|`                        ``}`|
|`083`|`                        ``// 切换buffer到读状态,内部指针归位.`|
|`084`|`                        ``temp.flip();`|
|`085`|`                        ``String msg = Charset.forName(``"UTF-8"``).decode(temp).toString();`|
|`086`|`                        ``System.out.println(``"Client received ["``+msg+``"] from server address:"``+ sc.getRemoteAddress());`|
|`087`|`                        `|
|`088`|`                        ``Thread.sleep(``1000``);`|
|`089`|`                        ``// echo back.`|
|`090`|`                        ``sc.write(ByteBuffer.wrap(msg.getBytes(Charset.forName(``"UTF-8"``))));`|
|`091`|`                        `|
|`092`|`                        ``// 清空buffer`|
|`093`|`                        ``temp.clear();`|
|`094`|`                    ``}`|
|`095`|`                ``}`|
|`096`|`            ``} ``catch``(IOException e) {`|
|`097`|`                ``e.printStackTrace();`|
|`098`|`            ``} ``catch``(InterruptedException e) {`|
|`099`|`                ``e.printStackTrace();`|
|`100`|`            ``}`|
|`101`|`        ``}`|
|`102`|`    ``}`|
|`103`||
|`104`|`}`|
下载以后黏贴到eclipse中, 先运行EchoServer,然后可以运行任意多的Client. 停止Server和client的方式就是直接terminate server.
