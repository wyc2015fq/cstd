# 在Spring中使用JOTM实现JTA事务管理 - 零度的博客专栏 - CSDN博客
2016年07月19日 16:19:55[零度anngle](https://me.csdn.net/zmx729618)阅读数：2778
        Spring 通过AOP技术可以让我们在脱离EJB的情况下享受声明式事务的丰盛大餐，脱离[Java
 EE](http://lib.csdn.net/base/17)应用服务器使用声明式事务的道路已经畅通无阻。但是很大部分人都还认为脱离Java EE应用服务器就无法使用JTA事务，这是一个误解。其实，通过配合使用ObjectWeb的JOTM开源项目，不需要Java EE应用服务器，Spring也可以提供JTA事务。
       
正因为AOP让Spring拥有了脱离EJB容器的声明式事务能力，而JOTM让我们在脱离Java EE应用服务器下拥有JTA事务能力。所以，人们将AOP和JOTM称为Java软件开发的两个圣杯。
       
本文将讲解Spring在不同环境下提供JTA事务的配置过程，这包括：Spring中直接集成JOTM提供JTA事务管理、将JOTM集成到Tomcat中，Spring通过引用Tomcat JNDI数据源提供JTA事务管理、引用其他功能完善JavaEE应用服务器所提供的JTA事务管理。
　　通过集成JOTM，直接在Spring中使用JTA事务
　 　JOTM(Java Open Transaction Manager)是ObjectWeb的一个开源JTA实现，它本身也是开源应用程序服务器JOnAS(Java Open Application Server)的一部分，为其提供JTA分布式事务的功能。
　　Spring 2.0附带的依赖类库中虽然包含jotm类库，但是并不完整，你可以到[http://jotm.objectweb.org](http://jotm.objectweb.org/)下载完全版的JOTM。
　　Spring为JOTM提供了一个org.springframework.transaction.jta.JotmFactoryBean支持类，通过该支持类可以方便地创建JOTM本地实例。
 下面，我们通过配置，使上节中BbtForumImpl#addTopic()方法工作在JTA事务的环境下。addTopic()内部使用两个DAO类（TopicDao和PostDao）分别访问不同数据库中的表。通过下面的步骤说明了使addTopic()方法拥有JTA事务的整个过程：
　　1． 将JOTM以下类库添加到类路径中：
　　jotm.jar
　　xapool.jar
　　jotm_jrmp_stubs.jar
　　jta-spec1_0_1.jar
　　connector-1_5.jar
　　 2． 编写JOTM配置文件，放到类路径下
　　carol.properties
　　#JNDI调用协议
　　carol.protocols=jrmp
　　#不使用CAROL JNDI封装器
　　carol.start.jndi=false
　　#不启动命名服务器
　　carol.start.ns=false
　　 3． 在[MySQL](http://lib.csdn.net/base/14)上建立两个数据库
　　在MySQL数据库中运行SQL脚本，建立topicdb和postdb两个数据库，在topicdb数据库中创建t_topic表，在postdb数据库中创建t_post表。我们希望在这两个数据库上进行JTA事务。SQL脚本如下所示：
```
DROP DATABASE IF EXISTS topicdb;
CREATE DATABASE topicdb DEFAULT CHARACTER SET utf8;
USE topicdb;
drop table if exists t_topic;
create table t_topic
(
topic_id int(11) not null auto_increment,
forum_id int(11) not null default 0,
topic_title varchar(100) not null default '',
user_id int(11) not null default 0,
topic_time datetime default NULL,
topic_views int(11) default 1,
topic_replies int(11) default 0,
primary key (topic_id)
);
create index IDX_TOPIC_USER_ID on t_topic
(
user_id
);
```
```
```
```
DROP DATABASE IF EXISTS postdb;
CREATE DATABASE postdb DEFAULT CHARACTER SET utf8;
USE postdb;
create table t_post
(
post_id int(11) not null auto_increment,
topic_id int(11) not null default 0,
forum_id int(11) not null default 0,
user_id int(11) not null default 0,
post_text text,
post_attach blob,
post_time datetime default NULL,
primary key (post_id)
);
create index IDX_POST_TOPIC_ID on t_post
(
topic_id
);
```
　　 4． 在Spring配置文件中配置JOTM
代码清单 1 applicationContext-jta.xml
　　…
　　<bean id="jotm" class="org.springframework.transaction.jta.JotmFactoryBean" />①JOTM本地实例
　　②JTA事务管理器
　　<bean id="txManager" class="org.springframework.transaction.jta.JtaTransactionManager">
　　<property name="userTransaction" ref="jotm" /> ②-1:指定userTransaction属性
　　</bean>
　　③XAPool配置，内部包含了一个XA数据源，对应topicdb数据库
　　<bean id="topicDS" class="org.enhydra.jdbc.pool.StandardXAPoolDataSource"
　　destroy-method="shutdown">
　　<property name="dataSource">
　　③-1:内部XA数据源
　　<bean class="org.enhydra.jdbc.standard.StandardXADataSource"
　　destroy-method="shutdown">
　　<property name="transactionManager" ref="jotm" />
　　<property name="driverName" value="com.MySQL.jdbc.Driver" />
　　<property name="url" value="jdbc:MySQL://localhost:3309/topicdb" />
　　</bean>
　　</property>
　　<property name="user" value="root" />
　　<property name="password" value="1234" />
　　</bean>
　　④按照③相似的方式配置另一个XAPool，对应postdb数据库，
```
<bean id="postDS" class="org.enhydra.jdbc.pool.StandardXAPoolDataSource"
destroy-method="shutdown">
< property name="dataSource">
< bean class="org.enhydra.jdbc.standard.StandardXADataSource"
destroy-method="shutdown">
< property name="transactionManager" ref="jotm" />
< property name="driverName" value="com.mysql.jdbc.Driver" />
< property name="url" value="jdbc:mysql://localhost:3309/postdb" />
< /bean>
< /property>
< property name="user" value="root" />
< property name="password" value="1234" />
< /bean>
```
⑤配置访问topicDB数据源的Spring JDBC模板
　　<bean id="topicTemplate"
　　class="org.springframework.jdbc.core.JdbcTemplate">
　　<property name="dataSource" ref="topicDS" />
　　</bean>
　　⑥配置访问postDB数据源的Spring JDBC模板
　　<bean id="postTemplate"
　　class="org.springframework.jdbc.core.JdbcTemplate">
　　<property name="dataSource" ref="postDS" />
　　</bean>
　　⑦基于topicTemplate数据源的topicDao
　　<bean id="topicDao" class="com.baobaotao.dao.jdbc.TopicJdbcDao">
　　<property name="jdbcTemplate" ref="topicTemplate" />
　　</bean>
　　⑧基于postTemplate数据源的postDao
　　<bean id="postDao" class="com.baobaotao.dao.jdbc.PostJdbcDao">
　　<property name="jdbcTemplate" ref="postTemplate" />
　　</bean>
　　⑨进行跨数据库JTA事务的业务类
　　<bean id="bbtForum" class="com.baobaotao.service.impl.BbtForumImpl">
　　<property name="topicDao" ref="topicDao" />
　　<property name="postDao" ref="postDao" />
　　</bean>
 ⑩对BbtForumImpl业务类中的@Transaction注解进行驱动
　　<tx:annotation-driven transaction-manager="txManager" />
 首 先，我们在①处通过Spring所提供的JotmFactoryBean创建一个本地JOTM实例，该实例同时实现了　 javax.transaction.UserTransaction和javax.transaction.TransactionManager接 口，它可以和ObjectWeb的XAPool一起工作。
　 　JTA事务管理器通过userTransaction属性引用本地JOTM实例，Spring的JtaTransactionManager会自动探测 到传入的javax.transaction.UserTransaction引用也实现了 javax.transaction.TransactionManager，所以我们无需再配置JtaTransactionManager的
 transactionManager属性，如②所示。
　　 在Spring中配置JOTM的另一个关键问题是配置XAPool，支持JTA事务的数据源必须封装成XAPool。首先，我们通过 org.enhydra.jdbc.standard.StandardXADataSource 配置一个XA数据源，它指向topicdb数据库，如③-1所示。而后，通过
 org.enhydra.jdbc.pool.StandardXAPoolDataSource将其封装成一个XAPool，如③所示。按照相同的方 式，配置指向postdb数据库的XAPool，如④所示。
　　 接下来的配置就顺理成章了，分别使用Spring JDBC的模板类配置DAO类，然后再配置引用DAO类的业务类。关于Spring JDBC的详细内容，参见第10章的内容。
 这里，我们使用@Transaction注解对业务类BbtForumImpl进行事务声明，所以通过<tx:annotation-driven/>对此进行驱动，BbtForumImpl的代码如下所示：
　　代码清单 2 BbtForumImpl
```
package com.baobaotao.service.impl;
import org.springframework.transaction.annotation.Transactional;
import com.baobaotao.dao.PostDao;
import com.baobaotao.dao.TopicDao;
import com.baobaotao.domain.Forum;
import com.baobaotao.domain.Topic;
import com.baobaotao.service.BbtForum;
@Transactional ①事务注解，以便Spring动态织入事务管理功能
public class BbtForumImpl implements BbtForum ...{
private TopicDao topicDao;
private PostDao postDao;
public void addTopic(Topic topic) throws Exception ...{ ②将方法将被施加JTA事务的增强
topicDao.addTopic(topic);
postDao.addPost(topic.getPost());
}
}
```
　　BbtForumImpl将Dao类组织起来，PostDao和TopicDao分别访问不同数据库中表，通过Spring注解驱动事务切面的增强后，它们将工作于同一个JTA事务中。
　　5． 在Spring中运行测试
　　代码清单 3 TestBbtForumJta
```
package com.baobaotao.service;
import org.springframework.test.AbstractDependencyInjectionSpringContextTests;
…
public class TestBbtForumJta extends AbstractDependencyInjectionSpringContextTests...{
private BbtForum bbtForum;
private final Logger logger = Logger.getLogger(getClass());
public void setBbtForum(BbtForum bbtForum) ...{
this.bbtForum = bbtForum;
}
protected String[] getConfigLocations() ...{
return new String[]...{"classpath:applicationContext-jta.xml"};
}
public void testAddPost() throws Exception...{
logger.info("begin........");
Topic topic = new Topic();
topic.setTopicTitle("Title -pfb");
Post post = new Post();
post.setPostText("post content -pfb");
topic.setPost(post);
bbtForum.addTopic(topic); ①使用了JTA事务的业务方法
logger.info("end........");
}
}
```
　 　通过Spring测试类AbstractDependencyInjectionSpringContextTests的支持，很容易编写一个测试类， 对启用了JTA事务的BbtForum#addTopic()方法进行测试。建议你将Log4J设置为DEBUG，这样就可以通过丰富的输出日志观测到
 JTA事务的执行情况。运行这个测试类后，你将可以看到JTA事务被正确实施。
　　Spring引用Tomcat的 JTA事务
 Tomcat是Servlet容器，但它提供了JNDI的实现，因此用户可以象在Java EE应用程序服务器中一样，在Tomcat中使用JNDI查找JDBC数据源。在事务处理方面，Tomcat本身并不支持JTA，但是可以通过集成JOTM达到目的。
 如果你的应用最终部署到一个功能齐备的Java EE应用服务器上，也许你更希望使用Java EE应用服务器的JTA功能，这样可以利用应用服务器本身许多优化措施。下面，我们让Tomcat通过JNDI开放JOTM的JTA的数据源，进而在Spring容器引用这个JNDI数据源，并在此基础上提供JTA事务。我们所使用的环境是：Tomcat
 5.5+JOTM 2.3。
　　1． 添加所需的JAR文件
　　将JOTM以下类包添加到<Tomcat安装目录>/common/lib目录中：
　　jotm.jar
　　jotm_jrmp_stubs.jar
　　jotm_iiop_stubs.jar
　　ow_carol.jar
　　jta-spec1_0_1.jar
　　jts1_0.jar
　　objectweb-datasource.jar
　　xapool.jar
　　howl.jar
　　connector-1_5.jar
　　同时，还需要添加相应数据库的JDBC驱动类包，例如MySQL的mysql.jar。
　　2． 配置JOTM
　　新建一个carol.properties配置文件，放置到<Tomcat安装目录>/common/classes目录下，配置文件内容如下：
　　#JNDI调用协议
　　carol.protocols=jrmp
　　# 本地RMI调用
　　carol.jvm.rmi.local.call=true
　# 不使用CAROL的JNDI封装器
　　carol.start.jndi=false
　　# 不启用命名服务器
　　carol.start.ns=false
　　# 命名工厂类
　　carol.jndi.java.naming.factory.url.pkgs=org.apache.naming
　　将carol.start.jndi设置为false，让JOTM不使用CAROL JNDI wrapper，从而可以避免类装载错误的发生。
　　3． 配置Tomcat环境，配置JNDI的数据源
　　在<Tomcat安装目录>/conf/context.xml文件中添加以下内容：
　　<Resource name="jdbc/topicDS" auth="Container" type="javax.sql.DataSource" ①-1:JNDI数据源
　　factory="org.objectweb.jndi.DataSourceFactory"
　　username="root" password="1234"
　　driverClassName="com.mysql.jdbc.Driver"
　　url="jdbc:mysql://localhost:3309/topicdb"
　　maxActive="30" maxIdle="30"/>
　　<Resource name="jdbc/postDS" auth="Container" type="javax.sql.DataSource" ①-2:JNDI数据源
　　factory="org.objectweb.jndi.DataSourceFactory"
　　username="root" password="1234"
　　driverClassName="com.mysql.jdbc.Driver"
　　url="jdbc:mysql://localhost:3309/postdb"
　　maxActive="30" maxIdle="30"/>
　　②JOTM JTA事务管理
　　<Transaction factory="org.objectweb.jotm.UserTransactionFactory" jotm.timeout="60"/>
　　在Tomcat中配置两个JNDI数据源，它们分别指向topicdb和postdb数据库，如①处所示。最后配置JOTM的JTA事务管理器，该事务管理器自动对两个JNDI中的数据源应用JTA事务。
　　 4． Spring中相应的配置
　　让应用服务器提供JNDI数据源管理和JTA事务后，Spring肩上的担子减轻了许多，Spring要做的只是简单地引用JNDI的数据源，并启用JtaTransactionManager就可以了。
代码清单 4 applicationContext-jta-tomcat.xml：使用应用服务器的JTA支持
```
<beans xmlns="http://www.springframework.org/schema/beans"
xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
xmlns:tx="http://www.springframework.org/schema/tx"
xmlns:jee="http://www.springframework.org/schema/jee"
xsi:schemaLocation="http://www.springframework.org/schema/beans
http://www.springframework.org/schema/beans/spring-beans-2.0.xsd
http://www.springframework.org/schema/tx http://www.springframework.org/schema/tx/spring-tx-2.0.xsd
http://www.springframework.org/schema/jee http://www.springframework.org/schema/jee/spring-jee-2.0.xsd">
```
　①使用Tomcat JNDI的数据源
```
<jee:jndi-lookup id="topicDS" jndi-name="java:comp/env/jdbc/topicDS" />
< jee:jndi-lookup id="postDS" jndi-name="java:comp/env/jdbc/postDS" />
< bean id="topicTemplate" class="org.springframework.jdbc.core.JdbcTemplate">
< property name="dataSource" ref="topicDS" />
< /bean>
< bean id="postTemplate" class="org.springframework.jdbc.core.JdbcTemplate">
< property name="dataSource" ref="postDS" />
< /bean>
< bean id="topicDao" class="com.baobaotao.dao.jdbc.TopicJdbcDao">
< property name="jdbcTemplate" ref="topicTemplate" />
< /bean>
< bean id="postDao" class="com.baobaotao.dao.jdbc.PostJdbcDao">
< property name="jdbcTemplate" ref="postTemplate" />
< /bean>
< bean id="bbtForum" class="com.baobaotao.service.impl.BbtForumImpl">
< property name="topicDao" ref="topicDao" />
< property name="postDao" ref="postDao" />
< /bean>
```
 ②只需要指定一个JTA事务管理器就可以了，Spring会自动使用Tomcat中的JTA事务功能
```
<bean id="txManager" class="org.springframework.transaction.jta.JtaTransactionManager"/>
< tx:annotation-driven transaction-manager="txManager" />
< /beans>
```
　　在①处，我们通过Spring jee命名空间提供的<jee:jndi-lookup>标签获取应用服务器中的JNDI资源，并将它们声明为一个Bean以供持久化模板类引用。
 和直接在Spring中使用JOTM不一样，这时，我们仅需要简单地配置一个JtaTransactionManager就可以了，该事务管理器将自动将JTA事务委托给应用程序器。
 Spring引用Java EE应用服务器 JTA事务功能和 Tomcat+JOTM提供JTA事务功能的配置步骤基本相似，相信大家可以对照这个实例完成相应的配置。另外，这里的实例采用了Spring JDBC作为持久层实现技术，你完全可以通过少量的调整将其应用到JPA、Hibernate、iBatis等持久化实现技术中。
　　 在特定应用服务器使用JTA
　 　一般来说，Spring的事务抽象与应用服务器是无关的。不过，如果你如果希望事务管理器使用特定的UserTransaction 和 TransactionManager 对象（可以被自动探测），以获取更多的增强事务语义。这时，针对不同的Java EE应用服务器，Spring的事务管理器可以采取不同的配置。
　　 BEA WebLogic
 在一个使 用WebLogic 7.0、8.1或更高版本的环境中，你一般会优先选用特定于WebLogic的 WebLogicJtaTransactionManager 类来取代基础的 JtaTransactionManager 类，因为在WebLogic环境中，该类提供了对Spring事务定义的完全支持，超过了标准的JTA语义。你可以使用以下的配置达到目的：
　　<bean id="txManager" class="org.springframework.transaction.jta.WebLogicJtaTransactionManager"/>
 它的特性包括：支持事务名，支持为每个事务定义隔离级别，以及在任何环境下正确地恢复事务的能力。
　　 IBM WebSphere
　 　在WebSphere 5.1、5.0和4.x环境下，你可以使用Spring的 WebSphereTransactionManagerFactoryBean 类。这是一个工厂类，通过WebSphere的 静态访问方法（每个版本的WebSphere中都不同）获取到JTA TransactionManager
 实例。一旦通过工厂bean获取到JTA TransactionManager 实例，就可以使用该实例装配一个Spring的  JtaTransactionManager bean，它封装了JTA UserTransaction，提供增强的事务语义。你可以按以下方式进行配置：
```
<bean id="wsJtaTm" class="org.springframework.transaction.jta.WebSphereTransactionManagerFactoryBean"/>
< bean id="transactionManager" class="org.springframework.transaction.jta.JtaTransactionManager">
< property name="transactionManager ref="wsJtaTm"/>①引用WebSphere的JTA事务管理器
</bean>
```
　　小结
你既可以在没有任务应用服务器支持的情况下，直接通过集成JOTM在Spring中使用JTA事务管理，也可以通过引用Java EE应用服务器的JNDI数据源，利用应用服务器提供的JTA事务功能间接实现Spring 中的JTA事务管理。为了利用一些高级Java EE应用服务器的JTA事务高级功能，你可以通过Spring所提供的特定于应用服务器的JTA事务管理器进行配置
