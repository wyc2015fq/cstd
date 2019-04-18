# MyBatis 实践 -配置 - z69183787的专栏 - CSDN博客
2017年07月28日 11:33:03[OkidoGreen](https://me.csdn.net/z69183787)阅读数：297
个人分类：[Mybatis-学习](https://blog.csdn.net/z69183787/article/category/7026142)
## Configuration
mybatis-configuration.xml是MyBatis的全局配置文件(文件名任意),其配置内容和顺序如下:
- properties : 属性(文件)加载/配置
- settings : 全局配置参数
- typeAliases : 定义类型别名
- typeHandlers : 类型处理器
- objectFactory : 对象工厂
- plugins : 插件
- environments : 环境集合属性对象 
- environment 
- transactionManager : 事务管理
- dataSource : 数据源
- databaseIdProvider:P数据库厂商标识
- mappers : 映射器
### properties
方便对配置参数统一管理,供其他XML引用,我们可以将[数据库](http://lib.csdn.net/base/mysql)的连接参数抽取出来:
- db.properties
```
## Data Source
mysql.driver.class=com.mysql.jdbc.Driver
mysql.url=jdbc:mysql://host:port/db?characterEncoding=utf-8
mysql.user=user
mysql.password=password
```
- 1
- 2
- 3
- 4
- 5
- 1
- 2
- 3
- 4
- 5
- mybatis-configuration.xml
```xml
<properties resource="db.properties"/>
<environments default="development">
    <environment id="development">
        <!-- 配置JDBC事务管理-->
        <transactionManager type="JDBC"/>
        <!-- 配置数据源-->
        <dataSource type="POOLED">
            <property name="driver" value="${mysql.driver.class}"/>
            <property name="url" value="${mysql.url}"/>
            <property name="username" value="${mysql.user}"/>
            <property name="password" value="${mysql.password}"/>
        </dataSource>
    </environment>
</environments>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
> 
注: MyBatis按照如下顺序加载properties: 
1) 在`<properties>`标签内定义的属性; 
2) .properties文件中定义的属性; 
3) 最后读取作为方法参数传递的属性.
### settings
MyBatis全局配置参数,会影响MyBatis运行时行为(如:开启二级缓存/延迟加载).见[MyBatis文档](http://www.mybatis.org/mybatis-3/zh/configuration.html#settings).
### typeAliases
MyBatis默认支持的类型别名可参考[MyBatis文档](http://www.mybatis.org/mybatis-3/zh/configuration.html#typeAliases),我们也可以自定义别名,但并不推荐,使用PO对象的全限定名可以提高Statement的可读性.
### typeHandlers
`typeHandlers`用于Java类型和JDBC类型转换,MyBatis提供了很多默认的类型处理器(详见[MyBatis文档](http://www.mybatis.org/mybatis-3/zh/configuration.html#typeHandlers)),而且也基本满足日常开发需求,因此一般就不再需要单独定义.
### mappers
前面已经将SQL语句定义到了mapper文件中,那么`<mappers/>`属性就是告诉MyBatis到哪里去寻找mapper文件,MyBatis提供了如下几种配置方法:
|配置|描述|
|----|----|
|`<mapper resource=""/>`|使用类路径的资源(`Resources`/`java`目录下)|
|`<mapper url=""/>`|使用完全限定路径|
|`<mapper class=""/>`|使用mapper接口类路径|
|`<package name=""/>`|注册指定包下的所有mapper接口|
> 
注意:后两种方式要求mapper接口名和mapper映射文件名称相同,且放在同一个目录中(不推荐).
其他关于MyBatis的配置信息可参考[MyBatis文档](http://www.mybatis.org/mybatis-3/zh/configuration.html).
## 整合Spring
实现MyBatis与[spring](http://lib.csdn.net/base/javaee)整合之后,可以使用Spring来管理`SqlSessionFactory`和mapper接口,Spring自动使用`SqlSessionFactory`创建`SqlSession`,并将实现好DAO接口注册到Spring容器中,
 供`@Autowired`使用.
### 1. 添加依赖
- 添加Spring支持
```xml
<dependency>
    <groupId>org.springframework</groupId>
    <artifactId>spring-core</artifactId>
    <version>${spring.version}</version>
</dependency>
<dependency>
    <groupId>org.springframework</groupId>
    <artifactId>spring-context</artifactId>
    <version>${spring.version}</version>
</dependency>
<dependency>
    <groupId>org.springframework</groupId>
    <artifactId>spring-beans</artifactId>
    <version>${spring.version}</version>
</dependency>
<dependency>
    <groupId>org.springframework</groupId>
    <artifactId>spring-expression</artifactId>
    <version>${spring.version}</version>
</dependency>
<dependency>
    <groupId>org.springframework</groupId>
    <artifactId>spring-aop</artifactId>
    <version>${spring.version}</version>
</dependency>
<dependency>
    <groupId>org.springframework</groupId>
    <artifactId>spring-test</artifactId>
    <version>${spring.version}</version>
</dependency>
<dependency>
    <groupId>org.springframework</groupId>
    <artifactId>spring-jdbc</artifactId>
    <version>${spring.version}</version>
</dependency>
<dependency>
    <groupId>org.springframework</groupId>
    <artifactId>spring-tx</artifactId>
    <version>${spring.version}</version>
</dependency>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 添加MyBatis-Spring包
```xml
<dependency>
    <groupId>org.mybatis</groupId>
    <artifactId>mybatis-spring</artifactId>
    <version>${mybatis-spring.version}</version>
</dependency>
```
- 1
- 2
- 3
- 4
- 5
- 1
- 2
- 3
- 4
- 5
- 添加Hikaricp数据库连接池
```xml
<dependency>
    <groupId>com.zaxxer</groupId>
    <artifactId>HikariCP</artifactId>
    <version>${hikaricp.version}</version>
</dependency>
```
- 1
- 2
- 3
- 4
- 5
- 1
- 2
- 3
- 4
- 5
- 不要忘了MySQL数据库驱动
```xml
<dependency>
    <groupId>mysql</groupId>
    <artifactId>mysql-connector-java</artifactId>
    <version>${mysql.version}</version>
</dependency>
```
- 1
- 2
- 3
- 4
- 5
- 1
- 2
- 3
- 4
- 5
### 2. 配置文件
- 精简mybatis-configuration.xml 
可以将数据源的配置移到下面的applicationContext-datasource.xml中.
```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE configuration PUBLIC "-//mybatis.org//DTD Config 3.0//EN"
        "http://mybatis.org/dtd/mybatis-3-config.dtd">
<configuration>
    <mappers>
        <mapper resource="mybatis/mapper/UserDAO.xml"/>
    </mappers>
</configuration>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 定义applicationContext-datasource.xml
```xml
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:context="http://www.springframework.org/schema/context"
       xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context.xsd">
    <context:property-placeholder location="classpath:db.properties"/>
    <!-- 配置数据源 -->
    <bean id="hikariConfig" class="com.zaxxer.hikari.HikariConfig">
        <property name="driverClassName" value="${mysql.driver.class}"/>
        <property name="jdbcUrl" value="${mysql.url}"/>
        <property name="username" value="${mysql.user}"/>
        <property name="password" value="${mysql.password}"/>
        <property name="maximumPoolSize" value="5"/>
        <property name="maxLifetime" value="700000"/>
        <property name="idleTimeout" value="600000"/>
        <property name="connectionTimeout" value="10000"/>
        <property name="dataSourceProperties">
            <props>
                <prop key="dataSourceClassName">com.mysql.jdbc.jdbc2.optional.MysqlDataSource</prop>
                <prop key="cachePrepStmts">true</prop>
                <prop key="prepStmtCacheSize">250</prop>
                <prop key="prepStmtCacheSqlLimit">2048</prop>
            </props>
        </property>
    </bean>
    <bean id="dataSource" class="com.zaxxer.hikari.HikariDataSource" destroy-method="close">
        <constructor-arg ref="hikariConfig"/>
    </bean>
    <!-- 配置SqlSessionFactory -->
    <bean id="sqlSessionFactory" class="org.mybatis.spring.SqlSessionFactoryBean">
        <property name="dataSource" ref="dataSource"/>
        <property name="configLocation" value="classpath:mybatis/mybatis-configuration.xml"/>
    </bean>
    <!-- 根据mapper接口生成代理对象 -->
    <bean id="dao" class="org.mybatis.spring.mapper.MapperFactoryBean">
        <property name="mapperInterface" value="com.fq.mybatis.UserDAO"/>
        <property name="sqlSessionFactory" ref="sqlSessionFactory"/>
    </bean>
</beans>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
上面的配置存在一个问题:需要针对每个mapper配置一个`MapperFactoryBean`(繁琐),因此这段根据mapper接口生成代理对象的配置可更改如下:
```xml
<!-- 基于包扫描的mapper配置 -->
<bean class="org.mybatis.spring.mapper.MapperScannerConfigurer">
    <property name="basePackage" value="com.fq.mybatis"/>
    <property name="sqlSessionFactoryBeanName" value="sqlSessionFactory"/>
</bean>
```
- 1
- 2
- 3
- 4
- 5
- 1
- 2
- 3
- 4
- 5
> 
附: applicationContext-database.xml完整配置可参考: [Git地址](http://git.oschina.net/feiqing/Basic-Java/blob/master/src/main/resources/spring/applicationContext-datasource.xml?dir=0&filepath=src/main/resources/spring/applicationContext-datasource.xml&oid=82a06aef02c535331e291cf818918e3551933918&sha=3034ebde59640a00a90ff4aa4462c4a83ce51ff3)
- 定义Spring主配置文件applicationContext.xml 
定义注解驱动及加载静态配置文件datasource:
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:context="http://www.springframework.org/schema/context"
       xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context.xsd">
    <!-- 注解驱动 -->
    <context:annotation-config/>
    <!-- 加载静态配置文件 -->
    <import resource="applicationContext-datasource.xml"/>
</beans>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- Client
```java
/**
 * @author jifang
 * @since 16/2/22 上午10:20.
 */
@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations = "classpath:spring/applicationContext.xml")
public class UserDAOClient {
    @Autowired
    private UserDAO dao;
    @Test
    public void client() throws Exception {
        User user = dao.selectUserById(1);
        System.out.println(user);
    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
## 缓存
与大多数持久层框架一样,MyBatis也支持一级缓存和二级缓存. 
![](http://7xrgh9.com1.z0.glb.clouddn.com/16-3-3/56533764.jpg)
缓存作用是提升系统整体性能(不是提升数据库性能:因为缓存将数据库中的数据存放到内存,下次查询同样内容时直接从内存读取,减轻数据库压力,而且直接从内存中读取数据要比从数据库检索快很多,因此可以提升系统整体性能).
> 
缓存数据更新:当一个作用域(一级缓存为`SqlSession`/二级缓存为namespace)进行了`C/U/D`操作后,默认该作用域下所有缓存都被清空.
### 一级缓存
MyBatis默认开启了一级缓存.一级缓存是基于`org.apache.ibatis.cache.impl.PerpetualCache`的`HashMap`本地缓存,其存储作用域为`SqlSession`,同一个`SqlSession`几次执行相同SQL,后面的查询会直接从缓存中加载,从而提高查询效率/减轻数据库压力.当`SqlSession`经`flush`/`close`后,该`SqlSession`中的所有Cache数据被清空.
### 二级缓存
与一级缓存机制类似,MyBatis二级缓存默认也是采用`PerpetualCache`的`HashMap`存储,不同在于二级缓存存储作用域为`namespace`/`mapper`,并且可以自定义缓存实现,如Ehcache.
MyBatis默认没有开启二级缓存,需要经过以下步骤才能使用:
- 启用二级缓存(可选) 
其需要在mybatis-configuration.xml的`settings`全局参数中开启:
```xml
<settings>
    <setting name="cacheEnabled" value="true"/>
</settings>
```
- 1
- 2
- 3
- 1
- 2
- 3
> 
`cacheEnabled`对此配置文件下的所有cache进行全局性开/关设置(默认为`true`).
- 配置缓存策略 
在mapper映射文件中添加`<cache/>`标签,以指定该namespace开启二级缓存, 并指定缓存策略:
```bash
<cache eviction="LRU" flushInterval="60000" size="512" readOnly="true"/>
```
- 1
- 1
1) `eviction`:缓存淘汰[算法](http://lib.csdn.net/base/datastructure):
|算法|描述|释义|
|----|----|----|
|LRU|最近最少使用|移除最长时间不被使用的对象(默认).|
|FIFO|先进先出|按对象进入缓存的顺序移除.|
|SOFT|软引用|移除基于垃圾回收器状态和软引用规则的对象.|
|WEAK|弱引用|更积极地移除基于垃圾收集器状态和弱引用规则的对象.|
2) `flushInterval`:刷新间隔(缓存过期时间),单位为毫秒,MyBatis会每隔一段时间自动清空缓存(默认刷新间隔为空, 即永不过期,仅调用语句时刷新). 
3) `size`:引用数目,要记住你缓存的对象的数目和运行环境可用内存资源数目(默认1024). 
4) `readOnly`: 只读.如果为true,则所有相同SQL返回同一对象(因此这些对象不能修改,有助于提高性能,但并发操作同一条数据时,可能不安全);如果为false,则相同SQL后面返回的是cache的clone副本(通过序列化,慢一些但更是安全,因此默认是false).
- 序列化 
PO对象要实现`Serializable`序列化,因为二级缓存的存储介质不一定只是内存:
```java
public class User implements Serializable {
    //...
}
```
- 1
- 2
- 3
- 1
- 2
- 3
- Client
```java
@Test
public void cacheClient() throws Exception {
    testCache(factory.openSession());
    testCache(factory.openSession());
    testCache(factory.openSession());
}
private void testCache(SqlSession session) throws Exception {
    UserDAO dao = session.getMapper(UserDAO.class);
    dao.selectUserById(1);
    // 需要将SqlSession关闭才能将数据写入缓存.
    session.close();
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
运行代码, 并观察log输出的命中率(Cache Hit Ratio).
- Statement配置
1) 禁用缓存: 在Statement中设置`useCache="false"`可以禁用当前select语句的二级缓存(默认为`true`:该SQL启用二级缓存).
```
<select id="selectUserById" parameterType="java.lang.Integer" resultType="com.fq.domain.User" useCache="true">
    SELECT *
    FROM user
    WHERE id = #{id};
</select>
```
- 1
- 2
- 3
- 4
- 5
- 1
- 2
- 3
- 4
- 5
2)刷新缓存: 同一个namespace中,如果还有其它insert/update/delete操作,需要刷新缓存,使用`flushCache="true"`属性设置(默认为`true`刷新缓存).
```xml
<insert id="insertUserList" parameterType="java.util.List" flushCache="true">
    INSERT INTO user(name, password) VALUES
    <if test="list != null and list.size != 0">
        <foreach collection="list" item="user" separator=",">
            (#{user.name}, #{user.password})
        </foreach>
    </if>
</insert>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
### 整合Ehcache
MyBatis暴露一个`org.apache.ibatis.cache.Cache`接口出来,通过实现该接口,可以实现各类缓存产品(如Ehcache/[Redis](http://lib.csdn.net/base/redis)/Memcached)与MyBatis的整合(MyBatis的特长操作数据库,缓存管理并不是其擅长,因此整合其他缓存产品可以提高系统整体性能). 
Ehcache是一个纯Java开发的进程内缓存框架,具有开源/快速/灵活等特点,是[hibernate](http://lib.csdn.net/base/javaee)默认的CacheProvider.使用Ehcache需要在pom.xml中添加如下依赖:
```xml
<dependency>
    <groupId>net.sf.ehcache</groupId>
    <artifactId>ehcache-core</artifactId>
    <version>2.6.11</version>
</dependency>
<dependency>
    <groupId>org.mybatis.caches</groupId>
    <artifactId>mybatis-ehcache</artifactId>
    <version>1.0.3</version>
</dependency>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 配置Ehcache 
在Resources目录下添加ehcache.xml配置文件
```xml
<ehcache>
    <diskStore path="/data/cache"/>
    <defaultCache
            maxElementsInMemory="1000"
            maxElementsOnDisk="10000000"
            eternal="false"
            overflowToDisk="false"
            timeToIdleSeconds="120"
            timeToLiveSeconds="120"
            diskExpiryThreadIntervalSeconds="120"
            memoryStoreEvictionPolicy="LRU">
    </defaultCache>
</ehcache>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
|属性|描述|
|----|----|
|`diskStore`|指定缓存数据在磁盘的存储位置|
|`maxElementsInMemory`|在内存中缓存element的最大数目|
|`maxElementsOnDisk`|在磁盘上缓存element的最大数目,0表示无穷大|
|`eternal`|设定缓存的elements是否永远不过期.true,则缓存的数据始终有效,如果为false那么还要根据timeToIdleSeconds,timeToLiveSeconds判断|
|`overflowToDisk`|设定当内存缓存溢出的时候是否将过期的element缓存到磁盘上|
|`timeToIdleSeconds`|刷新间隔:缓存数据前后两次访问时间超过timeToIdleSeconds时,这些数据便会删除(默认为0,时间间隔无穷大)|
|`timeToLiveSeconds`|缓存element的有效生命期(默认为0,时间无限)|
|`diskSpoolBufferSizeMB`|设置DiskStore(磁盘缓存)缓存区大小.默认是30MB.|
|`diskPersistent`|在JVM重启时是否使用磁盘保存Ehcache数据,默认是false.|
|`diskExpiryThreadIntervalSeconds`|磁盘缓存的清理线程运行间隔,默认是120秒.|
|`memoryStoreEvictionPolicy`|当内存缓存达到最大,有新的element加入的时候, 移除缓存中element的策略.默认是LRU(最近最少使用),可选的有LFU(最不常使用)和FIFO(先进先出)|
- mapper配置ehcache
```bash
<cache type="org.mybatis.caches.ehcache.EhcacheCache" eviction="LRU" flushInterval="60000" size="1024"
       readOnly="true"/>
```
- 1
- 2
- 1
- 2
还可以根据需求调整当前namespace的缓存参数:
```xml
<cache type="org.mybatis.caches.ehcache.EhcacheCache">
    <property name="timeToIdleSeconds" value="3600"/>
    <property name="timeToLiveSeconds" value="3600"/>
    <!-- 同ehcache参数maxElementsInMemory -->
    <property name="maxEntriesLocalHeap" value="1000"/>
    <!-- 同ehcache参数maxElementsOnDisk -->
    <property name="maxEntriesLocalDisk" value="10000000"/>
    <property name="memoryStoreEvictionPolicy" value="LRU"/>
</cache>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
### 二级缓存小结
- 适用场景 
对于查询请求多且对查询结果实时性要求不高的场景,可采用二级缓存降低数据库负担,提高访问速度(业务场景如:微博/动态/订单信息等).
- 局限 
二级缓存对细粒度级别的缓存实现不好,如”缓存所有的商品信息时,二级缓存就无法实现当一个商品信息变化时只刷新该商品缓存而不刷新全部商品缓存“,因为二级缓存区域以`namespace`为单位划分,当一个商品发生变化会将所有商品缓存清空,因此解决此类问题需要在上层对数据进行业务划分.
