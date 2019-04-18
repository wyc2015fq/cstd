# servlet中service() doGet() doPost() 方法 - z69183787的专栏 - CSDN博客
2014年09月02日 14:19:04[OkidoGreen](https://me.csdn.net/z69183787)阅读数：960
个人分类：[Servlet](https://blog.csdn.net/z69183787/article/category/2175855)
HttpServlet 里的三个方法：service(HttpServletRequest
 req, HttpServletResponse resp) ，doGet(HttpServletRequest req, HttpServletResponse resp)， doPost（HttpServletRequest req, HttpServletResponse res)的区别和联系：
- 在servlet中默认情况下，无论你是get还是post 提交过来 都会经过service（）方法来处理，然后转向到doGet 
- 或是doPost方法，我们可以查看HttpServlet 类的service方法：  我在tomcat的lib目录下，解压servlet-api.jar,然后用反编译软件把lib\javax\servlet\http下的HttpServlet.class反编译，看里头的service（）方法的原代码：
- 注意，sun只是定义了servlet接口，而实现servlet接口的就是类似于tomcat的服务器，所以我是在tomcat的安装目录下找到实现的类。
protected void service(HttpServletRequest req, HttpServletResponse resp)
        throws ServletException, IOException
    {
        String method = req.getMethod();
        if(method.equals("GET"))
        {
            long lastModified = getLastModified(req);
            if(lastModified == -1L)
            {
                doGet(req, resp);
            } else
            {
                long ifModifiedSince = req.getDateHeader("If-Modified-Since");
                if(ifModifiedSince < (lastModified / 1000L) * 1000L)
                {
                    maybeSetLastModified(resp, lastModified);
                    doGet(req, resp);
                } else
                {
                    resp.setStatus(304);
                }
            }
        } else
        if(method.equals("HEAD"))
        {
            long lastModified = getLastModified(req);
            maybeSetLastModified(resp, lastModified);
            doHead(req, resp);
        } else
        if(method.equals("POST"))
            doPost(req, resp);
        else
        if(method.equals("PUT"))
            doPut(req, resp);
        else
        if(method.equals("DELETE"))
            doDelete(req, resp);
        else
        if(method.equals("OPTIONS"))
            doOptions(req, resp);
        else
        if(method.equals("TRACE"))
        {
            doTrace(req, resp);
        } else
        {
            String errMsg = lStrings.getString("http.method_not_implemented");
            Object errArgs[] = new Object[1];
            errArgs[0] = method;
            errMsg = MessageFormat.format(errMsg, errArgs);
            resp.sendError(501, errMsg);
        }
    }
- 从上面可以看出 这里的service是用来转向的，但是如果你在自己的servlet类中覆盖了service方法，比如说你的service是这样的： 
- 
- Java代码 
- 
-    1. publicvoid service(ServletRequest req, ServletResponse res)    
-    2.                    throws ServletException, IOException {    
-    3.          res.getOutputStream().print(    
-    4.          "image is <img src='images/downcoin.gif'></img><br>");    
-    5.      }   
- - 那么这时service就不是用来转向的，而是用来处理业务的，现在不论你的客户端是用pos还是get来请求此servlet 
- 
- 都会执行service方法也只能执行servlet方法，不会去执行doPost或是doGet方法。 
- 
- 比如说：你的客户端代码是： 
- Java代码 
- 
-    1. <%@page contentType="text/html; charset=utf-8"%>    
-    2. <html>    
-    3. <head><title>选择</title></head>    
-    4. <body>    
-    5. 请选择你喜欢的水果：<br>    
-    6. <form action = "Test" method = "post">    
-    7. <input type="checkbox" name="fruit" value ="apple" >苹果<br>    
-    8. <input type="checkbox" name="fruit" value ="orange">桔子<br>    
-    9. <input type="checkbox" name="fruit" value ="mango">芒果<br>    
-   10. <input type="submit" value="提交">    
-   11. </form>    
-   12. </body>    
-   13. </html>    
-   14.    
-   15. 服务端servlet是：Test类    
-   16.    
-   17. import java.io.IOException;    
-   18.    
-   19. import javax.servlet.ServletException;    
-   20. import javax.servlet.ServletOutputStream;    
-   21. import javax.servlet.ServletRequest;    
-   22. import javax.servlet.ServletResponse;    
-   23. import javax.servlet.http.HttpServlet;    
-   24. import javax.servlet.http.HttpServletRequest;    
-   25. import javax.servlet.http.HttpServletResponse;    
-   26.    
-   27. /**
-   28. * 演示service方法
-   29. */   
-   30. publicclass Test extends HttpServlet {    
-   31.    
-   32. publicvoid service(ServletRequest req, ServletResponse res)    
-   33.    throws ServletException, IOException {    
-   34.             res.getOutputStream().print("This is the service");    
-   35.    
-   36. }    
-   37.    
-   38. protectedvoid doGet(HttpServletRequest request,    
-   39.     HttpServletResponse response) throws ServletException, IOException {    
-   40.    doPost(request,response);    
-   41.    
-   42. }    
-   43. protectedvoid doPost(HttpServletRequest request,    
-   44.     HttpServletResponse response) throws ServletException, IOException {    
-   45.    ServletOutputStream out=response.getOutputStream();    
-   46.    String[] args=(String[])request.getParameterValues("fruit");    
-   47.   for(int i=0;i<args.length;i++){    
-   48.     out.print(args[i]+"<br>");    
-   49.    }    
-   50.       
-   51. }    
-   52. }   
- 点击提交后：页面输出结果为“This is the service“；
- 
- 所以，我们在写servlet的时候，一般都是重写doGet或doPost方法，不会管service方法。
