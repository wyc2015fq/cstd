# HTTP协议详解 - HJ - CSDN博客
2018年03月03日 14:40:42[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：116
个人分类：[====Web开发====																[-----Web基础学习------																[HTTP与Tomcat](https://blog.csdn.net/feizaoSYUACM/article/category/7888074)](https://blog.csdn.net/feizaoSYUACM/article/category/7888048)](https://blog.csdn.net/feizaoSYUACM/article/category/7479655)
**HTTP协议详解**
**目录**
**1、HTTP主要特点**
**2、HTTP 主要原理**
**3、URL**
**4、HTTP请求方法**
**5、请求消息Request**
**6、响应消息Response**
**7、状态码**
**8、GET和POST请求的区别**
**1.HTTP主要特点**
HTTP协议是Hyper Text Transfer Protocol（超文本传输协议）的缩写,是用于从万维网服务器传输超文本到本地浏览器的传送协议。它基于TCP/IP通信协议来传递数据（HTML文件、图片、查询结果等）。
HTTP协议工作与客户端-服务端架构之上，浏览器作为HTTP客户端通过URL向HTTP服务器端即WEB服务器发送所欲请求，WEB服务器根据接收到的请求后，向客户端发送响应消息。
其主要特点如下：
（1）简单快速：客户向服务器发送请求服务时，只需传送请求方法和路径；
（2）灵活：HTTP允许传输任意类型的数据对象。
（3）无连接：限制每次连接只处理一个请求，节省传输时间；
（4）无状态：对于事务处理没有记忆能力；
（5）支持B/S以及C/S模式；
**2.HTTP主要原理**
HTTP协议定义Web客户端如何从Web服务器请求Web页面,以及服务器如何把Web页面传送给客户端。HTTP采用了请求/响应模型。客户端向服务器发送一个请求报文，请求报文包含请求的方法、URL、协议版本、请求头部和请求数据。服务器以一个状态行作为响应，响应的内容包括协议版本、成功或者错误代码、服务器信息、响应头部和响应数据。
HTTP请求/响应的步骤：
（1）客户端连接到Web服务器；
（2）发送HTTP请求；
（3）服务器接受请求并返回HTTP响应；
（4）释放TCP连接；
（5）客户端浏览器解析HTML内容；
**3.URL**
URI全称是Uniform Resource Identifiers（统一资源标识符），用来唯一的标识一个资源，一般由三部分组成：访问资源的命名机制、存放资源的主机名、资源自身的名称（由路径标识，着重强调于资源）；
URL全称是Uniform Resource Locator（统一资源定位符），它是一种具体的URI（URI包括URL和URN）,用来标识一个资源，而且还指明了如何locate这个资源；
采用URL可以用一种统一的格式来描述各种信息资源，包括文件、服务器的地址和目录等，一般由三部分组成：
①协议（或称为服务方式）；
②存有该资源的主机IP地址（有时也包括端口号）；
③主机资源的具体地址。如目录和文件名等；
以如下这个URL为例，介绍一下普通URL的各部分组成：
[http://www.aspxfans.com:8080/news/index.asp?boardID=5&ID=24618&page=1#name](#name)
一个完整的URL包括一下几个部分：
（1）协议部分：该URL的协议部分为“http：”，这代表网页使用的是HTTP协议。在Internet中可以使用多种协议，如FTP等。在“HTTP”后面的“//”为分隔符；
（2）域名部分：该URL的域名部分为“www.aspxfans.com”。一个URL中，也可以使用IP地址作为域名使用；
（3）端口部分：跟在域名后面的是端口，域名和端口之间使用“：”作为分隔符。端口不是一个URL必须的部分，如果省略端口部分，将采用默认端口；
（4）虚拟目录部分：从域名后的第一个“/”开始到最后一个“/”为止，是虚拟目录部分。虚拟目录也不是一个URL必须的部分。本例中的虚拟目录是“/news/”；
（5）文件名部分：从域名的最后一个“/”开始到“？”为止，是文件名部分，如果没有“？”，则是从域名的最后一个“/”开始到“#”为止，如果没有“？”和“#”，那么从域名后的最后一个“/”开始到结束，都是文件名部分。本例中的文件名是“index.asp”。文件名部分也不是一个URL必须的部分，如果省略该部分，则使用默认的文件名；
（6）锚部分：从“#”开始到最后，都是锚部分。锚部分也不是一个URL必须的部分，本例中的锚部分是“name”；
（7）参数部分：从“？”开始到“#”为止之间的部分为参数部分，又称搜索部分、查询部分。本例中的参数部分为“boardID=5&ID=24618&page=1”。参数可以允许有多个参数，参数与参数之间用“&”作为分隔符；
**4.HTTP请求方法**
HTTP1.0定义了三种请求方法： GET、POST和HEAD方法；
HTTP1.1新增了五种请求方法：OPTIONS、PUT、DELETE、TRACE和CONNECT方法；
但是主要使用GET和POST两种：
GET    请求指定的页面信息，并返回实体主体；
POST   向指定资源提交数据进行处理请求； 
5. **请求消息Request**
客户端发送一个HTTP请求到服务器的请求消息包括以下格式：
**请求行、请求头部、空行和请求数据四个部分组成；**
    请求行以一个方法符号开头，以空格分开，后面跟着请求的URI和协议的版本。
![](https://img-blog.csdn.net/20180303143840767)
GET请求例子
GET /562f25980001b1b106000338.jpg HTTP/1.1
Host    img.mukewang.com
User-Agent    Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/51.0.2704.106 Safari/537.36
Accept    image/webp,image/*,*/*;q=0.8
Referer    http://www.imooc.com/
Accept-Encoding    gzip, deflate, sdch
Accept-Language    zh-CN,zh;q=0.8
POST请求例子
POST / HTTP1.1
Host:www.wrox.com
User-Agent:Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727; .NET CLR 3.0.04506.648; .NET CLR 3.5.21022)
Content-Type:application/x-www-form-urlencoded
Content-Length:40
Connection: Keep-Alive
name=Professional%20Ajax&publisher=Wiley
第一部分：请求行，用来说明请求类型，要访问的资源以及所使用的HTTP版本；
第二部分：请求头部，紧接着请求行（即第一行）之后的部分，用来说明服务器要使用的信息；
第三部分：空行，请求头部后面的空行是必须的（即使第四部分的请求数据为空，也必须有空行）；
第四部分：请求数据，也叫主体，可以添加任意的其他数据（这个例子的请求数据为空）；
6. **响应消息Response**
服务器接收并处理客户端发过来的请求后返回的HTTP响应消息包括以下内容：
**状态行、消息报头、空行和响应正文四个部分组成；**
HTTP/1.1 200 OK
Date: Fri, 22 May 2009 06:07:21 GMT
Content-Type: text/html; charset=UTF-8
<html>
      <head></head>
      <body>
            <!--body goes here-->
      </body>
</html>
第一部分：请求行，由HTTP协议版本号，状态码，状态消息三部分组成；
第二部分：消息报头，用来说明客户端要使用的一些附加信息；
第三部分：空行，消息报头后面的空行是必须的（即使第四部分的请求数据为空，也必须有空行）；
第四部分：响应正文，服务器返回给客户端的文本信息（空行后面的HTML部分为响应正文）；
7. **状态码**
状态码由三位数字组成，第一个数字定义了响应的类别，总共分为五种类别：
**1xx    指示信息       表示请求已接收，继续处理**
**2xx    成功           表示请求已被成功接收、理解、接受**
**3xx    重定向         要完成请求必须进行更进一步的操作**
**4xx    客户端错误     请求有语法错误或请求无法实现**
**5xx    服务器端错误   服务器未能实现合法的请求**
常见状态码：
**200    OK             客户端请求成功**
**400    Bad Request    客户端请求有语法错误，不能被服务器所理解**
**401    Unauthorized   请求未经授权，这个状态码必须和WWW-Authenticate报头域一起使用**
**403    Forbidden      服务器收到请求，但是拒绝提供服务**
**404    Not Found      请求资源不存在，有可能是输入了错误的URL**
**500    Internal Server Error  服务器发生不可预期的错误**
**503    Server Unavailable  服务器当前不能处理客户端的请求，一段时间后可能恢复正常**
8. **GET和POST请求的区别**
（1）GET提交的数据会放在URL之后，以？分割URL和传输数据，参数之间以&相连；POST是把提交的数据放在HTTP包的Body中；
（2）GET提交的数据大小有限制（因为浏览器对URL的长度有限制），而POST提交的数据没有限制；
（3）GET需要使用Request.QueryString来取得变量的值，而POST通过Request.Form来获取变量的值；
（4）GET提交数据会带来安全问题，比如一个登陆页面，通过GET方式提交数据时，用户名和密码将出现在URL上，如果页面可以被缓存或者其他人可以访问这台机器，就可以从历史记录获得该用户的账号和密码；
