# Spring事务配置的五种方式 - z69183787的专栏 - CSDN博客
2012年11月19日 15:59:15[OkidoGreen](https://me.csdn.net/z69183787)阅读数：911
个人分类：[事务-原理																[Spring-知识](https://blog.csdn.net/z69183787/article/category/2175163)](https://blog.csdn.net/z69183787/article/category/2175493)
Spring事务配置的五种方式 
    前段时间对Spring的事务配置做了比较深入的研究，在此之间对Spring的事务配置虽说也配置过，但是一直没有一个清楚的认识。通过这次的学习发觉Spring的事务配置只要把思路理清，还是比较好掌握的。 
    总结如下： 
    Spring配置文件中关于事务配置总是由三个组成部分，分别是DataSource、TransactionManager和代理机制这三部分，无论哪种配置方式，一般变化的只是代理机制这部分。 
    DataSource、TransactionManager这两部分只是会根据数据访问方式有所变化，比如使用Hibernate进行数据访问时，DataSource实际为SessionFactory，TransactionManager的实现为HibernateTransactionManager。 
    具体如下图： 
![](http://www.blogjava.net/images/blogjava_net/robbie/WindowsLiveWriter/Spring_9C9C/Spring%E4%BA%8B%E5%8A%A1%E9%85%8D%E7%BD%AE%20(2).jpg)
 根据代理机制的不同，总结了五种Spring事务的配置方式，配置文件如下：
    第一种方式：每个Bean都有一个代理
Java代码  ![收藏代码](http://jie2workjava.iteye.com/images/icon_star.png)
- <bean id="sessionFactory"
- class="org.springframework.orm.hibernate3.LocalSessionFactoryBean">    
-        <property name="configLocation" value="classpath:hibernate.cfg.xml" />    
-        <property name="configurationClass" value="org.hibernate.cfg.AnnotationConfiguration" />  
-    </bean>    
- 
-    <!-- 定义事务管理器（声明式的事务） -->    
-    <bean id="transactionManager"
- class="org.springframework.orm.hibernate3.HibernateTransactionManager">  
-        <property name="sessionFactory" ref="sessionFactory" />  
-    </bean>  
- 
-    <!-- 配置DAO -->  
-    <bean id="userDaoTarget"class="com.bluesky.spring.dao.UserDaoImpl">  
-        <property name="sessionFactory" ref="sessionFactory" />  
-    </bean>  
- 
-    <bean id="userDao"
- class="org.springframework.transaction.interceptor.TransactionProxyFactoryBean">    
-           <!-- 配置事务管理器 -->    
-           <property name="transactionManager" ref="transactionManager" />       
-        <property name="target" ref="userDaoTarget" />    
-         <property name="proxyInterfaces" value="com.bluesky.spring.dao.GeneratorDao" />  
-        <!-- 配置事务属性 -->    
-        <property name="transactionAttributes">    
-            <props>    
-                <prop key="*">PROPAGATION_REQUIRED</prop>  
-            </props>    
-        </property>    
-    </bean>    
- lt;/beans>  
 第二种方式：所有Bean共享一个代理基类
Java代码  ![收藏代码](http://jie2workjava.iteye.com/images/icon_star.png)
- <bean id="sessionFactory"
- class="org.springframework.orm.hibernate3.LocalSessionFactoryBean">    
-         <property name="configLocation" value="classpath:hibernate.cfg.xml" />    
-         <property name="configurationClass" value="org.hibernate.cfg.AnnotationConfiguration" />  
-     </bean>    
- 
-     <!-- 定义事务管理器（声明式的事务） -->    
-     <bean id="transactionManager"
- class="org.springframework.orm.hibernate3.HibernateTransactionManager">  
-         <property name="sessionFactory" ref="sessionFactory" />  
-     </bean>  
- 
-     <bean id="transactionBase"
- class="org.springframework.transaction.interceptor.TransactionProxyFactoryBean"
-             lazy-init="true"abstract="true">    
-         <!-- 配置事务管理器 -->    
-         <property name="transactionManager" ref="transactionManager" />    
-         <!-- 配置事务属性 -->    
-         <property name="transactionAttributes">    
-             <props>    
-                 <prop key="*">PROPAGATION_REQUIRED</prop>    
-             </props>    
-         </property>    
-     </bean>      
- 
-     <!-- 配置DAO -->  
-     <bean id="userDaoTarget"class="com.bluesky.spring.dao.UserDaoImpl">  
-         <property name="sessionFactory" ref="sessionFactory" />  
-     </bean>  
- 
-     <bean id="userDao" parent="transactionBase" >    
-         <property name="target" ref="userDaoTarget" />     
-     </bean>  
- </beans>  
第三种方式：使用拦截器
Java代码  ![收藏代码](http://jie2workjava.iteye.com/images/icon_star.png)
- <bean id="sessionFactory"
- class="org.springframework.orm.hibernate3.LocalSessionFactoryBean">    
-         <property name="configLocation" value="classpath:hibernate.cfg.xml" />    
-         <property name="configurationClass" value="org.hibernate.cfg.AnnotationConfiguration" />  
-     </bean>    
- 
-     <!-- 定义事务管理器（声明式的事务） -->    
-     <bean id="transactionManager"
- class="org.springframework.orm.hibernate3.HibernateTransactionManager">  
-         <property name="sessionFactory" ref="sessionFactory" />  
-     </bean>   
- 
-     <bean id="transactionInterceptor"
- class="org.springframework.transaction.interceptor.TransactionInterceptor">    
-         <property name="transactionManager" ref="transactionManager" />    
-         <!-- 配置事务属性 -->    
-         <property name="transactionAttributes">    
-             <props>    
-                 <prop key="*">PROPAGATION_REQUIRED,-Exception</prop>    
-             </props>    
-         </property>    
-     </bean>  
- 
-     <bean class="org.springframework.aop.framework.autoproxy.BeanNameAutoProxyCreator">    
-         <property name="beanNames">    
-             <list>    
-                 <value>*Dao</value>  
-             </list>    
-         </property>    
-         <property name="interceptorNames">    
-             <list>    
-                 <value>transactionInterceptor</value>    
-             </list>    
-         </property>    
-     </bean>    
- 
-     <!-- 配置DAO -->  
-     <bean id="userDao"class="com.bluesky.spring.dao.UserDaoImpl">  
-         <property name="sessionFactory" ref="sessionFactory" />  
-     </bean>  
- </beans>  
第四种方式：使用tx标签配置的拦截器
Java代码  ![收藏代码](http://jie2workjava.iteye.com/images/icon_star.png)
- <context:annotation-config />  
-    <context:component-scan base-package="com.bluesky" />  
- 
-    <bean id="sessionFactory"
- class="org.springframework.orm.hibernate3.LocalSessionFactoryBean">    
-        <property name="configLocation" value="classpath:hibernate.cfg.xml" />    
-        <property name="configurationClass" value="org.hibernate.cfg.AnnotationConfiguration" />  
-    </bean>    
- 
-    <!-- 定义事务管理器（声明式的事务） -->    
-    <bean id="transactionManager"
- class="org.springframework.orm.hibernate3.HibernateTransactionManager">  
-        <property name="sessionFactory" ref="sessionFactory" />  
-    </bean>  
- 
-    <tx:advice id="txAdvice" transaction-manager="transactionManager">  
-        <tx:attributes>  
-            <tx:method name="*" propagation="REQUIRED" rollback-for="Exception" />  
-        </tx:attributes>  
-    </tx:advice>  
- 
-    <aop:config>  
-        <aop:pointcut id="interceptorPointCuts"
-            expression="execution(* com.bluesky.spring.dao.*.*(..))" />  
-        <aop:advisor advice-ref="txAdvice"
-            pointcut-ref="interceptorPointCuts" />          
-    </aop:config>        
- lt;/beans>  
第五种方式：全注解
Java代码  ![收藏代码](http://jie2workjava.iteye.com/images/icon_star.png)
- <context:annotation-config />  
-     <context:component-scan base-package="com.bluesky" />  
-   <!--开启注解事务-->
-     <tx:annotation-driven transaction-manager="transactionManager"/>  
- 
-     <bean id="sessionFactory"
- class="org.springframework.orm.hibernate3.LocalSessionFactoryBean">    
-         <property name="configLocation" value="classpath:hibernate.cfg.xml" />    
-         <property name="configurationClass" value="org.hibernate.cfg.AnnotationConfiguration" />  
-     </bean>    
- 
-     <!-- 定义事务管理器（声明式的事务） -->    
-     <bean id="transactionManager"
- class="org.springframework.orm.hibernate3.HibernateTransactionManager">  
-         <property name="sessionFactory" ref="sessionFactory" />  
-     </bean>  
- 
- </beans>  
此时在DAO上需加上@Transactional注解，如下：
Java代码  ![收藏代码](http://jie2workjava.iteye.com/images/icon_star.png)
- package com.bluesky.spring.dao;  
- 
- import java.util.List;  
- 
- import org.hibernate.SessionFactory;  
- import org.springframework.beans.factory.annotation.Autowired;  
- import org.springframework.orm.hibernate3.support.HibernateDaoSupport;  
- import org.springframework.stereotype.Component;  
- 
- import com.bluesky.spring.domain.User;  
- 
- @Transactional(value = "txManager",propagation=Propagation.REQUIRED,rollbackFor=Exception.class)
- @Component("userDao")  
- publicclass UserDaoImpl extends HibernateDaoSupport implements UserDao {  
- 
- public List<User> listUsers() {  
- returnthis.getSession().createQuery("from User").list();  
-     }  
- 
- 
- }  
