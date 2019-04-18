# 集群模式下的定时任务与Redis分布式锁 - z69183787的专栏 - CSDN博客
2018年08月02日 17:31:45[OkidoGreen](https://me.csdn.net/z69183787)阅读数：254
[https://blog.csdn.net/sinat_25295611/article/details/80420086](https://blog.csdn.net/sinat_25295611/article/details/80420086)
业务场景：在电商项目中，往往会有这样的一个功能设计，当用户下单后一段时间没有付款，系统就会在超时后关闭该订单。
通常我们会做一个定时任务每分钟来检查前半小时的订单，将没有付款的订单列表查询出来，然后对订单中的商品进行库存的恢复，然后将该订单设置为无效。
比如我们这里使用`Spring Schedule`的方式做一个定时任务：
> 
注：打开Spring Schedule 的自动注解扫描,在Spring配置中添加`<task:annotation-driven/>`
```
@Component
@Slf4j
public class CloseOrderTask {
    @Autowired
    private IOrderService iOrderService;
    @Scheduled(cron = "0 */1 * * * ? ")
    public void closeOrderTaskV1() {
        log.info("定时任务启动");
        //执行关闭订单的操作
        iOrderService.closeOrder();
        log.info("定时任务结束");
    }
}
```
在单服务器下这样执行并没有问题，但是随着业务量的增多，势必会演进成集群模式，在同一时刻有多个服务执行一个定时任务就会带来问题，首先是服务器资源的浪费，同时会带来业务逻辑的混乱，如果定时任务是做的数据库操作将会带来很大的风险。
Redis分布式锁
下面分析一下分布式情况下定时任务的解决方案
通常使用Redis作为分布式锁来解决这类问题，Redis分布式锁流程如下：
![](http://ww1.sinaimg.cn/large/78c3a6d7ly1frl80vs6rjj20op0g0dfw.jpg)
Redis分布式锁v1版本：
```
//注意：以下为了测试方便，定时时间都设置为10s
@Scheduled(cron = "0/10 * * * * ? ")
    public void closeOrderTaskV1() {
        log.info("定时任务启动");
        long lockTime = 5000;//5秒
        Long lockKeyResult = RedisShardedPoolUtil.setnx(Const.REDIS_LOCK.CLOSE_ORDER_TASK_LOCK, String.valueOf(System.currentTimeMillis() + lockTime));
        //如果获得了分布式锁，执行关单业务
        if (lockKeyResult != null && lockKeyResult.intValue() == 1) {
            closeOrder(Const.REDIS_LOCK.CLOSE_ORDER_TASK_LOCK);
        }else {
            log.info("没有获得分布式锁");
        }
        log.info("定时任务结束================================");
    }
//关闭订单，并释放锁
    private void closeOrder(String lockName) {
        RedisShardedPoolUtil.expire(lockName,50); //锁住50秒
        log.info("线程{} 获取锁 {}",Thread.currentThread().getName(),lockName);
        //模拟执行关单操作
        try {
            Thread.sleep(5000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        //主动关闭锁
        RedisShardedPoolUtil.del(lockName);
        log.info("线程{} 释放锁 {}",Thread.currentThread().getName(),lockName);
    }
```
(由于我电脑配置比较低，开2个IDEA进程调试会比较卡，所以一个项目在IDEA调试，另外一个打成war放在tomcat运行，打包命令
`mvn clean package -Dmaven.test.skip=true -Pdev`- 1
)
tomcat1调试日志
![tomcat1本地调试日志](http://ww1.sinaimg.cn/large/78c3a6d7ly1frl4mrru93j20tu07mn1q.jpg)
tomcat2日志
![tomcat2日志](http://ww1.sinaimg.cn/large/78c3a6d7ly1frl4p4c45jj20qr0f2wnr.jpg)
此时分布式锁已经生效，在集群环境下不会同时出现2个任务同时执行的情况，但是这时又引出了另外一个问题，
我们的逻辑是先`setnx`获取分布式锁（此时该锁没有设置过期时间，即不会过期）,然后`expire`设置过期锁过期时间，如果在获取锁和设置过期时间之间，服务器（tomcat）挂了就会出现锁永远都不会过期的情况！
- 在正常关闭tomcat的情况下（shutdown），我们可以通过@PreDestory执行删除锁逻辑，如下
```
@PreDestroy
    public void delCloseLock(){
        RedisShardedPoolUtil.del(Const.REDIS_LOCK.CLOSE_ORDER_TASK_LOCK);
        log.info("Tomcat shut down 释放锁 {}",Const.REDIS_LOCK.CLOSE_ORDER_TASK_LOCK);
    }
```
- 
在tomcat被kill或意外终止时，以上方法并不管用
Redis分布式锁v2版本 :
![](http://ww1.sinaimg.cn/large/78c3a6d7ly1frl8m0ig6jj20rs0h9wer.jpg)
我们将`setnx`未获取到锁的情况进行重新设计，为的是防止`v1`版本死锁的产生，当第一次未获取到锁时，取出`lockKey`中存放的过期时间，与当前时间进行对比，若已超时则通过`getset`操作重置获取锁并更新过期时间，若第一次取出时未达到过期时间，说明还在上次任务执行的有效时间范围内，可能就需要等这一段时间，通常过期时间设置为2~5秒，不会太长。
以上则是在超时的基础上防止死锁的产生，以下为代码实现：
```
//注意：以下为了测试方便，定时时间都设置为10s
@Scheduled(cron = "0/10 * * * * ? ")
    public void closeOrderTaskV2() {
        log.info("定时任务启动");
        long lockTime = 5000; //5s
        Long lockKeyResult = RedisShardedPoolUtil.setnx(Const.REDIS_LOCK.CLOSE_ORDER_TASK_LOCK, String.valueOf(System.currentTimeMillis() + lockTime));
        //如果获得了分布式锁，执行关单业务
        if (lockKeyResult != null && lockKeyResult.intValue() == 1) {
            closeOrder(Const.REDIS_LOCK.CLOSE_ORDER_TASK_LOCK);
        }else {
            String lockValue1 = RedisShardedPoolUtil.get(Const.REDIS_LOCK.CLOSE_ORDER_TASK_LOCK);
            //查到锁的值并与当前时间比较检查其是否已经超时，若超时则可以重新获取锁
            if (lockValue1 != null && System.currentTimeMillis() > Long.valueOf(lockValue1)) {
                //通过用当前时间戳getset操作会给对应的key设置新的值并返回旧值，这是一个原子操作
                //redis返回nil,则说明该值已经无效
                String lockValue2 = RedisShardedPoolUtil.getSet(Const.REDIS_LOCK.CLOSE_ORDER_TASK_LOCK, String.valueOf(System.currentTimeMillis() + lockTime));
                if (lockValue2 == null || StringUtils.equals(lockValue1, lockValue2)) {
                    //获取锁成功
                    closeOrder(Const.REDIS_LOCK.CLOSE_ORDER_TASK_LOCK);
                } else {
                    log.info("没有获得分布式锁:{}",Const.REDIS_LOCK.CLOSE_ORDER_TASK_LOCK);
                }
            }
            log.info("没有获得分布式锁:{}",Const.REDIS_LOCK.CLOSE_ORDER_TASK_LOCK);
        }
        log.info("定时任务结束================================");
    }
```
至此，我们的这个分布式锁是没有问题了。
下面介绍一下使用`Redisson`这个框架来实现分布式锁。
Redisson实现分布式锁
[Redisson](https://redisson.org/)是架设在[Redis](http://redis.cn/)基础上的一个Java驻内存数据网格（In-Memory Data Grid） ,其功能十分强大，解决很多分布式架构中的问题，附上其GitHub的WIKI地址：[https://github.com/redisson/redisson/wiki](https://github.com/redisson/redisson/wiki)
要引入Redisson，首先加入其pom依赖与spring整合：
```
<dependency>
      <groupId>org.redisson</groupId>
      <artifactId>redisson</artifactId>
      <version>2.9.0</version>
</dependency>
<!--redisson依赖-->
<dependency>
    <groupId>com.fasterxml.jackson.dataformat</groupId>
    <artifactId>jackson-dataformat-avro</artifactId>
    <version>2.9.0</version>
</dependency>
```
在我们的工具类中建立`RedissonManager`完成Redisson的初始化：
```
@Component
@Slf4j
public class RedissonManager {
    private Redisson redisson = null;
    private Config config = new Config();
    private static String host1 = PropertiesUtil.getProperty("redis1.host");
    private static int port1 = Integer.parseInt(PropertiesUtil.getProperty("redis1.port"));
    private static String host2 = PropertiesUtil.getProperty("redis2.host");
    private static int port2 = Integer.parseInt(PropertiesUtil.getProperty("redis2.port"));
    @PostConstruct
    private void init() {
        try {
            config.useSingleServer().setAddress(new StringBuilder().append(host1).append(":").append("port1").toString());
            redisson = (Redisson) Redisson.create(config);
            log.info("Redisson 初始化完成");
        } catch (Exception e) {
           log.error("init Redisson error ",e);
        }
    }
    public Redisson getRedisson() {
        return redisson;
    }
}
```
初始化完成之后就可以来写分布式锁了，使用完`Redisson`实现分布锁之后就会发现一切是那么的简便：
```
//使用Redisson实现分布式锁
@Scheduled(cron = "0/10 * * * * ? ")
    public void closeOrderTaskV3() {
        log.info("定时任务启动");
        RLock lock = redissonManager.getRedisson().getLock(Const.REDIS_LOCK.CLOSE_ORDER_TASK_LOCK);
        boolean getLock = false;
        try {
            //todo 若任务执行时间过短，则有可能在等锁的过程中2个服务任务都会获取到锁，这与实际需要的功能不一致，故需要将waitTime设置为0
            if (getLock = lock.tryLock(0, 5, TimeUnit.SECONDS)) {
                int hour = Integer.parseInt(PropertiesUtil.getProperty("close.redis.lock.time","2"));
                iOrderService.closeOrder(hour);
            } else {
                log.info("Redisson分布式锁没有获取到锁:{},ThreadName :{}", Const.REDIS_LOCK.CLOSE_ORDER_TASK_LOCK, Thread.currentThread().getName());
            }
        } catch (InterruptedException e) {
           log.error("Redisson 获取分布式锁异常",e);
        }finally {
            if (!getLock) {
                return;
            }
            lock.unlock();
            log.info("Redisson分布式锁释放锁:{},ThreadName :{}", Const.REDIS_LOCK.CLOSE_ORDER_TASK_LOCK, Thread.currentThread().getName());
        }
    }
```
以上就是Redisson的分布式锁实现代码，下面来分析一下：
1.`RLock lock = redissonManager.getRedisson().getLock(String lockName);`
`RLock`继承自`java.util.concurrent.locks.Lock`，可以将其理解为一个重入锁，需要手动加锁和释放锁
来看它其中的一个方法：`tryLock(long waitTime, long leaseTime, TimeUnit unit)`
2.`getLock = lock.tryLock(0, 5, TimeUnit.SECONDS)`
通过`tryLock()`的参数可以看出，在获取该锁时如果被其他线程先拿到锁就会进入等待，等待`waitTime`时间，如果还没用机会获取到锁就放弃，返回false；若获得了锁，除非是调用`unlock`释放，那么会一直持有锁，直到超过`leaseTime`指定的时间。
以上就是Redisson实现分布式锁的核心方法，有人可能要问，那怎么确定拿的是同一把锁，分布式锁在哪？
这就是Redisson的强大之处，其底层还是使用的Redis来作分布式锁，在我们的`RedissonManager`中已经指定了Redis实例，Redisson会进行托管，其原理与我们手动实现Redis分布式锁类似。
