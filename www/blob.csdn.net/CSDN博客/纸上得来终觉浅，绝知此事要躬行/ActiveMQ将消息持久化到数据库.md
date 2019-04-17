# ActiveMQ将消息持久化到数据库 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年04月27日 10:41:11[boonya](https://me.csdn.net/boonya)阅读数：8623








                
需求描述：将ActiveMQ消息持久化到mySql\Oracle数据库中 ；

环境描述：目前最新版本是ActiveMQ5.13.2，本文讲述的实例是ActiveMQ5.9.0。

使用默认的持久化机制，我们不容易直接看到消息究竟是如何持久的。ActiveMQ提供的JDBC持久化机制，能够将持久化信息存储到数据库。通过查看数据库中ActiveMQ生成的表结构和存储的数据，能够帮助我们更好的了解消息的持久化机制。现在介绍如何配置activemq，将数据持久化到mysql中。

### 1.配置activeMQ需要的mySql数据源

为了能够使用JDBC访问mysql数据库，显然必须要配置消息服务器的数据库源。在activemq\apache-activemq-5.9.0\conf\activemq.xml进行配置



```
<!-- MySQL DataSource -->
	<bean id="mysql-ds" class="org.apache.commons.dbcp.BasicDataSource" destroy-method="close">
		<property name="driverClassName" value="com.mysql.jdbc.Driver"/>
		<property name="url" value="jdbc:mysql://localhost/activemq?relaxAutoCommit=true"/>
		<property name="username" value="root"/>
		<property name="password" value="root"/>
		<property name="poolPreparedStatements" value="true"/>
	</bean>
```

在</broker>结点之后，增加数据源的配置

### 2.改变activeMQ默认的持久化方式

在activemq.xml中注释掉默认的kahadb，使用jdbc持久化



```java
<!--
        <persistenceAdapter>
            <kahaDB directory="${activemq.data}/kahadb"/>
        </persistenceAdapter>
		-->
		
		<persistenceAdapter>
			<jdbcPersistenceAdapter  dataSource="#mysql-ds"/>
		</persistenceAdapter>
```

### 3.设置JMS访问连接协议类型

修改原来的协议连接为如下方式：



```
<transportConnectors>
	<transportConnector name="default" uri="tcp://localhost:61616"/>
</transportConnectors>
```




### 4.提供mysql的驱动程序 

由于activeMQ消息服务器，没有自带mysql数据库的驱动程序。我们需要手动将mysql驱动添加到消息服务器。

方法是将驱动拷贝到apache-activemq-5.9.0\lib\目录下。




经过上面的三步配置，我们重新启动消息服务器，就可以发现activeMQ在数据库中新建了3张表activemq_acks  ，activemq_lock  ，activemq_msgs 。

