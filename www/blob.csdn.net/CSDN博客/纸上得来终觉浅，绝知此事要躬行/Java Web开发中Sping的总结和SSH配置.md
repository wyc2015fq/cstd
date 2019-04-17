# Java Web开发中Sping的总结和SSH配置 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2011年09月01日 13:46:31[boonya](https://me.csdn.net/boonya)阅读数：1852







 1、  Spring的诞生
为了解决大型企业级应用的结构复杂，涉及外部资源众多，事务密集、数据规模庞大，用户数量多，又较强的安全性考虑和较高的性能要求。------Rod Jonson (Spring的作者)

2、Spring的优化

Spring属于集成框架，其核心基于控制反转（Inverse of Control 即IOC），Spring是一个全面的解决方案:提倡不重新造轮子。

3、  Spring包含的模块

Core 、AOP、DAO、ORM、JEE、WEB。

4、  Spring的核心机制----依赖注入

所谓的依赖注入就是当某个角色需要另一个角色的协助时，通常由调用者来创建被调用者的实例。在Spring中，创建者调用实例的工作常由Spring容器来完成，然后注入调用者。

例如：在Action中需要使用Service的对象，我们可以通过注入来实现对象的引用。

**private** UserService userService;

**public void** setUserService(UserService userService) {

**this**.userService = userService;

   }

5、  log4j的日志记录-----log4j.properties

》》规范：

#ootLogger是所有日志的根志，修改该日志属性将对所有日志起作用

#有日志的输出级别info,输出源console

#log4j.rootLogger=info,console

#定义输出位置是控制台

log4j.appender.console=org.apache.log4j.ConsoleAppender

#日志布局采用的类

log4j.appender.console.layout=org.apache.log4j.PatternLayout

#日志输出布局

log4j.appender.console.ConversionPattern= [%p] %d{yyyy-MM-dd

HH:mm:ss} %m %l

》》实用：

log4j.rootLogger=DEBUG,console,file

log4j.appender.console=org.apache.log4j.ConsoleAppender

log4j.appender.console.layout=org.apache.log4j.PatternLayout

log4j.appender.console.ConversionPattern= [%p] %d{yyyy-MM-dd HH:mm:ss} %m %l

log4j.appender.file=org.apache.log4j.FileAppender

log4j.appender.file.file=${log4j_root}/logs.file

log4j.appender.file.layout=org.apache.log4j.PatternLayout

log4j.appender.file.layout.ConversionPattern=[%p] %d{yyyy-MM-dd HH:mm:ss} %m %r%n

6、  SSH整合

（1）SSH框架的搭建

步骤：

1、添加集成好的所有Jar包。

2、添加顺序Struts2，Hibernate3，Spring2.5或Spring3.0。

3、分别编写配置web.xml、Hibernate、dao.xml、srvice.xml、action.xml和ApplicationContent.xml

Web.xml文件下的主要配置：

<!-- 配置spring -->

   <context-param>

      <param-name>contextConfigLocation</param-name>

      <param-value>classpath:applicationContext.xml</param-value>

   </context-param>

   <listener>

      <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>

   </listener>

   <listener>

       <listener-class>com.*boonya*.jboa.utils.Log4jListener</listener-class>

   </listener>

   <!-- 配置字符过滤器 -->

   <filter>

      <filter-name>characterFilter</filter-name>

      <filter-class>org.springframework.web.filter.CharacterEncodingFilter</filter-class>

      <init-param>

         <param-name>encoding</param-name>

         <param-value>UTF-8</param-value>

      </init-param>

   </filter>

 <!-- 配置OSIV过滤器 -->

    <filter>

       <filter-name>OSIVFilter</filter-name>

       <filter-class>org.springframework.orm.hibernate3.support.OpenSessionInViewFilter</filter-class>

    </filter>


<!-- 配置Struts2 -->

   <filter>

      <filter-name>struts2</filter-name>

      <filter-class>

         org.apache.struts2.dispatcher.ng.filter.StrutsPrepareAndExecuteFilter</filter-class>

   </filter>

Hibernate.cfg.xml的配置：

<session-factory>

      <property name=*"myeclipse.connection.profile"*>JBOA</property>

      <property name=*"dialect"*>

         org.hibernate.dialect.Oracle9Dialect

      </property>

      <property name=*"show_sql"*>true</property>

      <mapping 
resource=*"com/boonya/jboa/entity/ClaimVoucher.hbm.xml"* />

      <mapping resource=*"com/*boonya*/jboa/entity/CheckResult.hbm.xml"*
 />

      <mapping resource=*"com/*boonya*/jboa/entity/Position.hbm.xml"*
 />

      <mapping resource=*"com/*boonya*/jboa/entity/Department.hbm.xml"*
 />

      <mapping resource=*"com/*boonya*/jboa/entity/ClaimVoucherDetail.hbm.xml"*
 />

      <mapping resource=*"com/*boonya*/jboa/entity/Employee.hbm.xml"*
 />

   </session-factory>

数据库访问属性文件：db.properties

driver=oracle.jdbc.driver.OracleDriver

url=jdbc:oracle:thin:@localhost:1521:orcl

user=jboa

password=jboa

Config文件夹下的dao.xml配置：

