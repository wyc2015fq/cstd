# Hibernate 数据的批量插入、更新和删除 - 零度的博客专栏 - CSDN博客
2016年05月18日 11:37:50[零度anngle](https://me.csdn.net/zmx729618)阅读数：6657
Hibernate完全以面向对象的方式来操作数据库，当程序里以面向对象的方式操作持久化对象时，将被自动转换为对数据库的操作。例如调用Session的delete()方法来删除持久化对象，Hibernate将负责删除对应的数据记录；当执行持久化对象的set方法时，Hibernate将自动转换为对应的update方法，修改数据库的对应记录。
问题是如果需要同时更新100 000条记录，是不是要逐一加载100 000条记录，然后依次调用set方法——这样不仅繁琐，数据访问的性能也十分糟糕。对这种批量处理的场景，Hibernate提供了批量处理的解决方案，下面分别从批量插入、批量更新和批量删除3个方面介绍如何面对这种批量处理的情形。
### 1  批量插入
如果需要将100 000条记录插入数据库，通常Hibernate可能会采用如下做法：
Session session = sessionFactory.openSession();
Transaction tx = session.beginTransaction();
for ( int i=0; i<100000; i++ ) {
    User u = new User (.....);
    session.save(customer);
}
tx.commit();
session.close();
但随着这个程序的运行，总会在某个时候运行失败，并且抛出OutOfMemoryException（内存溢出异常）。这是因为Hibernate的Session持有一个必选的一级缓存，所有的User实例都将在Session级别的缓存区进行了缓存的缘故。
为了解决这个问题，有个非常简单的思路：定时将Session缓存的数据刷新入数据库，而不是一直在Session级别缓存。可以考虑设计一个累加器，每保存一个User实例，累加器增加1。根据累加器的值决定是否需要将Session缓存中的数据刷入数据库。
下面是增加100 000个User实例的代码片段：
private void testUser()throws Exception
{
    //打开Session
    Session session = HibernateUtil.currentSession();
    //开始事务
    Transaction tx = session.beginTransaction();
    //循环100 000次，插入100 000条记录
    for (int i = 0 ; i < 1000000 ; i++ )
    {
        //创建User实例
        User u1 = new User();
        u1.setName("xxxxx" + i);
        u1.setAge(i);
        u1.setNationality("china");
        //在Session级别缓存User实例
        session.save(u1);
        //每当累加器是20的倍数时，将Session中的数据刷入数据库，并清空Session缓存
        if (i % 20 == 0)
        {
            session.flush();
            session.clear();
            tx.commit();
            tx = session.beginTransaction();
        }
    }
    //提交事务
    tx.commit();
    //关闭事务
    HibernateUtil.closeSession();
}
上面代码中，当i%20 == 0时，手动将Session处的缓存数据写入数据库，并手动提交事务。如果不提交事务，数据将依然缓存在事务处——未进入数据库，也将引起内存溢出的异常。
这是对Session级别缓存的处理，还应该通过如下配置来关闭SessionFactory的二级      缓存。
hibernate.cache.use_second_level_cache false
注意：除了要手动清空Session级别的缓存外，最好关闭SessionFactory级别的二级缓存。否则，即使手动清空Session级别的缓存，但因为在SessionFactory级别还有缓存，也可能引发异常。
### 2  批量更新
上面介绍的方法同样适用于批量更新数据，如果需要返回多行数据，可以使用scroll()方法，从而可充分利用服务器端游标所带来的性能优势。下面是进行批量更新的代码片段：
private void testUser()throws Exception
{
    //打开Session
    Session session = HibernateUtil.currentSession();
    //开始事务
    Transaction tx = session.beginTransaction();
    //查询出User表中的所有记录
    ScrollableResults users = session.createQuery("from User")
        .setCacheMode(CacheMode.IGNORE)
        .scroll(ScrollMode.FORWARD_ONLY);
    int count=0;
    //遍历User表中的全部记录
    while ( users.next() )
    {
        User u = (User) users.get(0);
        u.setName("新用户名" + count);
        //当count为20的倍数时，将更新的结果从Session中flush到数据库
        if ( ++count % 20 == 0 )
        {
            session.flush();
            session.clear();
        }
    }
    tx.commit();
    HibernateUtil.closeSession();
}
通过这种方式，虽然可以执行批量更新，但效果非常不好。执行效率不高，而且需要先执行数据查询，然后再执行数据更新，并且这种更新将是逐行更新，即每更新一行记录，都需要执行一条update语句，性能非常低下。
为了避免这种情况，Hibernate提供了一种类似于SQL的批量更新和批量删除的HQL语法。
### 3  SQL风格的批量更新/删除
Hibernate提供的HQL语句也支持批量的UPDATE和DELETE语法。
批量UPDATE和DELETE语句的语法格式如下：
UPDATE | DELETE FROM? ClassName  [WHERE WHERE_CONDITIONS]
关于上面的语法格式有以下四点值得注意：
  ● 在FROM子句中，FROM关键字是可选的。即完全可以不写FROM关键字。
  ● 在FROM子句中只能有一个类名，该类名不能有别名。
  ● 不能在批量HQL语句中使用连接，显式的或隐式的都不行。但可以在WHERE子句中使用子查询。
  ● 整个WHERE子句是可选的。
假设，需要批量更改User类实例的name属性，可以采用如下代码片段完成：
private void testUser()throws Exception
{
    //打开Session
    Session session = HibernateUtil.currentSession();
    //开始事务
    Transaction tx = session.beginTransaction();
    //定义批量更新的HQL语句
    String hqlUpdate = "update User set name = :newName";
    //执行更新
    int updatedEntities = session.createQuery( hqlUpdate )
                           .setString( "newName", "新名字" )
                           .executeUpdate();
    //提交事务
    tx.commit();
    HibernateUtil.closeSession();
}
从上面代码中可以看出，这种语法非常类似于PreparedStatement的executeUpdate语法。实际上，HQL的这种批量更新就是直接借鉴了SQL语法的UPDATE语句。
注意：使用这种批量更新语法时，通常只需要执行一次SQL的UPDATE语句，就可以完成所有满足条件记录的更新。但也可能需要执行多条UPDATE语句，这是因为有继承映射等特殊情况，例如有一个Person实例，它有Customer的子类实例。当批量更新Person实例时，也需要更新Customer实例。如果采用joined-subclass或union-subclass映射策略，Person和Customer实例保存在不同的表中，因此可能需要多条UPDATE语句。
执行一个HQL DELETE，同样使用 Query.executeUpdate() 方法，下面是一次删除上面全部记录的代码片段：
private void testUser()throws Exception
{
    //打开Session实例
    Session session = HibernateUtil.currentSession();
    //开始事务
    Transaction tx = session.beginTransaction();
    //定义批量删除的HQL语句
    String hqlUpdate = "delete User";
    //执行批量删除
    int updatedEntities = session.createQuery( hqlUpdate )
                           .executeUpdate();
    //提交事务
    tx.commit();
    //关闭Session
    HibernateUtil.closeSession();
}
由Query.executeUpdate()方法返回一个整型值，该值是受此操作影响的记录数量。实际上，Hibernate的底层操作是通过JDBC完成的。因此，如果有批量的UPDATE或DELETE操作被转换成多条UPDATE或DELETE语句，该方法返回的是最后一条SQL语句影响的记录行数。
