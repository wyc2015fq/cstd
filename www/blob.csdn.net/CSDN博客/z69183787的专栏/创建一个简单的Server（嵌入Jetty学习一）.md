# 创建一个简单的Server（嵌入Jetty学习一） - z69183787的专栏 - CSDN博客
2016年11月01日 15:15:32[OkidoGreen](https://me.csdn.net/z69183787)阅读数：820
  Jetty的口号是：“不要把你的程序放到Jetty里，而是把Jetty部署到你的程序里”，也就是说可以把war包放到Jetty里（就像tomcat一样），也可以把Jetty当成程序的一个HTTP模块。
    嵌入Jetty的使用方法是这样的，可以在maven中添加depency，然后就可以在Java程序中使用Jetty创建Web Server了，目前最新的Jetty版本是Jetty 9。
Xml代码  ![收藏代码](http://ipjmc.iteye.com/images/icon_star.png)
- <!-- jetty dependecies -->
- <dependency>
- <groupId>org.eclipse.jetty</groupId>
- <artifactId>jetty-server</artifactId>
- <version>9.0.0.v20130308</version>
- </dependency>
- 
- <dependency>
- <groupId>org.eclipse.jetty</groupId>
- <artifactId>jetty-webapp</artifactId>
- <version>9.0.0.v20130308</version>
- </dependency>
- 
- <dependency>
- <groupId>org.eclipse.jetty</groupId>
- <artifactId>jetty-continuation</artifactId>
- <version>9.0.0.v20130308</version>
- </dependency>
- 
- <dependency>
- <groupId>org.eclipse.jetty</groupId>
- <artifactId>jetty-jsp</artifactId>
- <version>9.0.0.v20130308</version>
- </dependency>
下面是一个最简单的示例代码：
可以参考Jetty源码工程里面的使用示例： [http://git.eclipse.org/c/jetty/org.eclipse.jetty.project.git/tree/examples/embedded/src/main/java/org/eclipse/jetty/embedded](http://git.eclipse.org/c/jetty/org.eclipse.jetty.project.git/tree/examples/embedded/src/main/java/org/eclipse/jetty/embedded)
也可以参考Jetty的Wiki，不过只有Jetty 8以前的：
[http://wiki.eclipse.org/Jetty/Tutorial/Embedding_Jetty](http://wiki.eclipse.org/Jetty/Tutorial/Embedding_Jetty)
创建一个Handler
Java代码  ![收藏代码](http://ipjmc.iteye.com/images/icon_star.png)
- publicclass HelloHandler extends AbstractHandler  
- {  
- publicvoid handle(String target,Request baseRequest,HttpServletRequest request,HttpServletResponse response)   
- throws IOException, ServletException  
-     {  
-         response.setContentType("text/html;charset=utf-8");  
-         response.setStatus(HttpServletResponse.SC_OK);  
-         baseRequest.setHandled(true);  
-         response.getWriter().println("<h1>Hello World</h1>");  
-     }  
- }  
启动Jetty Server，并设置Handler
```java
public static void main(String[] args) throws Exception
{
    Server server = new Server(8080);
    server.setHandler(new HelloHandler());
 
    server.start();
    server.join();
}
```
