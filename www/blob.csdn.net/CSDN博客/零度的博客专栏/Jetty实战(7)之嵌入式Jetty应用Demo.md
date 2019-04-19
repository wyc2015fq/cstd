# Jetty实战(7)之嵌入式Jetty应用Demo - 零度的博客专栏 - CSDN博客
2016年11月16日 13:49:23[零度anngle](https://me.csdn.net/zmx729618)阅读数：2136
pom.xml：
**[html]**[view plain](http://blog.csdn.net/z69183787/article/details/52996346#)[copy](http://blog.csdn.net/z69183787/article/details/52996346#)[print](http://blog.csdn.net/z69183787/article/details/52996346#)[?](http://blog.csdn.net/z69183787/article/details/52996346#)[](https://code.csdn.net/snippets/1960706)[](https://code.csdn.net/snippets/1960706/fork)
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
- <!-- jetty dependecies -->
![](http://static.blog.csdn.net/images/save_snippets.png)
```
<!-- jetty dependecies -->
        <dependency>
            <groupId>org.eclipse.jetty</groupId>
            <artifactId>jetty-server</artifactId>
            <version>9.0.0.v20130308</version>
        </dependency>
        <dependency>
            <groupId>org.eclipse.jetty</groupId>
            <artifactId>jetty-webapp</artifactId>
            <version>9.0.0.v20130308</version>
        </dependency>
        <dependency>
            <groupId>org.eclipse.jetty</groupId>
            <artifactId>jetty-continuation</artifactId>
            <version>9.0.0.v20130308</version>
        </dependency>
        <dependency>
            <groupId>org.eclipse.jetty</groupId>
            <artifactId>jetty-jsp</artifactId>
            <version>9.0.0.v20130308</version>
        </dependency>
        <!-- jetty dependecies -->
```
demo：
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/52996346#)[copy](http://blog.csdn.net/z69183787/article/details/52996346#)[print](http://blog.csdn.net/z69183787/article/details/52996346#)[?](http://blog.csdn.net/z69183787/article/details/52996346#)[](https://code.csdn.net/snippets/1960706)[](https://code.csdn.net/snippets/1960706/fork)
- package jetty.c1;  
- 
- import org.eclipse.jetty.server.Connector;  
- import org.eclipse.jetty.server.Request;  
- import org.eclipse.jetty.server.Server;  
- import org.eclipse.jetty.server.ServerConnector;  
- import org.eclipse.jetty.server.handler.*;  
- import org.eclipse.jetty.servlet.ServletContextHandler;  
- import org.eclipse.jetty.servlet.ServletHolder;  
- 
- import javax.servlet.ServletException;  
- import javax.servlet.http.HttpServletRequest;  
- import javax.servlet.http.HttpServletResponse;  
- import java.io.IOException;  
- 
- /**
-  * Created by Administrator on 2016/10/31.
-  */
- publicclass HelloHandler  extends AbstractHandler  
- {  
- publicvoid handle(String target,Request baseRequest,HttpServletRequest request,HttpServletResponse response)  
- throws IOException, ServletException  
-     {  
-         response.setContentType("text/html;charset=utf-8");  
-         response.setStatus(HttpServletResponse.SC_OK);  
-         baseRequest.setHandled(true);  
-         response.getWriter().println("<h1>Hello "+request.getAttribute("welcome")+"</h1><div style=\"height:500px;background-image: url(data/nm.png);\">");  
-     }  
- 
- publicstaticvoid main(String[] args) throws Exception  
-     {  
-         Server server = new Server();  
- // 设置在JVM退出时关闭Jetty的钩子。
-         server.setStopAtShutdown(true);  
- //server.setHandler(new HelloHandler());
- 
- //设定多个connector绑定多个端口
-         ServerConnector connector1 = new ServerConnector(server);  
-         connector1.setPort(8080);  
- 
-         ServerConnector connector2 = new ServerConnector(server);  
-         connector2.setPort(9090);  
- 
-         server.setConnectors(new Connector[] { connector1, connector2 });  
- 
- //资源目录 列出文件名
-         ResourceHandler resourceHandler = new ResourceHandler();  
- 
-         resourceHandler.setDirectoriesListed(true);  
-         resourceHandler.setResourceBase("D:\\360Downloads");  
-         resourceHandler.setStylesheet("");  
- 
- //设置静态目录路径
-         ContextHandler staticContextHandler = new ContextHandler();  
-         staticContextHandler.setContextPath("/data");  
- //staticContextHandler.setContextPath("/files");
-         staticContextHandler.setHandler(resourceHandler);  
- 
-         ServletContextHandler servletContextHandler = new ServletContextHandler(ServletContextHandler.NO_SESSIONS);  
-         servletContextHandler.setContextPath("/");  
- 
- /**可在下面依次添加多个servlet**/
-         servletContextHandler.addServlet(new ServletHolder(new MyTestServlet("show")), "/show");  
-         servletContextHandler.addServlet(new ServletHolder(new MyTestServlet("display")), "/display");  
- //servletContextHandler.addServlet(new ServletHolder(new SpuSimilarityServlet()), "/spu");
- //servletContextHandler.addServlet(new ServletHolder(new SpuResultServlet()), "/search");
- //  servletContextHandler.addServlet(new ServletHolder(new AdminServlet()), "/db");
- 
- //设置request等 回话值
-         HandlerWrapper wrapper = new HandlerWrapper() {  
- @Override
- publicvoid handle(String target, Request baseRequest, HttpServletRequest request, HttpServletResponse response)  
- throws IOException, ServletException {  
-                 request.setAttribute("welcome", "Wylazy");  
- super.handle(target, baseRequest, request, response);  
-             }  
-         };  
- 
-         HandlerList handlers = new HandlerList();  
-         handlers.addHandler(wrapper);  
-         handlers.addHandler(servletContextHandler);  
-         handlers.addHandler(staticContextHandler);  
-         handlers.addHandler(new HelloHandler());  
-         server.setHandler(handlers);  
- 
-         server.start();  
-         server.join();  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
package jetty.c1;
import org.eclipse.jetty.server.Connector;
import org.eclipse.jetty.server.Request;
import org.eclipse.jetty.server.Server;
import org.eclipse.jetty.server.ServerConnector;
import org.eclipse.jetty.server.handler.*;
import org.eclipse.jetty.servlet.ServletContextHandler;
import org.eclipse.jetty.servlet.ServletHolder;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
/**
 * Created by Administrator on 2016/10/31.
 */
public class HelloHandler  extends AbstractHandler
{
    public void handle(String target,Request baseRequest,HttpServletRequest request,HttpServletResponse response)
            throws IOException, ServletException
    {
        response.setContentType("text/html;charset=utf-8");
        response.setStatus(HttpServletResponse.SC_OK);
        baseRequest.setHandled(true);
        response.getWriter().println("<h1>Hello "+request.getAttribute("welcome")+"</h1><div style=\"height:500px;background-image: url(data/nm.png);\">");
    }
    public static void main(String[] args) throws Exception
    {
        Server server = new Server();
        // 设置在JVM退出时关闭Jetty的钩子。
        server.setStopAtShutdown(true);
        //server.setHandler(new HelloHandler());
	//设定多个connector绑定多个端口
        ServerConnector connector1 = new ServerConnector(server);
        connector1.setPort(8080);
        ServerConnector connector2 = new ServerConnector(server);
        connector2.setPort(9090);
        server.setConnectors(new Connector[] { connector1, connector2 });
        //资源目录 列出文件名
        ResourceHandler resourceHandler = new ResourceHandler();
        resourceHandler.setDirectoriesListed(true);
        resourceHandler.setResourceBase("D:\\360Downloads");
        resourceHandler.setStylesheet("");
        //设置静态目录路径
        ContextHandler staticContextHandler = new ContextHandler();
        staticContextHandler.setContextPath("/data");
        //staticContextHandler.setContextPath("/files");
        staticContextHandler.setHandler(resourceHandler);
        ServletContextHandler servletContextHandler = new ServletContextHandler(ServletContextHandler.NO_SESSIONS);
        servletContextHandler.setContextPath("/");
        /**可在下面依次添加多个servlet**/
        servletContextHandler.addServlet(new ServletHolder(new MyTestServlet("show")), "/show");
        servletContextHandler.addServlet(new ServletHolder(new MyTestServlet("display")), "/display");
        //servletContextHandler.addServlet(new ServletHolder(new SpuSimilarityServlet()), "/spu");
        //servletContextHandler.addServlet(new ServletHolder(new SpuResultServlet()), "/search");
        //	servletContextHandler.addServlet(new ServletHolder(new AdminServlet()), "/db");
        //设置request等 回话值
        HandlerWrapper wrapper = new HandlerWrapper() {
            @Override
            public void handle(String target, Request baseRequest, HttpServletRequest request, HttpServletResponse response)
                    throws IOException, ServletException {
                request.setAttribute("welcome", "Wylazy");
                super.handle(target, baseRequest, request, response);
            }
        };
        HandlerList handlers = new HandlerList();
        handlers.addHandler(wrapper);
        handlers.addHandler(servletContextHandler);
        handlers.addHandler(staticContextHandler);
        handlers.addHandler(new HelloHandler());
        server.setHandler(handlers);
        server.start();
        server.join();
    }
}
```
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/52996346#)[copy](http://blog.csdn.net/z69183787/article/details/52996346#)[print](http://blog.csdn.net/z69183787/article/details/52996346#)[?](http://blog.csdn.net/z69183787/article/details/52996346#)[](https://code.csdn.net/snippets/1960706)[](https://code.csdn.net/snippets/1960706/fork)
- package jetty.c1;  
- 
- import com.google.gson.Gson;  
- 
- import javax.servlet.ServletException;  
- import javax.servlet.ServletOutputStream;  
- import javax.servlet.annotation.WebServlet;  
- import javax.servlet.http.HttpServlet;  
- import javax.servlet.http.HttpServletRequest;  
- import javax.servlet.http.HttpServletResponse;  
- import java.io.IOException;  
- import java.util.ArrayList;  
- import java.util.List;  
- 
- /**
-  * Created by Administrator on 2016/10/31.
-  */
- publicclass MyTestServlet extends HttpServlet {  
- 
- private String msg = "init";  
- 
- public MyTestServlet(String msg) {  
- this.msg = msg;  
-     }  
- 
- @Override
- protectedvoid doGet(HttpServletRequest req, HttpServletResponse resp)  
- throws ServletException, IOException {  
- // TODO Auto-generated method stub
-         doPost(req, resp);  
-     }  
- 
- @Override
- protectedvoid doPost(HttpServletRequest req, HttpServletResponse resp)  
- throws ServletException, IOException {  
- 
-         List<String> list = new ArrayList<String>();  
- 
-         list.add("A");  
-         list.add("中国");  
-         list.add("外国");  
-         list.add(msg);  
- 
-         Gson g = new Gson();  
- 
- //resp.setContentType("charset=utf-8");
-         ServletOutputStream out = resp.getOutputStream();  
-         String json = g.toJson(list);  
-         out.write(json.getBytes());  
-         out.flush();  
-         out.close();  
-     }  
- 
- @Override
- protectedvoid doDelete(HttpServletRequest req, HttpServletResponse resp)  
- throws ServletException, IOException {  
- super.doDelete(req, resp);  
-     }  
- 
- @Override
- protectedvoid doPut(HttpServletRequest req, HttpServletResponse resp)  
- throws ServletException, IOException {  
- super.doPut(req, resp);  
-     }  
- 
- }  
[]()
```java
package jetty.c1;
import com.google.gson.Gson;
import javax.servlet.ServletException;
import javax.servlet.ServletOutputStream;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
/**
 * Created by Administrator on 2016/10/31.
 */
public class MyTestServlet extends HttpServlet {
    private String msg = "init";
    public MyTestServlet(String msg) {
        this.msg = msg;
    }
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp)
            throws ServletException, IOException {
        // TODO Auto-generated method stub
        doPost(req, resp);
    }
    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp)
            throws ServletException, IOException {
        List<String> list = new ArrayList<String>();
        list.add("A");
        list.add("中国");
        list.add("外国");
        list.add(msg);
        Gson g = new Gson();
        //resp.setContentType("charset=utf-8");
        ServletOutputStream out = resp.getOutputStream();
        String json = g.toJson(list);
        out.write(json.getBytes());
        out.flush();
        out.close();
    }
    @Override
    protected void doDelete(HttpServletRequest req, HttpServletResponse resp)
            throws ServletException, IOException {
        super.doDelete(req, resp);
    }
    @Override
    protected void doPut(HttpServletRequest req, HttpServletResponse resp)
            throws ServletException, IOException {
        super.doPut(req, resp);
    }
}
```
运行效果：
localhost:8080/9090
![](https://img-blog.csdn.net/20161101152754780?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
localhost:8080/show
![](https://img-blog.csdn.net/20161101152852225?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
http://localhost:8080/data/  文件目录展示：
![](https://img-blog.csdn.net/20161101152925735?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
