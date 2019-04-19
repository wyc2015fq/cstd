# Jetty实战(2)之嵌入式运行Jetty多Connector - 零度的博客专栏 - CSDN博客
2016年11月16日 10:14:35[零度anngle](https://me.csdn.net/zmx729618)阅读数：691
                
在嵌入运行Jetty的时候，有时候想要启动两个端口，或者通过一个Jetty server提供多个不同服务，比如说使用8080来指定默认访问端口，使用8433指定https访问端口等等，此时就可以通过创建多个Connector来解决。
1. 首先创建一个Server类，其中创建了两个Connector实例，一个绑定到了8080端口，另一个绑定到了9090端口：
[java][view
 plain](http://blog.csdn.net/kongxx/article/details/7218787#)[copy](http://blog.csdn.net/kongxx/article/details/7218787#)
- package com.google.code.garbagecan.jettystudy.sample2;  
- 
- import org.eclipse.jetty.server.Connector;  
- import org.eclipse.jetty.server.Server;  
- import org.eclipse.jetty.server.nio.SelectChannelConnector;  
- 
- publicclass MyServer {  
- publicstaticvoid main(String[] args) throws Exception {  
-         Server server = new Server();  
- 
-         SelectChannelConnector connector1 = new SelectChannelConnector();  
-         connector1.setPort(8080);  
- 
-         SelectChannelConnector connector2 = new SelectChannelConnector();  
-         connector2.setPort(9090);  
- 
-         server.setConnectors(new Connector[] { connector1, connector2 });  
- 
-         server.setHandler(new HelloHandler());  
- 
-         server.start();  
-         server.join();  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
2. 创建一个Handler类，用了处理所有客户端的请求
[java][view
 plain](http://blog.csdn.net/kongxx/article/details/7218787#)[copy](http://blog.csdn.net/kongxx/article/details/7218787#)
- package com.google.code.garbagecan.jettystudy.sample2;  
- 
- import java.io.IOException;  
- 
- import javax.servlet.ServletException;  
- import javax.servlet.http.HttpServletRequest;  
- import javax.servlet.http.HttpServletResponse;  
- 
- import org.eclipse.jetty.server.Request;  
- import org.eclipse.jetty.server.handler.AbstractHandler;  
- 
- publicclass HelloHandler extends AbstractHandler {  
- publicvoid handle(String target, Request baseRequest, HttpServletRequest request, HttpServletResponse response)  
- throws IOException, ServletException {  
-         response.setContentType("text/html;charset=utf-8");  
-         response.setStatus(HttpServletResponse.SC_OK);  
-         baseRequest.setHandled(true);  
-         response.getWriter().println("<h1>Hello World</h1>");  
-         response.getWriter().println("<li>Request url: " + target + "</li>");  
-         response.getWriter().println("<li>Server port: " + request.getServerPort() + "</li>");  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
3. 运行MyServer类，通过浏览器分别访问http://localhost:8080/和http://localhost:9090/。
