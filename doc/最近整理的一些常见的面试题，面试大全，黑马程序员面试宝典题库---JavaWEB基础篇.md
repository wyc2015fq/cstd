# 最近整理的一些常见的面试题，面试大全，黑马程序员面试宝典题库---JavaWEB基础篇

2018年09月03日 15:32:30 [高数老师](https://me.csdn.net/qq_29073921) 阅读数：175



 版权声明：代码共享，欢迎copy	https://blog.csdn.net/qq_29073921/article/details/82348956

## 一、 JDBC 技术（评论留言获取原件）

### 1. 说下原生 jdbc 操作数据库流程？（2017-11-25-wzz）

第一步： Class.forName()加载数据库连接驱动；
第二步： DriverManager.getConnection()获取数据连接对象;
第三步：根据 SQL 获取 sql 会话对象，有 2 种方式 Statement、 PreparedStatement ;
第四步：执行 SQL 处理结果集，执行 SQL 前如果有参数值就设置参数值 setXXX();
第五步：关闭结果集、关闭会话、关闭连接。
详细代码请看（封装）： <https://blog.csdn.net/qq_29542611/article/details/52426006>

## 2.什么要使用 PreparedStatement？

1、 PreparedStatement 接口继承 Statement， PreparedStatement 实例包含已编译的 SQL 语句，所以其执行速度要快于 Statement 对象。
2 、 作 为 Statement 的 子 类 ， PreparedStatement 继 承 了 Statement 的 所 有 功 能 。 三 种 方法 execute、 executeQuery 和 executeUpdate 已被更改以使之不再需要参数
3、在 JDBC 应用中,在任何时候都不要使用 Statement，原因如下：
一、代码的可读性和可维护性.Statement 需要不断地拼接，而 PreparedStatement 不会。
二、 PreparedStatement 尽最大可能提高性能.DB 有缓存机制，相同的预编译语句再次被调用不会再次需要编译。
三、最重要的一点是极大地提高了安全性.Statement 容易被 SQL 注入，而 PreparedStatementc 传入的内容不会和 sql 语句发生任何匹配关系。

### 3. 关系数据库中连接池的机制是什么？


前提：为数据库连接建立一个缓冲池。
1：从连接池获取或创建可用连接
2：使用完毕之后，把连接返回给连接池
3：在系统关闭前，断开所有连接并释放连接占用的系统资源
4：能够处理无效连接，限制连接池中的连接总数不低于或者不超过某个限定值。
其中有几个概念需要大家理解：
最小连接数是连接池一直保持的数据连接。如果应用程序对数据库连接的使用量不大，将会有大量的数据库连接源被浪费掉。
最大连接数是连接池能申请的最大连接数。如果数据连接请求超过此数，后面的数据连接请求将被加入到等待队列中，这会影响之后的数据库操作。
如果最小连接数与最大连接数相差太大，那么，最先的连接请求将会获利，之后超过最小连接数量的连接请求等价于建立一个新的数据库连接。不过，这些大于最小连接数的数据库连接在使用完不会马上被释放，它将被放到连接池中等待重复使用或是空闲超时后被释放。
上面的解释，可以这样理解：数据库池连接数量一直保持一个不少于最小连接数的数量，当数量不够时，数据库会创建一些连接，直到一个最大连接数，之后连接数据库就会等待。

##  三、 Http 协议

### 4. http 的长连接和短连接


HTTP 协议有 HTTP/1.0 版本和 HTTP/1.1 版本。 HTTP1.1 默认保持长连接（HTTP persistent connection，也翻译为持久连接），数据传输完成了保持 TCP 连接不断开（不发 RST 包、不四次握手），等待在同域名下继续用这个通道传输数据；相反的就是短连接。
在 HTTP/1.0 中，默认使用的是短连接。也就是说，浏览器和服务器每进行一次 HTTP 操作，就建立一次连接，任务结束就中断连接。 从 HTTP/1.1 起，默认使用的是长连接， 用以保持连接特性。

### 5. HTTP/1.1 与 HTTP/1.0 的区别


参考原文： <https://blog.csdn.net/forgotaboutgirl/article/details/6936982>

### 6.http 常见的状态码有哪些？

200 OK //客户端请求成功
301 Moved Permanently（永久移除)，请求的 URL 已移走。 Response 中应该包含一个 Location URL, 说明资
源现在所处的位置
302 found 重定向
400 Bad Request //客户端请求有语法错误，不能被服务器所理解
401 Unauthorized //请求未经授权，这个状态代码必须和 WWW-Authenticate 报头域一起使用
403 Forbidden //服务器收到请求，但是拒绝提供服务
404 Not Found //请求资源不存在， eg：输入了错误的 URL
500 Internal Server Error //服务器发生不可预期的错误
503 Server Unavailable //服务器当前不能处理客户端的请求，一段时间后可能恢复正常

###  7. GET 和 POST 的区别？

