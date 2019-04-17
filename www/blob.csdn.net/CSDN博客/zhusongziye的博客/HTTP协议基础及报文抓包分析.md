# HTTP协议基础及报文抓包分析 - zhusongziye的博客 - CSDN博客





2018年04月06日 19:43:05[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：491








## HTTP基本架构

下面我们用一张简单的流程图来展示HTTP协议基本架构，以便大家先有个基本的了解。

![](https://img-blog.csdn.net/20180406192710566?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 
Web Client可以是浏览器、搜索引擎、机器人等等一切基于HTTP协议发起http请求的工具。

- 
Web Server可以是任何的能解析HTTP请求，并返回给Web Client可识别的响应的服务，常见的有apache、nginx、IIS等等web服务器。


浓缩就是精华，看下最简洁的HTTP交互图：

![](https://img-blog.csdn.net/20180406192750533?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




## HTTP报文结构
- 
请求报文


HTTP请求报文由请求行、请求头、空行和请求内容4个部分构成。

如下图所示：

![](https://img-blog.csdn.net/20180406192841176?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

下面对上图进行简单的分析：

请求行

由请求方法字段、URL字段、协议版本字段三部分构成，它们之间由空格隔开。常用的请求方法有：GET、POST、HEAD、PUT、DELETE、OPTIONS、TRACE、CONNECT。

请求头

请求头由key/value对组成，每行为一对，key和value之间通过冒号(:)分割。请求头的作用主要用于通知服务端有关于客户端的请求信息。

典型的请求头有：

User-Agent：生成请求的浏览器类型

Accept：客户端可识别的响应内容类型列表；星号`*`用于按范围将类型分组。`*/*`表示可接受全部类型，`type/*`表示可接受type类型的所有子类型。

Accept-Language: 客户端可接受的自然语言

Accept-Encoding: 客户端可接受的编码压缩格式

Accept-Charset： 可接受的字符集

Host: 请求的主机名，允许多个域名绑定同一IP地址

connection：连接方式（close或keeplive）

Cookie: 存储在客户端的扩展字段

空行

最后一个请求头之后就是空行，用于告诉服务端以下内容不再是请求头的内容了。

请求内容

请求内容主要用于POST请求，与POST请求方法配套的请求头一般有Content-Type（标识请求内容的类型）和Content-Length（标识请求内容的长度）
- 
响应报文


HTTP响应报文由状态行、响应头、空行和响应内容4个部分构成。

如下图所示：

![](https://img-blog.csdn.net/20180406194032665?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

下面对响应报文格式进行简要的分析说明：



状态行

由HTTP协议版本、状态码、状态码描述三部分构成，它们之间由空格隔开。

状态码由3位数字组成，第一位标识响应的类型，常用的5大类状态码如下：

1xx：表示服务器已接收了客户端的请求，客户端可以继续发送请求

2xx：表示服务器已成功接收到请求并进行处理

3xx：表示服务器要求客户端重定向

4xx：表示客户端的请求有非法内容

5xx：标识服务器未能正常处理客户端的请求而出现意外错误

常见状态码说明：

200 OK： 表示客户端请求成功

400 Bad Request： 表示客户端请求有语法错误，不能被服务器端解析

401 Unauthonzed： 表示请求未经授权，该状态码必须与WWW-Authenticate报文头一起使用

404 Not Found：请求的资源不存在，例如输入了错误的url

500 Internal Server Error： 表示服务器发生了不可预期的错误，导致无法完成客户端的请求

503 Service Unavailable：表示服务器当前不能处理客户端的请求，在一段时间后服务器可能恢复正常

响应头

一般情况下，响应头会包含以下，甚至更多的信息。

Location：服务器返回给客户端，用于重定向到新的位置

Server： 包含服务器用来处理请求的软件信息及版本信息

Vary：标识不可缓存的请求头列表

Connection: 连接方式。

对于请求端来讲：close是告诉服务端，断开连接，不用等待后续的求请了。keeplive则是告诉服务端，在完成本次请求的响应后，保持连接，等待本次连接后的后续请求。

对于响应端来讲：close表示连接已经关闭。keeplive则表示连接保持中，可以继续处理后续请求。Keep-Alive表示如果请求端保持连接，则该请求头部信息表明期望服务端保持连接多长时间（秒），例如300秒，应该这样写Keep-Alive: 300

空行

最后一个响应头之后就是空行，用于告诉请求端以下内容不再是响应头的内容了。

响应内容

服务端返回给请求端的文本信息。

## HTTP报文示例

在这里我们在Firefox下用firebug随意抓取一个HTTP包和上文的报文结构做下一一对应关系图，以便大家了解实际的包和标准报文结构的对应关系。

![](https://img-blog.csdn.net/20180406194124729?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 总结

对于HTTP协议的交互过程这里就不再进行说明了，大家可以搜索下相关的资料进行学习，上述的内容请务必熟练掌握、深刻了解。更详细的内容推荐大家学习RFC 2616（http协议1.1版本，有中文版本）



