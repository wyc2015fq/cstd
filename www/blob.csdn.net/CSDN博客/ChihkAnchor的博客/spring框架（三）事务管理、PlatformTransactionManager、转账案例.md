# spring框架（三）事务管理、PlatformTransactionManager、转账案例 - ChihkAnchor的博客 - CSDN博客





2019年03月21日 09:37:47[Chihk-Anchor](https://me.csdn.net/weixin_40871455)阅读数：15








### **AOP ：切面编程**

切面：切入点 和 通知 结合

spring aop 编程
|<aop:config>方法1：  <aop:pointcut expression="切入点表达式" id="">  <aop:advisor  advice-ref="通知引用" pointcut-ref="切入点的引用">方法2：   <aop:advisor  advice-ref="通知引用" pointcut="切入点表达式">|
|----|

AspectJ xml
|<aop:config>  <aop:aspect ref="切面类">     <aop:pointcut>     <aop:before>  前置     <aop:afterReturning  returning="第二个参数名称"> 后置     <aop:around> 环绕     <aop:afterThrowing throwing="第二。。。"> 抛出异常     <aop:after> 最终|
|----|

AspectJ annotation

@Aspect

@Pointcut("表达式")  private void xxx(){}

@Before @...

切入点表达式

<aop:pointcut expression="execution(* com.itheima.crm.*.service..*.*(..))" id="">

## ******事务管理******
- 事务：一组业务操作ABCD，要么全部成功，要么全部不成功。
- 特性：ACID

原子性：整体

一致性：完成

隔离性：并发

持久性：结果

隔离问题：

脏读：一个事务读到另一个事务没有提交的数据

不可重复读：一个事务读到另一个事务已提交的数据（update）

虚读(幻读)：一个事务读到另一个事务已提交的数据（insert）

隔离级别：

read uncommitted：读未提交。存在3个问题

read committed：读已提交。解决脏读，存在2个问题

repeatable read：可重复读。解决：脏读、不可重复读，存在1个问题。

serializable ：串行化。都解决，单事务。

mysql 事务操作--简单

```java
ABCD 一个事务
Connection conn = null;
try{
  //1 获得连接
  conn = ...;
  //2 开启事务
  conn.setAutoCommit(false);
  A
  B
  C
  D
  //3 提交事务
  conn.commit();
} catche(){
  //4 回滚事务
  conn.rollback();
}
```

mysql 事务操作--Savepoint

```java
需求：AB（必须），CD（可选） 
Connection conn = null;
Savepoint savepoint = null;  //保存点，记录操作的当前位置，之后可以回滚到指定的位置。（可以回滚一部分）
try{
  //1 获得连接
  conn = ...;
  //2 开启事务
  conn.setAutoCommit(false);
  A
  B
  savepoint = conn.setSavepoint();
  C
  D
  //3 提交事务
  conn.commit();
} catche(){
  if(savepoint != null){   //CD异常
     // 回滚到CD之前
     conn.rollback(savepoint);
     // 提交AB
     conn.commit();
  } else{   //AB异常
     // 回滚AB
     conn.rollback();
  }
}
```

## ******Spring************事务管理介绍******
- ******导入jar包******

transaction  -->  tx    ![](https://img-blog.csdnimg.cn/20190321084805736.png)


- ******三个顶级接口******

![](https://img-blog.csdnimg.cn/20190321084936181.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

PlatformTransactionManager  平台事务管理器，spring要管理事务，必须使用事务管理器进行事务配置时，必须****配置事务管理器****。

TransactionDefinition：事务详情（事务定义、事务属性），spring用于确定事务具体详情，例如：隔离级别、是否只读、超时时间 等进行事务配置时，****必须配置详情****。spring将配置项封装到该对象实例。

TransactionStatus：事务状态，spring用于记录当前事务运行状态。例如：是否有保存点，事务是否完成。spring底层根据状态进行相应操作。

### ******PlatformTransactionManager  事务管理器******

导入jar包：需要时平台事务管理器的实现类

![](https://img-blog.csdnimg.cn/20190321085346438.png)

**常见的事务管理器**

****DataSourceTransactionManager  ，jdbc开发时事务管理器，采用JdbcTemplate****

****HibernateTransactionManager，hibernate开发时事务管理器，整合hibernate****

![](https://img-blog.csdnimg.cn/20190321085431135.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

**api详解**

TransactionStatus  getTransaction(TransactionDefinition definition) ，事务管理器 通过“事务详情”，获得“事务状态”，从而管理事务。

void commit(TransactionStatus status)  根据状态提交

void rollback(TransactionStatus status) 根据状态回滚

### ******TransactionStatus******

![](https://img-blog.csdnimg.cn/20190321085711414.png)

### ******TransactionDefinition******

![](https://img-blog.csdnimg.cn/20190321090008304.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

传播行为：在两个业务之间如何共享事务。

```bash
PROPAGATION_REQUIRED , required , 必须  【默认值】

    支持当前事务，A如果有事务，B将使用该事务。

    如果A没有事务，B将创建一个新的事务。

PROPAGATION_SUPPORTS ，supports ，支持

    支持当前事务，A如果有事务，B将使用该事务。

    如果A没有事务，B将以非事务执行。

PROPAGATION_MANDATORY，mandatory ，强制

    支持当前事务，A如果有事务，B将使用该事务。

    如果A没有事务，B将抛异常。

PROPAGATION_REQUIRES_NEW ， requires_new ，必须新的

    如果A有事务，将A的事务挂起，B创建一个新的事务

    如果A没有事务，B创建一个新的事务

PROPAGATION_NOT_SUPPORTED ，not_supported ,不支持

    如果A有事务，将A的事务挂起，B将以非事务执行

    如果A没有事务，B将以非事务执行

PROPAGATION_NEVER ，never，从不

    如果A有事务，B将抛异常

    如果A没有事务，B将以非事务执行

PROPAGATION_NESTED ，nested ，嵌套

    A和B底层采用保存点机制，形成嵌套事务。
```

掌握：PROPAGATION_REQUIRED、PROPAGATION_REQUIRES_NEW、PROPAGATION_NESTED 

## ******案例：转账******

******搭建环境******

******创建表******
|create database spring;use spring;create table account(  id int primary key auto_increment,  username varchar(50),  money int);insert into account(username,money) values('jack','10000');insert into account(username,money) values('rose','10000');|
|----|

******导入jar包******
- 核心：4+1
- aop ： 4 (aop联盟、spring aop、aspectj规范、spring aspect)
- 数据库：2  （jdbc/tx）
- 驱动：mysql
- 连接池：c3p0

![](https://img-blog.csdnimg.cn/20190321090349684.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

******dao层******​​​​​​​

```java
public class AccountDaoImpl extends JdbcDaoSupport implements AccountDao {

	@Override
	public void out(String outer, Integer money) {
		this.getJdbcTemplate().update("update account set money = money - ? where username = ?", money,outer);
	}

	@Override
	public void in(String inner, Integer money) {
		this.getJdbcTemplate().update("update account set money = money + ? where username = ?", money,inner);
	}
}
```

******service层******

```java
public class AccountServiceImpl implements AccountService {

	private AccountDao accountDao;
	public void setAccountDao(AccountDao accountDao) {
		this.accountDao = accountDao;
	}
	@Override
	public void transfer(String outer, String inner, Integer money) {
		accountDao.out(outer, money);
		//断电
//		int i = 1/0;
		accountDao.in(inner, money);
	}
}
```

​​​​​​​******spring配置******

```
<!-- 1 datasource -->
	<bean id="dataSource" class="com.mchange.v2.c3p0.ComboPooledDataSource">
		<property name="driverClass" value="com.mysql.jdbc.Driver"></property>
		<property name="jdbcUrl" value="jdbc:mysql://localhost:3306/spring"></property>
		<property name="user" value="root"></property>
		<property name="password" value="1234"></property>
	</bean>
	
	<!-- 2 dao  -->
	<bean id="accountDao" class="com.itheima.dao.impl.AccountDaoImpl">
		<property name="dataSource" ref="dataSource"></property>
	</bean>
	<!-- 3 service -->
	<bean id="accountService" class="com.itheima.service.impl.AccountServiceImpl">
		<property name="accountDao" ref="accountDao"></property>
	</bean>
```

​​​​​​​******测试******

```java
@Test
	public void demo01(){
		String xmlPath = "applicationContext.xml";
		ApplicationContext applicationContext = new ClassPathXmlApplicationContext(xmlPath);
		AccountService accountService =  (AccountService) applicationContext.getBean("accountService");
		accountService.transfer("jack", "rose", 1000);
	}
```

​​​​​​​******手动管理事务（了解）******
- spring底层使用 TransactionTemplate 事务模板进行操作。

1.service 需要获得 TransactionTemplate

2.spring 配置模板，并注入给service

3.模板需要注入事务管理器

4.配置事务管理器：DataSourceTransactionManager ，需要注入DataSource

​​​​​​​******修改service******

```java
//需要spring注入模板
	private TransactionTemplate transactionTemplate;
	public void setTransactionTemplate(TransactionTemplate transactionTemplate) {
		this.transactionTemplate = transactionTemplate;
	}
	@Override
	public void transfer(final String outer,final String inner,final Integer money) {
		transactionTemplate.execute(new TransactionCallbackWithoutResult() {
			
			@Override
			protected void doInTransactionWithoutResult(TransactionStatus arg0) {
				accountDao.out(outer, money);
				//断电
//				int i = 1/0;
				accountDao.in(inner, money);
			}
		});
	}
```

​​​​​​​******修改spring配置******

```
<!-- 3 service -->
	<bean id="accountService" class="com.itheima.service.impl.AccountServiceImpl">
		<property name="accountDao" ref="accountDao"></property>
		<property name="transactionTemplate" ref="transactionTemplate"></property>
	</bean>
	
	<!-- 4 创建模板 -->
	<bean id="transactionTemplate" class="org.springframework.transaction.support.TransactionTemplate">
		<property name="transactionManager" ref="txManager"></property>
	</bean>
	
	<!-- 5 配置事务管理器 ,管理器需要事务，事务从Connection获得，连接从连接池DataSource获得 -->
	<bean id="txManager" class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
		<property name="dataSource" ref="dataSource"></property>
	</bean>
```

​​​​​​​******工厂bean 生成代理：半自动******
- spring提供 管理事务的代理工厂bean TransactionProxyFactoryBean

1.getBean() 获得代理对象

2.spring 配置一个代理

​​​​​​​******spring配置******​​​​​​​

```
<!-- 4 service 代理对象 
		4.1 proxyInterfaces 接口 
		4.2 target 目标类
		4.3 transactionManager 事务管理器
		4.4 transactionAttributes 事务属性（事务详情）
			prop.key ：确定哪些方法使用当前事务配置
			prop.text:用于配置事务详情
				格式：PROPAGATION,ISOLATION,readOnly,-Exception,+Exception
					传播行为		隔离级别	是否只读		异常回滚		异常提交
				例如：
					<prop key="transfer">PROPAGATION_REQUIRED,ISOLATION_DEFAULT</prop> 默认传播行为，和隔离级别
					<prop key="transfer">PROPAGATION_REQUIRED,ISOLATION_DEFAULT,readOnly</prop> 只读
					<prop key="transfer">PROPAGATION_REQUIRED,ISOLATION_DEFAULT,+java.lang.ArithmeticException</prop>  有异常扔提交
	-->
	<bean id="proxyAccountService" class="org.springframework.transaction.interceptor.TransactionProxyFactoryBean">
		<property name="proxyInterfaces" value="com.itheima.service.AccountService"></property>
		<property name="target" ref="accountService"></property>
		<property name="transactionManager" ref="txManager"></property>
		<property name="transactionAttributes">
			<props>
				<prop key="transfer">PROPAGATION_REQUIRED,ISOLATION_DEFAULT</prop>
			</props>
		</property>
	</bean>


	<!-- 5 事务管理器 -->
	<bean id="txManager" class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
		<property name="dataSource" ref="dataSource"></property>
	</bean>
```

******测试******



![](https://img-blog.csdnimg.cn/20190321091850133.png)

​​​​​​​******AOP 配置基于xml【掌握】******
- 在spring xml 配置aop 自动生成代理，进行事务的管理

1.配置管理器

2.配置事务详情

3.配置aop

```
<!-- 4 事务管理 -->
	<!-- 4.1 事务管理器 -->
	<bean id="txManager" class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
		<property name="dataSource" ref="dataSource"></property>
	</bean>
	<!-- 4.2 事务详情（事务通知）  ， 在aop筛选基础上，对ABC三个确定使用什么样的事务。例如：AC读写、B只读 等
		<tx:attributes> 用于配置事务详情（属性属性）
			<tx:method name=""/> 详情具体配置
				propagation 传播行为 ， REQUIRED：必须；REQUIRES_NEW:必须是新的
				isolation 隔离级别
	-->
	<tx:advice id="txAdvice" transaction-manager="txManager">
		<tx:attributes>
			<tx:method name="transfer" propagation="REQUIRED" isolation="DEFAULT"/>
		</tx:attributes>
	</tx:advice>
	<!-- 4.3 AOP编程，目标类有ABCD（4个连接点），切入点表达式 确定增强的连接器，从而获得切入点：ABC -->
	<aop:config>
		<aop:advisor advice-ref="txAdvice" pointcut="execution(* com.itheima.service..*.*(..))"/>
	</aop:config>
```

******AOP配置基于注解【掌握】******



1.配置事务管理器，将并事务管理器交予spring

2.在目标类或目标方法添加注解即可 @Transactional

​​​​​​​******spring配置******

```
<!-- 4 事务管理 -->
	<!-- 4.1 事务管理器 -->
	<bean id="txManager" class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
		<property name="dataSource" ref="dataSource"></property>
	</bean>
	<!-- 4.2 将管理器交予spring 
		* transaction-manager 配置事务管理器
		* proxy-target-class
			true ： 底层强制使用cglib 代理
	-->
	<tx:annotation-driven transaction-manager="txManager"/>
```

​​​​​​​******service 层******

```java
@Transactional
public class AccountServiceImpl implements AccountService {
```

​​​​​​​******事务详情配置******

![](https://img-blog.csdnimg.cn/20190321092606319.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

```java
@Transactional(propagation=Propagation.REQUIRED , isolation = Isolation.DEFAULT)
public class AccountServiceImpl implements AccountService {
```

******整合Junit******
- 导入jar包

基本 ：4+1

测试：spring-test...jar
- 1.让Junit通知spring加载配置文件
- 2.让spring容器自动进行注入

修改测试类

```java
@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations="classpath:applicationContext.xml")
public class TestApp {
	
	@Autowired  //与junit整合，不需要在spring xml配置扫描
	private AccountService accountService;
	
	@Test
	public void demo01(){
//		String xmlPath = "applicationContext.xml";
//		ApplicationContext applicationContext = new ClassPathXmlApplicationContext(xmlPath);
//		AccountService accountService =  (AccountService) applicationContext.getBean("accountService");
		accountService.transfer("jack", "rose", 1000);
	}

}
```

******整合web******
- 导入jar包：spring-web.xml   ![](https://img-blog.csdnimg.cn/20190321093023885.png)
- tomcat启动加载配置文件

servlet --> init(ServletConfig) --> <load-on-startup>2

filter --> init(FilterConfig)  --> web.xml注册过滤器自动调用初始化

listener --> ServletContextListener --> servletContext对象监听

spring提供监听器 ****ContextLoaderListener****  --> web.xml  <listener><listener-class>....

如果只配置监听器，默认加载xml位置：/WEB-INF/applicationContext.xml

![](https://img-blog.csdnimg.cn/20190321093231909.png)
- 确定配置文件位置，通过系统初始化参数

ServletContext 初始化参数 web.xml  

<context-param>

****<param-name>contextConfigLocation****

****<param-value>classpath:********applicationContext.xml****

```
<!-- 确定配置文件位置 -->
  <context-param>
  	<param-name>contextConfigLocation</param-name>
  	<param-value>classpath:applicationContext.xml</param-value>
  </context-param>
  
  <!-- 配置spring 监听器，加载xml配置文件 -->
  <listener>
  	<listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
  </listener>
```
- 从servletContext作用域 获得spring容器 （了解）

```java
// 从application作用域（ServletContext）获得spring容器
//方式1： 手动从作用域获取
ApplicationContext applicationContext = (ApplicationContext) this.getServletContext().getAttribute(WebApplicationContext.ROOT_WEB_APPLICATION_CONTEXT_ATTRIBUTE);
//方式2：通过工具获取
ApplicationContext apppApplicationContext2 = WebApplicationContextUtils.getWebApplicationContext(this.getServletContext());
```

整理自黑马程序员课堂笔记



