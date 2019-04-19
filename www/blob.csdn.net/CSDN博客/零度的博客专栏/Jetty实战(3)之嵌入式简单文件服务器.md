# Jetty实战(3)之嵌入式简单文件服务器 - 零度的博客专栏 - CSDN博客
2016年11月16日 10:27:03[零度anngle](https://me.csdn.net/zmx729618)阅读数：754
                
对于嵌入式运行Jetty，可以通过简单的一些代码实现一个简单的文件服务器的功能，如下：
[java][view
 plain](http://blog.csdn.net/kongxx/article/details/7224423#)[copy](http://blog.csdn.net/kongxx/article/details/7224423#)
- package com.google.code.garbagecan.jettystudy.sample3;  
- 
- import org.eclipse.jetty.server.Handler;  
- import org.eclipse.jetty.server.Server;  
- import org.eclipse.jetty.server.handler.DefaultHandler;  
- import org.eclipse.jetty.server.handler.HandlerList;  
- import org.eclipse.jetty.server.handler.ResourceHandler;  
- 
- publicclass FileServer {  
- publicstaticvoid main(String[] args) throws Exception {  
-         Server server = new Server(8080);  
- 
-         ResourceHandler resourceHandler = new ResourceHandler();  
-         resourceHandler.setDirectoriesListed(true);  
-         resourceHandler.setResourceBase("E:/share");  
-         resourceHandler.setStylesheet("");  
- 
-         HandlerList handlers = new HandlerList();  
-         handlers.setHandlers(new Handler[] { resourceHandler, new DefaultHandler() });  
-         server.setHandler(handlers);  
- 
-         server.start();  
-         server.join();  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
这里使用了Jetty自带的ResourceHandler类来处理所有的请求，这个类指定了资源路径，并且允许列出目录和文件，这里需要注意的是“resourceHandler.setStylesheet("");”，如果没有这一行代码的话，在请求资源的时候后台总是会不停的抛出NullPointerException。
运行FileServer类，访问http://localhost:8080/就可以看到“E:/share”下的所有文件和目录了。
上面是通过编码的方式实现了简单的文件服务器，其实Jetty也可以通过配置文件的方式实现文件服务器的功能，如下：
创建一个配置文件com/google/code/garbagecan/jettystudy/sample3/fileserver.xml，内容如下：
[html][view
 plain](http://blog.csdn.net/kongxx/article/details/7224423#)[copy](http://blog.csdn.net/kongxx/article/details/7224423#)
- <?xmlversion="1.0"?>
- <!DOCTYPE Configure PUBLIC "-//Jetty//Configure//EN" "http://www.eclipse.org/jetty/configure.dtd">
- 
- <Configureid="FileServer"class="org.eclipse.jetty.server.Server">
- <Callname="addConnector">
- <Arg>
- <Newclass="org.eclipse.jetty.server.nio.SelectChannelConnector">
- <Setname="port">8080</Set>
- </New>
- </Arg>
- </Call>
- 
- <Setname="handler">
- <Newclass="org.eclipse.jetty.server.handler.HandlerList">
- <Setname="handlers">
- <Arraytype="org.eclipse.jetty.server.Handler">
- <Item>
- <Newclass="org.eclipse.jetty.server.handler.ResourceHandler">
- <Setname="directoriesListed">true</Set>
- <Setname="resourceBase">E:/share</Set>
- <Setname="stylesheet">.</Set>
- </New>
- </Item>
- <Item>
- <Newclass="org.eclipse.jetty.server.handler.DefaultHandler">
- </New>
- </Item>
- </Array>
- </Set>
- </New>
- </Set>
- </Configure>
![](http://static.blog.csdn.net/images/save_snippets.png)
创建一个java类FileServerWithXml
[java][view
 plain](http://blog.csdn.net/kongxx/article/details/7224423#)[copy](http://blog.csdn.net/kongxx/article/details/7224423#)
- package com.google.code.garbagecan.jettystudy.sample3;  
- 
- import org.eclipse.jetty.server.Server;  
- import org.eclipse.jetty.util.resource.Resource;  
- import org.eclipse.jetty.xml.XmlConfiguration;  
- 
- publicclass FileServerWithXml {  
- publicstaticvoid main(String[] args) throws Exception {  
-         Resource resource = Resource.newClassPathResource("/com/google/code/garbagecan/jettystudy/sample3/fileserver.xml");  
-         XmlConfiguration configuration = new XmlConfiguration(resource.getInputStream());  
-         Server server = (Server) configuration.configure();  
-         server.start();  
-         server.join();  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
运行FileServerWithXml类，访问http://localhost:8080/也可以看到“E:/share”下的所有文件和目录了。