<!-- 配置数据源 -->

   <bean id=*"dataSource"* class=*"org.springframework.jdbc.datasource.DriverManagerDataSource"*>

      <property name=*"driverClassName"* value=*"${driver}"*></property>

      <property name=*"url"* value=*"${url}"*></property>

      <property name=*"username"* value=*"${user}"*></property>

      <property name=*"password"* value=*"${password}"*></property>

   </bean>


<!-- 配置session工厂 -->

   <bean id=*"sessionFactory"*

      class=*"org.springframework.orm.hibernate3.LocalSessionFactoryBean"*>

      <property name=*"dataSource"* ref=*"dataSource"*></property>

      <property name=*"configLocation"* value=*"classpath:hibernate.cfg.xml"*>

      </property>

   </bean>

  <!-- 配置dao基类 -->

   <bean id=*"baseDao"* abstract=*"true"* class=*"com.*boonya*.jboa.dao.BaseDao"*>

      <property name=*"sessionFactory"* ref=*"sessionFactory"*></property>

   </bean>

   <!--********************以下为具体的****************** -->

   <bean id=*"claimVoucherDao"* class=*"com.*boonya*.jboa.dao.ClaimVoucherDao"*
 parent=*"baseDao"*></bean>

   <bean id=*"userDao"* class=*"com.*boonya*.jboa.dao.UserDao"*
 parent=*"baseDao"*></bean>

Config文件夹下的Service.xml配置：

xmlns:p=*"http://www.springframework.org/schema/p"*

   xmlns:tx=*"http://www.springframework.org/schema/tx"*

   xmlns:aop=*"http://www.springframework.org/schema/aop"*

   xsi:schemaLocation=*"*

*      http://www.springframework.org/schema/beans*

*      http://www.springframework.org/schema/beans/spring-beans-2.5.xsd*

*      http://www.springframework.org/schema/tx*

*      http://www.springframework.org/schema/tx/spring-tx-2.5.xsd*

*      http://www.springframework.org/schema/aop     *

*      http://www.springframework.org/schema/aop/spring-aop-2.5.xsd*

*   "*>


<!-- 事务配置 -->

   <bean id=*"txManager"*

      class=*"org.springframework.orm.hibernate3.HibernateTransactionManager"*>

      <property name=*"sessionFactory"* ref=*"sessionFactory"*></property>

   </bean>

 <!-- Advice -->

   <tx:advice transaction-manager=*"txManager"* id=*"serviceAdvice"*>

      <tx:attributes>

         <tx:method name=*"*"* propagation=*"REQUIRED"* />

         <tx:method name=*"get*"* read-only=*"true"* />

         <tx:method name=*"find*"* read-only=*"true"* />

      </tx:attributes>

   </tx:advice>


<!-- AOP -->

   <aop:config>

      <aop:pointcut expression=*"execution(* com.*boonya*.jboa.service..*.*(..))"*

         id=*"serviceMethod"* />

      <aop:advisor advice-ref=*"serviceAdvice"* pointcut-ref=*"serviceMethod"* />

   </aop:config>



   <aop:aspectj-autoproxy></aop:aspectj-autoproxy>



   <bean id=*"baseService"* class=*"com.*boonya*.jboa.service.BaseService"*
 abstract=*"true"*></bean>

   <bean id=*"claimVoucherService"* class=*"com.*boonya*.jboa.service.ClaimVoucherService"*
 parent=*"baseService"*>

      <property name=*"claimVoucherDao"* ref=*"claimVoucherDao"*></property>

   </bean>



   <bean id=*"userService"* class=*"com.*boonya*.jboa.service.UserService"*
 parent=*"baseService"*>

      <property name=*"userDao"* ref=*"userDao"*></property>

   </bean>

Config文件夹下的action.xml配置：

<!-- 分页bean -->

   <bean id=*"pageBean"* class=*"com.*boonya*.jboa.utils.PaginationSupport"*

      scope=*"prototype"*></bean>


<!-- 基础action -->

   <bean id=*"baseAction"* class=*"com.*boonya*.jboa.action.BaseAction"*
 abstract=*"true"* scope=*"prototype"*>

      <property name=*"pageBean"* ref=*"pageBean"*></property>

   </bean>

   <bean id=*"claimVoucherAction"* class=*"com.*boonya*.jboa.action.ClaimVoucherAction"*
 parent=*"baseAction"* scope=*"prototype"*>

      <property name=*"claimVoucherService"* ref=*"claimVoucherService"*></property>

   </bean>

在Struts.xml下编写相应的action:

<package name=*"default"* namespace=*"/"* extends=*"struts-default"*>

      <action name=*"user"* class=*"userAction"*>

         <result name=*"input"*>/login.jsp</result>

         <result name=*"success"*>/success.jsp</result>

      </action>

在applicationContext.xml配置，引入注入对象:

<!-- 配置属性文件读取 -->

   <bean

   class="org.springframework.beans.factory.config.PreferencesPlaceholderConfigurer">

      <property name="locations">

         <list>

   <value>classpath:com/*boonya*/jboa/config/db-config.properties</value>

         </list>

      </property>

   </bean>

   <!-- 导入分配置文件 -->

   <import resource="com/*boonya*/jboa/config/dao.xml"/>

   <import resource="com/*boonya*/jboa/config/service.xml"/>

   <import resource="com/*boonya*/jboa/config/action.xml"/>





