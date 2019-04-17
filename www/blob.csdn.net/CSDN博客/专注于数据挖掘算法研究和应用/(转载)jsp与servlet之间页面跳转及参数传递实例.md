# (转载)jsp与servlet之间页面跳转及参数传递实例 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年02月04日 14:06:40[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3878








##  兴致所来，重温动态web开发的基本mvc架构，从servlet开始 ，发现如下博客所介绍的清晰明了，故而转载。

## http://blog.csdn.net/ssy_shandong/article/details/9328985

## **1.从一个jsp页面跳转到另一个jsp页面时的参数传递**

    (1)使用request对象获取客户端提交的信息

login.jsp页面代码如下：




**[java]**[view
 plain](http://blog.csdn.net/ssy_shandong/article/details/9328985#)[copy](http://blog.csdn.net/ssy_shandong/article/details/9328985#)



- <%@ page language="java"import="java.util.*" pageEncoding="GB2312"%>  
- 
- <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">  
- <html>  
-     <body>  
-         <form action="getinfo.jsp">  
-             <input type="text" name="name">  
-             <input type="submit" value="Enter" name="submit">  
-         </form>  
-     </body>  
- </html>  

getinfo.jsp页面代码如下：




**[java]**[view
 plain](http://blog.csdn.net/ssy_shandong/article/details/9328985#)[copy](http://blog.csdn.net/ssy_shandong/article/details/9328985#)



- <%@ page language="java"import="java.util.*" pageEncoding="GB2312"%>  
- 
- <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">  
- <html>  
-     <body>  
-         <p>获取文本框提交的信息：  
-         <%  
-             String textContent = request.getParameter("name");  
-          %>  
-          <br/>  
-          <%=textContent%>  
- 
-         </p>获取按钮的名字：  
-          <%  
-             String buttonName = request.getParameter("submit");  
-           %>  
-          <br/>  
-          <%=buttonName%>  
- 
-     </body>  
- </html>  


![](https://img-blog.csdn.net/20130715113021171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3N5X3NoYW5kb25n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

    (2)使用session对象获取从客户端提交的信息

    session1.jsp页面代码如下：




**[java]**[view
 plain](http://blog.csdn.net/ssy_shandong/article/details/9328985#)[copy](http://blog.csdn.net/ssy_shandong/article/details/9328985#)



- <%@ page language="java"import="java.util.*" pageEncoding="GB2312"%>  
- 
- <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">  
- <html>  
-     <body>  
-         <form action="session2.jsp" method="post">  
-             请输入您的姓名：  
-             <input type="text" name="thename">  
-             <input type="submit" value="submit">  
-         </form>  
-     </body>  
- </html>  


    session2.jsp通过request对象提取session1.jsp表单中的thename值,并将其保存到session对象中，session2.jsp代码如下：




**[java]**[view
 plain](http://blog.csdn.net/ssy_shandong/article/details/9328985#)[copy](http://blog.csdn.net/ssy_shandong/article/details/9328985#)



- <%@ page language="java"import="java.util.*" pageEncoding="GB2312"%>  
- 
- <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">  
- <html>  
-     <body>  
-         <%  
-             String name = request.getParameter("thename");  
-             session.setAttribute("thename",name);  
-          %>  
-          您的姓名是：<%=name%>  
-          <p>  
-         <form action="session3.jsp?pwd=123456" method="post">  
-             您喜欢吃什么？  
-             <input type="text" name="food"/>  
-             <p>  
-             <input type="submit" value="submit">  
-         </form>          
-     </body>  
- </html>  


  session3.jsp的主要任务是显示结果，它从session对象提取thename的值并显示它，以证明在多个页面中可以通过session对象存储和维持信息，session3.jsp代码如下：




**[java]**[view
 plain](http://blog.csdn.net/ssy_shandong/article/details/9328985#)[copy](http://blog.csdn.net/ssy_shandong/article/details/9328985#)



- <pre name="code"class="java"><%@ page language="java"import="java.util.*" pageEncoding="GB2312"%>  
- 
- <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">  
- <html>  
-     <body>  
-         <%  
-             String food = request.getParameter("food");  
-             String name = (String)session.getAttribute("thename");  
-             String pwd = request.getParameter("pwd");  
-          %>  
-          您的姓名是：<%=name%>  
-          <p>  
-          您喜欢吃：<%=food%>  
-          <p>  
-          您的密码是：<%=pwd%>  
-     </body>  
- </html></pre>  
- <pre></pre>  
- <pre></pre>  
- <pre></pre>  
- <pre></pre>  
- <pre></pre>  
- <pre></pre>  
- <pre></pre>  



    程序运行结果如图2所示：

![](https://img-blog.csdn.net/20130717163647703)

    (3)利用隐藏域传递数据

    使用隐藏空间记录用户提交日期的hiddenindex.jsp代码如下：




**[java]**[view
 plain](http://blog.csdn.net/ssy_shandong/article/details/9328985#)[copy](http://blog.csdn.net/ssy_shandong/article/details/9328985#)



- <%@ page language="java"import="java.util.*" pageEncoding="GB2312"%>  
- 
- <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">  
- <html>  
-     <body>  
-         <form action="hidden.jsp" method="post" name="form1">  
-             <!-- 隐藏字段，系统时间，并转换为毫秒数 -->  
-             <input type="hidden" name="time" value="<%=new java.util.Date()%>">  
-             <p>  
-                 请输入您的姓名：<input type="text" name="name">  
-             </p>  
-             <input type="submit" name="submit" value="submit">  
-         </form>  
-     </body>  
- </html>  


     显示隐藏空间信息的hidden.jsp代码如下：




**[java]**[view
 plain](http://blog.csdn.net/ssy_shandong/article/details/9328985#)[copy](http://blog.csdn.net/ssy_shandong/article/details/9328985#)



- <%@ page language="java"import="java.util.*" pageEncoding="GB2312"%>  
- 
- <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">  
- <html>  
-     <body>  
-         <%  
-             String name = request.getParameter("name");  
-             String date = request.getParameter("time");  
-         %>  
-         <p>您的姓名是：  
-             <%=name %>  
-         </p>  
-         <p>提交时的系统时间为：  
-             <%=date %>  
-         </p>  
-     </body>  
- </html>  


    程序运行结果如下：

![](https://img-blog.csdn.net/20130715163936656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3N5X3NoYW5kb25n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



    使用隐藏域的一个缺点是用户可通过查看网页源程序读到隐藏域的值。

    (4)通过超链接传递数据

    建立链接到其他分页的超级链接page.jsp代码如下：




**[java]**[view
 plain](http://blog.csdn.net/ssy_shandong/article/details/9328985#)[copy](http://blog.csdn.net/ssy_shandong/article/details/9328985#)



- <%@ page language="java"import="java.util.*" pageEncoding="GB2312"%>  
- 
- <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">  
- <%  
- int PageCount = 15;  
- %>  
- <html>  
-     <body>  
-         <table>  
-             <tr>  
-                 <td>  
-                     <a href=showpage.jsp?ToPage=<%=1%>>到第1页</a>  
-                 </td>  
-                 <td>  
-                     <a href=showpage.jsp?ToPage=<%=PageCount%>>到最后一页</a>  
-                 </td>  
-             </tr>  
-         </table>        
-     </body>  
- </html>  



    接收显示分页showpage.jsp代码如下：




**[java]**[view
 plain](http://blog.csdn.net/ssy_shandong/article/details/9328985#)[copy](http://blog.csdn.net/ssy_shandong/article/details/9328985#)



- <%@ page language="java"import="java.util.*" pageEncoding="GB2312"%>  
- 
- <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">  
- <html>  
-     <body>  
-         <%  
-         String toPage = request.getParameter("ToPage");  
-         out.println("第" + toPage + "页");  
-         %>  
-     </body>  
- </html>  


 运行结果如下：

![](https://img-blog.csdn.net/20130715171149734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3N5X3NoYW5kb25n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

##     2.jsp页面传递参数给servlet


    (1)使用request对象接收参数


    新建名为ForwardServlet的Servlet类，代码如下：




**[java]**[view
 plain](http://blog.csdn.net/ssy_shandong/article/details/9328985#)[copy](http://blog.csdn.net/ssy_shandong/article/details/9328985#)



- package com.shi.servlet;  
- 
- import java.io.IOException;  
- 
- import javax.servlet.ServletException;  
- import javax.servlet.http.HttpServlet;  
- import javax.servlet.http.HttpServletRequest;  
- import javax.servlet.http.HttpServletResponse;  
- 
- /**
-  * 本类用来获得表单请求信息，并根据请求信息转发页面
-  */
- publicclass ForwardServlet extends HttpServlet  
- {  
- privatestaticfinallong serialVersionUID = 1L;  
- 
- /**
-      * 构造函数
-      */
- public ForwardServlet()  
-     {  
- super();  
-     }  
- 
- /**
-      * 初始化
-      */
- publicvoid init() throws ServletException  
-     {}  
- 
- /**
-      * doGet()方法
-      */
- publicvoid doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException  
-     {  
-         doPost(request, response);  
-     }  
- 
- /**
-      * doPost()方法
-      */
- publicvoid doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException  
-     {  
-         String name = request.getParameter("name");  
-         String pwd = request.getParameter("pwd");  
- if((name!=null && !name.equals("")) && (pwd!=null && !pwd.equals("")))  
-         {  
- if(name.equals("mr")&&pwd.equals("123"))  
-             {  
-                 request.getRequestDispatcher("success.jsp").forward(request, response);  
-             }  
- else
-             {  
-                 request.getRequestDispatcher("error.jsp").forward(request, response);  
-             }  
-         }  
-     }  
- 
- /**
-      * 销毁
-      */
- publicvoid destroy()  
-     {  
- super.destroy();  
-     }  
- }  



    在web.xml中配置ForwardServlet类，代码如下：




**[java]**[view
 plain](http://blog.csdn.net/ssy_shandong/article/details/9328985#)[copy](http://blog.csdn.net/ssy_shandong/article/details/9328985#)



- <?xml version="1.0" encoding="GB2312"?>  
- <web-app version="2.5" xmlns="http://java.sun.com/xml/ns/javaee"
-     xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
-     xsi:schemaLocation="http://java.sun.com/xml/ns/javaee 
-     http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd">
- 
-     <!-- 配置servlet -->  
-     <servlet>  
-         <servlet-name>ForwardServlet</servlet-name>  
-         <servlet-class>com.shi.servlet.ForwardServlet</servlet-class>  
-     </servlet>  
-     <servlet-mapping>  
-         <servlet-name>ForwardServlet</servlet-name>  
-         <url-pattern>/forward</url-pattern>  
-     </servlet-mapping>  
- 
-     <welcome-file-list>  
-         <welcome-file>index.jsp</welcome-file>  
-     </welcome-file-list>  
- </web-app>  


    新建index.jsp，其中<form>中的action属性值为Servlet类在web.xml中配置的url，提交方式为post，代码如下：




**[java]**[view
 plain](http://blog.csdn.net/ssy_shandong/article/details/9328985#)[copy](http://blog.csdn.net/ssy_shandong/article/details/9328985#)



- <%@ page language="java"import="java.util.*" pageEncoding="GB2312"%>  
- 
- <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">  
- <html>  
-     <body>  
-         <form action="forward" method="post">  
-             <table align="center">  
-                 <tr>  
-                     <td>用户名：</td>  
-                     <td>  
-                         <input type="text" name="name"/>  
-                     </td>  
-                 </tr>  
-                 <tr>  
-                     <td>密码：</td>  
-                     <td>  
-                         <input type="password" name="pwd"/>  
-                     </td>  
-                 </tr>  
-                 <tr>  
-                     <td colspan="2">  
-                         <input type="submit" value="登录"/>  
-                     </td>  
-                 </tr>  
-             </table>  
-         </form>  
-     </body>  
- </html>  



    登录成功页面success.jsp，代码如下：




**[java]**[view
 plain](http://blog.csdn.net/ssy_shandong/article/details/9328985#)[copy](http://blog.csdn.net/ssy_shandong/article/details/9328985#)



- <%@ page language="java"import="java.util.*" pageEncoding="GB2312"%>  
- 
- <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">  
- <html>  
-     <body>  
-         欢迎<%=request.getParameter("name") %>，登录成功！  
-     </body>  
- </html>  


    程序的运行结果图5所示：


![](https://img-blog.csdn.net/20130716111152734)

    (2)使用session对象，url，form表单传递参数

    在jsp2servlet.jsp中，分别使用request对象，session对象，url，form表单设置需要传递的参数，代码如下：




**[java]**[view
 plain](http://blog.csdn.net/ssy_shandong/article/details/9328985#)[copy](http://blog.csdn.net/ssy_shandong/article/details/9328985#)



- <%@ page language="java"import="java.util.*" pageEncoding="GB2312"%>  
- 
- <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">  
- <html>  
-     <body>  
-         <%  
-         session.setAttribute("testSession","Hello session");  
-         request.setAttribute("testRequest","Hello request");  
-         %>  
-         <a href="http://localhost:8080/086/jsp2servlet?hrefAction=toServlet">单击此处</a>  
-         <form action="jsp2servlet?action=toServlet" method="post" name="form1">  
-             <input type="text" name="username"/>  
-             <input type="submit" value="submit"/>  
-         </form>  
-     </body>  
- </html>  


    在Jsp2Servlet中接收jsp页面中传递的参数并将其显示，代码如下：




**[java]**[view
 plain](http://blog.csdn.net/ssy_shandong/article/details/9328985#)[copy](http://blog.csdn.net/ssy_shandong/article/details/9328985#)



- package com.shi.servlet;  
- 
- import java.io.IOException;  
- import java.io.PrintWriter;  
- 
- import javax.servlet.ServletException;  
- import javax.servlet.http.HttpServlet;  
- import javax.servlet.http.HttpServletRequest;  
- import javax.servlet.http.HttpServletResponse;  
- 
- publicclass Jsp2Servlet extends HttpServlet  
- {  
- /**
-      * 自动生成
-      */
- privatestaticfinallong serialVersionUID = 1L;  
- 
- /**
-      * 构造函数
-      */
- public Jsp2Servlet()  
-     {  
- super();  
-     }  
- 
- /**
-      * 初始化
-      */
- publicvoid init()  
-     {}  
- 
- /**
-      * doGet()
-      */
- publicvoid doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException  
-     {  
-         doPost(request, response);  
-     }  
- 
- /**
-      * doPost()
-      */
- publicvoid doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException  
-     {  
-         String strSession = (String)request.getSession().getAttribute("testSession");         
-         String strRequest = (String)request.getAttribute("testRequest");          
-         String strHref = request.getParameter("hrefAction");          
-         String strForm = request.getParameter("action");          
-         String username = request.getParameter("username");  
- 
-         PrintWriter out = response.getWriter();  
-         out.println("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">");  
-         out.println("<html>");  
-         out.println("<body>");  
-         out.println("保存到session中的内容： " + strSession + "<br>");  
-         out.println("保存到request中的内容：" + strRequest + "<br>");  
-         out.println("超链接中传递的参数值为：" + strHref + "<br>");  
-         out.println("form表单中传递的action值为：" + strForm + "<br>");  
-         out.println("form表单内input标签中username的值： " + username + "<br>");  
-         out.println("</body>");  
-         out.println("</html>");         
-     }  
- 
- /**
-      * 销毁
-      */
- publicvoid destroy()  
-     {  
- super.destroy();  
-     }  
- }  


    程序运行结果如图6所示：


![](https://img-blog.csdn.net/20130716164709718)

    servlet无法获取request.setAttribute()中的内容，因为在jsp2servlet.jsp中提交时，生成的一个新的request，因此无法取到jsp页面中request.setAttribute()中设置的值。




##     3.从servlet到jsp页面中传递参数


    从servlet到jsp页面传递参数，可以使用URL，request对象，session对象，新建Servlet2Jsp.java，代码如下：




**[java]**[view
 plain](http://blog.csdn.net/ssy_shandong/article/details/9328985#)[copy](http://blog.csdn.net/ssy_shandong/article/details/9328985#)



- package com.shi.servlet;  
- 
- import java.io.IOException;  
- 
- import javax.servlet.ServletException;  
- import javax.servlet.http.HttpServlet;  
- import javax.servlet.http.HttpServletRequest;  
- import javax.servlet.http.HttpServletResponse;  
- 
- publicclass Servlet2Jsp extends HttpServlet  
- {  
- /**
-      * 自动生成
-      */
- privatestaticfinallong serialVersionUID = 1L;  
- 
- /**
-      * 构造函数
-      */
- public Servlet2Jsp()  
-     {  
- super();  
-     }  
- 
- /**
-      * 初始化
-      */
- publicvoid init()  
-     {}  
- 
- /**
-      * doGet()
-      */
- publicvoid doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException  
-     {  
-         doPost(request, response);  
-     }  
- 
- /**
-      * doPost()
-      */
- publicvoid doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException  
-     {  
-         String url = "result.jsp?first=传递第一个参数&second=传递第二个参数";  
-         String strRequest = "request传值";  
-         String strSession = "session传值";  
-         request.setAttribute("strRequest", strRequest);  
-         request.getSession().setAttribute("strSession", strSession);  
- 
- /**
-          * 客户端跳转：效率低
-          * session范围属性，url中的参数会传递下去，request范围属性不传递
-          */
- //response.sendRedirect(url);
- 
- /**
-          * 服务器端跳转：常用，效率高
-          * request范围属性，session范围属性，url中的参数会传递
-          */
-         request.getRequestDispatcher(url).forward(request, response);  
-     }  
- 
- /**
-      * 销毁
-      */
- publicvoid destroy()  
-     {  
- super.destroy();  
-     }  
- }  





    在web.xml中配置该servlet后，新建参数接收页result.jsp，代码如下：




**[java]**[view
 plain](http://blog.csdn.net/ssy_shandong/article/details/9328985#)[copy](http://blog.csdn.net/ssy_shandong/article/details/9328985#)



- <%@ page language="java"import="java.util.*" pageEncoding="GB2312"%>  
- 
- <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">  
- <html>  
-     <body>  
-         <%  
-         String strURL = request.getParameter("first");  
-         String strRequest = (String)request.getAttribute("strRequest");  
-         String strSession = (String)request.getSession().getAttribute("strSession");  
- 
-         %>  
-         <p>  
-             URL中取得的属性值为：<%=strURL%>  
-         </p>        
-         <p>  
-             request中取得的属性值为：<%=strRequest%>  
-         </p>  
-         <p>  
-             session中取得的属性值为：<%=strSession%>  
-         </p>  
-     </body>  
- </html>  


    程序的运行结果如图7所示：

![](https://img-blog.csdn.net/20130717103244703)

##     4.从一个servlet到另一个servlet时的参数传递


    (1)通过超链接和表单传递数据


    通过超链接后面给定要传递参数的值，但长度有限制，通过超链接和表单传递数据，使用request.getParamter()方法，代码如下：




**[java]**[view
 plain](http://blog.csdn.net/ssy_shandong/article/details/9328985#)[copy](http://blog.csdn.net/ssy_shandong/article/details/9328985#)



- package com.shi.servlet;  
- 
- import java.io.IOException;  
- import java.io.PrintWriter;  
- 
- import javax.servlet.ServletException;  
- import javax.servlet.http.HttpServlet;  
- import javax.servlet.http.HttpServletRequest;  
- import javax.servlet.http.HttpServletResponse;  
- 
- publicclass ServletA extends HttpServlet  
- {  
- /**
-      * 自动生成
-      */
- privatestaticfinallong serialVersionUID = 1L;  
- 
- /**
-      * 构造函数
-      */
- public ServletA()  
-     {  
- super();  
-     }  
- 
- /**
-      * 初始化
-      */
- publicvoid init()  
-     {}  
- 
- /**
-      * doGet()
-      */
- publicvoid doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException  
-     {  
-         doPost(request, response);  
-     }  
- 
- /**
-      * doPost()
-      */
- publicvoid doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException  
-     {  
-         response.setContentType("text/html");  
-         PrintWriter out = response.getWriter();  
-         String name = "hello, servlet";  
-         out.println("<html>");  
-         out.println("<body>");  
- /**
-          * 超链接
-          */
-         out.println("<a href='ServletB?name="+name+"'>this is a test</a>");  
- /**
-          * 表单
-          */
-         out.println("<form action='ServletB' method='post'>");          
-         out.println("用户名：");  
-         out.println("<input type='text' name='username'>");  
-         out.println("<input type='submit' vlue='提交'>");  
-         out.println("</form>");  
-         out.println("</body>");  
-         out.println("</html>");  
-         out.flush();  
- 
-     }  
- 
- /**
-      * 销毁
-      */
- publicvoid destroy()  
-     {  
- super.destroy();  
-     }  
- }  


    接收servlet中传递参数的代码如下：




**[java]**[view
 plain](http://blog.csdn.net/ssy_shandong/article/details/9328985#)[copy](http://blog.csdn.net/ssy_shandong/article/details/9328985#)



- package com.shi.servlet;  
- 
- import java.io.IOException;  
- import java.io.PrintWriter;  
- 
- import javax.servlet.ServletException;  
- import javax.servlet.http.HttpServlet;  
- import javax.servlet.http.HttpServletRequest;  
- import javax.servlet.http.HttpServletResponse;  
- 
- publicclass ServletB extends HttpServlet  
- {  
- /**
-      * 自动生成
-      */
- privatestaticfinallong serialVersionUID = 1L;  
- 
- /**
-      * 构造函数
-      */
- public ServletB()  
-     {  
- super();  
-     }  
- 
- /**
-      * 初始化
-      */
- publicvoid init()  
-     {}  
- 
- /**
-      * doGet()
-      */
- publicvoid doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException  
-     {  
-         doPost(request, response);  
-     }  
- 
- /**
-      * doPost()
-      */
- publicvoid doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException  
-     {  
-         String name = request.getParameter("name");  
-         String username = request.getParameter("username");  
- 
- 
-         response.setContentType("text/html;charset=gb2312");  
-         PrintWriter out = response.getWriter();  
-         out.println("<html>");  
-         out.println("<body>");  
-         out.println("超链接传递的参数值为：" + name);  
-         out.println("form表单中提交的用户名为：" + username);  
-         out.println("</body>");  
-         out.println("</html>");  
-         out.flush();  
-     }  
- 
- /**
-      * 销毁
-      */
- publicvoid destroy()  
-     {  
- super.destroy();  
-     }  
- }  



    程序运行结果如图8所示：


![](https://img-blog.csdn.net/20130717144805484)

    (2)使用setAttribute()和getAttribute()方法传递参数

    设置参数的ServletA代码如下：




**[java]**[view
 plain](http://blog.csdn.net/ssy_shandong/article/details/9328985#)[copy](http://blog.csdn.net/ssy_shandong/article/details/9328985#)



- package com.shi.servlet;  
- 
- import java.io.IOException;  
- import java.io.PrintWriter;  
- 
- import javax.servlet.ServletException;  
- import javax.servlet.http.HttpServlet;  
- import javax.servlet.http.HttpServletRequest;  
- import javax.servlet.http.HttpServletResponse;  
- 
- publicclass ServletA extends HttpServlet  
- {  
- /**
-      * 自动生成
-      */
- privatestaticfinallong serialVersionUID = 1L;  
- 
- /**
-      * 构造函数
-      */
- public ServletA()  
-     {  
- super();  
-     }  
- 
- /**
-      * 初始化
-      */
- publicvoid init()  
-     {}  
- 
- /**
-      * doGet()
-      */
- publicvoid doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException  
-     {  
-         doPost(request, response);  
-     }  
- 
- /**
-      * doPost()
-      */
- publicvoid doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException  
-     {  
- /**
-          * setAttribute()和getAttribute()
-          */
-         String data = "This is a test, we need more test for this";  
-         request.setAttribute("strAtt", data);  
-         request.getRequestDispatcher("/ServletB").forward(request, response);  
-     }  
- 
- /**
-      * 销毁
-      */
- publicvoid destroy()  
-     {  
- super.destroy();  
-     }  
- }  


    获取参数的ServletB代码如下：




**[java]**[view
 plain](http://blog.csdn.net/ssy_shandong/article/details/9328985#)[copy](http://blog.csdn.net/ssy_shandong/article/details/9328985#)



- package com.shi.servlet;  
- 
- import java.io.IOException;  
- import java.io.PrintWriter;  
- 
- import javax.servlet.ServletException;  
- import javax.servlet.http.HttpServlet;  
- import javax.servlet.http.HttpServletRequest;  
- import javax.servlet.http.HttpServletResponse;  
- 
- publicclass ServletB extends HttpServlet  
- {  
- /**
-      * 自动生成
-      */
- privatestaticfinallong serialVersionUID = 1L;  
- 
- /**
-      * 构造函数
-      */
- public ServletB()  
-     {  
- super();  
-     }  
- 
- /**
-      * 初始化
-      */
- publicvoid init()  
-     {}  
- 
- /**
-      * doGet()
-      */
- publicvoid doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException  
-     {  
-         doPost(request, response);  
-     }  
- 
- /**
-      * doPost()
-      */
- publicvoid doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException  
-     {  
-         String strAtt = (String)request.getAttribute("strAtt");  
- 
-         response.setContentType("text/html;charset=gb2312");  
-         PrintWriter out = response.getWriter();  
-         out.println("<html>");  
-         out.println("<body>");  
-         out.println("getAttribute()得到的数据值为：" + strAtt);       
-         out.println("</body>");  
-         out.println("</html>");  
-         out.flush();  
-     }  
- 
- /**
-      * 销毁
-      */
- publicvoid destroy()  
-     {  
- super.destroy();  
-     }  
- }  


    程序的运行结果如图9所示：


![](https://img-blog.csdn.net/20130717145604125)

**    5.JSP页面跳转方法**

***(1)RequestDispatcher.forward()***


    在服务器端起作用，当使用forward()时，Servlet engine传递HTTP请求从当前的Servlet or JSP到另外一个Servlet，JSP或普通HTML文件，即你的form提交至a.jsp,在a.jsp用到了forward()重定向到b.jsp，此时form提交的所有信息在b.jsp都可以获得，参数自动传递。但forward()无法重定向至有frame的jsp文件，可以重定向至有frame的html文件，同时forward()无法再后面带参数传递，比如servlet?name=tom，这样不行，可以程序内通过response.setAttribute("name",name)来传至下一个页面。


    重定向后浏览器地址栏URL不变。


    通常在Servlet中使用，不在jsp中使用。




*** (2)response.sendRedirect()***


    在用户的浏览器端工作，sengRedirect()可以带参数传递，比如servlet?name=tom传至下个页面，同时它可以重定向至不同的主机上，sendRedirect()可以重定向至有frame的jsp文件。


    重定向后在浏览器地址栏上会出现重定向页面的URL。

**上述两种方式代码见图7。**


    由于response是jsp页面中的隐含对象，故在jsp页面中可以用response.sendRedirect()直接实现重定位。


    注意：


    ①.使用response.sendRedirect()时，前面不能有HTML输出


    这并不是绝对的，不能有HTML输出其实是指不能有HTML被送到了浏览器。事实上现在的Server都有cache机制，一般在8K(JSP SERVER)，这意味着，除非你关闭了cache，或者你用out.flush()强制刷新，那么在使用sendRedirect之前，有少量的HTML输出也是允许的。


    ②.response.sendRedirect()之后，应该紧跟一句return


    我们已经知道response.sendRedirect()是通过浏览器来做转向的，所以只有在页面处理完成后，才会有实际的动作。既然你已经要做转向了，那么后面的输出还有什么意义呢？而且有可能会因为后面的输出导致转向失败。


    比较：


    ①.Dispatcher.forward()是容器中控制权的转向，在客户端浏览器地址栏中不会出现转向后的地址


    ②.response.sendRedirect()则是完全的跳转，浏览器将会得到跳转的地址，并重新发送请求链接。这样，从浏览器的地址栏中可以看到跳转后的链接地址。

**前者更加高效，在前者可以满足需要时，尽量使用RequestDispatcher.forward()方法。**


    注：在有些情况下，比如，需要跳转到一个其它服务器上的资源，则必须使用HttpServletResponse.sendRedirect()方法。




***(3)<jsp:forward page=""/>***

它的底层部分是由RequestDispatcher来实现的，因此它带有RequestDispatcher.forward()方法的印记。


    如果在之前有很多输出，前面的输出已使缓冲区满，将自动输出至客户端，那么该语句将不起作用，这一点应该特别注意。


    另外要注意：它不能改变浏览器地址，刷新的话会导致重复提交。


    测试代码如下：




**[java]**[view
 plain](http://blog.csdn.net/ssy_shandong/article/details/9328985#)[copy](http://blog.csdn.net/ssy_shandong/article/details/9328985#)



- <%@ page language="java"import="java.util.*" pageEncoding="gb2312"%>  
- 
- <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">  
- <html>  
-     <head>  
-         <link rel="stylesheet" type="text/css" href="css/style.css">  
-     </head>  
-     <body>  
-         <jsp:forward page="UpdateServlet">  
-             <jsp:param value="list" name="action" />  
-         </jsp:forward>  
-     </body>  
- </html>  


    程序运行结果如图10所示：

![](https://img-blog.csdn.net/20130717154217453)

***(4)修改HTTP header的Location属性来重定向***


    通过设置直接修改地址栏来实现页面的重定向。


    索引页1.jsp代码如下：




**[java]**[view
 plain](http://blog.csdn.net/ssy_shandong/article/details/9328985#)[copy](http://blog.csdn.net/ssy_shandong/article/details/9328985#)



- <%@ page language="java"import="java.util.*" pageEncoding="GB2312"%>  
- 
- <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">  
- <html>  
-     <body>  
-         <%  
-         response.setStatus(HttpServletResponse.SC_MOVED_PERMANENTLY);  
-         String newLocn = "2.jsp";  
-         response.setHeader("Location",newLocn);  
-         %>  
-     </body>  
- </html>  


    转向页2.jsp代码如下：




**[java]**[view
 plain](http://blog.csdn.net/ssy_shandong/article/details/9328985#)[copy](http://blog.csdn.net/ssy_shandong/article/details/9328985#)



- <%@ page language="java"import="java.util.*" pageEncoding="GB2312"%>  
- 
- <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">  
- <html>  
-     <body>  
-         从1.jsp跳转至此页  
-     </body>  
- </html>  


    程序运行结果如图11所示：


![](https://img-blog.csdn.net/20130717155632625)

***    (5)JSP中实现在某页面停留若干秒后，自动重定向到另一页面***


    在jsp文件中，下面的代码：




**[java]**[view
 plain](http://blog.csdn.net/ssy_shandong/article/details/9328985#)[copy](http://blog.csdn.net/ssy_shandong/article/details/9328985#)



- <meta http-equiv="refresh" content="5; url=target.jsp"



    含义是：在5秒后正在浏览的页面将会自动变为target.jsp页。代码中5为刷新的延迟时间，以秒为单位。target.jsp为你想转向的目标页，若为本页则自动刷新本页。


    由上可知，可以通过setHeader来实现某页面停留若干秒后，自动重定向到另一页面。


    索引页5_1.jsp代码如下：




**[java]**[view
 plain](http://blog.csdn.net/ssy_shandong/article/details/9328985#)[copy](http://blog.csdn.net/ssy_shandong/article/details/9328985#)



- <%@ page language="java"import="java.util.*" pageEncoding="GB2312"%>  
- 
- <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">  
- <html>  
-     <body>  
-         <%  
-         String content = "5; url=5_2.jsp";  
-         response.setHeader("REFRESH",content);        
-         %>  
-         5_1.jsp页面，5秒后转向5_2.jsp页面。  
-     </body>  
- </html>  



    转向页5_2.jsp代码如下：




**[java]**[view
 plain](http://blog.csdn.net/ssy_shandong/article/details/9328985#)[copy](http://blog.csdn.net/ssy_shandong/article/details/9328985#)



- <%@ page language="java"import="java.util.*" pageEncoding="GB2312"%>  
- 
- <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">  
- <html>  
-     <body>  
-         从5_1.jsp转到的页面  
-     </body>  
- </html>  


    程序运行结果如下：


![](https://img-blog.csdn.net/20130717162126296)










    参考资料：

    [1].五种JSP页面跳转方法[http://developer.51cto.com/art/200902/109939.htm](http://developer.51cto.com/art/200902/109939.htm)















