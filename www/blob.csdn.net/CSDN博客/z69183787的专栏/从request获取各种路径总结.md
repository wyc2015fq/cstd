# 从request获取各种路径总结 - z69183787的专栏 - CSDN博客
2013年05月10日 15:12:01[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3832
关于request.getRealPath（）
 1、此方法已过时，现在可替换的实现方法有
     request.getSession().getServletContext().getRealPath("/")   
     在Servlet 里用this.getServletContext().getRealPath("/");获得绝对路径。
     struts里用this.getServlet().getServletContext().getRealPath("/")获得绝对路径。
request.getRealPath("url"); // 虚拟目录映射为实际目录
request.getRealPath("./");    // 网页所在的目录
request.getRealPath("../"); // 网页所在目录的上一层目录
request.getContextPath();    // 应用的web目录的名称
如[http://localhost:7001/bookStore/](http://localhost:7001/bookStore/)
/bookStore/ => [contextPath] (request.getContextPath())
获取Web项目的全路径 
String strDirPath = request.getSession().getServletContext().getRealPath("/");
以工程名为TEST为例：
(1)得到包含工程名的当前页面全路径：request.getRequestURI() 
结果：/TEST/test.jsp
(2)得到工程名：request.getContextPath() 
结果：/TEST
(3)得到当前页面所在目录下全名称：request.getServletPath() 
结果：如果页面在jsp目录下 /TEST/jsp/test.jsp
(4)得到页面所在服务器的全路径：application.getRealPath("页面.jsp") 
结果：D:\resin\webapps\TEST\test.jsp
(5)得到页面所在服务器的绝对路径：absPath=new java.io.File(application.getRealPath(request.getRequestURI())).getParent(); 
结果：D:\resin\webapps\TEST
2.在类中取得路径：
(1)类的绝对路径：Class.class.getClass().getResource("/").getPath() 
结果：/D:/TEST/WebRoot/WEB-INF/classes/pack/
(2)得到工程的路径：System.getProperty("user.dir") 
结果：D:\TEST
3.在Servlet中取得路径： 
(1)得到工程目录：request.getSession().getServletContext().getRealPath("attach") 参数可具体到包名。 
结果：E:\Tomcat\webapps\TEST\attach
(2)得到IE地址栏地址：request.getRequestURL() 
结果：http://localhost:8080/TEST/test 
(3)得到相对地址：request.getRequestURI() 
结果：/TEST/test
test1.jsp=======================
<a href ="test.jsp?p=fuck">跳转到test2</a>
test2.jsp=======================
<%@ page language="java" import="java.util.*" pageEncoding="gb2312"%>
<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
out.println("basePath:"+basePath);
out.println("<br/>");
out.println("getContextPath:"+request.getContextPath());
out.println("<br/>");
out.println("getServletPath:"+request.getServletPath());
out.println("<br/>");
out.println("getRequestURI:"+request.getRequestURI());
out.println("<br/>");
out.println("getRequestURL:"+request.getRequestURL());
out.println("<br/>");
out.println("getRealPath:"+request.getRealPath("/"));
out.println("<br/>");
out.println("getServletContext().getRealPath:"+getServletContext().getRealPath("/"));
out.println("<br/>");
out.println("getQueryString:"+request.getQueryString());
%>
**显示结果：**
**basePath**:http://localhost:8080/test/
**getContextPath**:/test 
**getServletPath**:/test.jsp 
**getRequestURI**:/test/test.jsp 
**getRequestURL**:http://localhost:8080/test/test.jsp 
**getRealPath**:D:\Tomcat 6.0\webapps\test\ 
**getServletContext().getRealPath**:D:\Tomcat 6.0\webapps\test\ 
**getQueryString**:p=fuck
在一些应用中，未登录用户请求了必须登录的资源时，提示用户登录，此时要记住用户访问的当前页面的URL，当他登录成功后根据记住的URL跳回用户最后访问的页面：
String lastAccessUrl = request.getRequestURL() + "?" + request.getQueryString();
