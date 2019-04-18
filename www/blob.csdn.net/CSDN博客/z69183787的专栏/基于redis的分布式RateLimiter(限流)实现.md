# 基于redis的分布式RateLimiter(限流)实现 - z69183787的专栏 - CSDN博客
2018年04月27日 17:15:59[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2252
[https://segmentfault.com/a/1190000012947169](https://segmentfault.com/a/1190000012947169)
# 业务背景
系统需要对接某IM厂商rest接口，向客户端推送消息（以及其他IM业务）
该厂商对rest接口调用有频率限制：总rest调用9000次/30s；消息推送600次/30s
系统为分布式集群，需要控制整个分布式集群总的接口调用频率满足以上限制
## Guava RateLimiter
上篇文章 《[Guava RateLimiter源码解析](https://segmentfault.com/a/1190000012875897)》中介绍了Guava RateLimiter的用法及原理，但为什么不直接使用Guava
 RateLimiter？原因有二：
- Guava RateLimiter只能应用于单进程，多进程间协同控制便无能为力
- Guava RateLimiter能够处理突发请求(预消费)，这里rest接口调用频率限制是固定的，不需要更不能使用预消费能力，否则将会导致接口调用失败
## Redis
为什么说选用redis是合理的？
- redis效率高，易扩展
- redis对语言无关，可以更好的接入不同语言开发的系统（异构）
- redis单进程单线程的特点可以更好的解决最终一致性，多进程间协同控制更为容易
# 基于Redis实现RateLimiter
这里完全参考Guava RateLimiter实现思路，不同的是，Guava将令牌桶数据存放于对象（内存）中，这里讲令牌桶数据存放在redis中，奉上源码 [https://github.com/manerfan/m...](https://github.com/manerfan/manerfan-spring-boot-starters/blob/master/manerfan-springboot-starter-ratelimiter/src/main/kotlin/com/manerfan/starter/ratelimiter/RateLimiter.kt)
首先创建令牌桶数据模型
```
class RedisPermits(
        val maxPermits: Long,
        var storedPermits: Long,
        val intervalMillis: Long,
        var nextFreeTicketMillis: Long = System.currentTimeMillis()
) {
    constructor(permitsPerSecond: Double, maxBurstSeconds: Int = 60, nextFreeTicketMillis: Long = System.currentTimeMillis()) :
            this((permitsPerSecond * maxBurstSeconds).toLong(), permitsPerSecond.toLong(), (TimeUnit.SECONDS.toMillis(1) / permitsPerSecond).toLong(), nextFreeTicketMillis)
    fun expires(): Long {
        val now = System.currentTimeMillis()
        return 2 * TimeUnit.MINUTES.toSeconds(1) + TimeUnit.MILLISECONDS.toSeconds(max(nextFreeTicketMillis, now) - now)
    }
    fun reSync(now: Long): Boolean {
        if (now > nextFreeTicketMillis) {
            storedPermits = min(maxPermits, storedPermits + (now - nextFreeTicketMillis) / intervalMillis)
            nextFreeTicketMillis = now
            return true
        }
        return false
    }
}
```
各属性字段含义与Guava相同（参见《[Guava RateLimiter源码解析](https://segmentfault.com/a/1190000012875897)》），且默认最多存储maxBurstSeconds秒生成的令牌
`reSync`函数同样是为了解决令牌桶数据更新问题，在每次获取令牌之前调用，这里不多介绍
`expires`函数计算redis数据过期时间。同样的例子，某接口需要分别对每个用户做访问频率限制，假设系统中存在6W用户，则至多需要在redis中创建6W条数据，对于长期运行的系统，这个数字会只增不减，这对于redis来说也是一个不小的挑战（虽然示例中的数字相对较小）。为了减轻redis压力，需要对令牌桶数据做过期处理，对于使用频率不是很高的业务场景，可以及时清理。
为了更好的操作，这里创建一个操作RedisPermits的Redis模板
```
@Configuration
class RateLimiterConfiguration {
    @Bean
    fun permitsTemplate(redisConnectionFactory: RedisConnectionFactory): PermitsTemplate {
        val template = PermitsTemplate()
        template.connectionFactory = redisConnectionFactory
        return template
    }
}
class PermitsTemplate : RedisTemplate<String, RedisPermits>() {
    private val objectMapper = jacksonObjectMapper()
    init {
        keySerializer = StringRedisSerializer()
        valueSerializer = object : RedisSerializer<RedisPermits> {
            override fun serialize(t: RedisPermits) = objectMapper.writeValueAsBytes(t)
            override fun deserialize(bytes: ByteArray?) = bytes?.let { objectMapper.readValue(it, RedisPermits::class.java) }
        }
    }
}
```
以下介绍几个关键函数，完整代码见 [https://github.com/manerfan/m...](https://github.com/manerfan/manerfan-spring-boot-starters/blob/master/manerfan-springboot-starter-ratelimiter/src/main/kotlin/com/manerfan/starter/ratelimiter/RateLimiter.kt)
```
/**
 * 生成并存储默认令牌桶
 */
private fun putDefaultPermits(): RedisPermits {
    val permits = RedisPermits(permitsPerSecond, maxBurstSeconds)
    permitsTemplate.opsForValue().set(key, permits, permits.expires(), TimeUnit.SECONDS)
    return permits
}
/**
 * 获取/更新令牌桶
 */
private var permits: RedisPermits
    get() = permitsTemplate.opsForValue()[key] ?: putDefaultPermits()
    set(permits) = permitsTemplate.opsForValue().set(key, permits, permits.expires(), TimeUnit.SECONDS)
/**
 * 获取redis服务器时间
 */
private val now get() = permitsTemplate.execute { it.time() } ?: System.currentTimeMillis()
```
`putDefaultPermits`用于生成默认令牌桶并存入redis
`permits`的`getter``setter`方法实现了redis中令牌桶的获取及更新
`now`用于获取redis服务器的时间，这样便能保证分布式集群中各节点对数据处理的一致性
```
private fun reserveAndGetWaitLength(tokens: Long): Long {
    val n = now
    var permit = permits
    permit.reSync(n)
    val storedPermitsToSpend = min(tokens, permit.storedPermits) // 可以消耗的令牌数
    val freshPermits = tokens - storedPermitsToSpend // 需要等待的令牌数
    val waitMillis = freshPermits * permit.intervalMillis // 需要等待的时间
    permit.nextFreeTicketMillis = LongMath.saturatedAdd(permit.nextFreeTicketMillis, waitMillis)
    permit.storedPermits -= storedPermitsToSpend
    permits = permit
    return permit.nextFreeTicketMillis - n
}
```
该函数用于获取tokens个令牌，并返回需要等待到的时长（毫秒）
其中，storedPermitsToSpend为桶中可以消费的令牌数，freshPermits为还需要的(需要补充的)令牌数，根据该值计算需要等待的时间，追加并更新到nextFreeTicketMillis
需要注意，这里与Guava RateLimiter不同的是，Guava中的返回是更新前的(上次请求计算的)nextFreeTicketMicros，本次请求通过为上次请求的预消费行为埋单而实现突发请求的处理；这里返回的是由于桶中令牌不足而需要真真切切等待的时间
通俗来讲
- Guava为寅吃卯粮，本次请求需要为上次请求的预消费行为埋单
- 这里为自力更生，谁消费谁埋单，为自己的行为负责
```
private fun reserve(tokens: Long): Long {
    checkTokens(tokens)
    try {
        syncLock.lock()
        return reserveAndGetWaitLength(tokens)
    } finally {
        syncLock.unLock()
    }
}
```
该函数与`reserveAndGetWaitLength`等同，只是为了避免并发问题而添加了同步锁（分布式同步锁的介绍请参见《[基于redis的分布式锁实现](https://segmentfault.com/a/1190000012919740)》）
```
private fun queryEarliestAvailable(tokens: Long): Long {
    val n = now
    var permit = permits
    permit.reSync(n)
    val storedPermitsToSpend = min(tokens, permit.storedPermits) // 可以消耗的令牌数
    val freshPermits = tokens - storedPermitsToSpend // 需要等待的令牌数
    val waitMillis = freshPermits * permit.intervalMillis // 需要等待的时间
    return LongMath.saturatedAdd(permit.nextFreeTicketMillis - n, waitMillis)
}
```
该函数用于计算，获取tokens个令牌需要等待的时长（毫秒）
```
private fun canAcquire(tokens: Long, timeoutMillis: Long): Boolean {
    return queryEarliestAvailable(tokens) - timeoutMillis <= 0
}
```
该函数用于计算，timeoutMillis时间内是否可以获取tokens个令牌
通过以上几个函数的了解，我们便可以很轻松的实现Guava RateLimiter中的`acquire`及`tryAcquire`功能
```
fun acquire(tokens: Long): Long {
    var milliToWait = reserve(tokens)
    logger.info("acquire for {}ms {}", milliToWait, Thread.currentThread().name)
    Thread.sleep(milliToWait)
    return milliToWait
}
fun acquire() = acquire(1)
```
```
fun tryAcquire(tokens: Long, timeout: Long, unit: TimeUnit): Boolean {
    val timeoutMicros = max(unit.toMillis(timeout), 0)
    checkTokens(tokens)
    var milliToWait: Long
    try {
        syncLock.lock()
        if (!canAcquire(tokens, timeoutMicros)) {
            return false
        } else {
            milliToWait = reserveAndGetWaitLength(tokens)
        }
    } finally {
        syncLock.unLock()
    }
    Thread.sleep(milliToWait)
    return true
}
fun tryAcquire(timeout: Long, unit: TimeUnit) = tryAcquire(1, timeout, unit)
```
# 回顾问题
至此，基于redis的分布式RateLimiter(限流)控制功能便完成了
回到文档起始处提出的问题，接某IM厂商rest接口，我们可以针对不同的频率限制创建不同的RateLimiter
```
val restRateLimiter = rateLimiterFactory.build("ratelimiter:im:rest", 9000 /30, 30)
val msgRateLimiter = rateLimiterFactory.build("ratelimiter:im:msg", 600 /30, 30)
```
推送消息时，可以如下调用
```
restRateLimiter.acquire()
msgRateLimiter.acquire(msgs.size)
msgUtil.push(msgs)
```
对于接口提供方限制接口访问频次，可以如下实现
```
val msgRateLimiter = rateLimiterFactory.build("ratelimiter:im:msg", 600 /30, 30)
fun receiveMsg(msgs: Array<Message>): Boolean {
    return when(msgRateLimiter.tryAcquire(msgs.size, 2, TimeUnit.SECONDS)) {
        true -> {
            thread(true) { msgUtil.receive(msgs) }
            true
        }
        else -> false
    }
```
```
```
