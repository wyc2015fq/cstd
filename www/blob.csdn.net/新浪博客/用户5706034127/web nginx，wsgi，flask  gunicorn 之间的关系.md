# web nginx，wsgi，flask  gunicorn 之间的关系_用户5706034127_新浪博客
||分类：[web](http://blog.sina.com.cn/s/articlelist_5706034127_7_1.html)|
			知识点：nginx，wsgi，flask  gunicorn
Web 服务器层
对于传统的客户端 - 服务器架构，客户端向服务器发送请求，服务器接收请求，处理请求，最后给客户端返回请求的响应，即简单来说，一个服务器的作用是：接收请求，处理请求，返回响应，负载均衡，拦截静态请求，依赖于nginx强大的功能和性能，可以做访问控制，限速，限连接数等
此文我们探讨[web服务器](https://www.baidu.com/s?wd=web%E6%9C%8D%E5%8A%A1%E5%99%A8&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1d-PHFbmWm4nyPWujfdnj60IAYqnWm3PW64rj0d0AP8IA3qPjfsn1bkrjKxmLKz0ZNzUjdCIZwsrBtEXh9GuA7EQhF9pywdQhPEUiqkIyN1IA-EUBtznHcsnH01PWb4P161PjmLnWmL)，常见的[web服务器](https://www.baidu.com/s?wd=web%E6%9C%8D%E5%8A%A1%E5%99%A8&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1d-PHFbmWm4nyPWujfdnj60IAYqnWm3PW64rj0d0AP8IA3qPjfsn1bkrjKxmLKz0ZNzUjdCIZwsrBtEXh9GuA7EQhF9pywdQhPEUiqkIyN1IA-EUBtznHcsnH01PWb4P161PjmLnWmL)有Nginx，Apache等。在这个[三层结构](https://www.baidu.com/s?wd=%E4%B8%89%E5%B1%82%E7%BB%93%E6%9E%84&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1d-PHFbmWm4nyPWujfdnj60IAYqnWm3PW64rj0d0AP8IA3qPjfsn1bkrjKxmLKz0ZNzUjdCIZwsrBtEXh9GuA7EQhF9pywdQhPEUiqkIyN1IA-EUBtznHcsnH01PWb4P161PjmLnWmL)中，web 服务器是最先接收到用户的请求的。
Web框架层
常见的web框架有Flask，Django等，我们以Flask框架为例子，展示web框架的作用：
from flask import Flask
app = Flask(__name__)
@app.route('/')
def hello_world():
    return 'Hello World!'
if __name__ == '__main__':
    app.run()
以上简单的几行代码，就创建了一个[web应用程序](https://www.baidu.com/s?wd=web%E5%BA%94%E7%94%A8%E7%A8%8B%E5%BA%8F&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1d-PHFbmWm4nyPWujfdnj60IAYqnWm3PW64rj0d0AP8IA3qPjfsn1bkrjKxmLKz0ZNzUjdCIZwsrBtEXh9GuA7EQhF9pywdQhPEUiqkIyN1IA-EUBtznHcsnH01PWb4P161PjmLnWmL)对象app。 
Web框架为我们提供路由管理，cookies管理等功能，让我们只需要关心业务的逻辑，提高开发的效率。
WSGI层
WSGI不是服务器，也不是用于与程序交互的API，更新不真实的代码，而只是一种接口。它只适用于Python语言，其全称为Web Server Gateway Interface，定义了[web服务器](https://www.baidu.com/s?wd=web%E6%9C%8D%E5%8A%A1%E5%99%A8&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1d-PHFbmWm4nyPWujfdnj60IAYqnWm3PW64rj0d0AP8IA3qPjfsn1bkrjKxmLKz0ZNzUjdCIZwsrBtEXh9GuA7EQhF9pywdQhPEUiqkIyN1IA-EUBtznHcsnH01PWb4P161PjmLnWmL)和web应用之间的接口规范。也就是说，只要web服务器和web应用都遵守WSGI协议，那么web服务器和web应用就可以随意的组合。
def application(env, start_response):    start_response('200 OK', [('Content-Type', 'text/html')])return [b"Hello World"]代码中的env，start_response变量由web服务器传入，其中，env是一个字典，包含了类似HTTP_HOST，HOST_USER_AGENT，SERVER_PROTOCO等[环境变量](https://www.baidu.com/s?wd=%E7%8E%AF%E5%A2%83%E5%8F%98%E9%87%8F&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1d-PHFbmWm4nyPWujfdnj60IAYqnWm3PW64rj0d0AP8IA3qPjfsn1bkrjKxmLKz0ZNzUjdCIZwsrBtEXh9GuA7EQhF9pywdQhPEUiqkIyN1IA-EUBtznHcsnH01PWb4P161PjmLnWmL)。start_response则是方法，接受两个参数，分别是status，response_headers。application方法设置http响应的状态码和Content-Type等头部信息，并返回响应具体结果。上述代码就是一个完整的WSGI应用，当一个支持WSGI的web服务器（例如Nginx）接收到客户端的请求后，便会调用这个application方法。WSGI层并不需要关心env，start_response这两个变量是如何实现的，直接使用即可。值得指出的是，WSGI是一种协议，需要区分几个相近的名词： 
- uwsgi 
同wsgi一样也是一种协议，uWSGI服务器正是使用了uwsgi协议 
- uWSGI 
实现了uwsgi和WSGI两种协议的web服务器。注意uWSGI本质上也是一种web服务器，处于上面描述的[三层结构](https://www.baidu.com/s?wd=%E4%B8%89%E5%B1%82%E7%BB%93%E6%9E%84&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1d-PHFbmWm4nyPWujfdnj60IAYqnWm3PW64rj0d0AP8IA3qPjfsn1bkrjKxmLKz0ZNzUjdCIZwsrBtEXh9GuA7EQhF9pywdQhPEUiqkIyN1IA-EUBtznHcsnH01PWb4P161PjmLnWmL)中的web服务器层。 
- CGI 
通用网关接口，并不限于python语言，定义了web服务器是如何向客户端提供动态的内容。例如，规定了客户端如何将[参数传递](https://www.baidu.com/s?wd=%E5%8F%82%E6%95%B0%E4%BC%A0%E9%80%92&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1d-PHFbmWm4nyPWujfdnj60IAYqnWm3PW64rj0d0AP8IA3qPjfsn1bkrjKxmLKz0ZNzUjdCIZwsrBtEXh9GuA7EQhF9pywdQhPEUiqkIyN1IA-EUBtznHcsnH01PWb4P161PjmLnWmL)给web服务器，web服务器如何将[参数传递](https://www.baidu.com/s?wd=%E5%8F%82%E6%95%B0%E4%BC%A0%E9%80%92&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1d-PHFbmWm4nyPWujfdnj60IAYqnWm3PW64rj0d0AP8IA3qPjfsn1bkrjKxmLKz0ZNzUjdCIZwsrBtEXh9GuA7EQhF9pywdQhPEUiqkIyN1IA-EUBtznHcsnH01PWb4P161PjmLnWmL)给web应用，web应用如何将它的输出如何发送给客户端，等等。 
生产环境下的web应用都不使用CGI了，CGI进程（类似Python解释器）针对每个请求创建，用完就抛弃，效率低下。WSGI正是为了替代CGI而出现的。
说到这，我们基本理清了WSGI在web服务器与web框架之间作用：WSGI就像一条纽带，将web服务器与web框架连接起来。回到本文的题目，Nginx属于一种web服务器，Flask属于一种web框架，因此，WSGI与Nginx、Flask的作用就不明而喻了。
不套Nginx，Gunicorn 也可以做到外网访问，那为什么还要套呢？是一样的道理。
Nginx 是专业的服务器，性能更好，更专业，并发更高，可以做负载均衡，可以做静态文件缓存，还可以限制 ip
访问的频率等等。
Gunicorn 一般用来管理多个进程，有进程挂了Gunicorn可以把它拉起来，防止服务器长时间停止服务，还可以动态调整
worker 的数量，请求多的时候增加 worker 的数量，请求少的时候减少，这就是所谓的 pre-fork
模型，这貌似就是它的主要优点。（worker
貌似就是进程，不是很确定，因为我们在使用的过程中没有发现进程数量有变化。。。）
也可以用 supervisor 来启动 Gunicorn，这样即使Gunicorn 的管理进程挂了也可以自动起来。
总结一下我们需要用到的服务有：
- 
Nginx：高性能Web服务器+负责反向代理；
- 
gunicorn：高性能WSGI服务器；
- 
gevent：把Python同步代码变成异步协程的库；
- 
Supervisor：监控服务进程的工具；
- 
MySQL：数据库服务。
在Linux服务器上用apt可以直接安装上述服务：
区别web服务器和，高性能WSGI服务器。一个针对web应用，一个是针对python
web开发。
符合WSGI接口，就可以在上面部署同样符合接口的python
web框架，以及关联的应用。
为什么不选择一个现成的Web框架而是自己从头开发呢？我们来考察一下现有的流行的Web框架：
**Django**：一站式开发框架，但不利于定制化；
**web.py**：使用类而不是更简单的函数来处理URL，并且URL映射是单独配置的；
**Flask**：使用@decorator的URL路由不错，但框架对应用程序的代码入侵太强；
**bottle**：缺少根据URL模式进行拦截的功能，不利于做权限检查。
我们先编写`hello.py`，实现Web应用程序的WSGI处理函数：
```
# hello.py
def application(environ, start_response):
    start_response('200 OK', [('Content-Type', 'text/html')])
    return '
```
然后，再编写一个`server.py`，负责启动WSGI服务器，加载`application()`函数：
```
# server.py
# 从wsgiref模块导入:
from wsgiref.simple_server import make_server
# 导入我们自己编写的application函数:
from hello import application
# 创建一个服务器，IP地址为空，端口是8000，处理函数是application:
httpd = make_server('', 8000, application)
print "Serving HTTP on port 8000..."
# 开始监听HTTP请求:
httpd.serve_forever()
```
# Hello, web!
'
from wsgiref.simple_server import make_server
httpd = make_server('', 8000, application)
httpd.serve_forever()
可以看到这三段代码，完成的是建立监听本地8000端口，并且负责解析http协议，
将其内容封装并调用application函数，完成了简单web服务器的功能。后面应该添加web框架。
