# hibernate学习笔记 之 实体几种状态的区别 - z69183787的专栏 - CSDN博客
2012年12月19日 15:15:12[OkidoGreen](https://me.csdn.net/z69183787)阅读数：511
个人分类：[对象映射框架-Hibernate](https://blog.csdn.net/z69183787/article/category/2175489)
Hibernate 中Java对象的状态 
1.临时状态 (transient) 
特征： 
   1】不处于Session 缓存中 
   2】数据库中没有对象记录 
Java如何进入临时状态 
   1】通过new语句刚创建一个对象时 
   2】当调用Session 的delete()方法，从Session 缓存中删除一个对象时。  
2.持久化状态(persisted) 
特征： 
   1】处于Session 缓存中 
   2】持久化对象数据库中设有对象记录 
   3】Session 在特定时刻会保持二者同步 
Java如何进入持久化状态 
   1】Session 的save()把临时－》持久化状态 
   2】Session 的load(),get()方法返回的对象 
   3】Session 的find()返回的list集合中存放的对象 
   4】Session 的update(),saveOrupdate()使游离－》持久化 
3.游离状态(detached) 
特征： 
   1】不再位于Session 缓存中 
   2】游离对象由持久化状态转变而来，数据库中可能还有对应记录。 
Java如何进入持久化状态－》游离状态 
   1】Session 的close()方法 
   2】Session 的evict()方法，从缓存中删除一个对象。提高性能。少用。
