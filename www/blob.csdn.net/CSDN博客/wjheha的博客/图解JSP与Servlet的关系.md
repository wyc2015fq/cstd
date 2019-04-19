# 图解JSP与Servlet的关系 - wjheha的博客 - CSDN博客
2017年06月17日 23:11:14[wjheha](https://me.csdn.net/wjheha)阅读数：130标签：[servlet																[jsp](https://so.csdn.net/so/search/s.do?q=jsp&t=blog)](https://so.csdn.net/so/search/s.do?q=servlet&t=blog)
个人分类：[Java/Java Web](https://blog.csdn.net/wjheha/article/category/6775424)
原文地址：http://blog.csdn.net/kaixinbingju/article/details/9409927
Servlet是[Java](http://lib.csdn.net/base/java)提供的用于开发Web服务器应用程序的一个组件，运行在服务器端，由Servlet容器所管理，用于生成动态的内容。Servlet是平台独立的Java类，编写一个Servlet，实际上就是按照Servlet规范编写一个Java类。
![](http://hi.csdn.net/attachment/201101/24/0_1295883647csC2.gif)
如图所示，Java提供一系列接口类（所谓接口类就是类中**所有**方法只提供方法声明，不提供任何的方法实现，这些类的实现就留给后继者去做。）：Servlet、ServletConfig、Serializable，然后通过多重继承产生一个最通用的Servlet实现类（图中Gerneric Servlet类），接下来，通过一个多重继承与实现，产生一个新的实现类HttpServlet，用户在开发Servlet程序时只需继承这个类，从而产生一个自己的类（图中Hello_Servlet类），然后根据实际开发功能与信息处理需要，去实现该类中的相关方法即可。这就是前面提到的按照Servlet规范编写一个Java类，从而编写一个Servlet。
至于JSP（JavaServlet Page）从图中可以看出，实际上它也是从Servlet继承而来。只不过它在Servlet当中又添加/修改了一些方法，作了新的封装。具体到Tomcat Web应用服务器中，它通过一个多重继承，分别从Java的HttpJspPage和HttpServlet两个类那里继承和实现一些方法，然后封装一个叫做HttpJspBase的类从而实现了一个通用化的JSP类，用户在开发自己的JSP时，只需要从HttpJspBase继承一个自己的类（如图中Hello_jsp类），然后根据需要去实现相应的方法即可。
因此这也是为什么JSP的代码中总是闪现Servlet代码框架影子的原因，其实它们只是为实现同样的功能而进行了不同封装的组件而已，血脉里留着的是一样的血。
“既生瑜何生亮？”呵呵，因为JSP确实比Servlet要更胜一筹，所谓“青出于蓝胜于蓝”，既然Sun公司要在Servlet基础上推出JSP技术，那肯定是因为JSP有它更高明的地方。
使用Servlet产生动态网页，需要在代码中打印输出很多HTML的标签，此外，在Servlet中，我们不得不将静态现实的内容和动态产生内容的代码混合在一起。使用Servlet开发动态网页，程序员和网页编辑人员将无法一起工作，因为网页编辑人员不了解Java语言，无法修改Servlet代码，而Java程序员可能也不是很了解网页编辑人员的意图，以至于无法修改和实现网页功能。为了解决这些问题，Sun公司就推出了JSP技术。
JSP是Servlet的扩展，在没有JSP之前，就已经出现了Servlet技术。Servlet是利用输出流动态生成HTML页面，包括每一个HTML标签和每个在HTML页面中出现的内容。
JSP通过在标准的HTML页面中插入Java代码，其静态的部分无须Java程序控制，只有那些需要从[数据库](http://lib.csdn.net/base/mysql)读取并根据程序动态生成信息时，才使用Java脚本控制。
事实上，JSP是Servlet的一种特殊形式，每个JSP页面就是一个Servlet实例——JSP页面由系统编译成Servlet，Servlet再负责响应用户请求。JSP其实也是Servlet的一种简化，使用JSP时，其实还是使用Servlet，因为Web应用中的每个JSP页面都会由Servlet容器生成对应的Servlet。对于Tomcat而言，JSP页面生成的Servlet放在work路径对应的Web应用下。
以apache-tomcat-7.0.37\webapps\myapp\index.jsp为例，
**[html]**[view
 plain](http://blog.csdn.net/kaixinbingju/article/details/9409927#)[copy](http://blog.csdn.net/kaixinbingju/article/details/9409927#)
- <html>
- <body>
- <center>
- Now time is: <%=new java.util.Date()%>
- </center>
- </body>
- </html>
当启动Tomcat之后，可以在Tomcat的apache-tomcat-7.0.37\work\Catalina\localhost\myapp\org\apache\jsp目录下找到如下文件：indexd.java和index.class。这两个文件都是Tomcat生成的，Tomcat根据JSP页面生成对应Servlet的Java文件及class文件。
index.java
**[html]**[view
 plain](http://blog.csdn.net/kaixinbingju/article/details/9409927#)[copy](http://blog.csdn.net/kaixinbingju/article/details/9409927#)
- //JSP页面经过Tomcat编译后默认的包  
- package org.apache.jsp;  
- 
- import javax.servlet.*;  
- import javax.servlet.http.*;  
- import javax.servlet.jsp.*;  
- 
- //继承HttpJspBase类，该类其实是个Servlet的子类  
- public final class index_jsp extends org.apache.jasper.runtime.HttpJspBase  
-     implements org.apache.jasper.runtime.JspSourceDependent {  
- 
-   private static final javax.servlet.jsp.JspFactory _jspxFactory =  
- javax.servlet.jsp.JspFactory.getDefaultFactory();  
- 
-   private static java.util.Map<java.lang.String,java.lang.Long> _jspx_dependants;  
- 
-   private javax.el.ExpressionFactory _el_expressionfactory;  
-   private org.apache.tomcat.InstanceManager _jsp_instancemanager;  
- 
-   public java.util.Map<java.lang.String,java.lang.Long> getDependants() {  
-     return _jspx_dependants;  
-   }  
- 
-   public void _jspInit() {  
- _el_expressionfactory = _jspxFactory.getJspApplicationContext(getServletConfig().getServletContext()).getExpressionFactory();  
- _jsp_instancemanager = org.apache.jasper.runtime.InstanceManagerFactory.getInstanceManager(getServletConfig());  
-   }  
- 
-   public void _jspDestroy() {  
-   }  
- 
-   //用于响应用户的方法  
-   public void _jspService(final javax.servlet.http.HttpServletRequest request, final javax.servlet.http.HttpServletResponse response)  
-         throws java.io.IOException, javax.servlet.ServletException {  
- 
-     final javax.servlet.jsp.PageContext pageContext;  
-     javax.servlet.http.HttpSession session = null;  
-     final javax.servlet.ServletContext application;  
-     final javax.servlet.ServletConfig config;  
-     //获得页面输出流  
-     javax.servlet.jsp.JspWriter out = null;  
-     final java.lang.Object page = this;  
-     javax.servlet.jsp.JspWriter _jspx_out = null;  
-     javax.servlet.jsp.PageContext _jspx_page_context = null;  
- 
- 
-     //开始生成响应  
-     try {  
-       //设置输出的页面格式  
-       response.setContentType("text/html");  
- pageContext = _jspxFactory.getPageContext(this, request, response,  
-                 null, true, 8192, true);  
- _jspx_page_context = pageContext;  
- application = pageContext.getServletContext();  
- config = pageContext.getServletConfig();  
- session = pageContext.getSession();  
-       //页面输出流  
- out = pageContext.getOut();  
- _jspx_out = out;  
- 
-       //输出流，开始输出页面文档  
-       out.write("<html>\r\n");  
-       out.write("<body>\r\n");  
-       out.write("<center> \r\n");  
-       out.write("Now time is: ");  
-       out.print(new java.util.Date());  
-       out.write(" \r\n");  
-       out.write("</center>\r\n");  
-       out.write("</body>\r\n");  
-       out.write("</html>");  
-     } catch (java.lang.Throwable t) {  
-       if (!(t instanceof javax.servlet.jsp.SkipPageException)){  
- out = _jspx_out;  
-         if (out != null && out.getBufferSize() != 0)  
-           try { out.clearBuffer(); } catch (java.io.IOException e) {}  
-         if (_jspx_page_context != null) _jspx_page_context.handlePageException(t);  
-         else throw new ServletException(t);  
-       }  
-     } finally {  
-       _jspxFactory.releasePageContext(_jspx_page_context);  
-     }  
-   }  
- }  
JSP页面中内置了几个对象，如pageContext、application、config、page、session、out等_jspService()方法，这几个内置对象就是在这里定义的。
根据上面的JSP页面工作原理图，可以得到如下结论： JSP文件必须在JSP服务器内运行。JSP文件必须生成Servlet才能执行。每个JSP页面的第一个访问者速度很慢，因为必须等待JSP编译成Servlet。JSP页面的访问者无须安装任何客户端，甚至不需要可以运行Java的运行环境，因为JSP页面输送到客户端的是标准HTML页面。index.jsp页面中的每个字符都由index.java文件的输出流生成.
servlet是在web服务器上的java程序，它提供服务，由它来传递给你html的格式。Servlet API为Servlet提供了统一的编程接口
Servlet必须部署在Servlet容器，才能响应客户端的请求  对外提供服务。要对外统一接口，由容器来调用。
jsp侧重显示；servlet侧重控制逻辑。
MVC模式：Jsp + Servlet +　JavaBean。M-JavaBean V-Jsp C-Servlet
小应用程序（Applet）是指采用Java创建的基于HTML的程序。浏览器将其暂时下载到用户的硬盘上，并在Web页打开时在本地运行。们可以直接嵌入到网页或者其他特定的容器中，并能够产生特殊的效果。
所有基于Java的服务器端编程都是构建在Servlet之上的。在J2EE中Servlet已经是一个标准的组件。
目前，Servlet引擎一般是第三方的插件，它通过一定的方法连接到Web服务器，Servlet引擎把它识别为Servlet请求的那些HTTP请求截获下来处理，而其他的HTTP请求由Web服务器按照通常的方式来处理，Servlet引擎会装载合适的Servlet到内存中，如果Servlet还没有运行的话，会分配一个可以使用的线程来处理请求，再把Servlet的输出返回到发出请求的Web客户机。
Java
 Servlet和[Java](http://lib.csdn.net/base/java)Applet正好是相对应的两种程序类型，Applet运行在客户端，在浏览器内执行，而Servlet在服务器内部运行，通过客户端提交的请求启动运行。
同样的Servlet完全可以在Apache，IIS等不同Web服务器上执行，不管底层的[操作系统](http://lib.csdn.net/base/operatingsystem)是Windows，Solaris，Mac，[Linux](http://lib.csdn.net/base/linux)。
Java
 Servlet有着十分广泛的应用。使用Servlet还可以实现大量的服务器端的管理维护功能，以及各种特殊的任务，比如，并发处理多个请求，转送请求，代理等。
典型的Servlet运行环境有JSWDK，Tomcat，Resin等，。它们都自带一个简单的HTTP
 Server，只需简单配置即可投入使用，你也可以把它们绑定到常用的Web服务器上，如Apache，IIS等，提供小规模的Web服务。还有一些商业的大中型的支持Servlet和JSP的Web服务器，如JRun，Web Sphere，Web Logic等等，配置比较复杂，并不适合初学者。但是功能较为强大，有条件的读者可以一试。
**[html]**[view
 plain](http://blog.csdn.net/kaixinbingju/article/details/9409927#)[copy](http://blog.csdn.net/kaixinbingju/article/details/9409927#)
-     import java.io.*;  
- 　import java.util.*;  
- 　import javax.servlet.http.*;  
- 　import javax.servlet.*;  
- 　//导入必要的包  
- 　public class HelloServlet extends HttpServlet {   
- 　　　//所有Servlet必须从HttpServlet派生  
- 　　　public void doGet (HttpServletRequest req, HttpServletResponse res)   
- 　　　throws ServletException, IOException   
- 　　　//doGet()是这个Servlet的核心，真正处理请求的地方  
- 　{  
- 　res.setContentType("text/html");   
- 　　　//设置相应的类型为text/html  
- 　PrintWriter pw = res.getWriter();   
- 　　　//从HttpServletResponse得到输出流 　　  
-   　pw.println("<!DOCTYPE HTML PUBLIC ""-//W3C//DTD HTML 4.0 Transitional//EN"">");  
- 　pw.println("<head>");  
- 　pw.println("<metahttp-equiv=""Content-Type"" content=""text/html; charset=ISO-8859-1"">");  
- 　pw.println("<!-- The Servlet expression tags interpolate script variables into the HTML -->");  
- 　pw.println("<title>Hello, world!</title>");  
- 　pw.println("</head>");  
- 　pw.println("<bodybgcolor=#cc99dd>");  
- 　pw.println("<h1>Hello, world!</h1>");  
- 　pw.println("</body>");  
- 　//上面的语句都是向客户端打印HTML文本  
- 　pw.close();   
- 　//关闭HttpServletResponse，使Web服务器知道相应结束  
- }  
- public HelloServlet() {} //构造函数，可以不要  
Servlet与表单交互的方法
Servlet使用HttpServlet类中的方法与表单进行交互。在HttpServlet类中有几个未完全实现的方法，你可以自己定义方法的内容，但是必须正确使用方法名称以使HTTP Server把客户请求正确的映射到相应的函数上。
　　doHeader 用于处理HEADER请求
　　doGet 用于处理GET请求，也可以自动的支持HEADER请求
　　doPost 用于处理POST请求
　　doPut 用于处理PUT请求
　　doDelete 用于处理DELETE请求
在使用这些方法时必须带两个参数。第一个包含来自客户端的数据HttpServletRequest。第二个参数包含客户端的相应HttpServletResponse。
一个HttpServletRequest对象提供请求HTTP头部数据，也允许获取客户端的数据。怎样获取这些数据取决于HTTP请求方法。不管何种HTTP方式，都可以用getParameterValues方法返回特定名称的参数值。HttpServletRequest,HttpServletResponse接口分别继承于ServletRequest和ServletResponse接口，getParameterValues和getWriter方法都是其祖先接口中的方法。
对于HTTP
 GET请求的方式，getQueryString方法将会返回一个可以用来解剖分析的参数值。
　　对于用HTTP POST，PUT和DELETE请求的方式，HttpServletRequest有两种方法可以选择：如果是文本数据，你能通过getReader的方法得到BufferedReader获取数据；如果是二进制数据，可以通过getInputStream方法得到ServletInputStream获取数据。
　　为了相应客户端，一个HttpServletResponse对象提供返回数据给用户的两个方法：一种是用getWriter方法得到一个PrintWriter，用于返回文本数据；另一种方法是用getOutputStream方法得到ServletOutputStream，用于返回二进制数据。在使用Writer或OutputStream之前应先设置头部（HttpServletResponse中有相应的方法），然后用Writer或OutputStream将相应的主体部分发给用户。完成后要关闭Writer或OutputStream以便让服务器知道相应已经结束。
在进行HTTP网络传输的时候，统一采用的编码方式是ISO-8859-1
字符编码转换常用的方法是
　　String native_encoded = "中文字符串"; //本地编码的字符串
　　Byte[] byte_array = native_encoded.getBytes(); //得到本地编码的字节数组
　　String net_encoded = new String(native_encoded, "ISO-8859-1"); //生成ISO-8859-1编码的字符串
例：out.println(new
 String(new String("<td>你的姓名：</td>").getBytes(),"ISO-8859-1"));
用Servlet控制会话
会话状态的维持是开发Web应用所必须面对的问题，有多种方法可以来解决这个问题，如使用Cookies，或直接把状态信息加到URL中等，还有Servlet本身提供了一个HttpSession接口来支持会话状态的维持
 。
Session的发明是为了填补HTTP协议的局限。
从服务器这端来看，每一个请求都是独立的，因此HTTP协议被认为是无状态协议，当用户在多个主页间切换时，服务器无法知道他的身份。Session的出现就是为了弥补这个局限。利用Session，您就可以当一个用户在多个主页间切换的时候也能保存他的信息。这样很多以前根本无法去做的事情就变得简单多了。
    在访问者从到达某个特定的主页到离开为止的那段时间，每个访问者都会单独获得一个Session
Java
 Servlet定义了一个HttpSession接口，实现的Session的功能，在Servlet中使用Session的过程如下：
　　（1） 使用HttpServletRequest的getSession方法得到当前存在的session，如果当前没有定义session，则创建一个新的session，还可以使用方法getSession（true）
　　（2） 写session变量。可以使用方法HttpSession.setAttribute（name，value）来向Session中存储一个信息。
　　（3） 读Session变量。可以使用方法HttpSession.getAttribute（name）来读取Session中的一个变量值，如果name是一个没有定义的变量，那么返回的是null。需要注意的是，从getAttribute读出的变量类型是Object，必须使用强制类型转换，比如：String uid = (String) session.getAttribute("uid");
　　（4） 关闭session，当时用完session后，可以使用session.invalidate()方法关闭session。但是这并不是严格要求的。因为，Servlet引擎在一段时间之后，自动关闭seesion。
      HttpSession session = request.getSession(true); //参数true是指在没有session时创建一个新的
　　　Date created = new Date(session.getCreationTime()); //得到session对象创建的时间
　　　out.println("ID " + session.getId()+"<br>"); //得到该session的id，并打印
　　　out.println("Created: " + created+"<br>");//打印session创建时间
　　　session.setAttribute("UID","12345678"); //在session中添加变量UID=12345678
　　　session.setAttribute("Name","Tom"); //在session中添加变量Name=Tom 　10.2.4 Servlet的生命周期
跟客户端的Applet相似，Servlet也遵循严格的生命周期。在每个Servlet实例的生命中有三种类型的事件，这三种事件分别对应于由Servlet引擎所唤醒的三个方法：
　　1．init()。当Servlet第一次被装载时，Servlet引擎调用这个Servlet的init()方法，只调用一次。系统保证，在init方法成功完成以前，是不会调用Servlet去处理任何请求的。
　　2．service()。这是Servlet最重要的方法，是真正处理请求的地方。对于每个请求，Servlet引擎将调用Servlet的service方法，并把Servlet请求对象和Servlet响应对象作为参数传递给它。
　　3．destroy()。这是相对于init的可选方法，当Servlet即将被卸载时由Servlet引擎来调用，这个方法用来清除并释放在init方法中所分配的资源。 
Servlet的生命周期可以被归纳为以下几步：
　　（1） 装载Servlet，这一项操作一般是动态执行的。然而，Servlet通常会提供一个管理的选项，用于在Servlet启动时强制装载和初始化特定的Servlet
　　（2） Server创建一个Servlet实例
　　（3） Server调用Servlet的init方法
　　（4） 一个客户端请求到达Server
　　（5） Server创建一个请求对象
　　（6） Server创建一个响应对象
　　（7） Server激活Servlet的service方法，传递请求和响应对象作为参数
　　（8） service方法获得关于请求对象的信息，处理请求，访问其他资源，获得需要的信息
　　（9） service方法使用响应对象的方法。将响应传回Server，最终到达客户端。Service方法可能激活其他方法以处理请求。如doGet，doPost或其他程序员自己开发的方法
　　（10） 对于更多的客户端请求，Server创建新的请求和响应对象，仍然激活此servlet的service方法，将这两个对象作为参数传递给它，如此重复以上的循环，但无需再次调用init方法，Servlet一般只初始化一次
　　（11） 当Server不再需要Servlet时，比如当Server要关闭时，Server调用Servlet的destroy
JSP一般的运行方式为：当服务器启动后，当Web浏览器端发送过来一个页面请求时，Web服务器先判断是否是JSP页面请求。如果该页面只是一般的HTML/XML页面请求，则直接将HTML/XML页面代码传给Web浏览器端。如果请求的页面是JSP页面，则由JSP引擎检查该JSP页面，如果该页面是第一次被请求、或不是第一次被请求但已被修改，则JSP引擎将此JSP页面代码转换成Servlet代码，然后JSP引擎调用服务器端的Java编译器javac.exe对Servlet代码进行编译，把它变成字节码(.class)文件，然后再调用JAVA虚拟机执行该字节码文件，然后将执行结果传给Web浏览器端。如果该JSP页面不是第一次被请求，且没有被修改过，则直接由JSP引擎调用JAVA虚拟机执行已编译过的字节码.class文件，然后将结果传送Web浏览器端。
采用JSP来表现页面，采用Servlet来完成大量的处理，Servlet扮演一个控制者的角色，并负责响应客户请求。Servlet创建JSP需要的Bean和对象，根据用户的行为，决定将哪个JSP页面发送给用户。特别要注意的是，JSP页面中没有任何商业处理逻辑，它只是简单的检索Servlet先前创建的Beans或者对象，再将动态内容插入预定义的模板。
**[html]**[view
 plain](http://blog.csdn.net/kaixinbingju/article/details/9409927#)[copy](http://blog.csdn.net/kaixinbingju/article/details/9409927#)
- <prename="code"class="html"></pre>
- <pre></pre>
- <prename="code"class="html"><pre></pre>
- <pre></pre>
- <pre></pre>
- <pre></pre>
- <pre></pre>
- <pre></pre>
- 
- </pre>
[](http://blog.csdn.net/kaixinbingju/article/details/9409927#)[](http://blog.csdn.net/kaixinbingju/article/details/9409927#)[](http://blog.csdn.net/kaixinbingju/article/details/9409927#)[](http://blog.csdn.net/kaixinbingju/article/details/9409927#)[](http://blog.csdn.net/kaixinbingju/article/details/9409927#)[](http://blog.csdn.net/kaixinbingju/article/details/9409927#)
