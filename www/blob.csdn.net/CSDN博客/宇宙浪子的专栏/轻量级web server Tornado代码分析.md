# 轻量级web server Tornado代码分析 - 宇宙浪子的专栏 - CSDN博客
2017年09月20日 17:44:14[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：363标签：[python																[tornado](https://so.csdn.net/so/search/s.do?q=tornado&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[Python																[tornado](https://blog.csdn.net/bluehawksky/article/category/7102850)](https://blog.csdn.net/bluehawksky/article/category/2542577)

转自：[http://blog.csdn.net/goldlevi/article/details/7047726](http://blog.csdn.net/goldlevi/article/details/7047726)
最近在做项目，使用了Tornado这个用python写的由Facebook开源出来的web服务器框架。这确实是一个轻量级的框架，只需要几行代码，直接运行脚本，就可以建立起一个server了。Tornado使用了epoll方式，在linux环境下，用了epoll的，总是受到更多的关注，呵呵。这比我们销售短信项目中使用的c++ Poco库使用的poll模型效率要高一些。再加上python语言本身强大的脚本能力，灰度发布从代码行数上来讲，非常得少。没有括号而依赖缩进的规则，使得Tornado的源代码看起来也比较舒服，而且他们均出自牛人之手，确实是学习网络编程的好对象。
Tornado采用多进程 + 非阻塞 + epoll的模型，可以提供比较强大的网络响应性能。在我们的项目中，单个实例的灰度发布server就可以支持每秒1500次的请求响应。而通过Nginx与tornado一起部署，可以同时支持多个实例的运行，从而支持加倍的请求响应，满足当前旺旺用户的升级需求。下图是旺旺灰度发布的架构图：
现在把Tornado里面的部分内容，以及一些重要的资料分享给大家，有兴趣的同学可以玩一下。
# **1 Tornado来历**
Tornado是一个开源的网络服务器框架，该平台基于社交聚合网站FriendFeed的实时信息服务开发而来。2007年，4名谷歌前软件工程师一起创办了FriendFeed，旨在使用户能方便地跟踪好友在Facebook和Twitter等多个社交网站上的活动。结果两年后，Facebook宣布收购FriendFeed，这一交易的价格约为5000万美元。而此时，FriendFeed只有12名员工。据说这帮人后来又到了Google，搞出了现在的Google App Engine …… 
Tornado由Python编写，跟其他主流的Web服务器框架不同是采用epoll非阻塞IO，响应快速，可处理数千并发连接，特别适用用于实时的Web服务。Tornado当前版本为2.1.1，官方网站为[http://www.tornadoweb.org/](http://www.tornadoweb.org/)，有兴趣的同学可以去尝试一下。
# 2 **Tornado**简介
         Tornado主要包含了如下四部分内容。官方的帮助文档，实际上只是源码注释的集合。大家直接看源码就可以了。
- [Core web framework](http://www.tornadoweb.org/documentation/webframework.html)- [**tornado.web** —**RequestHandler** and **Application** classes](http://www.tornadoweb.org/documentation/web.html)
- [**tornado.httpserver** — Non-blocking HTTP server](http://www.tornadoweb.org/documentation/httpserver.html)
- [**tornado.template** — Flexible output generation](http://www.tornadoweb.org/documentation/template.html)
- [**tornado.escape** — Escaping and string manipulation](http://www.tornadoweb.org/documentation/escape.html)
- [**tornado.locale** — Internationalization support](http://www.tornadoweb.org/documentation/locale.html)
- [Asynchronous networking](http://www.tornadoweb.org/documentation/networking.html)- [**tornado.ioloop** — Main event loop](http://www.tornadoweb.org/documentation/ioloop.html)
- [**tornado.iostream** — Convenient wrappers for non-blocking sockets](http://www.tornadoweb.org/documentation/iostream.html)
- [**tornado.httpclient** — Non-blocking HTTP client](http://www.tornadoweb.org/documentation/httpclient.html)
- [**tornado.netutil** — Miscellaneous network utilities](http://www.tornadoweb.org/documentation/netutil.html)
- [Integration with other services](http://www.tornadoweb.org/documentation/integration.html)- [**tornado.auth** — Third-party login with OpenID and OAuth](http://www.tornadoweb.org/documentation/auth.html)
- [**tornado.database** — Simple MySQL client wrapper](http://www.tornadoweb.org/documentation/database.html)
- [**tornado.platform.twisted** — Run code written for Twisted on Tornado](http://www.tornadoweb.org/documentation/twisted.html)
- [**tornado.websocket** — Bidirectional communication to the browser](http://www.tornadoweb.org/documentation/websocket.html)
- [**tornado.wsgi** — Interoperability with other Python frameworks and servers](http://www.tornadoweb.org/documentation/wsgi.html)
- [Utilities](http://www.tornadoweb.org/documentation/utilities.html)- [**tornado.autoreload** — Automatically detect code changes in development](http://www.tornadoweb.org/documentation/autoreload.html)
- [**tornado.gen** — Simplify asynchronous code](http://www.tornadoweb.org/documentation/gen.html)
- [**tornado.httputil** — Manipulate HTTP headers and URLs](http://www.tornadoweb.org/documentation/httputil.html)
- [**tornado.options** — Command-line parsing](http://www.tornadoweb.org/documentation/options.html)
- [**tornado.process** — Utilities for multiple processes](http://www.tornadoweb.org/documentation/process.html)
- [**tornado.stack_context** — Exception handling across asynchronous callbacks](http://www.tornadoweb.org/documentation/stack_context.html)
- [**tornado.testing** — Unit testing support for asynchronous code](http://www.tornadoweb.org/documentation/testing.html)
今天主要和大家分享一下HTTP SERVER的相关内容。
## 2.1Tornado HTTP SERVER
使用Tornado可以很方便地架构出各种类型的web服务器。我们现在从HTTP服务器入手，来看一下它的实现。下面这张图大家应该见得很多了，是所有web server的一般工作方式。
![](http://hi.csdn.net/attachment/201112/6/0_132317749449y4.gif)
l  服务器端bind到一个端口，然后开始listen。
l  客户端connect上来以后，将请求发送给服务端。
l  服务端处理完成后返回给客户端。
这样，一个请求就处理结束了。不过，当需要处理成千上万的连接的时候，我们就会在这个基础上考虑更多的情况。这也就是大家熟悉的The C10K problem。一般大家会有如下一些选择：
l  一个线程服务多个客户端，使用非阻塞I/O和**水平触发**的就绪通知
l  一个线程服务多个客户端，使用非阻塞I/O和就绪**改变**时通知
l  一个服务线程服务多个客户端，使用异步I/O
l  一个服务线程服务一个客户端，使用阻塞I/O
l  把服务代码编译进内核
Tornado采用的就是：多进程 + 非阻塞 + epoll模型
下面这张图基本上就显示了Tornado与网络相关的所有内容了：
![](http://hi.csdn.net/attachment/201112/6/0_1323177521qHfz.gif)
## 2.2 第一个HTTP server例子
下面是官网提供的一个hello world的代码示范。
import tornado.ioloop
import tornado.web
class MainHandler(tornado.web.RequestHandler):
    def get(self):
        self.write("Hello, world")
application = tornado.web.Application([
    (r"/", MainHandler),
])
if __name__ == "__main__":
    application.listen(8888)
    tornado.ioloop.IOLoop.instance().start()
实现非常简单, 只需要定义自己的处理方法, 其它的东西全部交给Tornado完成。首先创建web application, 并把我们的处理方法MainHandler传递过去。然后在8888开始监听。最后启动事件循环, 开始监听网络事件，主要是socket的读和写。Python又是这样一种便捷的语言，上面这段代码直接贴到文本中，无需编译，就可以直接运行，一个server就产生了。
## 2.3 模块分析
我们接下来将逐个分析这部分代码。首先对Tornado有个全面的了解。Tornado服务器有3大核心模块：
**(1) IOLoop**
从上面的代码可能看出，Tornado为了实现高并发和高性能, 使用了一个IOLoop来处理socket的读写事件, IOLoop基于epoll, 可以高效的响应网络事件. 这是Tornado高效的保证. 
**(2) IOStream**
为了在处理请求的时候, 实现对socket的异步读写, Tornado实现了IOStream类, 用来处理socket的异步读写。
**(3) HTTPConnection**
这个类用来处理http的请求，包括读取http请求头，读取post过来的数据，调用用户自定义的处理方法，以及把响应数据写给客户端socket。
下面这幅图描述了tornado服务器的大体处理流程, 接下来我们将会详细分析每一步流程的实现。
![](http://hi.csdn.net/attachment/201112/6/0_1323177517W1ll.gif)
3 源码分析
## **3.1 bind和listen**
**服务器的第一步就是bind。Httpserver.py的bind函数可以看到一个标准的服务器启动过程：**
def bind(self, port, address=None, family=socket.AF_UNSPEC):
        if address == "":
            address = None
                   // 查找网卡信息
        for res in socket.getaddrinfo(address, port, family, socket.SOCK_STREAM,
                                      0, socket.AI_PASSIVE | socket.AI_ADDRCONFIG):
            af, socktype, proto, canonname, sockaddr = res
            sock = socket.socket(af, socktype, proto)
            flags = fcntl.fcntl(sock.fileno(), fcntl.F_GETFD)
            flags |= fcntl.FD_CLOEXEC
            fcntl.fcntl(sock.fileno(), fcntl.F_SETFD, flags)
            sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            if af == socket.AF_INET6:
                if hasattr(socket, "IPPROTO_IPV6"):
                    sock.setsockopt(socket.IPPROTO_IPV6, socket.IPV6_V6ONLY, 1)
            sock.setblocking(0)
                            // bind和listen
            sock.bind(sockaddr)
            sock.listen(128)
            self._sockets[sock.fileno()] = sock
                   // 加入io_loop
            if self._started:
                self.io_loop.add_handler(sock.fileno(), self._handle_events,
                                         ioloop.IOLoop.READ)
**for循环保证对每张网卡上的请求都得到监听。对于每个网卡，先建立socket，然后bind listen，最后将socket加入到**io_loop，注册的事件是ioloop.IOLoop.READ，也就是读事件。程序中还添加了对ipv6的处理。回调函数为_handle_events, 一旦listen socket可读, 说明客户端请求到来, 然后调用_handle_events接受客户端的请求。接下来，看一下_handle_events是怎么处理的。
## 3.2 accept
接上一节，Httpserver.py的_handle_events函数实现了accept的过程。代码如下：
    def _handle_events(self, fd, events):
        while True:
            try:
**connection, address = self._sockets[fd].accept()**
            except socket.error, e:
                if e.args[0] in (errno.EWOULDBLOCK, errno.EAGAIN):
                    return
                raise
            if self.ssl_options is not None:
                                   //这里有一段处理ssl的代码，比较长，省略
            try:
                stream = iostream.**IOStream**(connection, io_loop=self.io_loop)
**HTTPConnection**(stream, address, self.request_callback,
                               self.no_keep_alive, self.xheaders)
            except:
                logging.error("Error in connection callback", exc_info=True)
accept方法返回客户端的socket, 以及客户端的地址。然后创建IOStream对象, 用来处理socket的异步读写. 这一步会调用ioloop.add_handler把client socket加入ioloop，再然后创建HTTPConnection, 处理用户的请求。接下来，我们看下iostream和httpconnection。
## 3.3 iostream
为了实现对client socket的异步读写, 需要为client socket创建两个缓冲区: _read_buffer和_write_buffer，这样我们就不用直接读写socket，进而实现异步读写。这些操作都封装在IOStream类中。概括来说，IOStream对socket的读写做了一层封装，通过使用两个缓冲区，实现对socket的异步读写。
IOStream与socket是一一对应的, 在iosteram.py可以找到iosteram的init方法：
    def __init__(self, socket, io_loop=None, max_buffer_size=104857600,
                 read_chunk_size=4096):
        self.socket = socket
        self.socket.setblocking(False)
        self.io_loop = io_loop or ioloop.IOLoop.instance()
**        self._read_buffer = collections.deque()**
**        self._write_buffer = collections.deque()**
        self._state = self.io_loop.ERROR
        with stack_context.NullContext():
**self.io_loop.add_handler**(
                self.socket.fileno(), self._handle_events, self._state)
可以看到，初始化的时候建立了两个buffer，然后把自己的socket放到了io_loop。这样，当这个socket有读写的时候，就会回调到注册的事件self._handle_events里面了。_handle_events就很容易理解了，代码如下：
    def _handle_events(self, fd, events):
        if not self.socket:
            logging.warning("Got events for closed stream %d", fd)
            return
        try:
            if events & self.io_loop.READ:
**self._handle_read()**
            if events & self.io_loop.WRITE:
** self._handle_write()**
            if events & self.io_loop.ERROR:
                self.io_loop.add_callback(self.close)
                return
            state = self.io_loop.ERROR
            if self.reading():
                state |= self.io_loop.READ
            if self.writing():
                state |= self.io_loop.WRITE
            if state != self._state:
                self._state = state
                self.io_loop.update_handler(self.socket.fileno(), self._state)
        except:
            logging.error("Uncaught exception, closing connection.",
                          exc_info=True)
            self.close()
            raise
## 3.4 ioloop
在Tornado服务器中，IOLoop是调度的核心模块，Tornado服务器回把所有的socket描述符都注册到IOLoop， 注册的时候指明回调处理函数，IOLoop内部不断的监听IO事件， 一旦发现某个socket可读写， 就调用其注册时指定的回调函数。 IOLoop使用了单例模式。
在Tornado运行的整个过程中，只有一个IOLoop实例，仅需一个 IOLoop实例, 就可以处理全部的IO事件。上文中多次用到了ioloop.IOLoop.instance()这个方法。它会返回ioloop的一个单例。
下面这段代码，可以看到python是怎么定义一个单例的。代码中使用了cls，这不是一个关键字，和self一样，cls是python的一个built-in变量，self表示类的实例，而cls表示类。所以大家看了几个函数会发现，python的成员函数的第一个参数，不是self就是cls。
class IOLoop(object):
    def **instance**(**cls**):
        if not hasattr(**cls**, "_instance"):
            cls._instance = **cls**()
        return cls._instance
    def initialized(cls):
        return hasattr(**cls**, "_instance")
def start(self):
        if self._stopped:
            self._stopped = False
            return
        self._running = True
        while True:
            poll_timeout = 0.2
            callbacks = self._callbacks
            self._callbacks = []
            for callback in callbacks:
                self._run_callback(callback)
            try:
**event_pairs = self._impl.poll(poll_timeout)**
            except Exception, e:
                if (getattr(e, 'errno', None) == errno.EINTR or
                    (isinstance(getattr(e, 'args', None), tuple) and
                     len(e.args) == 2 and e.args[0] == errno.EINTR)):
                    continue
                else:
                    raise
            if self._blocking_signal_threshold is not None:
                signal.setitimer(signal.ITIMER_REAL,
                                 self._blocking_signal_threshold, 0)
            self._events.update(event_pairs)
            while self._events:
                fd, events = self._events.popitem()
**self._handlers[fd](fd, events)**
        self._stopped = False
        if self._blocking_signal_threshold is not None:
            signal.setitimer(signal.ITIMER_REAL, 0, 0)
这里的poll根据不同的系统环境，支持select、epoll和KQueue三种模式。下面是epoll模式的处理：
class _EPoll(object):
    _EPOLL_CTL_ADD = 1
    _EPOLL_CTL_DEL = 2
    _EPOLL_CTL_MOD = 3
    def __init__(self):
        self._epoll_fd = epoll.epoll_create()
    def fileno(self):
        return self._epoll_fd
    def register(self, fd, events):
       epoll.**epoll_ctl**(self._epoll_fd, self._EPOLL_CTL_ADD, fd, events)
    def modify(self, fd, events):
        epoll.**epoll_ctl**(self._epoll_fd, self._EPOLL_CTL_MOD, fd, events)
    def unregister(self, fd):
        epoll.**epoll_ctl**(self._epoll_fd, self._EPOLL_CTL_DEL, fd, 0)
    def poll(self, timeout):
        return epoll.**epoll_wait**(self._epoll_fd, int(timeout * 1000))
# 4 性能比较
这是一段官网上的描述：
“一个 Web 应用的性能表现，主要看它的整体架构，而不仅仅是前端的表现。和其它的 Python Web 框架相比，Tornado 的速度要快很多。我们在一些流行的 Python Web 框架上（[Django](http://www.djangoproject.com/)、[web.py](http://webpy.org/)、[CherryPy](http://www.cherrypy.org/)），针对最简单的
 Hello, world 例子作了一个测试。对于 Django 和 web.py，我们使用 Apache/mod_wsgi 的方式来带，CherryPy 就让它自己裸跑。这也是在生产环境中各框架常用的部署方案。对于我们的 Tornado，使用的部署方案为前端使用[nginx](http://nginx.net/) 做反向代理，带动
 4 个线程模式的 Tornado，这种方案也是我们推荐的在生产环境下的 Tornado 部署方案（根据具体的硬件情况，我们推荐一个 CPU 核对应一个 Tornado 伺服实例，我们的负载测试使用的是四核处理器）。我们使用 Apache Benchmark (ab)，在另外一台机器上使用了如下指令进行负载测试：
ab -n 100000 -c 25 [http://10.0.1.x/](http://10.0.1.x/)
在 AMD Opteron 2.4GHz 的四核机器上，结果如下图所示：
![](http://hi.csdn.net/attachment/201112/6/0_1323177526f94U.gif)
在我们的测试当中，相较于第二快的服务器，Tornado 在数据上的表现也是它的 4 倍之多。即使只用了一个 CPU 核的裸跑模式，Tornado 也有 33% 的优势。”
使用同样的参数，对旺旺灰度发布服务器测试结果如下：
ab -n 20000 -c 50 'http://10.20.147.160:8080/redirect?uid=cnalichntest&ver=6.05.10&ctx=alitalk&site=cnalichn'
配置nginx + 1个tornado服务器的时候：Requests per second:    672.55 [#/sec] (mean)
配置nginx + 4个tornado服务器的时候：Requests per second:    2187.45 [#/sec] (mean)

