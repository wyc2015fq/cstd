# spring 嵌套事务(Nested Transaction) 和新建事务测试 - z69183787的专栏 - CSDN博客
2017年07月27日 17:34:34[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2117
nested提交是依赖外层requires提交后才提交的。
requires_new 自己就能提交。
nested跟复杂的时候会更有用处
查询Nested Transaction细节时，经典例子 serviceA 调用 serviceB，当SeviceB事务传播属性为NESTED时，serviceA
只要catch住了Exception就能保证调用serviceB之前的操作提交而不受ServiceB异常的影响，但是如果
REQUIRES_NEW时，serviceA和serviceB相对独立，serviceB有异常会不影响serviceA.下面测试。
我用的是(maven dependencies里主要版本号)spring3.11 + jpa2.0 + hibernate4.13.后来运行的时候发现有异常：
JpaDialect does not support savepoints - check your JPA provider's capabilities
Java代码  ![收藏代码](http://ahujyy.iteye.com/images/icon_star.png)
- private SavepointManager getSavepointManager() {  
- if (!isSavepointAllowed()) {  
- thrownew NestedTransactionNotSupportedException(  
- "Transaction manager does not allow nested transactions");  
-     }  
-     SavepointManager savepointManager = getEntityManagerHolder().getSavepointManager();  
- if (savepointManager == null) {  
- thrownew NestedTransactionNotSupportedException(  
- "JpaDialect does not support savepoints - check your JPA provider's capabilities");  
-     }  
- return savepointManager;  
- }  
    jpa transaction manager 不支持 nested transaction，但JpaTransactionManager的属性nestedTransactionAllowed默认为true，如下。
Java代码  ![收藏代码](http://ahujyy.iteye.com/images/icon_star.png)
- public JpaTransactionManager() {  
-     setNestedTransactionAllowed(true);  
- }  
   后来才知道Hibernate也不支持Nested Transaction，测试不了，只能用jdbc事务了，使用JdbcTemplate。
换成jdbc后的主要配置：
Xml代码  ![收藏代码](http://ahujyy.iteye.com/images/icon_star.png)
- <context:component-scanbase-package="cn.xx.xx"/>
- <beanid="transactionManager"class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
- <propertyname="dataSource"ref="dataSource"></property>
- </bean>
- <tx:annotation-driventransaction-manager="transactionManager"/>
- <beanid="jdbcTemplate"class="org.springframework.jdbc.core.JdbcTemplate">
- <propertyname="dataSource"ref="dataSource"></property>
- </bean>
- <beanid="dataSource"class="org.apache.commons.dbcp.BasicDataSource"destroy-method="close">
- <propertyname="driverClassName"value="com.mysql.jdbc.Driver"/>
- <propertyname="url"value="jdbc:mysql://localhost:3306/hibernatetest"/>
- <propertyname="username"value="userName"/>
- <propertyname="password"value="password"/>
- </bean>
   test case 主要相关代码：
Java代码  ![收藏代码](http://ahujyy.iteye.com/images/icon_star.png)
- publicvoid  testInvoke(){  
-     InvokeService ins = (InvokeService) appContext.getBean("invokeServive");  
- //ins.invokeTest();
-     ins.jdbcInvokeTest();  
-     System.out.println("test invoke end");  
- }  
  invokeservice：
Java代码  ![收藏代码](http://ahujyy.iteye.com/images/icon_star.png)
- @Service("invokeServive")  
- publicclass InvokeService {  
- 
- private InvokeServiceA serviceA;  
- private InvokeServiceB serviceB;      
- 
- public InvokeServiceA getServiceA() {  
- return serviceA;  
-     }  
- @Autowired
- publicvoid setServiceA(InvokeServiceA serviceA) {  
- this.serviceA = serviceA;  
-     }  
- 
- public InvokeServiceB getServiceB() {  
- return serviceB;  
-     }  
- @Autowired
- publicvoid setServiceB(InvokeServiceB serviceB) {  
- this.serviceB = serviceB;  
-     }  
- 
- @Transactional(propagation=Propagation.REQUIRED)  
- publicvoid jdbcInvokeTest(){  
-         serviceB.jdbcValidInsert();  
- try{  
-             serviceA.jdbcExceptionInsert();  
-         }catch (Exception e) {  
-             System.out.println("excetpion hander");  
-         }  
-         System.out.println("jdbc invoke method end.");  
-     }  
- 
- }  
   serviceB.jdbcValidInsert():
Java代码  ![收藏代码](http://ahujyy.iteye.com/images/icon_star.png)
- //@Transactional(propagation=Propagation.REQUIRES_NEW)
- @Transactional(propagation=Propagation.REQUIRED)  
- //@Transactional(propagation=Propagation.NESTED)
- publicvoid jdbcValidInsert(){  
-     comDao.jdbcValidInsertTest();  
- }  
   serviceA.jdbcExceptionInsert():
Java代码  ![收藏代码](http://ahujyy.iteye.com/images/icon_star.png)
- //@Transactional(propagation=Propagation.REQUIRED)
- //@Transactional(propagation=Propagation.REQUIRES_NEW)
- @Transactional(propagation=Propagation.NESTED)  
- publicvoid jdbcExceptionInsert(){  
-     comDao.jdbcExceptionInsertTest();  
- }  
  comDao相关方法（ 表User（id,password,userName）,这里使之抛运行时异常，因为checked异常没有特殊
  指定rollback处理的话，是不会理会异常而导致数据都正常提交。）：
Java代码  ![收藏代码](http://ahujyy.iteye.com/images/icon_star.png)
- publicvoid jdbcExceptionInsertTest(){  
-     jdbcTemplate.execute("insert into User(password,userName) values ('pwd','excepNor')");  
-     jdbcTemplate.execute("insert into User(oopassword,userName) values ('pwd','exception')");  
- }  
- 
- publicvoid jdbcValidInsertTest(){  
-     jdbcTemplate.execute("insert into User(password,userName) values ('pwd','valid')");  
- }  
- 
运行结果：
    控制台
         excetpion hander
         jdbc invoke method end.
         test invoke end
    数据库，一条 (pwd，valid)的记录。
没问题，jdbcExceptionInsertTest() 执行第二条时异常，DataSourceTransactionManager使数据库回滚到第一条执行
以前的savepoint，向上throw exception，被catch住，打印出excetpion hander，接着执行打印jdbc invoke method
end. 这时候事务要提交了，因为 serviceB.jdbcValidInsert()是required，所以一起commit，事务结束。
测试二：
 将try catch去掉：
Java代码  ![收藏代码](http://ahujyy.iteye.com/images/icon_star.png)
- @Transactional(propagation=Propagation.REQUIRED)  
- publicvoid jdbcInvokeTest(){  
-     serviceB.jdbcValidInsert();  
- //try{
-         serviceA.jdbcExceptionInsert();  
- //}catch (Exception e) {
- //  System.out.println("excetpion hander");
- //}
-     System.out.println("jdbc invoke method end.");  
- }  
 运行结果：
     控制台：errors：org.springframework.jdbc.BadSqlGrammarException: StatementCallback; bad SQL grammar
     数据库：没有数据
 jdbcExceptionInsertTest() 执行第二条时异常，回滚后没有被hold住，导致“jdbc invoke method end”没来及执行直
接退出，事务管理器回滚jdbcValidInsertTest执行的结果，然后commit。
测试三，只将jdbcExceptionInsert改为requires new，注意 try catch还是注释掉的状态：
Java代码  ![收藏代码](http://ahujyy.iteye.com/images/icon_star.png)
- //@Transactional(propagation=Propagation.REQUIRED)
- @Transactional(propagation=Propagation.REQUIRES_NEW)  
- //@Transactional(propagation=Propagation.NESTED)
- publicvoid jdbcExceptionInsert(){  
-     comDao.jdbcExceptionInsertTest();  
- }  
 运行结果：
     控制台：errors：org.springframework.jdbc.BadSqlGrammarException: StatementCallback; bad SQL grammar
     数据库：没有数据
本来我原这么理解requires new，就是总是新启事务，很独立，跟其他人无关，也就是有异常，自己回滚自己的，不影响
他人，如jdbcExceptionInsert回滚自身做的所有操作，但是的的确确它影响到了和他平级的事务，数据库没有数据就是证
明，他在遇到异常后，回滚他的所有操作，commit，然后throw 异常，上层没hold住，jdbcInvokeTest结束执行，故没
有打印输出，事务管理器回滚serviceB.jdbcValidInsert()的操作，然后commit。这也就意味着在没有try catch时他的行
为和 nested时候是一样的。 那catch的时候会怎么样呢？
测试四，打开try catch：
Java代码  ![收藏代码](http://ahujyy.iteye.com/images/icon_star.png)
- @Transactional(propagation=Propagation.REQUIRED)  
- publicvoid jdbcInvokeTest(){  
-     serviceB.jdbcValidInsert();  
- try{  
-         serviceA.jdbcExceptionInsert();  
-     }catch (Exception e) {  
-         System.out.println("excetpion hander");  
-     }  
-     System.out.println("jdbc invoke method end.");  
- }  
 运行结果：
    控制台：
         excetpion hander
         jdbc invoke method end.
         test invoke end
    数据库：一条数据（pwd,valid）
证明和nested相对应的情况仍然一致，按我的理解 requires new 就是一个独立王国，如果有异常，即使上层不catch应该
也不影响上层的正常操作的提交，事实却不是，是不是感觉 那要requires new 有什么用呢，这种情况requires new 和
nested确实是可以互相替换。但是如果想把InvokeServiceB的jdbcValidInsert首先提交不受后续操作影响，REQUIRES
NEW就很管用，此时 即使后续操作异常不catch,前面操作因先commit了而不受影响，（pwd,valid）会正常插入。
测试五，将jdbcExceptionInsert()改成required。
Java代码  ![收藏代码](http://ahujyy.iteye.com/images/icon_star.png)
- @Transactional(propagation=Propagation.REQUIRED)  
- //@Transactional(propagation=Propagation.REQUIRES_NEW)
- //@Transactional(propagation=Propagation.NESTED)
- publicvoid jdbcExceptionInsert(){  
-     comDao.jdbcExceptionInsertTest();  
- }  
 运行结果：
    控制台：errors,org.springframework.transaction.UnexpectedRollbackException: Transaction rolled back
                because it has been marked as rollback-only
        excetpion hander
        jdbc invoke method end.
    数据库：没有数据。
 required说明jdbcExceptionInsert是上层的一部分，回滚的时候会回滚到serviceB.jdbcValidInsert()执行前的
 savepoint，抛出的异常被捕获，输出"excetpion hander"，jdbcInvokeTest()接着执行，输出"jdbc invoke
 method end." 事务commit，因为事务被底层标志rollback-only，应该是此时回滚(也许是异常发生时)并throw
 出异常，所以testInvoke()里的"test invoke end"没机会执行。这里transaction manager貌似个过滤器，最后
 根据是否是受检查异常去决定是否回滚，根据是否回滚决定是否再抛出异常。
由测试知道上述情况下nested和requires new可以互换，并且requires new 是又用的，那么又问题又回来了，nested
存在的意义呢，它能在什么情况下不可替代呢，按我现在理解是不是这个只有在某个极端情况下出现呢，虽然没做多少年
开发但是真的没遇到过这种情况，是不是hibernate 不支持也能说明此情况呢？
