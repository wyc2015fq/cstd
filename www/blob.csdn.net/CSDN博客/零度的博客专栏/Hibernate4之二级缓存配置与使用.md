# Hibernate4之二级缓存配置与使用 - 零度的博客专栏 - CSDN博客
2017年07月25日 17:01:02[零度anngle](https://me.csdn.net/zmx729618)阅读数：1620
**缓存：缓存是什么，解决什么问题？**
位于速度相差较大的两种硬件/软件之间的，用于协调两者数据传输速度差异的结构，均可称之为缓存Cache。缓存目的：让数据更接近于应用程序，协调速度不匹配，使访问速度更快。 
**缓存的范围分为3类:**
1.事务范围（单Session即一级缓存）
   事务范围的缓存只能被当前事务访问,每个事务都有各自的缓存,缓存内的数据通常采用相互关联的对象形式.缓存的生命周期依赖于事务的生命周期,只有当事务结束时,缓存的生命周期才会结束.事务范围的缓存使用内存作为存储介质,一级缓存就属于事务范围. 
2.应用范围（单SessionFactory即二级缓存）
   应用程序的缓存可以被应用范围内的所有事务共享访问.缓存的生命周期依赖于应用的生命周期,只有当应用结束时,缓存的生命周期才会结束.应用范围的缓存可以使用内存或硬盘作为存储介质,二级缓存就属于应用范围. 
3.集群范围（多SessionFactory）
   在集群环境中,缓存被一个机器或多个机器的进程共享,缓存中的数据被复制到集群环境中的每个进程节点,进程间通过远程通信来保证缓存中的数据的一致,缓存中的数据通常采用对象的松散数据形式.
**二级缓存如何工作的**
Hibernate的二级缓存同一级缓存一样，也是针对对象ID来进行缓存。所以说，二级缓存的作用范围是针对根据ID获得对象的查询。 
● 在执行各种条件查询时，如果所获得的结果集为实体对象的集合，那么就会把所有的数据对象根据ID放入到二级缓存中。 
● 当Hibernate根据ID访问数据对象的时候，首先会从Session一级缓存中查找，如果查不到并且配置了二级缓存，那么会从二级缓存中查找，如果还查不到，就会查询数据库，把结果按照ID放入到缓存中。 
● 删除、更新、增加数据的时候，同时更新缓存。 
与Hibernate一级缓存Session范围相对的是SessionFactory范围的二级缓存，SessionFactory也提供了相应的缓存机制。SessionFactory缓存可以依据功能和目的的不同而划分为内置缓存和外置缓存。
SessionFactory的内置缓存中存放了映射元数据和预定义SQL语句，映射元数据是映射文件中数据的副本，而预定义SQL语句是在Hibernate初始化阶段根据映射元数据推导出来的。SessionFactory的内置缓存是只读的，应用程序不能修改缓存中的映射元数据和预定义SQL语句，因此SessionFactory不需要进行内置缓存与映射文件的同步。 
SessionFactory的外置缓存是一个可配置的插件。在默认情况下，SessionFactory不会启用这个插件。外置缓存的数据是数据库数据的副本，外置缓存的介质可以是内存或者硬盘。SessionFactory的外置缓存也被称为Hibernate的二级缓存。 
Hibernate的二级缓存的实现原理与一级缓存是一样的，也是通过以ID为key的Map来实现对对象的缓存。 
二级缓存是缓存实体对象的，由于Hibernate的二级缓存是作用在SessionFactory范围内的，因而它比一级缓存的范围更广，可以被所有的Session对象所共享。 
**在通常情况下会将具有以下特征的数据放入到二级缓存中：**
● 很少被修改的数据。 
● 不是很重要的数据，允许出现偶尔并发的数据。 
● 不会被并发访问的数据。 
● 常量数据。 
● 不会被第三方修改的数据 
**而对于具有以下特征的数据则不适合放在二级缓存中：**
● 经常被修改的数据。 
● 财务数据，绝对不允许出现并发。 
● 与其他应用共享的数据。 
在这里特别要注意的是对放入缓存中的数据不能有第三方的应用对数据进行更改（其中也包括在自己程序中使用其他方式进行数据的修改，例如，JDBC），因为那样Hibernate将不会知道数据已经被修改，也就无法保证缓存中的数据与数据库中数据的一致性。
**常见的缓存组件**
在默认情况下，Hibernate会使用EHCache作为二级缓存组件。但是，可以通过设置hibernate.cache.provider_class属性，指定其他的缓存策略，该缓存策略必须实现org.hibernate.cache.CacheProvider接口。 
通过实现org.hibernate.cache.CacheProvider接口可以提供对不同二级缓存组件的支持，此接口充当缓存插件与Hibernate之间的适配器。 
|组件|Provider类|类型|集群|查询缓存|
|----|----|----|----|----|
|Hashtable|org.hibernate.cache.HashtableCacheProvider|内存|不支持|支持|
|EHCache|org.hibernate.cache.EhCacheProvider|内存，硬盘|不支持|支持|
|OSCache|org.hibernate.cache.OSCacheProvider|内存，硬盘|支持|支持|
|SwarmCache|org.hibernate.cache.SwarmCacheProvider|集群|支持|不支持|
|JBoss TreeCache|org.hibernate.cache.TreeCacheProvider|集群|支持|支持|
Hibernate已经不再提供对JCS（Java Caching System）组件的支持了。 
**集群缓存的概念：**
当一台服务器上的执行了update方法修改了一条数据，那么只有这一台服务器上的二级缓存会同步于数据库，其他服务器上的二级缓存里面这条数据就没意义了。这个时候用OSCache缓存机制，只要有一台服务器上有数据修改了，马上会从配置文件中找到配置好的其他服务器IP地址，进行广播，告诉他们我这条数据修改了，你们也更新同步一下。（是不是有点像手机上微博的推送功能） 
**如何在程序里使用二级缓存：**
首先在hibernate.cfg.xml开启二级缓存 
Xml代码  ![收藏代码](http://zhou137520.iteye.com/images/icon_star.png)
- <hibernate-configuration>
- <session-factory>
- 
-       ......  
- 
- <!-- 开启二级缓存 -->
- <propertyname="hibernate.cache.use_second_level_cache">true</property>
- <!-- 启动"查询缓存"如果想缓存使用findall()、list()、Iterator()、createCriteria()、createQuery()等方法获得的数据结果集,必须配置此项-->
- <propertyname="hibernate.cache.use_query_cache">true</property>
- <!-- 设置二级缓存插件EHCache的Provider类-->
-       <!-- <propertyname="hibernate.cache.provider_class">
-          org.hibernate.cache.EhCacheProvider  
- </property> -->
- <!-- 二级缓存区域名的前缀 -->
- <!--<property name="hibernate.cache.region_prefix">test</property>-->
- <!-- 高速缓存提供程序 -->
- <propertyname="hibernate.cache.region.factory_class">
-          net.sf.ehcache.hibernate.EhCacheRegionFactory  
- </property>
- <!-- Hibernate4以后都封装到org.hibernate.cache.ehcache.EhCacheRegionFactory -->
- <!-- 指定缓存配置文件位置 -->
-       <!-- <propertyname="hibernate.cache.provider_configuration_file_resource_path">
-          ehcache.xml  
- </property> -->
- <!-- 强制Hibernate以更人性化的格式将数据存入二级缓存 -->
- <propertyname="hibernate.cache.use_structured_entries">true</property>
- 
- <!-- Hibernate将收集有助于性能调节的统计数据 -->
- <propertyname="hibernate.generate_statistics">true</property>
- 
-       ......  
- 
- </session-factory>
- </hibernate-configuration>
![](http://static.blog.csdn.net/images/save_snippets.png)
然后是ehcache配置（ehcache.xml） 
cache参数详解：
● name:指定区域名 
● maxElementsInMemory ：缓存在内存中的最大数目 
● maxElementsOnDisk：缓存在磁盘上的最大数目 
● eternal ：设置是否永远不过期 
● overflowToDisk ： 硬盘溢出数目 
● timeToIdleSeconds ：对象处于空闲状态的最多秒数后销毁 
● timeToLiveSeconds ：对象处于缓存状态的最多秒数后销毁 
● memoryStoreEvictionPolicy:缓存算法，有LRU（默认）、LFU、LFU 
关于缓存算法，常见有三种： 
● LRU：（Least Rencently Used）新来的对象替换掉使用时间算最近很少使用的对象 
● LFU：（Least Frequently Used）替换掉按命中率高低算比较低的对象 
● LFU：（First In First Out）把最早进入二级缓存的对象替换掉 
Xml代码  ![收藏代码](http://zhou137520.iteye.com/images/icon_star.png)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <ehcache>
- <!--如果缓存中的对象存储超过指定的缓存数量的对象存储的磁盘地址-->
- <diskStorepath="D:/ehcache"/>
- 
- <!-- 默认cache：如果没有对应的特定区域的缓存，就使用默认缓存 -->
- <defaultCachemaxElementsInMemory="10000"
- eternal="false"
- timeToIdleSeconds="300"
- timeToLiveSeconds="600"
- overflowToDisk="false"/>
- <!-- 指定区域cache：通过name指定，name对应到Hibernate中的区域名即可-->
- <cachename="cn.javass.h3test.model.UserModel"
- eternal="false"
- maxElementsInMemory="100"
- timeToIdleSeconds="1200"
- timeToLiveSeconds="1200"
- overflowToDisk="false">
- </cache>
- 
- </ehcache>
![](http://static.blog.csdn.net/images/save_snippets.png)
在每个实体的hbm文件中配置cache元素，usage可以是read-only或者是read-write等4种。
Xml代码  ![收藏代码](http://zhou137520.iteye.com/images/icon_star.png)
- <?xmlversion="1.0"encoding='UTF-8'?>
- <!DOCTYPE hibernate-mapping PUBLIC  
-                             "-//Hibernate/Hibernate Mapping DTD 3.0//EN"  
-                             "[http://hibernate.sourceforge.net/hibernate-mapping-3.0.dtd](http://hibernate.sourceforge.net/hibernate-mapping-3.0.dtd)" >
- <hibernate-mapping>
- <class>
- <!-- 设置该持久化类的二级缓存并发访问策略 read-only read-write nonstrict-read-write transactional-->
- <classname="cn.java.test.model.User"table="TBL_USER">
- <cacheusage="read-write"/>
-        ......    
- </class>
- </hibernate-mapping>
![](http://static.blog.csdn.net/images/save_snippets.png)
也可以用Hibernate注解配置缓存实体类
Java代码  ![收藏代码](http://zhou137520.iteye.com/images/icon_star.png)
- @Entity
- @Table
- @Cache(usage = CacheConcurrencyStrategy.READ_WRITE)  
- publicclass User implements Serializable {    
- privatestaticfinallong serialVersionUID = -5121812640999313420L;  
- private Integer id;  
- private String name;  
- 
-     ......  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
Query或Criteria接口查询时设置其setCacheable(true)： 
默认的如果不在程序中显示的执行查询缓存声明操作，Hibernate是不会对查询的list进行缓存的。
Java代码  ![收藏代码](http://zhou137520.iteye.com/images/icon_star.png)
- Session s1= HibernateSessionFactory.getCurrentSession();  
- s1.beginTransaction();  
- System.out.println("第一次查询User");  
- Query q = s1.createQuery("from User");  
- q.setCacheable(true);  
- q.list();  
- System.out.println("放进二级缓存");  
- s1.getTransaction().commit();  
- 
- Session s2= HibernateSessionFactory.getCurrentSession();  
- s2.beginTransaction();  
- System.out.println("第二次查询User,将不会发出sql");  
- Query q = s2.createQuery("from User");  
- q.setCacheable(true);  
- q.list();  
- s2.getTransaction().commit();  
- 
- //如果配置文件打开了generate_statistics性能调解，可以得到二级缓存命中次数等数据
- Statistics s = HibernateSessionFactoryUtil.getSessionFactory().getStatistics();  
- System.out.println(s);  
- System.out.println("put:"+s.getSecondLevelCachePutCount());  
- System.out.println("hit:"+s.getSecondLevelCacheHitCount());  
- System.out.println("miss:"+s.getSecondLevelCacheMissCount());  
![](http://static.blog.csdn.net/images/save_snippets.png)
如果开启了二级缓存，由于session是共享二级缓存的，只要缓存里面有要查询的对象，就不会向数据库发出sql，如果在二级缓存里没有找到需要的数据就会发出sql语句去数据库拿。 
一级缓存的管理: 
● evit(Object obj)将指定的持久化对象从一级缓存中清除,释放对象所占用的内存资源,指定对象从持久化状态变为脱管状态,从而成为游离对象. 
● clear()将一级缓存中的所有持久化对象清除,释放其占用的内存资源 
● contains(Object obj)判断指定的对象是否存在于一级缓存中. 
● flush()刷新一级缓存区的内容,使之与数据库数据保持同步. 
二级缓存的管理:
● evict(Class arg0, Serializable arg1)将某个类的指定ID的持久化对象从二级缓存中清除,释放对象所占用的资源. 
Java代码  ![收藏代码](http://zhou137520.iteye.com/images/icon_star.png)
- sessionFactory.evict(Customer.class, new Integer(1));  
![](http://static.blog.csdn.net/images/save_snippets.png)
evict(Class arg0)将指定类的所有持久化对象从二级缓存中清除,释放其占用的内存资源
Java代码  ![收藏代码](http://zhou137520.iteye.com/images/icon_star.png)
- sessionFactory.evict(Customer.class);  
![](http://static.blog.csdn.net/images/save_snippets.png)
evictCollection(String arg0)将指定类的所有持久化对象的指定集合从二级缓存中清除,释放其占用的内存资源.
Java代码  ![收藏代码](http://zhou137520.iteye.com/images/icon_star.png)
- sessionFactory.evictCollection("Customer.orders");  
![](http://static.blog.csdn.net/images/save_snippets.png)
**设置一级缓存和二级缓存的交互权限**
Java代码  ![收藏代码](http://zhou137520.iteye.com/images/icon_star.png)
- session = HibernateUtils.getSession();  
- session.beginTransaction();  
- 
- //仅向二级缓存读数据，而不向二级缓存写数据，这里load的数据就不会放入二级缓存，下次再查还是会去数据库拿
- session.setCacheMode(CacheMode.GET);  
- //只向二级缓存写数据，而不从二级缓存读数据
- //session.setCacheMode(CacheMode.PUT);
- //不与二级缓存交互
- //session.setCacheMode(CacheMode.IGNORE);
- //可以与二级缓存交互
- //session.setCacheMode(CacheMode.NORMAL);
- 
- Student student = (Student)session.load(Student.class, 1);  
- 
- session.getTransaction().commit();  
![](http://static.blog.csdn.net/images/save_snippets.png)
**设置二级缓存策略**
● READ_ONLY：实体只读缓存 
   只读缓存不允许更新，将报错Can't write to a readonly object。 
   允许新增，（从2.0以后新增直接添加到二级缓存）
Java代码  ![收藏代码](http://zhou137520.iteye.com/images/icon_star.png)
- //确保数据库中有标识符为1的FarmModel  
- FarmModel farm = (FarmModel) session.get(FarmModel.class, 1);    
- //如果修改将报错，只读缓存不允许修改  
- //farm.setName("aaa");
![](http://static.blog.csdn.net/images/save_snippets.png)
● NONSTRICT_READ_WRITE：实体非严格读/写缓存 
   允许更新，更新后缓存失效，需再查询一次。 
   允许新增，新增记录自动加到二级缓存中。 
   整个过程不加锁。 
● READ_WRITE：实体读/写缓存 
   允许更新，更新后自动同步到缓存。 
   允许新增，新增记录后自动同步到缓存。 
   保证read committed隔离级别及可重复读隔离级别（通过时间戳实现） 
   整个过程加锁，如果当前事务的时间戳早于二级缓存中的条目的时间戳，说明该条目已经被别的 
   事务修改了，此时重新查询一次数据库，否则才使用缓存数据，因此保证可重复读隔离级别。 
   读写缓存和不严格读写缓存在实现上的区别在于，读写缓存更新缓存的时候会把缓存里面的数据换成一个锁 
● TRANSACTIONAL：实体事务缓存 
   缓存支持事务，发生异常的时候，缓存也能够回滚，只支持jta环境 
● Collection集合缓存
Java代码  ![收藏代码](http://zhou137520.iteye.com/images/icon_star.png)
- <hibernate-mapping>  
-     <class name="cn.java.test.model.UserModel" table="TBL_USER">  
-         <cache usage="read-write" />  
-         <set name="farms" cascade="all" inverse="true" lazy="false">  
-             <cache usage="read-write"/>  
-             <key column="fk_user_id"/>  
-             <one-to-many class="cn.java.test.model.FarmModel"/>  
-         </set>  
-     </class>  
- </hibernate-mapping>  
![](http://static.blog.csdn.net/images/save_snippets.png)
和实体并发策略有相同含义； 
但集合缓存只缓存集合元素的标识符，在二级缓存中只存放相应实体的标识符，然后再通过标识符去二级缓存查找相应的实体最后组合为集合返回 
Collection的缓存和前面查询缓存的list一样，也是只保持一串id，但它不会因为这个表更新过就失效，一个collection缓存仅在这个collection里面的元素有增删时才失效。 
这样有一个问题，如果你的collection是根据某个字段排序的，当其中一个元素更新了该字段时，导致顺序改变时，collection缓存里面的顺序没有做更新 
**高速缓存区域**
Hibernate在不同的高速缓存区域保存不同的类（实体）/集合，如果不配置区域默认都保存到“默认缓存”（defaultCache）中。 
●每一个区域可以设置过期策略、缓存条目大小等等。 
●对于类缓存，默认区域名是全限定类名，如cn.javass.h3test.model.UserModel。 
●对于集合而言，默认区域名是全限定类名+属性名，如cn.javass.….UserModel.farms。 
●可通过hibernate.cache.region_prefix指定特定SessionFactory的区域前缀，如前缀是h3test，则如类缓存的区域名就是h3test.cn.javass.h3test.model.UserModel。如果应用程序使用多个SessionFactory这可能是必须的。 
可通过<cache usage="read-write" region="区域名"/>自定义区域名，不过默认其实就可以了。 
**一些对二级缓存的理解**
当hibernate更新数据库的时候，它怎么知道更新哪些查询缓存呢？
hibernate在一个地方维护每个表的最后更新时间，其实也就是放在上面net.sf.hibernate.cache.UpdateTimestampsCache所指定的缓存配置里面。 
当通过hibernate更新的时候，hibernate会知道这次更新影响了哪些表。然后它更新这些表的最后更新时间。每个缓存都有一个生成时间和这个缓存所查询的表，当hibernate查询一个缓存是否存在的时候，如果缓存存在，它还要取出缓存的生成时间和这个缓存所查询的表，然后去查找这些表的最后更新时间，如果有一个表在生成时间后更新过了，那么这个缓存是无效的。 
如果找到的时间戳晚于高速缓存查询结果的时间戳，那么缓存结果将被丢弃，重新执行一次查询。 
可以看出，只要更新过一个表，那么凡是涉及到这个表的查询缓存就失效了，因此查询缓存的命中率可能会比较低。 
使用二级缓存的前置条件
对于那些查询非常多但插入、删除、更新非常少的应用程序来说，查询缓存可提升性能。但写入多查询少的没有用，总失效。 
hibernate程序对数据库有独占的写访问权，其他的进程更新了数据库，hibernate是不可能知道的。 
你操作数据库必需直接通过hibernate，如果你调用存储过程，或者自己使用jdbc更新数据库，hibernate也是不知道的。 
这个限制相当的棘手，有时候hibernate做批量更新、删除很慢，但是你却不能自己写jdbc来优化。 
当然可以用SessionFactory提供的移除缓存的方法（上面的二级缓存的管理里面有介绍） 
**总结**
不要想当然的以为缓存一定能提高性能，仅仅在你能够驾驭它并且条件合适的情况下才是这样的。hibernate的二级缓存限制还是比较多的，不方便用jdbc可能会大大的降低更新性能。在不了解原理的情况下乱用，可能会有1+N的问题。不当的使用还可能导致读出脏数据。 
如果受不了Hibernate的诸多限制，那么还是自己在应用程序的层面上做缓存吧！ 
在越高的层面上做缓存，效果就会越好。就好像尽管磁盘有缓存，数据库还是要实现自己的缓存，尽管数据库有缓存，咱们的应用程序还是要做缓存。因为底层的缓存它并不知道高层要用这些数据干什么，只能做的比较通用，而高层可以有针对性的实现缓存，所以在更高的级别上做缓存，效果也要好些吧！ 
