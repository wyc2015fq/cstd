# JDBC+Servlet+JSP整合开发之26.JSP内建对象 - weixin_33985507的博客 - CSDN博客
2010年04月20日 12:51:20[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：23
**–使用内建对象的目的 –内建对象 –out 内建对象 –request 内建对象 –response 对象 –session 内建对象 –pageContext 内建对象 –application 内建对象 –config 内建对象 –page 内建对象 –exception 内建对象**
&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
**? 使用内建对象的目的**
–JSP 为了简化页面的开发提供了一些内建对象 
–这些内建对象在使用之前不需要实例化 
–它们由容器（如：Tomcat）来实现和管理 
–在所有的JSP页面中都能使用内建对象 
–所有的的内建对象只能在代码块和表达式中使用 
–不能在JSP声明中使用
**? 内建对象 **–request 
**TestJspServlet.java**
package com.michael.servlet;    
import java.io.IOException;    
import java.io.PrintWriter;    
import javax.servlet.ServletException;    
import javax.servlet.http.HttpServlet;    
import javax.servlet.http.HttpServletRequest;    
import javax.servlet.http.HttpServletResponse;    
publicclass TestJspServlet extends HttpServlet {    
/**    
         * Constructor of the object.    
         */
public TestJspServlet() {    
super();    
        }    
/**    
         * Destruction of the servlet. <br>    
         */
publicvoid destroy() {    
super.destroy(); // Just puts "destroy" string in log    
// Put your code here    
        }    
/**    
         * The doGet method of the servlet. <br>    
         *    
         * This method is called when a form has its tag value method equals to get.    
         *    
         * @param request the request send by the client to the server    
         * @param response the response send by the server to the client    
         * @throws ServletException if an error occurred    
         * @throws IOException if an error occurred    
         */
publicvoid doGet(HttpServletRequest request, HttpServletResponse response)    
throws ServletException, IOException {    
                doPost(request,response);    
        }    
/**    
         * The doPost method of the servlet. <br>    
         *    
         * This method is called when a form has its tag value method equals to post.    
         *    
         * @param request the request send by the client to the server    
         * @param response the response send by the server to the client    
         * @throws ServletException if an error occurred    
         * @throws IOException if an error occurred    
         */
publicvoid doPost(HttpServletRequest request, HttpServletResponse response)    
throws ServletException, IOException {    
                request.setAttribute("name", "michael");    
                request.getRequestDispatcher("/RequestJsp.jsp").forward(request, response);    
        }    
/**    
         * Initialization of the servlet. <br>    
         *    
         * @throws ServletException if an error occurs    
         */
publicvoid init() throws ServletException {    
// Put your code here    
        }    
} 
**RequestJsp.jsp**
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_12717385150yUC.png)
测试request是否被封装成HttpServletRequest接口
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738519K4dj.png)
–response 
–pageContext 
–session 
**TestJspServlet.java**
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738522yfvh.png)
**RequestJsp.jsp**
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738526vkLo.png)
测试
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_12717385282Lcu.png)
下面测试request和session的作用域
**RequestJsp2.jsp**
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738534r5ic.png)
看下效果：
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738537O3TP.png)
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738539zhFH.png)
发现session作用域针对当前页面，age为20，而request针对当前请求，请求结束就终止了，name为null。
**TestJspServlet.java**
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738543rC03.png)
**RequestJsp.jsp**
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738547v6DY.png)
看下效果：
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738551jww8.png)
–application 
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738554EZTw.png)
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738559921U.png)
看下效果：
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738563qbP7.png)
测试其作用域
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738570327h.png)
现在只有application有值，其他都为空哈~
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738573ZdLm.png)
–out 
–config 
–page 
–exception
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738577uqo3.png)
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_12717385796zVA.png)
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738581d0GL.png)
**? out 内建对象**
–作用 
? out 对象被封装成 javax.servlet.jsp.JspWriter接口 
? 表示为客户端打开的输出流 
? PrintWriter使用它向客户端发送输出流 
–主要方法 
? 输出各种数据类型的数据，如：out.print(boolean)、 
out.println(boolean)/out.print(int)、out.println(int)等 
? out.newLine() ：输出一个换行字符 
? out.flush()：输出缓冲区里的数据 
? out.close()：关闭输出流 
? out.clearBuffer()：清除缓冲区中的数据，并把数据输出到客户端 
? out.clear()：清除缓冲区中的数据，但不把数据输出到客户端 
? out.getBufferSize()：获得缓冲区大小 
? out.getRemaining()：获得缓冲区中剩余大小 
? out.isAutoFlush()：判断缓冲区是否自动刷新 
**TestOut.jsp**
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738585NbHp.png)
测试：
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_12717385880mhe.png)
**? request 内建对象 **– 作用 
? request 对象代表请求对象 
? 它被封装成HttpServletRequest接口 
? 具有HttpServletRequest接口的所有特征 
? 它作为jspService()方法的一个参数由容器传递给JSP页面 
– 主要方法 
? 取得请求参数的方法 
– String getParameter(String name) 取得name 的参数值 
– Enumeration getParameterNames( ) 取得所有的参数名称 
– String [] getParameterValues(String name) 取得所有name 的参数值 
– Map getParameterMap( ) 取得一个要求参数的Map 
? 取得请求标头的方法 
– String getHeader(String name) 取得name 的标头 
– Enumeration getHeaderNames() 取得所有的标头名称 
– Enumeration getHeaders(String name) 取得所有name 的标头 
– Map getParameterMap( ) 取得一个要求参数的Map 
– int getIntHeader(String name) 取得整数类型name 的标头 
– long getDateHeader(String name) 取得日期类型name 的标头 
– Cookie [] getCookies( ) 取得与请求有关的cookies 
? 其他请求的方法 
–String getContextPath( ) 取得Context 路径 
–String getMethod( ) 取得HTTP 的方法(GET、POST) 
–String getProtocol( ) 取得使用的协议 (HTTP/1.1、HTTP/1.0 ) 
–String getQueryString( ) 取得请求的参数字符串，不过，HTTP的方法必须为GET 
–String getRequestedSessionId( ) 取得用户端的Session ID 
–String getRequestURI( ) 取得请求的URL，但是不包括请求的参数字符串 
–Cookie [] getCookies( ) 取得与请求有关的cookies 
–String getRemoteAddr( ) 取得用户的IP 地址 
–String getRemoteHost( ) 取得用户的主机名称 
–int getRemotePort( ) 取得用户的主机端口 
–String getRemoteUser( ) 取得用户的名称 
–void getCharacterEncoding(String encoding) 设定编码格式，用来解决页 
面传递中文的问题
下面看下servlet和jsp是如何获取请求参数
先看下servlet哈~
**MyForm.jsp**
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738591kOZw.png)
**TestRequestServlet.java**
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_12717385966bUe.png)
测试：
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738599lhJU.png)
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738602pyiH.png)
下面我们通过JSP中request内建对象来获取哈~
**MyForm.jsp**
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738607fdKA.png)
**TestRequestJsp.jsp**
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738611jdOZ.png)
测试：
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738613gmMZ.png)
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738617s12S.png)
下面再测试Enumeration getHeaderNames() 取得所有的标头名称
首先我们还是来看下Servlet是怎么操作的哈~
**TestRequestServlet.java**
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738620t7t0.png)
测试：
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738640HsbF.png)
再看下jsp如何操作哈~
**TestRequestJsp.jsp**
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738650Tjjg.png)
测试：
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738656jLvY.png)
测试String getContextPath()取得Context路径
**TestRequestJsp.jsp**
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738661lMOP.png)
测试：
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738663sTMv.png)
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738667FJur.png)
测试String getQueryString( ) 取得请求的参数字符串，不过，HTTP的方法必须为GET 
**MyForm2.jsp**
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738671GURR.png)
**TestRequestJsp.jsp**
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738675RkKW.png)
测试：
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738678BXlm.png)
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738683GPc3.png)
同样我们可以动态指定，不过HTTP的方法必须为POST 
**MyForm2.jsp**
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738688n6UD.png)
测试
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738692aeF6.png)
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738696pVOG.png)
String getRemoteAddr()取得用户的IP地址
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738699F6ya.png)
测试：
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_12717387018D5B.png)
装localhost换成IP地址192.168.1.100测试下
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738707UB9I.png)
我们换台电脑测试下
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738712IayP.png)
**? response 对象 **– 作用 
? response 对象主要将JSP 处理数据后的结果传回到客户端 
? response 对象是实现 javax.servlet.http.HttpServletResponse 接口 
– 主要方法 
? 设定表头的方法 
– void addCookie(Cookie cookie) 新增cookie 
– void addDateHeader(String name, long date) 新增long类型的值到name标头 
– void addHeader(String name, String value) 新增String 类型的值到name 标头 
– void addIntHeader(String name, int value) 新增int 类型的值到name 标头 
– void setDateHeader(String name, long date) 指定long类型的值到name标头 
– void setHeader(String name, String value) 指定String 类型的值到name 标头 
– void setIntHeader(String name, int value) 指定int 类型的值到name 标头 
? 设定响应状态码的方法 
– void sendError(int sc) 传送状态码(status code) 
– void sendError(int sc, String msg) 传送状态码和错误信息 
– void setStatus(int sc) 设定状态码 
? 用来URL 重写(rewriting)的方法 
– String encodeRedirectURL(String url) 对使用sendRedirect( )方法的URL予以编码
void sendError(int sc) 传送状态码(status code)来动态手动设置状态码
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738715DtGn.png)
测试：
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738717rZQv.png)
测试void sendError(int sc, String msg) 传送状态码和错误信息
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_12717387219MLf.png)
测试：
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_12717387268EYN.png)
String encodeRedirectURL(String url) 对使用sendRedirect( )方法的URL予以编码
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738729qh5c.png)
测试：
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738733SOsu.png)
**? session 内建对象**
– 作用 
? session 对象表示目前用户的会话(session)状况。 
?用此项机制可以轻易识别每一个用户， 然后针对每一个别用户的要求，给予正确的响应。
? session 对象实现javax.servlet.http.HttpSession 接口 
– 主要方法 
? long getCreationTime() 取得session产生的时间，单位是毫秒，由1970 年1 月1日零时算起 
? String getId()  取得session 的ID 
? long getLastAccessedTime() 取得用户最后通过这个session送出请求的时间，单 位是毫秒，由1970 年1 月1 日零时算起 
? long getMaxInactiveInterval() 取得最大session不活动的时间，若超过这时间，session 将会失效，时间单位为秒
? void invalidate() 取消session 对象，并将对象存放的内容完全实效 
? boolean isNew() 判断session 是否为"新"的，所谓"新"的session，表示session 
已由服务器产生，但是client 尚未使用 
? void setMaxInactiveInterval(int interval) 设定最大session不活动的时间， 若超过这时间，session 将会失效，时间单位为秒
**? application 内建对象 **–作用 
? application 对象实现javax.servlet.ServletContext 接口 
? 它主要功用在于取得或更改 Servlet 的设定 
–主要方法 
? 容器相关信息的方法 
– int getMajorVersion( )  取得Container 主要的Servlet API 版本，如： 2  
–int getMinorVersion( ) 取得Container 次要的Servlet API 版本，如：4  
–String getServerInfo( ) 取得Container 的名称和版本 
?有关服务端的路径和文件的方法 
–String getMimeType(String file)  取得指定文件的MIME 类型 
–ServletContext getContext(String uripath)  取得指定Local URL 的 Application context    
–String getRealPath(String path)取得本地端path的绝对路径
?有关日志记录的方法 
–void log(String message)  将信息写入log 文件中 
–void log(String message, Throwable throwable)  将stack trace 所产生的异常信息写入log文件中
**TestApplication.jsp**
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_12717387379hF7.png)
测试：
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738741GXqx.png)
**? pageContext 内建对象**
– 作用 
? pageContext对象能够存取其他隐含对象。 
? 当隐含对象本身也支持属性时，pageContext对象也提供存取那些属性的方法。 
– 主要方法 
? 取得其他隐含对象的方法 
– Exception getException( ) 返回目前网页的异常，不过此网页要为error page， 例如：exception 隐含对象 
– JspWriter getOut( ) 返回目前网页的输出流，例如：out 隐含对象 
– Object getPage( ) 返回目前网页的Servlet 实体(instance)，例如：page 隐含对象 
– ServletRequest getRequest( ) 返回目前网页的请求，例如：request 隐含对象 
– ServletResponse getResponse( ) 返回目前网页的响应，例如：response 隐含对象 
– ServletConfig getServletConfig( ) 返回目前此网页的ServletConfig 对象，例如：config 隐含对象 
– ServletContext getServletContext( ) 返回目前此网页的执行环境(context)，例如： application隐含对象 
– HttpSession getSession( ) 返回和目前网页有联系的会话(session)，例如： session 隐含对象 
? 取得属性的方法 
– Object getAttribute(String name, int scope) 返回name 属性，范围为scope 的 属性对象， 回传类型为 java.lang.Object 
– Enumeration getAttributeNamesInScope(int scope) 返回所有属性范围为scope 的属性名称，回传类型为Enumeration 
– int getAttributesScope(String name) 返回属性名称为name 的属性范围 
– void removeAttribute(String name) 移除属性名称为name 的属性对象 
– void removeAttribute(String name, int scope) 移除属性名称为name，范围为 scope 的属性对象 
– void setAttribute(String name, Object value, int scope) 指定属性对象的名称为name、值 为value、范围为scope 
– Object findAttribute(String name) 寻找在所有范围中属性名称为name 的属性对象 
? 范围常量 
– PAGE_SCOPE  存入pageContext 对象的属性范围 
– REQUEST_SCOPE  存入request 对象的属性范围 
– SESSION_SCOPE  存入session 对象的属性范围 
– APPLICATION_SCOPE   存入application 对象的属性范围 
**TestPageContext.jsp**
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738746K89J.png)
测试：
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738750sY7c.png)
**? exception 内建对象**
–作用 
?当JSP 网页有错误时会产生异常，而exception 对象就 
来针对这个异常做处理 
–主要方法 
? getMessage( ) 获得错误信息 
? getLocalizedMessage( ) 获得本地错误信息 
? printStackTrace(new java.io.PrintWriter(out))  
打印堆栈信息
**MyErrorPage.jsp **
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738755AiRX.png)
**TestException.jsp **
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738758h3HA.png)
测试：
![image](http://redking.blog.51cto.com/attachment/201004/20/27212_1271738761Ecsj.png)
&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
