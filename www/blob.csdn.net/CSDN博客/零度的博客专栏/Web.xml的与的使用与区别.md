# Web.xml的与的使用与区别 - 零度的博客专栏 - CSDN博客
2016年05月13日 15:19:16[零度anngle](https://me.csdn.net/zmx729618)阅读数：1704
web.xml的配置中<context-param>配置作用：
1.启动一个WEB项目的时候,容器(如:Tomcat)会去读它的配置文件web.xml.读两个节点: <listener></listener> 和 <context-param></context-param>
2.紧接着,容器创建一个ServletContext(上下文),这个WEB项目所有部分都将共享这个上下文.
3.容器将<context-param></context-param>转化为键值对,并交给ServletContext.
4.容器创建<listener></listener>中的类实例,即创建监听.
5.在监听中会有contextInitialized(ServletContextEvent args)初始化方法,在这个方法中获得ServletContext
 = ServletContextEvent.getServletContext();context-param的值 = ServletContext.getInitParameter("context-param的键");
6.得到这个context-param的值之后,你就可以做一些操作了.注意,这个时候你的WEB项目还没有完全启动完成.这个动作会比所有的Servlet都要早.换句话说,这个时候,你对<context-param>中的键值做的操作,将在你的WEB项目完全启动之前被执行.
7.举例.你可能想在项目启动之前就打开数据库.那么这里就可以在<context-param>中设置数据库的连接方式,在监听类中初始化数据库的连接.
8.这个监听是自己写的一个类,除了初始化方法,它还有销毁方法.用于关闭应用前释放资源.比如说数据库连接的关闭.
举例说明:
```
<!-- 加载spring的配置文件 -->
<context-param>
    <param-name>contextConfigLocation</param-name>
    <param-value>/WEB-INF/applicationContext.xml,/WEB-INF/action-servlet.xml,/WEB-INF/jason-servlet.xml</param-value>
</context-param>
<listener>
    <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
</listener>
```
自定义Listener
```
public class SysListener extends HttpServlet implements ServletContextListener {
   private static final Log logger = LogFactory.getLog(SysListener.class);
   //用于在容器关闭时,操作
   public void contextDestroyed(ServletContextEvent sce) {   
   }
   //用于在容器开启时,操作
   public void contextInitialized(ServletContextEvent sce) {
       String rootpath = sce.getServletContext().getRealPath("/");
       System.out.println("-------------rootPath:"+rootpath);   
       if (rootpath != null) {
          rootpath = rootpath.replaceAll("\\\\", "/");
       } else {
          rootpath = "/";
       }
       if (!rootpath.endsWith("/")) {
            rootpath = rootpath + "/";
       }
       Constant.ROOTPATH = rootpath;
       logger.info("Application Run Path:" + rootpath);
       String urlrewrtie = sce.getServletContext().getInitParameter("urlrewrite");
       boolean burlrewrtie = false;
       if (urlrewrtie != null) {
           burlrewrtie = Boolean.parseBoolean(urlrewrtie);
       }
       Constant.USE_URL_REWRITE = burlrewrtie;
       logger.info("Use Urlrewrite:" + burlrewrtie);
       其它略之....    
   }
}
```
```
/*最终输出
   -------------rootPath:D:\tomcat_bbs\webapps\BBSCS_8_0_3\
   2009-06-09 21:51:46,526 [com.laoer.bbscs.web.servlet.SysListener]-[INFO]Application Run Path:D:/tomcat_bbs/webapps/BBSCS_8_0
   2009-06-09 21:51:46,526 [com.laoer.bbscs.web.servlet.SysListener]-[INFO]Use Urlrewrite:true
   2009-06-09 21:51:46,526 [com.laoer.bbscs.web.servlet.SysListener]-[INFO]Use Cluster:false
   2009-06-09 21:51:46,526 [com.laoer.bbscs.web.servlet.SysListener]-[INFO]SERVLET MAPPING:*.bbscs
   2009-06-09 21:51:46,573 [com.laoer.bbscs.web.servlet.SysListener]-[INFO]Post Storage Mode:1
   */
```
context-param和init-param区别
web.xml里面可以定义两种参数：
(1)application范围内的参数，存放在servletcontext中，在web.xml中配置如下：
**[html]**[view
 plain](http://blog.csdn.net/sccemstanford/article/details/9039409#)[copy](http://blog.csdn.net/sccemstanford/article/details/9039409#)
- <context-param>
- <param-name>context/param</param-name>
- <param-value>avalible during application</param-value>
- </context-param>
(2)servlet范围内的参数，只能在servlet的init()方法中取得，在web.xml中配置如下：
**[html]**[view
 plain](http://blog.csdn.net/sccemstanford/article/details/9039409#)[copy](http://blog.csdn.net/sccemstanford/article/details/9039409#)
- <servlet>
- <servlet-name>MainServlet</servlet-name>
- <servlet-class>com.wes.controller.MainServlet</servlet-class>
- <init-param>
- <param-name>param1</param-name>
- <param-value>avalible in servlet init()</param-value>
- </init-param>
- <load-on-startup>0</load-on-startup>
- </servlet>
在servlet中可以通过代码分别取用：
**[java]**[view
 plain](http://blog.csdn.net/sccemstanford/article/details/9039409#)[copy](http://blog.csdn.net/sccemstanford/article/details/9039409#)
- package com.wes.controller;import javax.servlet.ServletException;  
- import javax.servlet.http.HttpServlet;publicclass MainServlet extends HttpServlet {   
- public MainServlet() {  
- super();  
-     }  
- publicvoid init() throws ServletException ...{  
-          System.out.println("下面的两个参数param1是在servlet中存放的");  
-          System.out.println(this.getInitParameter("param1"));  
-          System.out.println("下面的参数是存放在servletcontext中的");  
-          System.out.println(getServletContext().getInitParameter("context/param"));  
-       }  
- }  
第一种参数在servlet里面可以通过getServletContext().getInitParameter("context/param")得到
第二种参数只能在servlet的init()方法中通过this.getInitParameter("param1")取得.
