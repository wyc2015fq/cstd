# Java缓存Ehcache-核心类和方法介绍及代码实例 - 江南烟雨 - CSDN博客
2014年09月27日 15:52:18[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：34444
# Ehcache中核心类和方法
EhCache里面有一个CacheManager类型，它负责管理cache。Cache里面存储着Element对象，Element必须是key-value对。Cache是实际物理实现的，在内存中或者磁盘。这些组件的逻辑表示就是下面即将要讨论的类。他们的方法提供了可编程的访问方式。
## CacheManager
负责Cache的创建、访问、移除。
### CacheManager创建
CacheManager支持两种创建模式：单例（Singleton mode）和实例（InstanceMode）。
在2.5之前的版本中，在同一个JVM中允许存在任意数量相同名字的CacheManager。每调用new CacheManager(...)一次，就会产生一个新的CacheManager实例，而不管已经存在多少个。调用CacheManager.create(...)，则返回的是已经存在的那个配置对应的单例CacheManager，如果不存在，则创建一个。
2.5之后的版本，不允许在同一个JVM内存在多个具有相同名字的CacheManager。创建非单例实例的CacheManager()构造函数可能会打破这一规则，但是会抛出NPE异常。如果你的代码要在同一个JVM创建多个同名的实例，请使用静态方法CacheManager.create()，总是返回对应名的CacheManager（如果已经存在），否则创建一个。
事实上，我们可以直接利用Spring中的EhCacheManagerFactoryBean[spring2.5.4]来帮我们完成CacheManager的创建，看看它的具体创建方式：
```java
if (this.shared) {
			// Shared CacheManager singleton at the VM level.
			if (this.configLocation != null) {
				this.cacheManager = CacheManager.create(this.configLocation.getInputStream());
			}
			else {
				this.cacheManager = CacheManager.create();
			}
		}
		else {
			// Independent CacheManager instance (the default).
			if (this.configLocation != null) {
				this.cacheManager = new CacheManager(this.configLocation.getInputStream());
			}
			else {
				this.cacheManager = new CacheManager();
			}
		}
		if (this.cacheManagerName != null) {
			this.cacheManager.setName(this.cacheManagerName);
		}
```
EhCache2.5.2及其以上版本的创建方法归纳如下：
- CacheManager.newInstance(Configuration configuration) ：创建一个新的CacheManager 对象或者返回已经存在的对应配置中名字的CacheManager
- CacheManager.create()：创建一个新的默认配置的单例CacheManager ，或者返回一个已经存在的单例。
- CacheManager.create(Configuration configuration)，创建一个对应传入配置文件中名字的单例CacheManager，或者返回已经存在的单例CacheManager。
- new CacheManager(Configuration configuration)，创建一个新的CacheManager，或者如果对应配置的CacheManager已经存在或配置参数为空，抛出异常。
在Instance Mode中，如果Cache均使用MemoryStore，则没有什么特别需要注意的。但是如果使用了DIskStore，那么每个CacheManager必须具有不同的diskStore路径。当一个新的CacheManager被创建的时候，需要检查没有别的CacheManager使用同样的DiskStore路径。如果有，就会抛出异常CacheException。如果CacheManager是集群中一部分，那么其监听端口必须唯一。
**Singletonmode和Instance Mode可以混合使用，而不会产生冲突。**
## Ehcache
所有的cache都实现了接口Ehcache。每个cache都有名字和属性，且包含Element。
Ehcache中的cache相当于其他缓存系统中的一块缓存区域。
### Element
Element是存放于cache中的原子单位。它有一个key、一个value以及关于访问的记录。Element被放进到cache或者从cache移除。他们也可能会由于过期被移除，这依赖于配置。
## 使用实例
下面给出了一个使用Ehcache的实际例子。
首先新建一个maven java工程，在pom.xml中添加Ehcache依赖。
```
<!-- Ehcache -->
    <dependency>
        <groupId>net.sf.ehcache</groupId>
        <artifactId>ehcache</artifactId>
        <version>2.8.3</version>
    </dependency>
```
下面是java代码。代码实现的功能非常简单，即创建CacheManager，往里面存放一个Cache，然后往cache里面存数据和取数据，目的是展示Ehcache的基本使用。
```java
/**
 * XXX.com Inc.
 * Copyright (c) 2004-2014 All Rights Reserved.
 */
package com.test.encache;
import net.sf.ehcache.Cache;
import net.sf.ehcache.CacheManager;
import net.sf.ehcache.Element;
/**
 * 
 * @author XXX
 * @version $Id: EncacheTest.java, v 0.1 2014年8月8日 下午5:30:03 XXX Exp $
 */
public class EncacheTest {
    //一些配置参数
    //private final static String configFileName      = "ehcache.xml";
    //private final static int    maxEntriesLocalHeap = 1000;
    private static CacheManager cacheManager;
    static String               cacheName = "cache1";
    public static void main(String[] args) {
        ehcacheSetUp();
        ehcacheUse();
    }
    private static void ehcacheSetUp() {
        cacheManager = CacheManager.create();
        //CacheConfiguration configuration = new CacheConfiguration(configFileName,
        //    maxEntriesLocalHeap);
        //Cache cache = new Cache(configuration);
        cacheManager.addCache(cacheName);
    }
    private static void ehcacheUse() {
        Cache cache1 = cacheManager.getCache(cacheName);
        String key = "key1";
        String value = "value1";
        writeSomeData(cache1, key, value);
        Element element = readSomeData(cache1, key, value);
        System.out.println(element);
    }
    private static void writeSomeData(Cache cache, String key, String value) {
        cache.put(new Element(key, value));
    }
    private static Element readSomeData(Cache cache, String key, String value) {
        return cache.get(key);
    }
}
```
程序输出：
```
SLF4J: Failed to load class "org.slf4j.impl.StaticLoggerBinder".
SLF4J: Defaulting to no-operation (NOP) logger implementation
SLF4J: See http://www.slf4j.org/codes.html#StaticLoggerBinder for further details.
[ key = key1, value=value1, version=1, hitCount=1, CreationTime = 1411807398768, LastAccessTime = 1411807398771 ]
```
其中的错误信息是因为没有配置日志相关的SLF4J所致。
下面我们要配置日志。首先在pom.xml中添加依赖：
```
<!-- SLF4J -->
    <dependency>
        <groupId>org.slf4j</groupId>
        <artifactId>slf4j-log4j12</artifactId>
        <version>1.6.1</version>
	</dependency>
```
然后建立log4j的配置文件log4j.properties：
```
# Root logger option
log4j.rootLogger=INFO, file, stdout
log4j.logger.com.test.encache.EncacheTest=INFO,file
 
# Direct log messages to a log file
log4j.appender.file=org.apache.log4j.DailyRollingFileAppender
log4j.appender.file.File=C:\\logging.log
log4j.appender.file.layout=org.apache.log4j.PatternLayout
log4j.appender.file.layout.ConversionPattern=%d{yyyy-MM-dd HH:mm:ss} %-5p %c{1}:%L - %m%n
 
# Direct log messages to stdout
log4j.appender.stdout=org.apache.log4j.ConsoleAppender
log4j.appender.stdout.Target=System.out
log4j.appender.stdout.layout=org.apache.log4j.PatternLayout
log4j.appender.stdout.layout.ConversionPattern=%d{yyyy-MM-dd HH:mm:ss} %-5p %c{1}:%L - %m%n
```
并且将这个文件放置到工程的classpath下，在这里我建立的是用Eclipse创建的maven工程，将其放置在工程主目录下的\target\classes文件夹下。
然后在代码中添加logger的初始化代码：
```java
private static final Logger logger    = LoggerFactory.getLogger(EncacheTest.class);
```
然后就可以使用了：
```java
logger.info("Setup ehcache");
```
输出：
```
2014-09-27 17:22:45 INFO  EncacheTest:35 - Setup ehcache
2014-09-27 17:22:45 WARN  ConfigurationFactory:136 - No configuration found. Configuring ehcache from ehcache-failsafe.xml  found in the classpath: jar:file:/D:/MavenRepo/net/sf/ehcache/ehcache/2.8.3/ehcache-2.8.3.jar!/ehcache-failsafe.xml
2014-09-27 17:22:46 WARN  DiskStorePathManager:162 - diskStorePath 'C:\Users\xxxx\AppData\Local\Temp' is already used by an existing CacheManager either in the same VM or in a different process.
The diskStore path for this CacheManager will be set to C:\Users\xxxx\AppData\Local\Temp\ehcache_auto_created7989392067865891865diskstore.
To avoid this warning consider using the CacheManager factory methods to create a singleton CacheManager or specifying a separate ehcache configuration (ehcache.xml) for each CacheManager instance.
[ key = key1, value=value1, version=1, hitCount=1, CreationTime = 1411809766273, LastAccessTime = 1411809766276 ]
```

## 参考资料：
源文档 <[http://ehcache.org/documentation/get-started/key-classes-methods](http://ehcache.org/documentation/get-started/key-classes-methods)> 
