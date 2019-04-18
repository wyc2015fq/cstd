# Hibernate乐观锁的实现原理剖析与使用乐观锁时的注意点 - z69183787的专栏 - CSDN博客
2014年04月02日 19:30:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1559
Hibernate支持乐观锁。当多个事务同时对数据库表中的同一条数据操作时，如果没有加锁机制的话，就会产生脏数据（duty data）。Hibernate有2种机制可以解决这个问题：乐观锁和悲观锁。这里我们只讨论乐观锁。
Hibernate乐观锁，能自动检测多个事务对同一条数据进行的操作，并根据先胜原则，提交第一个事务，其他的事务提交时则抛出org.hibernate.StaleObjectStateException异常。
Hibernate乐观锁是怎么做到的呢？
我们先从Hibernate乐观锁的实现说起。
要实现Hibenate乐观锁，我们首先要在数据库表里增加一个版本控制字段，字段名随意，比如就叫version，对应hibernate类型只能为long,integer,short,timestamp,calendar，也就是只能为数字或timestamp类型。然后在hibernatemapping里作如下类似定义：
    <versionname="version"
        column="VERSION"
        type="integer"
    />
告诉Hibernate version作为版本控制用，交由它管理。
当然在entity class里也需要给version加上定义，定义的方法跟其他字段完全一样。
private Integer version;
…
// setVersion() && getVersion(Integer)
Hibernate乐观锁的的使用：
Session session1 = sessionFactory.openSession();
Session session2 = sessionFactory.openSession();
MyEntity et1 = session1.load(MyEntity.class, id);
MyEntity et2 = session2.load(MyEntity.class, id);
//这里 et1, et2为同一条数据
Transaction tx1 = session1.beginTransaction();
//事务1开始
et1.setName(“Entity1”);
//事务1中对该数据修改
tx1.commit();
session1.close();
//事务1提交
Transaction tx2 = session2.beginTransaction();
//事务2开始
et2.setName(“Entity2”);
//事务2中对该数据修改
tx2.commit();
session2.close();
//事务2提交
在事务2提交时，因为它提交的数据比事务1提交后的数据旧，所以hibernate会抛出一个org.hibernate.StaleObjectStateException异常。
回到前面的问题，Hibernate怎么知道事务2提交的数据比事务1提交后的数据旧呢？
因为MyEntity有个version版本控制字段。
回头看看上面的源代码中的：
MyEntity et1 = session1.load(MyEntity.class, id);
MyEntity et2 = session2.load(MyEntity.class, id);
这里，et1.version==et2.version，比如此时version=1，
当事务1提交后，该数据的版本控制字段version=version+1=2，而事务2提交时version=1<2所以Hibernate认为事务2提交的数据为过时数据，抛出异常。
这就是Hibernate乐观锁的原理机制。
我 们已经知道了Hibernate乐观锁是根据version的值来判断数据是否过时，也就是说，在向数据库update某数据时，必须保证该entity 里的version字段被正确地设置为update之前的值，否则hibernate乐观锁机制将无法根据version作出正确的判断。
在我们的WEB应用中，尤其应该注意这个问题。
