# jabsorb 了解 - weixin_33985507的博客 - CSDN博客
2016年04月01日 16:27:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8
jabsorb是一种基于Ajax/Web 2.0的简单轻便的框架，可用于在Web浏览中通过HTTP请求向服务端发送请求，并获得响应数据。实际上jabsorb就是json的升级版（不仅改了个名，而且包名都改了），目前最新版本是1.3。
老版本的json可以从[http://oss.metaparadigm.com/jsonrpc/download.html](http://oss.metaparadigm.com/jsonrpc/download.html)下载。
jabsorb可以从[http://jabsorb.org/Download](http://jabsorb.org/Download)下载。
jabsorb在json基础上有了很大的改进，从1.2版开始支持ORB和循环引用（Circular References）。使用jabsorb至少有以下两个好处：
1. 在jabsorb中已经支持IE, Mozilla , Firefox , Safari , Opera, Konqueror 等浏览器，因此，使用jabsorb编写的AJAX程序也就可以跨不同的Web浏览器。
2. 使用jabsorb在客户端和服务端传递数据非常方便。在客户端可以象使用本地对象一样使用服务端的对象。
    下面我们就来看一下如何使用jabsorb来编写基于AJAX的Web程序。本文使用Tomcat6.x作为Web服务器，读者可以根据需要使用其他的Web服务器。
**一、jabsorb的安装**
安装jabsorb需要如下几步：
第1步：加入jar包
jabsorb需要三个jar包：jsonrpc-1.0.jar、jsonrpc-1.0.jar和slf4j-api-1.4.2.jar，这三个文件都可以在jabsorb的压缩包中找到。将这三个文件放到<Tomcat安装目录>"lib目录中，或是放到<Web根目录>"WEB-INF"lib目录中。
第2步：配置web.xml
    打开<Web根目录>\WEB-INF\web.xml，并加入如下的配置代码：
  <servlet>
    <servlet-name>JSONRPCServlet</servlet-name>
    <servlet-class>org.jabsorb.JSONRPCServlet</servlet-class>
    <init-param>
      <param-name>gzip_threshold</param-name>
      <param-value>0</param-value>
    </init-param>
  </servlet>
  <servlet-mapping>
    <servlet-name>JSONRPCServlet</servlet-name>
    <url-pattern>/JSON-RPC</url-pattern>
  </servlet-mapping>
这段代码配置了一个jabsorb引擎，实际上就是一个Servlet（和Struts1.x类似，也是通过Servlet作为入口的）。其中gzip_threshold可以取-1、0和一个正整数。如果值为-1，表示不会对响应的内容进行压缩，如果为0，表示对响应的所有内容进行压缩，如果为一个正整数，表示当响应内容超过这个整数时,进行压缩。
但当浏览器不支持gzip压缩格式，或是经过压缩后的尺寸要比不压缩的尺寸还大时（当响应内容比较少时可能发生这种情况），jabsorb就不会对响应内容进行压缩。因此，最好将这个值设为0，但这样做所付出的代价是可能会对所有的响应内容进行压缩。具体要设成什么值，读者可根据自己的具体情况决定。
第3步 将jsonrpc.js复制到<Web根目录>\script中，读者也可以将其放到<Web根目录>中的其他可访问的位置。这个文件也可以在jabsorb的压缩包中找到。
