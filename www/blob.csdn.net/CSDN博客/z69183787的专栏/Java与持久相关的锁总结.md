# Java与持久相关的锁总结 - z69183787的专栏 - CSDN博客
2018年02月21日 20:04:05[OkidoGreen](https://me.csdn.net/z69183787)阅读数：145
本文总结Java中有关数据库保存持久数据的锁机制，不只是纯粹数据库自身的锁，本文主要就Java持久层三个技术数据库、JPA和Hibernate的锁应用进行了总结。
　　在并发理论中，锁是用于保护可变的共享数据，以保证数据的完整性，大多数应用程序都是依赖于数据库本身提供的隐藏在数据库机制内的锁技术，很多初学者可能都没有意识到。
　　将整个系统的锁职责委托给数据库系统，虽然可以简化应用程序的开发，防止并发问题如死锁，但是还是有可能出现死锁的，但是数据库可以检测到并采取安全措施，比如任意释放两个竞争锁中一个。
### 物理锁
　　大多数的数据库系统使用的是共享读锁和排他写锁，这是因为其特定的锁元素(行 表)，而物理锁是由SQL标准确定的，其悲观的方式可能或妨碍系统的伸缩扩展性Scalable。
　　大多数数据库是使用轻量的锁技术，比如多版本并发控制，隐式的数据库锁机制的背后是事物隔离级别设置ACID，每个隔离级别是一个预定义的锁方案，目的是防止某一组数据的完整性异常。
　　READ COMMITTED(已提交读取)是使用查询级别的共享锁，对于当前事务中修改数据的操作采取排他锁，REPEATABLE (可重复读)和SERIALIZABLE (序列化)是在读取和写入排他锁需要时使用事务级别的共享锁。
### 逻辑锁
　　如果说数据库锁对于批处理系统已经足够，那么对于跨越多个数据库的多个Web请求来说，这种长对话事务需要一个逻辑(乐观)锁机制则是更合适些。
　　Hibernate提供的乐观锁(conversation-level repeatable read storage)可以确保数据的完整性，但是缺乏交易的可扩展性；JPA提供乐观锁和持久上下文可重复读两种机制，为实现逻辑锁提供了多样选择。
### 显式锁
　　前面提到由数据库和持久框架提供的隐式锁已经满足大多数应用程序的并发控制要求了，但是有时如果你需要一个更细粒度的锁策略，那么就需要使用显式锁了。
　　大多数数据库系统支持查询时间独占锁指令，如 [SELECT FOR UPDATE or SELECT FOR SHARE](http://www.postgresql.org/docs/9.4/static/sql-select.html#SQL-FOR-UPDATE-SHARE)，因此我们可以使用较低级别的默认隔离级别(READ
 COMMITTED)，而具体交易情况，使用共享和独占锁。
　　大部分乐观锁实现只是验证修改的数据，但是JPA允许你明确指定乐观锁的方式。
### JPA锁
　　作为数据库抽象层，JPA能够从底层RDBMS锁获得隐式的锁机制，JPA也提供优化可选自动的尸体版本控制机制。JPA提供显式锁为了如下操作：
- [finding](http://docs.oracle.com/javaee/7/api/javax/persistence/EntityManager.html#find%28java.lang.Class,%20java.lang.Object,%20javax.persistence.LockModeType%29) 发现一个实体
- [locking](http://docs.oracle.com/javaee/7/api/javax/persistence/EntityManager.html#lock%28java.lang.Object,%20javax.persistence.LockModeType%29) 锁定一个存在的持久化上下文实体
- [refreshing](http://docs.oracle.com/javaee/7/api/javax/persistence/EntityManager.html#refresh%28java.lang.Object,%20javax.persistence.LockModeType%29) 刷新一个实体
- [querying](http://docs.oracle.com/javaee/7/api/javax/persistence/Query.html#setLockMode%28javax.persistence.LockModeType%29) 通过JPQL查询, 或通过Criteria 或数据库SQL查询
### 显式锁类型
[LockModeType](http://docs.oracle.com/javaee/7/api/javax/persistence/LockModeType.html) 包含下面乐观和悲观锁模式：
|Lock Mode Type锁模型类型|描述|
|----|----|
|[NONE](http://docs.oracle.com/javaee/7/api/javax/persistence/LockModeType.html#NONE)|如果没有显式锁，应用将使用隐式锁(乐观或悲观)|
|[OPTIMISTIC](http://docs.oracle.com/javaee/7/api/javax/persistence/LockModeType.html#OPTIMISTIC)|总是在事务提交时进行版本检查，这样确保乐观锁可重复读的实现.|
|[READ](http://docs.oracle.com/javaee/7/api/javax/persistence/LockModeType.html#READ)|等同于[OPTIMISTIC](http://docs.oracle.com/javaee/7/api/javax/persistence/LockModeType.html#OPTIMISTIC).|
|[OPTIMISTIC_FORCE_INCREMENT](http://docs.oracle.com/javaee/7/api/javax/persistence/LockModeType.html#OPTIMISTIC_FORCE_INCREMENT)|总是累加实体版本(即使此时实体并没有改变) 并在事务提交时进行版本检查，这样确保乐观锁可重复读的实现|
|[WRITE](http://docs.oracle.com/javaee/7/api/javax/persistence/LockModeType.html#WRITE)|等同于[OPTIMISTIC_FORCE_INCREMENT](http://docs.oracle.com/javaee/7/api/javax/persistence/LockModeType.html#OPTIMISTIC_FORCE_INCREMENT).|
|[PESSIMISTIC_READ](http://docs.oracle.com/javaee/7/api/javax/persistence/LockModeType.html#PESSIMISTIC_READ)|将获得共享锁，以阻止其他事务获得[PESSIMISTIC_WRITE](http://docs.oracle.com/javaee/7/api/javax/persistence/LockModeType.html#PESSIMISTIC_WRITE)悲观锁.|
|[PESSIMISTIC_WRITE](http://docs.oracle.com/javaee/7/api/javax/persistence/LockModeType.html#PESSIMISTIC_WRITE)|获得排他锁，以阻止其他任何事务获得[PESSIMISTIC_READ](http://docs.oracle.com/javaee/7/api/javax/persistence/LockModeType.html#PESSIMISTIC_READ)或[PESSIMISTIC_WRITE](http://docs.oracle.com/javaee/7/api/javax/persistence/LockModeType.html#PESSIMISTIC_WRITE)锁.|
|[PESSIMISTIC_FORCE_INCREMENT](http://docs.oracle.com/javaee/7/api/javax/persistence/LockModeType.html#PESSIMISTIC_FORCE_INCREMENT)|获得数据库锁，阻止任何其他锁获得[PESSIMISTIC_READ](http://docs.oracle.com/javaee/7/api/javax/persistence/LockModeType.html#PESSIMISTIC_READ)或[PESSIMISTIC_WRITE](http://docs.oracle.com/javaee/7/api/javax/persistence/LockModeType.html#PESSIMISTIC_WRITE) 锁，实体版本在事务提交时累计。|
### 锁作用域和超时
　　 JPA 2.0通过下面值定义javax.persistence.lock.scope：
- [NORMAL](http://docs.oracle.com/javaee/7/api/javax/persistence/PessimisticLockScope.html#NORMAL)因为对象图会跨越多个数据表，一个显式锁也会涉及到多个表(例如 joined
 inheritance, secondary tables).因为整个实体相关联的行被锁住，*many-to-one*多对一 和 一对一*one-to-one*外键将锁定，但不会锁定对方父关联，这个作用域不会涉及到子集。
- [EXTENDED](http://docs.oracle.com/javaee/7/api/javax/persistence/PessimisticLockScope.html#EXTENDED)这个显式锁将涉及到元素集合和[junction
 tables](http://en.wikipedia.org/wiki/Junction_table), 但是不会实际锁定子实体，这个锁用于防止删除现有的子对象是有用的，同时允许幻读*phantom reads*或实际子实体状态的改变。
　　 JPA 2.0也引入了javax.persistence.lock.timeout属性，我们能够配置多少毫秒超时，这样一个请求在这个锁必须等待一直到过了超时，然后会抛出[PessimisticLockException](https://docs.oracle.com/javaee/7/api/javax/persistence/PessimisticLockException.html)。
### Hibernate锁
　　 Hibernate支持所有JPA锁模型，有一些附加的特定锁选项，显式锁能针对如下操作配置：
- [](https://docs.jboss.org/hibernate/orm/4.3/javadocs/org/hibernate/Session.html#buildLockRequest%28org.hibernate.LockOptions%29)通过各种[LockOptions](https://docs.jboss.org/hibernate/orm/4.3/javadocs/org/hibernate/LockOptions.html)选项锁住[locking](https://docs.jboss.org/hibernate/orm/4.3/javadocs/org/hibernate/Session.html#buildLockRequest%28org.hibernate.LockOptions%29)实体
- [getting](https://docs.jboss.org/hibernate/orm/4.3/javadocs/org/hibernate/Session.html#get%28java.lang.Class,%20java.io.Serializable,%20org.hibernate.LockOptions%29) 获得一个实体
- [loading](https://docs.jboss.org/hibernate/orm/4.3/javadocs/org/hibernate/Session.html#load%28java.lang.Class,%20java.io.Serializable,%20org.hibernate.LockOptions%29)加载一个实体
- [refreshing](https://docs.jboss.org/hibernate/orm/4.3/javadocs/org/hibernate/Session.html#refresh%28java.lang.Object,%20org.hibernate.LockOptions%29) 刷新一个实体
- 
创建一个实体或原生数据库查询 [Query](https://docs.jboss.org/hibernate/orm/4.3/javadocs/org/hibernate/Query.html#setLockOptions%28org.hibernate.LockOptions%29)
- 
创建一个creating a[Criteria](https://docs.jboss.org/hibernate/orm/4.3/javadocs/org/hibernate/Criteria.html#setLockMode%28org.hibernate.LockMode%29)query
[LockModeConverter](http://docs.jboss.org/hibernate/orm/4.3/javadocs/org/hibernate/internal/util/LockModeConverter.html)是负责JPA和Hibernate锁模型映射：
|Hibernate LockMode|JPA LockModeType|
|----|----|
|[NONE](https://docs.jboss.org/hibernate/orm/4.3/javadocs/org/hibernate/LockMode.html#NONE)|[NONE](http://docs.oracle.com/javaee/7/api/javax/persistence/LockModeType.html#NONE)|
|[OPTIMISTIC](https://docs.jboss.org/hibernate/orm/4.3/javadocs/org/hibernate/LockMode.html#OPTIMISTIC)[READ](https://docs.jboss.org/hibernate/orm/4.3/javadocs/org/hibernate/LockMode.html#READ)|[OPTIMISTIC](http://docs.oracle.com/javaee/7/api/javax/persistence/LockModeType.html#OPTIMISTIC)|
|[OPTIMISTIC_FORCE_INCREMENT](https://docs.jboss.org/hibernate/orm/4.3/javadocs/org/hibernate/LockMode.html#OPTIMISTIC_FORCE_INCREMENT)[WRITE](https://docs.jboss.org/hibernate/orm/4.3/javadocs/org/hibernate/LockMode.html#WRITE)|[OPTIMISTIC_FORCE_INCREMENT](http://docs.oracle.com/javaee/7/api/javax/persistence/LockModeType.html#OPTIMISTIC_FORCE_INCREMENT)|
|[PESSIMISTIC_READ](https://docs.jboss.org/hibernate/orm/4.3/javadocs/org/hibernate/LockMode.html#PESSIMISTIC_READ)|[PESSIMISTIC_READ](http://docs.oracle.com/javaee/7/api/javax/persistence/LockModeType.html#PESSIMISTIC_READ)|
|[PESSIMISTIC_WRITE](https://docs.jboss.org/hibernate/orm/4.3/javadocs/org/hibernate/LockMode.html#PESSIMISTIC_WRITE)[~~UPGRADE~~](https://docs.jboss.org/hibernate/orm/4.3/javadocs/org/hibernate/LockMode.html#UPGRADE)[UPGRADE_NOWAIT](https://docs.jboss.org/hibernate/orm/4.3/javadocs/org/hibernate/LockMode.html#UPGRADE_NOWAIT)[UPGRADE_SKIPLOCKED](https://docs.jboss.org/hibernate/orm/4.3/javadocs/org/hibernate/LockMode.html#UPGRADE_SKIPLOCKED)|[PESSIMISTIC_WRITE](http://docs.oracle.com/javaee/7/api/javax/persistence/LockModeType.html#PESSIMISTIC_WRITE)|
|[PESSIMISTIC_FORCE_INCREMENT](https://docs.jboss.org/hibernate/orm/4.3/javadocs/org/hibernate/LockMode.html#PESSIMISTIC_FORCE_INCREMENT)[~~FORCE~~](https://docs.jboss.org/hibernate/orm/4.3/javadocs/org/hibernate/LockMode.html#FORCE)|[PESSIMISTIC_FORCE_INCREMENT](http://docs.oracle.com/javaee/7/api/javax/persistence/LockModeType.html#PESSIMISTIC_FORCE_INCREMENT)|
[UPGRADE](https://docs.jboss.org/hibernate/orm/4.3/javadocs/org/hibernate/LockMode.html#UPGRADE) 和[FORCE](https://docs.jboss.org/hibernate/orm/4.3/javadocs/org/hibernate/LockMode.html#FORCE) 锁模型被[PESSIMISTIC_WRITE](https://docs.jboss.org/hibernate/orm/4.3/javadocs/org/hibernate/LockMode.html#PESSIMISTIC_WRITE)弃用.
[UPGRADE_NOWAIT](https://docs.jboss.org/hibernate/orm/4.3/javadocs/org/hibernate/LockMode.html#UPGRADE_NOWAIT)和[UPGRADE_SKIPLOCKED](https://docs.jboss.org/hibernate/orm/4.3/javadocs/org/hibernate/LockMode.html#UPGRADE_SKIPLOCKED) 使用Oracle-style[select
 for update nowait](http://docs.oracle.com/cd/B28359_01/server.111/b28286/statements_10002.htm#SQLRF55269)或[select
 for update skip locked](http://docs.oracle.com/cd/B28359_01/server.111/b28286/statements_10002.htm#SQLRF55270)语法
Hibernate 也定义了[scope and timeout locking
 options](https://docs.jboss.org/hibernate/orm/4.3/javadocs/org/hibernate/LockOptions.html):
- [scope](https://docs.jboss.org/hibernate/orm/4.3/javadocs/org/hibernate/LockOptions.html#setScope%28boolean%29)
锁作用域允许显式锁级联到[owned associations](http://www.javacodegeeks.com/2015/01/2014/11/01/hibernate-collections-optimistic-locking/).
- [timeout](https://docs.jboss.org/hibernate/orm/4.3/javadocs/org/hibernate/LockOptions.html#setTimeOut%28int%29)
超时阻止请求无限等待
