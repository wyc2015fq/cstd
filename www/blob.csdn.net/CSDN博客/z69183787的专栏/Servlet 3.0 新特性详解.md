# Servlet 3.0 新特性详解 - z69183787的专栏 - CSDN博客
2016年07月13日 16:45:00[OkidoGreen](https://me.csdn.net/z69183787)阅读数：425
个人分类：[Servlet-3.0](https://blog.csdn.net/z69183787/article/category/8796435)
# Servlet 3.0 新特性概览
1.Servlet、Filter、Listener无需在web.xml中进行配置，可以通过Annotation进行配置；
2.模块化编程，即将各个Servlet模块化，将配置文件也分开配置。
3.Servlet异步处理，应对复杂业务处理；
4.异步Listener，对于异步处理的创建、完成等进行监听；
5. 文件上传API简化；
tomcat 7.0.X 支持Servlet 3.0
# 一、Annotation支持
## 1.Servlet
原本Servlet开发完后，必须在web.xml中配置如下代码：
**[html]**[view plain](http://blog.csdn.net/xiazdong/article/details/7208316#)[copy](http://blog.csdn.net/xiazdong/article/details/7208316#)
- **<****servlet****>**
- **<****servlet-name****>****</****servlet-name****>**
- **<****servler-class****>****</****servlet-class****>**
- **<****load-on-startup****>****</****load-on-startup****>**
- **<****init-param****>**
- **<****param-name****>****</****param-name****>**
- **<****param-value****>****</****param-value****>**
- **</****init-param****>**
- **</****servlet****>**
- **<****servlet-mapping****>**
- **<****servlet-name****>****</****servlet-name****>**
- **<****url-pattern****>****</****url-pattern****>**
- **</****servlet-mapping****>**
现在只需要在java源文件的Servlet类前面加上：
@WebServlet(name="",urlPatterns={""},initParams={@WebInitParam(name="",value=""),loadOnStartup=1})
public class FirstServlet extends HttpServlet{}
代码示例：
实现一个最简单的Servlet。
**[java]**[view plain](http://blog.csdn.net/xiazdong/article/details/7208316#)[copy](http://blog.csdn.net/xiazdong/article/details/7208316#)
- **package** org.servlet;  
- **import** java.io.*;  
- **import** javax.servlet.*;  
- **import** javax.servlet.http.*;  
- **import** javax.servlet.annotation.*;  
- /*
-        name == <servlet-name>
-        urlPatterns == <url-pattern>,
-        loadOnStartup == <load-on-startup>
-        initParam == <init-param>
-        name == <param-name>
-        value == <param-value>
- */
- @WebServlet(name="HelloServlet" ,urlPatterns={"/HelloServlet"},loadOnStartup=1,  
-                     initParams={  
- @WebInitParam(name="name",value="xiazdong"),  
- @WebInitParam(name="age",value="20")  
-                     })  
- **public****class** HelloServlet **extends** HttpServlet{  
- **public****void** init(ServletConfig config)**throws** ServletException{  
- **super**.init(config);  
-        }  
- **public****void** service(HttpServletRequest request,HttpServletResponse response)**throws** ServletException,IOException{  
-               request.setCharacterEncoding("GBK");  
-               ServletConfig config = getServletConfig();  
-               PrintWriter out = response.getWriter();  
-               out.println("<html>");  
-               out.println("<body>");  
-               out.println("Hello world"+"<br />");  
-               out.println(config.getInitParameter("name"));  
-               out.println("</body>");  
-               out.println("</html>");  
-        }  
- }  
这样的话只需要将class文件放入WEB-INF\classes 中，不需要再web.xml中作任何改动就完成部署；
# 2.Filter
原本Filter的配置如下：
**[html]**[view plain](http://blog.csdn.net/xiazdong/article/details/7208316#)[copy](http://blog.csdn.net/xiazdong/article/details/7208316#)
- **<****filter****>**
- **<****filter-name****>****</****filter-name****>**
- **<****filter-class****>****</****filter-class****>**
- **<****filter****>**
- **<****filter-mapping****>**
- **<****filter-name****>****</****filter-name****>**
- **<****url-pattern****>****</****url-pattern****>**
- **</****filter-mapping****>**
- 
- 
现在只需要在java源文件的Filter类前面加上
@WebFilter(filterName="",urlPattern={"/"});
# 3.Listener
原本在web.xml中配置如下：
**[html]**[view plain](http://blog.csdn.net/xiazdong/article/details/7208316#)[copy](http://blog.csdn.net/xiazdong/article/details/7208316#)
- **<****listener****>**
- **<****listener-class****>****</****listener-class****>**
- **</****listener****>**
- 
- 
现在只需要在java源文件的Listener类前面加上@WebListener即可；
# 二、web模块化
原本一个web应用的任何配置都需要在web.xml中进行，因此会使得web.xml变得很混乱，而且灵活性差，因此Servlet 3.0可以将每个Servlet、Filter、Listener打成jar包，然后放在WEB-INF\lib中；注意各自的模块都有各自的配置文件，这个配置文件的名称为  web-fragment.xml ;
制作一个Servlet模块的步骤：
1.编写Servlet，并编译；
2.将此编译class文件及所在包通过jar包命令打成jar包；
3.将此jar包用winrar打开，并将其中的META-INF中的manifest删除并添加 web-fragment.xml；
4.将此jar包放入WEB-INF\lib中即可；
web-fragment.xml注意点：
1.根元素为<web-fragment>;
2.<name></name>表示模块名称；
3.<ordering></ordering>是此模块的加载顺序；
4.<before><others/></before>表示第一个加载；
5.<after><name>A</name></after>表示比A后面加载；
6.可以在里面部署listener、filter、servlet
当然可以不设置任何的模块加载顺序；
代码示例：
配置两个listener模块；
FirstListener.java
**[java]**[view plain](http://blog.csdn.net/xiazdong/article/details/7208316#)[copy](http://blog.csdn.net/xiazdong/article/details/7208316#)
- 
- 
- **package** org.listener;  
- **import** javax.servlet.annotation.*;  
- **import** javax.servlet.http.*;  
- **import** javax.servlet.*;  
- **import** java.util.*;  
- **import** java.sql.*;  
- **import** javax.naming.*;  
- **public****class** FirstListener **implements** ServletRequestListener{  
- **public****void** requestInitialized(ServletRequestEvent event){  
-               System.out.println("FirstListener created");  
-        }  
- **public****void** requestDestroyed(ServletRequestEvent event){  
-               System.out.println("FirstListener destroyed");  
-        }  
- }  
FirstListener 的 web-fragment.xml内容：
**[html]**[view plain](http://blog.csdn.net/xiazdong/article/details/7208316#)[copy](http://blog.csdn.net/xiazdong/article/details/7208316#)
- **<?****xml**version="1.0"encoding="ISO-8859-1"**?>**
- **<****web-fragment**xmlns="http://java.sun.com/xml/ns/javaee"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="http://java.sun.com/xml/ns/javaee  
- **<****A**href="http://java.sun.com/xml/ns/javaee/web-app_3_0.xsd"">http://java.sun.com/xml/ns/javaee/web-app_3_0.xsd"
- version="3.0"**>**
- **<****name****>**FirstListener**</****name****>**
- **<****listener****>**
- **<****listener-class****>**org.listener.FirstListener**</****listener-class****>**
- **</****listener****>**
- **<****ordering****>**
- **<****before****>**
- **<****others****/>**
- **</****before****>**
- **</****ordering****>**
- **</****web-fragment****>**
SecondListener.java 
**[java]**[view plain](http://blog.csdn.net/xiazdong/article/details/7208316#)[copy](http://blog.csdn.net/xiazdong/article/details/7208316#)
- **package** org.listener;  
- **import** javax.servlet.annotation.*;  
- **import** javax.servlet.http.*;  
- **import** javax.servlet.*;  
- **import** java.util.*;  
- **import** java.sql.*;  
- **import** javax.naming.*;  
- **public****class** SecondListener **implements** ServletRequestListener{  
- **public****void** requestInitialized(ServletRequestEvent event){  
-              System.out.println("SecondListener created");  
**[java]**[view plain](http://blog.csdn.net/xiazdong/article/details/7208316#)[copy](http://blog.csdn.net/xiazdong/article/details/7208316#)
-        }  
- **public****void** requestDestroyed(ServletRequestEvent event){  
-              System.out.println("SecondListener destroyed");  
-        }  
- }  
SecondListener的 web-fragment.xml内容是：
**[html]**[view plain](http://blog.csdn.net/xiazdong/article/details/7208316#)[copy](http://blog.csdn.net/xiazdong/article/details/7208316#)
- **<?****xml**version="1.0"encoding="ISO-8859-1"**?>**
- **<****web-fragment**xmlns="http://java.sun.com/xml/ns/javaee"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="http://java.sun.com/xml/ns/javaee  
- **<****A**href="http://java.sun.com/xml/ns/javaee/web-app_3_0.xsd"">http://java.sun.com/xml/ns/javaee/web-app_3_0.xsd"
- version="3.0"**>**
- **<****name****>**SecondListener**</****name****>**
- **<****listener****>**
**[html]**[view plain](http://blog.csdn.net/xiazdong/article/details/7208316#)[copy](http://blog.csdn.net/xiazdong/article/details/7208316#)
- **<****listener-class****>**org.listener.SecondListener**</****listener-class****>**
- **</****listener****>**
- **<****ordering****>**
- **<****after****>**
- **<****name****>**FirstListener**</****name****>**
- **</****after****>**
- **</****ordering****>**
- b-fragment**>**
然后分别打成jar包，放入 WEB-INF\lib中即可；
随便访问一个web应用，然后发现 tomcat控制台输出：
看出先加载FirstListener，再加载SecondListener；
# 三、Servlet 异步处理
Servlet在MVC中作为控制器，控制器负责分发任务给MODEL完成，然后把结果交给JSP显示；
而如果有许多MODEL，其中有一个MODEL处理时间很长，则会导致整个页面的显示很慢；
异步处理关键点：将复杂业务处理另外开一个线程，而Servlet将执行好的业务先送往jsp输出，等到耗时业务做完后再送往JSP页面；
一句话：先显示一部分，再显示一部分；
异步处理Servlet的注意点是：
1.需要在Annotation中注明 asyncSupported=true;
**[java]**[view plain](http://blog.csdn.net/xiazdong/article/details/7208316#)[copy](http://blog.csdn.net/xiazdong/article/details/7208316#)
- **package** org.sync;  
- **import** javax.servlet.*;  
- **import** javax.servlet.http.*;  
- **import** javax.servlet.annotation.*;  
- **import** java.io.*;  
- @WebServlet(name="AsyncServlet",urlPatterns={"/AsyncServlet"},asyncSupported=**true**)  
- **public****class** AsyncServlet **extends** HttpServlet{  
- **public****void** service(HttpServletRequest request,HttpServletResponse response)**throws** ServletException,IOException{    
-               request.setCharacterEncoding("GBK");  
-               response.setContentType("text/html;charset=GBK");  
-               PrintWriter out = response.getWriter();  
-               out.println("<html>");  
-               out.println("<body>");  
-               out.println("====页面开始====<hr />");  
-               AsyncContext actx = request.startAsync();  
-               actx.setTimeout(30*3000);  
-               actx.start(**new** MyThread(actx));  
-               out.println("====页面结束====<hr />");  
-               out.println("</body>");  
-               out.println("</html>");  
-               out.flush();  
-        }  
- }  
- **class** MyThread **implements** Runnable{  
- **private** AsyncContext actx;  
- **public** MyThread(AsyncContext actx){  
- **this**.actx = actx;  
-        }  
- **public****void** run(){  
- **try**{  
-                    Thread.sleep(5*1000); //消耗5秒
-                     actx.dispatch("/1.jsp");  
-               }  
- **catch**(Exception e){}  
-        }  
- }  
1.jsp
**[html]**[view plain](http://blog.csdn.net/xiazdong/article/details/7208316#)[copy](http://blog.csdn.net/xiazdong/article/details/7208316#)
- **<**%@ page contentType="text/html;charset=GBK"pageEncoding="GBK"session="false"%**>**
- **<****html****>**
- **<****body****>**
- **<**%  
-               out.println("======复杂业务方法====");  
-        %**>**
- **</****body****>**
- **</****html****>**
# 四、异步监听器
异步监听器用来监听异步处理事件；即“三”中讲到的知识点；
此监听器类似于ServletContextListener的机制；
只需要实现AsyncListener接口即可；
此接口有4个方法：
public void onStartAsync(AsyncEvent event)throws IOException;
public void onComplete(AsyncEvent event);
public void onTimeout(AsyncEvent event);
public void onError(AsyncEvent event);
以下是监听器实现的代码：
**[java]**[view plain](http://blog.csdn.net/xiazdong/article/details/7208316#)[copy](http://blog.csdn.net/xiazdong/article/details/7208316#)
- **package** org.listener;  
- **import** javax.servlet.annotation.*;  
- **import** javax.servlet.http.*;  
- **import** javax.servlet.*;  
- **import** java.util.*;  
- **import** java.sql.*;  
- **import** javax.naming.*;  
- **import** java.io.*;  
- **public****class** MyListener **implements** AsyncListener{  
- **public****void** onStartAsync(AsyncEvent event)**throws** IOException{}  
- **public****void** onComplete(AsyncEvent event){  
-               System.out.println("-----------------------Complete");  
-       }  
- **public****void** onTimeout(AsyncEvent event){  
-        }  
- **public****void** onError(AsyncEvent event){}  
- }  
在Servlet异步处理处添加：
actx.addListener(new MyListener())；就可以添加监听器，每当异步处理完成时就会触发onComplete()事件，输出Complete；
# 五、文件上传改进API
原本文件上传时通过 common-fileupload或者SmartUpload，上传比较麻烦，在Servlet 3.0 中不需要导入任何第三方jar包，并且提供了很方便进行文件上传的功能；
注意点：
1. html中 <input type="file">表示文件上传控件；
2. form的 enctype="multipart/form-data"；
3.在Servlet类前加上 @MultipartConfig
4.request.getPart()获得；
下面是一个文件上传的例子：
upload.html
**[html]**[view plain](http://blog.csdn.net/xiazdong/article/details/7208316#)[copy](http://blog.csdn.net/xiazdong/article/details/7208316#)
- **<****html****>**
- **<****body****>**
- **<****form**method="post"enctype="multipart/form-data"action="upload"**>**
- **<****input**type="file"id="file"name="file"**/>**
- **<****input**type="text"id="name"name="name"**/>**
- **<****input**type="submit"value="提交"**/>**
- **</****form****>**
- **</****body****>**
- **</****html****>**
UploadServlet.java
**[java]**[view plain](http://blog.csdn.net/xiazdong/article/details/7208316#)[copy](http://blog.csdn.net/xiazdong/article/details/7208316#)
- **package** org.servlet;  
- **import** java.io.*;  
- **import** javax.servlet.*;  
- **import** javax.servlet.http.*;  
- **import** javax.servlet.annotation.*;  
- 
- @WebServlet(name="UploadServlet" ,urlPatterns={"/upload"})  
- @MultipartConfig
- **public****class** UploadServlet **extends** HttpServlet{  
- **public****void** init(ServletConfig config)**throws** ServletException{  
- **super**.init(config);  
-        }  
- **public****void** service(HttpServletRequest request,HttpServletResponse response)**throws** ServletException,IOException{  
-               Part part = request.getPart("file");  
-               PrintWriter out = response.getWriter();  
-               out.println("此文件的大小："+part.getSize()+"<br />");  
-               out.println("此文件类型："+part.getContentType()+"<br />");  
-               out.println("文本框内容："+request.getParameter("name")+"<br />");  
-               out.println(UploadUtil.getFileName(part)+"<br />");  
-               part.write("F:\\1."+UploadUtil.getFileType(part));  
-        }  
- }  
UploadUtil.java
由于在Servlet 3.0中很难获取上传文件的类型，因此我写了两个工具类，可以方便开发；
**[java]**[view plain](http://blog.csdn.net/xiazdong/article/details/7208316#)[copy](http://blog.csdn.net/xiazdong/article/details/7208316#)
- /**
-  * 此工具类只适用于Servlet 3.0
-  * 为了弥补 Servlet 3.0 文件上传时获取文件类型的困难问题
-  * 
-  * @author xiazdong
-  */
- **import** javax.servlet.http.*;  
- **public****class** UploadUtil{  
- **public****static** String getFileType(Part p){  
-               String name = p.getHeader("content-disposition");  
-               String fileNameTmp = name.substring(name.indexOf("filename=")+10);  
-               String type = fileNameTmp.substring(fileNameTmp.indexOf(".")+1,fileNameTmp.indexOf("\""));  
- **return** type;  
-        }  
- **public****static** String getFileName(Part p){  
-               String name = p.getHeader("content-disposition");  
-               String fileNameTmp = name.substring(name.indexOf("filename=")+10);  
-               String fileName = fileNameTmp.substring(0,fileNameTmp.indexOf("\""));  
- **return** fileName;  
-        }  
- }  
- 
- 
- 
![](http://static.blog.csdn.net/images/save_snippets.png)
