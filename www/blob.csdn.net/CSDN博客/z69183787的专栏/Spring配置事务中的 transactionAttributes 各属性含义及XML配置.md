# Spring配置事务中的 transactionAttributes 各属性含义及XML配置 - z69183787的专栏 - CSDN博客
2013年12月06日 09:50:22[OkidoGreen](https://me.csdn.net/z69183787)阅读数：14620
**transactionAttributes 属性：**
**PROPAGATION**
|事务传播行为类型|说明|
|----|----|
|PROPAGATION_REQUIRED|如果当前没有事务，就新建一个事务，如果已经存在一个事务中，加入到这个事务中。这是最常见的选择。|
|PROPAGATION_SUPPORTS|支持当前事务，如果当前没有事务，就以非事务方式执行。|
|PROPAGATION_MANDATORY|使用当前的事务，如果当前没有事务，就抛出异常。|
|PROPAGATION_REQUIRES_NEW|新建事务，如果当前存在事务，把当前事务挂起。|
|PROPAGATION_NOT_SUPPORTED|以非事务方式执行操作，如果当前存在事务，就把当前事务挂起。|
|PROPAGATION_NEVER|以非事务方式执行，如果当前存在事务，则抛出异常。|
|PROPAGATION_NESTED|如果当前存在事务，则在嵌套事务内执行。如果当前没有事务，则执行与PROPAGATION_REQUIRED类似的操作。|
**Exception**：（注解用 rollback-for） 定义当遇到Exception异常 时 回滚。
[查看文本](http://www.verydemo.com/demo_c143_i31690.html#)[打印](http://www.verydemo.com/demo_c143_i31690.html#)[?](http://www.verydemo.com/demo_c143_i31690.html#)
- <propkey="save*">PROPAGATION_REQUIRED,-ApplicationException,+BusinessException</prop>
- 表示抛出该异常时需要回滚
+表示即使抛出该异常**事务**同样要提交
-ApplicationException ：表示抛出ApplicationException 时，**事务**需要回滚。但不是说只抛出ApplicationException 异常时，**事务**才回滚，如果程序抛出RuntimeException和Error时，**事务**一样会回滚，即使这里没有配置。因为Spring中默认对所有的RuntimeException和Error都会回滚**事务**。
**readonly ：**
readonly是设置操作权限为只读,一般用于查询的方法,优化作用.
**注解形式：**
```
<context:annotation-config />  
    <context:component-scan base-package="com.bluesky" />  
  
    <tx:annotation-driven transaction-manager="transactionManager"/>  
  
    <bean id="sessionFactory"    
            class="org.springframework.orm.hibernate3.LocalSessionFactoryBean">    
        <property name="configLocation" value="classpath:hibernate.cfg.xml" />    
        <property name="configurationClass" value="org.hibernate.cfg.AnnotationConfiguration" />  
    </bean>    
  
    <!-- 定义事务管理器（声明式的事务） -->    
    <bean id="transactionManager"  
        class="org.springframework.orm.hibernate3.HibernateTransactionManager">  
        <property name="sessionFactory" ref="sessionFactory" />  
    </bean>  
      
</beans>
```
```java
package com.bluesky.spring.dao;  
  
import java.util.List;  
  
import org.hibernate.SessionFactory;  
import org.springframework.beans.factory.annotation.Autowired;  
import org.springframework.orm.hibernate3.support.HibernateDaoSupport;  
import org.springframework.stereotype.Component;  
  
import com.bluesky.spring.domain.User;  
  
@Transactional(value = "txManager",propagation=Propagation.REQUIRED,rollbackFor=Exception.class,readOnly=true)
@Component("userDao")  
public class UserDaoImpl extends HibernateDaoSupport implements UserDao {  
  
    public List<User> listUsers() {  
        return this.getSession().createQuery("from User").list();  
    }  
      
      
}
```
**aop形式：**
```
<context:annotation-config />  
   <context:component-scan base-package="com.bluesky" />  
  
   <bean id="sessionFactory"    
           class="org.springframework.orm.hibernate3.LocalSessionFactoryBean">    
       <property name="configLocation" value="classpath:hibernate.cfg.xml" />    
       <property name="configurationClass" value="org.hibernate.cfg.AnnotationConfiguration" />  
   </bean>    
  
   <!-- 定义事务管理器（声明式的事务） -->    
   <bean id="transactionManager"  
       class="org.springframework.orm.hibernate3.HibernateTransactionManager">  
       <property name="sessionFactory" ref="sessionFactory" />  
   </bean>  
  
   <tx:advice id="txAdvice" transaction-manager="transactionManager">  
       <tx:attributes>  
           <tx:method name="*" propagation="REQUIRED" rollback-for="Exception" />  
       </tx:attributes>  
   </tx:advice>  
     
   <aop:config>  
       <aop:pointcut id="interceptorPointCuts"  
           expression="execution(* com.wonders.asset.service..*.*(..))" />
       <aop:advisor advice-ref="txAdvice"  
           pointcut-ref="interceptorPointCuts" />          
   </aop:config>        
</beans>
```
**共享基类：**
```
<bean id="sessionFactory"    
            class="org.springframework.orm.hibernate3.LocalSessionFactoryBean">    
        <property name="configLocation" value="classpath:hibernate.cfg.xml" />    
        <property name="configurationClass" value="org.hibernate.cfg.AnnotationConfiguration" />  
    </bean>    
  
    <!-- 定义事务管理器（声明式的事务） -->    
    <bean id="transactionManager"  
        class="org.springframework.orm.hibernate3.HibernateTransactionManager">  
        <property name="sessionFactory" ref="sessionFactory" />  
    </bean>  
      
    <bean id="transactionBase"    
            class="org.springframework.transaction.interceptor.TransactionProxyFactoryBean"    
            lazy-init="true" abstract="true">    
        <!-- 配置事务管理器 -->    
        <property name="transactionManager" ref="transactionManager" />    
        <!-- 配置事务属性 -->    
        <property name="transactionAttributes">    
            <props>    
                <prop key="*">PROPAGATION_REQUIRED</prop>    
            </props>    
        </property>    
    </bean>      
     
    <!-- 配置DAO -->  
    <bean id="userDaoTarget" class="com.bluesky.spring.dao.UserDaoImpl">  
        <property name="sessionFactory" ref="sessionFactory" />  
    </bean>  
      
    <bean id="userDao" parent="transactionBase" >    
        <property name="target" ref="userDaoTarget" />     
    </bean>  
</beans>
```
