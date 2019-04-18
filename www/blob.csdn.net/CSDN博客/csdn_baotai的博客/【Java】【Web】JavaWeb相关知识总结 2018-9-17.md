# 【Java】【Web】JavaWeb相关知识总结  2018-9-17 - csdn_baotai的博客 - CSDN博客

2018年09月18日 23:14:44[csdn-[baotai]](https://me.csdn.net/csdn_baotai)阅读数：31
个人分类：[Java](https://blog.csdn.net/csdn_baotai/article/category/7659813)



# JavaWeb相关知识

## C/S和B/S的区别：

> - 
### C/S:Client/Server:客户端和服务端的架构.

该软件运行在电脑桌面:比如,QQ,FeiQ,POS机/火车站卖票等.
- 特点: 一般的,服务端仅仅是安装一个数据库和数据存储,所有程序的代码(功能/界面)都编写在客户端软件中(把所有的业务逻辑编写在客户端,界面渲染也在客户端).
- 优点:直观,用户体验好,界面美观,安全性高等.
- 缺点:每次升级,需要重新安装,移植性较低,开发和维护成本较高

> - 
### B/S:Browser/Server:浏览器和服务端的架构(通过浏览器访问的应用).

- 该软件通过浏览器访问,比如:门户网站,管理系统等.

其实B/S属于特殊的C/S,因为,浏览器就是一个特殊的客户端.
- 特点:在服务端编写代码,所有的业务逻辑都由服务端完成,客户端只负责界面渲染.
- 优点:可移植性很高,开发和维护较好.
- 缺点:用户体验不够好,界面不够丰富,安全性很低.

## HTTP协议 ：浏览器和服务器之间的数据交互格式

> 
概念：WEB浏览器与WEB服务器之间的一问一答的交互过程必须遵循一定的规则，就是HTTP协议。

HTTP是hypertext transfer protocol（超文本传输协议）的简写，它是TCP/IP协议之上的一个应用层协议，用于定义WEB浏览器与WEB服务器之间交换数据的过程以及数据本身的格式

## HTTP协议 版本问题：

> 
HTTP1.0规范:

若请求的有N个资源,得建立N次连接,发送N次请求,接收N次响应,关闭N次连接.

> 
HTTP1.1规范:

能在一次连接之间,多次请求,多次响应,响应完之后再关闭连接.

浏览器给服务器发送数据:一次请求

服务器给浏览器反馈数据:一次响应

![请求报头](https://img-blog.csdn.net/20180918231041870?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NzZG5fYmFvdGFp/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![响应报头](https://img-blog.csdn.net/20180918231103644?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NzZG5fYmFvdGFp/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 常见状态码

> 
200: 完全OK啦,正确.

404: 找到请求的资源(客户端的问题,资源名写错了)

500: 服务端代码有问题(后台的Java代码有问题).

## GET和POST请求的区别:

> 
#### GET - 从指定的资源请求数据。

#### POST - 向指定的资源提交要被处理的数据
- GET的请求数据在地址栏,而POST不会.

​      POST比GET更安全.
- POST请求的参数存放于请求实体中,而GET存放于请求行中.
- GET方式请求的数据不能超过2K,而POST没有上限.

​        比如文件上传时,必须使用POST方式.
- GET可以缓存,而POST没有缓存.

### 使用场景：

​         查询的时候使用使用GET，超链接用GET

​         其他时候使用POST.表单全部使用POST提交.

## Servlet容器

> 
Servlet容器为JavaWeb应用提供运行时环境，它负责管理Servlet和JSP 的生命周期，以及管理它们的共享数据。

Servlet容器也称为JavaWeb容器，或者Servlet/JSP容器—>Tomcat

## 端口问题：

> 
Tomcat的默认端口是8080

HTTP协议的默认端口是80

而我们在访问的时候,都是遵循于http协议的.

