# RedisTemplate访问Redis数据结构（前言） - z69183787的专栏 - CSDN博客
2018年07月17日 11:33:28[OkidoGreen](https://me.csdn.net/z69183787)阅读数：129
### Redis五种基本数据结构
redis提供键值对的形式对数据进行存储。支持五种数据类型：String（字符串），List（链表），Hash（散列），Set（无序集合），ZSet（有序集合）。下面是网上对其数据结构简单的归纳比较好的，如下：
|结构类型|结构存储的值|结构的读写能力|
|----|----|----|
|String|可以是字符串、整数或者浮点数|对整个字符串或者字符串的其中一部分执行操作；对象和浮点数执行 自增(increment)或者自减(decrement)|
|List|一个链表，链表上的每个节点都包含了一个字符串|从链表的两端推入或者弹出元素；根据偏移量对链表进行修剪(trim)；读取单个或者多个元素；根据值来查找或者移除元素|
|Hash|包含键值对的无序散列表|添加、获取、移除单个键值对；获取所有键值对|
|Set|包含字符串的无序收集器(unorderedcollection)，并且被包含的每个字符串都是独一无二的、各不相同|添加、获取、移除单个元素；检查一个元素是否存在于某个集合中；计算交集、并集、差集；从集合里卖弄随机获取元素|
|ZSet|字符串成员(member)与浮点数分值(score)之间的有序映射，元素的排列顺序由分值的大小决定|添加、获取、删除单个元素；根据分值范围(range)或者成员来获取元素|
### Spring-data-Redis简介
对于JAVA语言，我们之前使用Jedis对redis进行基本的指令操作，随着Spring对Jedis进行了很好的封装之后，使用Spring-data-redis包对redis的操作变得更加简单和方便。而Spring-data-Redis则是通过RedisTemplate对象来操作Redis的五种数据结构。
### 如何引入Spring-data-Redis
1.导入jar包：spring-data-redis-1.8.7.RELEASE.jar 和 jedis-2.9.0.jar
```
<dependency>
    <groupId>org.springframework.data</groupId>
    <artifactId>spring-data-redis</artifactId>
    <version>1.8.7.RELEASE</version>
</dependency>
<dependency>
    <groupId>redis.clients</groupId>
    <artifactId>jedis</artifactId>
    <version>2.9.0</version>
</dependency>
```
2.配置文件（SpringBoot方式暂不介绍）
redis.properties
```
redis.host=192.168.132.128
redis.port=10000
redis.password=123456
redis.minIdle=50
redis.maxIdle=300
redis.maxActive=600
redis.maxWait=1000
redis.testOnBorrow=true
```
- 
applicationContext.xml
```
<bean id="propertyConfigurer" class="org.springframework.beans.factory.config.PropertyPlaceholderConfigurer">
    <property name="locations">
        <list>
            <!-- 引入小配置文件-->
            <value>classpath:redis.properties</value>
        </list>
    </property>
</bean>
<!-- 连接池 ,本质是对GenericObjectPoolConfig的属性的设置-->
<bean id="poolConfig" class="redis.clients.jedis.JedisPoolConfig">  
    <property name="minIdle" value="${redis.minIdle}" />  
    <property name="maxIdle" value="${redis.maxIdle}" />    
    <property name="maxTotal" value="${redis.maxActive}" />    
    <property name="maxWaitMillis" value="${redis.maxWait}" />    
    <property name="testOnBorrow" value="${redis.testOnBorrow}" />    
</bean>    
<!-- REDIS连接工厂 -->
<bean id="jedisConnFactory" class="org.springframework.data.redis.connection.jedis.JedisConnectionFactory">  
    <!-- 基础连接参数 -->
    <property name="hostName" value="${redis.host}" />  
    <property name="port" value="${redis.port}" />  
    <property name="password" value="${redis.password}" />  
    <!-- 是否启用连接池 -->
    <property name="usePool" value="true" />  
    <property name="poolConfig" ref="poolConfig" />  
</bean>   
<!-- 对String类型处理的RedisTemplate -->
<bean id="stringRedisTemplate" class="org.springframework.data.redis.core.StringRedisTemplate">  
    <property name="connectionFactory" ref="jedisConnFactory" />  
    <property name="keySerializer">
        <bean class="org.springframework.data.redis.serializer.StringRedisSerializer" />
    </property>
    <property name="valueSerializer">
        <bean class="org.springframework.data.redis.serializer.StringRedisSerializer" />
    </property>
    <property name="hashKeySerializer">
        <bean class="org.springframework.data.redis.serializer.StringRedisSerializer" />
    </property>
    <property name="hashValueSerializer">
        <bean class="org.springframework.data.redis.serializer.StringRedisSerializer" />
    </property> 
</bean>
 <!-- 对LIST,SET,ZSET,HASH等类型的处理RedisTemplate -->
<bean id="redisTemplate" class="org.springframework.data.redis.core.RedisTemplate">  
    <property name="connectionFactory" ref="jedisConnFactory"/>  
    <property name="keySerializer">
        <bean class="org.springframework.data.redis.serializer.StringRedisSerializer"/>
    </property>
    <property name="hashKeySerializer">
        <bean class="org.springframework.data.redis.serializer.StringRedisSerializer"/>
    </property>
    <property name="valueSerializer">
        <!-- 对象序列化方案 -->
        <bean class="org.springframework.data.redis.serializer.GenericJackson2JsonRedisSerializer"/>
    </property>
    <property name="hashValueSerializer">
        <bean class="org.springframework.data.redis.serializer.GenericJackson2JsonRedisSerializer"/>
    </property>
</bean>
```
3.注意
推荐使用GenericJackson2JsonRedisSerializer，而不是Jackson2JsonRedisSerializer，因为GenericJackson2JsonRedisSerializer提供了很好的对泛型的支持，而使用Jackson2JsonRedisSerializer对不同对象进行操作时都需要手动set序列化方案，不能直接集成到配置文件中将其直接托管给spring工厂。当然，我们可以自定义序列化方案，同时也可以使用spring-data-redis集成好的序列化方案，例如集成号称速度最快的fastjson序列化方案，下面提供一个fastjson的Serializer（暂时没有集成对泛型纳入工厂方案的支持）。
```
package util;
import java.nio.charset.Charset;
import org.springframework.data.redis.serializer.RedisSerializer;
import org.springframework.data.redis.serializer.SerializationException;
import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.serializer.SerializerFeature;
/**
 * FASTJSON序列化工具
 * @author LiuChengxiang
 * @time 2017年9月19日上午9:30:27
 * 
 * @param <T>
 */
public class FastJson2JsonRedisSerializer<T> implements RedisSerializer<T> {
    public static final Charset DEFAULT_CHARSET = Charset.forName("UTF-8");
    private Class<T> clazz;
    public FastJson2JsonRedisSerializer(Class<T> clazz){
        super();
        this.clazz = clazz;
    }
    @Override
    public byte[] serialize(T t) throws SerializationException {
        if (t == null) {
            return new byte[0];
        }
        return JSON.toJSONString(t, SerializerFeature.WriteClassName).getBytes(DEFAULT_CHARSET);
    }
    @Override
    public T deserialize(byte[] bytes) throws SerializationException {
        if (bytes == null || bytes.length <= 0) {
            return null;
        }
        String str = new String(bytes, DEFAULT_CHARSET);
        return (T)JSON.parseObject(str,clazz);
    }
}
```
