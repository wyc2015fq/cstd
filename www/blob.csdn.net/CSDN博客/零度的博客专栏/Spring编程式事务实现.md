# Spring编程式事务实现 - 零度的博客专栏 - CSDN博客
2016年04月08日 10:35:34[零度anngle](https://me.csdn.net/zmx729618)阅读数：3432
**编程式事务概述**
       所谓编程式事务指的是通过编码方式实现事务，即类似于JDBC编程实现事务管理。
       Spring框架提供一致的事务抽象，因此对于JDBC还是JTA事务都是采用相同的API进行编程。
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2506.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2506.html#)[打印](http://sishuok.com/forum/blogPost/list/2506.html#)
- Connection conn = null;  
- UserTransaction tx = null;  
- try {  
-     tx = getUserTransaction();                       //1.获取事务
-     tx.begin();                                    //2.开启JTA事务
-     conn = getDataSource().getConnection();           //3.获取JDBC
- //4.声明SQL
-     String sql = "select * from INFORMATION_SCHEMA.SYSTEM_TABLES";  
-     PreparedStatement pstmt = conn.prepareStatement(sql);//5.预编译SQL
-     ResultSet rs = pstmt.executeQuery();               //6.执行SQL
-     process(rs);                                   //7.处理结果集
-     closeResultSet(rs);                             //8.释放结果集
-     tx.commit();                                  //7.提交事务
- } catch (Exception e) {  
-     tx.rollback();                                 //8.回滚事务
- throw e;  
- } finally {  
-    conn.close();                                //关闭连接
- }  
       此处可以看到使用UserTransaction而不是Connection连接进行控制事务，从而对于JDBC事务和JTA事务是采用不同API进行编程控制的，并且JTA和JDBC事务管理的异常也是不一样的。
       具体如何使用JTA编程进行事务管理请参考cn.javass.spring.chapter9包下的TranditionalTransactionTest类。
       而在Spring中将采用一致的事务抽象进行控制和一致的异常控制，即面向PlatformTransactionManager接口编程来控制事务。
###  Spring对编程式事务的支持
Spring中的事务分为物理事务和逻辑事务；
- 物理事务：就是底层数据库提供的事务支持，如JDBC或JTA提供的事务；
- 逻辑事务：是Spring管理的事务，不同于物理事务，逻辑事务提供更丰富的控制，而且如果想得到Spring事务管理的好处，必须使用逻辑事务，因此在Spring中如果没特别强调一般就是逻辑事务；
逻辑事务即支持非常低级别的控制，也有高级别解决方案：
- 低级别解决方案：
工具类：使用工具类获取连接（会话）和释放连接（会话），如使用org.springframework.jdbc.datasource包中的ConnectionUtils类来获取和释放具有逻辑事务功能的连接。当然对集成第三方ORM框架也提供了类似的工具类，如对Hibernate提供了SessionFactoryUtils工具类，JPA的EntityManagerFactoryUtils等，其他工具类都是使用类似***Utils命名；
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2506.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2506.html#)[打印](http://sishuok.com/forum/blogPost/list/2506.html#)
- //获取具有Spring事务（逻辑事务）管理功能的连接
- DataSourceUtils. getConnection(DataSource dataSource)  
- //释放具有Spring事务（逻辑事务）管理功能的连接
- DataSourceUtils. releaseConnection(Connection con, DataSource dataSource)  
- 
TransactionAwareDataSourceProxy：使用该数据源代理类包装需要Spring事务管理支持的数据源，该包装类必须位于最外层，主要用于遗留项目中可能直接使用数据源获取连接和释放连接支持或希望在Spring中进行混合使用各种持久化框架时使用，其内部实际使用ConnectionUtils工具类获取和释放真正连接；
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2506.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2506.html#)[打印](http://sishuok.com/forum/blogPost/list/2506.html#)
- <!--使用该方式包装数据源，必须在最外层，targetDataSource 知道目标数据源-->  
- <bean id="dataSourceProxy"
- class="org.springframework.jdbc.datasource.  
- TransactionAwareDataSourceProxy">  
-     <property name="targetDataSource" ref="dataSource"/>  
- </bean>  
通过如上方式包装数据源后，可以在项目中使用物理事务编码的方式来获得逻辑事务的支持，即支持直接从DataSource获取连接和释放连接，且这些连接自动支持Spring逻辑事务；
- 高级别解决方案：
模板类：使用Spring提供的模板类，如JdbcTemplate、HibernateTemplate和JpaTemplate模板类等，而这些模板类内部其实是使用了低级别解决方案中的工具类来管理连接或会话；
Spring提供两种编程式事务支持：直接使用PlatformTransactionManager实现和使用TransactionTemplate模板类，用于支持逻辑事务管理。
如果采用编程式事务推荐使用TransactionTemplate模板类和高级别解决方案。
### 使用PlatformTransactionManager
首先让我们看下如何使用PlatformTransactionManager实现来进行事务管理：
1、数据源定义，此处使用第7章的配置文件，即“chapter7/ applicationContext-resources.xml”文件。
2、事务管理器定义（chapter9/applicationContext-jdbc.xml）：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2506.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2506.html#)[打印](http://sishuok.com/forum/blogPost/list/2506.html#)
- <bean id="transactionManager"class="org.springframework.jdbc.datasource.DataSourceTransactionManager">    
-     <property name="dataSource" ref="dataSource"/>  
- </bean>  
3、 准备测试环境：
3.1、首先准备测试时使用的SQL：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2506.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2506.html#)[打印](http://sishuok.com/forum/blogPost/list/2506.html#)
- package cn.javass.spring.chapter9;  
- //省略import
- publicclass TransactionTest {  
- //id自增主键从0开始
- privatestaticfinal String CREATE_TABLE_SQL = "create table test" +  
- "(id int GENERATED BY DEFAULT AS IDENTITY PRIMARY KEY, " +  
- "name varchar(100))";  
- privatestaticfinal String DROP_TABLE_SQL = "drop table test";  
- privatestaticfinal String INSERT_SQL = "insert into test(name) values(?)";  
- privatestaticfinal String COUNT_SQL = "select count(*) from test";  
-     ……  
- }  
3.2、初始化Spring容器
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2506.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2506.html#)[打印](http://sishuok.com/forum/blogPost/list/2506.html#)
- package cn.javass.spring.chapter9;  
- //省略import
- publicclass TransactionTest {  
- privatestatic ApplicationContext ctx;  
- privatestatic PlatformTransactionManager txManager;  
- privatestatic DataSource dataSource;  
- privatestatic JdbcTemplate jdbcTemplate;  
-     ……  
- @BeforeClass
- publicstaticvoid setUpClass() {  
-         String[] configLocations = new String[] {  
- "classpath:chapter7/applicationContext-resources.xml",  
- "classpath:chapter9/applicationContext-jdbc.xml"};  
-         ctx = new ClassPathXmlApplicationContext(configLocations);  
-         txManager = ctx.getBean(PlatformTransactionManager.class);  
-         dataSource = ctx.getBean(DataSource.class);  
-         jdbcTemplate = new JdbcTemplate(dataSource);  
-     }   
-     ……  
- }  
3.3、使用高级别方案JdbcTemplate来进行事务管理器测试：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2506.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2506.html#)[打印](http://sishuok.com/forum/blogPost/list/2506.html#)
- @Test
- publicvoid testPlatformTransactionManager() {  
-     DefaultTransactionDefinition def = new DefaultTransactionDefinition();  
-     def.setIsolationLevel(TransactionDefinition.ISOLATION_READ_COMMITTED);  
-     def.setPropagationBehavior(TransactionDefinition.PROPAGATION_REQUIRED);  
-     TransactionStatus status = txManager.getTransaction(def);  
-     jdbcTemplate.execute(CREATE_TABLE_SQL);  
- try {  
-         jdbcTemplate.update(INSERT_SQL, "test");  
-         txManager.commit(status);  
-     } catch (RuntimeException e) {  
-         txManager.rollback(status);  
-     }  
-     jdbcTemplate.execute(DROP_TABLE_SQL);  
- }  
- DefaultTransactionDefinition：事务定义，定义如隔离级别、传播行为等，即在本示例中隔离级别为ISOLATION_READ_COMMITTED（提交读），传播行为为PROPAGATION_REQUIRED（必须有事务支持，即如果当前没有事务，就新建一个事务，如果已经存在一个事务中，就加入到这个事务中）。
- TransactionStatus：事务状态类，通过PlatformTransactionManager的getTransaction方法根据事务定义获取；获取事务状态后，Spring根据传播行为来决定如何开启事务；
- JdbcTemplate：通过JdbcTemplate对象执行相应的SQL操作，且自动享受到事务支持，注意事务是线程绑定的，因此事务管理器可以运行在多线程环境；
- txManager.commit(status)：提交status对象绑定的事务；
- txManager.rollback(status)：当遇到异常时回滚status对象绑定的事务。
3.4、使用低级别解决方案来进行事务管理器测试：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2506.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2506.html#)[打印](http://sishuok.com/forum/blogPost/list/2506.html#)
- @Test
- publicvoid testPlatformTransactionManagerForLowLevel1() {  
- DefaultTransactionDefinition def = new DefaultTransactionDefinition();      def.setIsolationLevel(TransactionDefinition.ISOLATION_READ_COMMITTED);      def.setPropagationBehavior(TransactionDefinition.PROPAGATION_REQUIRED);  
-   TransactionStatus status = txManager.getTransaction(def);  
-   Connection conn = DataSourceUtils.getConnection(dataSource);  
- try {  
-       conn.prepareStatement(CREATE_TABLE_SQL).execute();  
-       PreparedStatement pstmt = conn.prepareStatement(INSERT_SQL);  
-       pstmt.setString(1, "test");  
-       pstmt.execute();  
-       conn.prepareStatement(DROP_TABLE_SQL).execute();  
-       txManager.commit(status);  
-   } catch (Exception e) {  
-       status.setRollbackOnly();  
-       txManager.rollback(status);  
-   } finally {  
-       DataSourceUtils.releaseConnection(conn, dataSource);  
- }  
- }  
- 
低级别方案中使用DataSourceUtils获取和释放连接，使用txManager开管理事务，而且面向JDBC编程，比起模板类方式来繁琐和复杂的多，因此不推荐使用该方式。在此就不介绍数据源代理类使用了，需要请参考platformTransactionManagerForLowLevelTest2测试方法。
到此事务管理是不是还很繁琐？必须手工提交或回滚事务，有没有更好的解决方案呢？Spring提供了TransactionTemplate模板类来简化事务管理。
### 9.3.4  使用TransactionTemplate
TransactionTemplate模板类用于简化事务管理，事务管理由模板类定义，而具体操作需要通过TransactionCallback回调接口或TransactionCallbackWithoutResult回调接口指定，通过调用模板类的参数类型为TransactionCallback或TransactionCallbackWithoutResult的execute方法来自动享受事务管理。
TransactionTemplate模板类使用的回调接口：
- TransactionCallback：通过实现该接口的“T doInTransaction(TransactionStatus status) ”方法来定义需要事务管理的操作代码；
- TransactionCallbackWithoutResult：继承TransactionCallback接口，提供“void doInTransactionWithoutResult(TransactionStatus status)”便利接口用于方便那些不需要返回值的事务操作代码。
1、接下来演示一下TransactionTemplate模板类如何使用：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2506.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2506.html#)[打印](http://sishuok.com/forum/blogPost/list/2506.html#)
- @Test
- publicvoid testTransactionTemplate() {//位于TransactionTest类中
-   jdbcTemplate.execute(CREATE_TABLE_SQL);  
-   TransactionTemplate transactionTemplate = new TransactionTemplate(txManager);  
-   transactionTemplate.setIsolationLevel(TransactionDefinition.ISOLATION_READ_COMMITTED);  
-   transactionTemplate.execute(new TransactionCallbackWithoutResult() {  
- @Override
- protectedvoid doInTransactionWithoutResult(TransactionStatus status) {  
-          jdbcTemplate.update(INSERT_SQL, "test");  
-   }});  
-   jdbcTemplate.execute(DROP_TABLE_SQL);  
- }  
- TransactionTemplate ：通过new TransactionTemplate(txManager)创建事务模板类，其中构造器参数为PlatformTransactionManager实现，并通过其相应方法设置事务定义，如事务隔离级别、传播行为等，此处未指定传播行为，其默认为PROPAGATION_REQUIRED；
- TransactionCallbackWithoutResult：此处使用不带返回的回调实现，其doInTransactionWithoutResult方法实现中定义了需要事务管理的操作；
- transactionTemplate.execute()：通过该方法执行需要事务管理的回调。
这样是不是简单多了，没有事务管理代码，而是由模板类来完成事务管理。
注：对于抛出Exception类型的异常且需要回滚时，需要捕获异常并通过调用status对象的setRollbackOnly()方法告知事务管理器当前事务需要回滚，如下所示：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2506.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2506.html#)[打印](http://sishuok.com/forum/blogPost/list/2506.html#)
- try {  
- //业务操作
- } catch (Exception e) { //可使用具体业务异常代替
-     status.setRollbackOnly();  
- }  
2、前边已经演示了JDBC事务管理，接下来演示一下JTA分布式事务管理：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2506.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2506.html#)[打印](http://sishuok.com/forum/blogPost/list/2506.html#)
- @Test
- publicvoid testJtaTransactionTemplate() {  
-     String[] configLocations = new String[] {  
- "classpath:chapter9/applicationContext-jta-derby.xml"};  
-     ctx = new ClassPathXmlApplicationContext(configLocations);  
- final PlatformTransactionManager jtaTXManager = ctx.getBean(PlatformTransactionManager.class);  
- final DataSource derbyDataSource1 = ctx.getBean("dataSource1", DataSource.class);  
- final DataSource derbyDataSource2 = ctx.getBean("dataSource2", DataSource.class);  
- final JdbcTemplate jdbcTemplate1 = new JdbcTemplate(derbyDataSource1);  
- final JdbcTemplate jdbcTemplate2 = new JdbcTemplate(derbyDataSource2);  
-     TransactionTemplate transactionTemplate = new TransactionTemplate(jtaTXManager);   
-     transactionTemplate.setIsolationLevel(TransactionDefinition.ISOLATION_READ_COMMITTED);  
-     jdbcTemplate1.update(CREATE_TABLE_SQL);  
- int originalCount = jdbcTemplate1.queryForInt(COUNT_SQL);  
- try {  
-         transactionTemplate.execute(new TransactionCallbackWithoutResult() {  
- @Override
- protectedvoid doInTransactionWithoutResult(TransactionStatus status) {  
-                 jdbcTemplate1.update(INSERT_SQL, "test");  
- //因为数据库2没有创建数据库表因此会回滚事务
-               jdbcTemplate2.update(INSERT_SQL, "test");  
-           }});  
-     } catch (RuntimeException e) {  
- int count = jdbcTemplate1.queryForInt(COUNT_SQL);  
-         Assert.assertEquals(originalCount, count);  
-     }  
-     jdbcTemplate1.update(DROP_TABLE_SQL);  
- }  
- 配置文件：使用此前定义的chapter9/applicationContext-jta-derby.xml；
- jtaTXManager： JTA事务管理器；
- derbyDataSource1和derbyDataSource2：derby数据源1和derby数据源2；
- jdbcTemplate1和jdbcTemplate2：分别使用derbyDataSource1和derbyDataSource2构造的JDBC模板类；
- transactionTemplate：使用jtaTXManager事务管理器的事务管理模板类，其隔离级别为提交读，传播行为默认为PROPAGATION_REQUIRED（必须有事务支持，即如果当前没有事务，就新建一个事务，如果已经存在一个事务中，就加入到这个事务中）；
- jdbcTemplate1.update(CREATE_TABLE_SQL)：此处只有derbyDataSource1所代表的数据库创建了“test”表，而derbyDataSource2所代表的数据库没有此表；
- TransactionCallbackWithoutResult：在此接口实现中定义了需要事务支持的操作：
jdbcTemplate1.update(INSERT_SQL, "test")：表示向数据库1中的test表中插入数据；
jdbcTemplate2.update(INSERT_SQL, "test")：表示向数据库2中的test表中插入数据，但数据库2没有此表将抛出异常，且JTA分布式事务将回滚；
- Assert.assertEquals(originalCount, count)：用来验证事务是否回滚，验证结果返回为true，说明分布式事务回滚了。
到此我们已经会使用PlatformTransactionManager和TransactionTemplate进行简单事务处理了，那如何应用到实际项目中去呢？接下来让我们看下如何在实际项目中应用Spring管理事务。
接下来看一下如何将Spring管理事务应用到实际项目中，为简化演示，此处只定义最简单的模型对象和不完整的Dao层接口和Service层接口：
1、 首先定义项目中的模型对象，本示例使用用户模型和用户地址模型：
模型对象一般放在项目中的model包里。
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2506.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2506.html#)[打印](http://sishuok.com/forum/blogPost/list/2506.html#)
- package cn.javass.spring.chapter9.model;  
- publicclass UserModel {  
- privateint id;  
- private String name;  
- private AddressModel address;  
- //省略getter和setter
- }  
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2506.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2506.html#)[打印](http://sishuok.com/forum/blogPost/list/2506.html#)
- package cn.javass.spring.chapter9.model;  
- publicclass AddressModel {  
- privateint id;  
- private String province;  
- private String city;  
-     privateString street;  
- privateint userId;  
- //省略getter和setter
- }  
2.1、定义Dao层接口：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2506.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2506.html#)[打印](http://sishuok.com/forum/blogPost/list/2506.html#)
- package cn.javass.spring.chapter9.service;  
- import cn.javass.spring.chapter9.model.UserModel;  
- publicinterface IUserService {  
- publicvoid save(UserModel user);  
- publicint countAll();  
- }  
- 
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2506.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2506.html#)[打印](http://sishuok.com/forum/blogPost/list/2506.html#)
- package cn.javass.spring.chapter9.service;  
- import cn.javass.spring.chapter9.model.AddressModel;  
- publicinterface IAddressService {  
- publicvoid save(AddressModel address);  
- publicint countAll();  
- }  
2.2、定义Dao层实现：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2506.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2506.html#)[打印](http://sishuok.com/forum/blogPost/list/2506.html#)
- package cn.javass.spring.chapter9.dao.jdbc;  
- //省略import，注意model要引用chapter包里的
- publicclass UserJdbcDaoImpl extends NamedParameterJdbcDaoSupport implements IUserDao {  
- privatefinal String INSERT_SQL = "insert into user(name) values(:name)";  
- privatefinal String COUNT_ALL_SQL = "select count(*) from user";  
- @Override
- publicvoid save(UserModel user) {  
-         KeyHolder generatedKeyHolder = new GeneratedKeyHolder();  
-         SqlParameterSource paramSource = new BeanPropertySqlParameterSource(user);  
-         getNamedParameterJdbcTemplate().update(INSERT_SQL, paramSource, generatedKeyHolder);  
-         user.setId(generatedKeyHolder.getKey().intValue());  
-     }  
- @Override
- publicint countAll() {  
- return getJdbcTemplate().queryForInt(COUNT_ALL_SQL);  
-     }  
- }  
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2506.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2506.html#)[打印](http://sishuok.com/forum/blogPost/list/2506.html#)
- package cn.javass.spring.chapter9.dao.jdbc;  
- //省略import，注意model要引用chapter包里的
- publicclass AddressJdbcDaoImpl extends NamedParameterJdbcDaoSupport implements IAddressDao {  
- privatefinal String INSERT_SQL = "insert into address(province, city, street, user_id)" + "values(:province, :city, :street, :userId)";  
- privatefinal String COUNT_ALL_SQL = "select count(*) from address";  
- @Override
- publicvoid save(AddressModel address) {  
-         KeyHolder generatedKeyHolder = new GeneratedKeyHolder();  
-         SqlParameterSource paramSource = new BeanPropertySqlParameterSource(address);  
-         getNamedParameterJdbcTemplate().update(INSERT_SQL, paramSource, generatedKeyHolder);  
-         address.setId(generatedKeyHolder.getKey().intValue());  
- }  
- @Override
- publicint countAll() {  
- return getJdbcTemplate().queryForInt(COUNT_ALL_SQL);  
-     }  
- }  
3.1、定义Service层接口，一般使用“I×××Service”命名：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2506.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2506.html#)[打印](http://sishuok.com/forum/blogPost/list/2506.html#)
- package cn.javass.spring.chapter9.service;  
- import cn.javass.spring.chapter9.model.UserModel;  
- publicinterface IUserService {  
- publicvoid save(UserModel user);  
- publicint countAll();  
- }  
- 
- 
- package cn.javass.spring.chapter9.service;  
- import cn.javass.spring.chapter9.model.AddressModel;  
- publicinterface IAddressService {  
- publicvoid save(AddressModel address);  
- publicint countAll();  
- }  
3.2、定义Service层实现，一般使用“×××ServiceImpl”或“×××Service”命名：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2506.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2506.html#)[打印](http://sishuok.com/forum/blogPost/list/2506.html#)
- package cn.javass.spring.chapter9.service.impl;  
- //省略import，注意model要引用chapter包里的
- publicclass AddressServiceImpl implements IAddressService {  
- private IAddressDao addressDao;  
- private PlatformTransactionManager txManager;  
- publicvoid setAddressDao(IAddressDao addressDao) {  
- this.addressDao = addressDao;  
-     }  
- publicvoid setTxManager(PlatformTransactionManager txManager) {  
- this.txManager = txManager;  
-     }  
- @Override
- publicvoid save(final AddressModel address) {  
-         TransactionTemplate transactionTemplate = TransactionTemplateUtils.getDefaultTransactionTemplate(txManager);  
-         transactionTemplate.execute(new TransactionCallbackWithoutResult() {  
- @Override
- protectedvoid doInTransactionWithoutResult(TransactionStatus status) {  
-                 addressDao.save(address);  
-            }  
-         });  
-     }  
- @Override
- publicint countAll() {  
- return addressDao.countAll();  
-     }  
- }  
- 
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2506.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2506.html#)[打印](http://sishuok.com/forum/blogPost/list/2506.html#)
- 
- package cn.javass.spring.chapter9.service.impl;  
- //省略import，注意model要引用chapter包里的
- publicclass UserServiceImpl implements IUserService {  
- private IUserDao userDao;  
- private IAddressService addressService;  
- private PlatformTransactionManager txManager;  
- publicvoid setUserDao(IUserDao userDao) {  
- this.userDao = userDao;  
-     }  
- publicvoid setTxManager(PlatformTransactionManager txManager) {  
- this.txManager = txManager;  
-     }  
- publicvoid setAddressService(IAddressService addressService) {  
- this.addressService = addressService;  
-     }  
- @Override
- publicvoid save(final UserModel user) {  
-         TransactionTemplate transactionTemplate =  
-             TransactionTemplateUtils.getDefaultTransactionTemplate(txManager);  
-         transactionTemplate.execute(new TransactionCallbackWithoutResult() {  
- @Override
- protectedvoid doInTransactionWithoutResult(TransactionStatus status) {  
-                 userDao.save(user);  
-                 user.getAddress().setUserId(user.getId());  
-                 addressService.save(user.getAddress());  
-            }  
-         });  
-     }  
- @Override
- publicint countAll() {  
- return userDao.countAll();  
-     }  
- }  
- 
- 
Service实现中需要Spring事务管理的部分应该使用TransactionTemplate模板类来包装执行。
4、定义TransactionTemplateUtils，用于简化获取TransactionTemplate模板类，工具类一般放在util包中：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2506.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2506.html#)[打印](http://sishuok.com/forum/blogPost/list/2506.html#)
- package cn.javass.spring.chapter9.util;  
- //省略import
- publicclass TransactionTemplateUtils {  
- publicstatic TransactionTemplate getTransactionTemplate(  
-             PlatformTransactionManager txManager,  
- int propagationBehavior,  
- int isolationLevel) {  
- 
-         TransactionTemplate transactionTemplate = new TransactionTemplate(txManager);  
-         transactionTemplate.setPropagationBehavior(propagationBehavior);  
-         transactionTemplate.setIsolationLevel(isolationLevel);  
- return transactionTemplate;  
-     }  
- 
- publicstatic TransactionTemplate getDefaultTransactionTemplate(PlatformTransactionManager txManager) {  
- return getTransactionTemplate(  
-                 txManager,  
-                 TransactionDefinition.PROPAGATION_REQUIRED,  
-                 TransactionDefinition.ISOLATION_READ_COMMITTED);  
-     }  
- }  
- 
getDefaultTransactionTemplate用于获取传播行为为PROPAGATION_REQUIRED，隔离级别为ISOLATION_READ_COMMITTED的模板类。
5、数据源配置定义，此处使用第7章的配置文件，即“chapter7/ applicationContext-resources.xml”文件。
6、Dao层配置定义（chapter9/dao/applicationContext-jdbc.xml）：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2506.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2506.html#)[打印](http://sishuok.com/forum/blogPost/list/2506.html#)
- <bean id="txManager"class="org.springframework.jdbc.datasource.DataSourceTransactionManager">    
-     <property name="dataSource" ref="dataSource"/>  
- </bean>  
- <bean id="abstractDao"abstract="true">  
-     <property name="dataSource" ref="dataSource"/>  
- </bean>    
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2506.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2506.html#)[打印](http://sishuok.com/forum/blogPost/list/2506.html#)
- <bean id="userDao"class="cn.javass.spring.chapter9.dao.jdbc.UserJdbcDaoImpl" parent="abstractDao"/>  
- <bean id="addressDao"class="cn.javass.spring.chapter9.dao.jdbc.AddressJdbcDaoImpl" parent="abstractDao"/>  
7、Service层配置定义（chapter9/service/applicationContext-service.xml）：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2506.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2506.html#)[打印](http://sishuok.com/forum/blogPost/list/2506.html#)
- <bean id="userService"class="cn.javass.spring.chapter9.service.impl.UserServiceImpl">  
-     <property name="userDao" ref="userDao"/>  
-     <property name="txManager" ref="txManager"/>  
-     <property name="addressService" ref="addressService"/>  
- </bean>  
- <bean id="addressService"class="cn.javass.spring.chapter9.service.impl.AddressServiceImpl">  
-     <property name="addressDao" ref="addressDao"/>  
-     <property name="txManager" ref="txManager"/>  
- </bean>  
8、准备测试需要的表创建语句，在TransactionTest测试类中添加如下静态变量：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2506.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2506.html#)[打印](http://sishuok.com/forum/blogPost/list/2506.html#)
- privatestaticfinal String CREATE_USER_TABLE_SQL =  
- "create table user" +  
- "(id int GENERATED BY DEFAULT AS IDENTITY PRIMARY KEY, " +  
- "name varchar(100))";  
- privatestaticfinal String DROP_USER_TABLE_SQL = "drop table user";  
- 
- privatestaticfinal String CREATE_ADDRESS_TABLE_SQL =  
- "create table address" +  
- "(id int GENERATED BY DEFAULT AS IDENTITY PRIMARY KEY, " +  
- "province varchar(100), city varchar(100), street varchar(100), user_id int)";  
- privatestaticfinal String DROP_ADDRESS_TABLE_SQL = "drop table address";  
9、 测试一下吧：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/2506.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/2506.html#)[打印](http://sishuok.com/forum/blogPost/list/2506.html#)
- @Test
- publicvoid testServiceTransaction() {  
-         String[] configLocations = new String[] {  
- "classpath:chapter7/applicationContext-resources.xml",  
- "classpath:chapter9/dao/applicationContext-jdbc.xml",  
- "classpath:chapter9/service/applicationContext-service.xml"};  
-         ApplicationContext ctx2 = new ClassPathXmlApplicationContext(configLocations);  
- 
-         DataSource dataSource2 = ctx2.getBean(DataSource.class);  
-         JdbcTemplate jdbcTemplate2 = new JdbcTemplate(dataSource2);  
-         jdbcTemplate2.update(CREATE_USER_TABLE_SQL);  
-         jdbcTemplate2.update(CREATE_ADDRESS_TABLE_SQL);  
- 
-         IUserService userService = ctx2.getBean("userService", IUserService.class);  
-         IAddressService addressService = ctx2.getBean("addressService", IAddressService.class);  
-         UserModel user = createDefaultUserModel();  
-         userService.save(user);  
-         Assert.assertEquals(1, userService.countAll());  
-         Assert.assertEquals(1, addressService.countAll());  
-         jdbcTemplate2.update(DROP_USER_TABLE_SQL);  
-         jdbcTemplate2.update(DROP_ADDRESS_TABLE_SQL);  
- }  
- private UserModel createDefaultUserModel() {  
-     UserModel user = new UserModel();  
-     user.setName("test");  
-     AddressModel address = new AddressModel();  
-     address.setProvince("beijing");  
-     address.setCity("beijing");  
-     address.setStreet("haidian");  
-     user.setAddress(address);  
- return user;  
- }  
- 
从Spring容器中获取Service层对象，调用Service层对象持久化对象，大家有没有注意到Spring事务全部在Service层定义，为什么会在Service层定义，而不是Dao层定义呢？这是因为在服务层可能牵扯到业务逻辑，而每个业务逻辑可能调用多个Dao层方法，为保证这些操作的原子性，必须在Service层定义事务。
