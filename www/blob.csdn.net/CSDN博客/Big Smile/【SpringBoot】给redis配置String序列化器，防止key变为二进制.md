# 【SpringBoot】给redis配置String序列化器，防止key变为二进制 - Big Smile - CSDN博客
2019年03月19日 22:37:42[王啸tr1912](https://me.csdn.net/tr1912)阅读数：101
我们在springboot中使用redis的时候，如果使用了springboot自带的redis管理，没有进行特殊配置的话，会出现一种情况，就是我们set到redis中的值，所有的key值我们获取到的，或者通过工具看到的，都是一堆乱码，根本
无法识别，其原因，是因为，springboot默认给redis使用了java自带的jdk序列化器（JdkSerializationRedisSerializer）：
![](https://img-blog.csdnimg.cn/20190319223340329.png)
我们需要手工进行配置，使得redis使用StringRedisSerializer  才能正常的存储和显示一般字符，那么需要怎么进行配置呢？
其实，我们只需要构建一个配置类即可：
```java
import com.fasterxml.jackson.annotation.JsonAutoDetect;
import com.fasterxml.jackson.annotation.PropertyAccessor;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.springframework.cache.CacheManager;
import org.springframework.cache.annotation.CachingConfigurerSupport;
import org.springframework.cache.annotation.EnableCaching;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.data.redis.cache.RedisCacheManager;
import org.springframework.data.redis.connection.RedisConnectionFactory;
import org.springframework.data.redis.core.RedisTemplate;
import org.springframework.data.redis.core.StringRedisTemplate;
import org.springframework.data.redis.serializer.Jackson2JsonRedisSerializer;
import org.springframework.data.redis.serializer.StringRedisSerializer;
@Configuration
@EnableCaching
public class CacheConfig extends CachingConfigurerSupport {
    @SuppressWarnings("rawtypes")
    @Bean
    public CacheManager cacheManager(RedisTemplate redisTemplate) {
        RedisCacheManager rcm = new RedisCacheManager(redisTemplate);
        return rcm;
    }
    @Bean
    public RedisTemplate<String, String> redisTemplate(RedisConnectionFactory factory) {
        StringRedisTemplate template = new StringRedisTemplate(factory);
        Jackson2JsonRedisSerializer jackson2JsonRedisSerializer = new Jackson2JsonRedisSerializer(Object.class);
        StringRedisSerializer stringRedisSerializer = new StringRedisSerializer();
        ObjectMapper om = new ObjectMapper();
        om.setVisibility(PropertyAccessor.ALL, JsonAutoDetect.Visibility.ANY);
        om.enableDefaultTyping(ObjectMapper.DefaultTyping.NON_FINAL);
        jackson2JsonRedisSerializer.setObjectMapper(om);
        template.setValueSerializer(stringRedisSerializer);
        template.afterPropertiesSet();
        return template;
    }
}
```
将这个类放在我们的项目中，他会在启动的时候自动加载，重新配置redis的序列化方式，从而达到我们的目的。
