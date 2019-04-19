# Jetty实战(5)之嵌入式Jetty运行Servlet - 零度的博客专栏 - CSDN博客
2016年11月16日 13:40:26[零度anngle](https://me.csdn.net/zmx729618)阅读数：416
                
在嵌入式Jetty中，有时候我们想运行一些的Servlet，此时就需要创建创建Context，然后让自己的Servlet运行在这些ServletContext中。
1. 首先创建一个ServletContextServer类，用来初始化web应用程序的Context，并且指定Servlet和Servlet匹配的url。这里指定了两个Servlet，分别是HelloServlet和GoodbyeServlet，并分别对应/hello/*和/goodbye/*。
[java][view
 plain](http://blog.csdn.net/kongxx/article/details/7230080#)[copy](http://blog.csdn.net/kongxx/article/details/7230080#)
- package com.google.code.garbagecan.jettystudy.sample5;  
- 
- import org.eclipse.jetty.server.Server;  
- import org.eclipse.jetty.servlet.ServletContextHandler;  
- import org.eclipse.jetty.servlet.ServletHolder;  
- 
- publicclass ServletContextServer {  
- publicstaticvoid main(String[] args) throws Exception {  
-         Server server = new Server(8080);  
- 
-         ServletContextHandler context = new ServletContextHandler(ServletContextHandler.SESSIONS);  
-         context.setContextPath("/");  
-         server.setHandler(context);  
- 
- // http://localhost:8080/hello
-         context.addServlet(new ServletHolder(new HelloServlet()), "/hello");  
- // http://localhost:8080/hello/kongxx
-         context.addServlet(new ServletHolder(new HelloServlet("Hello Kongxx!")), "/hello/kongxx");  
- 
- // http://localhost:8080/goodbye
-         context.addServlet(new ServletHolder(new GoodbyeServlet()), "/goodbye");  
- // http://localhost:8080/goodbye/kongxx
-         context.addServlet(new ServletHolder(new GoodbyeServlet("Goodbye kongxx!")), "/goodbye/kongxx");  
- 
-         server.start();  
-         server.join();  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
2. 两个简单的Servlet：HelloServlet和GoodbyeServlet：
[java][view
 plain](http://blog.csdn.net/kongxx/article/details/7230080#)[copy](http://blog.csdn.net/kongxx/article/details/7230080#)
- package com.google.code.garbagecan.jettystudy.sample5;  
- 
- import java.io.IOException;  
- import javax.servlet.ServletException;  
- import javax.servlet.http.HttpServlet;  
- import javax.servlet.http.HttpServletRequest;  
- import javax.servlet.http.HttpServletResponse;  
- 
- publicclass HelloServlet extends HttpServlet {  
- privatestaticfinallong serialVersionUID = 1L;  
- private String msg = "Hello World!";  
- 
- public HelloServlet() {  
-     }  
- 
- public HelloServlet(String msg) {  
- this.msg = msg;  
-     }  
- 
- protectedvoid doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {  
-         response.setContentType("text/html");  
-         response.setStatus(HttpServletResponse.SC_OK);  
-         response.getWriter().println("<h1>" + msg + "</h1>");  
-         response.getWriter().println("session=" + request.getSession(true).getId());  
-     }  
- }  
- 
- package com.google.code.garbagecan.jettystudy.sample5;  
- 
- import java.io.IOException;  
- import javax.servlet.ServletException;  
- import javax.servlet.http.HttpServlet;  
- import javax.servlet.http.HttpServletRequest;  
- import javax.servlet.http.HttpServletResponse;  
- 
- publicclass GoodbyeServlet extends HttpServlet {  
- privatestaticfinallong serialVersionUID = 1L;  
- private String msg = "Goodbye!";  
- 
- public GoodbyeServlet() {  
-     }  
- 
- public GoodbyeServlet(String msg) {  
- this.msg = msg;  
-     }  
- 
- protectedvoid doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {  
-         response.setContentType("text/html");  
-         response.setStatus(HttpServletResponse.SC_OK);  
-         response.getWriter().println("<h1>" + msg + "</h1>");  
-         response.getWriter().println("session=" + request.getSession(true).getId());  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
3. 运行ServletContextServer类，然后分别访问以下四个url
  http://localhost:8080/hello
  http://localhost:8080/hello/kongxx
  http://localhost:8080/goodbye
  http://localhost:8080/goodbye/kongxx
4. 除了上面的方式外，也可以创建两个个Context，分别绑定到"/hello"和"/goodbye"，如下：
[java][view
 plain](http://blog.csdn.net/kongxx/article/details/7230080#)[copy](http://blog.csdn.net/kongxx/article/details/7230080#)
- package com.google.code.garbagecan.jettystudy.sample5;  
- 
- import org.eclipse.jetty.server.Handler;  
- import org.eclipse.jetty.server.Server;  
- import org.eclipse.jetty.server.handler.ContextHandlerCollection;  
- import org.eclipse.jetty.servlet.ServletContextHandler;  
- import org.eclipse.jetty.servlet.ServletHolder;  
- 
- publicclass MultiContextServer {  
- publicstaticvoid main(String[] args) throws Exception {  
-         Server server = new Server(8080);  
- 
- // http://localhost:8080/hello/kongxx
-         ServletContextHandler context1 = new ServletContextHandler(ServletContextHandler.SESSIONS);  
-         context1.setContextPath("/hello");  
-         context1.setResourceBase(".");  
-         context1.setClassLoader(Thread.currentThread().getContextClassLoader());  
-         context1.addServlet(new ServletHolder(new HelloServlet("Hello Kongxx!")), "/kongxx");  
- 
- // http://localhost:8080/goodbye/kongxx
-         ServletContextHandler context2 = new ServletContextHandler(ServletContextHandler.SESSIONS);  
-         context2.setContextPath("/goodbye");  
-         context2.setResourceBase(".");  
-         context2.setClassLoader(Thread.currentThread().getContextClassLoader());  
-         context2.addServlet(new ServletHolder(new GoodbyeServlet("Goodbye kongxx!")), "/kongxx");  
- 
-         ContextHandlerCollection contexts = new ContextHandlerCollection();  
-         contexts.setHandlers(new Handler[] { context1, context2 });  
- 
-         server.setHandler(contexts);  
- 
-         server.start();  
-         server.join();  
-     }  
- }  