![](https://img-blog.csdn.net/20160427103449871)


至此数据库持久化完成。ActiveMQ持久化的中表结构是什么，表需要人工创建吗？其实不需要，ActiveMQ会帮助我们生成的。只需要制定采用的数据库名称并，创建数据库即可。以为为ActiveMQ采用MySQL5.7持久化产生的SQL语句：

持久化mysql数据库的3张表；

activemq_acks：ActiveMQ的签收信息。

activemq_lock:ActiveMQ的锁信息。

activemq_msgs:ActiveMQ的消息的信息


### 5.activemq.xml全部配置信息



```
<!--
    Licensed to the Apache Software Foundation (ASF) under one or more
    contributor license agreements.  See the NOTICE file distributed with
    this work for additional information regarding copyright ownership.
    The ASF licenses this file to You under the Apache License, Version 2.0
    (the "License"); you may not use this file except in compliance with
    the License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
-->
<!-- START SNIPPET: example -->
<beans
  xmlns="http://www.springframework.org/schema/beans"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd
  http://activemq.apache.org/schema/core http://activemq.apache.org/schema/core/activemq-core.xsd">

    <!-- Allows us to use system properties as variables in this configuration file -->
    <bean class="org.springframework.beans.factory.config.PropertyPlaceholderConfigurer">
        <property name="locations">
            <value>file:${activemq.conf}/credentials.properties</value>
        </property>
    </bean>

    <!-- Allows log searching in hawtio console -->
    <bean id="logQuery" class="org.fusesource.insight.log.log4j.Log4jLogQuery"
          lazy-init="false" scope="singleton"
          init-method="start" destroy-method="stop">
    </bean>
	
	<!-- MySql DataSource Sample Setup -->
  <bean id="mysql-ds" class="org.apache.commons.dbcp.BasicDataSource" destroy-method="close">
    <property name="driverClassName" value="com.mysql.jdbc.Driver"/>
    <property name="url" value="jdbc:mysql://localhost/activemq?relaxAutoCommit=true"/>
    <property name="username" value="root"/>
    <property name="password" value="root"/>
    <property name="poolPreparedStatements" value="true"/>
  </bean>
    
  <!-- Oracle DataSource Sample Setup -->
  <!--
  <bean id="oracle-ds" class="org.apache.commons.dbcp.BasicDataSource" destroy-method="close">
    <property name="driverClassName" value="oracle.jdbc.driver.OracleDriver"/>
    <property name="url" value="jdbc:oracle:thin:@localhost:1521:AMQDB"/>
    <property name="username" value="scott"/>
    <property name="password" value="tiger"/>
    <property name="poolPreparedStatements" value="true"/>
  </bean>
  -->

    <!--
        The <broker> element is used to configure the ActiveMQ broker.
    -->
    <broker xmlns="http://activemq.apache.org/schema/core" brokerName="localhost" dataDirectory="${activemq.data}" >

        <destinationPolicy>
            <policyMap>
              <policyEntries>
                <policyEntry topic=">" >
                    <!-- The constantPendingMessageLimitStrategy is used to prevent
                         slow topic consumers to block producers and affect other consumers
                         by limiting the number of messages that are retained
                         For more information, see:

                         http://activemq.apache.org/slow-consumer-handling.html

                    -->
                  <pendingMessageLimitStrategy>
                    <constantPendingMessageLimitStrategy limit="1000"/>
                  </pendingMessageLimitStrategy>
                </policyEntry>
              </policyEntries>
            </policyMap>
        </destinationPolicy>


        <!--
            The managementContext is used to configure how ActiveMQ is exposed in
            JMX. By default, ActiveMQ uses the MBean server that is started by
            the JVM. For more information, see:

            http://activemq.apache.org/jmx.html
        -->
        <managementContext>
            <managementContext createConnector="false"/>
        </managementContext>

        <!--
            Configure message persistence for the broker. The default persistence
            mechanism is the KahaDB store (identified by the kahaDB tag).
            For more information, see:

            http://activemq.apache.org/persistence.html
        
        <persistenceAdapter>
            <kahaDB directory="${activemq.data}/kahadb"/>
        </persistenceAdapter>
		-->
		 
         <persistenceAdapter>
            <jdbcPersistenceAdapter dataDirectory="${activemq.data}/activemq-data" dataSource="#mysql-ds"/>
		</persistenceAdapter>
   
		<transportConnectors>
			<transportConnector name="default" uri="tcp://localhost:61616"/>
		</transportConnectors>

          <!--
            The systemUsage controls the maximum amount of space the broker will
            use before disabling caching and/or slowing down producers. For more information, see:
            http://activemq.apache.org/producer-flow-control.html
          -->
          <systemUsage>
            <systemUsage>
                <memoryUsage>
                    <memoryUsage percentOfJvmHeap="70" />
                </memoryUsage>
                <storeUsage>
                    <storeUsage limit="100 gb"/>
                </storeUsage>
                <tempUsage>
                    <tempUsage limit="50 gb"/>
                </tempUsage>
            </systemUsage>
        </systemUsage>

        <!--
            The transport connectors expose ActiveMQ over a given protocol to
            clients and other brokers. For more information, see:

            http://activemq.apache.org/configuring-transports.html
        -->
		 <!--
        <transportConnectors>
            DOS protection, limit concurrent connections to 1000 and frame size to 100MB 
            <transportConnector name="openwire" uri="tcp://127.0.0.1:61616?maximumConnections=1000&wireFormat.maxFrameSize=104857600"/>
            <transportConnector name="amqp" uri="amqp://127.0.0.1:5672?maximumConnections=1000&wireFormat.maxFrameSize=104857600"/>
            <transportConnector name="stomp" uri="stomp://127.0.0.1:61613?maximumConnections=1000&wireFormat.maxFrameSize=104857600"/>
            <transportConnector name="mqtt" uri="mqtt://127.0.0.1:1883?maximumConnections=1000&wireFormat.maxFrameSize=104857600"/>
            <transportConnector name="ws" uri="ws://127.0.0.1:61614?maximumConnections=1000&wireFormat.maxFrameSize=104857600"/>
        </transportConnectors>
		
		-->

        <!-- destroy the spring context on shutdown to stop jetty -->
        <shutdownHooks>
            <bean xmlns="http://www.springframework.org/schema/beans" class="org.apache.activemq.hooks.SpringContextHook" />
        </shutdownHooks>

    </broker>

    <!--
        Enable web consoles, REST and Ajax APIs and demos
        The web consoles requires by default login, you can disable this in the jetty.xml file

        Take a look at ${ACTIVEMQ_HOME}/conf/jetty.xml for more details
    -->
    <import resource="jetty.xml"/>

</beans>
<!-- END SNIPPET: example -->
```




### 6.参考资料

ActiveMQ消息JDBC持久化：[http://activemq.apache.org/jdbc-master-slave.html](http://activemq.apache.org/jdbc-master-slave.html)

持久化的代码参考：[http://topmanopensource.iteye.com/blog/1066383](http://topmanopensource.iteye.com/blog/1066383)





