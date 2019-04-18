# hibernate缓存：一级缓存和二级缓存 - z69183787的专栏 - CSDN博客
2015年01月23日 13:43:45[OkidoGreen](https://me.csdn.net/z69183787)阅读数：583
个人分类：[对象映射框架-Hibernate](https://blog.csdn.net/z69183787/article/category/2175489)
### 1.什么是缓存？
　缓存是介于物理数据源与应用程序之间，是对数据库中的数据复制一份临时放在内存中的容器，其作用是为了减少应用程序对物理数据源访问的次数，从而提高了应用程序的运行性能。Hibernate在进行读取数据的时候，根据缓存机制在相应的缓存中查询，如果在缓存中找到了需要的数据(我们把这称做“缓存命 中")，则就直接把命中的数据作为结果加以利用，避免了大量发送SQL语句到数据库查询的性能损耗。
**缓存策略提供商：**
提供了HashTable缓存，EHCache，OSCache，SwarmCache，jBoss Cathe2，这些缓存机制，其中EHCache，OSCache是不能用于集群环境（Cluster Safe）的，而SwarmCache，jBoss Cathe2是可以的。HashTable缓存主要是用来测试的，只能把对象放在内存中，EHCache，OSCache可以把对象放在内存（memory）中，也可以把对象放在硬盘（disk）上（为什么放到硬盘上？上面解释了）。
**Hibernate缓存分类：**
一、Session缓存（又称作事务缓存）：Hibernate内置的，不能卸除。
缓存范围：缓存只能被当前Session对象访问。缓存的生命周期依赖于Session的生命周期，当Session被关闭后，缓存也就结束生命周期。
二、SessionFactory缓存（又称作应用缓存）：使用第三方插件，可插拔。
缓存范围：缓存被应用范围内的所有session共享,不同的Session可以共享。这些session有可能是并发访问缓存，因此必须对缓存进行更新。缓存的生命周期依赖于应用的生命周期，应用结束时，缓存也就结束了生命周期，二级缓存存在于应用程序范围。
### 一级缓存：
Hibernate一些与一级缓存相关的操作（时间点）：
数据放入缓存：
1. save()。当session对象调用save()方法保存一个对象后，该对象会被放入到session的缓存中。
2. get()和load()。当session对象调用get()或load()方法从数据库取出一个对象后，该对象也会被放入到session的缓存中。
3. 使用HQL和QBC等从数据库中查询数据。
例如：数据库有一张表如下：
![](http://pic002.cnblogs.com/images/2012/369936/2012012017030489.png)
使用get()或load()证明缓存的存在：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
public class Client
{
    public static void main(String[] args)
    {
        Session session = HibernateUtil.getSessionFactory().openSession();
        Transaction tx = null;
        try
        {
            /*开启一个事务*/
            tx = session.beginTransaction();
            /*从数据库中获取id="402881e534fa5a440134fa5a45340002"的Customer对象*/
            Customer customer1 = (Customer)session.get(Customer.class, "402881e534fa5a440134fa5a45340002");
            System.out.println("customer.getUsername is"+customer1.getUsername());
            /*事务提交*/
            tx.commit();
            
            System.out.println("-------------------------------------");
            
            /*开启一个新事务*/
            tx = session.beginTransaction();
            /*从数据库中获取id="402881e534fa5a440134fa5a45340002"的Customer对象*/
            Customer customer2 = (Customer)session.get(Customer.class, "402881e534fa5a440134fa5a45340002");
            System.out.println("customer2.getUsername is"+customer2.getUsername());
            /*事务提交*/
            tx.commit();
            
            System.out.println("-------------------------------------");
            
            /*比较两个get()方法获取的对象是否是同一个对象*/
            System.out.println("customer1 == customer2 result is "+(customer1==customer2));
        }
        catch (Exception e)
        {
            if(tx!=null)
            {
                tx.rollback();
            }
        }
        finally
        {
            session.close();
        }
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
**程序控制台输出结果：**
Hibernate:
    select
        customer0_.id as id0_0_,
        customer0_.username as username0_0_,
        customer0_.balance as balance0_0_
    from
        customer customer0_
    where
        customer0_.id=?
**customer.getUsername islisi**
-------------------------------------
**customer2.getUsername islisi**
-------------------------------------
**customer1 == customer2 result is true**
**其原理是**：**在同一个Session里面，**第一次调用get()方法， Hibernate先检索缓存中是否有该查找对象，发现没有，Hibernate发送SELECT语句到数据库中取出相应的对象，然后将该对象放入缓存中，以便下次使用，第二次调用get()方法，Hibernate先检索缓存中是否有该查找对象，发现正好有该查找对象，就从缓存中取出来，不再去数据库中检索，没有再次发送select语句。
**数据从缓存中清除：**
1. evit()将指定的持久化对象从缓存中清除，释放对象所占用的内存资源，指定对象从持久化状态变为脱管状态，从而成为游离对象。 
2. clear()将缓存中的所有持久化对象清除，释放其占用的内存资源。
其他缓存操作：
1. contains()判断指定的对象是否存在于缓存中。
2. flush()刷新缓存区的内容，使之与数据库数据保持同步。
### 二级缓存：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
@Test
public void testCache2() {
Session session1 = sf.openSession();//获得Session1
session1.beginTransaction();
Category c = (Category)session1.load(Category.class, 1);
System.out.println(c.getName());
session1.getTransaction().commit();
session1.close();
Session session2 = sf.openSession();//获得Session2
session2.beginTransaction();
Category c2 = (Category)session2.load(Category.class, 1);
System.out.println(c2.getName());
session2.getTransaction().commit();
session2.close();
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
当我们重启一个Session，第二次调用load或者get方法检索同一个对象的时候会重新查找数据库，会发select语句信息。
***原因：一个session不能取另一个session中的缓存。***
性能上的问题：假如是多线程同时去取Category这个对象，load一个对象，这个对像本来可以放到内存中的，可是由于是多线程，是分布在不同的session当中的，所以每次都要从数据库中取，这样会带来查询性能较低的问题。
**解决方案：使用二级缓存。**
**1.什么是二级缓存？**
SessionFactory级别的缓存，可以跨越Session存在，可以被多个Session所共享。
**2.适合放到二级缓存中：**
（1）经常被访问
（2）改动不大
（3）数量有限
（4）不是很重要的数据，允许出现偶尔并发的数据。
这样的数据非常适合放到二级缓存中的。
用户的权限：用户的数量不大，权限不多，不会经常被改动，经常被访问。
例如组织机构。
思考：什么样的类，里面的对象才适合放到二级缓存中？
改动频繁，类里面对象特别多，BBS好多帖子，这些帖子20000多条，哪些放到缓存中，不能确定。除非你确定有一些经常被访问的，数据量并不大，改动非常少，这样的数据非常适合放到二级缓存中的。
**3.二级缓存实现原理：**
 　Hibernate如何将数据库中的数据放入到二级缓存中？注意，你可以把缓存看做是一个Map对象，它的Key用于存储对象OID，Value用于存储POJO。首先，当我们使用Hibernate从数据库中查询出数据，获取检索的数据后，Hibernate将检索出来的对象的OID放入缓存中key 中，然后将具体的POJO放入value中，等待下一次再次向数据查询数据时，Hibernate根据你提供的OID先检索一级缓存，若有且配置了二级缓存，则检索二级缓存，如果还没有则才向数据库发送SQL语句，然后将查询出来的对象放入缓存中。
**4.使用二级缓存**
**（1）打开二级缓存：**
为Hibernate配置二级缓存：
在主配置文件中hibernate.cfg.xml ：
 <!-- 使用二级缓存 -->
```
<!-- 使用二级缓存 -->
<property name="hibernate.cache.use_second_level_cache">true</property>    
<!--设置缓存的类型，设置缓存的提供商-->
<property name="hibernate.cache.provider_class">org.hibernate.cache.EhCacheProvider</property>
```
或者当hibernate与Spring整合后直接配到Spring配置文件applicationContext.xml中
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<bean id="sessionFactory" class="org.springframework.orm.hibernate3.LocalSessionFactoryBean">
     <property name="dataSource" ref="dataSource"/>
     <property name="mappingResources">
        <list>
          <value>com/lp/ecjtu/model/Employee.hbm.xml</value>
          <value>com/lp/ecjtu/model/Department.hbm.xml</value>
        </list>
     </property>
     <property name="hibernateProperties">
        <value>
        hibernate.dialect=org.hibernate.dialect.OracleDialect
        hibernate.hbm2ddl.auto=update
        hibernate.show_sql=true
        hibernate.format_sql=true    
        hibernate.cache.use_second_level_cache=true
        hibernate.cache.provider_class=org.hibernate.cache.EhCacheProvider
         hibernate.generate_statistics=true           
     </value>
    </property>
</bean>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
（2）配置ehcache.xml
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<?xml version="1.0" encoding="UTF-8"?>
<ehcache>
    <!--
        缓存到硬盘的路径
    -->
    <diskStore path="d:/ehcache"></diskStore>
    
    <!--
        默认设置
        maxElementsInMemory : 在內存中最大緩存的对象数量。
        eternal : 缓存的对象是否永远不变。
        timeToIdleSeconds ：可以操作对象的时间。
        timeToLiveSeconds ：缓存中对象的生命周期，时间到后查询数据会从数据库中读取。
        overflowToDisk ：内存满了，是否要缓存到硬盘。
    -->
    <defaultCache maxElementsInMemory="200" eternal="false" 
        timeToIdleSeconds="50" timeToLiveSeconds="60" overflowToDisk="true"></defaultCache>
        
    <!--
        指定缓存的对象。
        下面出现的的属性覆盖上面出现的，没出现的继承上面的。
    -->
    <cache name="com.suxiaolei.hibernate.pojos.Order" maxElementsInMemory="200" eternal="false" 
        timeToIdleSeconds="50" timeToLiveSeconds="60" overflowToDisk="true"></cache>
</ehcache>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
**（3）使用二级缓存需要在实体类中加入注解：**
需要ehcache-1.2.3.jar包:
还需要 commons_loging1.1.1.jar包
在实体类中通过注解可以配置实用二级缓存：
**@Cache(usage = CacheConcurrencyStrategy.READ_WRITE)**
Load默认使用二级缓存，就是当查一个对象的时候，它先会去二级缓存里面去找，如果找到了就不去数据库中查了。
Iterator默认的也会使用二级缓存，有的话就不去数据库里面查了，不发送select语句了。
List默认的往二级缓存中加数据，假如有一个query，把数据拿出来之后会放到二级缓存，但是执行查询的时候不会到二级缓存中查，会在数据库中查。原因每个query中查询条件不一样。
**（4）也可以在需要被缓存的对象中hbm文件中的<class>标签下添加一个<cache>子标签:**
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 <hibernate-mapping>
        <class name="com.suxiaolei.hibernate.pojos.Order" table="orders">
            <cache usage="read-only"/>
            <id name="id" type="string">
                <column name="id"></column>
                <generator class="uuid"></generator>
            </id>
           
            <property name="orderNumber" column="orderNumber" type="string"></property>
            <property name="cost" column="cost" type="integer"></property>
           
            <many-to-one name="customer" class="com.suxiaolei.hibernate.pojos.Customer" 
                         column="customer_id" cascade="save-update">
            </many-to-one>       
        </class>
    </hibernate-mapping>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
存在一对多的关系，想要在在获取一方的时候将关联的多方缓存起来，需要再集合属性下添加<cache>子标签，这里需要将关联的对象的 hbm文件中必须在存在<class>标签下也添加<cache>标签，不然Hibernate只会缓存OID。
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<hibernate-mapping>
        <class name="com.suxiaolei.hibernate.pojos.Customer" table="customer">
            <!-- 主键设置 -->
            <id name="id" type="string">
                <column name="id"></column>
                <generator class="uuid"></generator>
            </id>
           
            <!-- 属性设置 -->
            <property name="username" column="username" type="string"></property>
            <property name="balance" column="balance" type="integer"></property>
           
            <set name="orders" inverse="true" cascade="all" lazy="false" fetch="join">
                <cache usage="read-only"/>
                <key column="customer_id" ></key>
                <one-to-many class="com.suxiaolei.hibernate.pojos.Order"/>
            </set>
        </class>
    </hibernate-mapping>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
