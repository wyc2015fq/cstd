# Redis一个异常的解决办法，异常描述：Could not get a resource from the pool - xqhrs232的专栏 - CSDN博客
2018年09月30日 16:36:51[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：62
个人分类：[JavaWeb技术](https://blog.csdn.net/xqhrs232/article/category/8231541)
原文地址::[https://blog.csdn.net/freebird_lb/article/details/7460328](https://blog.csdn.net/freebird_lb/article/details/7460328)
相关文章
1、常见JedisConnectionException异常分析----[https://blog.csdn.net/fachang/article/details/7984123](https://blog.csdn.net/fachang/article/details/7984123)
异常描述：
` `- 
`redis.clients.jedis.exceptions.JedisConnectionException: Could not get a resource from the pool`
- 
`at redis.clients.util.Pool.getResource(Pool.java:22)`
- 
`at com.derbysoft.jredis.longkeytest.BorrowObject.run(BorrowObject.java:22)`
- 
`at java.lang.Thread.run(Thread.java:662)`
- 
`Caused by: java.util.NoSuchElementException: Timeout waiting for idle object`
- 
`at org.apache.commons.pool.impl.GenericObjectPool.borrowObject(GenericObjectPool.java:1134)`
- 
`at redis.clients.util.Pool.getResource(Pool.java:20)`
- 
`... 2 more`
1、产生原因：客户端去redis服务器拿连接（代码描述的是租用对象borrowObject）的时候，池中无可用连接，即池中所有连接被占用，且在等待时候设定的超时时间后还没拿到时，报出此异常。
2、解决办法：调整JedisPoolConfig中maxActive为适合自己系统的阀值。
<bean id="dataJedisPoolConfig" class="redis.clients.jedis.JedisPoolConfig">
        <property name="maxActive" value="300"/>
       <property name="maxIdle" value="100"/>
        <property name="maxWait" value="10000"/>
        <property name="testOnBorrow" value="true"/>
</bean>
3、重现：
` `- 
`public class BorrowObject implements Runnable {`
- 
`private ShardedJedisPool jedisPool;`
- 
- 
`public BorrowObject(ShardedJedisPool jedisPool) {`
- 
`this.jedisPool = jedisPool;`
- 
`}`
- 
- 
`@Override`
- 
`public void run() {`
- 
`ShardedJedis shardedJedis = null;`
- 
`try {`
- 
`shardedJedis = jedisPool.getResource();`
- 
`String value = shardedJedis.hget("LONG_KEY_TEST:AA059E03E0AB7D806E6C351F87404B06C1190", "Roc El Pinar Aparthotel");`
- 
`System.out.println(value);`
- 
`} catch (Exception e) {`
- 
`//logger.error(e);`
- 
`e.printStackTrace();`
- 
`} finally {`
- 
`jedisPool.returnResource(shardedJedis);`
- 
`}`
- 
`}`
- 
`}`
` `- 
`public class BorrowObjectTest {`
- 
`private ShardedJedisPool jedisPool = null;`
- 
- 
`public BorrowObjectTest() {`
- 
`List<JedisShardInfo> jedisShardInfos = new ArrayList<JedisShardInfo>();`
- 
`JedisShardInfo jedisShardInfo = new JedisShardInfo("192.168.1.112");`
- 
`jedisShardInfo.setTimeout(1000000);`
- 
`jedisShardInfos.add(jedisShardInfo);`
- 
`jedisPool = new ShardedJedisPool(createJedisConfig(), jedisShardInfos);`
- 
`}`
- 
- 
`private JedisPoolConfig createJedisConfig() {`
- 
`JedisPoolConfig jedisConfig = new JedisPoolConfig();`
- 
`jedisConfig.setMaxActive(2);`
- 
`jedisConfig.setMaxIdle(2);`
- 
`jedisConfig.setMaxWait(5);`
- 
`jedisConfig.setTestOnBorrow(true);`
- 
`return jedisConfig;`
- 
`}`
- 
- 
`public static void main(String[] args) {`
- 
`BorrowObjectTest borrowObjectTest = new BorrowObjectTest();`
- 
`for (int i = 0; i < 300; i++) {`
- 
`new Thread(new BorrowObject(borrowObjectTest.jedisPool)).start();`
- 
`}`
- 
`}`
- 
`}`
`转载自：[http://www.iteye.com/topic/1122212](http://www.iteye.com/topic/1122212)`
--------------------- 本文来自 freebird_lb 的CSDN 博客 ，全文地址请点击：https://blog.csdn.net/freebird_lb/article/details/7460328?utm_source=copy
