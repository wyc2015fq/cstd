# Tornado框架01-高性能简介

 

# 01-简介

Tornado全称Tornado Web Server，是一个用Python语言写成的Web服务器兼Web应用框架，由FriendFeed公司在自己的网站FriendFeed中使用，被Facebook收购以后框架在2009年9月以开源软件形式开放给大众。

特点：

- 作为Web框架，是一个轻量级的Web框架，其拥有异步非阻塞IO的处理方式。
- 作为Web服务器，Tornado有较为出色的抗负载能力，官方用nginx反向代理的方式部署Tornado和其它Python web应用框架进行对比，结果最大浏览量超过第二名近40%。

性能：

- Tornado有着优异的性能。它试图解决C10k问题，即处理大于或等于一万的并发。 
   Tornado框架和服务器一起组成一个WSGI的全栈替代品。单独在WSGI容器中使用tornado网络框架或者tornaod http服务器，有一定的局限性，为了最大化的利用tornado的性能，推荐同时使用tornaod的网络框架和HTTP服务器。

# 02-应用场景

## 用户量大，高并发

如秒杀抢购、双十一某宝购物、春节抢火车票

## 大量的HTTP持久连接

使用同一个TCP连接来发送和接收多个HTTP请求/应答，而不是为每一个新的请求/应答打开新的连接的方法。

对于HTTP 1.0，可以在请求的包头（Header）中添加Connection: Keep-Alive。

对于HTTP 1.1，所有的连接默认都是持久连接。

# 03-Tornado与Django

**Django**

Django是走大而全的方向，注重的是高效开发，它最出名的是其全自动化的管理后台：只需要使用起ORM，做简单的对象定义，它就能自动生成数据库结构、以及全功能的管理后台。

Django提供的方便，也意味着Django内置的ORM跟框架内的其他模块耦合程度高，应用程序必须使用Django内置的ORM，否则就不能享受到框架内提供的种种基于其ORM的便利。

- session功能
- 后台管理
- ORM

**Tornado**

Tornado走的是少而精的方向，注重的是性能优越，它最出名的是异步非阻塞的设计方式。

- HTTP服务器
- 异步编程
- WebSockets

# 04-简单的Tornado案例

```
import tornado.web
import tornado.ioloop

class IndexHandler(tornado.web.RequestHandler):
    """主路由处理类"""
    def get(self):
        """对应http的get请求方式"""
        self.write("Hello Python Tornado!")

if __name__ == "__main__":
    app = tornado.web.Application([
        (r"/", IndexHandler),
    ])
    app.listen(8000)
    tornado.ioloop.IOLoop.current().start()123456789101112131415
```

以上代码解释说明：

tornado的基础web框架模块

- RequestHandler  
   封装了对应一个请求的所有信息和方法，write(响应信息)就是写响应信息的一个方法；对应每一种http请求方式（get、post等），把对应的处理逻辑写进同名的成员方法中（如对应get请求方式，就将对应的处理逻辑写在get()方法中），当没有对应请求方式的成员方法时，会返回“405:  Method Not Allowed”错误。
- Application  
   Tornado Web框架的核心应用类，是与服务器对接的接口，里面保存了路由信息表，其初始化接收的第一个参数就是一个路由信息映射元组的列表；其listen(端口)方法用来创建一个http服务器实例，并绑定到给定端口（注意：此时服务器并未开启监听）

# 05-Tornado核心IOLoop循环模块

tornado的核心io循环模块，封装了Linux的epoll和BSD的kqueue，tornado高性能的基石。 以Linux的epoll为例，其原理如下图：

![image](https://img-blog.csdn.net/20170801190522372?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDc0NTE5NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) 
 再详细一点的图解释： 
 ![image](https://img-blog.csdn.net/20170801190533158?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDc0NTE5NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

- IOLoop.current() 返回当前线程的IOLoop实例。
- IOLoop.start() 启动IOLoop实例的I/O循环,同时服务器监听被打开。

# 06-Tornado Web程序编写思路

- 创建web应用实例对象，第一个初始化参数为路由映射列表。
- 定义实现路由映射列表中的handler类。
- 创建服务器实例，绑定服务器端口。
- 启动当前线程的IOLoop。