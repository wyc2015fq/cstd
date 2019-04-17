# Spring不同事务管理方式与声明式事务管理局部回滚处理方案 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年03月23日 13:40:35[boonya](https://me.csdn.net/boonya)阅读数：1860








Spring配置文件中关于事务配置总是由三个组成部分，分别是DataSource、TransactionManager和代理机制这三部分，无论哪种配置方式，一般变化的只是代理机制这部分。  DataSource、TransactionManager这两部分只是会根据数据访问方式有所变化。

比如：使用Hibernate进行数据访问时，DataSource实际为SessionFactory，TransactionManager的实现为HibernateTransactionManager。

具体如下图：
![](https://img-blog.csdn.net/20170323131754803)


## Spring提供的五中事务管理方式





 根据代理机制的不同，总结了五种Spring事务的配置方式，配置文件如下：

###  第一种方式：每个Bean都有一个代理



```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xmlns:context="http://www.springframework.org/schema/context"
    xmlns:aop="http://www.springframework.org/schema/aop"
    xsi:schemaLocation="http://www.springframework.org/schema/beans 
           http://www.springframework.org/schema/beans/spring-beans-2.5.xsd
           http://www.springframework.org/schema/context
           http://www.springframework.org/schema/context/spring-context-2.5.xsd
           http://www.springframework.org/schema/aop http://www.springframework.org/schema/aop/spring-aop-2.5.xsd">

    <bean id="sessionFactory"  
            class="org.springframework.orm.hibernate3.LocalSessionFactoryBean">  
        <property name="configLocation" value="classpath:hibernate.cfg.xml" />  
        <property name="configurationClass" value="org.hibernate.cfg.AnnotationConfiguration" />
    </bean>  

    <!-- 定义事务管理器（声明式的事务） -->  
    <bean id="transactionManager"
        class="org.springframework.orm.hibernate3.HibernateTransactionManager">
        <property name="sessionFactory" ref="sessionFactory" />
    </bean>
    
    <!-- 配置DAO -->
    <bean id="userDaoTarget" class="com.bluesky.spring.dao.UserDaoImpl">
        <property name="sessionFactory" ref="sessionFactory" />
    </bean>
    
    <bean id="userDao"  
        class="org.springframework.transaction.interceptor.TransactionProxyFactoryBean">  
           <!-- 配置事务管理器 -->  
           <property name="transactionManager" ref="transactionManager" />     
        <property name="target" ref="userDaoTarget" />  
         <property name="proxyInterfaces" value="com.bluesky.spring.dao.GeneratorDao" />
        <!-- 配置事务属性 -->  
        <property name="transactionAttributes">  
            <props>  
                <prop key="*">PROPAGATION_REQUIRED</prop>
            </props>  
        </property>  
    </bean>  
</beans>
```




### 第二种方式：所有Bean共享一个代理基类



```
<?xml version="1.0" encoding="UTF-8"?>  
    <beans xmlns="http://www.springframework.org/schema/beans"  
        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"  
        xmlns:context="http://www.springframework.org/schema/context"  
        xmlns:aop="http://www.springframework.org/schema/aop"  
        xsi:schemaLocation="http://www.springframework.org/schema/beans   
               http://www.springframework.org/schema/beans/spring-beans-2.5.xsd  
               http://www.springframework.org/schema/context  
               http://www.springframework.org/schema/context/spring-context-2.5.xsd  
               http://www.springframework.org/schema/aop http://www.springframework.org/schema/aop/spring-aop-2.5.xsd">  
      
        <bean id="sessionFactory"    
                class="org.springframework.orm.hibernate3.LocalSessionFactoryBean">    
            <property name="configLocation" value="classpath:hibernate.cfg.xml" />    
            <property name="configurationClass" value="org.hibernate.cfg.AnnotationConfiguration" />  
        </bean>    
      
        <!-- 定义事务管理器（声明式的事务） -->    
        <bean id="transactionManager"  
            class="org.springframework.orm.hibernate3.HibernateTransactionManager">  
            <property name="sessionFactory" ref="sessionFactory" />  
        </bean>  
          
        <bean id="transactionBase"    
                class="org.springframework.transaction.interceptor.TransactionProxyFactoryBean"    
                lazy-init="true" abstract="true">    
            <!-- 配置事务管理器 -->    
            <property name="transactionManager" ref="transactionManager" />    
            <!-- 配置事务属性 -->    
            <property name="transactionAttributes">    
                <props>    
                    <prop key="*">PROPAGATION_REQUIRED</prop>    
                </props>    
            </property>    
        </bean>      
         
        <!-- 配置DAO -->  
        <bean id="userDaoTarget" class="com.bluesky.spring.dao.UserDaoImpl">  
            <property name="sessionFactory" ref="sessionFactory" />  
        </bean>  
          
        <bean id="userDao" parent="transactionBase" >    
            <property name="target" ref="userDaoTarget" />     
        </bean>  
    </beans>
```




### 第三种方式：使用拦截器



```
<?xml version="1.0" encoding="UTF-8"?>  
    <beans xmlns="http://www.springframework.org/schema/beans"  
        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"  
        xmlns:context="http://www.springframework.org/schema/context"  
        xmlns:aop="http://www.springframework.org/schema/aop"  
        xsi:schemaLocation="http://www.springframework.org/schema/beans   
               http://www.springframework.org/schema/beans/spring-beans-2.5.xsd  
               http://www.springframework.org/schema/context  
               http://www.springframework.org/schema/context/spring-context-2.5.xsd  
               http://www.springframework.org/schema/aop http://www.springframework.org/schema/aop/spring-aop-2.5.xsd">  
      
        <bean id="sessionFactory"    
                class="org.springframework.orm.hibernate3.LocalSessionFactoryBean">    
            <property name="configLocation" value="classpath:hibernate.cfg.xml" />    
            <property name="configurationClass" value="org.hibernate.cfg.AnnotationConfiguration" />  
        </bean>    
      
        <!-- 定义事务管理器（声明式的事务） -->    
        <bean id="transactionManager"  
            class="org.springframework.orm.hibernate3.HibernateTransactionManager">  
            <property name="sessionFactory" ref="sessionFactory" />  
        </bean>   
         
        <bean id="transactionInterceptor"    
            class="org.springframework.transaction.interceptor.TransactionInterceptor">    
            <property name="transactionManager" ref="transactionManager" />    
            <!-- 配置事务属性 -->    
            <property name="transactionAttributes">    
                <props>    
                    <prop key="*">PROPAGATION_REQUIRED</prop>    
                </props>    
            </property>    
        </bean>  
            
        <bean class="org.springframework.aop.framework.autoproxy.BeanNameAutoProxyCreator">    
            <property name="beanNames">    
                <list>    
                    <value>*Dao</value>  
                </list>    
            </property>    
            <property name="interceptorNames">    
                <list>    
                    <value>transactionInterceptor</value>    
                </list>    
            </property>    
        </bean>    
        
        <!-- 配置DAO -->  
        <bean id="userDao" class="com.bluesky.spring.dao.UserDaoImpl">  
            <property name="sessionFactory" ref="sessionFactory" />  
        </bean>  
    </beans>
```




### 第四种方式：使用tx标签配置的拦截器



```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xmlns:context="http://www.springframework.org/schema/context"
    xmlns:aop="http://www.springframework.org/schema/aop"
    xmlns:tx="http://www.springframework.org/schema/tx"
    xsi:schemaLocation="http://www.springframework.org/schema/beans 
           http://www.springframework.org/schema/beans/spring-beans-2.5.xsd
           http://www.springframework.org/schema/context
           http://www.springframework.org/schema/context/spring-context-2.5.xsd
           http://www.springframework.org/schema/aop http://www.springframework.org/schema/aop/spring-aop-2.5.xsd
           http://www.springframework.org/schema/tx http://www.springframework.org/schema/tx/spring-tx-2.5.xsd">

    <context:annotation-config />
    <context:component-scan base-package="com.bluesky" />

    <bean id="sessionFactory"  
            class="org.springframework.orm.hibernate3.LocalSessionFactoryBean">  
        <property name="configLocation" value="classpath:hibernate.cfg.xml" />  
        <property name="configurationClass" value="org.hibernate.cfg.AnnotationConfiguration" />
    </bean>  

    <!-- 定义事务管理器（声明式的事务） -->  
    <bean id="transactionManager"
        class="org.springframework.orm.hibernate3.HibernateTransactionManager">
        <property name="sessionFactory" ref="sessionFactory" />
    </bean>

    <tx:advice id="txAdvice" transaction-manager="transactionManager">
        <tx:attributes>
            <tx:method name="*" propagation="REQUIRED" />
        </tx:attributes>
    </tx:advice>
    
    <aop:config>
        <aop:pointcut id="interceptorPointCuts"
            expression="execution(* com.bluesky.spring.dao.*.*(..))" />
        <aop:advisor advice-ref="txAdvice"
            pointcut-ref="interceptorPointCuts" />        
    </aop:config>      
</beans>
```




### 第五种方式：全注解@Transactional



```
<?xml version="1.0" encoding="UTF-8"?>  
    <beans xmlns="http://www.springframework.org/schema/beans"  
        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"  
        xmlns:context="http://www.springframework.org/schema/context"  
        xmlns:aop="http://www.springframework.org/schema/aop"  
        xmlns:tx="http://www.springframework.org/schema/tx"  
        xsi:schemaLocation="http://www.springframework.org/schema/beans   
               http://www.springframework.org/schema/beans/spring-beans-2.5.xsd  
               http://www.springframework.org/schema/context  
               http://www.springframework.org/schema/context/spring-context-2.5.xsd  
               http://www.springframework.org/schema/aop http://www.springframework.org/schema/aop/spring-aop-2.5.xsd  
               http://www.springframework.org/schema/tx http://www.springframework.org/schema/tx/spring-tx-2.5.xsd">  
      
        <context:annotation-config />  
        <context:component-scan base-package="com.bluesky" />  
      
        <tx:annotation-driven transaction-manager="transactionManager"/>  
      
        <bean id="sessionFactory"    
                class="org.springframework.orm.hibernate3.LocalSessionFactoryBean">    
            <property name="configLocation" value="classpath:hibernate.cfg.xml" />    
            <property name="configurationClass" value="org.hibernate.cfg.AnnotationConfiguration" />  
        </bean>    
      
        <!-- 定义事务管理器（声明式的事务） -->    
        <bean id="transactionManager"  
            class="org.springframework.orm.hibernate3.HibernateTransactionManager">  
            <property name="sessionFactory" ref="sessionFactory" />  
        </bean>  
          
    </beans>
```

```java
package com.bluesky.spring.dao;  
      
    import java.util.List;  
      
    import org.hibernate.SessionFactory;  
    import org.springframework.beans.factory.annotation.Autowired;  
    import org.springframework.orm.hibernate3.support.HibernateDaoSupport;  
    import org.springframework.stereotype.Component;  
      
    import com.bluesky.spring.domain.User;  
      
    @Transactional  
    @Component("userDao")  
    public class UserDaoImpl extends HibernateDaoSupport implements UserDao {  
      
        public List<User> listUsers() {  
            return this.getSession().createQuery("from User").list();  
        }  
          
          
    }
```

注：除了在类前面加@Transactional可以在方法上加。





## Spring事务管理的特性




### spring支持事务管理两种方式：编程式事务管理和声明式事务管理



        编程式事务管理使用TransactionTemplate或者直接使用底层的PlatformTransactionManager。对于编程式事务管理，spring推荐使用TransactionTemplate。

 声明式事务管理建立在AOP之上的。其本质是对方法前后进行拦截，然后在目标方法开始之前创建或者加入一个事务，在执行完目标方法之后根据执行情况提交或者回滚事务。声明式事务最大的优点就是不需要通过编程的方式管理事务，这样就不需要在业务逻辑代码中掺杂事务管理的代码，只需在配置文件中做相关的事务规则声明(或通过基于@Transactional注解的方式)，便可以将事务规则应用到业务逻辑中。

显然声明式事务管理要优于编程式事务管理，这正是**spring倡导的非侵入式的开发方式**。声明式事务管理使业务代码不受污染，一个普通的POJO对象，只要加上注解就可以获得完全的事务支持。和编程式事务相比，声明式事务唯一不足地方是，后者的最细粒度只能作用到方法级别，无法做到像编程式事务那样可以作用到代码块级别。但是即便有这样的需求，也存在很多变通的方法，比如，可以将需要进行事务管理的代码块独立为方法等等。

  声明式事务管理也有两种常用的方式，一种是基于tx和aop名字空间的xml配置文件，另一种就是基于@Transactional注解。显然基于注解的方式更简单易用，更清爽。


### Spring事务特性


Spring所有的事务管理策略类都继承自org.springframework.transaction.PlatformTransactionManager接口；其中TransactionDefinition接口定义以下特性：



#### 事务隔离级别


隔离级别是指若干个并发的事务之间的隔离程度。TransactionDefinition 接口中定义了五个表示隔离级别的常量：


-     TransactionDefinition.ISOLATION_DEFAULT：这是默认值，表示使用底层数据库的默认隔离级别。对大部分数据库而言，通常这值就是  TransactionDefinition.ISOLATION_READ_COMMITTED。
-     TransactionDefinition.ISOLATION_READ_UNCOMMITTED：该隔离级别表示一个事务可以读取另一个事务修改但还没有提交的数据。该级别不能防止脏读，不可重复读和幻读，因此很少使用该隔离级别。比如PostgreSQL实际上并没有此级别。
-     TransactionDefinition.ISOLATION_READ_COMMITTED：该隔离级别表示一个事务只能读取另一个事务已经提交的数据。该级别可以防止脏读，这也是大多数情况下的推荐值。
-     TransactionDefinition.ISOLATION_REPEATABLE_READ：该隔离级别表示一个事务在整个过程中可以多次重复执行某个查询，并且每次返回的记录都相同。该级别可以防止脏读和不可重复读。
-     TransactionDefinition.ISOLATION_SERIALIZABLE：所有的事务依次逐个执行，这样事务之间就完全不可能产生干扰，也就是说，该级别可以防止脏读、不可重复读以及幻读。但是这将严重影响程序的性能。通常情况下也不会用到该级别。




#### 事务传播行为

所谓事务的传播行为是指，如果在开始当前事务之前，一个事务上下文已经存在，此时有若干选项可以指定一个事务性方法的执行行为。在TransactionDefinition定义中包括了如下几个表示传播行为的常量：


-     TransactionDefinition.PROPAGATION_REQUIRED：如果当前存在事务，则加入该事务；如果当前没有事务，则创建一个新的事务。这是默认值。
-     TransactionDefinition.PROPAGATION_REQUIRES_NEW：创建一个新的事务，如果当前存在事务，则把当前事务挂起。
-     TransactionDefinition.PROPAGATION_SUPPORTS：如果当前存在事务，则加入该事务；如果当前没有事务，则以非事务的方式继续运行。
-     TransactionDefinition.PROPAGATION_NOT_SUPPORTED：以非事务方式运行，如果当前存在事务，则把当前事务挂起。
-     TransactionDefinition.PROPAGATION_NEVER：以非事务方式运行，如果当前存在事务，则抛出异常。
-     TransactionDefinition.PROPAGATION_MANDATORY：如果当前存在事务，则加入该事务；如果当前没有事务，则抛出异常。
-     TransactionDefinition.PROPAGATION_NESTED：如果当前存在事务，则创建一个事务作为当前事务的嵌套事务来运行；如果当前没有事务，则该取值等价于TransactionDefinition.PROPAGATION_REQUIRED。




#### 事务超时

所谓事务超时，就是指一个事务所允许执行的最长时间，如果超过该时间限制但事务还没有完成，则自动回滚事务。在 TransactionDefinition 中以 int 的值来表示超时时间，其单位是秒。

  默认设置为底层事务系统的超时值，如果底层数据库事务系统没有设置超时值，那么就是none，没有超时限制。



#### 事务只读属性

 只读事务用于客户代码只读但不修改数据的情形，只读事务用于特定情景下的优化，比如使用Hibernate查询的时候，默认为只读事务。


 “只读事务”并不是一个强制选项，它只是一个“暗示”，提示数据库驱动程序和数据库系统，这个事务并不包含更改数据的操作，那么JDBC驱动程序和数据库就有可能根据这种情况对该事务进行一些特定的优化，比方说不安排相应的数据库锁，以减轻事务对数据库的压力，毕竟事务也是要消耗数据库的资源的。


但是你非要在“只读事务”里面修改数据，也并非不可以，只不过对于数据一致性的保护不像“读写事务”那样保险而已。 因此，“只读事务”仅仅是一个性能优化的推荐配置而已，并非强制你要这样做不可。


#### spring事务回滚规则



     指示Spring事务管理器回滚一个事务的推荐方法是在当前事务的上下文内抛出异常。spring事务管理器会捕捉任何未处理的异常，然后依据规则决定是否回滚抛出异常的事务。


        默认配置下，Spring只有在抛出的异常为运行时unchecked异常时才回滚该事务，也就是抛出的异常为RuntimeException的子类(Errors也会导致事务回滚)，而抛出checked异常则不会导致事务回滚。可以明确的配置在抛出那些异常时回滚事务，包括checked异常。也可以明确定义那些异常抛出时不回滚事务。还可以编程性的通过setRollbackOnly()方法来指示一个事务必须回滚，在调用完setRollbackOnly()后你所能执行的唯一操作就是回滚。





### **@Transactional注解**

详细的可以参考—— 《Spring 事务管理@Transactional》 ：[http://blog.csdn.net/boonya/article/details/51303640](http://blog.csdn.net/boonya/article/details/51303640)




@Transactional属性
|属性|类型|描述|
|----|----|----|
|value|String|可选的限定描述符，指定使用的事务管理器|
|propagation|enum: Propagation|可选的事务传播行为设置|
|isolation|enum: Isolation|可选的事务隔离级别设置|
|readOnly|boolean|读写或只读事务，默认读写|
|timeout|int (in seconds granularity)|事务超时时间设置|
|rollbackFor|Class对象数组，必须继承自Throwable|导致事务回滚的异常类数组|
|rollbackForClassName|类名数组，必须继承自Throwable|导致事务回滚的异常类名字数组|
|noRollbackFor|Class对象数组，必须继承自Throwable|不会导致事务回滚的异常类数组|
|noRollbackForClassName|类名数组，必须继承自Throwable|不会导致事务回滚的异常类名字数组|

**用法**

**@Transactional 可以作用于接口、接口方法、类以及类方法上。**

当作用于类上时，该类的所有 public 方法将都具有该类型的事务属性，同时，我们也可以在方法级别使用该标注来覆盖类级别的定义。

        虽然 @Transactional 注解可以作用于接口、接口方法、类以及类方法上，但是 Spring 建议不要在接口或者接口方法上使用该注解，因为这只有在使用基于接口的代理时它才会生效。

        另外， @Transactional 注解应该只被应用到 public 方法上，这是由 Spring AOP 的本质决定的。如果你在 protected、private
 或者默认可见性的方法上使用 @Transactional 注解，这将被忽略，也不会抛出任何异常。

        默认情况下，只有来自外部的方法调用才会被AOP代理捕获，也就是，类内部方法调用本类内部的其他方法并不会引起事务行为，即使被调用方法使用@Transactional注解进行修饰。


## Spring Mybatis事务管理示例：批量操作部分回滚




**实体类：WmTransaction.java**




```java
public class WmTransaction {
    private Integer id;

    private String url;

    public Integer getId() {
        return id;
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public String getUrl() {
        return url;
    }

    public void setUrl(String url) {
        this.url = url == null ? null : url.trim();
    }
}
```
**接口类：WmTransactionService.java**




```java
public interface WmTransactionService extends BaseService<WmTransaction, Integer>{
	
	/**
	 * 插入事务测试对象
	 *  
	 * @MethodName: insert 
	 * @Description: 
	 * @param wmTransaction
	 * @throws
	 */
	public Integer insert(WmTransaction wmTransaction);
	
	/**
	 * 批量插入
	 * 
	 * @MethodName: insertBatch 
	 * @Description: 
	 * @param wmTransaction
	 * @throws
	 */
	public void insertBatch(List<WmTransaction>  wmTransaction);

}
```
**实现类：WmTransactionServiceImpl.java**




```java
@Service
public class WmTransactionServiceImpl extends BaseServiceImpl<WmTransaction, Integer> implements WmTransactionService {
	
	@Autowired
	WmTransactionMapper wmTransactionMapper;
	
	@SuppressWarnings("unchecked")
	@PostConstruct
	private void init() {
		this.dao = wmTransactionMapper;
	}
	
	@Override
	public Integer insert(WmTransaction wmTransaction){
		try {
			return wmTransactionMapper.insert(wmTransaction);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return -1;
	}

	@Transactional(noRollbackFor=Exception.class)//杜绝循环出错全部回滚
	@Override
	public void insertBatch(List<WmTransaction> wmTransactions) {
		if(wmTransactions==null||wmTransactions.size()==0){
			return ;
		}
		for (WmTransaction wmTransaction : wmTransactions) {
			int result=insert(wmTransaction);
			Log.getLogger(getClass()).info("insert into transaction:"+(result>0));
		}
	}

	

}
```

**测试类：WmTransactionServiceTest.java**




```java
@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations = "classpath:applicationContext.xml")
public class WmTransactionServiceTest {
	
	@Autowired
	WmTransactionService wmTransactionService;
	
	@Test
	public void testTransaction(){
		WmTransaction trans1=new WmTransaction();
		trans1.setUrl("https://www.baidu.com"); 
		WmTransaction trans2=new WmTransaction();
		trans2.setUrl("https://www.baidu.com"); 
		WmTransaction trans3=new WmTransaction();
		trans3.setUrl("https://www.baidu.com/hsd/shiodhfiodsfdfdfs/hello.html"); 
		WmTransaction trans4=new WmTransaction();
		trans4.setUrl("https://www.baidu.com"); 
		List<WmTransaction> list=new ArrayList<WmTransaction>();
		list.add(trans1);
		list.add(trans2);
		list.add(trans3);
		list.add(trans4);
		
		wmTransactionService.insertBatch(list);
		
		try {
			Thread.sleep(50000);// 主线程等待执行完毕
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}

}
```


注：数据库url字段长度为varchar（30），所以trans3的保存会产生异常，但insertBatch整个事务不回滚。



**AOP切面事务配置**



```
<bean name="transactionManager"
        class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
        <property name="dataSource" ref="dataSource"></property>
    </bean>

<tx:advice id="txAdvice" transaction-manager="transactionManager">
		<tx:attributes>
			<tx:method name="del*" propagation="REQUIRED" read-only="false"
				rollback-for="java.lang.Exception" />
			<tx:method name="insert*" propagation="REQUIRED" read-only="false"
				rollback-for="java.lang.Exception" />
			<!-- <tx:method name="batch*" propagation="REQUIRED" read-only="false"
				no-rollback-for="java.lang.Exception" /> -->
			<tx:method name="update*" propagation="REQUIRED" read-only="false"
				rollback-for="java.lang.Exception" />
			<tx:method name="execute*" propagation="REQUIRED" read-only="false"
				rollback-for="java.lang.Exception" />
			<tx:method name="*" propagation="REQUIRED" read-only="true" />
		</tx:attributes>
	</tx:advice>

	<!-- 事务拦截 -->
	<aop:config>
		<aop:pointcut id="pc"
			expression="execution(* com.wlyd.fmcgwms.service..*(..))" />
		<aop:advisor pointcut-ref="pc" advice-ref="txAdvice" />
	</aop:config>
```


注：实际上上面示例所说的事务管理都是基于AOP声明式事务管理的，只是简单地用了@Transactional(noRollbackFor=Exception.class)来使整个循环不因为某个事务异常而全部回滚。如果要在事务方法控制如上面`<tx:method name="batch*" propagation="REQUIRED" read-only="false"no-rollback-for="java.lang.Exception" />`batch*所有的方法内部遇到Exception都不会回滚。



## 参考资料

Spring事务管理几种方式：[http://lzh166.iteye.com/blog/1134146](http://lzh166.iteye.com/blog/1134146)




Spring事物配置，声明式事务管理和基于@Transactional注解的使用：[http://blog.csdn.net/bao19901210/article/details/41724355](http://blog.csdn.net/bao19901210/article/details/41724355)





 Spring 事务管理@Transactional ：[http://blog.csdn.net/boonya/article/details/51303640](http://blog.csdn.net/boonya/article/details/51303640)











