# Spring分布式事务- 三种实现方式(Spring+JTA) - z69183787的专栏 - CSDN博客
2016年09月13日 18:14:45[OkidoGreen](https://me.csdn.net/z69183787)阅读数：11324
分布式事务是指事务的参与者、支持事务的服务器、资源管理器以及事务管理器分别位于分布系统的不同节点之上，在两个或多个网络计算机资源上访问并且更新数据，将两个或多个网络计算机的数据进行的多次操作作为一个整体进行处理。如不同银行账户之间的转账。
对于在项目中接触到JTA，大部分的原因是因为在项目中需要操作多个数据库，同时，可以保证操作的原子性，保证对多个数据库的操作一致性。 
项目结构图![这里写图片描述](http://images2015.cnblogs.com/blog/736676/201601/736676-20160115175245741-1627827586.png)
1、pom.xml
```
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/maven-v4_0_0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <packaging>war</packaging>
    <name>jta</name>
    <groupId>com.roden.jta</groupId>
    <artifactId>jta</artifactId>
    <version>0.0.1-SNAPSHOT</version>   
    <dependencies>  
        <dependency>
            <groupId>junit</groupId>
            <artifactId>junit</artifactId>
            <version>4.12</version>
            <!-- 表示开发的时候引入，发布的时候不会加载此包 -->
            <scope>test</scope>
        </dependency>               
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-test</artifactId>
            <version>3.2.14.RELEASE</version>
            <scope>provided</scope>
        </dependency>
        <dependency>
            <groupId>mysql</groupId>
            <artifactId>mysql-connector-java</artifactId>
            <version>5.1.34</version>
        </dependency>       
        <dependency>
            <groupId>com.atomikos</groupId>
            <artifactId>transactions-jdbc</artifactId>
            <version>3.9.3</version>
        </dependency>   
        <dependency>
            <groupId>javax.transaction</groupId>
            <artifactId>jta</artifactId>
            <version>1.1</version>
        </dependency>   
        <!-- javaee-api包含了JavaEE规范中的api,如servlet-api,persistence-api, transaction-api等 -->
        <dependency>
            <groupId>javax</groupId>
            <artifactId>javaee-api</artifactId>
            <version>7.0</version>
        </dependency>       
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-context</artifactId>
            <version>3.2.16.RELEASE</version>
        </dependency>       
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-jdbc</artifactId>
            <version>3.2.16.RELEASE</version>
        </dependency>       
    </dependencies>
</project>
```
2、applicationContext.xml
```
<?xml version="1.0" encoding="UTF-8"?>
<beans  xmlns="http://www.springframework.org/schema/beans"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
    xmlns:jee="http://www.springframework.org/schema/jee"
    xmlns:tx="http://www.springframework.org/schema/tx" 
    xmlns:context="http://www.springframework.org/schema/context"
    xsi:schemaLocation="http://www.springframework.org/schema/beans 
    http://www.springframework.org/schema/beans/spring-beans-3.0.xsd 
    http://www.springframework.org/schema/tx 
    http://www.springframework.org/schema/tx/spring-tx-3.0.xsd 
    http://www.springframework.org/schema/jee 
    http://www.springframework.org/schema/jee/spring-jee-3.0.xsd 
    http://www.springframework.org/schema/context 
    http://www.springframework.org/schema/context/spring-context-3.0.xsd"
    default-lazy-init="true">   
    <!-- mysql数据源 -->
    <bean id="master" class="com.atomikos.jdbc.AtomikosDataSourceBean"
        init-method="init" destroy-method="close">
        <description>master xa datasource</description>
        <property name="uniqueResourceName">
            <value>master</value>
        </property>
        <property name="xaDataSourceClassName" value="com.mysql.jdbc.jdbc2.optional.MysqlXADataSource" />
        <property name="xaProperties">
            <props>
                <prop key="user">root</prop>
                <prop key="password"></prop>
                <prop key="URL">jdbc:mysql://127.0.0.1:3306/master</prop>
            </props>
        </property>     
        <property name="poolSize" value="10"/> 
    </bean>
    <bean id="slave" class="com.atomikos.jdbc.AtomikosDataSourceBean"
        init-method="init" destroy-method="close">
        <description>slave xa datasource</description>
        <property name="uniqueResourceName">
            <value>slave</value>
        </property>
        <property name="xaDataSourceClassName">
            <value>com.mysql.jdbc.jdbc2.optional.MysqlXADataSource</value>
        </property>
        <property name="xaProperties">
            <props>
                <prop key="user">root</prop>
                <prop key="password"></prop>
                <prop key="URL">jdbc:mysql://127.0.0.1:3306/slave</prop>
            </props>
        </property>     
        <property name="poolSize" value="10"/>
    </bean>
    <!-- atomikos事务管理器 -->
    <bean id="atomikosTransactionManager" class="com.atomikos.icatch.jta.UserTransactionManager"
        init-method="init" destroy-method="close">
        <description>UserTransactionManager</description>
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
        <property name="transactionManager">
            <ref bean="atomikosTransactionManager" />
        </property>
        <property name="userTransaction">
            <ref bean="atomikosUserTransaction" />
        </property>
    </bean>
    <!-- spring　事务模板 -->
    <bean id="transactionTemplate"
        class="org.springframework.transaction.support.TransactionTemplate">
        <property name="transactionManager">
            <ref bean="springTransactionManager" />
        </property>
    </bean>  
    <bean id="masterJdbcTemplate" class="org.springframework.jdbc.core.JdbcTemplate">
        <constructor-arg>
            <ref bean="master" />
        </constructor-arg>
    </bean>
    <bean id="slaveJdbcTemplate" class="org.springframework.jdbc.core.JdbcTemplate">
        <constructor-arg>
            <ref bean="slave" />
        </constructor-arg>
    </bean>
    <context:annotation-config />   
    <tx:annotation-driven transaction-manager="springTransactionManager"/>  
    <context:component-scan base-package="com.roden.jta" />   
</beans>
```
3、java类 
package com.roden.jta.controller;
```
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import com.roden.jta.service.TestService;
@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations = "classpath:applicationContext.xml")
@Controller
public class TestController {   
    @Autowired
    private TestService testService;
    //MySQL的数据库引擎必须是InnoDB，否则无法回滚
    @Test
    public void test(){
        testService.test();
    }
    @Test
    public void test2(){
        testService.update();
    }
    @Test
    public void test3(){
        testService.test3();
    }
}
package com.roden.jta.service;
public interface TestService {
    public String test();
    public String update();
     public void test3();
}
package com.roden.jta.service.impl;
import javax.annotation.Resource;
import javax.transaction.SystemException;
import javax.transaction.UserTransaction;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.TransactionStatus;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.transaction.jta.JtaTransactionManager;
import org.springframework.transaction.support.TransactionCallback;
import org.springframework.transaction.support.TransactionCallbackWithoutResult;
import org.springframework.transaction.support.TransactionTemplate;
import com.roden.jta.dao.TestMasterDao;
import com.roden.jta.dao.TestSlaveDao;
import com.roden.jta.service.TestService;
@Service
public class TestServiceImpl implements TestService{
    @Resource(name = "springTransactionManager")
    private JtaTransactionManager txManager;
    @Autowired
    private TestMasterDao testMasterDao;
    @Autowired
    private TestSlaveDao testSlaveDao;  
    @Resource(name = "transactionTemplate")
    private TransactionTemplate transactionTemplate;  
    //编程式
    @Override
    public String test() {
         UserTransaction userTx = txManager.getUserTransaction(); 
         try {               
             userTx.begin();     
             testMasterDao.master(); 
             testSlaveDao.slave();    
             int a=1/0;
             System.out.println(a);
             userTx.commit();
         } catch (Exception e) {
             System.out.println("捕获到异常，进行回滚" + e.getMessage());
             e.printStackTrace();
             try {
                 userTx.rollback();
             } catch (IllegalStateException e1) {
                System.out.println("IllegalStateException:" + e1.getMessage());
             } catch (SecurityException e1) {
                 System.out.println("SecurityException:" + e1.getMessage());
             } catch (SystemException e1) {
                 System.out.println("SystemException:" + e1.getMessage());
             }              
         }
        return null;
    }
    //声明式
    @Override
    @Transactional
    public String update(){
         testMasterDao.master();        
         testSlaveDao.slave();   
         //int a=1/0;
         //System.out.println(a);
        return null;
    }
    //事务模板方式
    @Override
     public void test3() {  
            transactionTemplate.execute(new TransactionCallbackWithoutResult(){  
                @Override  
                protected void doInTransactionWithoutResult(TransactionStatus status) {  
                    try {  
                         testMasterDao.master();        
                         testSlaveDao.slave();   
                         int a=1/0;
                        System.out.println(a);
                    } catch (Exception ex) {  
                        // 通过调用 TransactionStatus 对象的 setRollbackOnly() 方法来回滚事务。  
                        status.setRollbackOnly();  
                        ex.printStackTrace();  
                    }  
                }  
            });         
               /* 
                //有返回值的回调
                 Object obj=transactionTemplate.execute(new TransactionCallback(){
                    @Override
                    public Object doInTransaction(TransactionStatus status) {
                        return 1;
                    }  
                });  */
        }  
}
package com.roden.jta.dao;
public interface TestMasterDao {
    public String master();
    public String update();
}
package com.roden.jta.dao;
public interface TestSlaveDao {
    public String slave();
}
package com.roden.jta.dao.impl;
import javax.annotation.Resource;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Repository;
import com.roden.jta.dao.TestMasterDao;
@Repository
public class TestMasterDaoImpl implements TestMasterDao{
    @Resource(name="masterJdbcTemplate")
    JdbcTemplate masterJdbcTemplate;
    @Override
    public String master() {
        masterJdbcTemplate.execute("update teacher set name='master' where id=1");
        return "success";
    }
    @Override
    public String update() {
        masterJdbcTemplate.execute("update teacher set name='8' where id=1");
        System.out.println("update");
        masterJdbcTemplate.execute("fff teacher set name=''6' where id=1");
        return null;
    }
}
package com.roden.jta.dao.impl;
import javax.annotation.Resource;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Repository;
import com.roden.jta.dao.TestSlaveDao;
@Repository
public class TestSlaveDaoImpl implements TestSlaveDao{
    @Resource(name="slaveJdbcTemplate")
    JdbcTemplate slaveJdbcTemplate;
    @Override
    public String slave() {
        slaveJdbcTemplate.execute("update student set name='slave' where id=1");            
        return "success";
    }   
}
```
数据源详细参数配置
<code class="hljs xml has-numbering" style="display: block; padding: 0px; color: inherit; box-sizing: border-box; font-family: "Source Code Pro", monospace;font-size:undefined; white-space: pre; border-radius: 0px; word-wrap: normal; background: transparent;">    <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"><!-- 两个数据源的功用配置，方便下面直接引用 --></span>
     <span class="hljs-tag" style="color: rgb(0, 102, 102); box-sizing: border-box;"><<span class="hljs-title" style="box-sizing: border-box; color: rgb(0, 0, 136);">bean</span> <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">id</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"abstractXADataSource"</span> <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">class</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"com.atomikos.jdbc.AtomikosDataSourceBean"</span> <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">init-method</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"init"</span> 
             <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">destroy-method</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"close"</span> <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">abstract</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"true"</span>></span> 
        <span class="hljs-tag" style="color: rgb(0, 102, 102); box-sizing: border-box;"><<span class="hljs-title" style="box-sizing: border-box; color: rgb(0, 0, 136);">property</span> <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">name</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"xaDataSourceClassName"</span> <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">value</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"com.mysql.jdbc.jdbc2.optional.MysqlXADataSource"</span>/></span> 
        <span class="hljs-tag" style="color: rgb(0, 102, 102); box-sizing: border-box;"><<span class="hljs-title" style="box-sizing: border-box; color: rgb(0, 0, 136);">property</span> <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">name</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"poolSize"</span> <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">value</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"10"</span> /></span> 
        <span class="hljs-tag" style="color: rgb(0, 102, 102); box-sizing: border-box;"><<span class="hljs-title" style="box-sizing: border-box; color: rgb(0, 0, 136);">property</span> <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">name</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"minPoolSize"</span> <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">value</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"10"</span>/></span> 
        <span class="hljs-tag" style="color: rgb(0, 102, 102); box-sizing: border-box;"><<span class="hljs-title" style="box-sizing: border-box; color: rgb(0, 0, 136);">property</span> <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">name</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"maxPoolSize"</span> <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">value</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"30"</span>/></span> 
        <span class="hljs-tag" style="color: rgb(0, 102, 102); box-sizing: border-box;"><<span class="hljs-title" style="box-sizing: border-box; color: rgb(0, 0, 136);">property</span> <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">name</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"borrowConnectionTimeout"</span> <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">value</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"60"</span>/></span>  <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"><!--获取连接失败重新获等待最大时间，在这个时间内如果有可用连接，将返回--></span>
        <span class="hljs-tag" style="color: rgb(0, 102, 102); box-sizing: border-box;"><<span class="hljs-title" style="box-sizing: border-box; color: rgb(0, 0, 136);">property</span> <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">name</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"reapTimeout"</span> <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">value</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"20"</span>/></span> <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"><!--最大获取数据时间，如果不设置这个值，Atomikos使用默认的5分钟，那么在处理大批量数据读取的时候，一旦超过5分钟，就会抛出类似 Resultset is close 的错误.--></span>        
        <span class="hljs-tag" style="color: rgb(0, 102, 102); box-sizing: border-box;"><<span class="hljs-title" style="box-sizing: border-box; color: rgb(0, 0, 136);">property</span> <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">name</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"maxIdleTime"</span> <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">value</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"60"</span>/></span>    <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"><!--最大闲置时间，超过最小连接池连接的连接将将关闭--></span>
        <span class="hljs-tag" style="color: rgb(0, 102, 102); box-sizing: border-box;"><<span class="hljs-title" style="box-sizing: border-box; color: rgb(0, 0, 136);">property</span> <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">name</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"maintenanceInterval"</span> <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">value</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"60"</span> /></span>  <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"><!--连接回收时间--></span>    
        <span class="hljs-tag" style="color: rgb(0, 102, 102); box-sizing: border-box;"><<span class="hljs-title" style="box-sizing: border-box; color: rgb(0, 0, 136);">property</span> <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">name</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"loginTimeout"</span> <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">value</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"60"</span> /></span>     <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"><!--java数据库连接池，最大可等待获取datasouce的时间--></span>
        <span class="hljs-tag" style="color: rgb(0, 102, 102); box-sizing: border-box;"><<span class="hljs-title" style="box-sizing: border-box; color: rgb(0, 0, 136);">property</span> <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">name</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"logWriter"</span> <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">value</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"60"</span>/></span>
        <span class="hljs-tag" style="color: rgb(0, 102, 102); box-sizing: border-box;"><<span class="hljs-title" style="box-sizing: border-box; color: rgb(0, 0, 136);">property</span> <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">name</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"testQuery"</span>></span>
            <span class="hljs-tag" style="color: rgb(0, 102, 102); box-sizing: border-box;"><<span class="hljs-title" style="box-sizing: border-box; color: rgb(0, 0, 136);">value</span>></span>select 1<span class="hljs-tag" style="color: rgb(0, 102, 102); box-sizing: border-box;"></<span class="hljs-title" style="box-sizing: border-box; color: rgb(0, 0, 136);">value</span>></span>
        <span class="hljs-tag" style="color: rgb(0, 102, 102); box-sizing: border-box;"></<span class="hljs-title" style="box-sizing: border-box; color: rgb(0, 0, 136);">property</span>></span>
    <span class="hljs-tag" style="color: rgb(0, 102, 102); box-sizing: border-box;"></<span class="hljs-title" style="box-sizing: border-box; color: rgb(0, 0, 136);">bean</span>></span> 
    <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"><!-- 配置第一个数据源 --></span>
    <span class="hljs-tag" style="color: rgb(0, 102, 102); box-sizing: border-box;"><<span class="hljs-title" style="box-sizing: border-box; color: rgb(0, 0, 136);">bean</span> <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">id</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"dataSource"</span> <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">parent</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"abstractXADataSource"</span>></span>
         <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"><!-- value只要两个数据源不同就行，随便取名 --></span>
        <span class="hljs-tag" style="color: rgb(0, 102, 102); box-sizing: border-box;"><<span class="hljs-title" style="box-sizing: border-box; color: rgb(0, 0, 136);">property</span> <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">name</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"uniqueResourceName"</span> <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">value</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"mysql/sitestone"</span> /></span>
        <span class="hljs-tag" style="color: rgb(0, 102, 102); box-sizing: border-box;"><<span class="hljs-title" style="box-sizing: border-box; color: rgb(0, 0, 136);">property</span> <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">name</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"xaDataSourceClassName"</span>
            <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">value</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"com.mysql.jdbc.jdbc2.optional.MysqlXADataSource"</span> /></span>
        <span class="hljs-tag" style="color: rgb(0, 102, 102); box-sizing: border-box;"><<span class="hljs-title" style="box-sizing: border-box; color: rgb(0, 0, 136);">property</span> <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">name</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"xaProperties"</span>></span>
            <span class="hljs-tag" style="color: rgb(0, 102, 102); box-sizing: border-box;"><<span class="hljs-title" style="box-sizing: border-box; color: rgb(0, 0, 136);">props</span>></span>
                <span class="hljs-tag" style="color: rgb(0, 102, 102); box-sizing: border-box;"><<span class="hljs-title" style="box-sizing: border-box; color: rgb(0, 0, 136);">prop</span> <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">key</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"URL"</span>></span>${jdbc.url}<span class="hljs-tag" style="color: rgb(0, 102, 102); box-sizing: border-box;"></<span class="hljs-title" style="box-sizing: border-box; color: rgb(0, 0, 136);">prop</span>></span>
                <span class="hljs-tag" style="color: rgb(0, 102, 102); box-sizing: border-box;"><<span class="hljs-title" style="box-sizing: border-box; color: rgb(0, 0, 136);">prop</span> <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">key</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"user"</span>></span>${jdbc.username}<span class="hljs-tag" style="color: rgb(0, 102, 102); box-sizing: border-box;"></<span class="hljs-title" style="box-sizing: border-box; color: rgb(0, 0, 136);">prop</span>></span>
                <span class="hljs-tag" style="color: rgb(0, 102, 102); box-sizing: border-box;"><<span class="hljs-title" style="box-sizing: border-box; color: rgb(0, 0, 136);">prop</span> <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">key</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"password"</span>></span>${jdbc.password}<span class="hljs-tag" style="color: rgb(0, 102, 102); box-sizing: border-box;"></<span class="hljs-title" style="box-sizing: border-box; color: rgb(0, 0, 136);">prop</span>></span>
            <span class="hljs-tag" style="color: rgb(0, 102, 102); box-sizing: border-box;"></<span class="hljs-title" style="box-sizing: border-box; color: rgb(0, 0, 136);">props</span>></span>
        <span class="hljs-tag" style="color: rgb(0, 102, 102); box-sizing: border-box;"></<span class="hljs-title" style="box-sizing: border-box; color: rgb(0, 0, 136);">property</span>></span>
    <span class="hljs-tag" style="color: rgb(0, 102, 102); box-sizing: border-box;"></<span class="hljs-title" style="box-sizing: border-box; color: rgb(0, 0, 136);">bean</span>></span>
    <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"><!-- 配置第二个数据源--></span>
    <span class="hljs-tag" style="color: rgb(0, 102, 102); box-sizing: border-box;"><<span class="hljs-title" style="box-sizing: border-box; color: rgb(0, 0, 136);">bean</span> <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">id</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"dataSourceB"</span> <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">parent</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"abstractXADataSource"</span>></span>
        <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"><!-- value只要两个数据源不同就行，随便取名 --></span>
        <span class="hljs-tag" style="color: rgb(0, 102, 102); box-sizing: border-box;"><<span class="hljs-title" style="box-sizing: border-box; color: rgb(0, 0, 136);">property</span> <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">name</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"uniqueResourceName"</span> <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">value</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"mysql/sitesttwo"</span> /></span>
        <span class="hljs-tag" style="color: rgb(0, 102, 102); box-sizing: border-box;"><<span class="hljs-title" style="box-sizing: border-box; color: rgb(0, 0, 136);">property</span> <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">name</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"xaDataSourceClassName"</span>
            <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">value</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"com.mysql.jdbc.jdbc2.optional.MysqlXADataSource"</span> /></span>
        <span class="hljs-tag" style="color: rgb(0, 102, 102); box-sizing: border-box;"><<span class="hljs-title" style="box-sizing: border-box; color: rgb(0, 0, 136);">property</span> <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">name</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"xaProperties"</span>></span>
            <span class="hljs-tag" style="color: rgb(0, 102, 102); box-sizing: border-box;"><<span class="hljs-title" style="box-sizing: border-box; color: rgb(0, 0, 136);">props</span>></span>
                <span class="hljs-tag" style="color: rgb(0, 102, 102); box-sizing: border-box;"><<span class="hljs-title" style="box-sizing: border-box; color: rgb(0, 0, 136);">prop</span> <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">key</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"URL"</span>></span>${jdbca.url}<span class="hljs-tag" style="color: rgb(0, 102, 102); box-sizing: border-box;"></<span class="hljs-title" style="box-sizing: border-box; color: rgb(0, 0, 136);">prop</span>></span>
                <span class="hljs-tag" style="color: rgb(0, 102, 102); box-sizing: border-box;"><<span class="hljs-title" style="box-sizing: border-box; color: rgb(0, 0, 136);">prop</span> <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">key</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"user"</span>></span>${jdbca.username}<span class="hljs-tag" style="color: rgb(0, 102, 102); box-sizing: border-box;"></<span class="hljs-title" style="box-sizing: border-box; color: rgb(0, 0, 136);">prop</span>></span>
                <span class="hljs-tag" style="color: rgb(0, 102, 102); box-sizing: border-box;"><<span class="hljs-title" style="box-sizing: border-box; color: rgb(0, 0, 136);">prop</span> <span class="hljs-attribute" style="box-sizing: border-box; color: rgb(102, 0, 102);">key</span>=<span class="hljs-value" style="box-sizing: border-box; color: rgb(0, 136, 0);">"password"</span>></span>${jdbca.password}<span class="hljs-tag" style="color: rgb(0, 102, 102); box-sizing: border-box;"></<span class="hljs-title" style="box-sizing: border-box; color: rgb(0, 0, 136);">prop</span>></span>
            <span class="hljs-tag" style="color: rgb(0, 102, 102); box-sizing: border-box;"></<span class="hljs-title" style="box-sizing: border-box; color: rgb(0, 0, 136);">props</span>></span>
        <span class="hljs-tag" style="color: rgb(0, 102, 102); box-sizing: border-box;"></<span class="hljs-title" style="box-sizing: border-box; color: rgb(0, 0, 136);">property</span>></span>
    <span class="hljs-tag" style="color: rgb(0, 102, 102); box-sizing: border-box;"></<span class="hljs-title" style="box-sizing: border-box; color: rgb(0, 0, 136);">bean</span>></span></code><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li><li style="box-sizing: border-box; padding: 0px 5px;">4</li><li style="box-sizing: border-box; padding: 0px 5px;">5</li><li style="box-sizing: border-box; padding: 0px 5px;">6</li><li style="box-sizing: border-box; padding: 0px 5px;">7</li><li style="box-sizing: border-box; padding: 0px 5px;">8</li><li style="box-sizing: border-box; padding: 0px 5px;">9</li><li style="box-sizing: border-box; padding: 0px 5px;">10</li><li style="box-sizing: border-box; padding: 0px 5px;">11</li><li style="box-sizing: border-box; padding: 0px 5px;">12</li><li style="box-sizing: border-box; padding: 0px 5px;">13</li><li style="box-sizing: border-box; padding: 0px 5px;">14</li><li style="box-sizing: border-box; padding: 0px 5px;">15</li><li style="box-sizing: border-box; padding: 0px 5px;">16</li><li style="box-sizing: border-box; padding: 0px 5px;">17</li><li style="box-sizing: border-box; padding: 0px 5px;">18</li><li style="box-sizing: border-box; padding: 0px 5px;">19</li><li style="box-sizing: border-box; padding: 0px 5px;">20</li><li style="box-sizing: border-box; padding: 0px 5px;">21</li><li style="box-sizing: border-box; padding: 0px 5px;">22</li><li style="box-sizing: border-box; padding: 0px 5px;">23</li><li style="box-sizing: border-box; padding: 0px 5px;">24</li><li style="box-sizing: border-box; padding: 0px 5px;">25</li><li style="box-sizing: border-box; padding: 0px 5px;">26</li><li style="box-sizing: border-box; padding: 0px 5px;">27</li><li style="box-sizing: border-box; padding: 0px 5px;">28</li><li style="box-sizing: border-box; padding: 0px 5px;">29</li><li style="box-sizing: border-box; padding: 0px 5px;">30</li><li style="box-sizing: border-box; padding: 0px 5px;">31</li><li style="box-sizing: border-box; padding: 0px 5px;">32</li><li style="box-sizing: border-box; padding: 0px 5px;">33</li><li style="box-sizing: border-box; padding: 0px 5px;">34</li><li style="box-sizing: border-box; padding: 0px 5px;">35</li><li style="box-sizing: border-box; padding: 0px 5px;">36</li><li style="box-sizing: border-box; padding: 0px 5px;">37</li><li style="box-sizing: border-box; padding: 0px 5px;">38</li><li style="box-sizing: border-box; padding: 0px 5px;">39</li><li style="box-sizing: border-box; padding: 0px 5px;">40</li><li style="box-sizing: border-box; padding: 0px 5px;">41</li><li style="box-sizing: border-box; padding: 0px 5px;">42</li><li style="box-sizing: border-box; padding: 0px 5px;">43</li><li style="box-sizing: border-box; padding: 0px 5px;">44</li><li style="box-sizing: border-box; padding: 0px 5px;">45</li><li style="box-sizing: border-box; padding: 0px 5px;">46</li><li style="box-sizing: border-box; padding: 0px 5px;">47</li></ul><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li><li style="box-sizing: border-box; padding: 0px 5px;">4</li><li style="box-sizing: border-box; padding: 0px 5px;">5</li><li style="box-sizing: border-box; padding: 0px 5px;">6</li><li style="box-sizing: border-box; padding: 0px 5px;">7</li><li style="box-sizing: border-box; padding: 0px 5px;">8</li><li style="box-sizing: border-box; padding: 0px 5px;">9</li><li style="box-sizing: border-box; padding: 0px 5px;">10</li><li style="box-sizing: border-box; padding: 0px 5px;">11</li><li style="box-sizing: border-box; padding: 0px 5px;">12</li><li style="box-sizing: border-box; padding: 0px 5px;">13</li><li style="box-sizing: border-box; padding: 0px 5px;">14</li><li style="box-sizing: border-box; padding: 0px 5px;">15</li><li style="box-sizing: border-box; padding: 0px 5px;">16</li><li style="box-sizing: border-box; padding: 0px 5px;">17</li><li style="box-sizing: border-box; padding: 0px 5px;">18</li><li style="box-sizing: border-box; padding: 0px 5px;">19</li><li style="box-sizing: border-box; padding: 0px 5px;">20</li><li style="box-sizing: border-box; padding: 0px 5px;">21</li><li style="box-sizing: border-box; padding: 0px 5px;">22</li><li style="box-sizing: border-box; padding: 0px 5px;">23</li><li style="box-sizing: border-box; padding: 0px 5px;">24</li><li style="box-sizing: border-box; padding: 0px 5px;">25</li><li style="box-sizing: border-box; padding: 0px 5px;">26</li><li style="box-sizing: border-box; padding: 0px 5px;">27</li><li style="box-sizing: border-box; padding: 0px 5px;">28</li><li style="box-sizing: border-box; padding: 0px 5px;">29</li><li style="box-sizing: border-box; padding: 0px 5px;">30</li><li style="box-sizing: border-box; padding: 0px 5px;">31</li><li style="box-sizing: border-box; padding: 0px 5px;">32</li><li style="box-sizing: border-box; padding: 0px 5px;">33</li><li style="box-sizing: border-box; padding: 0px 5px;">34</li><li style="box-sizing: border-box; padding: 0px 5px;">35</li><li style="box-sizing: border-box; padding: 0px 5px;">36</li><li style="box-sizing: border-box; padding: 0px 5px;">37</li><li style="box-sizing: border-box; padding: 0px 5px;">38</li><li style="box-sizing: border-box; padding: 0px 5px;">39</li><li style="box-sizing: border-box; padding: 0px 5px;">40</li><li style="box-sizing: border-box; padding: 0px 5px;">41</li><li style="box-sizing: border-box; padding: 0px 5px;">42</li><li style="box-sizing: border-box; padding: 0px 5px;">43</li><li style="box-sizing: border-box; padding: 0px 5px;">44</li><li style="box-sizing: border-box; padding: 0px 5px;">45</li><li style="box-sizing: border-box; padding: 0px 5px;">46</li><li style="box-sizing: border-box; padding: 0px 5px;">47</li></ul>
[](http://blog.csdn.net/luo_deng/article/details/50525073#)[](http://blog.csdn.net/luo_deng/article/details/50525073#)[](http://blog.csdn.net/luo_deng/article/details/50525073#)[](http://blog.csdn.net/luo_deng/article/details/50525073#)[](http://blog.csdn.net/luo_deng/article/details/50525073#)[](http://blog.csdn.net/luo_deng/article/details/50525073#)
