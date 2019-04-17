# No Hibernate Session bound to thread, and configuration does not allow create解决方案 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年12月10日 15:48:41[boonya](https://me.csdn.net/boonya)阅读数：3970








参考地址：[http://forum.spring.io/forum/spring-projects/data/96622-no-hibernate-session-bound-to-thread-and-configuration-does-not-allow-creation-of-no](http://forum.spring.io/forum/spring-projects/data/96622-no-hibernate-session-bound-to-thread-and-configuration-does-not-allow-creation-of-no)

**1、使用注解实现程序**

 dao                           @Repository

service                      @Service        @Transactional

controller                 @Controller     @RequestMapping(value="/")

**2、配置Spring 3 + Hibernate 3 （MVC）**

启动正常，请求数据库错误，后台报No Hibernate Session bound to thread, and configuration does not allow create错误。

但是事务是配置了的，但是貌似是事务的问题，多方寻求无果，最后在spring网站上找到了解决方案。

**3、解决方案在web.xml中加入Hibernate的过滤请求将session打开**



```
<filter>
    <filter-name>openSessionInViewFilter</filter-name>
    <filter-class>
        org.springframework.orm.hibernate3.support.OpenSessionInViewFilter
    </filter-class>
</filter>

<filter-mapping>
    <filter-name>openSessionInViewFilter</filter-name>
    <url-pattern>/*</url-pattern>
</filter-mapping>
```
**有关OpenSessionInViewFilter的参考**：[http://docs.spring.io/spring/docs/3.0.x/javadoc-api/org/springframework/orm/hibernate3/support/OpenSessionInViewFilter.html](http://docs.spring.io/spring/docs/3.0.x/javadoc-api/org/springframework/orm/hibernate3/support/OpenSessionInViewFilter.html)






