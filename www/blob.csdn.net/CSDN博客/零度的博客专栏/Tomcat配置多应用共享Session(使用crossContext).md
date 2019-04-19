# Tomcat配置多应用共享Session(使用crossContext) - 零度的博客专栏 - CSDN博客
2016年05月12日 15:29:25[零度anngle](https://me.csdn.net/zmx729618)阅读数：2038
                
1. 新建两个应用：
> 
test1
    |------WEB-INF
    |              |------web.xml
    |------test.jsp
test.jsp代码
**[java]**[view
 plain](http://blog.csdn.net/lip009/article/details/7952239#)[copy](http://blog.csdn.net/lip009/article/details/7952239#)
- <%  
-     session.setAttribute("test1Session","test1Session");  
-     session.getServletContext().setAttribute("t1_session",session);  
-     out.println("Application /test1 is ok!<br>");  
- 
- if(session.getServletContext().getContext("/test2")!=null){  
-         HttpSession t2_session = (HttpSession)session.getServletContext().getContext("/test2").getAttribute("t2_session");  
- if(t2_session!=null){  
-             String str = (String)t2_session.getAttribute("test2Session");  
-             String path = request.getContextPath();  
-             out.println("Application "+path +":"+str+"<br>");  
-         }else{  
-             out.println("Application /test2 no data!");  
-         }  
-     }  
- %>  
test2
    |------WEB-INF
    |              |------web.xml
    |------test.jsp
> 
test.jsp代码
**[java]**[view
 plain](http://blog.csdn.net/lip009/article/details/7952239#)[copy](http://blog.csdn.net/lip009/article/details/7952239#)
- <%  
-     session.setAttribute("test2Session","test2Session");  
-     session.getServletContext().setAttribute("t2_session",session);  
-     out.println("Application /test2 is ok!<br>");  
- 
- if(session.getServletContext().getContext("/test1")!=null){  
-         HttpSession t1_session = (HttpSession)session.getServletContext().getContext("/test1").getAttribute("t1_session");  
- if(t1_session!=null){  
-             String str = (String)t1_session.getAttribute("test1Session");  
-             String path = request.getContextPath();  
-             out.println("Application "+path +":"+str+"<br>");  
-         }else{  
-             out.println("Application /test1 no data!");  
-         }  
-     }  
- %>  
2. 配置tomcat，设置crossContext = true，让两个应用可以在tomcat中交叉使用上下文环境。
         在<Host></Host>结点中添加：
> 
<Context path="/test1" docBase="C:/test1" reloadable="true" crossContext="true"></Context>
<Context path="/test2" docBase="C:/test2" reloadable="true" crossContext="true"></Context>
3. 启动Tomcat，访问http://localhost:8080/test1/test.jsp
> 
页面输出：Application /test1 is ok!------------------------test1存储session成功
                    Application /test2 no data!--------------------test2还没有被访问，所以test2的session中没有数据
访问http://localhost:8080/test2/test.jsp
页面输出：Application /test2 is ok!------------------------test2存储session成功
                    Application /test2:test1Session-------------test1Session来自test1应用中session
刷新http://localhost:8080/test1/test.jsp
页面输出：Application /test1 is ok!------------------------test1存储session成功
                    Application /test1:test2Session-------------test2Session来自test2应用中session
应用test1和test2成功共享session，可以互相访问另一个应用中的session和session中的数据。
