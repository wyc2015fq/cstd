# spring DelegatingFilterProxy  过滤器 的原理及运用 - z69183787的专栏 - CSDN博客
2014年04月08日 11:36:36[OkidoGreen](https://me.csdn.net/z69183787)阅读数：17864
DelegatingFilterProxy的原理及使用
DelegatingFilterProxy就是一个对于servlet filter的代理，用这个类的好处主要是通过Spring容器来管理servlet filter的生命周期，还有就是如果filter中需要一些Spring容器的实例，可以通过spring直接注入，另外读取一些配置文件这些便利的操作都可以通过Spring来配置实现。
DelegatingFilterProxy的使用方法，
首先在web.xml中配置：
```
<filter>
< filter-name>myFilter</filter-name>
< filter-class>org.springframework.web.filter.DelegatingFilterProxy</filter-class>
</filter>
<filter-mapping>
< filter-name>myFilter</filter-name>
< url-pattern>/*</url-pattern>
</filter-mapping>
```
然后在Spring的配置文件中，配置具体的Filter类的实例。
**<bean name="myFilter" class="com.*.MyFilter"></bean>**
在Spring中配置的bean的name要和web.xml中的<filter-name>一样
或者在DelegatingFilterProxy的filter配置中配置初始参数：targetBeanName，对应到Spring配置中的beanname
如果要保留Filter原有的init，destroy方法的调用，还需要配置初始化参数targetFilterLifecycle为true，该参数默认为false
```
<filter>
        <filter-name>DelegatingFilterProxy</filter-name>
        <filter-class>org.springframework.web.filter.DelegatingFilterProxy</filter-class>
    	<init-param>
      		<param-name>ignoreUrls</param-name>
      		<param-value>/caClient.jsp,/403.jsp,/login.html*,/j_spring_security_check,/accounts/**,/secure/logout,/js/**,/app/**,/images/**,/icons/**,/lib/**,/resource/**,/secure/changepassword</param-value>
    	</init-param>        
        <init-param>
            <param-name>targetBeanName</param-name>
            <param-value>commonWebFilter</param-value>
        </init-param>
        <init-param>
            <param-name>targetFilterLifecycle</param-name>
            <param-value>true</param-value>
        </init-param>
    </filter>
    <filter-mapping>
        <filter-name>DelegatingFilterProxy</filter-name>
        <url-pattern>/*</url-pattern>
    </filter-mapping>
```
applicationContext.xml
`<bean id="commonWebFilter" class="com.wonders.LoginFilter"/>`
