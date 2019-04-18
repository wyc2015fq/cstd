# 基于redis的分布式锁实现 - z69183787的专栏 - CSDN博客
2018年08月05日 21:05:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：107
个人分类：[锁-分布式锁																[Redis-基础](https://blog.csdn.net/z69183787/article/category/2923783)](https://blog.csdn.net/z69183787/article/category/6393629)
[https://segmentfault.com/a/1190000012919740](https://segmentfault.com/a/1190000012919740)
随着业务越来越复杂，应用服务都会朝着分布式、集群方向部署，而分布式[CAP原则](https://baike.baidu.com/item/CAP%E5%8E%9F%E5%88%99/5712863)告诉我们，Consistency（一致性）、 Availability（可用性）、Partition tolerance（分区容错性），三者不可得兼。
很多场景中，需要使用分布式事务、分布式锁等技术来保证数据最终一致性。有的时候，我们需要保证某一方法同一时刻只能被一个线程执行。
在单机（单进程）环境中，JAVA提供了很多并发相关API，但在多机（多进程）环境中就无能为力了。
对于分布式锁，最好能够满足以下几点
> 可以保证在分布式部署的应用集群中，同一个方法在同一时间只能被一台机器上的一个线程执行
这把锁要是一把可重入锁（避免死锁）
这把锁最好是一把阻塞锁
有高可用的获取锁和释放锁功能
获取锁和释放锁的性能要好
针对分布式锁，目前有以下几种实现方案（From: [http://www.hollischuang.com/a...](http://www.hollischuang.com/archives/1716)）
> 基于数据库实现分布式锁 
基于缓存实现分布式锁 
基于zookeeper实现分布式锁
对于第一种（基于数据库）及第三种（基于zookeeper）的实现方式可以参考博文[http://www.hollischuang.com/a...](http://www.hollischuang.com/archives/1716)，本篇文章介绍如何基于redis实现分布式锁
首先奉上源码 [https://github.com/manerfan/m...](https://github.com/manerfan/manerfan-spring-boot-starters/blob/master/manerfan-springboot-starter-ratelimiter/src/main/kotlin/com/manerfan/starter/ratelimiter/SyncLock.kt)
# 分布式同步锁实现
## 实现思路
锁的实现主要基于redis的`SETNX`命令（SETNX详细解释参考[这里](http://doc.redisfans.com/string/setnx.html)），我们来看`SETNX`的解释
> **SETNX key value**
将 key 的值设为 value ，当且仅当 key 不存在。
若给定的 key 已经存在，则 SETNX 不做任何动作。
SETNX 是『SET if Not eXists』(如果不存在，则 SET)的简写。
**返回值：**
设置成功，返回 1 。
设置失败，返回 0 。
使用`SETNX`完成同步锁的流程及事项如下：
- 使用`SETNX`命令获取锁，若返回0（key已存在，锁已存在）则获取失败，反之获取成功
- 为了防止获取锁后程序出现异常，导致其他线程/进程调用`SETNX`命令总是返回0而进入死锁状态，需要为该key设置一个“合理”的过期时间
- 释放锁，使用`DEL`命令将锁数据删除
## 实现过程
### 创建同步锁实现类
```
/**
 * 同步锁
 *
 * @property key Redis key
 * @property stringRedisTemplate RedisTemplate
 * @property expire Redis TTL/秒
 * @property safetyTime 安全时间/秒
 */
class SyncLock(
        private val key: String,
        private val stringRedisTemplate: StringRedisTemplate,
        private val expire: Long,
        private val safetyTime: Long
)
```
`key` reids中的key，对应java api synchronized的对象
`expire` reids中key的过期时间
`safetyTime` 下文介绍其作用
### 实现锁的获取功能
```
private val value: String get() = Thread.currentThread().name
/**
 * 尝试获取锁（立即返回）
 *
 * @return 是否获取成功
 */
fun tryLock(): Boolean {
    val locked = stringRedisTemplate.opsForValue().setIfAbsent(key, value) ?: false
    if (locked) {
        stringRedisTemplate.expire(key, expire, TimeUnit.SECONDS)
    }
    return locked
}
```
这里使用`setIfAbsent`函数(对应`SETNX`命令)尝试设置key的值为value（当前线程id+线程名），若成功则同时设置key的过期时间并返回true，否则返回false
### 实现带超时时间的锁获取功能
```
private val waitMillisPer: Long = 10
/**
 * 尝试获取锁，并至多等待timeout时长
 *
 * @param timeout 超时时长
 * @param unit 时间单位
 *
 * @return 是否获取成功
 */
fun tryLock(timeout: Long, unit: TimeUnit): Boolean {
    val waitMax = unit.toMillis(timeout)
    var waitAlready: Long = 0
    while (stringRedisTemplate.opsForValue().setIfAbsent(key, value) != true && waitAlready < waitMax) {
        Thread.sleep(waitMillisPer)
        waitAlready += waitMillisPer
    }
    if (waitAlready < waitMax) {
        stringRedisTemplate.expire(key, expire, TimeUnit.SECONDS)
        return true
    }
    return false
}
```
这里使用while循环不断尝试锁的获取，并至多尝试timeout时长，在timeout时间内若成功则同时设置key的过期时间并返回true，否则返回false
其实以上两种`tryLock`函数还是有一种可能便是，在调用`setIfAbsent`后、调用`expire`之前若服务出现异常，也将导致该锁(key)无法释放(过期或删除)，使得其他线程/进程再无法获取锁而进入死循环，为了避免此问题的产生，我们引入了`safetyTime`
该参数的作用为，从获取锁开始直到safetyTime时长，若仍未获取成功则认为某一线程/进程出现异常导致数据不正确，此时强制获取，其实现如下
### 实现带保护功能的锁获取功能
```
/**
 * 获取锁
 */
fun lock() {
    val waitMax = TimeUnit.SECONDS.toMillis(safetyTime)
    var waitAlready: Long = 0
    while (stringRedisTemplate.opsForValue().setIfAbsent(key, value) != true && waitAlready < waitMax) {
        Thread.sleep(waitMillisPer)
        waitAlready += waitMillisPer
    }
    // stringRedisTemplate.expire(key, expire, TimeUnit.SECONDS)
    stringRedisTemplate.opsForValue().set(key, value, expire, TimeUnit.SECONDS)
}
```
这里同样使用while循环不断尝试锁的获取，但至多等待safetyTime时长，最终不论是否成功，均使用`SETEX `命令将key设置为当前先线程对应的value，并同时设置该key的过期时间
### 实现锁的释放功能
```
/**
 * 释放锁
 */
fun unLock() {
    stringRedisTemplate.opsForValue()[key]?.let {
        if (it == value) {
            stringRedisTemplate.delete(key)
        }
    }
}
```
锁的释放使用`DEL`命令删除key，但需要注意的是，释放锁时只能释放本线程持有的锁
若expire设置不合理，如expire设置为10秒，结果在获取锁后线程运行了20秒，该锁有可能已经被其他线程强制获取，即该key代表的锁已经不是当前线程所持有的锁，此时便不能冒然删除该key，而只能释放本线程持有的锁。
## 集成spring boot
为了更好的与spring集成，我们创建一个工厂类来辅助创建同步锁实例
```
/**
 * SyncLock同步锁工厂类
 */
@Component
class SyncLockFactory {
    @Autowired
    private lateinit var stringRedisTemplate: StringRedisTemplate
    private val syncLockMap = mutableMapOf<String, SyncLock>()
    /**
     * 创建SyncLock
     *
     * @param key Redis key
     * @param expire Redis TTL/秒，默认10秒
     * @param safetyTime 安全时间/秒，为了防止程序异常导致死锁，在此时间后强制拿锁，默认 expire * 5 秒
     */
    @Synchronized
    fun build(key: String, expire: Long = 10 /* seconds */, safetyTime: Long = expire * 5/* seconds */): SyncLock {
        if (!syncLockMap.containsKey(key)) {
            syncLockMap[key] = SyncLock(key, stringRedisTemplate, expire, safetyTime)
        }
        return syncLockMap[key]!!
    }
}
```
在spring框架下可以更方便的使用
```
@Component
class SomeLogic: InitializingBean {
  @Autowired
  lateinit var syncLockFactory: SyncLockFactory
  
  lateinit var syncLock
  override fun afterPropertiesSet() {
    syncLock = syncLockFactory.build("lock:some:name", 10)
  }
  fun someFun() {
    syncLock.lock()
    try {
      // some logic
    } finally {
      syncLock.unlock()
    }
  }
}
```
## 注解的实现
借助spring aop框架，我们可以将SyncLock的使用进一步简化
### 创建注解类
```
/**
 * 同步锁注解
 *
 * @property key Redis key
 * @property expire Redis TTL/秒，默认10秒
 */
@Target(AnnotationTarget.FUNCTION)
@Retention(AnnotationRetention.RUNTIME)
annotation class SyncLockable(
        val key: String,
        val expire: Long = 10
)
```
### 实现AOP
```
/**
 * 同步锁注解处理
 */
@Aspect
@Component
class SyncLockHandle {
    @Autowired
    private lateinit var syncLockFactory: SyncLockFactory
    /**
     * 在方法上执行同步锁
     */
    @Around("@annotation(syncLockable)")
    fun syncLock(jp: ProceedingJoinPoint, syncLockable: SyncLockable): Any? {
        val lock = syncLockFactory.build(syncLockable.key, syncLockable.expire)
        try {
            lock.lock()
            return jp.proceed()
        } finally {
            lock.unLock()
        }
    }
}
```
如此一来，我们便可以按照如下方式使用SyncLock
```
@Component
class SomeLogic {
    @SyncLockable("lock:some:name", 10)
    fun someFun() {
        // some logic
    }
}
```
是不是显得更加方便！
