# HTTP 协议 - Alex_McAvoy的博客 - CSDN博客





2018年08月17日 09:59:10[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：98
个人分类：[计算机网络](https://blog.csdn.net/u011815404/article/category/7944401)









# 【概述】

HTTP（Hypertext Transfer Protocol）超文本传输协议，用于从万维网（WWW：World Wide Web）服务器传输超文本到本地浏览器的传送协议。

其是一个基于 “请求与响应” 模式的、无状态的应用层协议，简单来说，即：用户发出请求，服务器给予响应的模式。

![](https://img-blog.csdn.net/20180816203137368?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

其中，无状态指的是第一次与第二次请求之间没有联系，应用层协议指的是建立在 TCP/IP 通信协议之上。

其具体实现为：浏览器作为 HTTP 客户端通过 URL 向 HTTP 服务端即 WEB 服务器发送请求，Web 服务器根据接收到的请求后，向客户端发送响应信息。

# 【URI 与 URL 及 URN】

## 1.URI

URI（uniform resource identifier），统一资源标识符，用来唯一的标识一个资源。

Web 上可用的每种资源，如：HTML 文档、图像、视频片段、程序等，都是一个来 URI 来定位。

URI 一般由三部分组成：访问资源的命名机制、存放资源的主机名、资源自身的名称(由路径表示，着重强调于资源)。

## 2.URL

URL（uniform resource locator），统一资源定位器，它是一种具体的 URI，其标识了互联网上某一处资源的地址，常用于传输数据和建立连接，即：通过 HTTP 协议存取资源的 Internet 路径，且一个 URL 对应一个数据资源。

URL 是 Internet 上用来描述信息资源的字符串，主要用在各种 WWW 客户程序和服务器程序上。

采用 URL 可以用一种统一的格式来描述各种信息资源，包括文件、服务器的地址和目录等。

URL 一般由三部分组成：协议(服务方式)、存有该资源的主机IP地址(有时包括端口号)、主机资源的具体地址。

## 3.URN

URN（uniform resource name），统一资源命名，是通过名字来标识资源，比如：mailto:java-net@java.sun.com

URI 是以一种抽象的，高层次概念定义统一资源标识，而 URL 和 URN 则是具体的资源标识的方式，笼统地说，每个 URL 都是 URI，但不一定每个 URI 都是 URL，这是因为 URI 还包括一个子类，即：统一资源名称 (URN)，它命名资源但不指定如何定位资源。

一个 URI 实例可以代表绝对的，也可以是相对的，只要它符合 URI 的语法规则即可，但 URL 类不仅要符合语义，还要包含定位该资源的信息，因此它不能是相对的。

URI 类不包含任何访问资源的方法，它唯一的作用就是解析，相反的是，URL 类可以打开一个到达资源的流。

## 4.URL 具体格式

### 1）格式

其格式为：**http://host[:port][path]**

1.**协议部分**：“ http: ”，代表网页使用的是 HTTP 协议，其后的 “ // ” 为分隔符

**注：在 Internet 中可使用多种协议，如：HTTP、FTP 等**

2.**合法的主机域名或 IP 地址**：“ host ”

3.**端口部分**：以 “ : ” 为分割符，紧跟在域名后面的 “ [port] ”，端口不是一个 URL 必须的部分，如果省略端口部分，将采用默认端口 80

4.**请求资源路径**：“ [path] ”

从域名后的第一个 “ / ” 开始到最后一个 “ / ” 为止，是**虚拟目录部分**；

从域名最后一个 “ / ” 开始到 “ ? ” 为止，是**文件名部分**，如果没有 “ ? ” 则是到  “ # ” 为止，如果 “ # ” 也没有，那么从最后一个 “ / ” 开始到结束，都是文件名部分；

从  “ ？ ”开始到 “ # ” 为止，中间的部分为**参数部分**，又称**搜索/查询部分**，参数允许有多个参数，之间以 “ & ” 为分隔符；

从  “ # ” 开始到最后，都是**锚部分**。

### 2）实例

以：http://www.aspxfans.com:8080/news/index.asp?boardID=5&ID=24618&page=1#name 为例

“ http: ” 代表使用协议是 HTTP 协议，“ www.aspxfans.com ” 是主机域名，“ :8080 ” 代表端口是8080，“ news ” 是请求资源路径的虚拟目录部分，“ index.asp ” 是文件名部分，“ boardID=5&ID=24618&page=1 ”是查询部分，其中有三个变量，分别是：“ boardID=5 ”、“ ID=24618 ”、“ page=1 ”，“ name ” 是锚部分。

# 【请求消息(Request)】

## 1.格式

客户端发送一个 HTTP 请求到服务器的请求消息包括以下格式：请求行(request line)、请求头部(header)、空行、请求数据。

格式如下：

![](https://img-blog.csdn.net/20180816211139906?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

1.**请求行**：用来说明请求类型、要访问的资源以及所使用的 HTTP 版本。

例如：GET 说明请求类型为要访问的资源，最后一部分说明使用的是 HTTP1.1 版本。
`GET /562f25980001b1b106000338.jpg HTTP/1.1`
2.**请求头部**：请求行(第一行)之后的部分，用于说明服务器要使用的附加信息。

3.**空行**：请求头部后面的空行是必须的，即使第四部分的请求数据为空，也必须有空行。

4.**请求数据**：也叫主体，可以添加任意的其他数据。

## 2.实例

POST 请求，使用 Charles 抓取的 request

```
POST / HTTP1.1
Host:www.wrox.com
User-Agent:Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727; .NET CLR 3.0.04506.648; .NET CLR 3.5.21022)
Content-Type:application/x-www-form-urlencoded
Content-Length:40
Connection: Keep-Alive

name=Professional%20Ajax&publisher=Wiley
```

第一部分：请求行，第一行

第二部分：请求头部，第二行至第六行

第三部分：空行，第七行的空行

第四部分：请求数据，第八行

# 【响应消息(Response)】

服务器接收并处理客户端发过来的请求后，会返回一个 HTTP 的响应消息。

## 1.格式

HTTP 响应消息也由四个部分组成，分别是：状态行、消息报头、空行和响应正文。

![](https://img-blog.csdn.net/20180816212159335?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

1.**状态行**：由 HTTP 协议版本号、 状态码、 状态消息三部分组成

例如：HTTP/1.1 表明 HTTP 版本为 1.1，状态码为 200，状态消息为 OK
`HTTP/1.1 200 OK`
2.**消息报头**：用来说明客户端要使用的一些附加信息

例如：Date 表明生成响应的日期和时间，Content-Type 指定了 MIME 类型的 HTML(text/html)，charset 指明了编码类型是 ISO-8859-1

```
Date: Sat, 31 Dec 2005 23:59:59 GMT
Content-Type: text/html;charset=ISO-8859-1
Content-Length:122
```

3.**空行**：消息报头后面的空行是必须的。

4.**响应正文**：服务器返回给客户端的文本信息，空行后面的部分即为响应正文。

## 2.实例

```
HTTP/1.1 200 OK
Date: Fri, 22 May 2009 06:07:21 GMT
Content-Type: text/html; charset=UTF-8

<html>
      <head></head>
      <body>
            <!--body goes here-->
      </body>
</html>
```

第一部分：状态行，第一行

第二部分：消息报头，第二行至第三行

第三部分：空行，第四行的空行

第四部分：响应正文，第五行至第十行

# 【HTTP 状态码】

状态代码由三位数字组成，其第一个数字定义了响应的类别，共分五种类别：
- 1xx：指示信息--表示请求已接收，继续处理
- 2xx：成功--表示请求已被成功接收、理解、接受
- 3xx：重定向--要完成请求必须进行更进一步的操作
- 4xx：客户端错误--请求有语法错误或请求无法实现
- 5xx：服务器端错误--服务器未能实现合法的请求

常见的状态码有：
|200 OK|客户端请求成功|
|----|----|
|400 Bad Request|客户端请求有语法错误，不能被服务器所理解|
|401 Unauthorized|请求未经授权，这个状态代码必须和 WWW-Authenticate 报头域一起使用|
|403 Forbidden|服务器收到请求，但是拒绝提供服务|
|404 Not Found|请求资源不存在，例如：输入了错误的 URL|
|500 Internal Server Error|服务器发生不可预期的错误|
|503 Server Unavailable|服务器当前不能处理客户端的请求，一段时间后可能恢复正常|

更多状态码：[点击这里](http://www.runoob.com/http/http-status-codes.html)

# 【HTTP 请求方法】

根据 HTTP 标准，HTTP 请求可以使用多种请求方法，以下是六个常用的方法：
|GET|请求获取 URL 位置的资源|
|----|----|
|HEAD|请求获取 URL 位置资源的响应消息报告，即获得该资源的头部信息|
|POST|请求向 URL 位置的资源后附加新的数据|
|PUT|请求向 URL 位置存储一个资源，覆盖原URL位置的资源|
|PATCH|请求局部更新 URL 位置的资源，即改变该处资源的部分内容。与 PUT 相比，其节省了网络带宽|
|DELETE|请求删除 URL 位置存储的资源|

![](https://img-blog.csdn.net/20180816215132338?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 【HTTP 工作原理】

HTTP 协议定义了 Web 客户端如何从 Web 服务器请求 Web 页面，以及服务器如何把 Web 页面传送给客户端。

HTTP 协议采用了请求/响应模型，客户端向服务器发送一个请求消息，请求消息包含请求的方法、URL、协议版本、请求头部和请求数据，服务器以一个状态行作为响应，响应的内容包括协议的版本、成功或者错误代码、服务器信息、响应头部和响应数据。

## 1.过程

**1）客户端连接到 Web 服务器**

一个HTTP客户端(通常是浏览器)，与 Web 服务器的 HTTP 端口(默认为80)建立一个 TCP 套接字连接。

例如：http://www.oakcms.cn

**2）发送 HTTP 请求**

通过 TCP 套接字，客户端向 Web 服务器发送一个文本的请求报文。

一个请求消息由请求行、请求头部、空行和请求数据4部分组成。

**3）服务器接受请求并返回 HTTP 响应**

Web 服务器解析请求、定位请求资源，服务器将资源复本写到 TCP 套接字，由客户端读取。

一个响应由状态行、响应头部、空行和响应数据4部分组成。

**4）释放连接 TCP 连接**

若 connection 模式为 close，则服务器主动关闭 TCP 连接，客户端被动关闭连接，释放 TCP 连接

若 connection 模式为 keepalive，则该连接会保持一段时间，在该时间内可以继续接收请求

**5）客户端浏览器解析HTML内容**

客户端浏览器首先解析状态行，查看表明请求是否成功的状态代码。

然后解析每一个响应头，响应头告知以下为若干字节的 HTML 文档和文档的字符集。

客户端浏览器读取响应数据 HTML，根据 HTML 的语法对其进行格式化，并在浏览器窗口中显示。

## 2.实例

在浏览器地址栏键入 URL，按下回车之后会经历以下流程：

1）浏览器向 DNS 服务器请求解析该 URL 中的域名所对应的 IP 地址

2）解析出 IP 地址后，根据该 IP 地址和端口，和服务器建立 TCP 连接

3）浏览器发出读取文件(URL 中域名后面部分对应的文件)的HTTP 请求，该请求报文作为 TCP 三次握手的第三个报文的数据发送给服务器

4）服务器对浏览器请求作出响应，并把对应的 HTML 文本发送给浏览器

5）释放 TCP连接

6）浏览器将该 HTML 文本并显示内容



