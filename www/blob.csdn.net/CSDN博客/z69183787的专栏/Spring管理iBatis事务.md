# Spring管理iBatis事务 - z69183787的专栏 - CSDN博客
2015年05月31日 14:08:37[OkidoGreen](https://me.csdn.net/z69183787)阅读数：921
```
<sqlMapConfig>
    <sqlMap resource="com/angi/ibatis/maps/User.xml" />
</sqlMapConfig>
```
以上配置省去了transactionManager的配置，就会使用external（外部）事务管理（ExternalTransaction），即等同如下配置：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<sqlMapConfig>
    <transactionManager type="EXTERNAL">
                <!--这个数据源其实没有什么意义，还是取上面的省略方式吧-->
        <dataSource type="DBCP">
        </dataSource>
    </transactionManager>
    <sqlMap resource="com/angi/ibatis/maps/User.xml" />
</sqlMapConfig>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
1、TransactionProxyFactoryBean
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:p="http://www.springframework.org/schema/p"
    xmlns:aop="http://www.springframework.org/schema/aop" xmlns:tx="http://www.springframework.org/schema/tx"
    xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-3.0.xsd http://www.springframework.org/schema/aop http://www.springframework.org/schema/aop/spring-aop-3.0.xsd http://www.springframework.org/schema/tx http://www.springframework.org/schema/tx/spring-tx-3.0.xsd ">
    <!-- DataSource -->
    <bean id="dataSource" class="org.apache.commons.dbcp.BasicDataSource">
        <property name="driverClassName">
            <value>com.mysql.jdbc.Driver</value>
        </property>
        <!--<property name="defaultAutoCommit" value="false"/>-->
        <property name="url">
            <value>jdbc:mysql://localhost/test</value>
        </property>
        <property name="username">
            <value>root</value>
        </property>
        <property name="password">
            <value>mysql</value>
        </property>
    </bean>
    <!-- Spring iBatis Template -->
    <bean id="sqlMapClient" class="org.springframework.orm.ibatis.SqlMapClientFactoryBean">
        <property name="configLocation" value="SqlMapConfig.xml" />
        <property name="dataSource" ref="dataSource" />
    </bean>
    <bean id="transactionManager"
        class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
        <property name="dataSource">
            <ref local="dataSource" />
        </property>
    </bean>
    <bean id="userDAO" class="com.angi.ibatis.dao.UserDaoImpl">
        <property name="sqlMapClient">
            <ref bean="sqlMapClient" />
        </property>
    </bean>
    <bean id="userService" class="com.angi.ibatis.service.UserService">
        <property name="userDao">
            <ref bean="userDAO" />
        </property>
    </bean>
    <bean id="userServiceProxy"
        class="org.springframework.transaction.interceptor.TransactionProxyFactoryBean">
        <property name="transactionManager">
            <ref bean="transactionManager" />
        </property>
        <property name="target">
            <ref local="userService" />
        </property>
        <property name="transactionAttributes">
            <props>
                <!-- 这里的方法签名可以精确到方法, 先懒惰一下全配置上 -->
                <prop key="*">PROPAGATION_REQUIRED</prop>
            </props>
        </property>
    </bean>
</beans>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
2、TransactionInterceptor
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:p="http://www.springframework.org/schema/p"
    xmlns:aop="http://www.springframework.org/schema/aop" xmlns:tx="http://www.springframework.org/schema/tx"
    xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-3.0.xsd http://www.springframework.org/schema/aop http://www.springframework.org/schema/aop/spring-aop-3.0.xsd http://www.springframework.org/schema/tx http://www.springframework.org/schema/tx/spring-tx-3.0.xsd ">
    <!-- DataSource -->
    <bean id="dataSource" class="org.apache.commons.dbcp.BasicDataSource">
        <property name="driverClassName">
            <value>com.mysql.jdbc.Driver</value>
        </property>
        <!--<property name="defaultAutoCommit" value="false"/>-->
        <property name="url">
            <value>jdbc:mysql://localhost/test</value>
        </property>
        <property name="username">
            <value>root</value>
        </property>
        <property name="password">
            <value>mysql</value>
        </property>
    </bean>
    <!-- Spring iBatis Template -->
    <bean id="sqlMapClient" class="org.springframework.orm.ibatis.SqlMapClientFactoryBean">
        <property name="configLocation" value="SqlMapConfig.xml" />
        <property name="dataSource" ref="dataSource" />
    </bean>
    <bean id="transactionManager"
        class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
        <property name="dataSource">
            <ref local="dataSource" />
        </property>
    </bean>
    <bean id="userDAO" class="com.angi.ibatis.dao.UserDaoImpl">
        <property name="sqlMapClient">
            <ref bean="sqlMapClient" />
        </property>
    </bean>
    <bean id="userService" class="com.angi.ibatis.service.UserService">
        <property name="userDao">
            <ref bean="userDAO" />
        </property>
    </bean>
    <bean
        class="org.springframework.aop.framework.autoproxy.BeanNameAutoProxyCreator">
        <property name="beanNames">
            <list>
                <value>userService</value>
            </list>
        </property>
        <property name="interceptorNames">
            <list>
                <value>transactionInterceptor</value>
            </list>
        </property>
    </bean>
    <bean id="transactionInterceptor"
        class="org.springframework.transaction.interceptor.TransactionInterceptor">
        <property name="transactionManager" ref="transactionManager" />
        <property name="transactionAttributes">
            <props>
                <!-- 这里的方法签名可以精确到方法, 先懒惰一下全配置上 -->
                <prop key="*">PROPAGATION_REQUIRED</prop>
            </props>
        </property>
    </bean>
</beans>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
3、AOP和TX配置
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:p="http://www.springframework.org/schema/p"
    xmlns:aop="http://www.springframework.org/schema/aop" xmlns:tx="http://www.springframework.org/schema/tx"
    xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-3.0.xsd http://www.springframework.org/schema/aop http://www.springframework.org/schema/aop/spring-aop-3.0.xsd http://www.springframework.org/schema/tx http://www.springframework.org/schema/tx/spring-tx-3.0.xsd ">
    <!-- DataSource -->
    <bean id="dataSource" class="org.apache.commons.dbcp.BasicDataSource"
        destroy-method="close">
        <property name="driverClassName">
            <value>com.mysql.jdbc.Driver</value>
        </property>
        <property name="url">
            <value>jdbc:mysql://localhost/test</value>
        </property>
        <property name="username">
            <value>root</value>
        </property>
        <property name="password">
            <value>mysql</value>
        </property>
    </bean>
    <!-- Spring iBatis Template -->
    <bean id="sqlMapClient" class="org.springframework.orm.ibatis.SqlMapClientFactoryBean">
        <property name="configLocation" value="SqlMapConfig.xml" />
        <property name="dataSource" ref="dataSource" />
    </bean>
    <bean id="transactionManager"
        class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
        <property name="dataSource" ref="dataSource" />
    </bean>
    <!-- 需要引入aop的命名空间 -->
    <aop:config>
        <!-- 切入点指明了在所有方法产生事务拦截操作 -->
        <aop:pointcut id="serviceMethods"
            expression="execution(* com.angi.ibatis.service.*.*(..))" />
        <!-- 定义了将采用何种拦截操作，这里引用到 txAdvice -->
        <aop:advisor advice-ref="txAdvice" pointcut-ref="serviceMethods" />
    </aop:config>
    <!-- 需要引入tx的命名空间 -->
    <!-- 这是事务通知操作，使用的事务管理器引用自 transactionManager -->
    <tx:advice id="txAdvice" transaction-manager="transactionManager">
        <tx:attributes>
            <!-- 指定哪些方法需要加入事务，这里懒惰一下全部加入，可以使用通配符来只加入需要的方法 -->
            <tx:method name="*" propagation="REQUIRED" />
        </tx:attributes>
    </tx:advice>
    <bean id="userDAO" class="com.angi.ibatis.dao.UserDaoImpl">
        <property name="sqlMapClient">
            <ref bean="sqlMapClient" />
        </property>
    </bean>
    <bean id="userService" class="com.angi.ibatis.service.UserService">
        <property name="userDao">
            <ref bean="userDAO" />
        </property>
    </bean>
</beans>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
4、anotation
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:p="http://www.springframework.org/schema/p"
    xmlns:tx="http://www.springframework.org/schema/tx"
    xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-3.0.xsd http://www.springframework.org/schema/aop http://www.springframework.org/schema/aop/spring-aop-3.0.xsd http://www.springframework.org/schema/tx http://www.springframework.org/schema/tx/spring-tx-3.0.xsd ">
    <!-- 需要引入tx的命名空间 -->
    <tx:annotation-driven transaction-manager="transactionManager" />
    <!-- DataSource -->
    <bean id="dataSource" class="org.apache.commons.dbcp.BasicDataSource">
        <property name="driverClassName">
            <value>com.mysql.jdbc.Driver</value>
        </property>
        <!--<property name="defaultAutoCommit" value="false"/>-->
        <property name="url">
            <value>jdbc:mysql://localhost/test</value>
        </property>
        <property name="username">
            <value>root</value>
        </property>
        <property name="password">
            <value>mysql</value>
        </property>
    </bean>
    <!-- Spring iBatis Template -->
    <bean id="sqlMapClient" class="org.springframework.orm.ibatis.SqlMapClientFactoryBean">
        <property name="configLocation" value="SqlMapConfig.xml" />
        <property name="dataSource" ref="dataSource" />
    </bean>
    <bean id="transactionManager"
        class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
        <property name="dataSource">
            <ref local="dataSource" />
        </property>
    </bean>
    <bean id="userDAO" class="com.angi.ibatis.dao.UserDaoImpl">
        <property name="sqlMapClient">
            <ref bean="sqlMapClient" />
        </property>
    </bean>
    <bean id="userService" class="com.angi.ibatis.service.UserService">
        <property name="userDao">
            <ref bean="userDAO" />
        </property>
    </bean>
</beans>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
Java代码：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
@Transactional
    public void doTransaction() {
        User user = new User();
        user.setName("11111");
        user.setSex(1);
        userDao.saveUser(user);
        User user1 = new User();
        user1.setName("Angikkk");
        user1.setSex(1);
        userDao.saveUser(user1);
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
    }
