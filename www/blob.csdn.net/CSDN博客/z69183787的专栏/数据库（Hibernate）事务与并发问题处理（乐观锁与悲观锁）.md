# 数据库（Hibernate）事务与并发问题处理（乐观锁与悲观锁） - z69183787的专栏 - CSDN博客
2016年08月16日 14:20:37[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1488
目录
[一、数据库事务的定义](http://www.cnblogs.com/otomedaybreak/archive/2012/01/27/2330008.html#C1)
[二、数据库事务并发可能带来的问题](http://www.cnblogs.com/otomedaybreak/archive/2012/01/27/2330008.html#C2)
[三、数据库事务隔离级别](http://www.cnblogs.com/otomedaybreak/archive/2012/01/27/2330008.html#C3)
[四、使用Hibernate设置数据库隔离级别](http://www.cnblogs.com/otomedaybreak/archive/2012/01/27/2330008.html#C4)
[五、使用悲观锁解决事务并发问题](http://www.cnblogs.com/otomedaybreak/archive/2012/01/27/2330008.html#C5)
[六、使用乐观锁解决事务并发问题](http://www.cnblogs.com/otomedaybreak/archive/2012/01/27/2330008.html#C6)
Hibernate事务与并发问题处理（乐观锁与悲观锁）
一、数据库事务的定义
　　数据库事务(Database Transaction) ，是指作为单个逻辑工作单元执行的一系列操作。事务处理可以确保除非事务性单元内的所有操作都成功完成，否则不会永久更新面向数据的资源。通过将一组相关操作组合为一个要么全部成功要么全部失败的单元，可以简化错误恢复并使应用程序更加可靠。一个逻辑工作单元要成为事务，必须满足所谓的ACID(原子性、一致性、隔离性和持久性)属性。 
　　1. 原子性(atomic)，事务必须是原子工作单元；对于其数据修改，要么全都执行，要么全都不执行
　　2. 一致性(consistent)，事务在完成时，必须使所有的数据都保持一致状态。
　　3. 隔离性(insulation)，由并发事务所作的修改必须与任何其它并发事务所作的修改隔离。
　　4. 持久性(Duration)，事务完成之后，它对于系统的影响是永久性的。
二、数据库事务并发可能带来的问题 
　　如果没有锁定且多个用户同时访问一个数据库，则当他们的事务同时使用相同的数据时可能会发生问题。由于并发操作带来的数据不一致性包括：丢失数据修改、读”脏”数据（脏读）、不可重复读、产生幽灵数据：
假设数据库中有如下一张表：
![](http://pic002.cnblogs.com/images/2012/369936/2012012701033576.png)
　　1. 第一类丢失更新(lost update)： 在完全未隔离事务的情况下，两个事物更新同一条数据资源，某一事物异常终止，回滚造成第一个完成的更新也同时丢失。
![](http://pic002.cnblogs.com/images/2012/369936/2012012701063225.png)
　　在T1时刻开启了事务1，T2时刻开启了事务2，在T3时刻事务1从数据库中取出了id="402881e535194b8f0135194b91310001"的数据，T4时刻事务2取出了同一条数据，T5时刻事务1将age字段值更新为30，T6时刻事务2更新age为35并提交了数据，但是T7事务1回滚了事务age最后的值依然为20，事务2的更新丢失了，这种情况就叫做"第一类丢失更新(lost
 update)"。
　　2. 脏读(dirty read)：如果第二个事务查询到第一个事务还未提交的更新数据，形成脏读。
![](http://pic002.cnblogs.com/images/2012/369936/2012012701161952.png)
　　在T1时刻开启了事务1，T2时刻开启了事务2，在T3时刻事务1从数据库中取出了id="402881e535194b8f0135194b91310001"的数据，在T5时刻事务1将age的值更新为30，但是事务还未提交，T6时刻事务2读取同一条记录，获得age的值为30，但是事务1还未提交，若在T7时刻事务1回滚了事务2的数据就是错误的数据(脏数据)，这种情况叫做" 脏读(dirty
 read)"。
　　3. 虚读(phantom read)：一个事务执行两次查询，第二次结果集包含第一次中没有或者某些行已被删除，造成两次结果不一致，只是另一个事务在这两次查询中间插入或者删除了数据造成的。
![](http://pic002.cnblogs.com/images/2012/369936/2012012713552176.png)
　　在T1时刻开启了事务1，T2时刻开启了事务2，T3时刻事务1从数据库中查询所有记录，记录总共有一条，T4时刻事务2向数据库中插入一条记录，T6时刻事务2提交事务。T7事务1再次查询数据数据时，记录变成两条了。这种情况是"虚读(phantom
 read)"。
　　4. 不可重复读(unrepeated read)：一个事务两次读取同一行数据，结果得到不同状态结果，如中间正好另一个事务更新了该数据，两次结果相异，不可信任。
![](http://pic002.cnblogs.com/images/2012/369936/2012012714005481.png)
　　在T1时刻开启了事务1，T2时刻开启了事务2，在T3时刻事务1从数据库中取出了id="402881e535194b8f0135194b91310001"的数据，此时age=20，T4时刻事务2查询同一条数据，T5事务2更新数据age=30，T6时刻事务2提交事务，T7事务1查询同一条数据，发现数据与第一次不一致。这种情况就是"不可重复读(unrepeated
 read)"。
　　5. 第二类丢失更新(second lost updates)：是不可重复读的特殊情况，如果两个事务都读取同一行，然后两个都进行写操作，并提交，第一个事务所做的改变就会丢失。
![](http://pic002.cnblogs.com/images/2012/369936/2012012714053510.png)
　　在T1时刻开启了事务1，T2时刻开启了事务2，T3时刻事务1更新数据age=25，T5时刻事务2更新数据age=30，T6时刻提交事务，T7时刻事务2提交事务，把事务1的更新覆盖了。这种情况就是"第二类丢失更新(second
 lost updates)"。
三、数据库事务隔离级别
为了解决数据库事务并发运行时的各种问题数据库系统提供四种事务隔离级别：
1. Serializable 串行化
2. Repeatable Read 可重复读
3. Read Commited 可读已提交
4. Read Uncommited 可读未提交
隔离级别与并发性能的关系： 
![](http://pic002.cnblogs.com/images/2012/369936/2012012714122797.png)
每一个隔离级别可以解决的问题：
![](http://pic002.cnblogs.com/images/2012/369936/2012012714131839.png)
四、使用Hibernate设置数据库隔离级别
在Hibernate的配置文件中可以显示的配置数据库事务隔离级别。每一个隔离级别用一个整数表示：
8 - Serializable 串行化
4 - Repeatable Read 可重复读
2 - Read Commited 可读已提交
1 - Read Uncommited 可读未提交
在hibernate.cfg.xml中使用hibernate.connection.isolation参数配置数据库事务隔离级别。
五、使用悲观锁解决事务并发问题
　　悲观锁，正如其名，它指的是对数据被外界（包括本系统当前的其他事务，以及来自外部系统的事务处理）修改持保守态度，因此，在整个数据处理过程中，将数据处于锁定状态。悲观锁的实现，往往依靠数据库提供的锁机制（也只有数据库层提供的锁机制才能真正保证数据访问的排他性，否则，即使在本系统中实现了加锁机制，也无法保证外部系统不会修改数据）。
　　一个典型的依赖数据库的悲观锁调用：select * from account where name=”Erica” for update这条 sql 语句锁定了 account 表中所有符合检索条件（ name=”Erica” ）的记录。本次事务提交之前（事务提交时会释放事务过程中的锁），外界无法修改这些记录。悲观锁，也是基于数据库的锁机制实现。
在Hibernate使用悲观锁十分容易，但实际应用中悲观锁是很少被使用的，因为它大大限制了并发性：
![](http://pic002.cnblogs.com/images/2012/369936/2012012714511778.png)
图为Hibernate3.6的帮助文档Session文档的get方法截图，可以看到get方法第三个参数"lockMode"或"lockOptions"，注意在Hibernate3.6以上的版本中"LockMode"已经不建议使用。方法的第三个参数就是用来设置悲观锁的，使用第三个参数之后，我们每次发送的SQL语句都会加上"for update"用于告诉数据库锁定相关数据。
![](http://pic002.cnblogs.com/images/2012/369936/2012012714582356.png)
LockMode参数选择该选项，就会开启悲观锁。
![](http://pic002.cnblogs.com/images/2012/369936/2012012715012820.png)
　　T1,T2时刻取款事务和转账事务分别开启，T3事务查询ACCOUNTS表的数据并用悲观锁锁定，T4转账事务也要查询同一条数据，数据库发现该记录已经被前一个事务使用悲观锁锁定了，然后让转账事务等待直到取款事务提交。T6时刻取款事务提交，T7时刻转账事务获取数据。
六、使用乐观锁解决事务并发问题
　　相对悲观锁而言，乐观锁机制采取了更加宽松的加锁机制。悲观锁大多数情况下依靠数据库的锁机制实现，以保证操作最大程度的独占性。但随之而来的就是数据库性能的大量开销，特别是对长事务而言，这样的开销往往无法承受。乐观锁机制在一定程度上解决了这个问题。乐观锁，大多是基于数据版本（Version）记录机制实现。何谓数据版本？即为数据增加一个版本标识，在基于数据库表的版本解决方案中，一般是通过为数据库表增加一个"version"字段来实现。
　　乐观锁的工作原理：读取出数据时，将此版本号一同读出，之后更新时，对此版本号加一。此时，将提交数据的版本数据与数据库表对应记录的当前版本信息进行比对，如果提交的数据版本号大于数据库表当前版本号，则予以更新，否则认为是过期数据。
Hibernate为乐观锁提供了3中实现：
1. 基于version
2. 基于timestamp
3. 为遗留项目添加添加乐观锁 
配置基于version的乐观锁：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE hibernate-mapping PUBLIC "-//Hibernate/Hibernate Mapping DTD 3.0//EN" "http://hibernate.sourceforge.net/hibernate-mapping-3.0.dtd">
<hibernate-mapping>
<class name="com.suxiaolei.hibernate.pojos.People" table="people">
<id name="id" type="string">
<column name="id"></column>
<generator class="uuid"></generator>
</id>
<!-- version标签用于指定表示版本号的字段信息 -->
<version name="version" column="version" type="integer"></version>
<property name="name" column="name" type="string"></property>
</class>
</hibernate-mapping>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
配置基于timestamp的乐观锁：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE hibernate-mapping PUBLIC "-//Hibernate/Hibernate Mapping DTD 3.0//EN" "http://hibernate.sourceforge.net/hibernate-mapping-3.0.dtd">
<hibernate-mapping>
<class name="com.suxiaolei.hibernate.pojos.People" table="people">
<id name="id" type="string">
<column name="id"></column>
<generator class="uuid"></generator>
</id>
<!-- timestamp标签用于指定表示版本号的字段信息 -->
<timestamp name="updateDate" column="updateDate"></timestamp>
<property name="name" column="name" type="string"></property>
</class>
</hibernate-mapping>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
遗留项目，由于各种原因无法为原有的数据库添加"version"或"timestamp"字段，这时不可以使用上面两种方式配置乐观锁，Hibernate为这种情况提供了一个"optimisitic-lock"属性，它位于<class>标签上：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE hibernate-mapping PUBLIC "-//Hibernate/Hibernate Mapping DTD 3.0//EN" "http://hibernate.sourceforge.net/hibernate-mapping-3.0.dtd">
<hibernate-mapping>
<class name="com.suxiaolei.hibernate.pojos.People" table="people" optimistic-lock="all">
<id name="id" type="string">
<column name="id"></column>
<generator class="uuid"></generator>
</id>
<property name="name" column="name" type="string"></property>
</class>
</hibernate-mapping>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
将该属性的值设置为all，让该记录所有的字段都为版本控制信息。
