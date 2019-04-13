
# Spring分布式事务实现 - 阳光岛主 - CSDN博客

2013年09月01日 23:20:37[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：17764


分布式事务是指操作多个数据库之间的事务，spring的org.springframework.transaction.jta.JtaTransactionManager，提供了分布式事务支持。如果使用WAS的JTA支持，把它的属性改为WebSphere对应的TransactionManager。
在tomcat下，是没有分布式事务的，不过可以借助于第三方软件jotm（Java Open Transaction Manager ）和AtomikosTransactionsEssentials实现，在spring中分布式事务是通过jta（jotm，atomikos）来进行实现。
1、http://jotm.objectweb.org/
2、http://www.atomikos.com/Main/TransactionsEssentials
**一、使用JOTM例子**
(1) Dao及实现
GenericDao接口：

```python
public interface GenericDao {
	public int save(String ds, String sql, Object[] obj) throws Exception;	
	public int findRowCount(String ds, String sql);	
}
```
GenericDaoImpl 实现：
```python
public class GenericDaoImpl implements GenericDao{
	private  JdbcTemplate jdbcTemplateA;
	private  JdbcTemplate jdbcTemplateB;
	public void setJdbcTemplateA(JdbcTemplate jdbcTemplate) {
		this.jdbcTemplateA = jdbcTemplate;
	}
	public void setJdbcTemplateB(JdbcTemplate jdbcTemplate) {
		this.jdbcTemplateB = jdbcTemplate;
	}
	
	public int save(String ds, String sql, Object[] obj) throws Exception{
		if(null == ds || "".equals(ds)) return -1;
		try{
			if(ds.equals("A")){
				return this.jdbcTemplateA.update(sql, obj);
			}else{
				return this.jdbcTemplateB.update(sql, obj);
			}
		}catch(Exception e){
			e.printStackTrace();
			throw new Exception("执行" + ds + "数据库时失败！");
		}
	}
	public int findRowCount(String ds, String sql) {
		if(null == ds || "".equals(ds)) return -1;
		
		if(ds.equals("A")){
			return this.jdbcTemplateA.queryForInt(sql);
		}else{
			return this.jdbcTemplateB.queryForInt(sql);
		}
	}
}
```
(2) Service及实现
UserService 接口：
```python
public interface UserService {
	public void saveUser() throws Exception;
}
```
UserServiceImpl 实现：
```python
public class UserServiceImpl implements UserService{
	private GenericDao genericDao;
	
	public void setGenericDao(GenericDao genericDao) {
		this.genericDao = genericDao;
	}
	public void saveUser() throws Exception {
		String userName = "user_" + Math.round(Math.random()*10000);
		System.out.println(userName);
		
		StringBuilder sql = new StringBuilder();
		sql.append(" insert into t_user(username, gender) values(?,?); ");
		Object[] objs = new Object[]{userName,"1"};
		
		genericDao.save("A", sql.toString(), objs);
		
		sql.delete(0, sql.length());
		sql.append(" insert into t_user(name, sex) values(?,?); ");
		objs = new Object[]{userName,"男的"};//值超出范围
		genericDao.save("B", sql.toString(), objs);
	}
}
```

(3) applicationContext-jotm.xml
```python
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans" 
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
	xmlns:context="http://www.springframework.org/schema/context" 
	xmlns:aop="http://www.springframework.org/schema/aop" 
	xmlns:tx="http://www.springframework.org/schema/tx" 
	xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-2.5.xsd 
	http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context-2.5.xsd 
	http://www.springframework.org/schema/tx http://www.springframework.org/schema/tx/spring-tx-2.5.xsd 
	http://www.springframework.org/schema/aop http://www.springframework.org/schema/aop/spring-aop-2.5.xsd">
	<description>springJTA</description>
	<!--指定Spring配置中用到的属性文件--> 
	<bean id="propertyConfig" 
			class="org.springframework.beans.factory.config.PropertyPlaceholderConfigurer"> 
		<property name="locations"> 
			<list> 
				<value>classpath:jdbc.properties</value> 
			</list> 
		</property> 
	</bean> 
	
	<!-- JOTM实例 -->
	<bean id="jotm" class="org.springframework.transaction.jta.JotmFactoryBean">
	      <property name="defaultTimeout" value="500000"/>
	</bean>
	<!-- JTA事务管理器 -->
	<bean id="jtaTransactionManager" class="org.springframework.transaction.jta.JtaTransactionManager">   
		<property name="userTransaction" ref="jotm" />   
	</bean>
    <!-- 数据源A --> 
    <bean id="dataSourceA" class="org.enhydra.jdbc.pool.StandardXAPoolDataSource" destroy-method="shutdown"> 
       <property name="dataSource"> 
           <bean class="org.enhydra.jdbc.standard.StandardXADataSource" destroy-method="shutdown"> 
               <property name="transactionManager" ref="jotm"/> 
               <property name="driverName" value="${jdbc.driver}"/> 
               <property name="url" value="${jdbc.url}"/> 
           </bean> 
       </property> 
       <property name="user" value="${jdbc.username}"/> 
       <property name="password" value="${jdbc.password}"/> 
    </bean> 
    <!-- 数据源B --> 
    <bean id="dataSourceB" class="org.enhydra.jdbc.pool.StandardXAPoolDataSource" destroy-method="shutdown"> 
       <property name="dataSource"> 
           <bean class="org.enhydra.jdbc.standard.StandardXADataSource" destroy-method="shutdown"> 
               <property name="transactionManager" ref="jotm"/> 
               <property name="driverName" value="${jdbc2.driver}"/> 
               <property name="url" value="${jdbc2.url}"/> 
           </bean> 
       </property> 
       <property name="user" value="${jdbc2.username}"/> 
       <property name="password" value="${jdbc2.password}"/> 
    </bean> 
    <bean id = "jdbcTemplateA" 
         class = "org.springframework.jdbc.core.JdbcTemplate"> 
         <property name = "dataSource" ref="dataSourceA"/> 
    </bean>
    
    <bean id = "jdbcTemplateB" 
         class = "org.springframework.jdbc.core.JdbcTemplate"> 
         <property name = "dataSource" ref="dataSourceB"/> 
    </bean>    
	<!-- 事务切面配置 --> 
	<aop:config> 
		<aop:pointcut id="pointCut"
				expression="execution(* com.logcd.service..*.*(..))"/><!-- 包及其子包下的所有方法 -->
        <aop:advisor pointcut-ref="pointCut" advice-ref="txAdvice"/> 
        
        <aop:advisor pointcut="execution(* *..common.service..*.*(..))" advice-ref="txAdvice"/>
	</aop:config> 
	<!-- 通知配置 --> 
	<tx:advice id="txAdvice" transaction-manager="jtaTransactionManager"> 
       <tx:attributes> 
          <tx:method name="delete*" rollback-for="Exception"/> 
          <tx:method name="save*" rollback-for="Exception"/> 
          <tx:method name="update*" rollback-for="Exception"/> 
          <tx:method name="find*" read-only="true" rollback-for="Exception"/> 
       </tx:attributes> 
	</tx:advice> 
	<bean id="genericDao"  class="com.logcd.dao.impl.GenericDaoImpl" autowire="byName"> </bean>
	<bean id="userService"  class="com.logcd.service.impl.UserServiceImpl" autowire="byName"> </bean>
</beans>
```

(4) 测试
```python
public class TestUserService{
	private static UserService userService;
	
	@BeforeClass
	public static void init(){
		ApplicationContext app = new ClassPathXmlApplicationContext("applicationContext-jotm.xml");
		userService = (UserService)app.getBean("userService");
	}
	
	@Test
	public void save(){
		System.out.println("begin...");
		try{
			userService.saveUser();
		}catch(Exception e){
			System.out.println(e.getMessage());
		}
		System.out.println("finish...");
	}
}
```

二、关于使用atomikos实现
(1) 数据源配置
```python
<bean id="dataSourceA" class="com.atomikos.jdbc.SimpleDataSourceBean" init-method="init" destroy-method="close">
	<property name="uniqueResourceName">
		<value>${datasource.uniqueResourceName}</value>
	</property>
	<property name="xaDataSourceClassName"> 
		<value>${database.driver_class}</value> 
	</property> 
	<property name="xaDataSourceProperties">
		<value>URL=${database.url};user=${database.username};password=${database.password}</value> 
	</property> 
	<property name="exclusiveConnectionMode"> 
		<value>${connection.exclusive.mode}</value> 
	</property>
	<property name="connectionPoolSize"> 
		<value>${connection.pool.size}</value>
	</property>
	<property name="connectionTimeout">
		<value>${connection.timeout}</value>
	</property>
	<property name="validatingQuery"> 
		<value>SELECT 1</value> 
	</property> 
</bean>
```
(2)、事务配置
```python
<bean id="atomikosTransactionManager" class="com.atomikos.icatch.jta.UserTransactionManager" 
		init-method="init" destroy-method="close"> 
		<property name="forceShutdown" value="true"/> 
	</bean> 
 
	<bean id="atomikosUserTransaction" class="com.atomikos.icatch.jta.UserTransactionImp"> 
		<property name="transactionTimeout" value="${transaction.timeout}"/> 
	</bean>
 
	<!-- JTA事务管理器 --> 
	<bean id="springTransactionManager" class="org.springframework.transaction.jta.JtaTransactionManager"> 
		<property name="transactionManager" ref="atomikosTransactionManager"/> 
		<property name="userTransaction" ref="atomikosUserTransaction"/> 
	</bean>
 
	<!-- 事务切面配置 --> 
	<aop:config> 
		<aop:pointcut id="serviceOperation"  expression="execution(* *..service*..*(..))"/> 
		<aop:advisor pointcut-ref="serviceOperation" advice-ref="txAdvice"/> 
	</aop:config>
	
	<!-- 通知配置 -->
	<tx:advice id="txAdvice" transaction-manager="springTransactionManager"> 
		<tx:attributes>
			<tx:method name="*" rollback-for="Exception"/> 
		</tx:attributes> 
	</tx:advice>
```

**有关JTA**
JTA全称为Java Transaction API，顾名思义JTA定义了一组统一的事务编程的接口，这些接口如下：
**XAResource**
XAResource接口是对实现了X/Open CAE规范的资源管理器 (Resource Manager，数据库就是典型的资源管理器) 的抽象，它由资源适配器 (Resource Apdater) 提供实现。XAResource是支持事务控制的核心。
**Transaction**
Transaction接口是一个事务实例的抽象，通过它可以控制事务内多个资源的提交或者回滚。二阶段提交过程也是由Transaction接口的实现者来完成的。

TransactionManager
托管模式 (managed mode) 下，TransactionManager接口是被应用服务器调用，以控制事务的边界的。
UserTransaction
非托管模式 (non-managed mode) 下，应用程序可以通过UserTransaction接口控制事务的边界
托管模式下的事务提交场景
![](https://img-blog.csdn.net/20130901231943312)
**注意**：在上图中3和5的步骤之间省略了应用程序对资源的操作 (如CRUD)。另外，应用服务器什么时机 enlistResource，又是什么时候delistResource呢？这在后文中会解释。

**有关JCA**
下图为JCA的架构图
![](https://img-blog.csdn.net/20130901231957500)
中间涉及元素说明如下：
1）Enterprise Information System
简称EIS，在JTA中它又被称为资源管理器。典型的EIS有数据库，事务处理系统(Transaction Processing System)，ERP系统。
2）Resource Adapter
资源适配器(Resource Adaper)是JCA的关键。要想把不同的EIS整合(或者连接)到J2EE运行环境中，就必须为每个EIS提供资源适配器，它会将将EIS适配为一个具备统一编程接口的资源 (Resource) 。这个统一编程接口就是上图中的System Contracts和Client API。下面的UML类图将完美诠释资源适配器。

3）Application Server
应用服务器 (Application Server) 通过System Contracts来管理对EIS的安全、事务、连接等。典型的应用服务器有JBoss、JOnAS、Geronimo、GlassFish等。
4）Application Component
应用组件 (Application Component) ，它封装了应用业务逻辑，像对资源的访问和修改。典型的应用组件就是EJB。
更多细节请参见：
Sun Microsystems Inc.J2EE Connector Architecture 1.5


**参考推荐：**
[Spring分布式事务实现](http://log-cd.iteye.com/blog/807607)
[JTA与JCA分布式事务](http://zhongl.iteye.com/blog/317041)
[理解 JCA 事务](http://www.ibm.com/developerworks/cn/java/j-jca/)（IBM）
[Jencks实现Hibernate与Jackrabbit的分布式事务](http://zhongl.iteye.com/blog/315430)



