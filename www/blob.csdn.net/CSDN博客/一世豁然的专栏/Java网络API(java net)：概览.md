# Java网络API(java.net)：概览 - 一世豁然的专栏 - CSDN博客





2017年07月13日 11:13:44[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1597








提供实现网络应用程序的类。

java.net包可以大致分为两部分：


1、低级API，涉及以下抽象：

 1）、地址，它们是网络标识符，如IP地址。

 2）、套接字是基本的双向数据通信机制。

 3）、接口，描述网络接口。




2、高级API，涉及以下抽象：

 1）、URI，表示通用资源标识符。

 2）、网址，代表普遍资源定位器。

 3）、连接，表示与URL指向的资源的连接。







一、地址

java.net API中的地址用作主机标识符或套接字端点标识符。





InetAddress类是表示IP（Internet协议）地址的抽象。 它有两个子类：


1、[Inet4Address](https://developer.android.google.cn/reference/java/net/Inet4Address.html)为IPv4地址

2、[Inet6Address](https://developer.android.google.cn/reference/java/net/Inet6Address.html)为IPv6地址




但是，在大多数情况下，不需要直接处理子类，因为InetAddress抽象应该涵盖大部分所需的功能。





一）、关于IPV6

并不是所有的系统都支持IPv6协议，当Java网络堆栈尝试检测它并在可用时透明地使用它，也可以使用系统属性禁用它。 在IPv6不可用或明确禁用的情况下，Inet6Address不再对大多数网络操作有效。 虽然getByName（String）等方法在查找主机名时不会返回Inet6Address，但是通过传递文字可以创建这样的对象。 在这种情况下，大多数方法在使用Inet6Address调用时都会抛出异常。








二、Sockets

套接字是通过网络在机器之间建立通信链路的手段。 java.net包提供了4种套接字：


1、Socket是一个TCP客户端API，通常用于连接到远程主机。

2、ServerSocket是一个TCP服务器API，通常会接受来自客户端套接字的连接

3、DatagramSocket是UDP端点API，用于发送和接收数据报包。

4、MulticastSocket是处理多播组时使用的DatagramSocket的子类。




使用TCP套接字发送和接收通过InputStreams和OutputStreams完成，可以通过getInputStream（）和getOutputStream（）方法获取。








二、接口

NetworkInterface类提供了用于浏览和查询本地计算机的所有网络接口（例如以太网连接或PPP端点）的API。 通过该类，您可以检查是否有任何本地接口配置为支持IPv6。





请注意，所有符合规范的实施必须至少支持一个NetworkInterface对象，该对象必须连接到网络，或者是一个“loopback”接口，该接口只能与同一机器上的实体进行通信。








四、高水平API

java.net包中的许多类确实提供了更高级别的抽象，并允许轻松访问网络上的资源。 类是：


1、URI是表示通用资源标识符的类，如RFC 2396所述。如名称所示，这只是一个标识符，不能直接提供访问资源的方法。

2、URL是表示通用资源定位符的类，它既是URI的旧概念，也是访问资源的一种手段。

3、URLConnection是从URL创建的，并且是用于访问URL指向的资源的通信链接。 这个抽象类将大部分的工作委托给底层协议处理程序，如http或https。

4、HttpURLConnection是URLConnection的子类，并提供了一些特定于HTTP协议的附加功能。




推荐的用法是使用URI来标识资源，然后在访问资源的时候将其转换为URL。 从该URL，您可以获得URLConnection进行精细控制，也可以直接获取InputStream。





这是一个例子：




```java
URI uri = new URI("http://java.sun.com/");
 URL url = uri.toURL();
 InputStream in = url.openStream();
```





五、协议处理程序

如前所述，URL和URLConnection依赖于必须存在的协议处理程序，否则抛出异常。 这是与仅识别资源的URI的主要区别，因此不需要访问协议处理程序。 因此，虽然可以使用任何类型的协议方案（例如myproto：//myhost.mydomain/resource/）创建URI，但是类似的URL将尝试实例化指定协议的处理程序; 如果不存在，将抛出异常。





默认情况下，协议处理程序从默认位置动态加载。 但是，可以通过设置java.protocol.handler.pkgs系统属性来添加到搜索路径。 例如，如果它设置为myapp.protocols，那么URL代码将尝试，在http的情况下，首先加载myapp.protocols.http.Handler，然后，如果此失败，http.Handler从默认位置。





请注意，Handler类必须是抽象类URLStreamHandler的子类。








六、附加规格

[Networking System Properties](https://developer.android.google.cn/reference/java/net/doc-files/net-properties.html)






**接口**

[ContentHandlerFactory](https://developer.android.google.cn/reference/java/net/ContentHandlerFactory.html)


该接口为内容处理程序定义了一个工厂。





[CookiePolicy](https://developer.android.google.cn/reference/java/net/CookiePolicy.html)


CookiePolicy实现决定哪些Cookie应该被接受，哪些应该被拒绝。





[CookieStore](https://developer.android.google.cn/reference/java/net/CookieStore.html)


CookieStore对象表示Cookie的存储




[DatagramSocketImplFactory](https://developer.android.google.cn/reference/java/net/DatagramSocketImplFactory.html)


该接口为数据报套接字实现定义了一个工厂。





[FileNameMap](https://developer.android.google.cn/reference/java/net/FileNameMap.html)


一个简单的接口，提供了一种在文件名和MIME类型字符串之间映射的机制。





[ProtocolFamily](https://developer.android.google.cn/reference/java/net/ProtocolFamily.html)


代表一系列通信协议。





[SocketImplFactory](https://developer.android.google.cn/reference/java/net/SocketImplFactory.html)


该接口为套接字实现定义了一个工厂。





[SocketOption](https://developer.android.google.cn/reference/java/net/SocketOption.html)<T>


与套接字关联的套接字选项。





[SocketOptions](https://developer.android.google.cn/reference/java/net/SocketOptions.html)


获取/设置套接字选项的方法接口。





[URLStreamHandlerFactory](https://developer.android.google.cn/reference/java/net/URLStreamHandlerFactory.html)


该界面定义了一个用于URL流协议处理程序的工厂。








**类**

[Authenticator](https://developer.android.google.cn/reference/java/net/Authenticator.html)


Authenticator类代表一个知道如何获取网络连接认证的对象。





[CacheRequest](https://developer.android.google.cn/reference/java/net/CacheRequest.html)


表示在ResponseCache中存储资源的通道。





[CacheResponse](https://developer.android.google.cn/reference/java/net/CacheResponse.html)


代表从ResponseCache中检索资源的通道。





[ContentHandler](https://developer.android.google.cn/reference/java/net/ContentHandler.html)


抽象类ContentHandler是从URLConnection读取对象的所有类的超类。





[CookieHandler](https://developer.android.google.cn/reference/java/net/CookieHandler.html)


CookieHandler对象提供一个回调机制，将HTTP状态管理策略实现连接到HTTP协议处理程序中。





[CookieManager](https://developer.android.google.cn/reference/java/net/CookieManager.html)


CookieManager提供了一个CookieHandler的具体实现，它将Cookie的存储与接受和拒绝Cookie的策略分开。





[DatagramPacket](https://developer.android.google.cn/reference/java/net/DatagramPacket.html)

该类表示数据报包。





[DatagramSocket](https://developer.android.google.cn/reference/java/net/DatagramSocket.html)


此类表示用于发送和接收数据报数据包的套接字。





[DatagramSocketImpl](https://developer.android.google.cn/reference/java/net/DatagramSocketImpl.html)


抽象数据报和组播套接字实现基类。





[HttpCookie](https://developer.android.google.cn/reference/java/net/HttpCookie.html)


HttpCookie对象表示HTTPcookie，它在服务器和用户代理之间承载状态信息。





[HttpURLConnection](https://developer.android.google.cn/reference/java/net/HttpURLConnection.html)


支持HTTP特定功能的URLConnection。





[IDN](https://developer.android.google.cn/reference/java/net/IDN.html)


提供在正常Unicode表示和ASCII兼容编码（ACE）表示之间转换国际化域名（IDN）的方法。





[Inet4Address](https://developer.android.google.cn/reference/java/net/Inet4Address.html)


此类表示Internet协议版本4（IPv4）地址。





[Inet6Address](https://developer.android.google.cn/reference/java/net/Inet6Address.html)


该类表示Internet协议版本6（IPv6）地址。





[InetAddress](https://developer.android.google.cn/reference/java/net/InetAddress.html)


此类表示Internet协议（IP）地址。





[InetSocketAddress](https://developer.android.google.cn/reference/java/net/InetSocketAddress.html)


此类实现IP套接字地址（IP地址+端口号）它也可以是一对（主机名+端口号），在这种情况下将尝试解析主机名。





[InterfaceAddress](https://developer.android.google.cn/reference/java/net/InterfaceAddress.html)


该类表示网络接口地址。





[JarURLConnection](https://developer.android.google.cn/reference/java/net/JarURLConnection.html)


与Java ARchive（JAR）文件或JAR文件中的条目的URL连接。





[MulticastSocket](https://developer.android.google.cn/reference/java/net/MulticastSocket.html)


组播数据报套接字类对发送和接收IP组播数据包很有用。





[NetPermission](https://developer.android.google.cn/reference/java/net/NetPermission.html)


旧版安全码; 不使用。





[NetworkInterface](https://developer.android.google.cn/reference/java/net/NetworkInterface.html)


此类表示由名称组成的网络接口和分配给此接口的IP地址列表。





[PasswordAuthentication](https://developer.android.google.cn/reference/java/net/PasswordAuthentication.html)


PasswordAuthentication类是Authenticator使用的数据持有者。





[Proxy](https://developer.android.google.cn/reference/java/net/Proxy.html)


此类表示代理设置，通常是类型（http，socks）和套接字地址。





[ProxySelector](https://developer.android.google.cn/reference/java/net/ProxySelector.html)


选择在连接到URL引用的网络资源时使用的代理服务器（如果有）。





[ResponseCache](https://developer.android.google.cn/reference/java/net/ResponseCache.html)


表示URLConnection缓存的实现。





[SecureCacheResponse](https://developer.android.google.cn/reference/java/net/SecureCacheResponse.html)


表示最初通过安全手段（如TLS）检索的缓存响应。





[ServerSocket](https://developer.android.google.cn/reference/java/net/ServerSocket.html)


这个类实现了服务器套接字。





[Socket](https://developer.android.google.cn/reference/java/net/Socket.html)


该类实现客户端套接字（也称为“套接字”）。





[SocketAddress](https://developer.android.google.cn/reference/java/net/SocketAddress.html)


这个类代表一个没有协议附件的Socket地址。





[SocketImpl](https://developer.android.google.cn/reference/java/net/SocketImpl.html)


抽象类SocketImpl是实际实现套接字的所有类的公共超类。





[SocketPermission](https://developer.android.google.cn/reference/java/net/SocketPermission.html)


旧版安全码; 不使用。





[StandardSocketOptions](https://developer.android.google.cn/reference/java/net/StandardSocketOptions.html)


定义标准套接字选项。





[URI](https://developer.android.google.cn/reference/java/net/URI.html)


表示统一资源标识符（URI）参考。





[URL](https://developer.android.google.cn/reference/java/net/URL.html)


类URL表示统一资源定位符，指向万维网上“资源”的指针。





[URLClassLoader](https://developer.android.google.cn/reference/java/net/URLClassLoader.html)


此类加载器用于从引用JAR文件和目录的URL的搜索路径加载类和资源。





[URLConnection](https://developer.android.google.cn/reference/java/net/URLConnection.html)


抽象类URLConnection是表示应用程序和URL之间的通信链接的所有类的超类。





[URLDecoder](https://developer.android.google.cn/reference/java/net/URLDecoder.html)


HTML表单解码的实用类。





[URLEncoder](https://developer.android.google.cn/reference/java/net/URLEncoder.html)


HTML表单编码的实用程序类。





[URLStreamHandler](https://developer.android.google.cn/reference/java/net/URLStreamHandler.html)


抽象类URLStreamHandler是所有流协议处理程序的常用超类。








枚举

[Authenticator.RequestorType](https://developer.android.google.cn/reference/java/net/Authenticator.RequestorType.html)


请求认证的实体的类型。





[Proxy.Type](https://developer.android.google.cn/reference/java/net/Proxy.Type.html)


代表代理类型。





[StandardProtocolFamily](https://developer.android.google.cn/reference/java/net/StandardProtocolFamily.html)


定义通信协议的标准系列。








**异常**

[BindException](https://developer.android.google.cn/reference/java/net/BindException.html)


指示尝试将套接字绑定到本地地址和端口时发生错误。





[ConnectException](https://developer.android.google.cn/reference/java/net/ConnectException.html)


指示尝试将套接字连接到远程地址和端口时发生错误。





[HttpRetryException](https://developer.android.google.cn/reference/java/net/HttpRetryException.html)


抛出以表示HTTP请求需要重试，但由于启用了流式传输模式，因此无法自动重试。





[MalformedURLException](https://developer.android.google.cn/reference/java/net/MalformedURLException.html)


抛出以表示发生格式不正确的网址。





[NoRouteToHostException](https://developer.android.google.cn/reference/java/net/NoRouteToHostException.html)


指示尝试将套接字连接到远程地址和端口时发生错误。





[PortUnreachableException](https://developer.android.google.cn/reference/java/net/PortUnreachableException.html)


表示在连接的数据报上收到了ICMP端口不可达消息。





[ProtocolException](https://developer.android.google.cn/reference/java/net/ProtocolException.html)


抛出以表示底层协议有错误，如TCP错误。





[SocketException](https://developer.android.google.cn/reference/java/net/SocketException.html)


抛出以表示创建或访问Socket时出错。





[SocketTimeoutException](https://developer.android.google.cn/reference/java/net/SocketTimeoutException.html)


指示在套接字上发生超时时读取或接受。





[UnknownHostException](https://developer.android.google.cn/reference/java/net/UnknownHostException.html)


抛出表示无法确定主机的IP地址。





[UnknownServiceException](https://developer.android.google.cn/reference/java/net/UnknownServiceException.html)


抛出以表示发生未知服务异常。





[URISyntaxException](https://developer.android.google.cn/reference/java/net/URISyntaxException.html)


检查的异常抛出，表示无法将字符串解析为URI引用。




