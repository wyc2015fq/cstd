# Tomcat Error Page配置 与 Error Page在IE下不能转发的问题 - z69183787的专栏 - CSDN博客
2013年11月15日 17:30:23[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1324
一：**Tomcat Error Page配置**
如果JSP页面出现异常，就会转到tomcat自动的那个异常页面，页面不怎么友好。jsp标准中提供了error page的配置，可以自己定义当出现错误时跳转到哪个页面。这个配置在web.xml里面进行配置，下面一步步来实现自定义错误页面。
1.首先，在web.xml加入下面的标签：
     <error-page>
         <error-code>400</error-code>
         <location>/400.html</location>
     </error-page>
     <error-page>
         <error-code>404</error-code>
         <location>/404.html</location>
     </error-page>
     <error-page>
         <error-code>500</error-code>
         <location>/error.jsp</location>
     </error-page>
     解释一下，error-code是错误代码，location是转向页面。如果这个配置成功，当服务器出现这个错误代码的时候，就会跳转到location这个页面。location可以是html文件，也可以是jsp页面。
2.下面编写一下error.jsp页面的代码，例如：
<[%@page](mailto:%25@page) contentType="text/html;charset=Big5" isErrorPage="true"%>
<html>
<head><title>出现错误</title></head>
<body>
     <H1>错误：</H1><%=exception%>
     <H2>错误内容：</H2>
     <%
         exception.printStackTrace(response.getWriter());
     %>
</body>
</html>
因为这个页面调用了exception内置对象，所以isErrorPage必须为true。
3.不仅可以根据html的错误代码来条转页面，也可以按异常类型来进行跳转，例如：
<error-page>
    <exception-type>javax.servlet.ServletException</exception-type>
    <location>/errorhandler.jsp</location>
</error-page>
不仅可以使用jsp内置exception对象来取得异常，也可以取得request中的attribute。例如：
<[%@page](mailto:%25@page) contentType="text/html;charset=Big5" isErrorPage="true"%>
<html>
<head><title>错误信息</title></head>
<body>
     错误码： <%=request.getAttribute("javax.servlet.error.status_code")%> <br>
     信息： <%=request.getAttribute("javax.servlet.error.message")%> <br>
     异常： <%=request.getAttribute("javax.servlet.error.exception_type")%> <br>
</body>
</html>
**二： Error Page在IE下不能转发的问题**
这是IE自身的设定导致的,经过百度,找到几个解决办法:     
1, IE设定   工具-->Internet选项-->高级--->显示http友好错误信息(取消选择) , 这样就可以了
2, 设置指定错误页页状态为正常,来告诉IE这不是一个服务器错误, 从而不显示IE的自定义错误页  
<%
    response.setStatus(200); // 200 = HttpServletResponse.SC_OK
%>
3, 把错误页做大一点,弄个几百K 就可以显示错误页面 (加一个div块,display设为none就可以了),这个问题比较奇怪.
