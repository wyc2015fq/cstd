# spring 整合 redis，以及spring的RedisTemplate如何使用 - z69183787的专栏 - CSDN博客
2017年07月03日 17:46:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2080
需要的jar包 
[spring](http://lib.csdn.net/base/javaee)-data-[Redis](http://lib.csdn.net/base/redis)-1.6.2.RELEASE.jar 
jedis-2.7.2.jar（依赖 commons-pool2-2.3.jar） 
commons-pool2-2.3.jar
spring-[redis](http://lib.csdn.net/base/redis).xml 配置文件
```xml
<beans xmlns="http://www.springframework.org/schema/beans"
    xmlns:context="http://www.springframework.org/schema/context" xmlns:p="http://www.springframework.org/schema/p"
    xmlns:mvc="http://www.springframework.org/schema/mvc" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xmlns:tx="http://www.springframework.org/schema/tx" xmlns:util="http://www.springframework.org/schema/util"
    xmlns:aop="http://www.springframework.org/schema/aop"
    xsi:schemaLocation="http://www.springframework.org/schema/beans
      http://www.springframework.org/schema/beans/spring-beans-3.0.xsd
      http://www.springframework.org/schema/context
      http://www.springframework.org/schema/context/spring-context-3.0.xsd
      http://www.springframework.org/schema/mvc
      http://www.springframework.org/schema/mvc/spring-mvc-3.0.xsd
        http://www.springframework.org/schema/tx
        http://www.springframework.org/schema/tx/spring-tx-3.0.xsd
        http://www.springframework.org/schema/aop
    http://www.springframework.org/schema/aop/spring-aop-3.0.xsd
        http://www.springframework.org/schema/util 
      http://www.springframework.org/schema/util/spring-util-3.0.xsd">
<!--[redis-JedisPoolConfig配置](http://blog.csdn.net/liang_love_java/article/details/50510753)-->
<!--    jedis-2.7.2.jar 依赖jar包 commons-pool2-2.3.jar 
        jedis基于 commons-pool2-2.3.jar 自己实现了一个资源池。
        配置参数 详见 http://blog.csdn.net/liang_love_java/article/details/50510753
-->
    <bean id="jedisPoolConfig" class="redis.clients.jedis.JedisPoolConfig"> 
        <property name="maxIdle" value="1" /> 
        <property name="maxTotal" value="5" /> 
        <property name="blockWhenExhausted" value="true" /> 
        <property name="maxWaitMillis" value="30000" /> 
        <property name="testOnBorrow" value="true" />  
    </bean> 
    <bean id="jedisConnectionFactory" class="org.springframework.data.redis.connection.jedis.JedisConnectionFactory"> 
        <property name="hostName" value="10.1.8.200" /> 
        <property name="port" value="6379"/> 
        <property name="poolConfig" ref="jedisPoolConfig" /> 
        <property name="usePool" value="true"/> 
    </bean> 
    <bean id="redisTemplate" class="org.springframework.data.redis.core.RedisTemplate">   
        <property name="connectionFactory"   ref="jedisConnectionFactory" />   
        <property name="keySerializer">   
            <bean class="org.springframework.data.redis.serializer.StringRedisSerializer" />   
        </property>      
        <property name="valueSerializer">   
            <bean class="org.springframework.data.redis.serializer.JdkSerializationRedisSerializer" />   
        </property>   
        <property name="hashKeySerializer">     
           <bean class="org.springframework.data.redis.serializer.StringRedisSerializer"/>     
        </property>   
        <property name="hashValueSerializer">   
           <bean class="org.springframework.data.redis.serializer.JdkSerializationRedisSerializer"/>     
        </property> 
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
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
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
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
[测试](http://lib.csdn.net/base/softwaretest)代码
```
import java.util.HashMap;
import java.util.Map;
import org.springframework.context.support.ClassPathXmlApplicationContext;
import org.springframework.data.redis.core.HashOperations;
import org.springframework.data.redis.core.ListOperations;
import org.springframework.data.redis.core.RedisTemplate;
import org.springframework.data.redis.core.ValueOperations;
public static void main(String[] args) {
        ClassPathXmlApplicationContext appCtx = new ClassPathXmlApplicationContext("spring-redis.xml");
        final RedisTemplate<String, Object> redisTemplate = appCtx.getBean("redisTemplate",RedisTemplate.class);
        //添加一个 key 
        ValueOperations<String, Object> value = redisTemplate.opsForValue();
        value.set("lp", "hello word");
        //获取 这个 key 的值
        System.out.println(value.get("lp"));
        //添加 一个 hash集合
        HashOperations<String, Object, Object>  hash = redisTemplate.opsForHash();
        Map<String,Object> map = new HashMap<String,Object>();
        map.put("name", "lp");
        map.put("age", "26");
        hash.putAll("lpMap", map);
        //获取 map
        System.out.println(hash.entries("lpMap"));
        //添加 一个 list 列表
        ListOperations<String, Object> list = redisTemplate.opsForList();
        list.rightPush("lpList", "lp");
        list.rightPush("lpList", "26");
        //输出 list
        System.out.println(list.range("lpList", 0, 1));
        //添加 一个 set 集合
        SetOperations<String, Object> set = redisTemplate.opsForSet();
        set.add("lpSet", "lp");
        set.add("lpSet", "26");
        set.add("lpSet", "178cm");
        //输出 set 集合
        System.out.println(set.members("lpSet"));
        //添加有序的 set 集合
        ZSetOperations<String, Object> zset = redisTemplate.opsForZSet();
        zset.add("lpZset", "lp", 0);
        zset.add("lpZset", "26", 1);
        zset.add("lpZset", "178cm", 2);
        //输出有序 set 集合
        System.out.println(zset.rangeByScore("lpZset", 0, 2));
    }
```
