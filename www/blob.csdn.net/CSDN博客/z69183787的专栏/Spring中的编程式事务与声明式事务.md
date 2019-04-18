# Spring中的编程式事务与声明式事务 - z69183787的专栏 - CSDN博客
2012年12月19日 15:35:57[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2210
编程式事务：
使用PlatformTransactionManager及TransactionTemplate ，其中PlatformTransactionManager可在xml文件中配置
1.声明数据源
2.声明一个事务管理类，例如：DataSourceTransactionManager,HibernateTransactionManger,JTATransactionManager等 
```
<bean id="dsTransactionManager" class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
		<property name="dataSource" ref="stptdemo" />
	</bean>
```
需要注入dataSource数据源。
3.事务处理方式：（TransactionTemplate也是为我们省去了部分事务提交、回滚代码；定义事务模板时，需注入事务管理对象。 ）
```java
private static PlatformTransactionManager transactionManager;
	try{
			TransactionTemplate tt = new TransactionTemplate(transactionManager);
			tt.execute(new TransactionCallback() {
				public Object doInTransaction(TransactionStatus status) {
					JdbcTemplate jt = DbUtil.getJdbcTemplate("stptdemo");
					StatInstance instance = new AuditStatInstance(jt);
					instance.action(list, sql);
					return null;
				}
			});  
		}catch(Exception e){
			log.error("service error:"+e.getMessage());
		}
public static PlatformTransactionManager getTransactionManager() {
		return transactionManager;
	}
	@Resource(name="dsTransactionManager")
	public void setTransactionManager(PlatformTransactionManager transactionManager) {
		AuditManager.transactionManager = transactionManager;
	}
```
声明式事务：
提供了TransactionInterceptor拦截器和常用的代理类TransactionProxyFactoryBean，可以直接对组件进行事务代理。
也可使用全注解方式。
1.定义数据源
2 配置事务管理器 xml中配置事务管理器，注入sessionfactory
```
<bean id="txManager" class="org.springframework.orm.hibernate3.HibernateTransactionManager">
       	<property name="sessionFactory" ref="sessionFactory"/>
	</bean>
```
具体实现类中：
3.全注解配置
```java
@Transactional(value="txManager" propagation = Propagation.REQUIRED, rollbackFor = Exception.class)
/* 功能模块入口点，beanid即数据库配置中的name */
@Service("sampleService")
@Scope("prototype")
public class SampleService implements ITaskService
```
具体声明式事务的5种模式可在 这里看到：[http://blog.csdn.net/z69183787/article/details/8199932](http://blog.csdn.net/z69183787/article/details/8199932)
