# Spring MVC @Transactional注解方式事务失效的解决办法 - z69183787的专栏 - CSDN博客
2014年07月15日 17:14:10[OkidoGreen](https://me.csdn.net/z69183787)阅读数：66197
前文提到，最新换了框架，新项目用SpringMVC + Spring JdbcTemplate。搭框架时，发现了一个事务无法正常回滚的问题，记录如下：
首先展示问题：
Spring applicationContext.xml配置：
**[html]**[view
 plain](http://blog.csdn.net/will_awoke/article/details/12002705#)[copy](http://blog.csdn.net/will_awoke/article/details/12002705#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/365899/fork)
- 
- <beanid="dataSource"class="org.springframework.jndi.JndiObjectFactoryBean">
- <propertyname="jndiName">
- <value>java:comp/env/jdbc/will</value>
- </property>
- </bean>
- 
- <beanid="jdbcTemplate"class="org.springframework.jdbc.core.JdbcTemplate">
- <propertyname="dataSource"ref="dataSource"/>
- </bean>
- 
- <beanid="txManager"
- class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
- <propertyname="dataSource"ref="dataSource"/>
- </bean>
- 
- <!-- 事务控制   -->
- <tx:annotation-driventransaction-manager="txManager"/>
Spring mvc.dispatcher.xml配置：
**[html]**[view
 plain](http://blog.csdn.net/will_awoke/article/details/12002705#)[copy](http://blog.csdn.net/will_awoke/article/details/12002705#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/365899/fork)
- <!-- 自动扫描的包名 -->
- <context:component-scanbase-package="com.will">
- </context:component-scan>
- 
- <!-- 默认的注解映射的支持 -->
- <mvc:annotation-driven/>
- 
- <!-- 对静态资源文件的访问  -->
- <mvc:default-servlet-handler/>
- 
- 
- <!-- 拦截器    
- <mvc:interceptors>
- <beanclass="com.will.mvc.MyInteceptor"/>
- </mvc:interceptors>
- -->
- 
- <!-- 视图解释类 -->
- <beanid="viewResolver"
- class="org.springframework.web.servlet.view.UrlBasedViewResolver">
- <propertyname="viewClass"value="org.springframework.web.servlet.view.JstlView"/>
- <propertyname="prefix"value="/WEB-INF/pages/"/>
- <propertyname="suffix"value=".jsp"/>
- </bean>
然后在Service层模拟了一个事务回滚的method case：
**[java]**[view
 plain](http://blog.csdn.net/will_awoke/article/details/12002705#)[copy](http://blog.csdn.net/will_awoke/article/details/12002705#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/365899/fork)
- @Transactional
- publicboolean save(Person person)  
- {  
- for(int id: newint[]{2,3})  
-     {  
-         personDao.del(id);  
- int j = 1/0;  
-     }                  
- 
- returnfalse;  
- }  
## 本以为大功告成，在运行save方法时，由于1/0 抛出 java.lang.ArithmeticException: / by zero  ```RuntimeException，导致事务回归。However，no way! So crazy~```
`查了下，发现Spring MVC对于事务配置比较讲究，需要额外的配置。**解决办法如下：**`
需要在 applicationContext.xml增加：
**[html]**[view
 plain](http://blog.csdn.net/will_awoke/article/details/12002705#)[copy](http://blog.csdn.net/will_awoke/article/details/12002705#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/365899/fork)
- <context:component-scanbase-package="com.will">
- <context:exclude-filtertype="annotation"expression="org.springframework.stereotype.Controller"/>
- </context:component-scan>
在 Spring mvc.dispatcher.xml增加：
**[html]**[view
 plain](http://blog.csdn.net/will_awoke/article/details/12002705#)[copy](http://blog.csdn.net/will_awoke/article/details/12002705#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/365899/fork)
- <context:component-scanbase-package="com.will">
- <context:include-filtertype="annotation"expression="org.springframework.stereotype.Controller"/>
- <context:exclude-filtertype="annotation"expression="org.springframework.stereotype.Service"/>
- </context:component-scan>
由于web.xml中配置：
**[html]**[view
 plain](http://blog.csdn.net/will_awoke/article/details/12002705#)[copy](http://blog.csdn.net/will_awoke/article/details/12002705#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/365899/fork)
- <context-param>
- <param-name>contextConfigLocation</param-name>
- <param-value>
-              classpath:applicationContext.xml  
- </param-value>
- </context-param>
- <listener>
- <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
- </listener>
- <servlet>
- <servlet-name>dispatcher</servlet-name>
- <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
- <init-param>
- <param-name>contextConfigLocation</param-name>
- <param-value>classpath*:/mvc_dispatcher_servlet.xml</param-value>
- </init-param>
- <load-on-startup>1</load-on-startup>
- </servlet>
- <servlet-mapping>
- <servlet-name>dispatcher</servlet-name>
- <url-pattern>*.do</url-pattern>
- </servlet-mapping>
Spring容器优先加载由ServletContextListener（对应applicationContext.xml）产生的父容器，而SpringMVC（对应mvc_dispatcher_servlet.xml）产生的是子容器。子容器Controller进行扫描装配时装配的@Service注解的实例是没有经过事务加强处理，即没有事务处理能力的Service，而父容器进行初始化的Service是保证事务的增强处理能力的。如果不在子容器中将Service
 exclude掉，此时得到的将是原样的无事务处理能力的Service。
（ update 2014.05.27  今天看见一种说法：key word =**双亲上下文。**不使用ContextLoaderListener监听器来加载spring的配置，改用DispatcherServlet来加载spring的配置，不要双亲上下文，只使用一个DispatcherServlet就不会出现上述问题。笔者这里未测过这个办法，因为我自己的业务需要一个extends ContextLoaderListener的selfListener，有兴趣的朋友可以自己测试下这个说法，并欢迎把测试的结果与我交流
 ：） ）
经过以上分析，故可以**优化上述配置**：
在 applicationContext.xml增加：
**[html]**[view
 plain](http://blog.csdn.net/will_awoke/article/details/12002705#)[copy](http://blog.csdn.net/will_awoke/article/details/12002705#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/365899/fork)
- <context:component-scanbase-package="com.will">
- </context:component-scan>
在 Spring mvc.dispatcher.xml增加：
**[html]**[view
 plain](http://blog.csdn.net/will_awoke/article/details/12002705#)[copy](http://blog.csdn.net/will_awoke/article/details/12002705#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/365899/fork)
- <context:component-scanbase-package="com.will">
- <context:exclude-filtertype="annotation"expression="org.springframework.stereotype.Service"/>
- </context:component-scan>
经过如上配置，可以发现事务控制部分的日志如下：
**[html]**[view
 plain](http://blog.csdn.net/will_awoke/article/details/12002705#)[copy](http://blog.csdn.net/will_awoke/article/details/12002705#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/365899/fork)
- 2013-09-25 09:53:13,031 [http-8080-2] DEBUG [org.springframework.transaction.annotation.AnnotationTransactionAttributeSource] - Adding transactional method 'save' with attribute: PROPAGATION_REQUIRED,ISOLATION_DEFAULT; ''  
- 2013-09-25 09:53:13,037 [http-8080-2] DEBUG [org.springframework.beans.factory.support.DefaultListableBeanFactory] - Returning cached instance of singleton bean 'txManager'  
- 2013-09-25 09:53:13,050 [http-8080-2] DEBUG [org.springframework.jdbc.datasource.DataSourceTransactionManager] - Creating new transaction with name [com.will.service.impl.PersonServiceImpl.save]: PROPAGATION_REQUIRED,ISOLATION_DEFAULT; ''  
- 2013-09-25 09:53:13,313 [http-8080-2] DEBUG [org.springframework.jdbc.datasource.DataSourceTransactionManager] - Acquired Connection [jdbc:mysql://localhost:3306/mvc?useUnicode=true&characterEncoding=UTF-8, UserName=root@localhost, MySQL-AB JDBC Driver] for JDBC transaction  
- 2013-09-25 09:53:13,323 [http-8080-2] DEBUG [org.springframework.jdbc.datasource.DataSourceTransactionManager] - Switching JDBC Connection [jdbc:mysql://localhost:3306/mvc?useUnicode=true&characterEncoding=UTF-8, UserName=root@localhost, MySQL-AB JDBC Driver] to manual commit  
- 2013-09-25 09:53:13,327 [http-8080-2] DEBUG [org.springframework.jdbc.core.JdbcTemplate] - Executing prepared SQL update  
- 2013-09-25 09:53:13,328 [http-8080-2] DEBUG [org.springframework.jdbc.core.JdbcTemplate] - Executing prepared SQL statement [delete from person where id=?]  
- 2013-09-25 09:53:13,348 [http-8080-2] DEBUG [org.springframework.jdbc.core.JdbcTemplate] - SQL update affected 1 rows  
- 2013-09-25 09:53:13,363 [http-8080-2] DEBUG [org.springframework.jdbc.datasource.DataSourceTransactionManager] - Initiating transaction rollback  
- 2013-09-25 09:53:13,364 [http-8080-2] DEBUG [org.springframework.jdbc.datasource.DataSourceTransactionManager] - Rolling back JDBC transaction on Connection [jdbc:mysql://localhost:3306/mvc?useUnicode=true&characterEncoding=UTF-8, UserName=root@localhost, MySQL-AB JDBC Driver]  
- 2013-09-25 09:53:13,377 [http-8080-2] DEBUG [org.springframework.jdbc.datasource.DataSourceTransactionManager] - Releasing JDBC Connection [jdbc:mysql://localhost:3306/mvc?useUnicode=true&characterEncoding=UTF-8, UserName=root@localhost, MySQL-AB JDBC Driver] after transaction  
- 2013-09-25 09:53:13,378 [http-8080-2] DEBUG [org.springframework.jdbc.datasource.DataSourceUtils] - Returning JDBC Connection to DataSource  
在2013-09-25 09:53:13,363处进行了rollback。
PS：习惯了Structs，对事务处理有点思维定式，这次花费不少时间来解决这个问题。颇为尴尬！
