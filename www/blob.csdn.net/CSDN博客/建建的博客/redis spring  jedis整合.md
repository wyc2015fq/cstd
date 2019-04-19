# redis  spring   jedis整合 - 建建的博客 - CSDN博客
2018年02月23日 17:07:32[纪建](https://me.csdn.net/u013898698)阅读数：52
                <beans xmlns="http://www.springframework.org/schema/beans"
    xmlns:context="http://www.springframework.org/schema/context" xmlns:p="http://www.springframework.org/schema/p"
    xmlns:aop="http://www.springframework.org/schema/aop" xmlns:tx="http://www.springframework.org/schema/tx"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-4.2.xsd
    http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context-4.2.xsd
    http://www.springframework.org/schema/aop http://www.springframework.org/schema/aop/spring-aop-4.2.xsd http://www.springframework.org/schema/tx http://www.springframework.org/schema/tx/spring-tx-4.2.xsd
    http://www.springframework.org/schema/util http://www.springframework.org/schema/util/spring-util-4.2.xsd">
    <!-- 开启注解 -->
   <!--  <context:annotation-config />--> 
      <context:property-placeholder location="classpath:properties/redis.properties" ignore-unresolvable="true" />
    <!--Jedis连接池的相关配置-->
    <bean id="jedisPoolConfig" class="redis.clients.jedis.JedisPoolConfig">
        <!--新版是maxTotal，旧版是maxActive-->
        <property name="maxTotal">
            <value>200</value>
        </property>
        <property name="maxIdle">
            <value>50</value>
        </property>
        <property name="testOnBorrow" value="true"/>
        <property name="testOnReturn" value="true"/>
    </bean>
    <bean id="jedisPool" class="redis.clients.jedis.JedisPool">
        <constructor-arg name="poolConfig" ref="jedisPoolConfig" />
        <constructor-arg name="host" value="47.97.175.99"/>
        <constructor-arg name="port" value="6379" type="int" />
        <constructor-arg name="timeout" value="3000" type="int" />
        <constructor-arg name="password" value="123" />
        <constructor-arg name="database" value="0" type="int" />
    </bean>
    <bean id="jedisClientPool" class="com.taotao.content.jedis.JedisClientPool"></bean>
</beans>
