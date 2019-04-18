# WebAppContext（嵌入Jetty学习三） - z69183787的专栏 - CSDN博客
2016年11月01日 15:22:19[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2447
先来看一下在Jetty里，什么是Context。在Jetty里Context是包含了在某一特定URL或Virtual Host下的一组Handler的Handler。可以这样理解，Context本身也是一种Handler，它里面包含了许多的Handler，这些Handler都只能处理某个特定URL下的请求。Jetty里的Context有ContextHandler，ServletContext和WebAppContext。这里主要学习一下WebAppContext。
    先回忆一下war里面web.xml里面可能有什么东西，有Security，Session，Servlet等。如果要通过Jetty部署war包的话，Jetty也必须能够处理这些东西才行。通过上一节我们知道，Jetty里都有相应的Handler与之对应，而WebAppContext就是用来组织这些Handler的。Jetty就是通过WebAppContext来部署war包的。
    WebAppContext的属性如下：
|_defaultsDescriptor|一般是$(jetty.home)/etc/webdefault.xml文件|
|----|----|
|_descriptor|web.xml配置文件路径，默认是$(baseResource)/WEB-INF/web.xml|
|_overrideDescriptors|是一个配置文件数组，会覆盖web.xml中的配置信息|
|_extractWAR|如果为true，则说明本应用是从war中解包后创建的|
|_copyDir|如果为true，则在解包时把WEB-INF目录拷贝出来|
|_parentLoaderPriority|如果为true，则加载类时优先使用父加载器|
|_tmpDir|war包解压后的目录|
|_war|原始war包路径或原始文件夹路径|
|_extraClasspath|扩展类库列表|
    下面以一段代码来说明如何通过Jetty部署war包
Java代码  ![收藏代码](http://ipjmc.iteye.com/images/icon_star.png)
- WebAppContext webapp = new WebAppContext();  
- webapp.setContextPath("/qman");  
- 
- // Additional web application descriptor containing test components.
- webapp.setDefaultsDescriptor("lib/web.xml");  
- 
- //设置原始War的位置
- webapp.setWar("qman.war");  
- 
- //将war解压的目录
- webapp.setTempDirecotry("wabapps/");  
- server.setHandler(webapp);  
- server.start();  
- server.join();  
