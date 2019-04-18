# Hibernate的clear(),flush(),evict()方法详解 - z69183787的专栏 - CSDN博客
2014年08月06日 16:44:39[OkidoGreen](https://me.csdn.net/z69183787)阅读数：21588
最近看了下session，发现session.flush（）方法有明显的误人子弟，明显乱说，也不对，讲的不清楚，对此我很气愤，你讲错了的让大家都跟着错了，还到处转载你的错误，你害羞么，我想问下作者。
特此给纠正下很多错误
session.flush（）方法的作用其实就是让session的缓存的数据（session就是一级缓存）刷入到数据库里面去，让数据库同步，你可以更简单的理解就是，强制让session的数据和数据库的数据同步，而不是什么清除缓存，我就奇怪了，清除缓存明明是session.clear()方法，在使用flush方法一般之前都是对一个对象进行CRUD的操作，然后你调用flush方法，就及时的同步到数据库里面去，其实session.flush()方法用的最好的一块是在处理大量数据的时候我们可以控制数量，比如，我们要存储1万个对象，我们可以这样做
if(i%20==0){
session.flush();//强制同步数据到数据库里面去
session.clear()；清除缓存
}
这样提高工作性能。
1.Clear 方法
      无论是Load 还是 Get 都会首先查找缓存（一级缓存） 如果没有，才会去数据库查找，调用Clear() 方法，可以强制清除Session缓存。
例：
**[c-sharp]**[view
 plain](http://blog.csdn.net/wkcgy/article/details/6192839#)[copy](http://blog.csdn.net/wkcgy/article/details/6192839#)[print](http://blog.csdn.net/wkcgy/article/details/6192839#)[?](http://blog.csdn.net/wkcgy/article/details/6192839#)
- publicvoid testClear(){  
-         Session session =  HibernateUitl.getSessionFactory().getCurrentSession();  
-         session.beginTransaction();  
-         Teacher t = (Teacher) session.get(Teacher.class, 3);  
-         System.out.println(t.getName());  
-         Teacher t2 = (Teacher) session.get(Teacher.class, 3);  
-         System.out.println(t2.getName());  
-         session.getTransaction().commit();  
-     }  
这里虽然用了2 个 get 方法（ get 方法会立即执行 sql 语句），但因为第一次执行了会缓存一个 ID 为 3 的实体，所以虽然有 2 个 get 方法只执行一次 SQL 语句。
**[c-sharp]**[view
 plain](http://blog.csdn.net/wkcgy/article/details/6192839#)[copy](http://blog.csdn.net/wkcgy/article/details/6192839#)[print](http://blog.csdn.net/wkcgy/article/details/6192839#)[?](http://blog.csdn.net/wkcgy/article/details/6192839#)
- publicvoid testClear(){  
-         Session session =  HibernateUitl.getSessionFactory().getCurrentSession();  
-         session.beginTransaction();  
-         Teacher t = (Teacher) session.get(Teacher.class, 3);  
-         System.out.println(t.getName());  
-         session.clear();//这里不clear只会执行一次sql语句，有clear会执行2次
-         Teacher t2 = (Teacher) session.get(Teacher.class, 3);  
-         System.out.println(t2.getName());  
-         session.getTransaction().commit();  
-     }  
这里在第2 次 get 前执行 session.clear(), 我们把 hibernate show_sql  出来，它就会执行 2 次 sql 语句了。所以session.clear() 会清除缓存。
2.Flush方法
      可以强制进行从内存到数据库的同步。
例：
**[c-sharp]**[view
 plain](http://blog.csdn.net/wkcgy/article/details/6192839#)[copy](http://blog.csdn.net/wkcgy/article/details/6192839#)[print](http://blog.csdn.net/wkcgy/article/details/6192839#)[?](http://blog.csdn.net/wkcgy/article/details/6192839#)
- @Test  
- /**
-      * flush 强制与数据库同步
-      */
- publicvoid testFlush(){  
-         Session session =  HibernateUitl.getSessionFactory().getCurrentSession();  
-         session.beginTransaction();  
-         Teacher t = (Teacher) session.get(Teacher.class, 3);  
-         t.setName("yyy");  
- 
-         t.setName("yyyyy");  
-         session.getTransaction().commit();  
-     }  
看这段代码，我们setName() 2 次， 但程序只会更改数据库一次，在 commit 时。
**[c-sharp]**[view
 plain](http://blog.csdn.net/wkcgy/article/details/6192839#)[copy](http://blog.csdn.net/wkcgy/article/details/6192839#)[print](http://blog.csdn.net/wkcgy/article/details/6192839#)[?](http://blog.csdn.net/wkcgy/article/details/6192839#)
- @Test  
- /**
-      * flush 强制与数据库同步
-      */
- publicvoid testFlush(){  
-         Session session =  HibernateUitl.getSessionFactory().getCurrentSession();  
-         session.beginTransaction();  
-         Teacher t = (Teacher) session.get(Teacher.class, 3);  
-         t.setName("yyy");  
-         session.flush();//有flush会执行2次UPDAE,没有会只执行一次
-         t.setName("yyyyy");  
-         session.getTransaction().commit();  
-     }  
我们在第2 次 setName （）时 执行 session.flush().
再看hibernate  执行的 sql  语句
**[c-sharp]**[view
 plain](http://blog.csdn.net/wkcgy/article/details/6192839#)[copy](http://blog.csdn.net/wkcgy/article/details/6192839#)[print](http://blog.csdn.net/wkcgy/article/details/6192839#)[?](http://blog.csdn.net/wkcgy/article/details/6192839#)
- Hibernate:   
-     update  
-         Teacher   
- set
-         birthday=?,  
-         name=?,  
-         title=?   
-     where  
-         id=?  
- Hibernate:   
-     update  
-         Teacher   
- set
-         birthday=?,  
-         name=?,  
-         title=?   
-     where  
-         id=?  
执行了2 次 Update
所以看出来flush 方法会强制与数据库同步。
Flush方法是可以设置的，也就是 fulsh 什么时候执行是可以设置的
 在session.beginTransaction 前设置 FlushMode
session.setFlushMode(FlushMode.Always|AUTO|COMMIT|NEVER|MANUAL)
FlushMode有 5 个值可选
Always:任何代码都会 Flush 
AUTO:默认方式 – 自动
Commit:COMMIT时
Never:始终不
MANUAL:手动方式
1、NEVEL：已经废弃了，被MANUAL取代了
2 MANUAL：
如果FlushMode是MANUAL或NEVEL,在操作过程中hibernate会将事务设置为readonly，所以在增加、删除或修改操作过程中会出现如下错误
org.springframework.dao.InvalidDataAccessApiUsageException: Write operations are not allowed in read-only mode (FlushMode.NEVER) - turn your Session into FlushMode.AUTO or remove 'readOnly' marker from transaction definition；
解决办法：配置事务，spring会读取事务中的各种配置来覆盖hibernate的session中的FlushMode；
3 AUTO
设置成auto之后，当程序进行查询、提交事务或者调用session.flush()的时候，都会使缓存和数据库进行同步，也就是刷新数据库
4 COMMIT
提交事务或者session.flush()时，刷新数据库；查询不刷新
5 ALWAYS：
每次进行查询、提交事务、session.flush()的时候都会刷数据库
ALWAYS和AUTO的区别：当hibernate缓存中的对象被改动之后，会被标记为脏数据（即与数据库不同步了）。当 session设置为FlushMode.AUTO时，hibernate在进行查询的时候会判断缓存中的数据是否为脏数据，是则刷数据库，不是则不刷，而always是直接刷新，不进行任何判断。很显然auto比always要高效得多。
总结：若OpenSessionInViewFilter在getSession的时候,会把获取回来的session的flush mode 设为FlushMode.NEVER。然后把该sessionFactory绑定到TransactionSynchronizationManager，使request的整个过程都使用同一个session，在请求过后再解除该sessionFactory的绑定，最后closeSessionIfNecessary根据该session是否已和transaction绑定来决定是否关闭session。在这个过程中，若HibernateTemplate
 发现自当前session有不是readOnly的transaction，就会获取到FlushMode.AUTO Session，使方法拥有写权限。
也即是，如果有不是readOnly的transaction就可以由Flush.NEVER转为Flush.AUTO,拥有insert,update,delete操作权限，如果没有transaction，并且没有另外人为地设flush model的话，则doFilter的整个过程都是Flush.NEVER。所以受transaction保护的方法有写权限，没受保护的则没有。
设置FlushMode  有个好处是可以节省开销，比如默认 session 只做查询时，就可以不让他与数据库同步了。
session.evict(obj)：会把指定的缓冲对象进行清除。
  session.clear()：把缓冲区内的全部对象清除，但不包括操作中的对象。
  Hibernate执行的顺序如下：
 (1)生成一个事务的对象，并标记当前的Session处于事务状态（注：此时并未启动数据库级事务）。
 (2)应用使用s.save保存对象，这个时候Session将这个对象放入entityEntries，用来标记对象已经和当前的会话建立了关联，由于应用对对象做了保存的操作，Session还要在insertions中登记应用的这个插入行为（行为包括：对象引用、对象id、Session、持久化处理类）。
 (3)s.evict将对象从s会话中拆离，这时s会从entityEntries中将这个对象移出。
 (4)事务提交，需要将所有缓存flush入数据库，Session启动一个事务，并按照insert,update,……,delete的顺序提交所有之前登记的操作（注意：所有insert执行完毕后才会执行update，这里的特殊处理也可能会将你的程序搞得一团糟，如需要控制操作的执行顺序，要善于使用flush），现在对象不在entityEntries中，但在执行insert的行为时只需要访问insertions就足够了，所以此时不会有任何的异常。异常出现在插入后通知Session该对象已经插入完毕这个步骤上，这个步骤中需要将entityEntries中对象的existsInDatabase标志置为true，由于对象并不存在于entityEntries中，此时Hibernate就认为insertions和entityEntries可能因为线程安全的问题产生了不同步（也不知道Hibernate的开发者是否考虑到例子中的处理方式，如果没有的话，这也许算是一个bug吧），于是一个net.sf.hibernate.AssertionFailure就被抛出，程序终止。
一般我们会错误的认为s.save会立即执行，而将对象过早的与Session拆离，造成了Session的insertions和entityEntries中内容的不同步。所以我们在做此类操作时一定要清楚Hibernate什么时候会将数据flush入数据库，在未flush之前不要将已进行操作的对象从Session上拆离。解决办法是在save之后，添加session.flush。
