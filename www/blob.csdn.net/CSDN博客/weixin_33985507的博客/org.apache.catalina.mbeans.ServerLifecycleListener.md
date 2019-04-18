# org.apache.catalina.mbeans.ServerLifecycleListener - weixin_33985507的博客 - CSDN博客
2013年10月30日 21:58:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
Tomcat 启动报错：
```
java.lang.ClassNotFoundException: org.apache.catalina.mbeans.ServerLifecycleListener
	at java.net.URLClassLoader$1.run(URLClassLoader.java:200)
	at java.security.AccessController.doPrivileged(Native Method)
	at java.net.URLClassLoader.findClass(URLClassLoader.java:188)
	at java.lang.ClassLoader.loadClass(ClassLoader.java:307)
	at java.lang.ClassLoader.loadClass(ClassLoader.java:252)
	at org.apache.tomcat.util.digester.ObjectCreateRule.begin(ObjectCreateRule.java:144)
	at org.apache.tomcat.util.digester.Digester.startElement(Digester.java:1282)
	at com.sun.org.apache.xerces.internal.parsers.AbstractSAXParser.startElement(AbstractSAXParser.java:501)
	at com.sun.org.apache.xerces.internal.parsers.AbstractXMLDocumentParser.emptyElement(AbstractXMLDocumentParser.java:179)
	at com.sun.org.apache.xerces.internal.impl.XMLDocumentFragmentScannerImpl.scanStartElement(XMLDocumentFragmentScannerImpl.java:1339)
	at com.sun.org.apache.xerces.internal.impl.XMLDocumentFragmentScannerImpl$FragmentContentDriver.next(XMLDocumentFragmentScannerImpl.java:2747)
	at com.sun.org.apache.xerces.internal.impl.XMLDocumentScannerImpl.next(XMLDocumentScannerImpl.java:648)
	at com.sun.org.apache.xerces.internal.impl.XMLDocumentFragmentScannerImpl.scanDocument(XMLDocumentFragmentScannerImpl.java:510)
	at com.sun.org.apache.xerces.internal.parsers.XML11Configuration.parse(XML11Configuration.java:807)
	at com.sun.org.apache.xerces.internal.parsers.XML11Configuration.parse(XML11Configuration.java:737)
	at com.sun.org.apache.xerces.internal.parsers.XMLParser.parse(XMLParser.java:107)
	at com.sun.org.apache.xerces.internal.parsers.AbstractSAXParser.parse(AbstractSAXParser.java:1205)
	at com.sun.org.apache.xerces.internal.jaxp.SAXParserImpl$JAXPSAXParser.parse(SAXParserImpl.java:522)
	at org.apache.tomcat.util.digester.Digester.parse(Digester.java:1543)
	at org.apache.catalina.startup.Catalina.load(Catalina.java:554)
	at org.apache.catalina.startup.Catalina.load(Catalina.java:595)
	at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)
	at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:39)
	at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:25)
	at java.lang.reflect.Method.invoke(Method.java:597)
	at org.apache.catalina.startup.Bootstrap.load(Bootstrap.java:262)
	at org.apache.catalina.startup.Bootstrap.main(Bootstrap.java:430)
```
 原因：catalina.jar中means包是空的，具体该类的作用是配置JMX下
 JMX（Java Management Extensions，即Java管理扩展）是一个为[应用程序](http://baike.baidu.com/view/330120.htm)、[设备](http://baike.baidu.com/view/209665.htm)、系统等植入管理功能的框架。JMX可以跨越一系列异构[操作系统](http://baike.baidu.com/view/880.htm)平台、[系统体系结构](http://baike.baidu.com/view/3928625.htm)和[网络传输协议](http://baike.baidu.com/view/16807.htm)，灵活的开发无缝集成的系统、网络和服务管理应用。
 解决办法：
 把server.xml中的<Listener className="org.apache.catalina.mbeans.ServerLifecycleListener" />注释掉就可以了
 <!--<Listener className="org.apache.catalina.mbeans.ServerLifecycleListener" /> -->
 当然，如果你是少了jar，也可以把jar添加进去就搞定。
 如果你是我下面这种情况，可以考虑注释掉，也可以考虑把Tomcat7原有的Listener替换掉当前的Listener就行
 这次错误是我把Tomcat6的server.xml文件拷贝的Tomcat7下面来，想着懒得复制内容，干脆直接复制文件，没想到这个懒不能偷！
 在Tomcat6的catalina.jar中有org.apache.catalina.mbeans.ServerLifecycleListener这么个类：
![](https://img-blog.csdn.net/20131030094524359?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHp3MjMxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
而Tomcat7中则木有这个类：
![](https://img-blog.csdn.net/20131030094618031?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHp3MjMxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
并且Tomcat7的server.xml跟Tomcat7的server.xml中的Listener都不相同，在Tomcat7中并没有org.apache.catalina.mbeans.ServerLifecycleListener这个Listener。
Tomcat6的server.xml：
```
<Server port="8005" shutdown="SHUTDOWN">
  <!--APR library loader. Documentation at /docs/apr.html -->
  <Listener className="org.apache.catalina.core.AprLifecycleListener" SSLEngine="on" />
  <!--Initialize Jasper prior to webapps are loaded. Documentation at /docs/jasper-howto.html -->
  <Listener className="org.apache.catalina.core.JasperListener" />
  <!-- JMX Support for the Tomcat server. Documentation at /docs/non-existent.html -->
  <Listener className="org.apache.catalina.mbeans.ServerLifecycleListener" />
  <Listener className="org.apache.catalina.mbeans.GlobalResourcesLifecycleListener" />
```
Tomcat7的server.xml：
```
<Server port="8003" shutdown="SHUTDOWN">
  <!-- Security listener. Documentation at /docs/config/listeners.html
  <Listener className="org.apache.catalina.security.SecurityListener" />
  -->
  <!--APR library loader. Documentation at /docs/apr.html -->
  <Listener className="org.apache.catalina.core.AprLifecycleListener" SSLEngine="on" />
  <!--Initialize Jasper prior to webapps are loaded. Documentation at /docs/jasper-howto.html -->
  <Listener className="org.apache.catalina.core.JasperListener" />
  <!-- Prevent memory leaks due to use of particular java/javax APIs-->
  <Listener className="org.apache.catalina.core.JreMemoryLeakPreventionListener" />
  <Listener className="org.apache.catalina.mbeans.GlobalResourcesLifecycleListener" />
  <Listener className="org.apache.catalina.core.ThreadLocalLeakPreventionListener" />
```
