# Spring实现分布式事务JTA(使用atomiko实现) - 零度的博客专栏 - CSDN博客
2017年01月11日 14:15:41[零度anngle](https://me.csdn.net/zmx729618)阅读数：13363标签：[spring																[JTA](https://so.csdn.net/so/search/s.do?q=JTA&t=blog)](https://so.csdn.net/so/search/s.do?q=spring&t=blog)
个人分类：[JTA																[Spring](https://blog.csdn.net/zmx729618/article/category/6168885)](https://blog.csdn.net/zmx729618/article/category/6169359)
       分布式事务是指事务的参与者、支持事务的服务器、资源管理器以及事务管理器分别位于分布系统的不同节点之上，在两个或多个网络计算机资源上访问并且更新数据，将两个或多个网络计算机的数据进行的多次操作作为一个整体进行处理。如不同银行账户之间的转账。
       对于在项目中接触到JTA，大部分的原因是因为在项目中需要操作多个数据库，同时，可以保证操作的原子性，保证对多个数据库的操作一致性。 
      1、项目结构图：
![](https://img-blog.csdn.net/20170111154055699?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem14NzI5NjE4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
2、pom.xml
```
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>
  <groupId>org.nercita.ltxx</groupId>
  <artifactId>jtaTest</artifactId>
  <version>0.0.1-SNAPSHOT</version>
  <packaging>jar</packaging>
  <name>jtaTest</name>
  <url>http://maven.apache.org</url>
  <properties>
    <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
    <spring.version>4.1.6.RELEASE</spring.version>
  </properties>
    <dependencies>  
        <dependency>
            <groupId>junit</groupId>
            <artifactId>junit</artifactId>
            <version>4.12</version>
            <scope>test</scope>
        </dependency>
                       
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-test</artifactId>
            <version>${spring.version}</version>
            <scope>provided</scope>
        </dependency>
	    <!-- 数据库驱动-mysql -->
	    <dependency>
		      <groupId>mysql</groupId>
		      <artifactId>mysql-connector-java</artifactId>
		      <version>5.1.26</version>
	    </dependency>
	    
	    <!-- 数据库驱动-oracle -->
		<dependency>         
			  <groupId>com.oracle</groupId>
			  <artifactId>ojdbc6</artifactId>
			  <version>12.1.0.1</version>
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
            <version>${spring.version}</version>
        </dependency>       
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-jdbc</artifactId>
            <version>${spring.version}</version>
        </dependency>
        
        <dependency>
			<groupId>org.springframework</groupId>
			<artifactId>spring-aspects</artifactId>
			<version>${spring.version}</version>
	    </dependency>
        
        
        <!-- aspectj weaver.jar 这是SpringAOP所要用到的依赖包 -->
	    <dependency>
	        <groupId>org.aspectj</groupId>
		    <artifactId>aspectjweaver</artifactId>
		    <version>1.8.2</version>
	    </dependency>
	    <dependency>
	        <groupId>org.aspectj</groupId>
	        <artifactId>aspectjrt</artifactId>
	        <version>1.8.2</version>
	    </dependency> 
    </dependencies>
</project>
```
2、applicationContext.xml
``
```
<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<beans xmlns="http://www.springframework.org/schema/beans"
	xmlns:aop="http://www.springframework.org/schema/aop" 
	xmlns:context="http://www.springframework.org/schema/context"
	xmlns:jee="http://www.springframework.org/schema/jee" 
	xmlns:tx="http://www.springframework.org/schema/tx"
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xsi:schemaLocation="http://www.springframework.org/schema/aop http://www.springframework.org/schema/aop/spring-aop-4.0.xsd   
	                    http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-4.0.xsd   
	                    http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context-4.0.xsd   
	                    http://www.springframework.org/schema/jee http://www.springframework.org/schema/jee/spring-jee-4.0.xsd   
	                    http://www.springframework.org/schema/tx http://www.springframework.org/schema/tx/spring-tx-4.0.xsd">
    <!-- 引入属性文件 -->
    <context:property-placeholder location="classpath*:/application.properties" />
    
    <!-- 对于没有被IOC容器管理的对象也可以使用依赖注入 -->
    <context:spring-configured /> 
    <!-- 注解bean及依赖注入 -->
    <context:component-scan base-package="org.nercita.ltxx.jtatest" />
	
	
    <!-- mysql数据源1 -->
    <bean id="masterDataSource" class="com.atomikos.jdbc.AtomikosDataSourceBean"
        init-method="init" destroy-method="close">
        <description>master xa datasource</description>
        <property name="uniqueResourceName">
            <value>masterDataSource</value>
        </property>
        <property name="xaDataSourceClassName" value="com.mysql.jdbc.jdbc2.optional.MysqlXADataSource" />
        <property name="xaProperties">
            <props>
                <prop key="user">root</prop>
                <prop key="password">0729</prop>
                <prop key="URL">jdbc:mysql://127.0.0.1:3306/master</prop>
            </props>
        </property>     
        <property name="poolSize" value="10"/> 
    </bean>
    <!-- mysql数据源1 -->
    <bean id="slaveDataSource" class="com.atomikos.jdbc.AtomikosDataSourceBean"
        init-method="init" destroy-method="close">
        <description>slave xa datasource</description>
        <property name="uniqueResourceName">
            <value>slaveDataSource</value>
        </property>
        <property name="xaDataSourceClassName" value="com.mysql.jdbc.jdbc2.optional.MysqlXADataSource" />
        <property name="xaProperties">
            <props>
                <prop key="user">root</prop>
                <prop key="password">0729</prop>
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
    <!-- atomikos用户事务实现 -->
    <bean id="atomikosUserTransaction" class="com.atomikos.icatch.jta.UserTransactionImp">
        <property name="transactionTimeout" value="300" />
    </bean>
    <!-- spring 事务管理器 -->
    <bean id="springTransactionManager" class="org.springframework.transaction.jta.JtaTransactionManager">
        <!--注入 atomikos事务管理器 -->
        <property name="transactionManager">
            <ref bean="atomikosTransactionManager" />
        </property>
        <!--注入 atomikos用户事务实现 -->
        <property name="userTransaction">
            <ref bean="atomikosUserTransaction" />
        </property>
    </bean>
    <!-- spring事务模板 -->
    <bean id="transactionTemplate"  class="org.springframework.transaction.support.TransactionTemplate">
        <property name="transactionManager">
            <ref bean="springTransactionManager" />
        </property>
    </bean>  
    <!--jdbc模板  -->
    <bean id="masterJdbcTemplate" class="org.springframework.jdbc.core.JdbcTemplate">
        <constructor-arg>
            <ref bean="masterDataSource" />
        </constructor-arg>
    </bean>
    <!--jdbc模板  -->
    <bean id="slaveJdbcTemplate" class="org.springframework.jdbc.core.JdbcTemplate">
        <constructor-arg>
            <ref bean="slaveDataSource" />
        </constructor-arg>
    </bean>
 
     <!-- 开启Spring注解式事务 --> 
    <tx:annotation-driven transaction-manager="springTransactionManager"/>  
</beans>
```
ps：数据库和表需自行创建
3、java类 
dao：
```
package org.nercita.ltxx.jtatest.dao;
import javax.annotation.Resource;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Repository;
@Repository
public class JtaTestMasterDao{
    @Resource(name="masterJdbcTemplate")
    JdbcTemplate masterJdbcTemplate;
    public String master() {
        masterJdbcTemplate.execute("update teacher set name='master' where id=1");
        return "success";
    }
    public void update() {
        masterJdbcTemplate.execute("update teacher set name='8' where id=1");
        System.out.println("update");
        masterJdbcTemplate.execute("fff teacher set name=''6' where id=1");
    }
}
```
```
package org.nercita.ltxx.jtatest.dao;
import javax.annotation.Resource;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Repository;
@Repository
public class JtaTestSlaveDao{
    @Resource(name="slaveJdbcTemplate")
    JdbcTemplate slaveJdbcTemplate;
   
    
    public String slave() {
        slaveJdbcTemplate.execute("update student set name='slave' where id=1");            
        return "success";
    }   
}
```
service：
```
package org.nercita.ltxx.jtatest.service;
import javax.annotation.Resource;
import javax.transaction.SystemException;
import javax.transaction.UserTransaction;
import org.nercita.ltxx.jtatest.dao.JtaTestMasterDao;
import org.nercita.ltxx.jtatest.dao.JtaTestSlaveDao;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.TransactionStatus;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.transaction.jta.JtaTransactionManager;
import org.springframework.transaction.support.TransactionCallback;
import org.springframework.transaction.support.TransactionCallbackWithoutResult;
import org.springframework.transaction.support.TransactionTemplate;
@Service
public class JtaTestService{
    @Resource(name = "springTransactionManager")
    private JtaTransactionManager txManager;
    @Autowired
    private JtaTestMasterDao jtaTestMasterDao;
    @Autowired
    private JtaTestSlaveDao jtaTestSlaveDao;  
    @Resource(name = "transactionTemplate")
    private TransactionTemplate transactionTemplate;  
   
    //编程式
    public String test() {
         UserTransaction userTx = txManager.getUserTransaction(); 
         try {               
             userTx.begin();     
             jtaTestMasterDao.master(); 
             jtaTestSlaveDao.slave();    
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
    @Transactional
    public void update(){
    	jtaTestMasterDao.master(); 
        int a=1/0;
        System.out.println(a);
    	jtaTestSlaveDao.slave();   
    }
    
    
    //事务模板方式
    public void test3() {  
            transactionTemplate.execute(new TransactionCallbackWithoutResult(){  
                @Override  
                protected void doInTransactionWithoutResult(TransactionStatus status) {  
                    try {  
                    	jtaTestMasterDao.master();        
                    	jtaTestSlaveDao.slave();   
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
                });  
                */
        }  
}
```
controller(本文直接使用controller进行测试)
```
package org.nercita.ltxx.jtatest.web;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.nercita.ltxx.jtatest.service.JtaTestService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations = "classpath:applicationContext.xml")
@Controller
public class JtaTestController {   
    @Autowired
    private JtaTestService jtaTestService;
    //MySQL的数据库引擎必须是InnoDB，否则无法回滚
    @Test
    public void test(){
    	jtaTestService.test();
    }
    @Test
    public void test2(){
    	jtaTestService.update();
    }
    @Test
    public void test3(){
    	jtaTestService.test3();
    }
}
```
4、数据源详细参数配置
```xml
```
```
<!-- 两个数据源的功用配置，方便下面直接引用 -->
     <bean id="abstractXADataSource" class="com.atomikos.jdbc.AtomikosDataSourceBean" init-method="init" 
             destroy-method="close" abstract="true"> 
        <property name="xaDataSourceClassName" value="com.mysql.jdbc.jdbc2.optional.MysqlXADataSource"/> 
        <property name="poolSize" value="10" /> 
        <property name="minPoolSize" value="10"/> 
        <property name="maxPoolSize" value="30"/> 
        <property name="borrowConnectionTimeout" value="60"/>  <!--获取连接失败重新获等待最大时间，在这个时间内如果有可用连接，将返回-->
        <property name="reapTimeout" value="20"/> <!--最大获取数据时间，如果不设置这个值，Atomikos使用默认的5分钟，那么在处理大批量数据读取的时候，一旦超过5分钟，就会抛出类似 Resultset is close 的错误.-->        
        <property name="maxIdleTime" value="60"/>    <!--最大闲置时间，超过最小连接池连接的连接将将关闭-->
        <property name="maintenanceInterval" value="60" />  <!--连接回收时间-->    
        <property name="loginTimeout" value="60" />     <!--java数据库连接池，最大可等待获取datasouce的时间-->
        <property name="logWriter" value="60"/>
        <property name="testQuery">
            <value>select 1</value>
        </property>
    </bean> 
    <!-- 配置第一个数据源 -->
    <bean id="dataSource" parent="abstractXADataSource">
         <!-- value只要两个数据源不同就行，随便取名 -->
        <property name="uniqueResourceName" value="mysql/sitestone" />
        <property name="xaDataSourceClassName"
            value="com.mysql.jdbc.jdbc2.optional.MysqlXADataSource" />
        <property name="xaProperties">
            <props>
                <prop key="URL">${jdbc.url}</prop>
                <prop key="user">${jdbc.username}</prop>
                <prop key="password">${jdbc.password}</prop>
            </props>
        </property>
    </bean>
    <!-- 配置第二个数据源-->
    <bean id="dataSourceB" parent="abstractXADataSource">
        <!-- value只要两个数据源不同就行，随便取名 -->
        <property name="uniqueResourceName" value="mysql/sitesttwo" />
        <property name="xaDataSourceClassName"
            value="com.mysql.jdbc.jdbc2.optional.MysqlXADataSource" />
        <property name="xaProperties">
            <props>
                <prop key="URL">${jdbca.url}</prop>
                <prop key="user">${jdbca.username}</prop>
                <prop key="password">${jdbca.password}</prop>
            </props>
        </property>
    </bean>
```

