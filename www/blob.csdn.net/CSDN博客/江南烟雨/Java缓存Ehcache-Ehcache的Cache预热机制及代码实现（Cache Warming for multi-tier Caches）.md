# Java缓存Ehcache-Ehcache的Cache预热机制及代码实现（Cache Warming for multi-tier Caches） - 江南烟雨 - CSDN博客
2014年10月18日 15:22:00[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：17486
# Ehcache中Cache预热机制
## Cache预热机制简介
Ehcache在程序启动的时候并不会立即去加载位于磁盘上的数据到内存，而是在数据被用到的时候去加载（lazy load）。因此在cache启动的时候，其内部没有数据。如果我们想在用到这些数据之前，它们全部被装载进内存，应该怎么做？
Ehcache提供了BootstrapCacheLoader机制来解决这个问题，在Cache被激活之前，它会得到运行。并且有两种模式：同步和异步。如果是同步模式，在CacheMana启动之前，加载便会完成；如果是异步模式，在CacheManager启动的时候，加载会在后台继续，而不是等到所需数据被需要的时候。
## 具体实现
我们只需要实现接口BootstrapCacheLoader定义自己的加载器MyBootstrapCacheLoader，继承BootstrapCacheLoaderFactory实现一个具体的加载工厂MyBootstrapCacheLoaderFactory即可实现数据的预热。
MyBootstrapCacheLoader负责实现怎么将数据加载进Cache，我们可以进行个性化的实现。MyBootstrapCacheLoaderFactory是一个具体的加载工厂，负责创建加载器实例，我们需要实现一些抽象方法。
下面看具体的代码实现（Java）。
MyBootstrapCacheLoader.java：
```java
/**
 * 
 * Copyright (c) 2004-2014 All Rights Reserved.
 */
package com..test.encache;
import java.util.List;
import net.sf.ehcache.CacheException;
import net.sf.ehcache.Ehcache;
import net.sf.ehcache.Element;
import net.sf.ehcache.bootstrap.BootstrapCacheLoader;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
/**
 * 
 * @author 
 * @version $Id: CustomBootstrapCacheLoader.java, v 0.1 2014年10月18日 上午10:57:26  Exp $
 */
public class MyBootstrapCacheLoader implements BootstrapCacheLoader {
    private static final Logger logger = LoggerFactory
                                           .getLogger(MyBootstrapCacheLoaderFactory.class);
    StatesDAO                   statesDAO;
    boolean                     asynchronous;
    /** 
     * @see net.sf.ehcache.bootstrap.BootstrapCacheLoader#load(net.sf.ehcache.Ehcache)
     */
    public void load(Ehcache cache) throws CacheException {
        logger.info("load your cache with whatever you want....");
        List keys = cache.getKeys();
        for (int i = 0; i < keys.size(); i++) {
            logger.info("keys->" + keys.get(i));
        }
        try {
            List<String> dataList = getStatesDAO().findAllStates();
            cache.put(new Element(CacheConstants.KEY_ARRAY[0], dataList.get(0)));
            cache.put(new Element(CacheConstants.KEY_ARRAY[1], dataList.get(1)));
        } catch (Exception e) {
            // TODO Auto-generated catch block 
            e.printStackTrace();
        }
        logger.info("load end....");
    }
    /** 
     * @see net.sf.ehcache.bootstrap.BootstrapCacheLoader#isAsynchronous()
     */
    public boolean isAsynchronous() {
        return asynchronous;
    }
    /** 
     * @see java.lang.Object#clone()
     */
    @Override
    public Object clone() throws CloneNotSupportedException {
        return super.clone();
    }
    public StatesDAO getStatesDAO() {
        return statesDAO;
    }
    public void setStatesDAO(StatesDAO statesDAO) {
        this.statesDAO = statesDAO;
    }
    /**
     * Setter method for property <tt>asynchronous</tt>.
     * 
     * @param asynchronous value to be assigned to property asynchronous
     */
    public void setAsynchronous(boolean asynchronous) {
        this.asynchronous = asynchronous;
    }
}
```
MyBootstrapCacheLoaderFactory.java
```java
/**
 * 
 * Copyright (c) 2004-2014 All Rights Reserved.
 */
package com.test.encache;
import java.util.Properties;
import net.sf.ehcache.bootstrap.BootstrapCacheLoader;
import net.sf.ehcache.bootstrap.BootstrapCacheLoaderFactory;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
/**
 * bootstrap cache loader
 * 
 * @author 
 * @version $Id: MyBootstrapCacheLoaderFactory.java, v 0.1 2014年10月17日 下午8:02:55  Exp $
 */
public class MyBootstrapCacheLoaderFactory extends BootstrapCacheLoaderFactory {
    private final String ASYNCHRONOUS_PROPERTY_KEY = "ASYNCHRONOUS";
    @Autowired
    private StatesDAO    statesDAO;
    public MyBootstrapCacheLoaderFactory() {
        super();
        // TODO Auto-generated constructor stub 
    }
    private static final Logger logger = LoggerFactory
                                           .getLogger(MyBootstrapCacheLoaderFactory.class);
    @Override
    public BootstrapCacheLoader createBootstrapCacheLoader(Properties properties) {
        logger.info("MyBootstrapCacheLoaderFactory : create a BootstrapCacheLoader");
        MyBootstrapCacheLoader loader = new MyBootstrapCacheLoader();
        statesDAO = new StatesDAO();
        loader.setStatesDAO(statesDAO);
        loader.setAsynchronous(getAsyncFromProperty(properties));
        return loader;
    }
    private boolean getAsyncFromProperty(Properties properties) {
        String asynchronous = properties.getProperty(ASYNCHRONOUS_PROPERTY_KEY);
        return Boolean.valueOf(asynchronous);
    }
}
```
使用了Cache的读取磁盘数据的方法在StatesDAO类中，对此我们只是进行了模拟，从数据库中读取数据。
StatesDAO.java
```java
/**
 * 
 * Copyright (c) 2004-2014 All Rights Reserved.
 */
package com.test.encache;
import java.util.ArrayList;
import java.util.List;
import com.googlecode.ehcache.annotations.Cacheable;
/**
 * 
 * @author 
 * @version $Id: StatesDAO.java, v 0.1 2014年10月17日 下午8:07:05  Exp $
 */
public class StatesDAO {
    //annotation based caching and the name of cache should be defined in ehcache.xml. 
    @Cacheable(cacheName = "stateCache")
    public List<String> findAllStates() {
        List<String> dataList = new ArrayList<String>();
        //your call to database that returns a list of objects  
        dataList.add("value1");
        dataList.add("value2");
        return dataList;
    }
}
```
Cache配置myehcache.xml：
```
<ehcache>
<diskStore path="D://localcache"/>
<cache 
	name="stateCache" 
	maxEntriesLocalHeap="10000" 
	maxEntriesLocalDisk="1000" 
	eternal="false" 
	diskSpoolBufferSizeMB="20" 
	timeToIdleSeconds="300" 
	timeToLiveSeconds="600" 
	memoryStoreEvictionPolicy="LFU" 
	transactionalMode="off">
	<bootstrapCacheLoaderFactory class="com.test.encache.MyBootstrapCacheLoaderFactory"  properties="ASYNCHRONOUS=true"/>
</cache>
<cache 
	name="stateCache2" 
	maxEntriesLocalHeap="10000" 
	maxEntriesLocalDisk="1000" 
	eternal="false" 
	diskSpoolBufferSizeMB="20" 
	timeToIdleSeconds="300" 
	timeToLiveSeconds="600" 
	memoryStoreEvictionPolicy="LFU" 
	transactionalMode="off">
	<bootstrapCacheLoaderFactory class="com.test.encache.MyBootstrapCacheLoaderFactory"  properties="ASYNCHRONOUS=false"/>
</cache>
</ehcache>
```
CacheManager配置ApplicationContext.xml：
```
<?xml version="1.0" encoding="UTF-8" ?>
<beans xmlns="http://www.springframework.org/schema/beans"
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:p="http://www.springframework.org/schema/p"
	xmlns:context="http://www.springframework.org/schema/context"
	xmlns:aop="http://www.springframework.org/schema/aop" xmlns:tx="http://www.springframework.org/schema/tx"
	xmlns:ehcache="http://ehcache-spring-annotations.googlecode.com/svn/schema/ehcache-spring"
	xsi:schemaLocation="http://www.springframework.org/schema/beans 
       http://www.springframework.org/schema/beans/spring-beans-3.0.xsd
       http://ehcache-spring-annotations.googlecode.com/svn/schema/ehcache-spring
     http://ehcache-spring-annotations.googlecode.com/svn/schema/ehcache-spring/ehcache-spring-1.1.xsd"
     default-autowire="byType">
    
	<ehcache:annotation-driven cache-manager="ehCacheManager" /> 
	<bean id="ehCacheManager" 
		class="org.springframework.cache.ehcache.EhCacheManagerFactoryBean"> 
		<property name="configLocation"> 
			<value>src/config/myehcache.xml</value> 
		</property>
	</bean> 
    
</beans>
```
最后是测试代码：BootstrapCacheLoaderTest.java：
```java
/**
 * 
 * Copyright (c) 2004-2014 All Rights Reserved.
 */
package com.test.encache;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Properties;
import net.sf.ehcache.Cache;
import net.sf.ehcache.CacheManager;
import net.sf.ehcache.Element;
import org.apache.log4j.PropertyConfigurator;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.ApplicationContext;
import org.springframework.context.support.FileSystemXmlApplicationContext;
/**
 * 
 * @author 
 * @version $Id: BootstrapCacheLoader.java, v 0.1 2014年10月18日 上午11:31:06 Exp $
 */
public class BootstrapCacheLoaderTest {
    private static String       log4jFileName      = "src/config/log4j.properties";
    private static String       xmlFileName        = "src/config/ApplicationContext.xml";
    private static String       ehcacheXmlFileName = "src/config/myehcache.xml";
    private static final Logger logger             = LoggerFactory
                                                       .getLogger(BootstrapCacheLoaderTest.class);
    private static CacheManager ehCacheManager;
    public static void main(String[] args) {
        configProperty();
        xmlLoad(ehcacheXmlFileName);
        String[] cacheNamesStrings = ehCacheManager.getCacheNames();
        logger.info("the number of caches in ehCacheManager : " + cacheNamesStrings.length);
        Cache cache = ehCacheManager.getCache(CacheConstants.CACHE_NAME1);
        Element element = cache.get(CacheConstants.KEY_ARRAY[0]);
        logger.info("the element of key  " + CacheConstants.KEY_ARRAY[0] + " is " + element);
    }
    /**
     * config properties
     * 
     */
    private static void configProperty() {
        Properties properties = new Properties();
        FileInputStream istream;
        try {
            istream = new FileInputStream(log4jFileName);
            properties.load(istream);
            istream.close();
        } catch (FileNotFoundException e) {
            logger.error("File not found", e);
        } catch (IOException e) {
            logger.error("load file erroe", e);
        } finally {
        }
        //properties.setProperty("log4j.appender.file.File",logFile); 
        PropertyConfigurator.configure(properties);
        logger.info("config properties success.");
    }
    private static void xmlLoad(String fileName) {
        ApplicationContext ctx = new FileSystemXmlApplicationContext(xmlFileName);
        ehCacheManager = (CacheManager) ctx.getBean("ehCacheManager");
    }
}
```
输出结果：
2014-10-18 15:17:45 INFO  BootstrapCacheLoaderTest:71 - config properties success.
2014-10-18 15:17:45 INFO  FileSystemXmlApplicationContext:513 - Refreshing org.springframework.context.support.FileSystemXmlApplicationContext@687b6889: startup date [Sat Oct 18 15:17:45 CST 2014]; root of context hierarchy
2014-10-18 15:17:45 INFO  XmlBeanDefinitionReader:316 - Loading XML bean definitions from file [D:\code\test\encache\src\config\ApplicationContext.xml]
2014-10-18 15:17:45 INFO  EhCacheManagerFactoryBean:136 - Initializing EhCache CacheManager
2014-10-18 15:17:46 INFO  MyBootstrapCacheLoaderFactory:38 - MyBootstrapCacheLoaderFactory : create a BootstrapCacheLoader
2014-10-18 15:17:46 INFO  MyBootstrapCacheLoaderFactory:38 - MyBootstrapCacheLoaderFactory : create a BootstrapCacheLoader
2014-10-18 15:17:46 INFO  MyBootstrapCacheLoaderFactory:34 - load your cache with whatever you want....
2014-10-18 15:17:46 INFO  MyBootstrapCacheLoaderFactory:51 - load end....
2014-10-18 15:17:46 INFO  MyBootstrapCacheLoaderFactory:34 - load your cache with whatever you want....
2014-10-18 15:17:46 INFO  MyBootstrapCacheLoaderFactory:51 - load end....
2014-10-18 15:17:46 INFO  BootstrapCacheLoaderTest:43 - the number of caches in ehCacheManager : 2
2014-10-18 15:17:46 INFO  BootstrapCacheLoaderTest:46 - the element of key  KEY1 is [ key = KEY1, value=value1, version=1, hitCount=1, CreationTime = 1413616666238, LastAccessTime = 1413616666302 ]
## 参考资料
[Ehcache关于预热机制的官方文档](http://ehcache.org/documentation/2.8/configuration/configuration#cache-warming-for-multi-tier-caches)
[Load EhCache diskstore content into memory](http://stackoverflow.com/questions/10448274/load-ehcache-diskstore-content-into-memory)
[How to load data to Ehcache when the application starts](http://forums.terracotta.org/forums/posts/list/4335.page)
