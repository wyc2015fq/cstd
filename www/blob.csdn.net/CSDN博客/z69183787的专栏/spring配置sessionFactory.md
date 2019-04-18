# spring配置sessionFactory - z69183787的专栏 - CSDN博客
2012年11月19日 15:36:00[OkidoGreen](https://me.csdn.net/z69183787)阅读数：727
个人分类：[Spring-知识](https://blog.csdn.net/z69183787/article/category/2175163)
方法1：
第一步： 在spring文件中配置如下：
<!-- 配置sessionFactory -->
 <bean id="sessionFactory" class="org.springframework.orm.hibernate3.LocalSessionFactoryBean">
  <property name="configLocation">
   <value>classpath:hibernate.cfg.xml</value>
  </property> 
 </bean>
第二步：配置hibernate.cfg.xml文件：
  <!DOCTYPE hibernate-configuration PUBLIC
 "-//Hibernate/Hibernate Configuration DTD 3.0//EN"
 "[http://hibernate.sourceforge.net/hibernate-configuration-3.0.dtd](http://hibernate.sourceforge.net/hibernate-configuration-3.0.dtd)">
<hibernate-configuration>
 <session-factory>
  <property name="hibernate.connection.url">jdbc:mysql://localhost/spring_hibernate_2</property>
  <property name="hibernate.connection.driver_class">com.mysql.jdbc.Driver</property>
  <property name="hibernate.connection.username">root</property>
  <property name="hibernate.connection.password">root</property>
  <property name="hibernate.dialect">org.hibernate.dialect.MySQLDialect</property>
  <property name="hibernate.show_sql">true</property>
  <property name="hibernate.current_session_context_class">thread</property>
  <!--
  <property name="hibernate.current_session_context_class">jta</property>
   -->  
  <mapping resource="com/bjsxt/usermgr/model/User.hbm.xml"/>
  <mapping resource="com/bjsxt/usermgr/model/Log.hbm.xml"/>
 </session-factory>
</hibernate-configuration>
方法2：用AnnotationSessionFactoryBean类来配置sessionFactory
  <bean id="sessionFactory"
    class="org.springframework.orm.hibernate3.annotation.AnnotationSessionFactoryBean">
    <property name="dataSource" ref="dataSource" />
    <property name="annotatedClasses">
     <list>
      <value>com.bjsxt.model.User</value>
      <value>com.bjsxt.model.Log</value>
     </list>
     </property>
     <property name="hibernateProperties">
     <props>
      <prop key="hibernate.dialect">
        org.hibernate.dialect.MySQLDialect
       </prop>
      <prop key="hibernate.show_sql">true</prop>
     </props>
  </property>
 </bean>
