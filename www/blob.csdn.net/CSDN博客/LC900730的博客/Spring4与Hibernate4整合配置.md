# Spring4与Hibernate4整合配置 - LC900730的博客 - CSDN博客
2017年08月10日 11:09:30[lc900730](https://me.csdn.net/LC900730)阅读数：162
### 整合关键点
hibernate对象交给Spring创建
```
1.Hibernate的SessionFactory对象交给Spring创建
2.Hibernate事务交给spring声明式事务管理。
```
SSH整合： 
    SessionFactory:启动的时候初始化，整个应用都有效；因此创建的工作，交给spring管理。
##  加载hibernate配置文件去整合
用的是spring4+hibernate4的jar包，但是总是失败，unwrap javax.sql.dataSource
## 连接池方式
将datasource直接在bean.xml中配置，然后sessionFactory直接使用，这种方式成功了。 
记录配置文件如下
```xml
bean.xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xmlns:p="http://www.springframework.org/schema/p"
    xmlns:context="http://www.springframework.org/schema/context"
    xmlns:aop="http://www.springframework.org/schema/aop"
    xmlns:tx="http://www.springframework.org/schema/tx" 
    xsi:schemaLocation="
        http://www.springframework.org/schema/beans
        http://www.springframework.org/schema/beans/spring-beans.xsd
        http://www.springframework.org/schema/context
        http://www.springframework.org/schema/context/spring-context.xsd
        http://www.springframework.org/schema/aop
        http://www.springframework.org/schema/aop/spring-aop.xsd
        http://www.springframework.org/schema/tx 
        http://www.springframework.org/schema/tx/spring-tx.xsd">
     <bean id="deptDao" class="cn.itcast.dao.DeptDao">
        <property name="sessionFactory" ref="sessionFactory"></property>
     </bean>
     <!-- service -->
     <bean id="deptService" class="cn.itcast.service.DeptService">
        <property name="deptDao" ref="deptDao"></property>
     </bean>
     <bean id="dataSource" class="com.mchange.v2.c3p0.ComboPooledDataSource">
        <property name="driverClass" value="com.mysql.cj.jdbc.Driver"></property>
        <property name="jdbcUrl" value="jdbc:mysql://localhost:3306/test?characterEncoding=utf8&useSSL=false"></property>
        <property name="user" value="root"></property>
            <property name="password" value="qqaazz"></property>
        <property name="initialPoolSize" value="3"></property>
            <property name="maxPoolSize" value="10"></property>
        <property name="maxStatements" value="100"></property>
            <property name="acquireIncrement" value="2"></property>
      </bean>
     <!-- Spring与Hibernate整合 -->
    <!-- 连接池交给spring管理 -->
    <bean id="sessionFactory" class="org.springframework.orm.hibernate4.LocalSessionFactoryBean">
          <property name="configLocation" value="classpath:hibernate.cfg.xml"></property>
        <property name="dataSource" ref="dataSource"></property>
    </bean>
    <!--配置事务管理器类  -->
    <bean id="txManager" class="org.springframework.orm.hibernate4.HibernateTransactionManager">
    <property name="sessionFactory" ref="sessionFactory"></property>
    </bean>
    <!-- 配置事务增强（拦截到方法之后，如何管理事务？只读？只写等） -->
    <tx:advice id="txAdvice" transaction-manager="txManager">
        <tx:attributes>
            <tx:method name="*" read-only="false"/>
        </tx:attributes>
    </tx:advice>
    <!--aop配置  -->
    <aop:config>
        <aop:pointcut expression="execution(* cn.itcast.service.*.*(..))" id="pt"/>
        <aop:advisor advice-ref="txAdvice" pointcut-ref="pt"/>
    </aop:config>               
</beans>
```
hibernate.cfg.xml文件
```xml
<!DOCTYPE hibernate-configuration PUBLIC
    "-//Hibernate/Hibernate Configuration DTD 3.0//EN"
    "http://www.hibernate.org/dtd/hibernate-configuration-3.0.dtd">
<hibernate-configuration>
    <session-factory>
        <property name="dialect">org.hibernate.dialect.MySQL5Dialect</property>
        <!--常用配置  -->
        <property name="show_sql">true</property>
        <property name="format_sql">true</property>
        <property name="hbm2ddl.auto">update</property>
         <mapping resource="cn/itcast/entity/Dept.hbm.xml"/>
    </session-factory>
</hibernate-configuration>
```
测试：
```java
public class App {
    private ApplicationContext ac=new ClassPathXmlApplicationContext("bean.xml");
    @Test
    public void testApp() throws Exception{
        DeptService deptService=(DeptService)ac.getBean("deptService");
        deptService.save(new Dept(30,"xiaom"));
    }
}
```
