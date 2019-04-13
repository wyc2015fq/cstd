
# http协议 - 李昆鹏的博客 - CSDN博客


2018年07月12日 11:00:25[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：47标签：[tomcat																](https://so.csdn.net/so/search/s.do?q=tomcat&t=blog)[http																](https://so.csdn.net/so/search/s.do?q=http&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=tomcat&t=blog)个人分类：[Tomcat																](https://blog.csdn.net/weixin_41547486/article/category/7796475)


----------------------http协议-----------------------------
# 1. HTTP协议
![](https://img-blog.csdn.net/20180712104556398?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 1　什么是HTTP协议
HTTP，即超文本传输协议。所有浏览器都可以发出http协议。
协议：就是规定的文本格式！例如写本书也要有格式，写总结也有格式。当然客户端发送请求也要有格式，这个格式是已经约定的格式，所以服务器端按请求格式来获取请求信息，然后服务器端再按照响应格式来响应，当然客户端还要按响应的格式来解析。

## 2　无状态协议
HTTP是无状态协议，必须必须理解这一点！！！
所谓无状态，就是客户端发送一个请求，服务器返回一个响应，然后就断开连接了。
无状态协议的好处，当然是效率高了，用户连接一下就断开，当用户再次发送请求时，再连接，然后在响应之后再次断开。如果用户在服务器上找到了自己想看的小说，那么服务器只是把小说的内容（html）发送给用户的浏览器，然后用户的浏览器显示已经发送到本地的html，其实这时已经与服务器断开连接了。用户可能会看很久，但这不会占用服务器的任何资源！

## 3.HTTP1.0和HTTP1.1
HTTP现在有两个版本，HTTP1.0和HTTP1.1。
HTTP1.0：响应结束后会马上断开；
HTTP1.1：必须在请求中包含Host（主机）头信息，响应结束后不会马上断开，会有一个超时时间，可能会在超时之前完成多个请求/响应，当超时的时候就断开。
GET /web_demo/index.jsp HTTP/1.0
Host:localhost:8080
GET /hello/index.jsp HTTP/1.0
Host:localhost:8080
**开启telnet****功能第一种方式**
![](https://img-blog.csdn.net/20180712104620780?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**开启telnet****功能第二种方式**
![](https://img-blog.csdn.net/2018071210464919?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**响应http1.0请求方式：**
1.在dos命令窗口输入telnet localhost 端口号
![](https://img-blog.csdn.net/2018071210470367?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
2.然后复制
GET /hello/index.jsp HTTP/1.0
Host:localhost:8080
按回车
![](https://img-blog.csdn.net/20180712104712600?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
200代表请求成功
**结果会发现http1.0****在请求完之后会立马断开连接**
**响应http1.1请求方式：**
**输入以下命令**
GET /hello/index.jsp HTTP/1.1
Host:localhost:8080
![](https://img-blog.csdn.net/20180712104727525?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**会发现http1.1****在请求完之后并马上不会断开连接**
## 4　请求协议和响应协议
无论是请求协议还是响应协议，都是由如下部分构成的：
l  请求（或响应）首行；
l  请求（或响应）头信息；
l  空行；
l  请求（或响应）正文。

## 5. 请求协议
我们可以通过浏览器来监视请求和响应，获取完整的请求和响应信息。
l  Fire Fox：
l  Google：内置了这功能，也无需安装。
**请求**
GET/hello/index.jsp HTTP/1.1
![](https://img-blog.csdn.net/20180712104749449?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**响应**
![](https://img-blog.csdn.net/20180712104806654?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
|Request  URL = 请求网址
|Request  Method = 请求方法
|Status  Code = 状态码
|Remote  Address = 远程地址 [: : 1]代表本地
|请求头：
|Host  = 主机地址，请求的消息头
|User-Agent  = 包括浏览器、操作系统等信息
|Accept  = 请求或接收的格式
|Accept-Language  = 接收的语言
|Accept-Encoding  = 接收的编码
|Connection  = 连接状态
|Cache-Control  = 缓存管理
|响应头：
|Content-Length  = 内容的长度
|Content-Type  = 内容的类型
|Date  = 日期
|Server  = 服务器版本
|

### 1. GET请求
下面是通过FireFox获取到的请求信息，这就是浏览器发送给服务器的全部请求信息内容。请求的地址是：http://localhost/hello/index.jsp
|//|请求首行
|GET  /hello/index.jsp HTTP/1.1
|//|下面全部是请求头信息，因为GET|请求没有正文
|Host:  localhost
|User-Agent:  Mozilla/5.0 (Windows NT 5.1; rv:5.0) Gecko/20100101 Firefox/5.0
|Accept:  text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
|Accept-Language:  zh-cn,zh;q=0.5
|Accept-Encoding:  gzip, deflate
|Accept-Charset:  GB2312,utf-8;q=0.7,*;q=0.7
![](https://img-blog.csdn.net/20180712104936228?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
|//|空行，因为GET|没有正文，所以下面没有东西了
|
lGET /hello/index.jsp HTTP/1.1：GET请求，请求服务器路径为/hello/index.jsp，协议为1.1；
lHost:localhost：请求的主机名为localhost；
l  User-Agent: Mozilla/5.0 (**WindowsNT 5.1**; rv:5.0) Gecko/20100101**Firefox/5.0**：与浏览器和OS相关的信息。有些网站会显示用户的系统版本和浏览器版本信息，这都是通过获取User-Agent头信息而来的；
l  Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8：告诉服务器，当前客户端可以接收的文档类型，其实这里包含了*/*，就表示什么都可以接收；
l  Accept-Language: zh-cn,zh;q=0.5：当前客户端支持的语言，可以在浏览器的工具à选项中找到语言相关信息；
l  Accept-Encoding: gzip, deflate：支持的压缩格式。数据在网络上传递时，可能服务器会把数据压缩后再发送；
l  Connection: keep-alive：客户端支持的链接方式，保持一段时间链接，默认为3000ms；
l  Cookie: JSESSIONID=369766FDF6220F7803433C0B2DE36D98：因为不是第一次访问这个地址，所以会在请求中把上一次服务器响应中发送过来的Cookie在请求中一并发送去过；这个Cookie的名字为JSESSIONID，然后在讲会话是讲究它！
![](https://img-blog.csdn.net/20180712104959178?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**Get****请求没有正文，只有头信息，请求参数是在请求的首行的url****中传递的。请求的参数的容量是有限的，不能超过1024kb，（一般通过地址栏或者超链接访问）**
### 2　POST请求
![](https://img-blog.csdn.net/20180712105011699?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
下面是通过POST请求获取到的请求信息
|// 请求首行
|POST /hello/index.jsp HTTP/1.1
|Host: localhost
|User-Agent: Mozilla/5.0 (Windows NT 5.1; rv:5.0) Gecko/20100101 Firefox/5.0
|Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
|Accept-Language: zh-cn,zh;q=0.5
|Accept-Encoding: gzip, deflate
|Accept-Charset: GB2312,utf-8;q=0.7,*;q=0.7
|Connection: keep-alive
|Referer: http://localhost/hello/index.jsp
|Cookie: JSESSIONID=369766FDF6220F7803433C0B2DE36D98
|Content-Type: application/x-www-form-urlencoded
|Content-Length: 14
|// 这里是空行
|//POST有请求正文
|username=hello|
l  Content-Type: application/x-www-form-urlencoded：表单的数据类型，说明会使用url格式的UTF-8编码数据；url编码的数据都是以“%”为前缀，后面跟随两位的16进制；I Referer: http://localhost/hello/index.jsp：请求来自哪个页面。这个值说明请求是从index.jsp页面发送出去的！这个东东还是比较有用的！
如果这个表单的作用是下载，那么在给用户提供资源之前可以去判断请求是从哪个页面发送过来的，如果不是从本网站发送过来的，那么就可以拒绝下载，例如盗链！
l  Content-Length:14：表示请求数据的长度，这里表示14个字节。
l  username=hello：这是请求正文！hello是在表单中输入的数据，username是文本框的名字。

## 6.响应协议

###### 1　响应信息

|// 响应首行
|HTTP/1.1 200 OK
|// 响应头信息
|Server: Apache-Coyote/1.1
|Content-Type: text/html;charset=UTF-8
|Content-Length: 777
|Date: Sat, 09 Feb 2012 18:30:52 GMT
|// 空行，下面是响应正文，即HTML代码
|<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
|<html>
|<head>
|<base href="http://localhost:80/hello/">
|<title>My JSP 'index.jsp' starting page</title>
|<meta http-equiv="pragma" content="no-cache">
|<meta http-equiv="cache-control" content="no-cache">
|<meta http-equiv="expires" content="0">
|<meta http-equiv="keywords" content="keyword1,keyword2,keyword3">
|<meta http-equiv="description" content="This is my page">
|<!--
|<link rel="stylesheet" type="text/css" href="styles.css">
|-->
|</head>
|<body>
|This is my JSP page. <br>
|<form method="post" action="index.jsp">
|<input type="text" name="username" /><br/>
|<input type="submit" value="SUBMIT"/>
|</form>
|</body>
|</html>|
I HTTP/1.1 200 OK：响应协议为HTTP1.1，状态码为200，表示请求成功，OK是对状态码的解释；
I Server: Apache-Coyote/1.1：这是服务器的版本信息；
I Content-Type: text/html;charset=UTF-8：请求正文使用的编码为UTF-8；
l  Content-Length: 777：响应的内容为777字节；
l  Set-Cookie: JSESSIONID=C97E2B4C55553EAB46079A4F263435A4; Path=/hello：响应给客户端的Cookie；
l  Date: Sat, 09 Feb 2012 18:30:52 GMT：响应的时间，这可能会有8小时的时区差；
**常见状态码：**
l  404 not found：没有找到；
l  500 error：找到了，但内部出错；
l  200 ok：成功！
**请求：GET/POST**
只有表单才可能是POST，其他都是GET
POST有正文，即表单内容：username=zhangSan&password=123
GET没有正文，但是空行可是有的！
**请求首行**
(GET/POST) URL HTTP/1.1
响应首行
HTTP/1.1 状态码 状态码的解释
响应的正文就是HTML！
**Post****请求：有正文，而且正文内容不受限制，通过表单的方式提交（除了表单方式的提交时post****请求方式，其余的都是get请求）。**
![](https://img-blog.csdn.net/20180712105622895?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

