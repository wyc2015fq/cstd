# 使用Handler（嵌入Jetty学习二） - z69183787的专栏 - CSDN博客
2016年11月01日 15:15:57[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1340
一个Jetty Server可以看成由一下几部分组成，其中Connector负责接收客户端的HTTP请求，请求的处理是由Handler来完成的。在上一个例子中HelloHandler的作用就是处理用户请求，并输出结果。
![Image:JettyUML1.png](http://wiki.eclipse.org/images/8/88/JettyUML1.png)
    Handler在Jetty中是一个非常重要的东西，Jetty内部实现了一些Handler，可以分为一下几类：
    1.协调Handler：负责将request路由到其他Handler的Handler（如：HandlerConnection, ContextHandlerConnection）
    2.过滤Handler：负责向request中设置一些参数，然后再把请求转交给其他Handler（如：HandlerWapper, ContextHandler, SessionHandler）
    3.生成Handler：负责生成响应的内容（如：ResourceHandler, ServletHandler）
    Handler的接口是这样的：
Java代码  ![收藏代码](http://ipjmc.iteye.com/images/icon_star.png)
- publicvoid handle(String target, Request baseRequest, HttpServletRequest request, HttpServletResponse response)  
- throws IOException, ServletException  
    在自定义Handler的时候，必须要实现这个方法，可以直接输出响应，也可以对Request做一些包装再把它交给下一个Handler。
    Handler的强大的地方是可以为Jetty Server设置若干Handler，每个Handler完成自己的功能，Handler的处理过程如下：
![](http://dl.iteye.com/upload/attachment/0063/1963/d923a65b-a9e0-3032-8abb-e60988849c09.png)
   HandlerList和HandlerConnection内部都可以设置若干Handler，Handler按顺序一个接一个的执行。对于HandlerList而言，只要有一个Handler将请求标记为已处理，或抛出异常，Handler的调用就到此结束。而HandlerConnection则不会结束，一直调用到最后一个Handler。
    下面以一个具体的例子来说明，需要注意HandlerList和HandlerConnection的区别，代码里有注释：
Java代码  ![收藏代码](http://ipjmc.iteye.com/images/icon_star.png)
- publicclass ManyHandlers {  
- publicstaticvoid main(String[] args) throws Exception {  
-         Server server = new Server(8080);  
- 
- // create the handlers
-         Handler param = new ParamHandler();  
-         HandlerWrapper wrapper = new HandlerWrapper() {  
- @Override
- publicvoid handle(String target, Request baseRequest, HttpServletRequest request, HttpServletResponse response)  
- throws IOException, ServletException {  
-                 request.setAttribute("welcome", "Wylazy");  
- super.handle(target, baseRequest, request, response);  
-             }  
-         };  
- 
-         Handler hello = new HelloHandler();  
-         wrapper.setHandler(hello);  
-         Handler dft = new DefaultHandler();  
- 
- //HandlerList会依次调用每一个Handler，直到某个Handler将请求标记为已处理，即setHandled(true);
-         HandlerList list = new HandlerList();  
-         list.setHandlers(new Handler[] { param, wrapper, dft });  
- 
-         RequestLogHandler log = new RequestLogHandler();  
-         log.setRequestLog(new NCSARequestLog(File.createTempFile("demo", "log").getAbsolutePath()));  
- 
- //HandlerCollection会依次调用每一个Handler，即使请求已经被处理了
-         HandlerCollection handlers = new HandlerCollection();  
-         handlers.setHandlers(new Handler[] { list, log });  
- 
-         server.setHandler(handlers);  
- 
-         server.start();  
-         server.join();  
-     }  
- 
- publicstaticclass ParamHandler extends AbstractHandler {  
- publicvoid handle(String target, Request baseRequest, HttpServletRequest request, HttpServletResponse response)  
- throws IOException, ServletException {  
-             Map params = request.getParameterMap();  
- if (params.size() > 0) {  
-                 response.setContentType("text/plain");  
-                 response.getWriter().println(new JSONObject(params));  
-                 ((Request) request).setHandled(true);  
-             }  
-         }  
-     }  
- }  
参考：
[http://zldeng1984.iteye.com/blog/1368376](http://zldeng1984.iteye.com/blog/1368376)
[http://wiki.eclipse.org/Jetty/Howto/Write_Jetty_Handler](http://wiki.eclipse.org/Jetty/Howto/Write_Jetty_Handler)
[http://wiki.eclipse.org/Jetty/Reference/Jetty_Architecture](http://wiki.eclipse.org/Jetty/Reference/Jetty_Architecture)
