# Hibernate总结 - 深之JohnChen的专栏 - CSDN博客

2017年10月05日 23:55:06[byxdaz](https://me.csdn.net/byxdaz)阅读数：277
个人分类：[hibernate](https://blog.csdn.net/byxdaz/article/category/6892926)



Hibernate是一个对象关系映射框架，当然从分层的角度看，我们也说它是数据持久层的框架。

        一、Hibernate核心对象

     Hibernate的核心：面向对象、关系映射以及数据持久化。对于“数据持久化”，就是将数据或者某物体，永久的保存起来。而对于编程而言就是将数据保存在文件或磁盘以及数据库中。

![](https://img-blog.csdn.net/20171005235533308)

     上图说明：

1、Hibernate有主要的配置文件： xxx.hbm.xml用于对应一个纯javabean类配置表字段以及映射等。Javabean类中加入hibernate注解，通过注解配置表字段以及映射等。Hibernate.cfg.xml用于配置数据库参数、关联表等。

<mappingresource="com/zychen/www/User.hbm.xml"></mapping>

<mappingclass="com.zychen.www.User"></mapping>

2、Hibernate有五个或六个接口：Configuration、SessionFactory、Session、Transaction、Query和Criteria。

3、Hibernate的执行原理，看上图的箭头即可。

4、Session对象是通过SessionFactory构建的，这是Hibernate创建Session的两种方式之一。

二、基本映射

关系映射包括：一对一、一对多、多对一、多对多。

1、从对象的加载方向上分为单向和双向两种。

      单向和双向只影响数据的加载，并不影响数据的存储。不论是一对一，一对多还是多对多，单向和双向生成的数据库表是一样，单向和双向的不同是由对象模型决定的。

2、从对象的映射关系上分为一对多和多对一两种，它们又是从不同角度说的，所以也可以说是一种。

      一对一关联映射是多对一关联映射的特例，只是在“多”的一端加上唯一的限制之后，用来表示一对一的关联关系。

      多对多关联映射是一对多关联映射的特例，它们呢都是使用集合来表示多的关系，用<key>标签定义当前表的主键。

     当然它们既有联系也有区别，区别就不再一一列举了，系列博文中已经详细的介绍了各自的联系与区别。

三、反向工程

MyEclipse从数据库反向生成实体类之Hibernate方式 反向工程。

[http://www.cnblogs.com/chenbg2001/p/4284286.html](http://www.cnblogs.com/chenbg2001/p/4284286.html)

四、使用过程中注意事项

1、注意hibernate管理的对象的状态：是瞬时态还是托管态？有时候事务结束了之后还去读代理对象的值，这个时候设置了延迟加载的话会报错的。

2、注意在一对多主键级联操作的时候外键为NOT NULL可能会引发违例操作，这就和hibernate执行sql语句有关了；在一对多关联中，常把多的一方作为主动关联方，由它去维护关联关系；也就是将many的一方inverse=false，而且这样也有助于改善性能；维护关联关系体现在将对方的引用设置到自己的属性当中。

3、尽量少用多对多关联，可以用多个一对多关联来来代替多对多关联。

4、在修改或这个删除的时候最好都是先查出要修改的对象，再进行操作，这样可以避免原来的关联关系。

5、在编写POJO类的时候，必须使用JDK的接口，而不应该使用JDK Collection的实现类，因为Hibernate返回的Set是Hibernate对Set接口的特定实现，因而会造成强转错误。

6、hibenate的长对话session实现在web项目可以用Spring提供的OpenSessoinInView实现。

7、Hibernate中Session之get和load方法比较。

1）get不支持lazy，load支持lazy。

2）数据不存在时，get返回null，load则抛出ObjectNotFoundException异常。

3）load方法与get方法都可以返回实体的代理类实例，先从session缓存中查找，未找到时再从数据库中查找。

8、在双向关联的关系中，映射的column(和table)的值要一致(即要用相同的表名和列名)，不然设置为inverse="true"的这方将失去这个双向关系，而变成了一个单向关联。

9、Inverse和Cascade的比较

    Inverse：负责控制关系，默认为false，也就是关系的两端都能控制，但这样会造成一些问题，更新的时候会因为两端都控制关系，于是重复更新。一般来说有一端要设为true。

    Cascade：负责控制关联对象的级联操作，包括更新、删除等，也就是说对一个对象进行更新、删除时，其它对象也受影响，比如我删除一个对象，那么跟它是多对一关系的对象也全部被删除。

举例说明区别：删除“一”那一端一个对象O的时候，如果“多”的那一端的Inverse设为true，则把“多”的那一端所有与O相关联的对象外键清空；如果“多”的那一端的Cascade设为Delete，则把“多”的那一端所有与O相关联的对象全部删除。

10、Hibernate大量操作数据时造成内存溢出解决方案

1）、首先应该明确，session对象调用flush（）时是将一级缓存与数据库同步，hibernate一般会自己维护，是在查询前或是更新前都会进行调用，来保证缓存中的数据和数据库中的数据一致。

2）、大量操作数据时可能造成内存溢出，解决办法（有以下四种）如下：

    方法一：清除session中的数据

    for(int i=0;i<100000;i++)session.save(obj);//会出现内存溢出

    for(int i=0;i<100000;i++){

        session.save(obj);

        if(i% 50 ==0){session.flush();    session.clear();}

    }

    方法二：用StatelessSession接口：它不和一级缓存、二级缓存交互，也不触发任何事件、监听器、拦截器，通过该接口的操作会立刻发送给数据库，与JDBC的功能一样。

    StatelessSession s = sessionFactory.openStatelessSession();该接口的方法与Session类似。

    方法三：Query.executeUpdate()执行批量更新，会清除相关联的类二级缓存。

方法四：在核心配置文件的<session-factory>标签中加入<propertyname="hibernate.jdbc.batch_size">25</property>来进行批量插入。

11、Hibernate缓存机制

Hibernate缓存包括两大类：Hibernate一级缓存和Hibernate二级缓存。

Hibernate一级缓存又称为“Session的缓存”。Session内置不能被卸载，Session的缓存是事务范围的缓存（Session对象的生命周期通常对应一个数据库事务或者一个应用事务）。

evit(Object obj)  将指定的持久化对象从一级缓存中清除,释放对象所占用的内存资源,指定对象从持久化状态变为脱管状态,从而成为游离对象。

clear()  将一级缓存中的所有持久化对象清除,释放其占用的内存资源。 

contains(Object obj) 判断指定的对象是否存在于一级缓存中。 

flush() 刷新一级缓存区的内容,使之与数据库数据保持同步。

save()，当session对象调用save()方法保存一个对象后，该对象会被放入到session的缓存中。 get()和load()，当session对象调用get()或load()方法从数据库取出一个对象后，该对象也会被放入到session的缓存中。

Hibernate二级缓存又称为“SessionFactory的缓存”， Hibernate二级缓存是进程范围或者集群范围的缓存，有可能出现并发问题，因此需要采用适当的并发访问策略，该策略为被缓存的数据提供了事务隔离级别。

evict(Classarg0, Serializable arg1)将某个类的指定ID的持久化对象从二级缓存中清除,释放对象所占用的资源。

sessionFactory.evict(Customer.class,new Integer(1)); 

evict(Classarg0)  将指定类的所有持久化对象从二级缓存中清除,释放其占用的内存资源。

sessionFactory.evict(Customer.class);  

evictCollection(Stringarg0)  将指定类的所有持久化对象的指定集合从二级缓存中清除,释放其占用的内存资源。

sessionFactory.evictCollection("Customer.orders"); 

一级缓存与二级缓存的对比图。
||一级缓存|二级缓存|
|----|----|----|
|存放数据的形式|相互关联的持久化对象|对象的散装数据|
|缓存的范围|事务范围，每个事务都拥有单独的一级缓存|进程范围或集群范围，缓存被同一个进程或集群范围内所有事务共享|
|并发访问策略|由于每个事务都拥有单独的一级缓存不会出现并发问题，因此无须提供并发访问策略|由于多个事务会同时访问二级缓存中的相同数据，因此必须提供适当的并发访问策略，来保证特定的事务隔离级别|
|数据过期策略|处于一级缓存中的对象永远不会过期，除非应用程序显示清空或者清空特定对象|必须提供数据过期策略，如基于内存的缓存中对象的最大数目，允许对象处于缓存中的最长时间，以及允许对象处于缓存中的最长空闲时间|
|物理介质|内存|内存和硬盘，对象的散装数据首先存放到基于内存的缓存中，当内存中对象的数目达到数据过期策略的maxElementsInMemory值，就会把其余的对象写入基于硬盘的缓存中|
|缓存软件实现|在Hibernate的Session的实现中包含|由第三方提供，Hibernate仅提供了缓存适配器，用于把特定的缓存插件集成到Hibernate中|
|启用缓存的方式|只要通过Session接口来执行保存，更新，删除，加载，查询，Hibernate就会启用一级缓存，对于批量操作，如不希望启用一级缓存，直接通过JDBCAPI来执行|用户可以再单个类或类的单个集合的粒度上配置第二级缓存，如果类的实例被经常读，但很少被修改，就可以考虑使用二级缓存，只有为某个类或集合配置了二级缓存，Hibernate在运行时才会把它的实例加入到二级缓存中|
|用户管理缓存的方式|一级缓存的物理介质为内存，由于内存的容量有限，必须通过恰当的检索策略和检索方式来限制加载对象的数目，Session的evit（）方法可以显示的清空缓存中特定对象，但不推荐|二级缓存的物理介质可以使内存和硬盘，因此第二级缓存可以存放大容量的数据，数据过期策略的maxElementsInMemory属性可以控制内存中的对象数目，管理二级缓存主要包括两个方面：选择需要使用第二级缓存的持久化类，设置合适的并发访问策略；选择缓存适配器，设置合适的数据过期策略。SessionFactory的evit（）方法也可以显示的清空缓存中特定对象，但不推荐|

12、常用的二级缓存插件

EHCache org.hibernate.cache.EhCacheProvider

OSCache org.hibernate.cache.OSCacheProvider

SwarmCahe org.hibernate.cache.SwarmCacheProvider

JBossCache org.hibernate.cache.TreeCacheProvider

<!-- EHCache的配置,hibernate.cfg.xml --> 

<hibernate-configuration>

   <session-factory>

      <!-- 设置二级缓存插件EHCache的Provider类-->

      <property name="hibernate.cache.provider_class">

         org.hibernate.cache.EhCacheProvider

      </property>

      <!-- 启动"查询缓存" -->

      <property name="hibernate.cache.use_query_cache">

         true

      </property>

   </session-factory>

 </hibernate-configuration>

<!-- ehcache.xml -->

<?xml version="1.0"encoding="UTF-8"?>

<ehcache>

    <!--

        缓存到硬盘的路径

    -->

    <diskStorepath="d:/ehcache"></diskStore>

    <!--

        默认设置

        maxElementsInMemory : 在內存中最大緩存的对象数量。

        eternal : 缓存的对象是否永远不变。

        timeToIdleSeconds ：可以操作对象的时间。

        timeToLiveSeconds ：缓存中对象的生命周期，时间到后查询数据会从数据库中读取。

        overflowToDisk ：内存满了，是否要缓存到硬盘。

    -->

    <defaultCache maxElementsInMemory="200"eternal="false" 

        timeToIdleSeconds="50"timeToLiveSeconds="60"overflowToDisk="true"></defaultCache>

    <!--

        指定缓存的对象。

        下面出现的的属性覆盖上面出现的，没出现的继承上面的。

    -->

    <cachename="com.suxiaolei.hibernate.pojos.Order"maxElementsInMemory="200" eternal="false"

        timeToIdleSeconds="50"timeToLiveSeconds="60"overflowToDisk="true"></cache>

</ehcache>

<!-- *.hbm.xml -->

<?xml version="1.0"encoding='UTF-8'?>

<!DOCTYPE hibernate-mapping PUBLIC

 "-//Hibernate/Hibernate Mapping DTD3.0//EN"

 "http://hibernate.sourceforge.net/hibernate-mapping-3.0.dtd">

<hibernate-mapping>

  <class>

      <!-- 设置该持久化类的二级缓存并发访问策略 read-only read-write nonstrict-read-write transactional-->

      <cache usage="read-write"/>    

  </class>

</hibernate-mapping>

若存在一对多的关系，想要在在获取一方的时候将关联的多方缓存起来，需要在集合属性下添加<cache>子标签，这里需要将关联的对象的hbm文件中必须在存在<class>标签下也添加<cache>标签，不然Hibernate只会缓存OID。

<hibernate-mapping>

       <class name="com.suxiaolei.hibernate.pojos.Customer"table="customer">

           <!-- 主键设置 -->

           <id name="id" type="string">

                <columnname="id"></column>

                <generatorclass="uuid"></generator>

           </id>

           <!-- 属性设置 -->

           <property name="username" column="username"type="string"></property>

           <property name="balance" column="balance"type="integer"></property>

           <set name="orders" inverse="true"cascade="all" lazy="false" fetch="join">

                <cacheusage="read-only"/>

                <keycolumn="customer_id" ></key>

                <one-to-manyclass="com.suxiaolei.hibernate.pojos.Order"/>

           </set>

       </class>

</hibernate-mapping>

13、调用存储过程。

    Hibernate调用存储过程有两种方式：xml映射方式、类似jdbc方式。

数据库表以及存储过程

CREATE   TABLE `proctab` (

`id`  int ( 11 ) NOT   NULL  auto_increment,

`Name`  varchar ( 20 ),

`age`  int ( 11 ),

PRIMARY   KEY  (`id`)

);

create   PROCEDURE proc()

 begin

    select  *   from  proctab;

 end ;

1）hibernate传统的xml映射方式

<class name="com.test.User"table="proctab">

       <id name="id" column="id">

           <generator class="native"/>

       </id>

       <property name="name" column="name"type="string" />

       <property name="age" column="age"type="integer" />

</class>

  <sql-query name="getUser" callable="true">

    <return alias="user" class="com.test.User">

    <return-property name="id" column="id" />

    <return-property name="name" column="name" />

    <return-property name="age" column="age" />

    </return>

    {call proc()}

  </sql-query>

调用方法 

 Sessionss= HibernateSessionFactory.getSession()

 Listli=ss.getNamedQuery("getUser").list();

 ss.close();

2）类似jdbc方式

Session session=HibernateSessionFactory.getSession();

Connection conn = session.connection(); 

ResultSet rs =null;

CallableStatement call = conn.prepareCall("{Callproc()}");

rs = call.executeQuery();

rs.close();

session.close();

hibernate 调用存储过程返回参数

Connection conn= getSession().connection();

CallableStatement cs=null;

//指定调用的存储过程 

cs = conn.prepareCall("{CallPRO_PROCESSING_COMMON.GET_STR(?,?)}"); 

//设置参数 

cs.setString(1, "TESTSTR"); 

//这里需要配置OUT的参数新型 

cs.registerOutParameter(2,Types.VARCHAR); 

//执行调用 

cs.execute();   

//输入返回值

System.out.println(cs.getString(2));


