# spring+jotm 多数据源事务管理(三)JNDI+Tomcat - z69183787的专栏 - CSDN博客
2014年04月02日 10:32:02[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1278
首先需要将jotm相关jar包加入到tomcat中，如下：
o jotm.jar
o jotm_jrmp_stubs.jar
o ow_carol.jar
o jta.jar
o jta-spec1_0_1.jar
o jts1_0.jar
o objectweb-datasource.jar
o xapool.jar
o howl.jar
o commons-cli-1.0.jar
o connector-1_5.jar
tomcat中配置context：
Xml代码  ![收藏代码](http://hanqunfeng.iteye.com/images/icon_star.png)
- <ContextdocBase="D:\workspace\test\WebContent"path="/test"reloadable="true">
- 
- <Resourcename="jdbc/test1"
- auth="Container"
- factory="org.objectweb.jndi.DataSourceFactory"
- type="javax.sql.DataSource"
- driverClassName="com.mysql.jdbc.Driver"
- url="jdbc:mysql://localhost:3306/test1?useUnicode=true&characterEncoding=utf-8"
- username="root"password="123456"maxActive="20"maxIdle="10"maxWait="10000"/>
- <Resourcename="jdbc/test2"
- auth="Container"
- factory="org.objectweb.jndi.DataSourceFactory"
- type="javax.sql.DataSource"
- driverClassName="com.mysql.jdbc.Driver"
- url="jdbc:mysql://localhost:3306/test2?useUnicode=true&characterEncoding=utf-8"
- username="root"password="123456"maxActive="20"maxIdle="10"maxWait="10000"/>
- 
- <Transactionfactory="org.objectweb.jotm.UserTransactionFactory"jotm.timeout="60"/>
- 
- </Context>
spring配置文件中使用jndi的方式关联数据源：
Xml代码  ![收藏代码](http://hanqunfeng.iteye.com/images/icon_star.png)
- <jee:jndi-lookupid="dataSource1"jndi-name="jdbc/test1"/>
- <jee:jndi-lookupid="dataSource2"jndi-name="jdbc/test2"/>
- 
- <beanid="jotm"class="org.springframework.transaction.jta.JotmFactoryBean"/>
- 
- <beanid="transactionManager"class="org.springframework.transaction.jta.JtaTransactionManager">
- <propertyname="userTransaction"ref="jotm"/>
- </bean>
 只需要将数据源换成jndi的方式即可，其它不变,
 另外记得要在项目编译路径下添加前文中提到的carol.properties。
