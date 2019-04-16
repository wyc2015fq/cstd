# Servlet3.0学习总结(四)——使用注解标注监听器(Listener) - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [Servlet3.0学习总结(四)——使用注解标注监听器(Listener)](https://www.cnblogs.com/xdp-gacl/p/4226851.html)



**　　Servlet3.0提供@WebListener注解将一个实现了特定监听器接口的类定义为监听器**，这样我们在web应用中使用监听器时，也不再需要在web.xml文件中配置监听器的相关描述信息了。

　　下面我们来创建一个监听器，体验一下使用@WebListener注解标注监听器，如下所示：

![](https://images0.cnblogs.com/blog/289233/201501/151718041209457.png)

监听器的代码如下：

```
1 package me.gacl.web.listener;
 2 
 3 import javax.servlet.ServletContextEvent;
 4 import javax.servlet.ServletContextListener;
 5 import javax.servlet.annotation.WebListener;
 6 
 7 /**
 8  * 使用@WebListener注解将实现了ServletContextListener接口的MyServletContextListener标注为监听器
 9  */
10 @WebListener
11 public class MyServletContextListener implements ServletContextListener {
12 
13     @Override
14     public void contextDestroyed(ServletContextEvent sce) {
15         System.out.println("ServletContex销毁");
16     }
17 
18     @Override
19     public void contextInitialized(ServletContextEvent sce) {
20         System.out.println("ServletContex初始化");
21         System.out.println(sce.getServletContext().getServerInfo());
22     }
23 }
```

　　Web应用启动时就会初始化这个监听器，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201501/151726394959941.png)

　　有了@WebListener注解之后，我们的web.xml就无需任何配置了

```
1 <?xml version="1.0" encoding="UTF-8"?>
 2 <web-app version="3.0" 
 3     xmlns="http://java.sun.com/xml/ns/javaee" 
 4     xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
 5     xsi:schemaLocation="http://java.sun.com/xml/ns/javaee 
 6     http://java.sun.com/xml/ns/javaee/web-app_3_0.xsd">
 7   <display-name></display-name>    
 8   <welcome-file-list>
 9     <welcome-file>index.jsp</welcome-file>
10   </welcome-file-list>
11 </web-app>
```

　　Servlet3.0规范的出现，让我们开发Servlet、Filter和Listener的程序在web.xml实现零配置。









