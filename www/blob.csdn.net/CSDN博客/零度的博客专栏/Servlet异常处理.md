# Servlet异常处理 - 零度的博客专栏 - CSDN博客
2016年05月13日 13:38:56[零度anngle](https://me.csdn.net/zmx729618)阅读数：446标签：[servlet																[异常处理](https://so.csdn.net/so/search/s.do?q=异常处理&t=blog)](https://so.csdn.net/so/search/s.do?q=servlet&t=blog)
个人分类：[Servlet/JSP](https://blog.csdn.net/zmx729618/article/category/6227151)
当一个servlet抛出一个异常，Web容器中搜索inweb.xml使用与之匹配的抛出的异常类型的异常类型元素的配置。
需要使用error-page元素在web.xml中指定的servlet调用若干例外情况或HTTP状态码。
# web.xml 配置:
考虑一下，有一个ErrorHandler的servelt将被调用在有异常或错误出现时。以下将是在web.xml中创建的项。
```
<span style="font-family:Arial;font-size:12px;"><!-- servlet definition -->
<servlet>
        <servlet-name>ErrorHandler</servlet-name>
        <servlet-class>ErrorHandler</servlet-class>
</servlet>
<!-- servlet mappings -->
<servlet-mapping>
        <servlet-name>ErrorHandler</servlet-name>
        <url-pattern>/ErrorHandler</url-pattern>
</servlet-mapping>
<!-- error-code related error pages -by www.yiibai.com-->
<error-page>
    <error-code>404</error-code>
    <location>/ErrorHandler</location>
</error-page>
<error-page>
    <error-code>403</error-code>
    <location>/ErrorHandler</location>
</error-page>
<!-- exception-type related error pages -->
<error-page>
    <exception-type>
          javax.servlet.ServletException
    </exception-type >
    <location>/ErrorHandler</location>
</error-page>
<error-page>
    <exception-type>java.io.IOException</exception-type >
    <location>/ErrorHandler</location>
</error-page>
</span>
```
 如果想有一个通用的错误处理所有的异常，那么应该定义错误页，而不是定义单独的错误页面元素的每一个异常：
```
<span style="font-family:Arial;font-size:12px;"><error-page>
    <exception-type>java.lang.Throwable</exception-type >
    <location>/ErrorHandler</location>
</error-page></span>
```
 以下是关于上面的web.xml异常处理的点要注意：
- 
servelt ErrorHandler常用的方法中定义的任何其他servlet在web.xml中配置。
- 
如果有任何错误状态代码为404（未找到）或403（禁止），然后ErrorHandler的servlet将被调用。
- 
如果Web应用程序抛出或者ServletException异常或IOException异常，然后在Web容器调用的servlet/ ErrorHandler的。
- 
您可以定义不同的错误处理程序来处理不同类型的错误或异常。上面的例子是非常通用。
# 请求属性 - 错误/异常：
以下是错误处理的servlet请求属性列表中可以访问的错误/异常的性质进行分析。
|S.N.|Attribute & Description|
|----|----|
|1|javax.servlet.error.status_code该属性给一个java.lang.Integer数据类型在分析后可以被存储状态代码。|
|2|javax.servlet.error.exception_type该属性给出的异常类型可以存储和分析后存储java.lang.Class的数据类型。|
|3|javax.servlet.error.message该属性给出确切的错误信息，可以存储和分析中一个java.lang.String的数据类型后存储。|
|4|javax.servlet.error.request_uri该属性给出URL调用这个servlet，它可以存储和分析中一个java.lang.String的数据类型后存储的信息。|
|5|javax.servlet.error.exception该属性给出信息引发异常可以被存储和分析后，存储一个java.lang.Throwable数据类型。|
|6|javax.servlet.error.servlet_name此属性提供了servlet的名字，它可以存储和分析中一个java.lang.String的数据类型后存储。|
# servlet错误处理程序实例：
以下是Servlet的实例，将用于错误处理程序中的任何错误或异常的情况下发生任何定义的servlet。
这个例子将让你在Servlet中的异常处理基本的了解，但你可以编写更复杂的过滤器的应用程序使用相同的概念：
```
<span style="font-family:Arial;font-size:12px;">// Import required java libraries
import java.io.*;
import javax.servlet.*;
import javax.servlet.http.*;
import java.util.*;
// Extend HttpServlet class
public class ErrorHandler extends HttpServlet {
 
  // Method to handle GET method request.
  public void doGet(HttpServletRequest request,
                    HttpServletResponse response)
            throws ServletException, IOException
  {
      // Analyze the servlet exception - www.yiibai.com       
      Throwable throwable = (Throwable)
      request.getAttribute("javax.servlet.error.exception");
      Integer statusCode = (Integer)
      request.getAttribute("javax.servlet.error.status_code");
      String servletName = (String)
      request.getAttribute("javax.servlet.error.servlet_name");
      if (servletName == null){
         servletName = "Unknown";
      }
      String requestUri = (String)
      request.getAttribute("javax.servlet.error.request_uri");
      if (requestUri == null){
         requestUri = "Unknown";
      }
      // Set response content type - by www.yiibai.com
      response.setContentType("text/html");
 
      PrintWriter out = response.getWriter();
	  String title = "Error/Exception Information";
      String docType =
      "<!doctype html public \"-//w3c//dtd html 4.0 " +
      "transitional//en\">\n";
      out.println(docType +
        "<html>\n" +
        "<head><title>" + title + "</title></head>\n" +
        "<body bgcolor=\"#f0f0f0\">\n");
      if (throwable == null && statusCode == null){
         out.println("<h2>Error information is missing</h2>");
         out.println("Please return to the <a href=\"" + 
           response.encodeURL("http://localhost:8080/") + 
           "\">Home Page</a>.");
      }else if (statusCode != null){
         out.println("The status code : " + statusCode);
      }else{
         out.println("<h2>Error information</h2>");
         out.println("Servlet Name : " + servletName + 
                             "</br></br>");
         out.println("Exception Type : " + 
                             throwable.getClass( ).getName( ) + 
                             "</br></br>");
         out.println("The request URI: " + requestUri + 
                             "<br><br>");
         out.println("The exception message: " + 
                                 throwable.getMessage( ));
      }
      out.println("</body>");
      out.println("</html>");
  }
  // Method to handle POST method request.
  public void doPost(HttpServletRequest request,
                     HttpServletResponse response)
      throws ServletException, IOException {
     doGet(request, response);
  }
}
</span>
```
用通常的方式编译ErrorHandler.java，把你的类文件放置到<Tomcat-installation-directory>/webapps/ROOT/WEB-INF/classes中。
让我们在web.xml文件中添加以下配置来处理异常：
```
<span style="font-family:Arial;font-size:12px;"> <servlet>
        <servlet-name>ErrorHandler</servlet-name>
        <servlet-class>ErrorHandler</servlet-class>
</servlet>
<!-- servlet mappings -->
<servlet-mapping>
        <servlet-name>ErrorHandler</servlet-name>
        <url-pattern>/ErrorHandler</url-pattern>
</servlet-mapping>
<error-page>
    <error-code>404</error-code>
    <location>/ErrorHandler</location>
</error-page>
<error-page>
    <exception-type>java.lang.Throwable</exception-type >
    <location>/ErrorHandler</location>
</error-page></span>
```
现在，尝试使用一个servlet，它引发任何异常，或键入了错误的URL，这将引发Web容器调用的ErrorHandler的servlet和显示适当的消息编程。例如，如果你输入了错误的URL，然后它会显示下面的结果：
The status code : 404
上面的代码中，某些Web浏览器可能无法正常起作用。因此，尝试与Mozilla和Safari浏览器，它们应该正常工作。
