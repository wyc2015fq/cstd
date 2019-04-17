# Redis+Spring缓存实例（windows环境，附实例源码及详解） - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年04月20日 13:57:24[boonya](https://me.csdn.net/boonya)阅读数：534标签：[java																[redis																[windows](https://so.csdn.net/so/search/s.do?q=windows&t=blog)
个人分类：[Memcache/Redis](https://blog.csdn.net/boonya/article/category/5904837)





原文出处： [小宝鸽](http://blog.csdn.net/u013142781/article/details/50515320)

## 一、Redis了解

**1.1、Redis介绍：**

redis是一个key-value存储系统。和Memcached类似，它支持存储的value类型相对更多，包括string(字符串)、list(链表)、set(集合)、zset(sorted set –有序集合)和hash（哈希类型）。这些数据类型都支持push/pop、add/remove及取交集并集和差集及更丰富的操作，而且这些操作都是原子性的。在此基础上，redis支持各种不同方式的排序。与memcached一样，为了保证效率，数据都是缓存在内存中。区别的是redis会周期性的把更新的数据写入磁盘或者把修改操作写入追加的记录文件，并且在此基础上实现了master-slave(主从)同步。

Redis[数据库](http://lib.csdn.net/base/14)完全在内存中，使用磁盘仅用于持久性。相比许多键值数据存储，Redis拥有一套较为丰富的数据类型。Redis可以将数据复制到任意数量的从服务器。

**1.2、Redis优点：**

（1）异常快速：Redis的速度非常快，每秒能执行约11万集合，每秒约81000+条记录。

（2）支持丰富的数据类型：Redis支持最大多数开发人员已经知道像列表，集合，有序集合，散列数据类型。这使得它非常容易解决各种各样的问题，因为我们知道哪些问题是可以处理通过它的数据类型更好。

（3）操作都是原子性：所有Redis操作是原子的，这保证了如果两个客户端同时访问的Redis服务器将获得更新后的值。

（4）多功能实用工具：Redis是一个多实用的工具，可以在多个用例如缓存，消息，队列使用(Redis原生支持发布/订阅)，任何短暂的数据，应用程序，如Web应用程序会话，网页命中计数等。

**1.3、Redis缺点：**

（1）单线程

（2）耗内存

## 二、64位windows下Redis安装

Redis官方是不支持windows的，但是Microsoft Open Tech group 在 GitHub上开发了一个Win64的版本，下载地址：[https://github.com/MSOpenTech/redis/releases](https://github.com/MSOpenTech/redis/releases)。**注意只支持64位哈**。

小宝鸽是下载了Redis-x64-3.0.500.msi进行安装。安装过程中全部采取默认即可。

安装完成之后可能已经帮你开启了Redis对应的服务，博主的就是如此。查看资源管理如下，说明已经开启：

![图1](https://img-blog.csdn.net/20160114095940982)

已经开启了对应服务的，我们让它保持，下面例子需要用到。如果没有开启的，我们命令开启，进入Redis的安装目录（博主的是C:\Program Files\Redis），然后如下命令开启：



```
```java
redis-server  redis.windows.conf
```
```



![这里写图片描述](https://img-blog.csdn.net/20160114100006742)

OK，下面我们进行实例。

## 三、详细实例

本工程采用的环境：[Eclipse](http://res.importnew.com/eclipse) + maven + spring + junit

**3.1、添加相关依赖（spring+junit+redis依赖），pom.xml：**



```
```java
<project xmlns=
```

```java
"http://maven.apache.org/POM/4.0.0"
```

```java
xmlns:xsi=
```

```java
"http://www.w3.org/2001/XMLSchema-instance"
```

```java
xsi:schemaLocation=
```

```java
"http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd"
```

```java
>
```



```java

```

```java
<modelVersion>
```

```java
4.0
```

```java
.
```

```java
0
```

```java
</modelVersion>
```



```java

```

```java
<groupId>com.luo</groupId>
```



```java

```

```java
<artifactId>redis_project</artifactId>
```



```java

```

```java
<version>
```

```java
0.0
```

```java
.
```

```java
1
```

```java
-SNAPSHOT</version>
```





```java

```

```java
<properties>
```



```java

```

```java
<!-- spring版本号 -->
```



```java

```

```java
<spring.version>
```

```java
3.2
```

```java
.
```

```java
8
```

```java
.RELEASE</spring.version>
```



```java

```

```java
<!-- junit版本号 -->
```



```java

```

```java
<junit.version>
```

```java
4.10
```

```java
</junit.version>
```



```java

```

```java
</properties>
```





```java

```

```java
<dependencies>
```



```java

```

```java
<!-- 添加Spring依赖 -->
```



```java

```

```java
<dependency>
```



```java

```

```java
<groupId>org.springframework</groupId>
```



```java

```

```java
<artifactId>spring-core</artifactId>
```



```java

```

```java
<version>${spring.version}</version>
```



```java

```

```java
</dependency>
```



```java

```

```java
<dependency>
```



```java

```

```java
<groupId>org.springframework</groupId>
```



```java

```

```java
<artifactId>spring-webmvc</artifactId>
```



```java

```

```java
<version>${spring.version}</version>
```



```java

```

```java
</dependency>
```



```java

```

```java
<dependency>
```



```java

```

```java
<groupId>org.springframework</groupId>
```



```java

```

```java
<artifactId>spring-context</artifactId>
```



```java

```

```java
<version>${spring.version}</version>
```



```java

```

```java
</dependency>
```



```java

```

```java
<dependency>
```



```java

```

```java
<groupId>org.springframework</groupId>
```



```java

```

```java
<artifactId>spring-context-support</artifactId>
```



```java

```

```java
<version>${spring.version}</version>
```



```java

```

```java
</dependency>
```



```java

```

```java
<dependency>
```



```java

```

```java
<groupId>org.springframework</groupId>
```



```java

```

```java
<artifactId>spring-aop</artifactId>
```



```java

```

```java
<version>${spring.version}</version>
```



```java

```

```java
</dependency>
```



```java

```

```java
<dependency>
```



```java

```

```java
<groupId>org.springframework</groupId>
```



```java

```

```java
<artifactId>spring-aspects</artifactId>
```



```java

```

```java
<version>${spring.version}</version>
```



```java

```

```java
</dependency>
```



```java

```

```java
<dependency>
```



```java

```

```java
<groupId>org.springframework</groupId>
```



```java

```

```java
<artifactId>spring-tx</artifactId>
```



```java

```

```java
<version>${spring.version}</version>
```



```java

```

```java
</dependency>
```



```java

```

```java
<dependency>
```



```java

```

```java
<groupId>org.springframework</groupId>
```



```java

```

```java
<artifactId>spring-jdbc</artifactId>
```



```java

```

```java
<version>${spring.version}</version>
```



```java

```

```java
</dependency>
```



```java

```

```java
<dependency>
```



```java

```

```java
<groupId>org.springframework</groupId>
```



```java

```

```java
<artifactId>spring-web</artifactId>
```



```java

```

```java
<version>${spring.version}</version>
```



```java

```

```java
</dependency>
```





```java

```

```java
<!--单元测试依赖 -->
```



```java

```

```java
<dependency>
```



```java

```

```java
<groupId>junit</groupId>
```



```java

```

```java
<artifactId>junit</artifactId>
```



```java

```

```java
<version>${junit.version}</version>
```



```java

```

```java
<scope>test</scope>
```



```java

```

```java
</dependency>
```





```java

```

```java
<!--spring单元测试依赖 -->
```



```java

```

```java
<dependency>
```



```java

```

```java
<groupId>org.springframework</groupId>
```



```java

```

```java
<artifactId>spring-test</artifactId>
```



```java

```

```java
<version>${spring.version}</version>
```



```java

```

```java
<scope>test</scope>
```



```java

```

```java
</dependency>
```





```java

```

```java
<!-- Redis 相关依赖  -->
```



```java

```

```java
<dependency>
```



```java

```

```java
<groupId>org.springframework.data</groupId>
```



```java

```

```java
<artifactId>spring-data-redis</artifactId>
```



```java

```

```java
<version>
```

```java
1.6
```

```java
.
```

```java
1
```

```java
.RELEASE</version>
```



```java

```

```java
</dependency>
```



```java

```

```java
<dependency>
```



```java

```

```java
<groupId>redis.clients</groupId>
```



```java

```

```java
<artifactId>jedis</artifactId>
```



```java

```

```java
<version>
```

```java
2.7
```

```java
.
```

```java
3
```

```java
</version>
```



```java

```

```java
</dependency>
```





```java

```

```java
</dependencies>
```



```java
</project>
```
```



**3.2、spring配置文件application.xml：**



```
```java
<?xml version=
```

```java
"1.0"
```

```java
encoding=
```

```java
"UTF-8"
```

```java
?>
```



```java
<beans xmlns=
```

```java
"http://www.springframework.org/schema/beans"
```



```java

```

```java
xmlns:xsi=
```

```java
"http://www.w3.org/2001/XMLSchema-instance"
```



```java

```

```java
xmlns:context=
```

```java
"http://www.springframework.org/schema/context"
```



```java

```

```java
xmlns:aop=
```

```java
"http://www.springframework.org/schema/aop"
```



```java

```

```java
xsi:schemaLocation="
```





```java
http:
```

```java
//www.springframework.org/schema/beans
```







```java
http:
```

```java
//www.springframework.org/schema/beans/spring-beans-3.0.xsd
```







```java
http:
```

```java
//www.springframework.org/schema/aop
```







```java
http:
```

```java
//www.springframework.org/schema/aop/spring-aop-3.0.xsd
```







```java
http:
```

```java
//www.springframework.org/schema/context
```







```java
http:
```

```java
//www.springframework.org/schema/context/spring-context-3.0.xsd">
```





```java

```

```java
<!-- 自动扫描注解的bean -->
```



```java

```

```java
<context:component-scan base-
```

```java
package
```

```java
=
```

```java
"com.luo.service"
```

```java
/>
```





```java

```

```java
<!-- 引入properties配置文件 -->
```



```java

```

```java
<bean id=
```

```java
"propertyConfigurer"
```

```java
class
```

```java
=
```

```java
"org.springframework.beans.factory.config.PropertyPlaceholderConfigurer"
```

```java
>
```



```java

```

```java
<property name=
```

```java
"locations"
```

```java
>
```



```java

```

```java
<list>
```



```java

```

```java
<value>classpath:properties/*.properties</value>
```



```java

```

```java
<!--要是有多个配置文件，只需在这里继续添加即可 -->
```



```java

```

```java
</list>
```



```java

```

```java
</property>
```



```java

```

```java
</bean>
```





```java

```

```java
<!-- jedis 配置 -->
```



```java

```

```java
<bean id=
```

```java
"poolConfig"
```

```java
class
```

```java
=
```

```java
"redis.clients.jedis.JedisPoolConfig"
```

```java
>
```



```java

```

```java
<property name=
```

```java
"maxIdle"
```

```java
value=
```

```java
"${redis.maxIdle}"
```

```java
/>
```



```java

```

```java
<property name=
```

```java
"maxWaitMillis"
```

```java
value=
```

```java
"${redis.maxWait}"
```

```java
/>
```



```java

```

```java
<property name=
```

```java
"testOnBorrow"
```

```java
value=
```

```java
"${redis.testOnBorrow}"
```

```java
/>
```



```java

```

```java
</bean >
```





```java

```

```java
<!-- redis服务器中心 -->
```



```java

```

```java
<bean id=
```

```java
"connectionFactory"
```

```java
class
```

```java
=
```

```java
"org.springframework.data.redis.connection.jedis.JedisConnectionFactory"
```

```java
>
```



```java

```

```java
<property name=
```

```java
"poolConfig"
```

```java
ref=
```

```java
"poolConfig"
```

```java
/>
```



```java

```

```java
<property name=
```

```java
"port"
```

```java
value=
```

```java
"${redis.port}"
```

```java
/>
```



```java

```

```java
<property name=
```

```java
"hostName"
```

```java
value=
```

```java
"${redis.host}"
```

```java
/>
```



```java

```

```java
<!-- <property name=
```

```java
"password"
```

```java
value=
```

```java
"${redis.password}"
```

```java
/> -->
```



```java

```

```java
<property name=
```

```java
"timeout"
```

```java
value=
```

```java
"${redis.timeout}"
```

```java
></property>
```



```java

```

```java
</bean >
```



```java

```

```java
<bean id=
```

```java
"redisTemplate"
```

```java
class
```

```java
=
```

```java
"org.springframework.data.redis.core.RedisTemplate"
```

```java
>
```



```java

```

```java
<property name=
```

```java
"connectionFactory"
```

```java
ref=
```

```java
"connectionFactory"
```

```java
/>
```



```java

```

```java
<property name=
```

```java
"keySerializer"
```

```java
>
```



```java

```

```java
<bean
```

```java
class
```

```java
=
```

```java
"org.springframework.data.redis.serializer.StringRedisSerializer"
```

```java
/>
```



```java

```

```java
</property>
```



```java

```

```java
<property name=
```

```java
"valueSerializer"
```

```java
>
```



```java

```

```java
<bean
```

```java
class
```

```java
=
```

```java
"org.springframework.data.redis.serializer.JdkSerializationRedisSerializer"
```

```java
/>
```



```java

```

```java
</property>
```



```java

```

```java
</bean >
```





```java

```

```java
<!-- cache配置 -->
```



```java

```

```java
<bean id=
```

```java
"methodCacheInterceptor"
```

```java
class
```

```java
=
```

```java
"com.luo.redis.cache.MethodCacheInterceptor"
```

```java
>
```



```java

```

```java
<property name=
```

```java
"redisTemplate"
```

```java
ref=
```

```java
"redisTemplate"
```

```java
/>
```



```java

```

```java
</bean >
```





```java

```

```java
<!-- aop配置切点跟通知 -->
```



```java

```

```java
<bean id=
```

```java
"methodCachePointCut"
```

```java
class
```

```java
=
```

```java
"org.springframework.aop.support.RegexpMethodPointcutAdvisor"
```

```java
>
```



```java

```

```java
<property name=
```

```java
"advice"
```

```java
ref=
```

```java
"methodCacheInterceptor"
```

```java
/>
```



```java

```

```java
<property name=
```

```java
"pattern"
```

```java
value=
```

```java
".*ServiceImpl.*getTimestamp"
```

```java
/>
```



```java

```

```java
</bean>
```



```java

```

```java
<bean id=
```

```java
"redisTestService"
```

```java
class
```

```java
=
```

```java
"com.luo.service.impl.RedisTestServiceImpl"
```

```java
>
```



```java

```

```java
</bean>
```



```java

```

```java
<bean
```

```java
class
```

```java
=
```

```java
"org.springframework.aop.framework.autoproxy.DefaultAdvisorAutoProxyCreator"
```

```java
/>
```





```java
</beans>
```
```



**3.3、Redis配置参数，redis.properties：**



```
```java
#redis中心
```



```java
#绑定的主机地址
```



```java
redis.host=
```

```java
127.0
```

```java
.
```

```java
0.1
```



```java
#指定Redis监听端口，默认端口为
```

```java
6379
```



```java
redis.port=
```

```java
6379
```



```java
#授权密码（本例子没有使用）
```



```java
redis.password=
```

```java
123456
```



```java
#最大空闲数：空闲链接数大于maxIdle时，将进行回收
```



```java
redis.maxIdle=
```

```java
100
```



```java
#最大连接数：能够同时建立的“最大链接个数”
```



```java
redis.maxActive=
```

```java
300
```



```java
#最大等待时间：单位ms
```



```java
redis.maxWait=
```

```java
1000
```



```java
#使用连接时，检测连接是否成功
```



```java
redis.testOnBorrow=
```

```java
true
```



```java
#当客户端闲置多长时间后关闭连接，如果指定为
```

```java
0
```

```java
，表示关闭该功能
```



```java
redis.timeout=
```

```java
10000
```
```



**3.4、添加接口及对应实现RedisTestService.[Java](http://lib.csdn.net/base/17)和RedisTestServiceImpl.java：**



```
```java
package
```

```java
com.luo.service;
```





```java
public
```

```java
interface
```

```java
RedisTestService {
```



```java

```

```java
public
```

```java
String getTimestamp(String param);
```



```java
}
```
```





```
```java
package
```

```java
com.luo.service.impl;
```





```java
import
```

```java
org.springframework.stereotype.Service;
```



```java
import
```

```java
com.luo.service.RedisTestService;
```





```java
@Service
```



```java
public
```

```java
class
```

```java
RedisTestServiceImpl
```

```java
implements
```

```java
RedisTestService {
```





```java

```

```java
public
```

```java
String getTimestamp(String param) {
```



```java

```

```java
Long timestamp = System.currentTimeMillis();
```



```java

```

```java
return
```

```java
timestamp.toString();
```



```java

```

```java
}
```





```java
}
```
```



**3.5、本例采用spring aop切面方式进行缓存，配置已在上面spring配置文件中，对应实现为MethodCacheInterceptor.java：**



```
```java
package
```

```java
com.luo.redis.cache;
```





```java
import
```

```java
java.io.Serializable;
```



```java
import
```

```java
java.util.concurrent.TimeUnit;
```



```java
import
```

```java
org.aopalliance.intercept.MethodInterceptor;
```



```java
import
```

```java
org.aopalliance.intercept.MethodInvocation;
```



```java
import
```

```java
org.springframework.data.redis.core.RedisTemplate;
```



```java
import
```

```java
org.springframework.data.redis.core.ValueOperations;
```





```java
public
```

```java
class
```

```java
MethodCacheInterceptor
```

```java
implements
```

```java
MethodInterceptor {
```





```java

```

```java
private
```

```java
RedisTemplate<Serializable, Object> redisTemplate;
```



```java

```

```java
private
```

```java
Long defaultCacheExpireTime = 10l;
```

```java
// 缓存默认的过期时间,这里设置了10秒
```





```java

```

```java
public
```

```java
Object invoke(MethodInvocation invocation)
```

```java
throws
```

```java
Throwable {
```



```java

```

```java
Object value =
```

```java
null
```

```java
;
```





```java

```

```java
String targetName = invocation.getThis().getClass().getName();
```



```java

```

```java
String methodName = invocation.getMethod().getName();
```





```java

```

```java
Object[] arguments = invocation.getArguments();
```



```java

```

```java
String key = getCacheKey(targetName, methodName, arguments);
```





```java

```

```java
try
```

```java
{
```



```java

```

```java
// 判断是否有缓存
```



```java

```

```java
if
```

```java
(exists(key)) {
```



```java

```

```java
return
```

```java
getCache(key);
```



```java

```

```java
}
```



```java

```

```java
// 写入缓存
```



```java

```

```java
value = invocation.proceed();
```



```java

```

```java
if
```

```java
(value !=
```

```java
null
```

```java
) {
```



```java

```

```java
final
```

```java
String tkey = key;
```



```java

```

```java
final
```

```java
Object tvalue = value;
```



```java

```

```java
new
```

```java
Thread(
```

```java
new
```

```java
Runnable() {
```



```java

```

```java
public
```

```java
void
```

```java
run() {
```



```java

```

```java
setCache(tkey, tvalue, defaultCacheExpireTime);
```



```java

```

```java
}
```



```java

```

```java
}).start();
```



```java

```

```java
}
```



```java

```

```java
}
```

```java
catch
```

```java
(Exception e) {
```



```java

```

```java
e.printStackTrace();
```



```java

```

```java
if
```

```java
(value ==
```

```java
null
```

```java
) {
```



```java

```

```java
return
```

```java
invocation.proceed();
```



```java

```

```java
}
```



```java

```

```java
}
```



```java

```

```java
return
```

```java
value;
```



```java

```

```java
}
```





```java

```

```java
/**
```



```java

```

```java
* 创建缓存key
```



```java

```

```java
*
```



```java

```

```java
* @param targetName
```



```java

```

```java
* @param methodName
```



```java

```

```java
* @param arguments
```



```java

```

```java
*/
```



```java

```

```java
private
```

```java
String getCacheKey(String targetName, String methodName,
```



```java

```

```java
Object[] arguments) {
```



```java

```

```java
StringBuffer sbu =
```

```java
new
```

```java
StringBuffer();
```



```java

```

```java
sbu.append(targetName).append(
```

```java
"_"
```

```java
).append(methodName);
```



```java

```

```java
if
```

```java
((arguments !=
```

```java
null
```

```java
) && (arguments.length !=
```

```java
0
```

```java
)) {
```



```java

```

```java
for
```

```java
(
```

```java
int
```

```java
i =
```

```java
0
```

```java
; i < arguments.length; i++) {
```



```java

```

```java
sbu.append(
```

```java
"_"
```

```java
).append(arguments[i]);
```



```java

```

```java
}
```



```java

```

```java
}
```



```java

```

```java
return
```

```java
sbu.toString();
```



```java

```

```java
}
```





```java

```

```java
/**
```



```java

```

```java
* 判断缓存中是否有对应的value
```



```java

```

```java
*
```



```java

```

```java
* @param key
```



```java

```

```java
* @return
```



```java

```

```java
*/
```



```java

```

```java
public
```

```java
boolean
```

```java
exists(
```

```java
final
```

```java
String key) {
```



```java

```

```java
return
```

```java
redisTemplate.hasKey(key);
```



```java

```

```java
}
```





```java

```

```java
/**
```



```java

```

```java
* 读取缓存
```



```java

```

```java
*
```



```java

```

```java
* @param key
```



```java

```

```java
* @return
```



```java

```

```java
*/
```



```java

```

```java
public
```

```java
Object getCache(
```

```java
final
```

```java
String key) {
```



```java

```

```java
Object result =
```

```java
null
```

```java
;
```



```java

```

```java
ValueOperations<Serializable, Object> operations = redisTemplate
```



```java

```

```java
.opsForValue();
```



```java

```

```java
result = operations.get(key);
```



```java

```

```java
return
```

```java
result;
```



```java

```

```java
}
```





```java

```

```java
/**
```



```java

```

```java
* 写入缓存
```



```java

```

```java
*
```



```java

```

```java
* @param key
```



```java

```

```java
* @param value
```



```java

```

```java
* @return
```



```java

```

```java
*/
```



```java

```

```java
public
```

```java
boolean
```

```java
setCache(
```

```java
final
```

```java
String key, Object value, Long expireTime) {
```



```java

```

```java
boolean
```

```java
result =
```

```java
false
```

```java
;
```



```java

```

```java
try
```

```java
{
```



```java

```

```java
ValueOperations<Serializable, Object> operations = redisTemplate
```



```java

```

```java
.opsForValue();
```



```java

```

```java
operations.set(key, value);
```



```java

```

```java
redisTemplate.expire(key, expireTime, TimeUnit.SECONDS);
```



```java

```

```java
result =
```

```java
true
```

```java
;
```



```java

```

```java
}
```

```java
catch
```

```java
(Exception e) {
```



```java

```

```java
e.printStackTrace();
```



```java

```

```java
}
```



```java

```

```java
return
```

```java
result;
```



```java

```

```java
}
```





```java

```

```java
public
```

```java
void
```

```java
setRedisTemplate(
```



```java

```

```java
RedisTemplate<Serializable, Object> redisTemplate) {
```



```java

```

```java
this
```

```java
.redisTemplate = redisTemplate;
```



```java

```

```java
}
```



```java
}
```
```



**3.6、单元测试相关类：**



```
```java
package
```

```java
com.luo.baseTest;
```





```java
import
```

```java
org.junit.runner.RunWith;
```



```java
import
```

```java
org.springframework.test.context.ContextConfiguration;
```



```java
import
```

```java
org.springframework.test.context.junit4.AbstractJUnit4SpringContextTests;
```



```java
import
```

```java
org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
```





```java
//指定bean注入的配置文件
```



```java
@ContextConfiguration
```

```java
(locations = {
```

```java
"classpath:application.xml"
```

```java
})
```



```java
//使用标准的JUnit @RunWith注释来告诉JUnit使用Spring TestRunner
```



```java
@RunWith
```

```java
(SpringJUnit4ClassRunner.
```

```java
class
```

```java
)
```



```java
public
```

```java
class
```

```java
SpringTestCase
```

```java
extends
```

```java
AbstractJUnit4SpringContextTests {
```





```java
}
```
```





```
```java
package
```

```java
com.luo.service;
```





```java
import
```

```java
org.junit.Test;
```



```java
import
```

```java
org.springframework.beans.factory.annotation.Autowired;
```





```java
import
```

```java
com.luo.baseTest.SpringTestCase;
```





```java
public
```

```java
class
```

```java
RedisTestServiceTest
```

```java
extends
```

```java
SpringTestCase {
```





```java

```

```java
@Autowired
```



```java

```

```java
private
```

```java
RedisTestService redisTestService;
```





```java

```

```java
@Test
```



```java

```

```java
public
```

```java
void
```

```java
getTimestampTest()
```

```java
throws
```

```java
InterruptedException{
```



```java

```

```java
System.out.println(
```

```java
"第一次调用："
```

```java
+ redisTestService.getTimestamp(
```

```java
"param"
```

```java
));
```



```java

```

```java
Thread.sleep(
```

```java
2000
```

```java
);
```



```java

```

```java
System.out.println(
```

```java
"2秒之后调用："
```

```java
+ redisTestService.getTimestamp(
```

```java
"param"
```

```java
));
```



```java

```

```java
Thread.sleep(
```

```java
11000
```

```java
);
```



```java

```

```java
System.out.println(
```

```java
"再过11秒之后调用："
```

```java
+ redisTestService.getTimestamp(
```

```java
"param"
```

```java
));
```



```java

```

```java
}
```



```java
}
```
```



**3.7、运行结果：**

![这里写图片描述](https://img-blog.csdn.net/20160114101040765)

## 四、源码下载

[http://download.csdn.net/detail/u013142781/9403316](http://download.csdn.net/detail/u013142781/9403316)](https://so.csdn.net/so/search/s.do?q=redis&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)




