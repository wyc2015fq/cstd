# 浅析Tomcat NIO 配置 - z69183787的专栏 - CSDN博客
2014年03月22日 14:27:08[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1605
Tomcat 6.X实现了JCP的Servlet 2.5和JSP2.1的规范，并且包括其它很多有用的功能，使它成为开发和部署web应用和web服务的坚实平台。
NIO （No-blocking I/O）从JDK 1.4起，NIO API作为一个基于缓冲区，并能提供非阻塞I/O操作的API被引入。
作为开源web服务器的java实现，tomcat几乎就是web开发者开发、测试的首选，有很多其他商业服务器的开发者也会优先选择tomcat作为开发时候使用，而在部署的时候，把应用发布在商业服务器上。也有许多商业应用部署在tomcat上，tomcat承载着其核心的应用。但是很多开发者很迷惑，为什么在自己的应用里使用tomcat作为平台的时候，而并发用户超过一定数量，服务器就变的非常繁忙，而且很快就出现了connection
 refuse的错误。但是很多商业应用部署在tomcat上运行却安然无恙。
其中有个很大的原因就是，配置良好的tomcat都会使用APR(Apache Portable Runtime),APR是Apache HTTP Server2.x的核心，它是高度可移植的本地库，它使用高性能的UXIN I/O操作，低性能的java io操作，但是APR对很多Java开发者而言可能稍稍有点难度，在很多OS平台上，你可能需要重新编译APR。但是从Tomcat6.0以后, Java开发者很容易就可以是用NIO的技术来提升tomcat的并发处理能力。
但是为什么NIO可以提升tomcat的并发处理能力呢，我们先来看一下java 传统io与 java NIO的差别。
Java 传统的IO操作都是阻塞式的(blocking I/O), 如果有socket的编程基础，你会接触过堵塞socket和非堵塞socket，堵塞socket就是在accept、read、write等IO操作的的时候，如果没有可用符合条件的资源，不马上返回，一直等待直到有资源为止。而非堵塞socket则是在执行select的时候，当没有资源的时候堵塞，当有符合资源的时候，返回一个信号，然后程序就可以执行accept、read、write等操作，一般来说，如果使用堵塞socket，通常我们通常开一个线程accept
 socket，当读完这次socket请求的时候，开一个单独的线程处理这个socket请求；如果使用非堵塞socket，通常是只有一个线程，一开始是select状，当有信号的时候可以通过 可以通过多路复用（Multiplexing)技术传递给一个指定的线程池来处理请求，然后原来的线程继续select状态。 最简单的多路复用技术可以通过java管道(Pipe)来实现。换句话说，如果客户端的并发请求很大的时候，我们可以使用少于客户端并发请求的线程数来处理这些请求，而这些来不及立即处理的请求会被阻塞在java管道或者队列里面，等待线程池的处理。请求
 听起来很复杂，在这个架构当道的java 世界里，现在已经有很多优秀的NIO的架构方便开发者使用，比如Grizzly,Apache Mina等等，如果你对如何编写高性能的网络服务器有兴趣，你可以研读这些源代码。
简单说一下，在web服务器上阻塞IO(BIO)与NIO一个比较重要的不同是，我们使用BIO的时候往往会为每一个web请求引入多线程，每个web请求一个单独的线程，所以并发量一旦上去了，线程数就上去了，CPU就忙着线程切换，所以BIO不合适高吞吐量、高可伸缩的web服务器；而NIO则是使用单线程(单个CPU)或者只使用少量的多线程(多CPU)来接受Socket，而由线程池来处理堵塞在pipe或者队列里的请求.这样的话，只要OS可以接受TCP的连接，web服务器就可以处理该请求。大大提高了web服务器的可伸缩性。
我们来看一下配置，你只需要在server.xml里把 HTTP Connector做如下更改，
- <Connectorport="8080"protocol="HTTP/1.1"
- connectionTimeout="20000"
- redirectPort="8443"/>
改为
- <Connectorport="8080"protocol="org.apache.coyote.http11.Http11NioProtocol"
- connectionTimeout="20000"
- redirectPort="8443"/>
然后启动服务器，你会看到org.apache.coyote.http11.Http11NioProtocol start的信息，表示NIO已经启动。其他的配置请参考官方配置文档。
Enjoy it.
最后贴上官方文档上对tomcat的三种Connector的方式做一个简单比较。
||Java Nio Blocking Connector|Java Nio Blocking Connector|APR Connector|
|----|----|----|----|
|Classname|Http11Protocol|Http11NioProtocol|Http11AprProtocol|
|Tomcat Version|3.x 4.x 5.x 6.x|6.x|5.5.x 6.x|
|Support Polling|NO|YES|YES|
|Polling Size|N/A|Unlimited - Restricted by mem|Unlimited|
|Read HTTP Request|Blocking|Blocking|Blocking|
|Read HTTP Body|Blocking|Blocking|Blocking|
|Write HTTP Response|Blocking|Blocking|Blocking|
|SSL Support|Java SSL|Java SSL|OpenSSL|
|SSL Handshake|Blocking|Non blocking|Blocking|
|Max Connections|maxThreads|See polling size|See polling size|
原文链接：http://northc.iteye.com/blog/1290186
