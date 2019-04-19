# HTTP协议 - 左直拳的马桶_日用桶 - CSDN博客
2014年10月23日 23:47:27[左直拳](https://me.csdn.net/leftfist)阅读数：1455
个人分类：[计算机基础](https://blog.csdn.net/leftfist/article/category/2622721)
某从出道到现在，一十又五年矣。期间一直捣鼓开发，其中又以WEB开发、BS结构程序居多，开发语言、工具换了一套又一套，但技术却没有什么进展。究其原因，是因为懒惰和愚笨，忙于搞应用，却忽视原理和基础的探究，所以忙忙碌碌下来，除了开发出一堆如过眼云烟的垃圾而外，什么也没剩下。比如说，号称搞WEB开发这么久，HTTP协议却从没认真学习一下，实在是说不过去，简直死有余辜。哪怕是抽个把小时也好啊！
孔子说：练武不练功，到老一场空！秃驴说：孔子说得对！
HTTP协议包括 URL 和 消息两部分；而消息包括请求和响应两部分：
1、URL
2、消息
2.1 请求
2.2 响应
HTTP消息基本格式如下：
--------------------------------------------------------------------------------------------------------------------------------------------
开始行（请求行 | 响应行 ）
消息报头（普通报头 | 请求报头 | 响应报头 | 实体报头）（可选）
CRLF
消息正文（可选）
---------------------------------------------------------------------------------------------------------------------------------------------
1、URL
URL是一种特殊类型的URI，包含了用于查找某个资源的足够的信息。格式如下：
http://host[":"port][abs_path]
http表示要通过HTTP协议来定位网络资源；host表示合法的Internet主机域名或者IP地址；port指定一个端口号，为空则使用缺省端口80；abs_path指定请求资源的URI；如果URL中没有给出abs_path，那么当它作为请求URI时，必须以“/”的形式给出，通常这个工作浏览器自动帮我们完成。
eg:
1）输入：www.guet.edu.cn
浏览器自动转换成：http://www.guet.edu.cn/
2）http:192.168.0.116:8080/index.jsp 
URI 与 URL 的区别：
URI属于URL更低层次的抽象，一种字符串文本标准。
就是说，URI属于父类，而URL属于URI的子类。URL是URI的一个子集。
在《HTTP权威指南》一书中，对于URI的定义是：统一资源标识符；对于URL的定义是：统一资源定位符。
二者的区别在于，URI表示请求服务器的路径，定义这么一个资源。而URL同时说明要如何访问这个资源（http://，使用http或https协议）。
2、请求
客户端向服务器端请求。
http请求由3部分组成
1）请求行
2）消息报头
3）请求正文
**1）请求行**
格式如下：
Method    Request-URI      HTTP-Version    CRLF  （回车换行符）
请求方法  请求地址  HTTP版本 CRLF
**eg:**
GET  /form.html  HTTP/1.1  CRLF
其中请求方法有多种，比如get,post,等等
**2)消息报头**
见下面的消息报头
**3）请求正文**
可选，略
3、响应
服务器端回应客户端。在接收和解释来自客户端的请求消息后，服务器返回一个HTTP响应消息。
HTTP响应也是由三个部分组成，分别是：
1）状态行
2）消息报头
3）响应正文
**1）状态行**
格式如下：
HTTP-Version Status-Code Reason-Phrase CRLF
HTTP版本  状态码 状态说明 CRLF
其中状态码由3位数字组成，分别以1、2、3、4、5开头：
1**：消息
2**：成功
3**：重定向
4**：请求错了，比如，地址不存在
5**：服务器故障
**2）消息报头**
见下面的消息报头
**3）响应正文**
可选，略
4、消息报头
消息报头包括：
1）普通报头
2）请求报头
3）响应报头
4）实体报头
每个报头都由若干个报头域组成。 报头域格式 = 名字 + “：” + 空格 + 值 。报头域的名字是大小写无关的。
**1）普通报头**
请求和响应都可以使用这种报头。该报头的报头域不是很多，只用于传输消息。如：
Cache-Control 缓存
Date 消息产生日期和时间
Connection 连接选项
**2）请求报头**
请求报头允许客户端向服务器端传递请求的附加信息以及客户端自身的信息。
**[plain]**[view plain](http://blog.csdn.net/leftfist/article/details/40406089#)[copy](http://blog.csdn.net/leftfist/article/details/40406089#)
- GET /form.html HTTP/1.1 (CRLF)  
- Accept:image/gif,image/x-xbitmap,image/jpeg,application/x-shockwave-flash,application/vnd.ms-excel,application/vnd.ms-powerpoint,application/msword,*/* (CRLF)  
- Accept-Language:zh-cn (CRLF)  
- Accept-Encoding:gzip,deflate (CRLF)  
- If-Modified-Since:Wed,05 Jan 2007 11:21:25 GMT (CRLF)  
- If-None-Match:W/"80b1a4c018f3c41:8317" (CRLF)  
- User-Agent:Mozilla/4.0(compatible;MSIE6.0;Windows NT 5.0) (CRLF)  
- Host:www.guet.edu.cn (CRLF)  
- Connection:Keep-Alive (CRLF)  
- (CRLF)  
**3）响应报头**
响应报头允许服务器传递不能放在状态行中的附加响应信息，以及关于服务器的信息和对Request-URI所标识的资源进行下一步访问的信息。
常用的响应报头
Location：响应报头域用于重定向接受者到一个新的位置。Location响应报头域常用在更换域名的时候。
Server：包含了服务器用来处理请求的软件信息。与User-Agent请求报头域是相对应的。
**4）实体报头**
请求和响应消息都可以传送一个实体。
一个实体由实体报头域和实体正文组成，但并不是说实体报头域和实体正文要在一起发送，可以只发送实体报头域的。
实体报头定义了关于实体正文和请求所标识的资源的元信息。
常用的实体报头：
Content-Encoding
被用作媒体类型的修饰符，它的值指示了已经被应用到实体正文的附加内容的编码，因而要获得Content-Type报头域中所引用的媒体类型，必须采用相应的解码机制。eg：Content-Encoding：gzip
Content-Language
描述了资源所用的自然语言。没有设置该域则认为实体内容将提供给所有的语言阅读者。eg：Content-Language:da
Content-Length
用于指明实体正文的长度，以字节方式存储的十进制数字来表示。
Content-Type
用于指明发送给接收者的实体正文的媒体类型。eg：
Content-Type:text/html;charset=ISO-8859-1
Content-Type:text/html;charset=GB2312
Last-Modified
用于指示资源的最后修改日期和时间。
Expires
给出响应过期的日期和时间。
