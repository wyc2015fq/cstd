# Spring 缓存注解@Cacheable的用法 - z69183787的专栏 - CSDN博客
2019年01月04日 19:11:33[OkidoGreen](https://me.csdn.net/z69183787)阅读数：243
[https://blog.csdn.net/qq_17555933/article/details/51320800](https://blog.csdn.net/qq_17555933/article/details/51320800)
  在Spring中通过获取MemCachedClient来实现与memcached服务器进行数据读取的方式。不过，在实际开发中，我们往往是通过Spring的@Cacheable来实现数据的缓存的，所以，本文给大家详细介绍一下@Cacheable的用法。首先，在使用@Cacheable之前，我们要做好准备工作。
第一步：要导入相应的jar包。
```
<classpathentry kind="lib" path="lib/spring-core-4.1.4.RELEASE.jar"/>
    <classpathentry kind="lib" path="lib/spring-cache-1.0.10.jar"/>
    <classpathentry kind="lib" path="lib/spring-context-4.1.4.RELEASE.jar"/>
    <classpathentry kind="lib" path="lib/spring-beans-4.1.4.RELEASE.jar"/>
    <classpathentry kind="lib" path="lib/commons-logging-1.2.jar"/>
    <classpathentry kind="lib" path="lib/log4j-1.2.17.jar"/>
    <classpathentry kind="lib" path="lib/spring-expression-4.1.4.RELEASE.jar"/>
    <classpathentry kind="lib" path="lib/java_memcached-release_2.0.1.jar"/>
    <classpathentry kind="lib" path="lib/spring-aop-4.1.4.RELEASE.jar"/>
    <classpathentry kind="lib" path="lib/spring-aspects-4.1.4.RELEASE.jar"/>
    <classpathentry kind="lib" path="lib/spring-context-support-4.1.4.RELEASE.jar"/>
    <classpathentry kind="lib" path="lib/spring-tx-4.1.4.RELEASE.jar"/>
    <classpathentry kind="lib" path="lib/aopalliance-1.0.jar"/>
    <classpathentry kind="lib" path="lib/ognl-3.0.6.jar"/>
    <classpathentry kind="lib" path="lib/trafficCounter-1.0.2.jar"/>
    <classpathentry kind="lib" path="lib/aspectjweaver-1.8.4.jar"/>
    <classpathentry kind="lib" path="lib/javassist-3.11.0.GA.jar"/>
```
第二步：xml文件中增加命名空间。
```
<?xml version="1.0" encoding="UTF-8"?>  
<beans xmlns="http://www.springframework.org/schema/beans"  
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xmlns:cache="http://www.springframework.org/schema/cache"
    xmlns:context="http://www.springframework.org/schema/context"
    xsi:schemaLocation="http://www.springframework.org/schema/beans  
    http://www.springframework.org/schema/beans/spring-beans-3.1.xsd
    http://www.springframework.org/schema/cache
    http://www.springframework.org/schema/cache/spring-cache-3.1.xsd
    http://www.springframework.org/schema/context 
    http://www.springframework.org/schema/context/spring-context-3.1.xsd">
```
之后添加如下声明：
```
<?xml version="1.0" encoding="UTF-8"?>  
<beans xmlns="http://www.springframework.org/schema/beans"  
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xmlns:cache="http://www.springframework.org/schema/cache"
    xmlns:context="http://www.springframework.org/schema/context"
    xsi:schemaLocation="http://www.springframework.org/schema/beans  
    http://www.springframework.org/schema/beans/spring-beans-3.1.xsd
    http://www.springframework.org/schema/cache
    http://www.springframework.org/schema/cache/spring-cache-3.1.xsd
    http://www.springframework.org/schema/context 
    http://www.springframework.org/schema/context/spring-context-3.1.xsd">
    
    <!-- 启用缓存注解功能，这个是必须的，否则注解不会生效，另外，该注解一定要声明在spring主配置文件中才会生效 --> 
    <cache:annotation-driven cache-manager="cacheManager" />
    
    <!-- cacheManager工厂类，指定ehcache.xml的位置 -->   
    <bean id="cacheManagerFactory" class="org.springframework.cache.ehcache.EhCacheManagerFactoryBean">  
        <property name="configLocation"  value="classpath:cache/ehcache.xml"/>  
    </bean>
    
    <!-- 声明cacheManager -->  
    <bean id="cacheManager" class="org.springframework.cache.ehcache.EhCacheCacheManager">
        <property name="cacheManager" ref="cacheManagerFactory"/>
    </bean>
</beans>
```
第二步： ehcache.xml
```
<?xml version="1.0" encoding="UTF-8"?>
<ehcache xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
    xsi:noNamespaceSchemaLocation="http://ehcache.org/ehcache.xsd"
    updateCheck="false" dynamicConfig="false" monitoring="autodetect">  
    <diskStore path="java.io.tmpdir" />
    <!--
        diskStore path:用来配置磁盘缓存使用的物理路径
        name:    缓存名称,cache的唯一标识(ehcache会把这个cache放到HashMap里)
        eternal="false"   元素是否永恒,如果是就永不过期(必须设置)
        maxElementsOnDisk====磁盘缓存中最多可以存放的元素数量,0表示无穷大 
        maxElementsInMemory="1000" 内存缓存中最多可以存放的元素数量(必须设置)
        timeToIdleSeconds="0"   导致元素过期的访问间隔(秒为单位). 0表示可以永远空闲,默认为0
        timeToLiveSeconds="600" 元素在缓存里存在的时间(秒为单位). 0 表示永远存在不过期
        overflowToDisk="false"  当缓存达到maxElementsInMemory值是,是否允许溢出到磁盘(必须设置)
        diskPersistent="false"  磁盘缓存在VM重新启动时是否保持(默认为false)
        diskExpiryThreadIntervalSeconds="100" 磁盘失效线程运行时间间隔,默认是120秒
        memoryStoreEvictionPolicy="LFU" 内存存储与释放策略.当达到maxElementsInMemory时
               共有三种策略,分别为LRU(最近最少使用)、LFU(最常用的)、FIFO(先进先出)默认使用"最近使用"策略
    -->
    <defaultCache  
        eternal="false"
        maxElementsInMemory="3000"  
        timeToIdleSeconds="3600"  
        timeToLiveSeconds="0"  
        overflowToDisk="true"  
        diskPersistent="false"  
        diskExpiryThreadIntervalSeconds="100"  
        memoryStoreEvictionPolicy="LRU"/>
 
    <cache name="propConfigCache"  
        eternal="false"
        maxElementsInMemory="3000"  
        overflowToDisk="true"
        timeToIdleSeconds="0"  
        timeToLiveSeconds="1440" 
        memoryStoreEvictionPolicy="LFU"/>
        
    <cache name="workTimeCache"  
        eternal="false"
        maxElementsInMemory="3000"  
        overflowToDisk="true"
        timeToIdleSeconds="0"  
        timeToLiveSeconds="1440" 
        memoryStoreEvictionPolicy="LFU"/>
    
    <cache name="threeInOneCache"  
        eternal="false"
        maxElementsInMemory="3000"  
        overflowToDisk="true"
        timeToIdleSeconds="0"  
        timeToLiveSeconds="1440" 
        memoryStoreEvictionPolicy="LFU"/>
    
    <cache name="transferCache"  
        eternal="false"
        maxElementsInMemory="1000"  
        overflowToDisk="true"
        timeToIdleSeconds="0"  
        timeToLiveSeconds="1440" 
        memoryStoreEvictionPolicy="LFU"/>
        
    <cache name="threeInOneFavCache"  
        eternal="false"
        maxElementsInMemory="3000"  
        overflowToDisk="true"
        timeToIdleSeconds="0"  
        timeToLiveSeconds="1440" 
        memoryStoreEvictionPolicy="LFU"/>
        
    <cache name="reserveTimeCache"  
        eternal="false"
        maxElementsInMemory="3000"  
        overflowToDisk="true"
        timeToIdleSeconds="0"  
        timeToLiveSeconds="1440" 
        memoryStoreEvictionPolicy="LFU"/>
        
    <cache name="mqServerNameCache"  
        eternal="false"
        maxElementsInMemory="3000"  
        overflowToDisk="true"
        timeToIdleSeconds="0"  
        timeToLiveSeconds="1440" 
        memoryStoreEvictionPolicy="LFU"/>
        
    <cache name="schWorkTimeCache"  
        eternal="false"
        maxElementsInMemory="3000"  
        overflowToDisk="true"
        timeToIdleSeconds="0"  
        timeToLiveSeconds="1440" 
        memoryStoreEvictionPolicy="LFU"/>
        
</ehcache>
```
@Cacheable 支持如下几个参数：
value：缓存位置名称，不能为空，如果使用EHCache，就是ehcache.xml中声明的cache的name
key：缓存的key，默认为空，既表示使用方法的参数类型及参数值作为key，支持SpEL
condition：触发条件，只有满足条件的情况才会加入缓存，默认为空，既表示全部都加入缓存，支持SpEL
```java
//将缓存保存进andCache，并使用参数中的userId加上一个字符串(这里使用方法名称)作为缓存的key   
@Cacheable(value="andCache",key="#userId + 'findById'")  
public SystemUser findById(String userId) {  
    SystemUser user = (SystemUser) dao.findById(SystemUser.class, userId);        
    return user ;         
}  
//将缓存保存进andCache，并当参数userId的长度小于32时才保存进缓存，默认使用参数值及类型作为缓存的key  
@Cacheable(value="andCache",condition="#userId.length < 32")  
public boolean isReserved(String userId) {  
    System.out.println("hello andCache"+userId);  
    return false;  
}
```
@CacheEvict 支持如下几个参数：
value：缓存位置名称，不能为空，同上
key：缓存的key，默认为空，同上
condition：触发条件，只有满足条件的情况才会清除缓存，默认为空，支持SpEL
allEntries：true表示清除value中的全部缓存，默认为false
```java
//清除掉指定key的缓存  
    @CacheEvict(value="andCache",key="#user.userId + 'findById'")  
    public void modifyUserRole(SystemUser user) {  
             System.out.println("hello andCache delete"+user.getUserId());  
    }  
      
    //清除掉全部缓存  
    @CacheEvict(value="andCache",allEntries=true)  
    public final void setReservedUsers(String[] reservedUsers) {  
        System.out.println("hello andCache deleteall");  
    }
```
一般来说，我们的更新操作只需要刷新缓存中某一个值，所以定义缓存的key值的方式就很重要，最好是能够唯一，因为这样可以准确的清除掉特定的缓存，而不会影响到其它缓存值 ，
比如我这里针对用户的操作，使用(userId+方法名称)的方式设定key值 ，当然，你也可以找到更适合自己的方式去设定。
EhCache 是一个纯Java的进程内缓存框架，具有快速、精干等特点，是Hibernate中默认的CacheProvider。
Ehcache是一种广泛使用的开源Java分布式缓存。主要面向通用缓存,Java EE和轻量级容器。它具有内存和磁盘存储，缓存加载器,缓存扩展,缓存异常处理程序,一个gzip缓存servlet过滤器,支持REST和SOAP api等特点。
Ehcache最初是由Greg Luck于2003年开始开发。2009年,该项目被Terracotta购买。软件仍然是开源,但一些新的主要功能(例如，快速可重启性之间的一致性的)只能在商业产品中使用，例如Enterprise EHCache and BigMemory。,维基媒体Foundationannounced目前使用的就是Ehcache技术。
主要的特性有：
1. 快速
2. 简单
3. 多种缓存策略
4. 缓存数据有两级：内存和磁盘，因此无需担心容量问题
5. 缓存数据会在虚拟机重启的过程中写入磁盘
6. 可以通过RMI、可插入API等方式进行分布式缓存
7. 具有缓存和缓存管理器的侦听接口
8. 支持多缓存管理器实例，以及一个实例的多个缓存区域
9. 提供Hibernate的缓存实现
