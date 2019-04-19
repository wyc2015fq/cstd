# 第二篇：白话tornado源码之待请求阶段 - 宇宙浪子的专栏 - CSDN博客
2017年09月08日 13:18:35[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：217标签：[python																[tornado](https://so.csdn.net/so/search/s.do?q=tornado&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[Python																[tornado](https://blog.csdn.net/bluehawksky/article/category/7102850)](https://blog.csdn.net/bluehawksky/article/category/2542577)

转自：[http://www.cnblogs.com/wupeiqi/p/4375610.html](http://www.cnblogs.com/wupeiqi/p/4375610.html)
上篇《[白话tornado源码之一个脚本引发的血案](http://www.cnblogs.com/wupeiqi/p/4375445.html)》用上帝视角多整个框架做了一个概述，同时也看清了web框架的的本质，下面我们从tornado程序的起始来分析其源码。
### 概述
![](http://images0.cnblogs.com/blog2015/425762/201506/171327191853514.png)
上图是tornado程序启动以及接收到客户端请求后的整个过程，对于整个过程可以分为两大部分：
- 启动程序阶段，又称为待请求阶段（上图1、2所有系列和3.0）
- 接收并处理客户端请求阶段（上图3系列）
简而言之：
> 
1、在启动程序阶段，第一步，获取配置文件然后生成url映射（即：一个url对应一个XXRequestHandler，从而让XXRequestHandler来处理指定url发送的请求）；第二步，创建服务器socket对象并添加到epoll中；第三步，创建无线循环去监听epoll。
2、在接收并处理请求阶段，第一步，接收客户端socket发送的请求（socket.accept）；第二步，从请求中获取请求头信息，再然后根据请求头中的请求url去匹配某个XXRequestHandler；第三步，匹配成功的XXRequestHandler处理请求；第四步，将处理后的请求发送给客户端；第五步，关闭客户端socket。
本篇的内容主要剖析【启动程序阶段】，下面我们就来一步一步的剖析整个过程，在此阶段主要是有下面重点标注的三个方法来实现。
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
import tornado.ioloop
import tornado.web
 
class MainHandler(tornado.web.RequestHandler):
    def get(self):
        self.write("Hello, world")
 
application = tornado.web.Application([
    (r"/index", MainHandler),
])
 
if __name__ == "__main__":
    application.listen(8888)
    tornado.ioloop.IOLoop.instance().start()
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
### 一、application = tornado.web.Application([(xxx,xxx)])
　　执行Application类的构造函数，并传入一个列表类型的参数，这个列表里保存的是url规则和对应的处理类，即：当客户端的请求url可以配置这个规则时，那么该请求就交由对应的Handler去执行。
注意：Handler泛指继承自RequestHandler的所有类
        Handlers泛指继承自RequestHandler的所有类的集合
```
Application.__init__
```
```
Application.add_handlers
```
```
URLSpec
```
上述代码主要完成了以下功能：加载配置信息和生成url映射，并且把所有的信息封装在一个application对象中。
加载的配置信息包括：
- 编码和返回方式信息
- 静态文件路径
- ui_modules（模版语言中使用，暂时忽略）
- ui_methods（模版语言中使用，暂时忽略）
- 是否debug模式运行
以上的所有配置信息，都可以在settings中配置，然后在创建Application对象时候，传入参数即可。如：application = tornado.web.Application([(r"/index",
 MainHandler),],**settings)
生成url映射：
- 将url和对应的Handler添加到对应的主机前缀中，如：safe.index.com、www.auto.com
![](http://images.cnitblog.com/blog2015/425762/201503/292228582243341.png)
 封装数据：
　　将配置信息和url映射关系封装到Application对象中，信息分别保存在Application对象的以下字段中：
- self.transforms，保存着编码和返回方式信息
- self.settings，保存着配置信息
- self.ui_modules，保存着ui_modules信息
- self.ui_methods，保存这ui_methods信息
- self.handlers，保存着所有的主机名对应的Handlers，每个handlers则是url正则对应的Handler
### 二、application.listen(xxx)
　　第一步操作将配置和url映射等信息封装到了application对象中，而这第二步执行application对象的listen方法，该方法内部又把之前包含各种信息的application对象封装到了一个HttpServer对象中，然后继续调用HttpServer对象的liseten方法。
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
class Application(object):
    #创建服务端socket，并绑定IP和端口并添加相应设置，注：未开始通过while监听accept，等待客户端连接
    def listen(self, port, address="", **kwargs):
        from tornado.httpserver import HTTPServer
        server = HTTPServer(self, **kwargs)
        server.listen(port, address) 
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
![](http://images.cnitblog.com/blog2015/425762/201504/032152146232494.png)
详细代码：
```
HTTPServer
```
```
IOLoop
```
```
stack_context.wrap
```
备注：stack_context.wrap其实就是对函数进行一下封装，即：函数在不同情况下上下文信息可能不同。
上述代码本质上就干了以下这么四件事：
- 把包含了各种配置信息的application对象封装到了HttpServer对象的request_callback字段中
- 创建了服务端socket对象
- 单例模式创建IOLoop对象，然后将socket对象句柄作为key，被封装了的函数_handle_events作为value，添加到IOLoop对象的_handlers字段中
- 向epoll中注册监听服务端socket对象的读可用事件
目前，我们只是看到上述代码大致干了这四件事，而其目的有什么？他们之间的联系又是什么呢？
答：现在不妨先来做一个猜想，待之后再在源码中确认验证是否正确！猜想：通过epoll监听服务端socket事件，一旦请求到达时，则执行3中被封装了的_handle_events函数，该函数又利用application中封装了的各种配置信息对客户端url来指定判定，然后指定对应的Handler处理该请求。
注意：使用epoll创建服务端socket
```
Code
```
上述，其实就是利用epoll对象的poll(timeout)方法去轮询已经注册在epoll中的socket句柄，当有读可用的信息时候，则返回包含当前句柄和Event Code的序列，然后在通过句柄对客户端的请求进行处理
### 三、tornado.ioloop.IOLoop.instance().start()
上一步中创建了socket对象并使得socket对象和epoll建立了关系，该步骤则就来执行epoll的epoll方法去轮询已经注册在epoll对象中的socket句柄，当有读可用信息时，则触发一些操作什么的....
```
```python
class
```
```python
IOLoop(
```
```python
object
```
```python
):
```
```python
```
```python
def
```
```python
add_handler(
```
```python
self
```
```python
,
 fd, handler, events):
```
```python
```
```python
#HttpServer的Start方法中会调用该方法
```
```python
```
```python
self
```
```python
._handlers[fd]
```
```python
=
```
```python
stack_context.wrap(handler)
```
```python
```
```python
self
```
```python
._impl.register(fd,
 events |
```
```python
self
```
```python
.ERROR)
```
```python
```
```python
```
```python
def
```
```python
start(
```
```python
self
```
```python
):
```
```python
```
```python
while
```
```python
True
```
```python
:
```
```python
```
```python
poll_timeout
```
```python
=
```
```python
0.2
```
```python
```
```python
try
```
```python
:
```
```python
```
```python
#epoll中轮询
```
```python
```
```python
event_pairs
```
```python
=
```
```python
self
```
```python
._impl.poll(poll_timeout)
```
```python
```
```python
except
```
```python
Exception,
 e:
```
```python
```
```python
#省略其他
```
```python
```
```python
#如果有读可用信息，则把该socket对象句柄和Event
 Code序列添加到self._events中
```
```python
```
```python
self
```
```python
._events.update(event_pairs)
```
```python
```
```python
#遍历self._events，处理每个请求
```
```python
```
```python
while
```
```python
self
```
```python
._events:
```
```python
```
```python
fd,
 events
```
```python
=
```
```python
self
```
```python
._events.popitem()
```
```python
```
```python
try
```
```python
:
```
```python
```
```python
#以socket为句柄为key，取出self._handlers中的stack_context.wrap(handler)，并执行
```
```python
```
```python
#stack_context.wrap(handler)包装了HTTPServer类的_handle_events函数的一个函数
```
```python
```
```python
#是在上一步中执行add_handler方法时候，添加到self._handlers中的数据。
```
```python
```
```python
self
```
```python
._handlers[fd](fd,
 events)
```
```python
```
```python
except
```
```python
:
```
```python
```
```python
#省略其他
```
```
```
View
 Code
```
对于上述代码，执行start方法后，程序就进入“死循环”，也就是会一直不停的轮询的去检查是否有请求到来，如果有请求到达，则执行封装了HttpServer类的_handle_events方法和相关上下文的stack_context.wrap(handler)（其实就是执行HttpServer类的_handle_events方法），详细见下篇博文，简要代码如下：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
class HTTPServer(object):
    def _handle_events(self, fd, events):
        while True:
            try:
                connection, address = self._socket.accept()
            except socket.error, e:
                if e.args[0] in (errno.EWOULDBLOCK, errno.EAGAIN):
                    return
                raise
            if self.ssl_options is not None:
                assert ssl, "Python 2.6+ and OpenSSL required for SSL"
                try:
                    connection = ssl.wrap_socket(connection,
                                                 server_side=True,
                                                 do_handshake_on_connect=False,
                                                 **self.ssl_options)
                except ssl.SSLError, err:
                    if err.args[0] == ssl.SSL_ERROR_EOF:
                        return connection.close()
                    else:
                        raise
                except socket.error, err:
                    if err.args[0] == errno.ECONNABORTED:
                        return connection.close()
                    else:
                        raise
            try:
                if self.ssl_options is not None:
                    stream = iostream.SSLIOStream(connection, io_loop=self.io_loop)
                else:
                    stream = iostream.IOStream(connection, io_loop=self.io_loop)
                HTTPConnection(stream, address, self.request_callback,
                               self.no_keep_alive, self.xheaders)
            except:
                logging.error("Error in connection callback", exc_info=True) 
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
### 结束
本篇博文介绍了“待请求阶段”的所作所为，简要来说其实就是三件事：其一、把setting中的各种配置以及url和Handler之间的映射关系封装到来application对象中（application对象又被封装到了HttpServer对象的request_callback字段中）；其二、结合epoll创建服务端socket；其三、当请求到达时交由HttpServer类的_handle_events方法处理请求，即：处理请求的入口。对于处理请求的详细，请参见下篇博客（客官莫急，加班编写中...）
