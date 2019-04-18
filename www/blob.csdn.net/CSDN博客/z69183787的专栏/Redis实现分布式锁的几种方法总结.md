# Redis实现分布式锁的几种方法总结 - z69183787的专栏 - CSDN博客
2018年08月05日 12:36:57[OkidoGreen](https://me.csdn.net/z69183787)阅读数：140
[https://m.jb51.net/article/118312.htm](https://m.jb51.net/article/118312.htm)
这篇文章主要介绍了Redis实现分布式锁的几种方法总结的相关资料, Redis实现与Zookeeper实现和数据库实现，需要的朋友可以参考下
**Redis实现分布式锁的几种方法总结**
分布式锁是控制分布式系统之间同步访问共享资源的一种方式。在分布式系统中，常常需要协调他们的动作。如果不同的系统或是同一个系统的不同主机之间共享了一个或一组资源，那么访问这些资源的时候，往往需要互斥来防止彼此干扰来保证一致性，在这种情况下，便需要使用到分布式锁。
我们来假设一个最简单的秒杀场景：数据库里有一张表，column分别是商品ID，和商品ID对应的库存量，秒杀成功就将此商品库存量-1。现在假设有1000个线程来秒杀两件商品，500个线程秒杀第一个商品，500个线程秒杀第二个商品。我们来根据这个简单的业务场景来解释一下分布式锁。 
通常具有秒杀场景的业务系统都比较复杂，承载的业务量非常巨大，并发量也很高。这样的系统往往采用分布式的架构来均衡负载。那么这1000个并发就会是从不同的地方过来，商品库存就是共享的资源，也是这1000个并发争抢的资源，这个时候我们需要将并发互斥管理起来。这就是分布式锁的应用。 
**1.实现分布式锁的几种方案**
    1.Redis实现   (推荐)
    2.Zookeeper实现
    3.数据库实现
Redis实现分布式锁
*
* 在集群等多服务器中经常使用到同步处理一下业务，这是普通的事务是满足不了业务需求，需要分布式锁
*
* 分布式锁的常用3种实现：
*        0.数据库乐观锁实现
*        1.Redis实现  --- 使用redis的setnx()、get()、getset()方法，用于分布式锁，解决死锁问题
*        2.Zookeeper实现
*           参考：http://surlymo.iteye.com/blog/2082684
*              //www.jb51.net/article/103617.htm
*              http://www.hollischuang.com/archives/1716?utm_source=tuicool&utm_medium=referral
*          1、实现原理：
基于zookeeper瞬时有序节点实现的分布式锁，其主要逻辑如下（该图来自于IBM网站）。大致思想即为：每个客户端对某个功能加锁时，在zookeeper上的与该功能对应的指定节点的目录下，生成一个唯一的瞬时有序节点。判断是否获取锁的方式很简单，只需要判断有序节点中序号最小的一个。当释放锁的时候，只需将这个瞬时节点删除即可。同时，其可以避免服务宕机导致的锁无法释放，而产生的死锁问题。
2、优点
锁安全性高，zk可持久化
3、缺点
性能开销比较高。因为其需要动态产生、销毁瞬时节点来实现锁功能。
4、实现
可以直接采用zookeeper第三方库curator即可方便地实现分布式锁
*
* Redis实现分布式锁的原理：
*  1.通过setnx(lock_timeout)实现，如果设置了锁返回1， 已经有值没有设置成功返回0
*  2.死锁问题：通过实践来判断是否过期，如果已经过期，获取到过期时间get(lockKey)，然后getset(lock_timeout)判断是否和get相同，
*   相同则证明已经加锁成功，因为可能导致多线程同时执行getset(lock_timeout)方法，这可能导致多线程都只需getset后，对于判断加锁成功的线程，
*   再加expire(lockKey, LOCK_TIMEOUT, TimeUnit.MILLISECONDS)过期时间，防止多个线程同时叠加时间，导致锁时效时间翻倍
*  3.针对集群服务器时间不一致问题，可以调用redis的time()获取当前时间
**2.Redis分分布式锁的代码实现**
  1.定义锁接口
package com.jay.service.redis; 
 
/** 
 * Redis分布式锁接口 
 * Created by hetiewei on 2017/4/7. 
 */ 
public interface RedisDistributionLock { 
  /** 
   * 加锁成功，返回加锁时间 
   * @param lockKey 
   * @param threadName 
   * @return 
   */ 
  public long lock(String lockKey, String threadName); 
 
  /** 
   * 解锁， 需要更新加锁时间，判断是否有权限 
   * @param lockKey 
   * @param lockValue 
   * @param threadName 
   */ 
  public void unlock(String lockKey, long lockValue, String threadName); 
 
  /** 
   * 多服务器集群，使用下面的方法，代替System.currentTimeMillis()，获取redis时间，避免多服务的时间不一致问题！！！ 
   * @return 
   */ 
  public long currtTimeForRedis(); 
} 
**   2.定义锁实现**
package com.jay.service.redis.impl; 
 
import com.jay.service.redis.RedisDistributionLock; 
import org.slf4j.Logger; 
import org.slf4j.LoggerFactory; 
import org.springframework.dao.DataAccessException; 
import org.springframework.data.redis.connection.RedisConnection; 
import org.springframework.data.redis.core.RedisCallback; 
import org.springframework.data.redis.core.StringRedisTemplate; 
import org.springframework.data.redis.serializer.RedisSerializer; 
 
import java.util.concurrent.TimeUnit; 
 
/** 
 * Created by hetiewei on 2017/4/7. 
 */ 
public class RedisLockImpl implements RedisDistributionLock { 
 
  //加锁超时时间，单位毫秒， 即：加锁时间内执行完操作，如果未完成会有并发现象 
  private static final long LOCK_TIMEOUT = 5*1000; 
 
  private static final Logger LOG = LoggerFactory.getLogger(RedisLockImpl.class); 
 
  private StringRedisTemplate redisTemplate; 
 
  public RedisLockImpl(StringRedisTemplate redisTemplate) { 
    this.redisTemplate = redisTemplate; 
  } 
 
  /** 
   * 加锁 
   * 取到锁加锁，取不到锁一直等待知道获得锁 
   * @param lockKey 
   * @param threadName 
   * @return 
   */ 
  @Override 
  public synchronized long lock(String lockKey, String threadName) { 
    LOG.info(threadName+"开始执行加锁"); 
    while (true){ //循环获取锁 
      //锁时间 
      Long lock_timeout = currtTimeForRedis()+ LOCK_TIMEOUT +1; 
      if (redisTemplate.execute(new RedisCallback<Boolean>() { 
        @Override 
        public Boolean doInRedis(RedisConnection redisConnection) throws DataAccessException { 
          //定义序列化方式 
          RedisSerializer<String> serializer = redisTemplate.getStringSerializer(); 
          byte[] value = serializer.serialize(lock_timeout.toString()); 
          boolean flag = redisConnection.setNX(lockKey.getBytes(), value); 
          return flag; 
        } 
      })){ 
        //如果加锁成功 
        LOG.info(threadName +"加锁成功 ++++ 111111"); 
        //设置超时时间，释放内存 
        redisTemplate.expire(lockKey, LOCK_TIMEOUT, TimeUnit.MILLISECONDS); 
        return lock_timeout; 
      }else { 
        //获取redis里面的时间 
        String result = redisTemplate.opsForValue().get(lockKey); 
        Long currt_lock_timeout_str = result==null?null:Long.parseLong(result); 
        //锁已经失效 
        if (currt_lock_timeout_str != null && currt_lock_timeout_str < System.currentTimeMillis()){ 
          //判断是否为空，不为空时，说明已经失效，如果被其他线程设置了值，则第二个条件判断无法执行 
          //获取上一个锁到期时间，并设置现在的锁到期时间 
          Long old_lock_timeout_Str = Long.valueOf(redisTemplate.opsForValue().getAndSet(lockKey, lock_timeout.toString())); 
          if (old_lock_timeout_Str != null && old_lock_timeout_Str.equals(currt_lock_timeout_str)){ 
            //多线程运行时，多个线程签好都到了这里，但只有一个线程的设置值和当前值相同，它才有权利获取锁 
            LOG.info(threadName + "加锁成功 ++++ 22222"); 
            //设置超时间，释放内存 
            redisTemplate.expire(lockKey, LOCK_TIMEOUT, TimeUnit.MILLISECONDS); 
 
            //返回加锁时间 
            return lock_timeout; 
          } 
        } 
      } 
 
      try { 
        LOG.info(threadName +"等待加锁， 睡眠100毫秒"); 
//        TimeUnit.MILLISECONDS.sleep(100); 
        TimeUnit.MILLISECONDS.sleep(200); 
      } catch (InterruptedException e) { 
        e.printStackTrace(); 
      } 
    } 
  } 
 
  /** 
   * 解锁 
   * @param lockKey 
   * @param lockValue 
   * @param threadName 
   */ 
  @Override 
  public synchronized void unlock(String lockKey, long lockValue, String threadName) { 
    LOG.info(threadName + "执行解锁==========");//正常直接删除 如果异常关闭判断加锁会判断过期时间 
    //获取redis中设置的时间 
    String result = redisTemplate.opsForValue().get(lockKey); 
    Long currt_lock_timeout_str = result ==null?null:Long.valueOf(result); 
 
    //如果是加锁者，则删除锁， 如果不是，则等待自动过期，重新竞争加锁 
    if (currt_lock_timeout_str !=null && currt_lock_timeout_str == lockValue){ 
      redisTemplate.delete(lockKey); 
      LOG.info(threadName + "解锁成功------------------"); 
    } 
  } 
 
  /** 
   * 多服务器集群，使用下面的方法，代替System.currentTimeMillis()，获取redis时间，避免多服务的时间不一致问题！！！ 
   * @return 
   */ 
  @Override 
  public long currtTimeForRedis(){ 
    return redisTemplate.execute(new RedisCallback<Long>() { 
      @Override 
      public Long doInRedis(RedisConnection redisConnection) throws DataAccessException { 
        return redisConnection.time(); 
      } 
    }); 
  } 
 
} 
**  3.分布式锁验证     **
@RestController 
@RequestMapping("/distribution/redis") 
public class RedisLockController { 
 
  private static final String LOCK_NO = "redis_distribution_lock_no_"; 
 
  private static int i = 0; 
 
  private ExecutorService service; 
 
  @Autowired 
  private StringRedisTemplate redisTemplate; 
 
  /** 
   * 模拟1000个线程同时执行业务，修改资源 
   * 
   * 使用线程池定义了20个线程 
   * 
   */ 
  @GetMapping("lock1") 
  public void testRedisDistributionLock1(){ 
 
    service = Executors.newFixedThreadPool(20); 
 
    for (int i=0;i<1000;i++){ 
      service.execute(new Runnable() { 
        @Override 
        public void run() { 
          task(Thread.currentThread().getName()); 
        } 
      }); 
    } 
 
  } 
 
  @GetMapping("/{key}") 
  public String getValue(@PathVariable("key") String key){ 
    Serializable result = redisTemplate.opsForValue().get(key); 
    return result.toString(); 
  } 
 
  private void task(String name) { 
//    System.out.println(name + "任务执行中"+(i++)); 
 
    //创建一个redis分布式锁 
    RedisLockImpl redisLock = new RedisLockImpl(redisTemplate); 
    //加锁时间 
    Long lockTime; 
    if ((lockTime = redisLock.lock((LOCK_NO+1)+"", name))!=null){ 
      //开始执行任务 
      System.out.println(name + "任务执行中"+(i++)); 
      //任务执行完毕 关闭锁 
      redisLock.unlock((LOCK_NO+1)+"", lockTime, name); 
    } 
 
  } 
} 
**4.结果验证：**
      在Controller中模拟了1000个线程，通过线程池方式提交，每次20个线程抢占分布式锁，抢到分布式锁的执行代码，没抢到的等待
     结果如下：
2017-04-07 16:27:17.385 INFO 8652 --- [pool-2-thread-4] c.jay.service.redis.impl.RedisLockImpl  : pool-2-thread-4等待加锁， 睡眠100毫秒
2017-04-07 16:27:17.385 INFO 8652 --- [pool-2-thread-7] c.jay.service.redis.impl.RedisLockImpl  : pool-2-thread-7解锁成功------------------
    2017-04-07 16:27:17.391 INFO 8652 --- [pool-2-thread-5] c.jay.service.redis.impl.RedisLockImpl  : pool-2-thread-5加锁成功 ++++ 111111
pool-2-thread-5任务执行中994
2017-04-07 16:27:17.391 INFO 8652 --- [pool-2-thread-5] c.jay.service.redis.impl.RedisLockImpl  : pool-2-thread-5执行解锁==========
    2017-04-07 16:27:17.391 INFO 8652 --- [pool-2-thread-1] c.jay.service.redis.impl.RedisLockImpl  : pool-2-thread-1等待加锁， 睡眠100毫秒
2017-04-07 16:27:17.391 INFO 8652 --- [pool-2-thread-5] c.jay.service.redis.impl.RedisLockImpl  : pool-2-thread-5解锁成功------------------
    2017-04-07 16:27:17.397 INFO 8652 --- [pool-2-thread-6] c.jay.service.redis.impl.RedisLockImpl  : pool-2-thread-6加锁成功 ++++ 111111
pool-2-thread-6任务执行中995
2017-04-07 16:27:17.398 INFO 8652 --- [pool-2-thread-6] c.jay.service.redis.impl.RedisLockImpl  : pool-2-thread-6执行解锁==========
    2017-04-07 16:27:17.398 INFO 8652 --- [pool-2-thread-6] c.jay.service.redis.impl.RedisLockImpl  : pool-2-thread-6解锁成功------------------
    2017-04-07 16:27:17.400 INFO 8652 --- [ool-2-thread-19] c.jay.service.redis.impl.RedisLockImpl  : pool-2-thread-19加锁成功 ++++ 111111
pool-2-thread-19任务执行中996
2017-04-07 16:27:17.400 INFO 8652 --- [ool-2-thread-19] c.jay.service.redis.impl.RedisLockImpl  : pool-2-thread-19执行解锁==========
    2017-04-07 16:27:17.400 INFO 8652 --- [ool-2-thread-19] c.jay.service.redis.impl.RedisLockImpl  : pool-2-thread-19解锁成功------------------
    2017-04-07 16:27:17.571 INFO 8652 --- [ool-2-thread-11] c.jay.service.redis.impl.RedisLockImpl  : pool-2-thread-11加锁成功 ++++ 111111
pool-2-thread-11任务执行中997
2017-04-07 16:27:17.572 INFO 8652 --- [ool-2-thread-11] c.jay.service.redis.impl.RedisLockImpl  : pool-2-thread-11执行解锁==========
    2017-04-07 16:27:17.572 INFO 8652 --- [ool-2-thread-11] c.jay.service.redis.impl.RedisLockImpl  : pool-2-thread-11解锁成功------------------
    2017-04-07 16:27:17.585 INFO 8652 --- [pool-2-thread-4] c.jay.service.redis.impl.RedisLockImpl  : pool-2-thread-4加锁成功 ++++ 111111
pool-2-thread-4任务执行中998
2017-04-07 16:27:17.586 INFO 8652 --- [pool-2-thread-4] c.jay.service.redis.impl.RedisLockImpl  : pool-2-thread-4执行解锁==========
    2017-04-07 16:27:17.586 INFO 8652 --- [pool-2-thread-4] c.jay.service.redis.impl.RedisLockImpl  : pool-2-thread-4解锁成功------------------
    2017-04-07 16:27:17.591 INFO 8652 --- [pool-2-thread-1] c.jay.service.redis.impl.RedisLockImpl  : pool-2-thread-1加锁成功 ++++ 111111
pool-2-thread-1任务执行中999
2017-04-07 16:27:17.591 INFO 8652 --- [pool-2-thread-1] c.jay.service.redis.impl.RedisLockImpl  : pool-2-thread-1执行解锁==========
    2017-04-07 16:27:17.591 INFO 8652 --- [pool-2-thread-1] c.jay.service.redis.impl.RedisLockImpl  : pool-2-thread-1解锁成功------------------
