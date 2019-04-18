# Spring+Redis Cluster3.0.x - z69183787的专栏 - CSDN博客
2017年07月03日 17:11:30[OkidoGreen](https://me.csdn.net/z69183787)阅读数：470
个人分类：[Spring-RedisTemplate](https://blog.csdn.net/z69183787/article/category/7004300)
## Redis 3.0集群搭建方法：
[http://hot66hot.iteye.com/blog/2050676](http://hot66hot.iteye.com/blog/2050676)
## Redis 3.0.x使用方法
### 1.Maven依赖
```xml
<dependency>
<groupId>redis.clients</groupId>
<artifactId>jedis</artifactId>
<version>2.7.2</version>
</dependency>
```
### 2.Spring配置
```xml
<bean name="genericObjectPoolConfig" class="org.apache.commons.pool2.impl.GenericObjectPoolConfig" >  
        <property name="maxWaitMillis" value="-1" />  
        <property name="maxTotal" value="1000" />  
        <property name="minIdle" value="8" />  
        <property name="maxIdle" value="100" />  
</bean>  
  
<bean id="jedisCluster" class="xxx.JedisClusterFactory">  
    <property name="addressConfig">  
        <value>classpath:connect-redis.properties</value>  
    </property>  
    <property name="addressKeyPrefix" value="address" />   <!--  属性文件里  key的前缀 -->  
      
    <property name="timeout" value="300000" />  
    <property name="maxRedirections" value="6" />  
    <property name="genericObjectPoolConfig" ref="genericObjectPoolConfig" />  
</bean>
```
### 3.增加connect-redis.properties  配置文件
这里配置了6个节点
```
address1=172.16.23.27:6379  
address2=172.16.23.27:6380  
address3=172.16.23.27:6381  
address4=172.16.23.27:6382  
address5=172.16.23.27:6383  
address6=172.16.23.27:6384
```
###  4.增加实现的Java类
```java
```java
import java.util.HashSet;
import java.util.Properties;
import java.util.Set;
import java.util.regex.Pattern;
import org.apache.commons.pool2.impl.GenericObjectPoolConfig;
import org.springframework.beans.factory.FactoryBean;
import org.springframework.beans.factory.InitializingBean;
import org.springframework.core.io.Resource;
import redis.clients.jedis.HostAndPort;
import redis.clients.jedis.JedisCluster;
public class JedisClusterFactory implements FactoryBean<JedisCluster>, InitializingBean {
	private Resource addressConfig;
	private String addressKeyPrefix ;
	private JedisCluster jedisCluster;
	private Integer timeout;
	private Integer maxRedirections;
	private GenericObjectPoolConfig genericObjectPoolConfig;
	
	private Pattern p = Pattern.compile("^.+[:]\\d{1,5}\\s*$");
	@Override
	public JedisCluster getObject() throws Exception {
		return jedisCluster;
	}
	@Override
	public Class<? extends JedisCluster> getObjectType() {
		return (this.jedisCluster != null ? this.jedisCluster.getClass() : JedisCluster.class);
	}
	@Override
	public boolean isSingleton() {
		return true;
	}
	private Set<HostAndPort> parseHostAndPort() throws Exception {
		try {
			Properties prop = new Properties();
			prop.load(this.addressConfig.getInputStream());
			Set<HostAndPort> haps = new HashSet<HostAndPort>();
			for (Object key : prop.keySet()) {
				if (!((String) key).startsWith(addressKeyPrefix)) {
					continue;
				}
				String val = (String) prop.get(key);
				boolean isIpPort = p.matcher(val).matches();
				if (!isIpPort) {
					throw new IllegalArgumentException("ip 或 port 不合法");
				}
				String[] ipAndPort = val.split(":");
				HostAndPort hap = new HostAndPort(ipAndPort[0], Integer.parseInt(ipAndPort[1]));
				haps.add(hap);
			}
			return haps;
		} catch (IllegalArgumentException ex) {
			throw ex;
		} catch (Exception ex) {
			throw new Exception("解析 jedis 配置文件失败", ex);
		}
	}
	
	@Override
	public void afterPropertiesSet() throws Exception {
		Set<HostAndPort> haps = this.parseHostAndPort();
		
		jedisCluster = new JedisCluster(haps, timeout, maxRedirections,genericObjectPoolConfig);
		
	}
	public void setAddressConfig(Resource addressConfig) {
		this.addressConfig = addressConfig;
	}
	public void setTimeout(int timeout) {
		this.timeout = timeout;
	}
	public void setMaxRedirections(int maxRedirections) {
		this.maxRedirections = maxRedirections;
	}
	public void setAddressKeyPrefix(String addressKeyPrefix) {
		this.addressKeyPrefix = addressKeyPrefix;
	}
	public void setGenericObjectPoolConfig(GenericObjectPoolConfig genericObjectPoolConfig) {
		this.genericObjectPoolConfig = genericObjectPoolConfig;
	}
}
```
```
### 5.自动注入
```
```java
@Autowired
private JedisCluster jedisCluster;
```
```
## 附录：
### １.Redis Sentinel模式
Spring配置
```xml
<!-- Redis  Sentinel 集群配置 -->
	<bean id="redisSentinelConfiguration"
		class="org.springframework.data.redis.connection.RedisSentinelConfiguration">
		<constructor-arg name="master" value="${redis.master}"/>
		<constructor-arg name="sentinelHostAndPorts">
			<set>
			    <value>${redis.sentinels}</value>
			</set>
		</constructor-arg>
	</bean>
	<bean id="jedisPoolConfig" class="redis.clients.jedis.JedisPoolConfig">
		<property name="maxIdle" value="${redis.maxIdle}" />
		<property name="maxTotal" value="${redis.maxTotal}" />
		<property name="testOnBorrow" value="${redis.testOnBorrow}" />
	</bean>
	<bean id="jedisConnectionFactory"
		class="org.springframework.data.redis.connection.jedis.JedisConnectionFactory">
		<constructor-arg name="sentinelConfig" ref="redisSentinelConfiguration"/>
		<constructor-arg name="poolConfig" ref="jedisPoolConfig"/>
		<property name="usePool" value="true"></property>
		<property name="password" value="${redis.password}" />
		<property name="timeout" value="${redis.timeout}" />
		<property name="database" value="${redis.db}" />
	</bean>
	<bean id="stringRedisSerializer"
		class="org.springframework.data.redis.serializer.StringRedisSerializer" />
	<bean id="redisTemplate" class="org.springframework.data.redis.core.RedisTemplate">
		<property name="connectionFactory" ref="jedisConnectionFactory" />
		<property name="defaultSerializer" ref="stringRedisSerializer" />
	</bean>
	<bean id="cacheUtil" class="com.top.biz.util.RedisCacheUtil">
		<property name="redisTemplate" ref="redisTemplate" />
	</bean>
```
   Redis properties:
```
redis.master=mymaster
redis.sentinels=127.0.0.1:26379
redis.password=
redis.db=0
redis.timeout=2000
redis.maxIdle=5
redis.maxTotal=20
#redis.testOnBorrow=true
redis.testOnBorrow=false
```
Redis哨兵模式配置方法：
[http://my.oschina.net/manmao/blog/550428](http://my.oschina.net/manmao/blog/550428)
### ２.Redis单机配置
```xml
<!-- 配置Redis方法  单机配置 -->
	<bean id="jedisPoolConfig" class="redis.clients.jedis.JedisPoolConfig">
		<property name="maxIdle" value="${redis.maxIdle}" />
		<property name="maxTotal" value="${redis.maxTotal}"></property>
		<property name="testOnBorrow" value="${redis.testOnBorrow}" />
	</bean>
	<bean id="jedisConnectionFactory"
		class="org.springframework.data.redis.connection.jedis.JedisConnectionFactory">
		<property name="usePool" value="true"></property>
		<property name="hostName" value="${redis.host}" />
		<property name="port" value="${redis.port}" />
		<property name="password" value="${redis.password}" />
		<property name="timeout" value="${redis.timeout}" />
		<property name="database" value="${redis.db}"></property>
		<property name="poolConfig" ref="jedisPoolConfig" />
	</bean>
	<bean id="stringRedisSerializer" class="org.springframework.data.redis.serializer.StringRedisSerializer" />
	
	<bean id="redisTemplate" class="org.springframework.data.redis.core.RedisTemplate">
		<property name="connectionFactory" ref="jedisConnectionFactory" />
		<property name="defaultSerializer" ref="stringRedisSerializer" />
	</bean>
	<bean id="cacheUtil" class="com.top.biz.util.RedisCacheUtil">
		<property name="redisTemplate" ref="redisTemplate" />
	</bean>
    
    <!--StringRedis Template  使用方法 -->
    <!--
    <bean id="stringRedisTemplate" class="org.springframework.data.redis.core.StringRedisTemplate">
        <property name="connectionFactory" ref="jedisConnectionFactory" />
    </bean>
    -->
    <!-- 
       @Resource(name='stringRedisTemplate')
       private StringRedisTemplate redisTemplate;
    -->
```
 Redis properties:
```
redis.host=127.0.0.1
redis.port=6379
redis.password=
redis.db=0
redis.timeout=2000
redis.maxIdle=5
redis.maxTotal=20
#redis.testOnBorrow=true
redis.testOnBorrow=false
```
### 3.RedisUtil封装类
com.top.biz.util.RedisCacheUtil
RedisCacheUtil.java
实现代码
[http://www.oschina.net/code/snippet_1403215_55824](http://www.oschina.net/code/snippet_1403215_55824)
```java
public class RedisCacheUtil {
     
    private static RedisTemplate<String, String> redisTemplate;
     
    public void setRedisTemplate(RedisTemplate<String, String> redisTemp) {
        redisTemplate = redisTemp;
    }
     
    /* ----------- common --------- */
    public static Collection<String> keys(String pattern) {
        return redisTemplate.keys(pattern);
    }
     
    public static void delete(String key) {
        redisTemplate.delete(key);
    }
 
    public static void delete(Collection<String> key) {
        redisTemplate.delete(key);
    }
     
    /* ----------- string --------- */
    public static <T> T get(String key, Class<T> clazz) {
        String value = redisTemplate.opsForValue().get(key);
        return parseJson(value, clazz);
    }
     
    public static <T> List<T> mget(Collection<String> keys, Class<T> clazz) {
        List<String> values = redisTemplate.opsForValue().multiGet(keys);
        return parseJsonList(values, clazz);
    }
 
    public static <T> void set(String key, T obj, Long timeout, TimeUnit unit) {
        if (obj == null) {
            return;
        }
         
        String value = toJson(obj);
        if (timeout != null) {
            redisTemplate.opsForValue().set(key, value, timeout, unit);
        } else {
            redisTemplate.opsForValue().set(key, value);
        }
    }
 
    public static <T> T getAndSet(String key, T obj, Class<T> clazz) {
        if (obj == null) {
            return get(key, clazz);
        }
         
        String value = redisTemplate.opsForValue().getAndSet(key, toJson(obj));
        return parseJson(value, clazz);
    }
     
    public static int decrement(String key, int delta) {
        Long value = redisTemplate.opsForValue().increment(key, -delta);
        return value.intValue();
    }
     
    public static int increment(String key, int delta) {
        Long value = redisTemplate.opsForValue().increment(key, delta);
        return value.intValue();
    }
     
    /* ----------- list --------- */
    public static int size(String key) {
        return redisTemplate.opsForList().size(key).intValue();
    }
 
    public static <T> List<T> range(String key, long start, long end, Class<T> clazz) {
        List<String> list = redisTemplate.opsForList().range(key, start, end);
        return parseJsonList(list, clazz);
    }
 
    public static void rightPushAll(String key, Collection<?> values, Long timeout,
            TimeUnit unit) {
        if (values == null || values.isEmpty()) {
            return;
        }
         
        redisTemplate.opsForList().rightPushAll(key, toJsonList(values));
        if (timeout != null) {
            redisTemplate.expire(key, timeout, unit);
        }
    }
     
    public static <T> void leftPush(String key, T obj) {
        if (obj == null) {
            return;
        }
         
        redisTemplate.opsForList().leftPush(key, toJson(obj));
    }
 
    public static <T> T leftPop(String key, Class<T> clazz) {
        String value = redisTemplate.opsForList().leftPop(key);
        return parseJson(value, clazz);
    }
     
    public static void remove(String key, int count, Object obj) {
        if (obj == null) {
            return;
        }
         
        redisTemplate.opsForList().remove(key, count, toJson(obj));
    }
     
    /* ----------- zset --------- */
    public static int zcard(String key) {
        return redisTemplate.opsForZSet().zCard(key).intValue();
    }
     
    public static <T> List<T> zrange(String key, long start, long end, Class<T> clazz) {
        Set<String> set = redisTemplate.opsForZSet().range(key, start, end);
        return parseJsonList(setToList(set), clazz);
    }
     
    private static List<String> setToList(Set<String> set) {
        if (set == null) {
            return null;
        }
        return new ArrayList<String>(set);
    }
     
    public static void zadd(String key, Object obj, double score) {
        if (obj == null) {
            return;
        }
        redisTemplate.opsForZSet().add(key, toJson(obj), score);
    }
     
    public static void zaddAll(String key, List<TypedTuple<?>> tupleList, Long timeout, TimeUnit unit) {
        if (tupleList == null || tupleList.isEmpty()) {
            return;
        }
         
        Set<TypedTuple<String>> tupleSet = toTupleSet(tupleList);
        redisTemplate.opsForZSet().add(key, tupleSet);
        if (timeout != null) {
            redisTemplate.expire(key, timeout, unit);
        }
    }
     
    private static Set<TypedTuple<String>> toTupleSet(List<TypedTuple<?>> tupleList) {
        Set<TypedTuple<String>> tupleSet = new LinkedHashSet<TypedTuple<String>>();
        for (TypedTuple<?> t : tupleList) {
            tupleSet.add(new DefaultTypedTuple<String>(toJson(t.getValue()), t.getScore()));
        }
        return tupleSet;
    }
     
    public static void zrem(String key, Object obj) {
        if (obj == null) {
            return;
        }
        redisTemplate.opsForZSet().remove(key, toJson(obj));
    }
     
    public static void unionStore(String destKey, Collection<String> keys, Long timeout, TimeUnit unit) {
        if (keys == null || keys.isEmpty()) {
            return;
        }
         
        Object[] keyArr = keys.toArray();
        String key = (String) keyArr[0];
         
        Collection<String> otherKeys = new ArrayList<String>(keys.size() - 1);
        for (int i = 1; i < keyArr.length; i++) {
            otherKeys.add((String) keyArr[i]);
        }
         
        redisTemplate.opsForZSet().unionAndStore(key, otherKeys, destKey);
        if (timeout != null) {
            redisTemplate.expire(destKey, timeout, unit);
        }
    }
     
    /* ----------- tool methods --------- */
    public static String toJson(Object obj) {
        return JSON.toJSONString(obj, SerializerFeature.SortField);
    }
 
    public static <T> T parseJson(String json, Class<T> clazz) {
        return JSON.parseObject(json, clazz);
    }
 
    public static List<String> toJsonList(Collection<?> values) {
        if (values == null) {
            return null;
        }
 
        List<String> result = new ArrayList<String>();
        for (Object obj : values) {
            result.add(toJson(obj));
        }
        return result;
    }
     
    public static <T> List<T> parseJsonList(List<String> list, Class<T> clazz) {
        if (list == null) {
            return null;
        }
 
        List<T> result = new ArrayList<T>();
        for (String s : list) {
            result.add(parseJson(s, clazz));
        }
        return result;
    }
}
```
