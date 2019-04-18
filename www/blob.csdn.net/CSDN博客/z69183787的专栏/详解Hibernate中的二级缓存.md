# 详解Hibernate中的二级缓存 - z69183787的专栏 - CSDN博客
2017年07月14日 10:32:36[OkidoGreen](https://me.csdn.net/z69183787)阅读数：415
个人分类：[对象映射框架-Hibernate](https://blog.csdn.net/z69183787/article/category/2175489)
[http://m.blog.csdn.net/ZHOUCHAOQIANG/article/details/47748179](http://m.blog.csdn.net/ZHOUCHAOQIANG/article/details/47748179)
**关于get 方法：Hibernate3.x回去查二级缓存的，然后再去查数据库，没有则返回null，这跟load(或find)一样，当然load找不到则返回异常。Hibernate3.x之前的是越过二级的 **
**[http://blog.csdn.net/z69183787/article/details/42394977](http://blog.csdn.net/z69183787/article/details/42394977)**
1.前言
> 
这篇博客再前几篇博客的基础上来讲解一下，Hibernate中的二级缓存，二级缓存是属于SessionFactory级别的缓存机制。第一级别的缓存是Session级别的缓存，是属于事务范围的缓存，由Hibernate管理，一般无需进行干预。第二级别的缓存是SessionFactory级别的缓存，是属于进程范围的缓存。
2.Hibernate二级缓存
> 
1.分类
二级缓存也分为了两种
内置缓存：Hibernate自带的，不可卸载，通常在Hibernate的初始化阶段，Hibernate会把映射元数据和预定义的SQL语句放置到SessionFactory的缓存中。该内置缓存是只读的。
外置缓存：通常说的二级缓存也就是外置缓存，在默认情况下SessionFactory不会启用这个缓存插件，外置缓存中的数据是数据库数据的复制，外置缓存的物理介质可以是内存或者硬盘。
hibernate二级缓存的结构
![](https://img-blog.csdn.net/20150818103845460?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
2.并发访问策略
|transactional（事务型）|仅在受管理的环境中适用提供Repeatable Read事务隔离级别适用经常被读，很少修改的数据可以防止脏读和不可重复读的并发问题缓存支持事务，发生异常的时候，缓存也能够回滚|
|----|----|
|read-write（读写型）|提供Read Committed事务隔离级别在非集群的环境中适用适用经常被读，很少修改的数据可以防止脏读更新缓存的时候会锁定缓存中的数据|
|nonstrict-read-write（非严格读写型）|适用极少被修改，偶尔允许脏读的数据（两个事务同时修改数据的情况很少见）不保证缓存和数据库中数据的一致性为缓存数据设置很短的过期时间，从而尽量避免脏读不锁定缓存中的数据|
|read-only（只读型）|适用从来不会被修改的数据（如参考数据）在此模式下，如果对数据进行更新操作，会有异常事务隔离级别低，并发性能高在集群环境中也能完美运作|
分析：通过上述表格分析如下
适合放入二级缓存中数据
> 
> 
很少被修改
> 
不是很重要的数据，允许出现偶尔的并发问题
> 
不适合放入二级缓存中的数据
> 
经常被修改
> 
> 
财务数据，绝对不允许出现并发问题
> 
与其他应用数据共享的数据
> 
3.二级缓存的配置
> 
1.hibernate支持的缓存插件
•EHCache: 可作为进程范围内的缓存,存放数据的物理介质可以是内存或硬盘,对Hibernate的查询缓存提供了支持
•OpenSymphony`:可作为进程范围内的缓存,存放数据的物理介质可以是内存或硬盘,提供了丰富的缓存数据过期策略,对Hibernate的查询缓存提供了支持
•SwarmCache:可作为集群范围内的缓存,但不支持Hibernate的查询缓存
•JBossCache:可作为集群范围内的缓存,支持Hibernate的查询缓存
四种缓存插件支持的并发范围策略如下图
![](https://img-blog.csdn.net/20150818104717019?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
2.二级缓存配置
下面以ehcache缓存为例，来讲一下二级缓存的配置
> 
> 
2.1 拷贝jar包
如要第三方的jar包ehcache-1.5.0.jar，并且依赖于
依赖backport-util-concurrent 和 commons-logging
2.2 在hibernate.cfg.xml中开启二级缓存
<propertyname="hibernate.cache.use_second_level_cache">true</property>
2.3 配置二级缓存技术提供商
<propertyname="hibernate.cache.provider_class">org.hibernate.cache.EhCacheProvider</property>
2.4 配置缓存数据对象并发策略
> 
> 
> 
方式一 在hbm文件中配置
```
<span style="font-family:SimSun;font-size:18px;"><class name="cn.itcast.domain.Customer" table="customers" catalog="hibernate3day4" >
				<!-- 类级别缓存 -->
				<cache usage="read-write"/>
				<set name="orders" cascade="all-delete-orphan" inverse="true" >
					<!-- 关联集合级别缓存 -->
					<cache usage="read-write"/>	
				</set>
			</class>
</span>
```
方式二 在cfg文件配置（集中配置）
> 
```
<span style="font-family:SimSun;font-size:18px;"><!-- 类级别缓存 -->
			<class-cache usage="read-write" class="cn.itcast.domain.Customer"/>
			<class-cache usage="read-write" class="cn.itcast.domain.Order"/>
			<!-- 集合缓存 -->
			<collection-cache usage="read-write" collection="cn.itcast.domain.Customer.orders"/>
</span>
```
2.5 添加二级缓存配置文件
在src中配置ehcache.xml，将ehcache.jar包中的ehcache-failsafe.xml 改名 ehcache.xml 放入 src 
```
<ehcache xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="../config/ehcache.xsd">
    <diskStore path="java.io.tmpdir"/> 配置二级缓存硬盘临时目录位置 
		 <defaultCache  
            maxElementsInMemory="10000" // 内存中最大对象数量 ，超过数量，数据会被缓存到硬盘 
            eternal="false"
            timeToIdleSeconds="120" // 是否缓存为永久性 false 不永久
            timeToLiveSeconds="120" // 存活时间，对象不管是否使用，到了时间回收
            overflowToDisk="true" // 是否可以缓存到硬盘
            maxElementsOnDisk="10000000" // 硬盘缓存最大对象数量 
            // 当jvm结束时是否持久化对象 true false 默认是false
            diskExpiryThreadIntervalSeconds="120"  // 指定专门用于清除过期对象的监听线程的轮询时间 
            memoryStoreEvictionPolicy="LRU" 
            />
</ehcache>
```

 4.Demo测试二级缓存
> 
```java
@Test
	public void fun1() {
		Session s1 = HibernateUtils.getSession();
		s1.beginTransaction();
		Customer c1 = (Customer) s1.get(Customer.class, 1); // 从数据库中加载数据
		System.out.println(c1.getName());//此时才会发出SQL语句
		s1.getTransaction().commit();
		s1.close(); // 关闭session级别的一级缓存
		Session s2 = HibernateUtils.getSession();
		s2.beginTransaction();
		Customer c2 = (Customer) s2.get(Customer.class, 1); // 因为有了二级缓存的存在，直接从二级缓存中取出即可
		System.out.println(c2.getName());
		Customer c3 = (Customer) s2.get(Customer.class, 1); //从二级缓存中取出
		System.out.println(c3.getName());
		s2.getTransaction().commit();
		s2.close();
	}
```
