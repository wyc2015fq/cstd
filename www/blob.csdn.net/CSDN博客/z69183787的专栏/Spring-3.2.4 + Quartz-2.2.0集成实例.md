# Spring-3.2.4 + Quartz-2.2.0集成实例 - z69183787的专栏 - CSDN博客
2013年12月08日 18:51:18[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3399
Spring3.0不支持Quartz2.0,因为org.quartz.CronTrigger在2.0从class变成了一个interface造成IncompatibleClassChangeError错误:
Caused by: java.lang.IncompatibleClassChangeError: class org.springframework.scheduling.quartz.CronTriggerBean has interface org.quartz.CronTrigger as super class
今天刚下载了Spring3.2.4,发现这个新版本已经支持quartz2.x了.以前的Spring版本只支持Quartz-1.8.x及以前的版本,做个小实例分享一下.
注：Spring3.2.4配置文件中使用CronTriggerFactoryBean来集成quartz2.x,使用CronTriggerBean来集成quartz1.8.x及以前版本.
代码结构及引用jar见下图:
![](https://img-blog.csdn.net/20130916182318656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGtfYmxvZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
applicationContext.xml:
**[html]**[view
 plain](http://blog.csdn.net/lk_blog/article/details/11744621#)[copy](http://blog.csdn.net/lk_blog/article/details/11744621#)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <beansxmlns="http://www.springframework.org/schema/beans"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"xmlns:p="http://www.springframework.org/schema/p"
- xsi:schemaLocation="http://www.springframework.org/schema/beans  
-      http://www.springframework.org/schema/beans/spring-beans-3.0.xsd">
- 
- <!-- 启动触发器的配置开始 -->
- <beanname="startQuertz"lazy-init="false"autowire="no"
- class="org.springframework.scheduling.quartz.SchedulerFactoryBean">
- <propertyname="triggers">
- <list>
- <refbean="myJobTrigger"/>
- </list>
- </property>
- </bean>
- <!-- 启动触发器的配置结束 -->
- 
- <!-- 调度的配置开始 -->
-     <!--  
-         quartz-1.8以前的配置   
- <beanid="myJobTrigger"
- class="org.springframework.scheduling.quartz.CronTriggerBean">
- <propertyname="jobDetail">
- <refbean="myJobDetail"/>
- </property>
- <propertyname="cronExpression">
- <value>0/1 * * * * ?</value>
- </property>
- </bean>
-     -->
- <!-- quartz-2.x的配置 -->
- <beanid="myJobTrigger"
- class="org.springframework.scheduling.quartz.CronTriggerFactoryBean">
- <propertyname="jobDetail">
- <refbean="myJobDetail"/>
- </property>
- <propertyname="cronExpression">
- <value>0/1 * * * * ?</value>
- </property>
- </bean>
- <!-- 调度的配置结束 -->
- 
- <!-- job的配置开始 -->
- <beanid="myJobDetail"
- class="org.springframework.scheduling.quartz.MethodInvokingJobDetailFactoryBean">
- <propertyname="targetObject">
- <refbean="myJob"/>
- </property>
- <propertyname="targetMethod">
- <value>work</value>
- </property>
- </bean>
- <!-- job的配置结束 -->
- 
- <!-- 工作的bean -->
- <beanid="myJob"class="com.tgb.lk.demo.quartz.MyJob"/>
- 
- </beans>
**MyJob.java:**
**[java]**[view
 plain](http://blog.csdn.net/lk_blog/article/details/11744621#)[copy](http://blog.csdn.net/lk_blog/article/details/11744621#)
- import java.util.Date;  
- 
- publicclass MyJob {  
- 
- publicvoid work() {  
-         System.out.println("date:" + new Date().toString());  
-     }  
- }  
**web.xml:**
**[html]**[view
 plain](http://blog.csdn.net/lk_blog/article/details/11744621#)[copy](http://blog.csdn.net/lk_blog/article/details/11744621#)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <web-appversion="2.5"xmlns="http://java.sun.com/xml/ns/javaee"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="http://java.sun.com/xml/ns/javaee   
-     http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd">
- 
- <!-- Spring config start -->
- <context-param>
- <param-name>contextConfigLocation</param-name>
- <param-value>/WEB-INF/classes/applicationContext.xml</param-value>
- </context-param>
- 
- <listener>
- <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
- </listener>
- <!-- Spring config end -->
- 
- <welcome-file-list>
- <welcome-file>index.jsp</welcome-file>
- </welcome-file-list>
- </web-app>
代码下载地址:[http://download.csdn.net/detail/lk_blog/6277021](http://download.csdn.net/detail/lk_blog/6277021)
