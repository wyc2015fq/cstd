# redis+lua 实现分布式令牌桶，高并发限流 - z69183787的专栏 - CSDN博客
2018年08月09日 11:20:55[OkidoGreen](https://me.csdn.net/z69183787)阅读数：540
[https://blog.csdn.net/sunlihuo/article/details/79700225](https://blog.csdn.net/sunlihuo/article/details/79700225)
方案一、在提供给业务方的Controller层进行控制。 
1、使用guava提供工具库里的RateLimiter类(内部采用令牌捅算法实现)进行限流 
2、使用Java自带delayqueue的延迟队列实现(编码过程相对麻烦，此处省略代码) 
3、使用Redis实现，存储两个key，一个用于计时，一个用于计数。请求每调用一次，计数器增加1，若在计时器时间内计数器未超过阈值，则可以处理任务 
方案二、在短信发送至服务商时做限流处理 
方案三、同时使用方案一和方案二
```
可行性分析 
最快捷且有效的方式是使用RateLimiter实现，但是这很容易踩到一个坑，单节点模式下，使用RateLimiter进行限流一点问题都没有。但是…线上是分布式系统，布署了多个节点，而且多个节点最终调用的是同一个短信服务商接口。虽然我们对单个节点能做到将QPS限制在400/s，但是多节点条件下，如果每个节点均是400/s，那么到服务商那边的总请求就是节点数x400/s，于是限流效果失效。使用该方案对单节点的阈值控制是难以适应分布式环境的，至少目前我还没想到更为合适的方式。 
对于第二种，使用delayqueue方式。其实主要存在两个问题，1：短信系统本身就用了一层消息队列，有用kafka，或者rabitmq，如果再加一层延迟队列，从设计上来说是不太合适的。2：实现delayqueue的过程相对较麻烦，耗时可能比较长，而且达不到精准限流的效果 
对于第三种，使用redis进行限流，其很好地解决了分布式环境下多实例所导致的并发问题。因为使用redis设置的计时器和计数器均是全局唯一的，不管多少个节点，它们使用的都是同样的计时器和计数器，因此可以做到非常精准的流控。同时，这种方案编码并不复杂，可能需要的代码不超过10行。
```
![这里写图片描述](https://img-blog.csdn.net/20180326163522118?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3VubGlodW8=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
令牌桶算法(Token Bucket)和 Leaky Bucket 效果一样但方向相反的算法,更加容易理解.随着时间流逝,系统会按恒定1/QPS时间间隔(如果QPS=100,则间隔是10ms)往桶里加入Token(想象和漏洞漏水相反,有个水龙头在不断的加水),如果桶已经满了就不再加了.新请求来临时,会各自拿走一个Token,如果没有Token可拿了就阻塞或者拒绝服务. 
令牌桶的另外一个好处是可以方便的改变速度. 一旦需要提高速率,则按需提高放入桶中的令牌的速率. 一般会定时(比如100毫秒)往桶中增加一定数量的令牌, 有些变种算法则实时的计算应该增加的令牌的数量. 
![这里写图片描述](https://img-blog.csdn.net/20180326162903821?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3VubGlodW8=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
last_mill_second 最后时间毫秒 
curr_permits 当前可用的令牌 
max_burst 令牌桶最大值 
rate 每秒生成几个令牌 
app 应用 
令牌桶内令牌生成借鉴Guava-RateLimiter类的设计 
每次getToken根据时间戳生成token，不超过最大值
```
local ratelimit_info=redis.pcall("HMGET",KEYS[1],"last_mill_second","curr_permits","max_burst","rate","app")
local last_mill_second=ratelimit_info[1]
local curr_permits=tonumber(ratelimit_info[2])
local max_burst=tonumber(ratelimit_info[3])
local rate=tonumber(ratelimit_info[4])
local app=tostring(ratelimit_info[5])
if app == nil then
    return 0
end
local local_curr_permits=max_burst;
if(type(last_mill_second) ~='boolean' and last_mill_second ~=nil) then
    local reverse_permits=math.floor((ARGV[2]-last_mill_second)/1000)*rate
    if(reverse_permits>0) then
        redis.pcall("HMSET",KEYS[1],"last_mill_second",ARGV[2])
    end
    local expect_curr_permits=reverse_permits+curr_permits
    local_curr_permits=math.min(expect_curr_permits,max_burst);
else
    redis.pcall("HMSET",KEYS[1],"last_mill_second",ARGV[2])
end
local result=-1
if(local_curr_permits-ARGV[1]>0) then
    result=1
    redis.pcall("HMSET",KEYS[1],"curr_permits",local_curr_permits-ARGV[1])
else
    redis.pcall("HMSET",KEYS[1],"curr_permits",local_curr_permits)
end
return result
```
Lua脚本在Redis中运行，保证了取令牌和生成令牌两个操作的原子性。
```
# REDIS (RedisProperties)
# Redis数据库索引（默认为0）
spring.redis.database=0
# Redis服务器地址
spring.redis.host=127.0.0.1
# Redis服务器连接端口
spring.redis.port=6379
# Redis服务器连接密码（默认为空）
spring.redis.password=
# 连接池最大连接数（使用负值表示没有限制）
spring.redis.jedis.pool.max-active=8
# 连接池最大阻塞等待时间（使用负值表示没有限制）
spring.redis.jedis.pool.max-wait=-1
# 连接池中的最大空闲连接
spring.redis.jedis.pool.max-idle=8
# 连接池中的最小空闲连接
spring.redis.jedis.pool.min-idle=0
# 连接超时时间（毫秒）
spring.redis.timeout=2000
```
```
@Configuration
@EnableCaching
public class RedisConfig extends CachingConfigurerSupport {
    @Override
    @Bean
    public KeyGenerator keyGenerator() {
        return new KeyGenerator() {
            @Override
            public Object generate(Object target, Method method, Object... params) {
                StringBuilder sb = new StringBuilder();
                sb.append(target.getClass().getName());
                sb.append(method.getName());
                for (Object obj : params) {
                    sb.append(obj.toString());
                }
                return sb.toString();
            }
        };
    }
    @Bean
    public RedisTemplate<String, String> redisTemplate(RedisConnectionFactory factory) {
        StringRedisTemplate template = new StringRedisTemplate(factory);
        Jackson2JsonRedisSerializer jackson2JsonRedisSerializer = new Jackson2JsonRedisSerializer(Object.class);
        ObjectMapper om = new ObjectMapper();
        om.setVisibility(PropertyAccessor.ALL, JsonAutoDetect.Visibility.ANY);
        om.enableDefaultTyping(ObjectMapper.DefaultTyping.NON_FINAL);
        jackson2JsonRedisSerializer.setObjectMapper(om);
        template.setValueSerializer(jackson2JsonRedisSerializer);
        template.afterPropertiesSet();
        return template;
    }
    @Bean("ratelimitLua")
    public DefaultRedisScript getRedisScript() {
        DefaultRedisScript redisScript = new DefaultRedisScript();
        redisScript.setLocation(new ClassPathResource("ratelimit.lua"));
        redisScript.setResultType(java.lang.Long.class);
        return redisScript;
    }
    @Bean("ratelimitInitLua")
    public DefaultRedisScript getInitRedisScript() {
        DefaultRedisScript redisScript = new DefaultRedisScript();
        redisScript.setLocation(new ClassPathResource("ratelimitInit.lua"));
        redisScript.setResultType(java.lang.Long.class);
        return redisScript;
    }
}
public class Constants {
    public static final String RATE_LIMIT_KEY = "ratelimit:";
}
public enum Token {
    SUCCESS,
    FAILED;
    public boolean isSuccess(){
        return this.equals(SUCCESS);
    }
    public boolean isFailed(){
        return this.equals(FAILED);
    }
}
```
```
@Service
public class RateLimitClient {
    @Autowired
    StringRedisTemplate stringRedisTemplate;
    @Qualifier("getRedisScript")
    @Resource
    RedisScript<Long> ratelimitLua;
    @Qualifier("getInitRedisScript")
    @Resource
    RedisScript<Long> ratelimitInitLua;
    public Token initToken(String key){
        Token token = Token.SUCCESS;
        Long currMillSecond = stringRedisTemplate.execute(
                (RedisCallback<Long>) redisConnection -> redisConnection.time()
        );
        /**
         * redis.pcall("HMSET",KEYS[1],
         "last_mill_second",ARGV[1],
         "curr_permits",ARGV[2],
         "max_burst",ARGV[3],
         "rate",ARGV[4],
         "app",ARGV[5])
         */
        Long accquire = stringRedisTemplate.execute(ratelimitInitLua,
                Collections.singletonList(getKey(key)), currMillSecond.toString(), "1", "10", "10", "skynet");
        if (accquire == 1) {
            token = Token.SUCCESS;
        } else if (accquire == 0) {
            token = Token.SUCCESS;
        } else {
            token = Token.FAILED;
        }
        return token;
    }
    /**
     * 获得key操作
     *
     * @param key
     * @return
     */
    public Token accquireToken(String key) {
        return accquireToken(key, 1);
    }
    public Token accquireToken(String key, Integer permits) {
        Token token = Token.SUCCESS;
        Long currMillSecond = stringRedisTemplate.execute(
                (RedisCallback<Long>) redisConnection -> redisConnection.time()
        );
        Long accquire = stringRedisTemplate.execute(ratelimitLua,
                Collections.singletonList(getKey(key)), permits.toString(), currMillSecond.toString());
        if (accquire == 1) {
            token = Token.SUCCESS;
        } else {
            token = Token.FAILED;
        }
        return token;
    }
    public String getKey(String key) {
        return Constants.RATE_LIMIT_KEY + key;
    }
}
```
```
local result=1
redis.pcall("HMSET",KEYS[1],
        "last_mill_second",ARGV[1],
        "curr_permits",ARGV[2],
        "max_burst",ARGV[3],
        "rate",ARGV[4],
        "app",ARGV[5])
return result
```
