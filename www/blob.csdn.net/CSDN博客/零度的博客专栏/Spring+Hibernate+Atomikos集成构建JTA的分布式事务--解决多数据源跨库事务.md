# Spring+Hibernate+Atomikos集成构建JTA的分布式事务--解决多数据源跨库事务 - 零度的博客专栏 - CSDN博客
2016年04月06日 14:54:54[零度anngle](https://me.csdn.net/zmx729618)阅读数：2846
**一、概念**
**分布式事务**
分布式事务是指事务的参与者、支持事务的服务器、资源服务器以及事务管理器分别位于不同的分布式系统的不同节点之上。简言之，同时操作多个数据库保持事务的统一，达到跨库事务的效果。
**JTA**
JTA，即Java Transaction API，JTA允许应用程序执行分布式事务处理———在两个或多个网络计算机资源上访问并且更新数据。JDBC驱动程序的JTA支持极大地增强了数据访问能力。
**JTA和JTS**
Java事务API（JTA：Java Transaction API）和它的同胞Java事务服务（JTS：Java Transaction Service），为J2EE平台提供了分布式事务服务（distributed transaction）。
一个分布式事务（distributed transaction）包括一个事务管理器（transaction manager）和一个或多个资源管理器(resource manager）。
一个资源管理器（resource manager）是任意类型的持久化数据存储。
事务管理器（transaction manager）承担着所有事务参与单元者的相互通讯的责任。
**JTA与JDBC**
JTA事务比JDBC事务更强大。一个JTA事务可以有多个参与者，而一个JDBC事务则被限定在一个单一的数据库连接。下列任一个Java平台的组件都可以参与到一个JTA事务中：JDBC连接、JDO PersistenceManager 对象、JMS 队列、JMS 主题、企业JavaBeans（EJB）、一个用J2EE Connector Architecture 规范编译的资源分配器。
**JOTM**
JOTM (Java Open Transaction Manager)是一个开源独立的事务管理器，支持分布式事务，Spring3.X已经不再支持JOTM。
**Atomikos**
Atomikos TransactionsEssentials是一个为Java平台提供增值服务的并且开源类事务管理器，支持分布式事务，本文所采用的技术。
Atomikos是目前在分布式事务管理中做得相当不错的开源软件。有10年以上的经验，Atomikos保障您的关键事务和防止昂贵的数据丢失在发生系统故障或事故中．Atomikos支持XA(全局事务)和NON-XA(非全局事务)，NON-XA效率高于XA。本文主要是讲XA事件，因为要在不同的数据库中操作多张表。
接下来说一下Spring.x+Hibernate中集成Atomikos构建JTA的分布式事务
**二、Spring.x+Hibernate+Atomikos集成**
Spring.x+Hibernate怎么集成此处省略不说，主要讲解Spring.x+Atomikos的集成，操作步骤如下：
1、下载Atomikos，需要以下这些包
```
atomikos-util-1.0.jar
cglib-nodep-2.2.2.jar
transactions-3.7.0.jar
transactions-api-3.7.0.jar
transactions-jdbc-3.7.0.jar
transactions-jta-3.7.0.jar
```
2、配置
在applicationContext.xml文件当中作如下配置 
```
[]()
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xmlns:context="http://www.springframework.org/schema/context"
    xmlns:aop="http://www.springframework.org/schema/aop"
    xmlns:tx="http://www.springframework.org/schema/tx"
    xmlns:jaxws="http://cxf.apache.org/jaxws"
    xsi:schemaLocation="http://www.springframework.org/schema/beans
           http://www.springframework.org/schema/beans/spring-beans-2.5.xsd
           http://www.springframework.org/schema/context
           http://www.springframework.org/schema/context/spring-context-2.5.xsd
           http://www.springframework.org/schema/aop
           http://www.springframework.org/schema/aop/spring-aop-2.5.xsd
           http://www.springframework.org/schema/tx 
           http://www.springframework.org/schema/tx/spring-tx-2.5.xsd
           http://cxf.apache.org/jaxws 
           http://cxf.apache.org/schemas/jaxws.xsd">
    <context:annotation-config />
    
    <!-- 通过注解方式自动扫描需交给sping管理的Bean -->
    <context:component-scan base-package="com.ljq" />
    
    <!-- spring atomikos配置 -->
    <!-- bpm数据源 -->
    <bean id="bpmDataSource" class="com.atomikos.jdbc.AtomikosDataSourceBean" 
        init-method="init" destroy-method="close">
        <!-- Set unique name for this DataSource -->  
        <property name="uniqueResourceName"><value>bpm</value></property>
        <!-- Set XADatasource class name-->  
        <property name="xaDataSourceClassName" value="com.mysql.jdbc.jdbc2.optional.MysqlXADataSource" />
        <property name="xaProperties">
            <props>
                <prop key="user">root</prop>
                <prop key="password">parami2013</prop>
                <prop key="url">jdbc:mysql://192.168.1.254:3306/parami_bpm</prop>
            </props>
        </property>
        <!-- set properties for datasource connection pool -->  
        <property name="poolSize" value="3" />
        <!-- 管理 Connection 被占用的时间 -->
        <!-- 如果不设置这个值，Atomikos使用默认的300秒(即5分钟)，那么在处理大批量数据读取的时候，一旦超过5分钟，就会抛出类似 Resultset is close 的错误 -->
        <property name="reapTimeout"><value>20000</value></property>  
    </bean>
    <!-- center数据源 -->
    <bean id="centerDataSource" class="com.atomikos.jdbc.AtomikosDataSourceBean" 
        init-method="init" destroy-method="close">
        <property name="uniqueResourceName"><value>center</value></property>
        <property name="xaDataSourceClassName" value="com.mysql.jdbc.jdbc2.optional.MysqlXADataSource" />
        <property name="xaProperties">
            <props>
                <prop key="user">root</prop>
                <prop key="password">parami2013</prop>
                <prop key="url">jdbc:mysql://192.168.1.254:3306/parami_center</prop>
            </props>
        </property>
        <!-- 连接池里面连接的个数 -->
        <property name="poolSize" value="3" />
        <!-- 管理 Connection 被占用的时间 -->
        <!-- 如果不设置这个值，Atomikos使用默认的300秒(即5分钟)，那么在处理大批量数据读取的时候，一旦超过5分钟，就会抛出类似 Resultset is close 的错误 -->
        <property name="reapTimeout"><value>20000</value></property>  
    </bean>
    
    <!-- 将hibernate数据源注入sessionFactory -->
    <bean id="bpmSessionFactory"
        class="org.springframework.orm.hibernate3.annotation.AnnotationSessionFactoryBean">
        <property name="dataSource" ref="bpmDataSource" />
        <property name="packagesToScan">
            <list>
                <value>com.ljq.pojo</value>
            </list>
        </property>
        <property name="hibernateProperties">
            <props>
                <prop key="hibernate.dialect">org.hibernate.dialect.MySQLDialect</prop>
                <prop key="hibernate.show_sql">true</prop>
                <!-- 慎重(推荐禁止) 固定为update -->
                <prop key="hibernate.hbm2ddl.auto">update</prop>
                <prop key="hibernate.cache.provider_class">org.hibernate.cache.HashtableCacheProvider</prop>
            </props>
        </property>
    </bean>
    
    <!-- 将hibernate数据源注入sessionFactory -->
    <bean id="centerSessionFactory"
        class="org.springframework.orm.hibernate3.annotation.AnnotationSessionFactoryBean">
        <property name="dataSource" ref="centerDataSource" />
        <property name="packagesToScan">
            <list>
                <value>com.ljq.pojo</value>
            </list>
        </property>
        <property name="hibernateProperties">
            <props>
                <prop key="hibernate.dialect">org.hibernate.dialect.MySQLDialect</prop>
                <prop key="hibernate.show_sql">true</prop>
                <!-- 慎重(推荐禁止) 固定为update -->
                <prop key="hibernate.hbm2ddl.auto">update</prop>
                <prop key="hibernate.cache.provider_class">org.hibernate.cache.HashtableCacheProvider</prop>
            </props>
        </property>
    </bean>    
    <!-- atomikos事务管理器 -->
    <bean id="atomikosTransactionManager" class="com.atomikos.icatch.jta.UserTransactionManager" 
        init-method="init" destroy-method="close">
        <property name="forceShutdown">
            <value>true</value>
        </property>
    </bean>
    <bean id="atomikosUserTransaction" class="com.atomikos.icatch.jta.UserTransactionImp">
        <property name="transactionTimeout" value="300" />
    </bean>
    <!-- spring 事务管理器 -->
    <bean id="springTransactionManager"
        class="org.springframework.transaction.jta.JtaTransactionManager">
        <property name="transactionManager" ref="atomikosTransactionManager"/>
        <property name="userTransaction" ref="atomikosUserTransaction" />
        <property name="allowCustomIsolationLevels" value="true"/> 
    </bean>
    <!-- 使用annotation定义事务，对于要加入事物的类，只需对该类加 @Transactional  -->
    <tx:annotation-driven transaction-manager="springTransactionManager" />
    <!-- hibernate Dao层模板 -->
    <bean id="bpmHibernateTemplate"
        class="org.springframework.orm.hibernate3.HibernateTemplate">
        <property name="sessionFactory" ref="bpmSessionFactory"></property>
    </bean>
    
    <bean id="centerHibernateTemplate"
        class="org.springframework.orm.hibernate3.HibernateTemplate">
        <property name="sessionFactory" ref="centerSessionFactory"></property>
    </bean>
    
</beans>
[]()
```
3、在src文件夹下面加入一个transactions.properties文件
```
[]()
# SAMPLE PROPERTIES FILE FOR THE TRANSACTION SERVICE
# THIS FILE ILLUSTRATES THE DIFFERENT SETTINGS FOR THE TRANSACTION MANAGER
# UNCOMMENT THE ASSIGNMENTS TO OVERRIDE DEFAULT VALUES;
# Required: factory implementation class of the transaction core.
# NOTE: there is no default for this, so it MUST be specified! 
# 
com.atomikos.icatch.service=com.atomikos.icatch.standalone.UserTransactionServiceFactory
        
# Set base name of file where messages are output 
# (also known as the 'console file').
#
# com.atomikos.icatch.console_file_name = tm.out
# Size limit (in bytes) for the console file;
# negative means unlimited.
#
# com.atomikos.icatch.console_file_limit=-1
# For size-limited console files, this option
# specifies a number of rotating files to 
# maintain.
#
# com.atomikos.icatch.console_file_count=1
# Set the number of log writes between checkpoints
#
# com.atomikos.icatch.checkpoint_interval=500
# Set output directory where console file and other files are to be put
# make sure this directory exists!
#
# com.atomikos.icatch.output_dir = ./
# Set directory of log files; make sure this directory exists!
#
# com.atomikos.icatch.log_base_dir = ./
# Set base name of log file
# this name will be  used as the first part of 
# the system-generated log file name
#
# com.atomikos.icatch.log_base_name = tmlog
# Set the max number of active local transactions 
# or -1 for unlimited.
#
# com.atomikos.icatch.max_actives = 50
# Set the default timeout (in milliseconds) for local transactions
#
# com.atomikos.icatch.default_jta_timeout = 10000
# Set the max timeout (in milliseconds) for local transactions
#
# com.atomikos.icatch.max_timeout = 300000
# The globally unique name of this transaction manager process
# override this value with a globally unique name
#
# com.atomikos.icatch.tm_unique_name = tm
    
# Do we want to use parallel subtransactions? JTA's default
# is NO for J2EE compatibility
#
# com.atomikos.icatch.serial_jta_transactions=true
                    
# If you want to do explicit resource registration then
# you need to set this value to false.
#
# com.atomikos.icatch.automatic_resource_registration=true  
    
# Set this to WARN, INFO or DEBUG to control the granularity
# of output to the console file.
#
# com.atomikos.icatch.console_log_level=WARN
    
# Do you want transaction logging to be enabled or not?
# If set to false, then no logging overhead will be done
# at the risk of losing data after restart or crash.
#
# com.atomikos.icatch.enable_logging=true
# Should two-phase commit be done in (multi-)threaded mode or not?
# Set this to false if you want commits to be ordered according
# to the order in which resources are added to the transaction.
#
# NOTE: threads are reused on JDK 1.5 or higher. 
# For JDK 1.4, thread reuse is enabled as soon as the 
# concurrent backport is in the classpath - see 
# http://mirrors.ibiblio.org/pub/mirrors/maven2/backport-util-concurrent/backport-util-concurrent/
#
# com.atomikos.icatch.threaded_2pc=false
# Should shutdown of the VM trigger shutdown of the transaction core too?
#
# com.atomikos.icatch.force_shutdown_on_vm_exit=false
[]()
```
4、注入HibernateTemplate模板 
a、为bpm库注入HibernateTemplate模板
```
[]()
public abstract class BpmDaoSupport<T> extends DaoSupport<T> {
    protected HibernateTemplate bpmHibernateTemplate;
    /**
     * HibernateTemplate注入
     * 
     * @param bpmHibernateTemplate
     */
    @Resource
    public void setBpmHibernateTemplate(HibernateTemplate bpmHibernateTemplate) {
        this.bpmHibernateTemplate = bpmHibernateTemplate;
        setHibernateTemplate(bpmHibernateTemplate);
    }
}
[]()
```
b、为center库注入HibernateTemplate模板
```
[]()
public abstract class CenterDaoSupport<T> extends DaoSupport<T> {
    protected HibernateTemplate centerHibernateTemplate;
    /**
     * HibernateTemplate注入
     * 
     * @param centerHibernateTemplate
     */
    @Resource
    public void setCenterHibernateTemplate(HibernateTemplate centerHibernateTemplate) {
        this.centerHibernateTemplate = centerHibernateTemplate;
        setHibernateTemplate(centerHibernateTemplate);
    }
}
[]()
```
5、编写业务逻辑代码
```
[]()
@Service(value="userServiceBean")
@Transactional
public class UserServiceBean implements UserService {
    @Resource(name="userDaoBean") private UserDao userDao;
    @Resource(name="userRoleDaoBean") private UserRoleDao userRoleDao;
    
    public List<User> queryUser() {
        return userDao.queryUser();
    }
    
    public void save(User user, UserRole ur) {
        userDao.save(user); //a处
        //此处报异常
        System.out.println(1/0); //b处
        ur.setUserId(user.getUid());
        userRoleDao.save(ur); //c处
    }
}
[]()
```
在执行save()方法时会报java.lang.ArithmeticException: / by zero异常，b处的“1/0”有问题，因为除数不能为0；
如果事务生效，save()方法会回滚掉，即a处、c处不做save操作，保持事务的原子性；
如果事务不起作用，那么a处会执行成功，往数据库添加一条记录；b处抛出异常终止向下执行；c处这条代码不执行，导致脏数据的出现。
Atomikos的集成到此就结束了，亲！是不是很简单。
**注意**
mysql的驱动需要5.1.10版本以上，低版本会出现如下错误：
java.lang.IllegalArgumentException: null source
