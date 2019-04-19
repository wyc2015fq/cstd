# 解惑spring事务传播特性之嵌套事务 - 零度的博客专栏 - CSDN博客
2017年09月14日 10:35:47[零度anngle](https://me.csdn.net/zmx729618)阅读数：6111
       在所有使用 spring 的应用中, 声明式事务管理可能是使用率最高的功能了, 但是, 从我观察到的情况看, 绝大多数人并不能深刻理解事务声明中不同事务传播属性配置的的含义,
 让我们来看一下 TransactionDefinition 接口中的定义 。
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- /**
-      * Support a current transaction, create a new one if none exists.
-      * Analogous to EJB transaction attribute of the same name.
-      * <p>This is typically the default setting of a transaction definition.
-      */
- int PROPAGATION_REQUIRED = 0;  
- 
- /**
-      * Support a current transaction, execute non-transactionally if none exists.
-      * Analogous to EJB transaction attribute of the same name.
-      * <p>Note: For transaction managers with transaction synchronization,
-      * PROPAGATION_SUPPORTS is slightly different from no transaction at all,
-      * as it defines a transaction scopp that synchronization will apply for.
-      * As a consequence, the same resources (JDBC Connection, Hibernate Session, etc)
-      * will be shared for the entire specified scope. Note that this depends on
-      * the actual synchronization configuration of the transaction manager.
-      * @see org.springframework.transaction.support.AbstractPlatformTransactionManager#setTransactionSynchronization
-      */
- int PROPAGATION_SUPPORTS = 1;  
- 
- /**
-      * Support a current transaction, throw an exception if none exists.
-      * Analogous to EJB transaction attribute of the same name.
-      */
- int PROPAGATION_MANDATORY = 2;  
- 
- /**
-      * Create a new transaction, suspend the current transaction if one exists.
-      * Analogous to EJB transaction attribute of the same name.
-      * <p>Note: Actual transaction suspension will not work on out-of-the-box
-      * on all transaction managers. This in particular applies to JtaTransactionManager,
-      * which requires the <code>javax.transaction.TransactionManager</code> to be
-      * made available it to it (which is server-specific in standard J2EE).
-      * @see org.springframework.transaction.jta.JtaTransactionManager#setTransactionManager
-      */
- int PROPAGATION_REQUIRES_NEW = 3;  
- 
- /**
-      * Execute non-transactionally, suspend the current transaction if one exists.
-      * Analogous to EJB transaction attribute of the same name.
-      * <p>Note: Actual transaction suspension will not work on out-of-the-box
-      * on all transaction managers. This in particular applies to JtaTransactionManager,
-      * which requires the <code>javax.transaction.TransactionManager</code> to be
-      * made available it to it (which is server-specific in standard J2EE).
-      * @see org.springframework.transaction.jta.JtaTransactionManager#setTransactionManager
-      */
- int PROPAGATION_NOT_SUPPORTED = 4;  
- 
- /**
-      * Execute non-transactionally, throw an exception if a transaction exists.
-      * Analogous to EJB transaction attribute of the same name.
-      */
- int PROPAGATION_NEVER = 5;  
- 
- /**
-      * Execute within a nested transaction if a current transaction exists,
-      * behave like PROPAGATION_REQUIRED else. There is no analogous feature in EJB.
-      * <p>Note: Actual creation of a nested transaction will only work on specific
-      * transaction managers. Out of the box, this only applies to the JDBC
-      * DataSourceTransactionManager when working on a JDBC 3.0 driver.
-      * Some JTA providers might support nested transactions as well.
-      * @see org.springframework.jdbc.datasource.DataSourceTransactionManager
-      */
- int PROPAGATION_NESTED = 6;  
![](http://static.blog.csdn.net/images/save_snippets.png)
         我们可以看到, 在 spring 中一共定义了六种事务传播属性, 如果你觉得看起来不够直观, 那么我来转贴一个满大街都有的翻译，如下：
PROPAGATION_REQUIRED -- 支持当前事务，如果当前没有事务，就新建一个事务。这是最常见的选择。 
PROPAGATION_SUPPORTS -- 支持当前事务，如果当前没有事务，就以非事务方式执行。 
PROPAGATION_MANDATORY -- 支持当前事务，如果当前没有事务，就抛出异常。 
PROPAGATION_REQUIRES_NEW -- 新建事务，如果当前存在事务，把当前事务挂起。 
PROPAGATION_NOT_SUPPORTED -- 以非事务方式执行操作，如果当前存在事务，就把当前事务挂起。 
PROPAGATION_NEVER -- 以非事务方式执行，如果当前存在事务，则抛出异常。 
PROPAGATION_NESTED -- 如果当前存在事务，则在嵌套事务内执行。如果当前没有事务，则进行与PROPAGATION_REQUIRED类似的操作。 
前六个策略类似于EJB CMT，第七个（PROPAGATION_NESTED）是Spring所提供的一个特殊变量。 
它要求事务管理器或者使用JDBC 3.0 Savepoint API提供嵌套事务行为（如Spring的DataSourceTransactionManager） 
      在我所见过的误解中, 最常见的是下面这种：
假如有两个业务接口 ServiceA 和 ServiceB, 其中 ServiceA 中有一个方法实现如下 ：
/** 
* 事务属性配置为 PROPAGATION_REQUIRED 
*/ 
void methodA() { 
       // 调用 ServiceB 的方法 
      ServiceB.methodB(); 
} 
那么如果 ServiceB 的 methodB  如果配置了事务, 就必须配置为 PROPAGATION_NESTED 
      这种想法可能害了不少人, 认为 Service 之间应该避免互相调用, 其实根本不用担心这点，PROPAGATION_REQUIRED 已经说得很明白。如果当前线程中已经存在事务,
 方法调用会加入此事务, 如果当前没有事务，就新建一个事务, 所以 ServiceB#methodB() 的事务只要遵循最普通的规则配置为PROPAGATION_REQUIRED 即可, 如果 ServiceB#methodB (我们称之为内部事务, 为下文打下基础) 抛了异常, 那么 ServiceA#methodA(我们称之为外部事务) 如果没有特殊配置此异常时事务提交 (即 +MyCheckedException的用法), 那么整个事务是一定要 rollback 的, 什么 Service 只能调 Dao
 之类的言论纯属无稽之谈, spring 只负责配置了事务属性方法的拦截, 它怎么知道你这个方法是在 Service 还是 Dao 里 。
     说了这么半天, 那到底什么是真正的事务嵌套呢, 解释之前我们来看一下  Juergen Hoeller 的原话 。
Juergen Hoeller 写道：
PROPAGATION_REQUIRES_NEW starts a new, independent "inner" transaction for the given scope. This transaction will be committed or rolled back completely independent
 from the outer transaction, having its own isolation scope, its own set of locks, etc. The outer transaction will get suspended at the beginning of the inner one, and resumed once the inner one has completed. 
Such independent inner transactions are for example used for id generation through manual sequences, where the access to the sequence table should happen in its own transactions, to keep the lock there as short as possible. The goal there is to avoid tying
 the sequence locks to the (potentially much longer running) outer transaction, with the sequence lock not getting released before completion of the outer transaction. 
PROPAGATION_NESTED on the other hand starts a "nested" transaction, which is a true subtransaction of the existing one. What will happen is that a savepoint will be taken at the start of the nested transaction. íf the nested transaction fails, we will roll
 back to that savepoint. The nested transaction is part of of the outer transaction, so it will only be committed at the end of of the outer transaction. 
Nested transactions essentially allow to try some execution subpaths as subtransactions: rolling back to the state at the beginning of the failed subpath, continuing with another subpath or with the main execution path there - all within one isolated transaction,
 and not losing any previous work done within the outer transaction. 
For example, consider parsing a very large input file consisting of account transfer blocks: The entire file should essentially be parsed within one transaction, with one single commit at the end. But if a block fails, its transfers need to be rolled back,
 writing a failure marker somewhere. You could either start over the entire transaction every time a block fails, remembering which blocks to skip - or you mark each block as a nested transaction, only rolling back that specific set of operations, keeping the
 previous work of the outer transaction. The latter is of course much more efficient, in particular when a block at the end of the file fails. 
Juergen Hoeller 写道：
Rolling back the entire transaction is the choice of the demarcation code/config that started the outer transaction. 
So if an inner transaction throws an exception and is supposed to be rolled back (according to the rollback rules), the transaction will get rolled back to the savepoint taken at the start of the
 inner transaction. The immediate calling code can then decide to catch the exception and proceed down some other path within the outer transaction. 
If the code that called the inner transaction lets the exception propagate up the call chain, the exception will eventually reach the demarcation code of the outer transaction. At that point, the rollback rules
 of the outer transaction decide whether to trigger a rollback. That would be a rollback of the entire outer transaction then. 
So essentially, it depends on your exception handling. If you catch the exception thrown by the inner transaction, you can proceed down some other path within the outer transaction. If you let the exception propagate up the call chain, it's eventually gonna
 cause a rollback of the entire outer transaction. 
    也就是说, 最容易弄混淆的其实是 PROPAGATION_REQUIRES_NEW 和 PROPAGATION_NESTED, 那么这两种方式又有何区别呢? 我简单的翻译一下。         
Juergen Hoeller 的话:
PROPAGATION_REQUIRES_NEW 启动一个新的, 不依赖于环境的 "内部" 事务. 这个事务将被完全 commited 或 rolled back 而不依赖于外部事务, 它拥有自己的隔离范围, 自己的锁, 等等. 当内部事务开始执行时, 外部事务将被挂起, 内务事务结束时, 外部事务将继续执行。
另一方面, PROPAGATION_NESTED 开始一个 "嵌套的" 事务,  它是已经存在事务的一个真正的子事务. 潜套事务开始执行时,  它将取得一个 savepoint. 如果这个嵌套事务失败, 我们将回滚到此 savepoint. 潜套事务是外部事务的一部分, 只有外部事务结束后它才会被提交。
由此可见, PROPAGATION_REQUIRES_NEW 和 PROPAGATION_NESTED 的最大区别在于, PROPAGATION_REQUIRES_NEW 完全是一个新的事务, 而 PROPAGATION_NESTED 则是外部事务的子事务, 如果外部事务 commit, 潜套事务也会被
 commit, 这个规则同样适用于 roll back。
     那么，外部事务如何利用嵌套事务的 savepoint 特性呢, 我们用代码来说话。
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- ServiceA {  
- 
- /**
-      * 事务属性配置为 PROPAGATION_REQUIRED
-      */
- void methodA() {  
-         ServiceB.methodB();  
-     }  
- 
- }  
- 
- ServiceB {  
- 
- /**
-      * 事务属性配置为 PROPAGATION_REQUIRES_NEW
-      */
- void methodB() {  
-     }  
- 
- }     
![](http://static.blog.csdn.net/images/save_snippets.png)
这种情况下, 因为 ServiceB#methodB 的事务属性为 PROPAGATION_REQUIRES_NEW,
 所以两者不会发生任何关系, ServiceA#methodA 和 ServiceB#methodB 不会因为对方的执行情况而影响事务的结果, 因为它们根本就是两个事务, 在 ServiceB#methodB 执行时 ServiceA#methodA 的事务已经挂起了 (关于事务挂起的内容已经超出了本文的讨论范围, 有时间我会再写一些挂起的文章) . 
         那么，PROPAGATION_NESTED 又是怎么回事呢? 继续看代码：
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- ServiceA {  
- 
- /**
-      * 事务属性配置为 PROPAGATION_REQUIRED
-      */
- void methodA() {  
-         ServiceB.methodB();  
-     }  
- 
- }  
- 
- ServiceB {  
- 
- /**
-      * 事务属性配置为 PROPAGATION_NESTED
-      */
- void methodB() {  
-     }  
- 
- }     
![](http://static.blog.csdn.net/images/save_snippets.png)
       现在的情况就变得比较复杂了, ServiceB#methodB 的事务属性被配置为 PROPAGATION_NESTED, 此时两者之间又将如何协作呢? 从 Juergen Hoeller 的原话中我们可以找到答案,
 ServiceB#methodB 如果 rollback, 那么内部事务(即 ServiceB#methodB) 将回滚到它执行前的 SavePoint(注意, 这是本文中第一次提到它, 潜套事务中最核心的概念), 而外部事务(即 ServiceA#methodA) 可以有以下两种处理方式。
1. 改写 ServiceA 如下 
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- ServiceA {  
- 
- /**
-      * 事务属性配置为 PROPAGATION_REQUIRED
-      */
- void methodA() {  
- try {  
-             ServiceB.methodB();  
-         } catch (SomeException) {  
- // 执行其他业务, 如 ServiceC.methodC();
-         }  
-     }  
- 
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
       这种方式也是潜套事务最有价值的地方, 它起到了分支执行的效果, 如果 ServiceB.methodB 失败, 那么执行 ServiceC.methodC(), 而 ServiceB.methodB
 已经回滚到它执行之前的 SavePoint, 所以不会产生脏数据(相当于此方法从未执行过), 这种特性可以用在某些特殊的业务中, 而 PROPAGATION_REQUIRED 和 PROPAGATION_REQUIRES_NEW 都没有办法做到这一点. (题外话 : 看到这种代码, 似乎似曾相识, 想起了 prototype.js 中的 Try 函数 ) 
2. 代码不做任何修改, 那么如果内部事务(即 ServiceB#methodB) rollback, 那么首先 ServiceB.methodB 回滚到它执行之前的 SavePoint(在任何情况下都会如此), 
   外部事务(即 ServiceA#methodA) 将根据具体的配置决定自己是 commit 还是 rollback (+MyCheckedException)。
     上面大致讲述了潜套事务的使用场景, 下面我们来看如何在 spring 中使用 PROPAGATION_NESTED, 首先来看 AbstractPlatformTransactionManager
 。
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- /**
-  * Create a TransactionStatus for an existing transaction.
-  */
- private TransactionStatus handleExistingTransaction(  
-         TransactionDefinition definition, Object transaction, boolean debugEnabled)  
- throws TransactionException {  
- 
-    ... 省略  
- 
- if (definition.getPropagationBehavior() == TransactionDefinition.PROPAGATION_NESTED) {  
- if (!isNestedTransactionAllowed()) {  
- thrownew NestedTransactionNotSupportedException(  
- "Transaction manager does not allow nested transactions by default - " +  
- "specify 'nestedTransactionAllowed' property with value 'true'");  
-         }  
- if (debugEnabled) {  
-             logger.debug("Creating nested transaction with name [" + definition.getName() + "]");  
-         }  
- if (useSavepointForNestedTransaction()) {  
- // Create savepoint within existing Spring-managed transaction,
- // through the SavepointManager API implemented by TransactionStatus.
- // Usually uses JDBC 3.0 savepoints. Never activates Spring synchronization.
-             DefaultTransactionStatus status =  
-                     newTransactionStatus(definition, transaction, false, false, debugEnabled, null);  
-             status.createAndHoldSavepoint();  
- return status;  
-         }  
- else {  
- // Nested transaction through nested begin and commit/rollback calls.
- // Usually only for JTA: Spring synchronization might get activated here
- // in case of a pre-existing JTA transaction.
-             doBegin(transaction, definition);  
- boolean newSynchronization = (this.transactionSynchronization != SYNCHRONIZATION_NEVER);  
- return newTransactionStatus(definition, transaction, true, newSynchronization, debugEnabled, null);  
-         }  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
      根据上述源码，可以看出：我们要设置 transactionManager 的 nestedTransactionAllowed 属性为 true，注意：此属性默认为 false。
     再看 AbstractTransactionStatus#createAndHoldSavepoint() 方法：
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- /**
-  * Create a savepoint and hold it for the transaction.
-  * @throws org.springframework.transaction.NestedTransactionNotSupportedException
-  * if the underlying transaction does not support savepoints
-  */
- publicvoid createAndHoldSavepoint() throws TransactionException {  
-     setSavepoint(getSavepointManager().createSavepoint());  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
        可以看到 Savepoint 是 SavepointManager.createSavepoint 实现的, 再看 SavepointManager 的层次结构, 发现 其
 Template 实现是JdbcTransactionObjectSupport, 常用的 DatasourceTransactionManager, HibernateTransactionManager 中的 TransactonObject 都是它的子类 ， 
JdbcTransactionObjectSupport 告诉我们必须要满足两个条件才能 createSavepoint 。
      1. java.sql.Savepoint 必须存在, 即 jdk 版本要 1.4+ 
      2. Connection.getMetaData().supportsSavepoints() 必须为 true, 即 jdbc drive 必须支持 JDBC 3.0 
     确保以上条件都满足后, 你就可以尝试使用 PROPAGATION_NESTED 了。
