# Spring3核心技术之事务管理机制 - z69183787的专栏 - CSDN博客
2014年04月02日 19:57:41[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2453
文章转至http://zhou137520.iteye.com/blog/1675199
Spring对事务的解决办法其实分为2种：编程式实现事务，AOP配置声明式解决方案。
[http://jinnianshilongnian.iteye.com/blog/1496953](http://jinnianshilongnian.iteye.com/blog/1496953)
Spring提供了许多内置事务管理器实现，常用的有以下几种：
- DataSourceTransactionManager：位于org.springframework.jdbc.datasource包中，数据源事务管理器，提供对单个javax.sql.DataSource事务管理，用于Spring JDBC抽象框架、iBATIS框架的事务管理；
- HibernateTransactionManager：位于org.springframework.orm.hibernate3或者hibernate4包中，提供对单个org.hibernate.SessionFactory事务支持，用于集成Hibernate框架时的事务管理；该事务管理器只支持Hibernate3+版本，且Spring3.0+版本只支持Hibernate
 3.2+版本；
- JtaTransactionManager：位于org.springframework.transaction.jta包中，提供对分布式事务管理的支持，并将事务管理委托给Java EE应用服务器事务管理器；
Spring不仅提供这些事务管理器，还提供对如JMS事务管理的管理器
两个不依赖于应用服务器的开源JTA事务实现组件：JOTM和Atomikos Transactions Essentials 
具体用法参考[http://jinnianshilongnian.iteye.com/blog/1439900](http://jinnianshilongnian.iteye.com/blog/1439900)
这篇博客讲解了对于JDBC和JTA分别用PlatformTransactionManager实现和使用TransactionTemplate实现编程式事务管理：[http://jinnianshilongnian.iteye.com/blog/1441271](http://jinnianshilongnian.iteye.com/blog/1441271)
编程式实现事务
Spring提供两种编程式事务支持：直接使用PlatformTransactionManager实现和使用TransactionTemplate模板类，用于支持逻辑事务管理。
如果采用编程式事务推荐使用TransactionTemplate模板类。 
Spring框架支持事务管理的核心是事务管理器抽象，对于不同的数据访问框架（如Hibernate）通过实现策略接口PlatformTransactionManager，从而能支持各种数据访问框架的事务管理，PlatformTransactionManager接口定义如下：
**[java]**[view
 plain](http://blog.csdn.net/edward0830ly/article/details/8703123#)[copy](http://blog.csdn.net/edward0830ly/article/details/8703123#)
- publicinterface PlatformTransactionManager {  
- //返回一个已经激活的事务或创建一个新的事务
-        TransactionStatus getTransaction(TransactionDefinition definition)   
- throws TransactionException;  
- void commit(TransactionStatus status) throws TransactionException;  
- void rollback(TransactionStatus status) throws TransactionException;  
- }  
关于TransactionDefinition接口和TransactionStatus接口： 
[http://jinnianshilongnian.iteye.com/blog/1439900](http://jinnianshilongnian.iteye.com/blog/1439900)
Spring声明式事务 
在日常开发中，用的最多的就是声明式事务了，下面将介绍SpringJdbc的声明式事务的配置方法：
**[html]**[view
 plain](http://blog.csdn.net/edward0830ly/article/details/8703123#)[copy](http://blog.csdn.net/edward0830ly/article/details/8703123#)
- <context:component-scanbase-package="com.chou.spring.jdbc"/>
- 
- <!-- 配置数据源 -->
- <beanid="dataSource"class="com.mchange.v2.c3p0.ComboPooledDataSource"destroy-method="close">
- <!-- Connection Info -->
- <propertyname="driverClass"value="${db.driverClass}"/>
- <propertyname="jdbcUrl"value="${db.url}"/>
- <propertyname="user"value="${db.username}"/>
- <propertyname="password"value="${db.password}"/>
- 
- <!-- Connection Pooling Info -->
- <propertyname="initialPoolSize"value="1"/>
- <propertyname="minPoolSize"value="1"/>
- <propertyname="maxPoolSize"value="15"/>
- <propertyname="maxIdleTime"value="1800"/>
- <propertyname="maxStatements"value="0"/>
- </bean>
- 
- <beanid="jdbcTemplateDao"class="com.chou.spring.jdbc.dao.JdbcTemplateDao">
- <propertyname="dataSource"ref="dataSource"/>
- </bean>
- 
- <!-- JDBC事务管理器 -->
- <beanid="jdbctTxManager"class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
- <propertyname="dataSource"ref="dataSource"/>
- </bean>
- 
- <tx:adviceid="txAdvice"transaction-manager="jdbctTxManager">
- <tx:attributes>
- <tx:methodname="domain*"/>
- </tx:attributes>
- </tx:advice>
- 
- <aop:configproxy-target-class="true">
- <aop:advisoradvice-ref="txAdvice"
- pointcut="execution(* com.chou.spring.jdbc.service.JdbcTemplateService.*(..))"/>
- </aop:config>
**[java]**[view
 plain](http://blog.csdn.net/edward0830ly/article/details/8703123#)[copy](http://blog.csdn.net/edward0830ly/article/details/8703123#)
- publicclass JdbcTemplateDao extends JdbcDaoSupport{  
- 
- publicvoid save() {  
-         String insertSql = "insert into tab_item values(?,?,?)";  
-         Assert.isTrue(getJdbcTemplate().update(insertSql, new Object[]{6, "HP", "PT540"}) == 1, "插入失败");  
-     }  
- 
- publicvoid delete() {  
-         String deleteSql = "delete tab_item where id = ?";  
-         Assert.isTrue(getJdbcTemplate().update(deleteSql, new Object[]{6}) == 1, "删除失败");  
-     }  
- 
- publicvoid update() {  
-         String updateSql = "update tab_item set itemno = ?, itemname = ? where id = ?";  
-         Assert.isTrue(getJdbcTemplate().update(updateSql, new Object[]{"HP", "PT555", 6}) == 1, "修改失败");  
-     }  
- }  
**[java]**[view
 plain](http://blog.csdn.net/edward0830ly/article/details/8703123#)[copy](http://blog.csdn.net/edward0830ly/article/details/8703123#)
- /**
-  * 
-  * @author Chou
-  * @since 2012-9-9
-  * 把事务定义在Service层是为了避免报错：
-  * All calls to this method via a proxy will be routed directly to the proxy.
-  * 这是是事务转移问题，你如果在控制层加入事务就不会有提示了，也没有警告，
-  * 一般很多人在final DAO里加入事务那是有警告的，
-  * 如果配置文件定义了AOP获取代理对象是proxy-target-class="true"即采用CGLIB方式
-  * 而cglib动态代理是利用asm开源包，对代理对象类的class文件加载进来，
-  * 通过修改其字节码生成子类并继承你写的类,然后在你的基础上加事物管理,
-  * 而JdbcDaoSupport中的setDataSource是final的他继承不了
-  * 当然你可以无视它，也没有问题。
-  */
- @Service
- publicclass JdbcTemplateService {  
- 
- @Autowired
- private JdbcTemplateDao jdbcTemplateDao;  
- 
- publicvoid domain(){  
-         jdbcTemplateDao.save();  
- int i = 2/0;//这里出错了，事务就会回滚，之前的save就无效了
-         jdbcTemplateDao.update();  
-         jdbcTemplateDao.delete();  
-     }  
- }  
- 
- //main方法
- String[] configLocations = new String[] {"applicationContext.xml"};    
- ApplicationContext ctx = new ClassPathXmlApplicationContext(configLocations);  
- JdbcTemplateService j = ctx.getBean(JdbcTemplateService.class);  
- j.domain();  
**<tx:advice/>配置详解**
**[html]**[view
 plain](http://blog.csdn.net/edward0830ly/article/details/8703123#)[copy](http://blog.csdn.net/edward0830ly/article/details/8703123#)
- <tx:adviceid="……"transaction-manager="……">
- <tx:attributes>
- <tx:methodname="*"
- propagation="REQUIRED"
- isolation="DEFAULT"
- timeout="-1"
- read-only="true"
- no-rollback-for=""
- rollback-for="java.lang.Exception"/>
- </tx:attributes>
- </tx:advice>
- 
- <!-- 最常用的配置 -->
- <tx:adviceid="txAdvice"transaction-manager="txManager">
- <tx:attributes>
- <tx:methodname="save*"propagation="REQUIRED"/>
- <tx:methodname="add*"propagation="REQUIRED"/>
- <tx:methodname="create*"propagation="REQUIRED"/>
- <tx:methodname="insert*"propagation="REQUIRED"/>
- <tx:methodname="update*"propagation="REQUIRED"/>
- <tx:methodname="merge*"propagation="REQUIRED"/>
- <tx:methodname="del*"propagation="REQUIRED"/>
- <tx:methodname="remove*"propagation="REQUIRED"/>
- <tx:methodname="put*"propagation="REQUIRED"/>
- <tx:methodname="get*"propagation="SUPPORTS"read-only="true"/>
- <tx:methodname="count*"propagation="SUPPORTS"read-only="true"/>
- <tx:methodname="find*"propagation="SUPPORTS"read-only="true"/>
- <tx:methodname="list*"propagation="SUPPORTS"read-only="true"/>
- <tx:methodname="*"propagation="SUPPORTS"read-only="true"/>
- <tx:methodname="batchSaveOrUpdate"propagation="REQUIRES_NEW"/>
- </tx:attributes>
- </tx:advice>
- <aop:config>
- <aop:pointcutid="txPointcut"expression="execution(* cn.javass..service.*.*(..))"/>
- <aop:advisoradvice-ref="txAdvice"pointcut-ref="txPointcut"/>
- </aop:config>
XML形式的事务配置<tx:method >的属性详解
|属性|类型|默认值|说明|
|----|----|----|----|
|propagation|Propagation枚举|REQUIRED|事务传播属性|
|isolation|isolation枚举|DEFAULT(所用数据库默认级别)|事务隔离级别|
|readOnly|boolean|false|是否才用优化的只读事务|
|timeout|int|-1|超时(秒)|
|rollbackFor|Class[]|{}|需要回滚的异常类|
|rollbackForClassName|String[]|{}|需要回滚的异常类名|
|noRollbackFor|Class[]|{}|不需要回滚的异常类|
|noRollbackForClassName|String[]|{}|不需要回滚的异常类名|
readOnly
事务属性中的readOnly标志表示对应的事务应该被最优化为只读事务。如果值为true就会告诉Spring我这个方法里面没有insert或者update，你只需要提供只读的数据库Connection就行了，这种执行效率会比read-write的Connection高，所以这是一个最优化提示。在一些情况下，一些事务策略能够起到显著的最优化效果，例如在使用Object/Relational映射工具（如：Hibernate或TopLink）时避免dirty
 checking（试图“刷新”）。 
timeout
在属性中还有定义“timeout”值的选项，指定事务超时为几秒。一般不会使用这个属性。在JTA中，这将被简单地传递到J2EE服务器的事务协调程序，并据此得到相应的解释。 
Isolation Level(事务隔离等级)的5个枚举值
为什么事务要有Isolation Level这个属性？先回顾下数据库事务的知识： 
第一类丢失更新(lost update)：在完全未隔离事务的情况下，两个事物更新同一条数据资源，某一事物异常终止，回滚造成第一个完成的更新也同时丢失。 
第二类丢失更新(second lost updates)：是不可重复读的特殊情况，如果两个事务都读取同一行，然后两个都进行写操作，并提交，第一个事务所做的改变就会丢失。 
脏读(dirty read)：如果第二个事务查询到第一个事务还未提交的更新数据，形成脏读。因为第一个事务你还不知道是否提交，所以数据不一定是正确的。 
虚读(phantom read)：一个事务执行两次查询，第二次结果集包含第一次中没有或者某些行已被删除，造成两次结果不一致，只是另一个事务在这两次查询中间插入或者删除了数据造成的。 
不可重复读(unrepeated read)：一个事务两次读取同一行数据，结果得到不同状态结果，如中间正好另一个事务更新了该数据，两次结果相异，不可信任。 
具体关于事务机制可以看我以前的博客：[http://zhou137520.iteye.com/admin/blogs/1638574](http://zhou137520.iteye.com/admin/blogs/1638574)
当遇到以上这些情况时我们可以设置isolation下面这些枚举值： 
DEFAULT：采用数据库默认隔离级别 
SERIALIZABLE：最严格的级别，事务串行执行，资源消耗最大； 
REPEATABLE_READ：保证了一个事务不会修改已经由另一个事务读取但未提交（回滚）的数据。避免了“脏读取”和“不可重复读取”的情况，但是带来了更多的性能损失。 
READ_COMMITTED:**大多数主流数据库的默认事务等级**，保证了一个事务不会读到另一个并行事务已修改但未提交的数据，避免了“脏读取”。该级别适用于大多数系统。 
READ_UNCOMMITTED：保证了读取过程中不会读取到非法数据。隔离级别在于处理多事务的并发问题。 
关于propagation属性的7个传播行为
**REQUIRED**：指定当前方法必需在事务环境中运行，如果当前有事务环境就加入当前正在执行的事务环境，如果当前没有事务，就新建一个事务。这是默认值。 
SUPPORTS：指定当前方法加入当前事务环境，如果当前没有事务，就以非事务方式执行。 
**MANDATORY**：指定当前方法必须加入当前事务环境，如果当前没有事务，就抛出异常。 
REQUIRES_NEW：指定当前方法总是会为自己发起一个新的事务，如果发现当前方法已运行在一个事务中,则原有事务被挂起,我自己创建一个属于自己的事务,直我自己这个方法commit结束,原先的事务才会恢复执行。 
NOT_SUPPORTED：指定当前方法以非事务方式执行操作，如果当前存在事务，就把当前事务挂起，等我以非事务的状态运行完，再继续原来的事务。 
NEVER：指定当前方法绝对不能在事务范围内执行，如果方法在某个事务范围内执行，容器就抛异常，只有没关联到事务，才正常执行。 
NESTED：指定当前方法执行时，如果已经有一个事务存在,则运行在这个嵌套的事务中.如果当前环境没有运行的事务，就新建一个事务，并与父事务相互独立，这个事务拥有多个可以回滚的保证点。就是指我自己内部事务回滚不会对外部事务造成影响，只对DataSourceTransactionManager事务管理器起效。 
注解形式@Transactional实现事务管理
注意@Transactional只能被应用到public方法上，对于其它非public的方法，如果标记了@Transactional也不会报错,但方法没有事务功能。 
默认情况下，一个有事务的方法，遇到RuntiomeException时会回滚。遇到受检查的异常是不会回滚的，要想所有异常都回滚，要加上属性rollbackFor={Exception.class}
**[html]**[view
 plain](http://blog.csdn.net/edward0830ly/article/details/8703123#)[copy](http://blog.csdn.net/edward0830ly/article/details/8703123#)
- <!-- 事务管理器配置 -->
- <beanid="txManager"class="org.springframework.orm.hibernate3.HibernateTransactionManager">
- <propertyname="sessionFactory"ref="sessionFactory"/>
- </bean>
- 
- <!-- 使用annotation定义事务 -->
- <tx:annotation-driventransaction-manager="txManager"proxy-target-class="true"/>
transaction-manager：指定事务管理器名字，默认为transactionManager，当使用其他名字时需要明确指定； 
proxy-target-class：默认false表示使用JDK代理，如果为true将使用CGLIB代理 
order：定义事务通知顺序，默认Ordered.LOWEST_PRECEDENCE，表示将顺序决定权交给AOP来处理。 
建议只在实现类或实现类的方法上使用@Transactional，而不要在接口上使用，这是因为如果使用JDK代理机制是没问题，因为其使用基于接口的代理；而使用使用CGLIB代理机制时就会遇到问题，因为其使用基于类的代理而不是接口，这是因为接口上的@Transactional注解是“不能继承的”。 
[http://jinnianshilongnian.iteye.com/blog/1508018](http://jinnianshilongnian.iteye.com/blog/1508018)这篇博客讲解了基于JDK动态代理和CGLIB动态代理的实现Spring注解管理事务（@Trasactional）到底有什么区别。
**[java]**[view
 plain](http://blog.csdn.net/edward0830ly/article/details/8703123#)[copy](http://blog.csdn.net/edward0830ly/article/details/8703123#)
- @Transactional//放在这里表示所有方法都加入事务管理
- publicclass AnnotationUserServiceImpl implements IUserService {  
- private IUserDao userDao;  
- private IAddressService addressService;  
- 
- @Transactional(propagation=Propagation.REQUIRED, isolation=Isolation.READ_COMMITTED)  
- publicvoid save(UserModel user) {  
-         userDao.save(user);  
-         user.getAddress().setUserId(user.getId());  
-         addressService.save(user.getAddress());  
-     }  
- 
- @Transactional(propagation=Propagation.REQUIRED, readOnly=true,  
-                    isolation=Isolation.READ_COMMITTED)  
- publicint countAll() {  
- return userDao.countAll();  
-     }  
- //setter...
- }  
**总结**
编程式事务是不推荐的，即使有很少事务操作，Spring发展到现在，没有理由使用编程式事务，只有在为了深入理解Spring事务管理才需要学习编程式事务使用。 
       推荐使用声明式事务，而且强烈推荐使用<tx:tags>方式的声明式事务，因为其是无侵入代码的，可以配置模板化的事务属性并运用到多个项目中。 
       而@Transaction注解事务，不过如果一个项目模块太多，service方法太多导致每个方法都要手动去加注解，是不是很麻烦，也容易出错。如果一个项目结构清晰，分层明确，那么标签形式的配置将是最直观和方便的办法。 
       总之，能保证项目正常工作的事务配置就是最好的。 ![](http://zhou137520.iteye.com/images/smiles/icon_lol.gif)
