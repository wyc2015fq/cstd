# Guava RateLimiter源码解析 - z69183787的专栏 - CSDN博客
2018年04月27日 16:52:16[OkidoGreen](https://me.csdn.net/z69183787)阅读数：366
[https://segmentfault.com/a/1190000012875897](https://segmentfault.com/a/1190000012875897)
# 前言
在开发高并发系统时有三把利器用来保护系统：缓存、降级和限流
- `缓存` 缓存的目的是提升系统访问速度和增大系统处理容量
- `降级` 降级是当服务出现问题或者影响到核心流程时，需要暂时屏蔽掉，待高峰或者问题解决后再打开
- `限流` 限流的目的是通过对并发访问/请求进行限速，或者对一个时间窗口内的请求进行限速来保护系统，一旦达到限制速率则可以拒绝服务、排队或等待、降级等处理
# 常用限流算法
常用的限流算法有两种：漏桶算法和令牌桶算法
> 
漏桶算法思路很简单，水（请求）先进入到漏桶里，漏桶以一定的速度出水，当水流入速度过大会直接溢出，可以看出漏桶算法能强行限制数据的传输速率。
对于很多应用场景来说，除了要求能够限制数据的平均传输速率外，还要求允许某种程度的突发传输。这时候漏桶算法可能就不合适了，令牌桶算法更为适合。
> 
令牌桶算法的原理是系统会以一个恒定的速度往桶里放入令牌，而如果请求需要被处理，则需要先从桶里获取一个令牌，当桶里没有令牌可取时，则拒绝服务。
更多关于漏桶算法和令牌桶算法的介绍可以参考 [http://blog.csdn.net/charlesl...](http://blog.csdn.net/charleslei/article/details/53152883)
# 信号量
操作系统的信号量是个很重要的概念，Java 并发库 的Semaphore 可以很轻松完成信号量控制，Semaphore可以控制某个资源可被同时访问的个数，通过 acquire() 获取一个许可，如果没有就等待，而 release() 释放一个许可。
信号量的本质是控制某个资源可被同时访问的个数，在一定程度上可以控制某资源的访问频率，但不能精确控制。
```
@Test
fun semaphoreTest() {
    val semaphore = Semaphore(2)
    (1..10).map {
        thread(true) {
            semaphore.acquire()
            println("$it\t${Date()}")
            Thread.sleep(1000)
            semaphore.release()
        }
    }.forEach { it.join() }
}
```
以上示例，创建信号量，指定并发数为2，其输出如下
```
1   Wed Jan 17 10:31:49 CST 2018
2   Wed Jan 17 10:31:49 CST 2018
3   Wed Jan 17 10:31:50 CST 2018
4   Wed Jan 17 10:31:50 CST 2018
5   Wed Jan 17 10:31:51 CST 2018
6   Wed Jan 17 10:31:51 CST 2018
7   Wed Jan 17 10:31:52 CST 2018
8   Wed Jan 17 10:31:52 CST 2018
9   Wed Jan 17 10:31:53 CST 2018
10  Wed Jan 17 10:31:53 CST 2018
```
可以很清楚的看到，同一时刻最多只能有2个线程进行输出。
虽然信号量可以在一定程度上控制资源的访问频率，却不能精确控制。
# RateLimiter
Google开源工具包Guava提供了限流工具类RateLimiter，该类基于令牌桶算法实现流量限制，使用十分方便。
```
@Test
fun rateLimiterTest() {
    val rateLimiter = RateLimiter.create(0.5)
    arrayOf(1,6,2).forEach {
        println("${System.currentTimeMillis()} acq $it:\twait ${rateLimiter.acquire(it)}s")
    }
}
```
以上示例，创建一个RateLimiter，指定每秒放0.5个令牌（2秒放1个令牌），其输出见下
```
1516166482561 acq 1: wait 0.0s
1516166482563 acq 6: wait 1.997664s
1516166484569 acq 2: wait 11.991958s
```
从输出结果可以看出，RateLimiter具有预消费的能力：
`acq 1`时并没有任何等待直接预消费了1个令牌
`acq 6`时，由于之前预消费了1个令牌，故而等待了2秒，之后又预消费了6个令牌
`acq 2`时同理，由于之前预消费了6个令牌，故而等待了12秒
从另一方面讲，RateLimiter通过限制后面请求的等待时间，来支持一定程度的突发请求(预消费)。
但是某些情况下并`不需要`这种突发请求处理能力，如某IM厂商提供消息推送接口，但推送接口有严格的频率限制(600次/30秒)，在调用该IM厂商推送接口时便不能预消费，否则，则可能出现推送频率超出限制而失败。该情况的处理会在其他博文中介绍。
## 源码解读
Guava有两种限流模式，一种为稳定模式(SmoothBursty:令牌生成速度恒定)，一种为渐进模式(SmoothWarmingUp:令牌生成速度缓慢提升直到维持在一个稳定值)
两种模式实现思路类似，主要区别在等待时间的计算上，本篇重点介绍SmoothBursty
在调用`create`接口时，实际实例化的为`SmoothBursty`类
```java
public static RateLimiter create(double permitsPerSecond) {
    return create(permitsPerSecond, SleepingStopwatch.createFromSystemTimer());
}
static RateLimiter create(double permitsPerSecond, SleepingStopwatch stopwatch) {
    RateLimiter rateLimiter = new SmoothBursty(stopwatch, 1.0 /* maxBurstSeconds */);
    rateLimiter.setRate(permitsPerSecond);
    return rateLimiter;
}
```
在解析SmoothBursty原理前，重点解释下SmoothBursty中几个属性的含义
```java
/**
 * The currently stored permits.
 * 当前存储令牌数
 */
double storedPermits;
/**
 * The maximum number of stored permits.
 * 最大存储令牌数
 */
double maxPermits;
/**
 * The interval between two unit requests, at our stable rate. E.g., a stable rate of 5 permits
 * per second has a stable interval of 200ms.
 * 添加令牌时间间隔
 */
double stableIntervalMicros;
/**
 * The time when the next request (no matter its size) will be granted. After granting a request,
 * this is pushed further in the future. Large requests push this further than small requests.
 * 下一次请求可以获取令牌的起始时间
 * 由于RateLimiter允许预消费，上次请求预消费令牌后
 * 下次请求需要等待相应的时间到nextFreeTicketMicros时刻才可以获取令牌
 */
private long nextFreeTicketMicros = 0L; // could be either in the past or future
```
接下来介绍几个关键函数
```java
/**
 * Updates {@code storedPermits} and {@code nextFreeTicketMicros} based on the current time.
 */
void resync(long nowMicros) {
    // if nextFreeTicket is in the past, resync to now
    if (nowMicros > nextFreeTicketMicros) {
      double newPermits = (nowMicros - nextFreeTicketMicros) / coolDownIntervalMicros();
      storedPermits = min(maxPermits, storedPermits + newPermits);
      nextFreeTicketMicros = nowMicros;
    }
}
```
根据令牌桶算法，桶中的令牌是持续生成存放的，有请求时需要先从桶中拿到令牌才能开始执行，谁来持续生成令牌存放呢？
一种解法是，开启一个定时任务，由定时任务持续生成令牌。这样的问题在于会极大的消耗系统资源，如，某接口需要分别对每个用户做访问频率限制，假设系统中存在6W用户，则至多需要开启6W个定时任务来维持每个桶中的令牌数，这样的开销是巨大的。
另一种解法则是延迟计算，如上`resync`函数。该函数会在每次获取令牌之前调用，其实现思路为，若当前时间晚于nextFreeTicketMicros，则计算该段时间内可以生成多少令牌，将生成的令牌加入令牌桶中并更新数据。这样一来，只需要在获取令牌时计算一次即可。
```java
final long reserveEarliestAvailable(int requiredPermits, long nowMicros) {
  resync(nowMicros);
  long returnValue = nextFreeTicketMicros; // 返回的是上次计算的nextFreeTicketMicros
  double storedPermitsToSpend = min(requiredPermits, this.storedPermits); // 可以消费的令牌数
  double freshPermits = requiredPermits - storedPermitsToSpend; // 还需要的令牌数
  long waitMicros =
      storedPermitsToWaitTime(this.storedPermits, storedPermitsToSpend)
          + (long) (freshPermits * stableIntervalMicros); // 根据freshPermits计算需要等待的时间
  this.nextFreeTicketMicros = LongMath.saturatedAdd(nextFreeTicketMicros, waitMicros); // 本次计算的nextFreeTicketMicros不返回
  this.storedPermits -= storedPermitsToSpend;
  return returnValue;
}
```
该函数用于获取requiredPermits个令牌，并返回需要等待到的时间点
其中，`storedPermitsToSpend`为桶中可以消费的令牌数，`freshPermits`为还需要的(需要补充的)令牌数，根据该值计算需要等待的时间，追加并更新到`nextFreeTicketMicros`
需要注意的是，该函数的返回是更新前的(上次请求计算的)`nextFreeTicketMicros`，而不是本次更新的`nextFreeTicketMicros`，通俗来讲，本次请求需要为上次请求的预消费行为埋单，这也是RateLimiter可以预消费(处理突发)的原理所在。若需要禁止预消费，则修改此处返回更新后的`nextFreeTicketMicros`值。
回头来看`SmoothBursty`的构造函数
```java
SmoothBursty(SleepingStopwatch stopwatch, double maxBurstSeconds) {
  super(stopwatch);
  this.maxBurstSeconds = maxBurstSeconds; // 最大存储maxBurstSeconds秒生成的令牌
}
@Override
void doSetRate(double permitsPerSecond, double stableIntervalMicros) {
  double oldMaxPermits = this.maxPermits;
  maxPermits = maxBurstSeconds * permitsPerSecond; // 计算最大存储令牌数
  if (oldMaxPermits == Double.POSITIVE_INFINITY) {
    // if we don't special-case this, we would get storedPermits == NaN, below
    storedPermits = maxPermits;
  } else {
    storedPermits =
        (oldMaxPermits == 0.0)
            ? 0.0 // initial state
            : storedPermits * maxPermits / oldMaxPermits;
  }
}
```
桶中可存放的最大令牌数由maxBurstSeconds计算而来，其含义为最大存储maxBurstSeconds秒生成的令牌。
该参数的作用在于，可以更为灵活地控制流量。如，某些接口限制为300次/20秒，某些接口限制为50次/45秒等。
在了解以上概念后，就非常容易理解RateLimiter暴露出来的接口
```java
@CanIgnoreReturnValue
public double acquire() {
  return acquire(1);
}
@CanIgnoreReturnValue
public double acquire(int permits) {
  long microsToWait = reserve(permits);
  stopwatch.sleepMicrosUninterruptibly(microsToWait);
  return 1.0 * microsToWait / SECONDS.toMicros(1L);
}
final long reserve(int permits) {
  checkPermits(permits);
  synchronized (mutex()) {
    return reserveAndGetWaitLength(permits, stopwatch.readMicros());
  }
}
```
`acquire`函数主要用于获取permits个令牌，并计算需要等待多长时间，进而挂起等待，并将该值返回
```java
public boolean tryAcquire(int permits) {
  return tryAcquire(permits, 0, MICROSECONDS);
}
public boolean tryAcquire() {
  return tryAcquire(1, 0, MICROSECONDS);
}
public boolean tryAcquire(int permits, long timeout, TimeUnit unit) {
  long timeoutMicros = max(unit.toMicros(timeout), 0);
  checkPermits(permits);
  long microsToWait;
  synchronized (mutex()) {
    long nowMicros = stopwatch.readMicros();
    if (!canAcquire(nowMicros, timeoutMicros)) {
      return false;
    } else {
      microsToWait = reserveAndGetWaitLength(permits, nowMicros);
    }
  }
  stopwatch.sleepMicrosUninterruptibly(microsToWait);
  return true;
}
private boolean canAcquire(long nowMicros, long timeoutMicros) {
  return queryEarliestAvailable(nowMicros) - timeoutMicros <= nowMicros;
}
@Override
final long queryEarliestAvailable(long nowMicros) {
  return nextFreeTicketMicros;
}
```
`tryAcquire`函数可以尝试在timeout时间内获取令牌，如果可以则挂起等待相应时间并返回true，否则立即返回false
`canAcquire`用于判断timeout时间内是否可以获取令牌
至此，Guava RateLimiter的原理及用法介绍完毕，对`SmoothWarmingUp`感兴趣的童鞋可以自行查阅文档或源码。
