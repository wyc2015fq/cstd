# 使用 @Lock 注解实现Spring JAP锁 - z69183787的专栏 - CSDN博客
2018年02月21日 19:29:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2246
[http://blog.csdn.net/terry_long/article/details/54291455](http://blog.csdn.net/terry_long/article/details/54291455)
JPA 2.0增加了6种新的锁模式，其中两个是乐观锁。JPA 2.0也允许悲观锁，并增加了3种悲观锁，第6种锁模式是无锁。 
下面是新增的两个乐观锁模式： 
1、OPTIMISTIC：它和READ锁模式相同，JPA 2.0仍然支持READ锁模式，但明确指出在新应用程序中推荐使用OPTIMISTIC。 
2、OPTIMISTIC_FORCE_INCREMENT：它和WRITE锁模式相同，JPA 2.0仍然支持WRITE锁模式，但明确指出在新应用程序中推荐使用OPTIMISTIC_FORCE_INCREMENT。 
下面是新增的三个悲观锁模式： 
1、PESSIMISTIC_READ：只要事务读实体，实体管理器就锁定实体，直到事务完成锁才会解开，当你想使用重复读语义查询数据时使用这种锁模式，换句话说就是，当你想确保数据在连续读期间不被修改，这种锁模式不会阻碍其它事务读取数据。 
2、PESSIMISTIC_WRITE：只要事务更新实体，实体管理器就会锁定实体，这种锁模式强制尝试修改实体数据的事务串行化，当多个并发更新事务出现更新失败几率较高时使用这种锁模式。 
3、PESSIMISTIC_FORCE_INCREMENT：当事务读实体时，实体管理器就锁定实体，当事务结束时会增加实体的版本属性，即使实体没有修改。 
最后一种是NONE,也就是无锁。
可以通过@Lock注解来进行锁模式的指定，我们来看一下@Lock 注解源码：
[java][view plain](http://blog.csdn.net/terry_long/article/details/54291455#)[copy](http://blog.csdn.net/terry_long/article/details/54291455#)
- @Target({ ElementType.METHOD, ElementType.ANNOTATION_TYPE })  
- @Retention(RetentionPolicy.RUNTIME)  
- @Documented
- public@interface Lock {  
- 
- /**
-      * The {@link LockModeType} to be used when executing the annotated query or CRUD method.
-      * 
-      * @return
-      */
-     LockModeType value();  
- }  
LockModeType就是指定所模式的枚举了，看一下它的 的源码：
[java][view plain](http://blog.csdn.net/terry_long/article/details/54291455#)[copy](http://blog.csdn.net/terry_long/article/details/54291455#)
- publicenum LockModeType {  
- /**
-      *  Synonymous with <code>OPTIMISTIC</code>.
-      *  <code>OPTIMISTIC</code> is to be preferred for new
-      *  applications.
-      *
-      */
-     READ,  
- 
- /**
-      *  Synonymous with <code>OPTIMISTIC_FORCE_INCREMENT</code>.
-      *  <code>OPTIMISTIC_FORCE_IMCREMENT</code> is to be preferred for new
-      *  applications.
-      *
-      */
-     WRITE,  
- 
- /**
-      * Optimistic lock.
-      *
-      * @since Java Persistence 2.0
-      */
-     OPTIMISTIC,  
- 
- /**
-      * Optimistic lock, with version update.
-      *
-      * @since Java Persistence 2.0
-      */
-     OPTIMISTIC_FORCE_INCREMENT,  
- 
- /**
-      *
-      * Pessimistic read lock.
-      *
-      * @since Java Persistence 2.0
-      */
-     PESSIMISTIC_READ,  
- 
- /**
-      * Pessimistic write lock.
-      *
-      * @since Java Persistence 2.0
-      */
-     PESSIMISTIC_WRITE,  
- 
- /**
-      * Pessimistic write lock, with version update.
-      *
-      * @since Java Persistence 2.0
-      */
-     PESSIMISTIC_FORCE_INCREMENT,  
- 
- /**
-      * No lock.
-      *
-      * @since Java Persistence 2.0
-      */
-     NONE  
- }  
来写一个简单的例子（不完全代码）：
@Lock注解需要添加在Repository 层，也就是Hibernate的DAO层，如下：
[java][view plain](http://blog.csdn.net/terry_long/article/details/54291455#)[copy](http://blog.csdn.net/terry_long/article/details/54291455#)
- publicinterface TestRepository extends
-         JpaRepository<TestObject, String>,  
-         JpaSpecificationExecutor<TestObject> {  
- 
- @Lock(value = LockModeType.PESSIMISTIC_READ)  
- @Query(value = "select o from TestObject where o.id= :id ")  
- public TestObject getByIdForUpdate(@Param("id") long id);  
- 
- }  
这里用到了@Query 注解，只需要定义接口就可以了，spring会根据注解里的HQL语句自动生成实现。
对Repository的调用一般放在service层，调用方法必须被事务包裹，不然启动会报错。
[java][view plain](http://blog.csdn.net/terry_long/article/details/54291455#)[copy](http://blog.csdn.net/terry_long/article/details/54291455#)
- publicinterface MyTestService{  
- /**
- * 定义接口
- */
- public TestObject updateObjStatus(long id,String status);  
- }  
- 
- publicclass MyTestServiceImpl{  
- @Autowired
- private TestRepository repos;  
- 
- /**
- * 实现一个带锁的更新方法
- */
- @Transactional
- @Override
- public TestObject updateObjStatus(long id,String status){  
- TestObject  obj = repos.getByIdForUpdate(id);  
- if(null == obj ){  
- thrownew RuntimeException("TestObject id = "+id+",not found！");  
- }  
- if(status.equals(obj.getStatus())){  
- thrownew RuntimeException("TestObject id = "+id+",status is alread be："+status+",update fail!");  
- }  
- obj.setStatus(status);  
- repos.save(obj);  
- 
- }  
- }  
在上面的例子中，我们尝试更新TestObject 对象的status，更新之前先将数据查询出来加上行级锁，确保某个线程在更新之时数据不会被其他线程修改掉，更新之前也进行判断，如果发现数据已被更新就跑出去异常结束更新。这样在多个线程同时对TestObject 对象进行更新时，最终只有一个线程会更新成功。
