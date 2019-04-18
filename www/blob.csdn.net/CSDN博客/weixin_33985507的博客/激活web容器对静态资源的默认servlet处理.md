# 激活web容器对静态资源的默认servlet处理 - weixin_33985507的博客 - CSDN博客
2014年05月04日 16:17:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
在某些servlet的url匹配模式使用/时会拦截一些静态的资源的请求导致无法正确访问，可以采取web容器默认的servlet来处理，当然那些mvc一般也都提供了处理的方法，用何种方式可以自行决定，这里只介绍web容器的默认servlet的处理。
注意：要写在MVC框架的servlet的前面
tomcat:
```
<servlet-mapping> 
    <servlet-name>default</servlet-name>
    <url-pattern>*.jpg</url-pattern>   
</servlet-mapping>  
<servlet-mapping>     
    <servlet-name>default</servlet-name>  
    <url-pattern>*.js</url-pattern>  
</servlet-mapping>  
<servlet-mapping>      
    <servlet-name>default</servlet-name>     
    <url-pattern>*.css</url-pattern>    
</servlet-mapping>  
要配置多个，每种文件配置一个
```
Tomcat, Jetty, JBoss, and GlassFish 自带的默认Servlet的名字 -- "default"
*Google App Engine 自带的默认Servlet的名字 -- "_ah_default"Resin 自带的默认Servlet的名字 -- "resin-file"WebLogic自带的默认Servlet的名字  -- "FileServlet"WebSphere  自带的默认Servlet的名字 -- "SimpleFileServlet" *