从表面现像上面看 GET 和 POST 的区别：
\1. GET 请求的数据会附在 URL 之后（就是把数据放置在 HTTP 协议头中），以?分割 URL 和传输数据，参数之间以&相连，如： login.action?name=zhagnsan&password=123456。 POST 把提交的数据则放置在是 HTTP 包的包体中。
\2. GET 方式提交的数据最多只能是 1024 字节，理论上 POST 没有限制，可传较大量的数据。其实这样说是错误的，不准确的：“GET 方式提交的数据最多只能是 1024 字节"，因为 GET 是通过 URL 提交数据，那么 GET 可提交的数据量就跟URL 的长度有直接关系了。而实际上， URL 不存在参数上限的问题， HTTP 协议规范没有对 URL 长度进行限制。这个限制是特定的浏览器及服务器对它的限制。IE对URL长度的限制是2083 字节(2K+35)。对于其他浏览器，如 Netscape、FireFox 等，理论上没有长度限制，其限制取决于操作系统的支持。
3.POST 的安全性要比 GET 的安全性高。注意：这里所说的安全性和上面 GET 提到的“安全”不是同个概念。上面“安全”的含义仅仅是不作数据修改，而这里安全的含义是真正的 Security 的含义，比如：通过 GET 提交数据，用户名和密码将明文出现在 URL 上，因为(1)登录页面有可能被浏览器缓存， (2)其他人查看浏览器的历史纪录，那么别人就可以拿到你的账号和密码了，除此之外，使用 GET 提交数据还可能会造成 Cross-site request forgery 攻击。Get 是向服务器发索取数据的一种请求，而 Post 是向服务器提交数据的一种请求，在 FORM（表单）中， Method默认为"GET"，实质上， GET 和 POST 只是发送机制不同，并不是一个取一个发！
参考原文： <https://www.cnblogs.com/hyddd/archive/2009/03/31/1426026.html>

###  8.http 中重定向和请求转发的区别？

本质区别：转发是服务器行为，重定向是客户端行为。
重定向特点：两次请求，浏览器地址发生变化，可以访问自己 web 之外的资源，传输的数据会丢失。
请求转发特点：一次强求，浏览器地址不变，访问的是自己本身的 web 资源，传输的数据不会丢失。

##  四、 Cookie 和 Session

### 9. Cookie 和 Session 的区别

​        Cookie 是 web 服务器发送给浏览器的一块信息，浏览器会在本地一个文件中给每个 web 服务器存储 cookie。以后浏览器再给特定的 web 服务器发送请求时，同时会发送所有为该服务器存储的 cookie。
​        Session 是存储在 web 服务器端的一块信息。 session 对象存储特定用户会话所需的属性及配置信息。当用户在应用程序的 Web 页之间跳转时，存储在 Session 对象中的变量将不会丢失，而是在整个用户会话中一直存在下去。
Cookie 和 session 的不同点：
1、无论客户端做怎样的设置， session 都能够正常工作。当客户端禁用 cookie 时将无法使用 cookie。
2、在存储的数据量方面： session 能够存储任意的 java 对象， cookie 只能存储 String 类型的对象。

### 10.session 共享怎么做的（分布式如何实现 session 共享）？


参考原文： <https://blog.csdn.net/sxiaobei/article/details/57086489>

### 11.在单点登录中，如果 cookie 被禁用了怎么办？

单点登录的原理是后端生成一个 session ID，然后设置到 cookie，后面的所有请求浏览器都会带上 cookie，然后服务端从 cookie 里获取 session ID，再查询到用户信息。所以，保持登录的关键不是 cookie，而是通过cookie 保存和传输的 session ID，其本质是能获取用户信息的数据。除了 cookie，还通常使用 HTTP 请求头来传输。但是这个请求头浏览器不会像 cookie 一样自动携带，需要手工处理。
 

## 五、 jsp 技术

### 12. 什么是 jsp，什么是 Servlet？ jsp 和 Servlet 有什么区别？

jsp 本质上就是一个 Servlet，它是 Servlet 的一种特殊形式（由 SUN 公司推出），每个 jsp 页面都是一个 servlet实例。Servlet 是由 Java 提供用于开发 web 服务器应用程序的一个组件，运行在服务端，由 servlet 容器管理，用来生成动态内容。一个 servlet 实例是实现了特殊接口 Servlet 的 Java 类，所有自定义的 servlet 均必须实现 Servlet 接口。
区别：
jsp 是 html 页面中内嵌的 Java 代码，侧重页面显示；
Servlet 是 html 代码和 Java 代码分离，侧重逻辑控制， mvc 设计思想中 jsp 位于视图层， servlet 位于控制层Jsp 运行机制：如下图:

![img](https://img-blog.csdn.net/20180903152540950?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzI5MDczOTIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

JVM 只能识别 Java 类，并不能识别 jsp 代码！ web 容器收到以.jsp 为扩展名的 url 请求时，会将访问请求交给tomcat 中 jsp 引擎处理，每个 jsp 页面第一次被访问时， jsp 引擎将 jsp 代码解释为一个 servlet 源程序，接着编译servlet 源程序生成.class 文件，再有 web 容器 servlet 引擎去装载执行 servlet 程序，实现页面交互。
 

### 13.jsp 有哪些域对象和内置对象及他们的作用？

四大域对象：
（1） pageContext page 域-指当前页面，在当前 jsp 页面有效，跳到其它页面失效
（2） request request 域-指一次请求范围内有效，从 http 请求到服务器处理结束，返回响应的整个过程。在这个过程中使用 forward（请求转发）方式跳转多个 jsp，在这些页面里你都可以使用这个变量
（3） session session 域-指当前会话有效范围，浏览器从打开到关闭过程中，转发、重定向均可以使用

（4） application context 域-指只能在同一个 web 中使用，服务器未关闭或者重启，数据就有效
九大内置对象：
![img](https://img-blog.csdn.net/20180903152915319?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzI5MDczOTIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

###   


六、 XML 技术

14.什么是 xml，使用 xml 的优缺点， xml 的解析器有哪几种，分别有什么区别？

xml 是一种可扩展性标记语言，支持自定义标签（使用前必须预定义）使用 DTD 和 XML Schema 标准化 XML 结构。
具体了解 xml 详见： <http://www.importnew.com/10839.html>