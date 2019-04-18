# Apache Tomcat NIO设置 - z69183787的专栏 - CSDN博客
2014年03月22日 14:24:56[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1056
NIO BIO 等区别 [http://blog.csdn.net/z69183787/article/details/21370807](http://blog.csdn.net/z69183787/article/details/21370807)
tomcat 默认 BIO
大家都听说了在**Apache **Tomcat6 中支持了Java语言的特性 NIO( New I/O)，不管你对NIO的技术是否熟悉，但你肯定能想象的到NIO是一个好东西。的确，使用NIO在服务器端会有更好的性能，加强服务器端对并发处理的性能。  请注意：很抱歉，在tomcat6在默认的配置选项中是没有把NIO功能打开。所以很多正在使用Tomcat6的朋友们本以为能快活的使用上NIO。
不信，你可以试试，从Apache Tomcat 站点[下载](http://apache.etoak.com/tomcat/tomcat-6/v6.0.24/bin/apache-tomcat-6.0.24.zip) 的Tomct6 压缩包中 conf/server.xml的 配置文件第**69**行，内容如下：
    <Connector port="8080" protocol="HTTP/1.1" 
       connectionTimeout="20000" 
        redirectPort="8443" />
进行测试，可以在控制台的启动信息里看见，默认状态下没有被打开nio配置，启动时的信息,如下：
2010-2-1 12:59:40 org.apache.coyote.http11.Http11Protocol init
信息: Initializing Coyote HTTP/1.1 on http-8080
2010-2-1 12:59:40 org.apache.catalina.startup.Catalina load
**修改成支持NIO的类型，配置如下**：
<Connector port="8080" protocol="**org.apache.coyote.http11.Http11NioProtocol**"
               connectionTimeout="20000"
               redirectPort="8443" />
进行测试，被打开nio配置，启动时的信息，如下：
2010-2-1 13:01:01 org.apache.**tomcat.util.net.NioSelectorPool getSharedSelector**
信息: **Using a shared selector for servlet write/read**
2010-2-1 13:01:01 org.apache.coyote.**http11.Http11NioProtocol init**
信息: Initializing Coyote HTTP/1.1 on http-8080
