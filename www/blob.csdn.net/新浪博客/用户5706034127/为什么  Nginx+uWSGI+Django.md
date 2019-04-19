# 为什么  Nginx+uWSGI+Django_用户5706034127_新浪博客
||分类：[web](http://blog.sina.com.cn/s/articlelist_5706034127_7_1.html)|
转载：[http://www.cnblogs.com/Xjng/p/aa4dd23918359c6414d54e4b972e9081.html](http://www.cnblogs.com/Xjng/p/aa4dd23918359c6414d54e4b972e9081.html)
[http://www.python88.com/topic/101/](http://www.python88.com/topic/101/)
nginx是反向代理服务器，支持负载均衡，静态处理，安全防止攻击，提升io稳定性，
uwsgi是将http协议解析，解析成符合WSGI的规范，也可以控制多线程，比django自带的要高效。将生成动态网页的代码单独抽离出来，
接着就是web框架，包括，路由，数据库，orm，提供整个的mvc架构。如Django 和flask
Django 自带WSGI协议处理，可以处理完整web请求，不过效率低，仅用于测试。
所以需要uwsgi+django，他们两个也是可以处理完整web。
但是为了安全稳定等因素，加上nginx+uwsgi+django
Python的Web开发中，如果使用Django框架，那么较为成熟稳定的服务器架构一般是Nginx+uWSGI+Django。而为什么一定要三个结合在一起呢？直接使用Django的runserver来启动服务器进程，或者uWSGI+Django可不可以呢？为什么？ 
**概念说明：**
- 
APP（应用程序），就是开发者写的应用程序，例如django，bottle这些。记录怎么处理客户端发来的请求的逻辑部分。
- 
WSGI，是一个协议，Python用于Web开发的协议
- 
uWSGI，是一个程序，充当Web服务器或中间件。
- 
如果架构是Nginx+uWSGI+APP，uWSGI是一个中间件
- 
如果架构是uWSGI+APP，uWSGI是一个服务器
- 
uwsgi，是uWSGI程序实现的一个自有的协议。
Web协议出现顺序： 
CGI -> FCGI -> WSGI -> uwsgi
- 
CGI，最早的协议
- 
FCGI，比CGI快
- 
WSGI，Python专用的协议
- 
uwsgi，比FCGI和WSGI都快，是uWSGI项目自有的协议，主要特征是采用二进制来存储数据，之前的协议都是使用字符串，所以在存储空间和解析速度上，都优于字符串型协议.[官方介绍](http://uwsgi-docs.readthedocs.io/en/latest/FAQ.html#what-about-the-protocol)
### 一、WSGI协议
浏览器请求一个页面的流程：
- 
浏览器发送请求给服务器，包含请求头和请求体
- 
服务器解析请求头和请求体
- 
服务器根据请求信息来处理请求，生成返回内容
- 
服务器生成响应头和响应体
- 
服务器返回响应给浏览器，浏览器显示给用户
一个网站，一般有很多个不同的请求，在这些请求中，基本1，2，4，5部都是固定的，变的只有第三步，所以把这四步抽象出来，让开发者只关注第三步，这样就可以极大提升开发效率。所以WSGI协议诞生了。 
WSGI，全称 Web Server Gateway
Interface。是Python专用的协议，其他语言没有。用于处理Web服务器和应用程序（APP）的交互信息。很多Web框架（如：django）都会自带WSGI服务器，但是性能不好，只作测试用途。
实现一个最简单的服务器
- 
app.py
- 
```
import pprint
```
- 
```
def application(environ, start_response):
```
- 
```
pprint.pprint(environ)
```
- 
```
start_response('200
OK',[('Content-Type','text/html')])
```
- 
```
return'
```
# ```Hello, web!```
'
- 
environ参数是一个字典对象，保存HTTP请求的信息。例如URL路径，域名，请求头，请求参数等
- 
start_response参数是一个函数，用于向wsgiref提供响应头的设置，只能调用一次。
- 
server.py
- 
```
# 从wsgiref模块导入:
```
- 
```
from wsgiref.simple_server import make_server
```
- 
```
# 导入我们自己编写的application函数:
```
- 
```
from app import application
```
- 
```
# 创建一个服务器，IP地址为空，端口是8000，处理函数是application:
```
- 
```
httpd = make_server('',8000, application)
```
- 
```
print"Serving
HTTP on port 8000..."
```
- 
```
# 开始监听HTTP请求:
```
- 
```
httpd.serve_forever()
```
- 
启动
```
python
server.py
```
,就可以通过localhost:8000访问了
wsgiref模块是python提供的，用于测试和学习的简单的WSGI服务器模块。 
这个模块监听8000端口，把Http请求，根据WSGI协议，转换application函数中的environ参数，然后调用application函数。 
wsgiref会把application函数提供的响应头设置转换为HTTP协议的响应头，把application的返回（return）作为响应体，根据HTTP协议，生成响应，返回给浏览器。
![Alt text](http://7xpt1q.com1.z0.glb.clouddn.com/img/doc/WSGI%E5%8E%9F%E7%90%86.png)
这样，应用程序就不需要关注底层的HTTP协议细则了
### 二、CGI和FastCGI
CGI是Common Gateway
Interface，即通用网关接口，是一个协议，是外部应用程序（CGI程序）与Web服务器之间的接口标准。该协议定义了Web服务器在调用应用程序时需要传输的参数和应用程序怎么返回结果给Web服务器，其实跟WSGI类似。 
CGI的一个特点是，对于每一个HTTP请求，Web服务器都会新建一个进程（fork），等应用程序返回结果后，这个进程就会结束。这样的后果是，一旦HTTP请求多的时候，Web服务器会频繁创建进程，大家都知道，创建进程的开销是非常大的，所以这种做法会影响服务器的性能，所以就有了FastCGI。 
FCGI的做法是在Web服务器启动的时候，就创建多个应用程序进程，当Web服务器接收到HTTP请求时，就把请求分发给其中一个空闲的进程。相当于MYSQL连接池的原理。这样就可以避免频繁地fork进程。FCGI另一个特点是支持分布式，也就是Web服务器和应用程序可以在不同的机器。 
**CGI和WSGI的区别是**：
- 
CGI的出现更加早，这个是通用的接口，应用程序可以是JAVA，Python，等多种语言程序
- 
WSGI是Python专用的，在CGI的基础上改进的协议
## 三、Nginx
Ningx是一个反向代理服务器 
什么是反向代理？
- 
正向代理，例如FQ用的代理服务器就是正向代理，浏览器主动请求代理服务器，代理服务器转发请求到对应的目标服务器
- 
反向代理，部署在Web服务器上，代理所有外部网络对内部网络的访问。浏览器访问服务器，必须经过这个代理，是被动的。 
正向代理的主动方是客户端，反向代理的主动方是Web服务器。 
结构图： 
![Alt text](http://7xpt1q.com1.z0.glb.clouddn.com/img/doc/%E5%8F%8D%E5%90%91%E4%BB%A3%E7%90%86%E5%8E%9F%E7%90%86.png)
反向代理的作用：
- 
安全，客户端对Web服务器的访问需要先经过反向代理服务器。这样可以防止外部程序对Web服务器的直接攻击。
- 
负载均衡，反向代理服务器可以根据Web服务器的负载情况，动态地把HTTP请求交给不同的Web服务器来处理，前提是要有多个Web服务器。
- 
提升Web服务器的IO性能。一个HTTP请求的数据，从客户端传输给服务器，是需要时间的，例如N秒，如果直接传给Web服务器，Web服务器就需要让一个进程阻塞N秒，来接收IO，这样会降低Web服务器的性能。如果使用反向代理服务器，先让反向代理服务器接收完整个HTTP请求，再把请求发给Web服务器，就能提升Web服务器的性能。还有一些静态文件的请求，可以直接交给反向代理来处理，不需要经过Web服务器。
Nginx是一个高性能的HTTP和反向代理服务器。
Nginx+uWSGI+应用程序的架构： 
![Alt text](http://7xpt1q.com1.z0.glb.clouddn.com/img/doc/Nginx+uWSGI%E5%8E%9F%E7%90%86.png)
其中Nginx和uWSGI之间可以通过CGI，FCGI和uwsgi协议通信，当然uwsgi的性能是最好的。
### 四、总结
- 
uWSGI+Django比单独使用Django的好处：
- 
支持的并发量更高
- 
方便管理多进程，发挥多核的优势
- 
提升性能，因为uwsgi协议比WSGI协议有优势
- 
Nginx+uWSGI+Django比uWSGI+Django好处（参考反向代理的作用）：
最后附上一个介绍Nginx+uWSGI+Django的[幻灯片](http://brianmcdonnell.github.io/pycon_ie_2013/)
