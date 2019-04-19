# Tomcat配置Error Page - 零度的博客专栏 - CSDN博客
2016年05月13日 10:10:06[零度anngle](https://me.csdn.net/zmx729618)阅读数：4258
                
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
<%@page contentType="text/html;charset=Big5" isErrorPage="true"%>
< html>
< head><title>出现错误</title></head>
< body>
     <H1>错误：</H1><%=exception%>
     <H2>错误内容：</H2>
     <%
         exception.printStackTrace(response.getWriter());
     %>
< /body>
< /html>
因为这个页面调用了exception内置对象，所以isErrorPage必须为true。
3.不仅可以根据html的错误代码来条转页面，也可以按异常类型来进行跳转，例如：
<error-page>
    <exception-type>javax.servlet.ServletException</exception-type>
    <location>/errorhandler.jsp</location>
< /error-page>
不仅可以使用jsp内置exception对象来取得异常，也可以取得request中的attribute。例如：
<%@page contentType="text/html;charset=Big5" isErrorPage="true"%>
< html>
< head><title>错误信息</title></head>
< body>
     错误码： <%=request.getAttribute("javax.servlet.error.status_code")%> <br>
     信息： <%=request.getAttribute("javax.servlet.error.message")%> <br>
     异常： <%=request.getAttribute("javax.servlet.error.exception_type")%> <br>
< /body>
< /html>
