# Spring Security简明实践及相关国际化处理 - =朝晖= - 博客园
# [Spring Security简明实践及相关国际化处理](https://www.cnblogs.com/dhcn/p/7100160.html)
     别人的都是最佳实践，因为我目前的设置没有按照参考文档推荐，还是采用DelegatingFilterProxy，所以我只能说简明实践。先贴我的applicationContext-security.xml
```
<?xml version="1.0" encoding="UTF-8"?>  
  
<beans:beans xmlns="http://www.springframework.org/schema/security"  
    xmlns:beans="http://www.springframework.org/schema/beans"  
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"  
    xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/<a href="http://lib.csdn.net/base/javaee" class='replace_word' title="Java EE知识库" target='_blank' style='color:#df3434; font-weight:bold;'>spring</a>-beans-2.0.xsd  
                        http://www.springframework.org/schema/security http://www.springframework.org/schema/security/spring-security-2.0.4.xsd">  
    <global-method-security secured-annotations="enabled">  
    </global-method-security>  
    <http auto-config="true">  
      
        <!-- intercept-url pattern="/**" filters="none" /-->  
        <intercept-url pattern="/login.jsp*" filters="none"/>  
        <intercept-url pattern="/common/pages/**" access="IS_AUTHENTICATED_REMEMBERED" />  
        <intercept-url pattern="/common/**" filters="none" />  
        <intercept-url pattern="/secure/extreme/**" access="ROLE_SUPERVISOR"/>  
        <intercept-url pattern="/secure/**" access="IS_AUTHENTICATED_REMEMBERED" />         
        <intercept-url pattern="/**" access="IS_AUTHENTICATED_REMEMBERED" />  
          
        <form-login login-page='/login.jsp' authentication-failure-url="/login.jsp?login_error=1" default-target-url='/index.jsp' />  
      
    </http>  
    <authentication-provider>  
        <jdbc-user-service data-source-ref="dataSource"   
        group-authorities-by-username-query="select U.username,G.group_name,GA.authority  as 'authority' from users U join group_members  GM on U.username=GM.username join groups G on GM.group_id=G.id join group_authorities GA on G.id=GA.group_id where  U.username=?"  
        />  
          
        <password-encoder hash="plaintext"/>  
    </authentication-provider>  
</beans:beans>
```
  对上面的配置说明一下,form-login的authentication-failure-url和default-target-url属性设置基本就可以免去使用ExceptionTranslationFilter的麻烦，authentication-provider使用上面这个配置方式是最方便可用的，至于因为我的[数据库](http://lib.csdn.net/base/mysql)建立在sql Server上，所以添加了group-authorities-by-username-query属性，password-encoder我个人建议采用参考文档上推荐的以username做salt的sha编码，我这儿使用plaintext，是因为我这个项目目前还在开发[测试](http://lib.csdn.net/base/softwaretest)阶段，用户管理这个模块还没有完全建立，所以为了测试的方便，用了plaintext的编码。那个dataSource，我是按照API文档推荐，建立了一个容器JNDI，然后在spring里用JndiObjectFactoryBean代理了一下。
       用户管理的设施代码采用以JdbcUserDetailsManager为主，jdbcTemplate为辅的方式，其中前者不要建立在springContext下面，否则会导致多userDetailService冲突。
       下面说一下spring Security国际化(i18N)的配置处理，先贴代码:
```
<bean id="messageSource"  
        class="org.springframework.context.support.ReloadableResourceBundleMessageSource">  
        <property name="basename" value="/WEB-INF/ssm"/>  
    </bean>  
    <bean id="localeResolver" class="org.springframework.web.servlet.i18n.AcceptHeaderLocaleResolver"/>
```
    注意了：我上面的bean定义不在applicationContext-security.xml里面，而是在一个以beans作为默认命名空间的applicationContext-beans.xml里面,我说一下上面这个配置做的时候出现的问题，其中的basename的value，我刚开始采用开发指南上说的"org/springframework/security/messages",可是始终找不到jar包里面的messages文件，后来，我看了一个老外的同问题贴，他采用上面这种方式，把messages文件放在了/WEB-INF/下面，反正这种配置方式下，messages文件终于可以找到，i18N可以正常运行了，至于那个messages文件，还是从jar包里面解压出来的东西。

