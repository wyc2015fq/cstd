# Hibernate Session的Flush模式 - 零度的博客专栏 - CSDN博客
2016年04月29日 10:52:50[零度anngle](https://me.csdn.net/zmx729618)阅读数：397标签：[session																[hibernate](https://so.csdn.net/so/search/s.do?q=hibernate&t=blog)](https://so.csdn.net/so/search/s.do?q=session&t=blog)
个人分类：[JPA/Hibernate](https://blog.csdn.net/zmx729618/article/category/6206615)
首先要知道的是：
**Hibernate会尽量将与数据库的操作延迟，直到必须要与数据库进行交互，例如save方法一般会在提交时才真正执行，最终在提交时会以批处理的方式与数据库进行交互，以提高效率。**
**而将操作延迟，就是利用缓存，将最后要处理的操作放到缓存中。**
flush方法的主要作用就是清理缓存，强制数据库与Hibernate缓存同步，以保证数据的一致性。它的主要动作就是向数据库发送一系列的sql语句，并执行这些sql语句，但是不会向数据库提交。而commit方法则会首先调用flush方法，然后提交事务。
      在下面的情况下，Hibernate会调用Session.flush()以清理缓存： 
     1)事务提交时，如果flush模式不为FlushMode.NEVER,commit()将调用flush(). 
     2)在某些查询语句之前（此查询语句之前的语句已经改变了数据库状态，所以需要调用flush（）以同步数据库是查出来的数据是经过更改的）。
     除非明确地指定了flush（）命令，否则关于Session何时会执行这些JDBC调用完全是无法保证的，只能保证他们执行的前后顺序。
  通过设置session.setFlushMode(),可以精确控制Hibernate的FlushMode. 
  (1)  FlushMode.AUTO:Hibernate判断对象属性有没有改变，如果被更改成为脏数据，则在一个查询语句前将更新此改动以保证数据库的同步。这也是Hibernate的默认清理模式。 
  (2) FlushMode.COMMIT:在事务结束之前清理session的缓存。这样有可能导致查出脏数据 
  (3) FlushMode.NEVER：除非强制调用Session.flush(),否则永远不清理Session。相当于将数据库设置为一个只读的数据库。 
       【如果此时进行数据的写入操作，会发生错误】
  (4) FlushMode.ALWAYS：在每一个查询数据之前都调用Session.flush()。很显然这种效率很低。 
 在调用Session.flush()时，涉及的SQL语句会按照下面的顺序执行。 
（1）         所有的实体进行插入的语句，其顺序按照对象执行Session.save()的时间顺序。 
（2）         所有对实体进行更新的语句 
（3）         所有对集合元素进行删除，更新或者插入的语句 
（4）         所有对实体经行删除的语句，其顺序按照对象执行Session.delete()的时间顺序。 
（5）         有一个例外是，如果对象使用native方式生成的ID（持久化标识），则他们一执行save就会被插入。 
                 【因为native方式若想得到主键，必须与数据库交互，而hilo方式则不用】）
