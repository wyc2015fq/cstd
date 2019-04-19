# hibernate中的session.flush()和commit()的区别 - 零度的博客专栏 - CSDN博客
2016年05月19日 16:49:41[零度anngle](https://me.csdn.net/zmx729618)阅读数：11176标签：[hibernate](https://so.csdn.net/so/search/s.do?q=hibernate&t=blog)
个人分类：[JPA/Hibernate](https://blog.csdn.net/zmx729618/article/category/6206615)
       1、flush()方法进行清理缓存的操作,执行一系列的SQL语句,但不会提交事务;commit()方法会先调用flush()方法,然后提交事务. 提交事务意味着对数据库所做的更新会永久保持下来   所谓清理,是指Hibernate 按照持久化象的状态来同步更新数据库   
       2、Flush()后只是将Hibernate缓存中的数据提交到数据库,如果这时数据库处在一个事物当中,则数据库将这些SQL语句缓存起来,当Hibernate进行commit时,会告诉数据库,你可以真正提交了,这时数据才会永久保存下来,也就是被持久化了.    
       3、commit针对事物的，flush针对缓存的， 数据同步到数据库中后只要没有commit还是可以rollback的。
       可以这么理解，hibiernate有二级缓存，而平时一般只用一级缓存（默认开启），也就是session级的缓存。处于一个事务当中，当save的时候，只是把相应的insert行为登记在了以及缓存上，而flush是把缓存清空，同时把insert行为登记在数据库的事务上。当commit提交之后，才会执行相应的insert代码，而commit又是隐性的调用flush的，那在commit之前调用flush的作用的什么？我的理解是防止多条SQL语句冲突，这是因为flush到数据库中执行SQL语句的顺序不是按照你代码的先后顺序，而是按照insert，update....delete的顺序执行的，如果你不按照这个顺序在代码中编写，如果逻辑一旦出错就会抛exception了，解决这个的办法之一就是在可能其冲突的SQL操作后面flush一下，防止后面的语句其冲突
同时flush的作用，也有提交大量数据时候清理缓存的作用
分析下面一段代码：
**[java]**[view plain](http://blog.csdn.net/middlekingt/article/details/7830089#)[copy](http://blog.csdn.net/middlekingt/article/details/7830089#)[print](http://blog.csdn.net/middlekingt/article/details/7830089#)[?](http://blog.csdn.net/middlekingt/article/details/7830089#)
- publicvoid testSave1() {    
-   Session session = null;    
-   Transaction tx = null;    
- try {    
-    session = HibernateUtils.getSession();    
-    tx = session.beginTransaction();    
-    User1 user = new User1();    
-    user.setName("李四");    
-    user.setPassword("123");    
-    user.setCreateTime(new Date());    
-    user.setExpireTime(new Date());    
- 
- //因为user的主键生成策略采用的是uuid，所以调用完成save后，只是将user纳入到了session的管理  
- //不会发出insert语句，但是id已经生成，session中existsInDatebase状态为false  
-    session.save(user);    
- 
- //调用flush，hibernate会清理缓存，执行sql  
- //如果数据库的隔离级别设置为未提交读，那么我们可以看到flush过的数据  
- //并且session中existsInDatebase状态为true  </span>
-    session.flush();    
- 
- //提交事务  
- //默认情况下commit操作会先执行flush清理缓存，所以不用显示的调用flush  
- //commit后数据是无法回滚的  
-    tx.commit();    
-   }catch(Exception e) {    
-    e.printStackTrace();    
-    tx.rollback();    
-   }finally {    
-    HibernateUtils.closeSession(session);    
-   }    
-  }    
- 
- 
- publicvoid testSave2() {    
-   Session session = null;    
-   Transaction tx = null;    
- try {    
-    session = HibernateUtils.getSession();    
-    tx = session.beginTransaction();    
-    User2 user = new User2();    
-    user.setName("张三1");    
-    user.setPassword("123");    
-    user.setCreateTime(new Date());    
-    user.setExpireTime(new Date());    
- 
- //因为user的主键生成策略为native,所以调用session.save后，将执行insert语句，返回有数据库生成的id  
- //纳入了session的管理，修改了session中existsInDatebase状态为true  
- //如果数据库的隔离级别设置为未提交读，那么我们可以看到save过的数据  
-    session.save(user);    
- 
-    tx.commit();    
-   }catch(Exception e) {    
-    e.printStackTrace();    
-    tx.rollback();    
-   }finally {    
-    HibernateUtils.closeSession(session);    
-   }    
-  }  </span>  
```java
<span style="font-family:Arial;font-size:12px;">public void testSave1() {  
  Session session = null;  
  Transaction tx = null;  
  try {  
   session = HibernateUtils.getSession();  
   tx = session.beginTransaction();  
   User1 user = new User1();  
   user.setName("李四");  
   user.setPassword("123");  
   user.setCreateTime(new Date());  
   user.setExpireTime(new Date());  
     
   //因为user的主键生成策略采用的是uuid，所以调用完成save后，只是将user纳入到了session的管理  
   //不会发出insert语句，但是id已经生成，session中existsInDatebase状态为false  
   session.save(user);  
     
<span style="background-color: rgb(255, 255, 255);">   //调用flush，hibernate会清理缓存，执行sql  
   //如果数据库的隔离级别设置为未提交读，那么我们可以看到flush过的数据  
   //并且session中existsInDatebase状态为true  </span>
   session.flush();  
     
   //提交事务  
   //默认情况下commit操作会先执行flush清理缓存，所以不用显示的调用flush  
   //commit后数据是无法回滚的  
   tx.commit();  
  }catch(Exception e) {  
   e.printStackTrace();  
   tx.rollback();  
  }finally {  
   HibernateUtils.closeSession(session);  
  }  
 }  
   
   
 public void testSave2() {  
  Session session = null;  
  Transaction tx = null;  
  try {  
   session = HibernateUtils.getSession();  
   tx = session.beginTransaction();  
   User2 user = new User2();  
   user.setName("张三1");  
   user.setPassword("123");  
   user.setCreateTime(new Date());  
   user.setExpireTime(new Date());  
     
   //因为user的主键生成策略为native,所以调用session.save后，将执行insert语句，返回有数据库生成的id  
   //纳入了session的管理，修改了session中existsInDatebase状态为true  
   //如果数据库的隔离级别设置为未提交读，那么我们可以看到save过的数据  
   session.save(user);  
  
   tx.commit();  
  }catch(Exception e) {  
   e.printStackTrace();  
   tx.rollback();  
  }finally {  
   HibernateUtils.closeSession(session);  
  }  
 }  </span>
```
