# spring+springMVC,声明式事务失效，原因以及解决办法 - z69183787的专栏 - CSDN博客
2014年07月15日 17:01:48[OkidoGreen](https://me.csdn.net/z69183787)阅读数：17560
一.声明式事务配置：
**[html]**[view
 plain](http://blog.csdn.net/yu412346928/article/details/24810635#)[copy](http://blog.csdn.net/yu412346928/article/details/24810635#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/321340/fork)
- 
**[html]**[view
 plain](http://blog.csdn.net/yu412346928/article/details/24810635#)[copy](http://blog.csdn.net/yu412346928/article/details/24810635#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/321340/fork)
- <beanid="transactionManager"class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
- <propertyname="dataSource"ref="dataSource"/>
- </bean>
- <tx:adviceid="txAdvice"transaction-manager="transactionManager">
- <tx:attributes>
- <tx:methodname="add*"propagation="REQUIRED"read-only="false"/>
- <tx:methodname="del*"propagation="REQUIRED"read-only="false"/>
- <tx:methodname="get*"propagation="REQUIRED"read-only="true"/>
- <tx:methodname="mod*"propagation="REQUIRED"read-only="false"/>
- </tx:attributes>
- </tx:advice>
- <aop:config>
- <aop:pointcutid="serviceMethods"expression="execution(public * com.lexing.platform.service.*.*(..))"/>
- <aop:advisoradvice-ref="txAdvice"pointcut-ref="serviceMethods"/>
- </aop:config>
二.声明式事务失效,原因
根本原因：由子容器扫描装配了@Service 注解的实例。
spring的context是父子容器，由ServletContextListener 加载spring配置文件产生的是父容器，springMVC加载配置文件产生的是子容器，子容器对Controller进行扫描装配时装配了@Service注解的实例 (@Controller 实例依赖@Service实例)，而该实例理应由父容器进行初始化以保证事务的增强处理，所以此时得到的将是原样的Service（没有经过事务加强处理，故而没有事务处理能力。
三.解决办法
1.spring配置文件applicationContext中:
**[html]**[view
 plain](http://blog.csdn.net/yu412346928/article/details/24810635#)[copy](http://blog.csdn.net/yu412346928/article/details/24810635#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/321340/fork)
- <!-- 不扫描带有@Controller注解的类 ,让 springMVC 子容器加载。  
**[html]**[view
 plain](http://blog.csdn.net/yu412346928/article/details/24810635#)[copy](http://blog.csdn.net/yu412346928/article/details/24810635#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/321340/fork)
- <context:component-scanbase-package="com.lexing.platform">
- <context:exclude-filtertype="annotation"expression="org.springframework.stereotype.Controller"/>
- </context:component-scan>
2.springMVC配置文件 servlet-context.xml中
**[html]**[view
 plain](http://blog.csdn.net/yu412346928/article/details/24810635#)[copy](http://blog.csdn.net/yu412346928/article/details/24810635#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/321340/fork)
- <!-- 将 带有 @Service注解的类,交由spring 父容器实例化,[ @Service实例依赖@Repository实例，故spring父容器也会装配<spanstyle="font-family: Arial, Helvetica, sans-serif;">@Repository 实例</span><spanstyle="font-family: Arial, Helvetica, sans-serif;">]</span>
**[html]**[view
 plain](http://blog.csdn.net/yu412346928/article/details/24810635#)[copy](http://blog.csdn.net/yu412346928/article/details/24810635#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/321340/fork)
- <context:component-scanbase-package="com.lexing.platform">
- <context:exclude-filtertype="annotation"expression="org.springframework.stereotype.Service"/>
- </context:component-scan>
- 
