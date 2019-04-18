# Hibernate缓存机制 - z69183787的专栏 - CSDN博客
2013年11月08日 09:07:55[OkidoGreen](https://me.csdn.net/z69183787)阅读数：707
个人分类：[对象映射框架-Hibernate](https://blog.csdn.net/z69183787/article/category/2175489)
缓存是位于应用程序与物理数据源之间，用于临时存放复制数据的内存区域，目的是为了减少应用程序对物理数据源访问的次数，从而提高应用程序的运行性能. 
  Hibernate在查询数据时，首先到缓存中去查找，如果找到就直接使用，找不到的时候就会从物理数据源中检索，所以，把频繁使用的数据加载到缓存区后，就可以大大减少应用程序对物理数据源的访问，使得程序的运行性能明显的提升.
Hibernate缓存分类：
Session缓存，一级缓存. 
SessionFactory的缓存分为内置缓存和外置缓存.内置缓存中存放的是SessionFactory对象的一些集合属性包含的数据(映射元素据及预定义SQL语句等),对于应用程序来说,它是只读的.外置缓存中存放的是数据库数据的副本,其作用和一级缓存类似.二级缓存除了以内存作为存储介质外,还可以选用硬盘等外部存储设备. 
Hibernate的缓存范围
Hibernate的一级缓存和二级缓存都位于均位于持久层,且均用于存放数据库数据的副本,最大的区别就是缓存的范围各不一样. 
缓存的范围分为3类: 
1.事务范围 
   事务范围的缓存只能被当前事务访问,每个事务都有各自的缓存,缓存内的数据通常采用相互关联的对象形式.缓存的生命周期依赖于事务的生命周期,只有当事务结束时,缓存的生命周期才会结束.事务范围的缓存使用内存作为存储介质,一级缓存就属于事务范围. 
2.应用范围 
   应用程序的缓存可以被应用范围内的所有事务共享访问.缓存的生命周期依赖于应用的生命周期,只有当应用结束时,缓存的生命周期才会结束.应用范围的缓存可以使用内存或硬盘作为存储介质,二级缓存就属于应用范围. 
3.集群范围 
   在集群环境中,缓存被一个机器或多个机器的进程共享,缓存中的数据被复制到集群环境中的每个进程节点,进程间通过远程通信来保证缓存中的数据的一致,缓存中的数据通常采用对象的松散数据形式. 
  Hibernate的缓存管理
一级缓存的管理:
evit(Object
 obj)  将指定的持久化对象从一级缓存中清除,释放对象所占用的内存资源,指定对象从持久化状态变为脱管状态,从而成为游离对象. 
clear() 
 将一级缓存中的所有持久化对象清除,释放其占用的内存资源 
contains(Object
 obj) 判断指定的对象是否存在于一级缓存中. 
flush() 刷新一级缓存区的内容,使之与数据库数据保持同步. 
二级缓存的管理:
evict(Class
 arg0, Serializable arg1)  将某个类的指定ID的持久化对象从二级缓存中清除,释放对象所占用的资源. 
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- sessionFactory.evict(Customer.class, new Integer(1));  
evict(Class
 arg0)  将指定类的所有持久化对象从二级缓存中清除,释放其占用的内存资源. 
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- sessionFactory.evict(Customer.class);  
evictCollection(String
 arg0)  将指定类的所有持久化对象的指定集合从二级缓存中清除,释放其占用的内存资源. 
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- sessionFactory.evictCollection("Customer.orders");  
Hibernate的二级缓存的配置
首先,不是所有的数据都适合放在二级缓存中,看一下,什么样的数据适合放在二级缓存中来?什么样的数据不适合放在二级缓存中来? 
  下面这几种情况就不适合加载到二级缓存中: 
  1.经常被修改的数据 
  2.绝对不允许出现并发访问的数据 
  3.与其他应用共享的数据 
  下面这己种情况合适加载到二级缓存中: 
  1.数据更新频率低 
  2.允许偶尔出现并发问题的非重要数据 
  3.不会被并发访问的数据 
  4.常量数据 
  5.不会被第三方修改的数据 
Hibernate的二级缓存功能是靠配置二级缓存插件来实现的,Hibernate为了集成这些插件,Hibernate提供了org.hibernate.cache.CacheProvider借口,它充当缓存插件与Hibernate之间的适配器 . 
常用的二级缓存插件 
EHCache  org.hibernate.cache.EhCacheProvider 
OSCache  org.hibernate.cache.OSCacheProvider 
SwarmCahe  org.hibernate.cache.SwarmCacheProvider 
JBossCache  org.hibernate.cache.TreeCacheProvider 
简单介绍一下EHCache的配置 
hibernate.cfg.xml 
Xml代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- <hibernate-configuration>
- <session-factory>
- <!-- 设置二级缓存插件EHCache的Provider类-->
- <propertyname="hibernate.cache.provider_class">
-          org.hibernate.cache.EhCacheProvider  
- </property>
- <!-- 启动"查询缓存" -->
- <propertyname="hibernate.cache.use_query_cache">
-          true  
- </property>
- </session-factory>
- </hibernate-configuration>
ehcache.xml 
Xml代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- <ehcache>
- <!-- maxElementsInMemory为缓存对象的最大数目, eternal设置是否永远不过期,timeToIdleSeconds对象处于空闲状态的最多秒数,timeToLiveSeconds对象处于缓存状态的最多秒数 -->
- <diskStorepath="java.io.tmpdir"/>
- <defaultCachemaxElementsInMemory="10000"eternal="false"timeToIdleSeconds="300"timeToLiveSeconds="600"overflowToDisk="true"/>
- </ehcache>
****.hbm.xml 
Xml代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- <?xmlversion="1.0"encoding='UTF-8'?>
- <!DOCTYPE hibernate-mapping PUBLIC  
-                             "-//Hibernate/Hibernate Mapping DTD 3.0//EN"  
-                             "http://hibernate.sourceforge.net/hibernate-mapping-3.0.dtd" >
- 
- <hibernate-mapping>
- 
- <class>
- <!-- 设置该持久化类的二级缓存并发访问策略 read-only read-write nonstrict-read-write transactional-->
- <cacheusage="read-write"/>
- </class>
- 
- </hibernate-mapping>
这是面试中经常问到的一个问题，楼主可以按照我的思路回答，准你回答得很完美，首先说下Hibernate缓存的作用（即为什么要用缓存机制），然后再具体说说Hibernate中缓存的分类情况，
最后可以举个具体的例子。
Hibernate缓存的作用：
    Hibernate是一个[持久层](http://zhidao.baidu.com/search?word=%E6%8C%81%E4%B9%85%E5%B1%82&fr=qb_search_exp&ie=utf8)框架，经常访问物理数据库，为了降低应用程序对物理[数据源](http://zhidao.baidu.com/search?word=%E6%95%B0%E6%8D%AE%E6%BA%90&fr=qb_search_exp&ie=utf8)访问的频次，从而提高应用程序的运行性能。缓存内的数据是对物理[数据源](http://zhidao.baidu.com/search?word=%E6%95%B0%E6%8D%AE%E6%BA%90&fr=qb_search_exp&ie=utf8)中的数据的复制，应用程序在运行时从缓存读写数据，在特定的时刻或事件会同步缓存和物理[数据源](http://zhidao.baidu.com/search?word=%E6%95%B0%E6%8D%AE%E6%BA%90&fr=qb_search_exp&ie=utf8)的数据
Hibernate缓存分类：
  Hibernate缓存包括两大类：Hibernate[一级缓存](http://zhidao.baidu.com/search?word=%E4%B8%80%E7%BA%A7%E7%BC%93%E5%AD%98&fr=qb_search_exp&ie=utf8)和Hibernate二级缓存
Hibernate[一级缓存](http://zhidao.baidu.com/search?word=%E4%B8%80%E7%BA%A7%E7%BC%93%E5%AD%98&fr=qb_search_exp&ie=utf8)又称为“Session的缓存”，它是内置的，不能被卸载（不能被卸载的意思就是这种缓存不具有可选性，必须有的功能，不可以取消session缓存）。由于[Session对象](http://zhidao.baidu.com/search?word=Session%E5%AF%B9%E8%B1%A1&fr=qb_search_exp&ie=utf8)的生命周期通常对应一个[数据库事务](http://zhidao.baidu.com/search?word=%E6%95%B0%E6%8D%AE%E5%BA%93%E4%BA%8B%E5%8A%A1&fr=qb_search_exp&ie=utf8)或者一个应用事务，因此它的缓存是事务范围的缓存。第[一级缓存](http://zhidao.baidu.com/search?word=%E4%B8%80%E7%BA%A7%E7%BC%93%E5%AD%98&fr=qb_search_exp&ie=utf8)是必需的，不允许而且事实上也无法卸除。在第一级缓存中，持久化类的每个实例都具有唯一的OID。 
Hibernate二级缓存又称为“SessionFactory的缓存”，由于SessionFactory对象的生命周期和应用程序的整个过程对应，因此Hibernate二级缓存是进程范围或者集群范围的缓存，有可能出现并发问题，因此需要采用适当的并发访问策略，该策略为被缓存的数据提供了[事务隔离级别](http://zhidao.baidu.com/search?word=%E4%BA%8B%E5%8A%A1%E9%9A%94%E7%A6%BB%E7%BA%A7%E5%88%AB&fr=qb_search_exp&ie=utf8)。第二级缓存是可选的，是一个可配置的插件，在默认情况下，SessionFactory不会启用这个插件。
什么样的数据适合存放到第二级缓存中？ 　　
1 很少被修改的数据 　　
2 不是很重要的数据，允许出现偶尔并发的数据 　　
3 不会被并发访问的数据 　　
4 [常量](http://zhidao.baidu.com/search?word=%E5%B8%B8%E9%87%8F&fr=qb_search_exp&ie=utf8)数据 　　
不适合存放到第二级缓存的数据？ 　　
1经常被修改的数据 　　
2 .绝对不允许出现并发访问的数据，如财务数据，绝对不允许出现并发 　　
3 与其他应用共享的数据。 
Hibernate查[找对象](http://zhidao.baidu.com/search?word=%E6%89%BE%E5%AF%B9%E8%B1%A1&fr=qb_search_exp&ie=utf8)如何应用缓存？
当Hibernate根据ID访问数据对象的时候，首先从Session一级缓存中查；查不到，如果配置了二级缓存，那么从二级缓存中查；如果都查不到，再查询数据库，把结果按照ID放入到缓存
删除、更新、增加数据的时候，同时更新缓存
Hibernate管理缓存实例
无论何时，我们在管理Hibernate缓存（Managing the caches）时，当你给save()、update()或saveOrUpdate()方法传递一个对象时，或使用load()、 get()、list()、iterate() 或scroll()方法获得一个对象时, 该对象都将被加入到Session的内部缓存中。 
当随后flush()方法被调用时，对象的状态会和数据库取得同步。 如果你不希望此同步操作发生，或者你正处理大量对象、需要对有效管理内存时，你可以调用evict() 方法，从一级缓存中去掉这些对象及其集合。 
    这样从头到尾一说的话，很全很详细，估计[面试官](http://zhidao.baidu.com/search?word=%E9%9D%A2%E8%AF%95%E5%AE%98&fr=qb_search_exp&ie=utf8)对你的印象很好。不过也不要[面面俱到](http://zhidao.baidu.com/search?word=%E9%9D%A2%E9%9D%A2%E4%BF%B1%E5%88%B0&fr=qb_search_exp&ie=utf8)，楼主可以挑一些自己懂得的内容说，不是很懂的内容可以省略，免得出漏子。
