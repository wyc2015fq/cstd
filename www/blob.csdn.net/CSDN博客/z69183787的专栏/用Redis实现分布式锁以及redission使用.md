# 用Redis实现分布式锁以及redission使用 - z69183787的专栏 - CSDN博客
2018年02月21日 19:19:34[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5283
[https://my.oschina.net/wangnian/blog/668830](https://my.oschina.net/wangnian/blog/668830)
前言：分布式环境有很多问题，比如你前一个请求访问的是服务器A，第二个请求访问到了服务器B，就会发生并发重复插入问题，这个时候需要依赖单点来实现分布锁，而redis就是。
先导入maven依赖  redission
```
<dependency>
    <groupId>org.redisson</groupId>
    <artifactId>redisson</artifactId>
    <version>2.2.13</version>
</dependency>
```
创建redission工具类
```java
package redis;
import org.redisson.Config;
import org.redisson.Redisson;
import org.redisson.RedissonClient;
import org.redisson.core.RAtomicLong;
import org.redisson.core.RBucket;
import org.redisson.core.RCountDownLatch;
import org.redisson.core.RDeque;
import org.redisson.core.RList;
import org.redisson.core.RLock;
import org.redisson.core.RMap;
import org.redisson.core.RQueue;
import org.redisson.core.RSet;
import org.redisson.core.RSortedSet;
import org.redisson.core.RTopic;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
/***
 * Redis client的辅助工具类
 * 用于连接Redis服务器 创建不同的Redis Server对应的客户端对象
 * @author wangnian
 * 博客地址：http://my.oschina.net/wangnian
 */
public class RedisUtils {
   private static   Logger logger= LoggerFactory.getLogger(RedisUtils.class);
   
   private static RedisUtils redisUtils;
   
   private RedisUtils(){}
   
   /**
    * 提供单例模式
    * @return
    */
   public static RedisUtils getInstance(){
      if(redisUtils==null) 
         synchronized (RedisUtils.class) {
            if(redisUtils==null) redisUtils=new RedisUtils();
         }
      return redisUtils;
   }
   
   
   /**
    * 使用config创建Redisson
    * Redisson是用于连接Redis Server的基础类
    * @param config
    * @return
    */
   public RedissonClient getRedisson(Config config){
      RedissonClient redisson=Redisson.create(config);
      logger.info("成功连接Redis Server");
      return redisson;
   } 
   
   /**
    * 使用ip地址和端口创建Redisson
    * @param ip
    * @param port
    * @return
    */
   public RedissonClient getRedisson(String ip,String port){
      Config config=new Config();
      config.useSingleServer().setAddress(ip+":"+port);
      RedissonClient redisson=Redisson.create(config);
      logger.info("成功连接Redis Server"+"\t"+"连接"+ip+":"+port+"服务器");
      return redisson;
   }
   
   /**
    * 关闭Redisson客户端连接
    * @param redisson
    */
   public void closeRedisson(RedissonClient redisson){
      redisson.shutdown();
      logger.info("成功关闭Redis Client连接");
   }
   
   /**
    * 获取字符串对象
    * @param redisson
    * @param objectName
    * @return
    */
   public <T> RBucket<T> getRBucket(RedissonClient redisson,String objectName){
      RBucket<T> bucket=redisson.getBucket(objectName);
      return bucket;
   }
   
   /**
    * 获取Map对象
    * @param redisson
    * @param objectName
    * @return
    */
   public <K,V> RMap<K, V> getRMap(RedissonClient redisson,String objectName){
      RMap<K, V> map=redisson.getMap(objectName);
      return map;
   }
   
   /**
    * 获取有序集合
    * @param redisson
    * @param objectName
    * @return
    */
   public <V> RSortedSet<V> getRSortedSet(RedissonClient redisson,String objectName){
      RSortedSet<V> sortedSet=redisson.getSortedSet(objectName);
      return sortedSet;
   }
   
   /**
    * 获取集合
    * @param redisson
    * @param objectName
    * @return
    */
   public <V> RSet<V> getRSet(RedissonClient redisson,String objectName){
      RSet<V> rSet=redisson.getSet(objectName);
      return rSet;
   }
   
   /**
    * 获取列表
    * @param redisson
    * @param objectName
    * @return
    */
   public <V> RList<V> getRList(RedissonClient redisson,String objectName){
      RList<V> rList=redisson.getList(objectName);
      return rList;
   }
   
   /**
    * 获取队列
    * @param redisson
    * @param objectName
    * @return
    */
   public <V> RQueue<V> getRQueue(RedissonClient redisson,String objectName){
      RQueue<V> rQueue=redisson.getQueue(objectName);
      return rQueue;
   }
   
   /**
    * 获取双端队列
    * @param redisson
    * @param objectName
    * @return
    */
   public <V> RDeque<V> getRDeque(RedissonClient redisson,String objectName){
      RDeque<V> rDeque=redisson.getDeque(objectName);
      return rDeque;
   }
   
   /**
    * 此方法不可用在Redisson 1.2 中 
    * 在1.2.2版本中 可用
    * @param redisson
    * @param objectName
    * @return
    */
   /**
   public <V> RBlockingQueue<V> getRBlockingQueue(RedissonClient redisson,String objectName){
      RBlockingQueue rb=redisson.getBlockingQueue(objectName);
      return rb;
   }*/
   
   /**
    * 获取锁
    * @param redisson
    * @param objectName
    * @return
    */
   public RLock getRLock(RedissonClient redisson,String objectName){
      RLock rLock=redisson.getLock(objectName);
      return rLock;
   }
   
   /**
    * 获取原子数
    * @param redisson
    * @param objectName
    * @return
    */
   public RAtomicLong getRAtomicLong(RedissonClient redisson,String objectName){
      RAtomicLong rAtomicLong=redisson.getAtomicLong(objectName);
      return rAtomicLong;
   }
   
   /**
    * 获取记数锁
    * @param redisson
    * @param objectName
    * @return
    */
   public RCountDownLatch getRCountDownLatch(RedissonClient redisson,String objectName){
      RCountDownLatch rCountDownLatch=redisson.getCountDownLatch(objectName);
      return rCountDownLatch;
   }
   
   /**
    * 获取消息的Topic
    * @param redisson
    * @param objectName
    * @return
    */
   public <M> RTopic<M> getRTopic(RedissonClient redisson,String objectName){
       RTopic<M> rTopic=redisson.getTopic(objectName);
       return rTopic;
   }
   
   
}
```
例子
```java
package redis;
import org.junit.Test;
import org.redisson.Config;
import org.redisson.RedissonClient;
import org.redisson.SingleServerConfig;
import org.redisson.core.RBucket;
import org.redisson.core.RLock;
import java.util.concurrent.TimeUnit;
/**
 * Created by wangnian on 2016/5/2.
 *博客地址：http://my.oschina.net/wangnian
 */
public class RedissonTest {
    @Test
    public void test() throws InterruptedException {
        //redisson配置
        Config config = new Config();
        SingleServerConfig singleSerververConfig = config.useSingleServer();
        singleSerververConfig.setAddress("127.0.0.1:6379");
        singleSerververConfig.setPassword("redis");
        //redisson客户端
        RedissonClient redissonClient = RedisUtils.getInstance().getRedisson(config);
        RBucket<Object> rBucket = RedisUtils.getInstance().getRBucket(redissonClient, "key");
        //rBucket.set("wangnian");
        System.out.println(rBucket.get());
        while (true) {
            RLock lock = redissonClient.getLock("lock");
            lock.tryLock(0, 1, TimeUnit.SECONDS);//第一个参数代表等待时间，第二是代表超过时间释放锁，第三个代表设置的时间制
            try {
                System.out.println("执行");
            } finally {
                lock.unlock();
            }
        }
    }
}
```
如果生产上，我建议用 注解aop的方式 写个 lock的注解  比如  @lock("name")
