# Hibernate应用 批量处理 - z69183787的专栏 - CSDN博客
2012年12月28日 10:19:14[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2806
一、批量插入
     在项目的开发过程之中，由于项目需求，我们常常需要把大批量的数据插入到数据库。数量级有万级、十万级、百万级、甚至千万级别的。如此数量级别的数据用Hibernate做插入操作，就可能会发生异常，常见的异常是OutOfMemoryError（内存溢出异常）。
    首先，我们简单来回顾一下Hibernate插入操作的机制。Hibernate要对它内部缓存进行维护，当我们执行插入操作时，就会把要操作的对象全部放到自身的内部缓存来进行管理。
    谈到Hibernate的缓存，Hibernate有内部缓存与二级缓存之说。由于Hibernate对这两种缓存有着不同的管理机制，对于二级缓存，我们可以对它的大小进行相关配置,而对于内部缓存，Hibernate就采取了“放任自流”的态度了，对它的容量并没有限制。现在症结找到了，我们做海量数据插入的时候，生成这么多的对象就会被纳入内部缓存（内部缓存是在内存中做缓存的），这样你的系统内存就会一点一点的被蚕食，如果最后系统被挤“炸”了，也就在情理之中了。
    我们想想如何较好的处理这个问题呢？有的开发条件又必须使用Hibernate来处理，当然有的项目比较灵活，可以去寻求其他的方法。
    笔者在这里推荐两种方法：
    (1)：优化Hibernate,程序上采用分段插入及时清除缓存的方法。
    (2)：绕过Hibernate API ，直接通过 JDBC API 来做批量插入，这个方法性能上是最好的，也是最快的。
    对于上述中的方法1，其基本是思路为：优化Hibernate，在配置文件中设置ibernate.jdbc.batch_size参数，来指定每次提交SQL的数量；程序上采用分段插入及时清除缓存的方法(Session实现了异步write-behind，它允许Hibernate显式地写操作的批处理),也就是每插入一定量的数据后及时的把它们从内部缓存中清除掉，释放占用的内存。
    配置hibernate.jdbc.batch_size参数的原因就是尽量少读数据库，hibernate.jdbc.batch_size参数值越大，读数据库的次数越少，速度越快。
    程序实现方面，笔者以插入10000条数据为例子，如：
    Session session=HibernateUtil.currentSession();
    Transatcion tx=session.beginTransaction();
    for(int i=0;i<10000;i++){
      Student st=new Student();
      st.setName(“feifei”);
      session.save(st);
      if(i%20==0){ //以每20个数据作为一个处理单元,与JDBC批量设置相同
        session.flush(); //保持与数据库数据的同步
        session.clear(); //清除内部缓存的全部数据，及时释放出占用的内存
      }
    }
    tx.commit();
    在一定的数据规模下，这种做法可以把系统内存资源维持在一个相对稳定的范围。
    注意：前面提到二级缓存，笔者在这里有必要再提一下。如果启用了二级缓存，从机制上讲Hibernate为了维护二级缓存，我们在做插入、更新、删除操作时，Hibernate都会往二级缓存充入相应的数据。性能上就会有很大损失，所以笔者建议在批处理情况下禁用二级缓存。
    对于方法2，采用传统的JDBC的批处理，使用JDBC API来处理。
    Connection conn=DB.getConnection();
    PreparedStatement stmt=conn.prepareStatement(“insert into T_STUDENT(name) values(?) ”);
    for(int j=0;j<200;j++){
      for(int i=0;i<50;i++){
        stmt.setString(1,”feifei”);
        stmt.addBatch();
      }
      stmt.executeBatch();
      conn.commit();
    }
    看看上面的代码，是不是总觉得有不妥的地方？对，没发现么！这还是JDBC的传统编程，没有一点
Hibernate味道。
    可以对以上的代码修改成下面这样：
    Transaction tx=session.beginTransaction(); //使用Hibernate事务处理边界Connection    
    conn=session.connection();
    PrepareStatement stmt=conn.prepareStatement(“insert into T_STUDENT(name) values(?)”);
    for(int j=0;j++;j<200){
      for(int i=0;i++;j<50){
        stmt.setString(1,”feifei”);
      }
    }
    stmt.executeUpdate();
    tx.commit(); //使用 Hibernate事务处理边界
     这样改动就很有Hibernate的味道了。笔者经过测试，采用JDBC API来做批量处理，性能上比使用
Hibernate API要高将近10倍，性能上JDBC 占优这是无疑的。
    二、批量更新与删除
    Hibernate2中，对于批量更新操作，Hibernate是将符合要求的数据查出来，然后再做更新操作。批量删除也是这样，先把符合条件的数据查出来，然后再做删除操作。
    这样有两个大缺点： 
    (1)：占用大量的内存。
    (2)：处理海量数据的时候，执行update/delete语句就是海量了，而且一条update/delete语句只能操作一个对象，这样频繁的操作数据库，性能低下应该是可想而知的了。
     Hibernate3 发布后，对批量更新/删除操作引入了bulk update/delete，其原理就是通过一条HQL语句完成批量更新/删除操作，很类似JDBC的批量更新/删除操作。在性能上，比Hibernate2的批量更新/删除有很大的提升。
    下面给出参考代码：把T_STUDENT表中数据都删除。
    Transaction tx=session.beginSession();
    String HQL=”delete STUDENT”;
    Query query=session.createQuery(HQL);
    int size=query.executeUpdate();
    tx.commit();
    控制台输出了也就一条删除语句Hibernate:delete from T_STUDENT,语句执行少了，性能上也与使用JDBC相差无几，是一个提升性能很好的方法。当然为了有更好的性能，笔者建议批量更新与删除操作还是使用JDBC，方法以及基本的知识点与上面的批量插入方法2基本相同，这里就不在冗述。
    Session session = sessionFactory.openSession();
    Transaction tx = session.beginTransaction();
    String hqlUpdate = "update Customer set name = :newName where name = :oldName";
    int updatedEntities = s.createQuery( hqlUpdate )
                           .setString( "newName", newName )
                           .setString( "oldName", oldName )
                           .executeUpdate();
    tx.commit();
    session.close();
    执行一个HQL DELETE，同样使用 Query.executeUpdate() 方法 （此方法是为 那些熟悉JDBC
PreparedStatement.executeUpdate() 的人们而设定的）
    Session session = sessionFactory.openSession();
    Transaction tx = session.beginTransaction();
    String hqlDelete = "delete Customer where name = :oldName";
    int deletedEntities = s.createQuery( hqlDelete )
                           .setString( "oldName", oldName )
                           .executeUpdate();
    tx.commit();
    session.close();
    笔者这里再提供一个方法，就是从数据库端来考虑提升性能，在Hibernate程序端调用存储过程。存储过程在数据库端运行，速度更快。以批量更新为例，给出参考代码。
    首先在数据库端建立名为batchUpdateStudent存储过程:
    create or replace produre batchUpdateStudent(a in number) as
    begin
    update STUDENT set AGE=AGE+1 where AGE>a;
    end;
    调用代码如下：
    Transaction tx=session.beginSession();
    Connection conn=session.connection();
    String pd=”{call batchUpdateStudent(?)}”;
    CallableStatement cstmt=conn.PrepareCall(pd);
    cstmt.setInt(1,20); //把年龄这个参数设为20
    tx.commit()；
    观察上面的代码，也是绕过Hibernate API,使用 JDBC API来调用存储过程，使用的还是Hibernate的事务边界。存储过程无疑是提高批量处理性能的一个好方法，直接运行与数据库端，某种程度上讲把批处理的压力转接给了数据库。
hibernateTemplate类中有一个saveOrUpdateAll(Collection c)的方法 ，应该也是可以进行批量处理数据的。
自己以前使用的方法：
```java
//批量增加
	public void addPatch(List<TBuildPipeLineHistArea> list){
		Session session=getHibernateTemplate().getSessionFactory().getCurrentSession();
		for(int i=0;i<list.size();i++)
		{
		    session.save(list.get(i));
		    // 以每50个数据作为一个处理单元
		    if(i%50==0)  
		    {
		        // 只是将Hibernate缓存中的数据提交到数据库，保持与数据库数据的同步
		        session.flush();  
		        // 清除内部缓存的全部数据，及时释放出占用的内存
		        session.clear();  
		    }
		}
	}
```
