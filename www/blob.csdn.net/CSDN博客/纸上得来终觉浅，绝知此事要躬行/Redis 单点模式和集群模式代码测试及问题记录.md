# Redis 单点模式和集群模式代码测试及问题记录 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年10月28日 11:36:05[boonya](https://me.csdn.net/boonya)阅读数：9329








前言：Redis的测试有一些讲究，如果你是先安装集群再来测试会出现很多意想不到的问题。单个redis-server测试很简单 就是照着API搞就行，但是如果是先搭建集群环境而使用集群节点（单个IP和端口）来操作就不适用了。集群的特点就是分担服务端的压力，此时采用Redis一般的操作API就不适用了，因为集群节点会发生转移（MOVED）到另一个节点上去，所以API操作要对应模式——集群应当使用与cluster API进行操作。Redis访问是建立的c/s模式的socket连接，所以不同的节点要么主机（Host）IP不同要么端口（port）不同，redis以此绑定(bind)IP和port
 (ip:port)。

#### 1.单点模式RedisServer操作示例

##### 1.1普通操作验证：

Connection



```java
package com.boonya.redis.samles;

import redis.clients.jedis.Jedis;

public class RedisTest {
	
	 public static void main(String[] args) {
	      //Connecting to Redis server on 192.168.28.194
	      Jedis jedis = new Jedis("192.168.28.194",6379,1000);
	      System.out.println("Connection to server sucessfully");
	      //check whether server is running or not
	      System.out.println("Server is running: "+jedis.ping());
	      jedis.quit();
	      jedis.disconnect();
	 }

}
```


Key





```java
package com.boonya.redis.samles;

import java.util.Iterator;
import java.util.Set;
import redis.clients.jedis.Jedis;

public class RedisKeyTest {

	public static void main(String[] args) {
		// Connecting to Redis server on 192.168.28.194
		Jedis jedis = new Jedis("192.168.28.194",6379,1000);
		System.out.println("Connection to server sucessfully");
		// store data in redis list
		// Get the stored data and print it
		Set<String> set = jedis.keys("*");
		Iterator<String> it=set.iterator();
		while (it.hasNext()) {
			String key = it.next();
			System.out.println("List of stored keys:: " + key);
		}
		System.out.println("It's ready to work.");
		jedis.quit();
	    jedis.disconnect();
	}

}
```


List





```java
package com.boonya.redis.samles;

import java.util.List;

import redis.clients.jedis.Jedis;

public class RedisListTest {

	public static void main(String[] args) {
		// Connecting to Redis server on 192.168.28.194
		Jedis jedis = new Jedis("192.168.28.194",6379,1000);
		System.out.println("Connection to server sucessfully");
		// store data in redis list
		jedis.lpush("tutorial-list", "Redis");
		jedis.lpush("tutorial-list", "Mongodb");
		jedis.lpush("tutorial-list", "Mysql");
		// Get the stored data and print it
		List<String> list = jedis.lrange("tutorial-list", 0, 5);
		for (int i = 0; i < list.size(); i++) {
			System.out.println("Stored string in redis:: " + list.get(i));
		}
		System.out.println("It's ready to work.");
		jedis.quit();
	    jedis.disconnect();
	}
}
```


String





```java
package com.boonya.redis.samles;

import redis.clients.jedis.Jedis;

public class RedisStringTest {

	public static void main(String[] args) {
		// Connecting to Redis server on 192.168.28.194
		Jedis jedis = new Jedis("192.168.28.194", 6379, 1000);
		System.out.println("Connection to server sucessfully");
		
		// set the data in redis string
		jedis.set("tutorial-name", "Redis tutorial");
		
		// Get the stored data and print it
		System.out.println("Stored string in redis:: "
				+ jedis.get("tutorial-name"));
		jedis.quit();
		jedis.disconnect();
	}

}
```


Server/client





```java
package com.boonya.redis.samles;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import redis.clients.jedis.Jedis;

public class RedisServerTest {
	
	private Jedis jedis;
	
	@Before
	public  void testBeforeClass(){
		jedis = new Jedis("192.168.28.194",6379,1000);
		jedis.clientSetname("boonya-pc");
		jedis.flushAll();
	}
	
	@After
	public void release(){
		jedis.quit();
		jedis.disconnect();
	}
	
	@Test
	public void testKillClient(){
		
		String clientGetname=jedis.clientGetname();
		System.out.println("clientGetname="+clientGetname);
		
		String clientList=jedis.clientList();
		System.out.println("clientList="+clientList);
	}

}
```



##### 1.2客户端测试示例一：



```java
package com.boonya.redis.singlenode;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Set;
import redis.clients.jedis.Jedis;
import redis.clients.jedis.JedisPool;
import redis.clients.jedis.JedisPoolConfig;
import redis.clients.jedis.JedisShardInfo;
import redis.clients.jedis.Pipeline;
import redis.clients.jedis.ShardedJedis;
import redis.clients.jedis.ShardedJedisPipeline;
import redis.clients.jedis.ShardedJedisPool;
import redis.clients.jedis.SortingParams;
import redis.clients.jedis.Transaction;

public class RedisClient {
	
	private String host="192.168.28.194";

	private Jedis jedis;// 非切片额客户端连接

	private JedisPool jedisPool;// 非切片连接池

	private ShardedJedis shardedJedis;// 切片额客户端连接

	private ShardedJedisPool shardedJedisPool;// 切片连接池
	
	private ShardedJedis sharded;

	public RedisClient() {
		initialPool();
		initialShardedPool();
		shardedJedis = shardedJedisPool.getResource();
		jedis = jedisPool.getResource();
		System.out.println("initial completed!");
	}

	/**
	 * 初始化非切片池
	 */
	private void initialPool() {
		// 池基本配置
		JedisPoolConfig config = new JedisPoolConfig();
		// config.setMaxActive(20);
		config.setMaxIdle(5);
		// config.setMaxWait(1000l);
		config.setMaxWaitMillis(1000);
		config.setTestOnBorrow(false);
		jedisPool = new JedisPool(config, host, 6379);
	}

	/**
	 * 初始化切片池
	 */
	private void initialShardedPool() {
		// 池基本配置
		JedisPoolConfig config = new JedisPoolConfig();
		// config.setMaxActive(20);
		config.setMaxIdle(5);
		// /config.setMaxWait(1000l);
		config.setMaxWaitMillis(1000);
		config.setTestOnBorrow(false);
		// slave链接
		List<JedisShardInfo> shards = new ArrayList<JedisShardInfo>();
		shards.add(new JedisShardInfo(host, 6379, "master"));
		// socket模式测试对象初始化
		sharded=new ShardedJedis(shards);
		// 构造池
		shardedJedisPool = new ShardedJedisPool(config, shards);
	}

	private void KeyOperate() {

		System.out
				.println("======================key==========================");
		// 清空数据
		System.out.println("清空库中所有数据：" + jedis.flushDB());
		// 判断key否存在
		System.out.println("判断key999键是否存在：" + shardedJedis.exists("key999"));
		System.out.println("新增key001,value001键值对："
				+ shardedJedis.set("key001", "value001"));
		System.out.println("判断key001是否存在：" + shardedJedis.exists("key001"));
		// 输出系统中所有的key
		System.out.println("新增key002,value002键值对："
				+ shardedJedis.set("key002", "value002"));
		System.out.println("系统中所有键如下：");
		Set<String> keys = jedis.keys("*");
		Iterator<String> it = keys.iterator();
		while (it.hasNext()) {
			String key = it.next();
			System.out.println(key);
		}
		// 删除某个key,若key不存在，则忽略该命令。
		System.out.println("系统中删除key002: " + jedis.del("key002"));
		System.out.println("判断key002是否存在：" + shardedJedis.exists("key002"));
		// 设置 key001的过期时间
		System.out.println("设置 key001的过期时间为5秒:" + jedis.expire("key001", 5));
		try {
			Thread.sleep(2000);
		} catch (InterruptedException e) {
		}
		// 查看某个key的剩余生存时间,单位【秒】.永久生存或者不存在的都返回-1
		System.out.println("查看key001的剩余生存时间：" + jedis.ttl("key001"));
		// 移除某个key的生存时间
		System.out.println("移除key001的生存时间：" + jedis.persist("key001"));
		System.out.println("查看key001的剩余生存时间：" + jedis.ttl("key001"));
		// 查看key所储存的值的类型
		System.out.println("查看key所储存的值的类型：" + jedis.type("key001"));
		/*
		 * 一些其他方法：1、修改键名：jedis.rename("key6", "key0");
		 * 2、将当前db的key移动到给定的db当中：jedis.move("foo", 1)
		 */
	}

	private void StringOperate() {
		System.out
				.println("======================String_1==========================");
		// 清空数据
		System.out.println("清空库中所有数据：" + jedis.flushDB());

		System.out.println("=============增=============");
		jedis.set("key001", "value001");
		jedis.set("key002", "value002");
		jedis.set("key003", "value003");
		System.out.println("已新增的3个键值对如下：");
		System.out.println(jedis.get("key001"));
		System.out.println(jedis.get("key002"));
		System.out.println(jedis.get("key003"));

		System.out.println("=============删=============");
		System.out.println("删除key003键值对：" + jedis.del("key003"));
		System.out.println("获取key003键对应的值：" + jedis.get("key003"));

		System.out.println("=============改=============");
		// 1、直接覆盖原来的数据
		System.out.println("直接覆盖key001原来的数据："
				+ jedis.set("key001", "value001-update"));
		System.out.println("获取key001对应的新值：" + jedis.get("key001"));
		// 2、直接覆盖原来的数据
		System.out.println("在key002原来值后面追加："
				+ jedis.append("key002", "+appendString"));
		System.out.println("获取key002对应的新值" + jedis.get("key002"));

		System.out.println("=============增，删，查（多个）=============");
		/**
		 * mset,mget同时新增，修改，查询多个键值对 等价于： jedis.set("name","ssss");
		 * jedis.set("jarorwar","xxxx");
		 */
		System.out.println("一次性新增key201,key202,key203,key204及其对应值："
				+ jedis.mset("key201", "value201", "key202", "value202",
						"key203", "value203", "key204", "value204"));
		System.out.println("一次性获取key201,key202,key203,key204各自对应的值："
				+ jedis.mget("key201", "key202", "key203", "key204"));
		System.out.println("一次性删除key201,key202："
				+ jedis.del(new String[] { "key201", "key202" }));
		System.out.println("一次性获取key201,key202,key203,key204各自对应的值："
				+ jedis.mget("key201", "key202", "key203", "key204"));
		System.out.println();

		// jedis具备的功能shardedJedis中也可直接使用，下面测试一些前面没用过的方法
		System.out
				.println("======================String_2==========================");
		// 清空数据
		System.out.println("清空库中所有数据：" + jedis.flushDB());

		System.out.println("=============新增键值对时防止覆盖原先值=============");
		System.out.println("原先key301不存在时，新增key301："
				+ shardedJedis.setnx("key301", "value301"));
		System.out.println("原先key302不存在时，新增key302："
				+ shardedJedis.setnx("key302", "value302"));
		System.out.println("当key302存在时，尝试新增key302："
				+ shardedJedis.setnx("key302", "value302_new"));
		System.out.println("获取key301对应的值：" + shardedJedis.get("key301"));
		System.out.println("获取key302对应的值：" + shardedJedis.get("key302"));

		System.out.println("=============超过有效期键值对被删除=============");
		// 设置key的有效期，并存储数据
		System.out.println("新增key303，并指定过期时间为2秒"
				+ shardedJedis.setex("key303", 2, "key303-2second"));
		System.out.println("获取key303对应的值：" + shardedJedis.get("key303"));
		try {
			Thread.sleep(3000);
		} catch (InterruptedException e) {
		}
		System.out.println("3秒之后，获取key303对应的值：" + shardedJedis.get("key303"));

		System.out.println("=============获取原值，更新为新值一步完成=============");
		System.out.println("key302原值："
				+ shardedJedis.getSet("key302", "value302-after-getset"));
		System.out.println("key302新值：" + shardedJedis.get("key302"));

		System.out.println("=============获取子串=============");
		System.out.println("获取key302对应值中的子串："
				+ shardedJedis.getrange("key302", 5, 7));
	}

	private void ListOperate() {
		System.out
				.println("======================list==========================");
		// 清空数据
		System.out.println("清空库中所有数据：" + jedis.flushDB());

		System.out.println("=============增=============");
		shardedJedis.lpush("stringlists", "vector");
		shardedJedis.lpush("stringlists", "ArrayList");
		shardedJedis.lpush("stringlists", "vector");
		shardedJedis.lpush("stringlists", "vector");
		shardedJedis.lpush("stringlists", "LinkedList");
		shardedJedis.lpush("stringlists", "MapList");
		shardedJedis.lpush("stringlists", "SerialList");
		shardedJedis.lpush("stringlists", "HashList");
		shardedJedis.lpush("numberlists", "3");
		shardedJedis.lpush("numberlists", "1");
		shardedJedis.lpush("numberlists", "5");
		shardedJedis.lpush("numberlists", "2");
		System.out.println("所有元素-stringlists："
				+ shardedJedis.lrange("stringlists", 0, -1));
		System.out.println("所有元素-numberlists："
				+ shardedJedis.lrange("numberlists", 0, -1));

		System.out.println("=============删=============");
		// 删除列表指定的值 ，第二个参数为删除的个数（有重复时），后add进去的值先被删，类似于出栈
		System.out.println("成功删除指定元素个数-stringlists："
				+ shardedJedis.lrem("stringlists", 2, "vector"));
		System.out.println("删除指定元素之后-stringlists："
				+ shardedJedis.lrange("stringlists", 0, -1));
		// 删除区间以外的数据
		System.out.println("删除下标0-3区间之外的元素："
				+ shardedJedis.ltrim("stringlists", 0, 3));
		System.out.println("删除指定区间之外元素后-stringlists："
				+ shardedJedis.lrange("stringlists", 0, -1));
		// 列表元素出栈
		System.out.println("出栈元素：" + shardedJedis.lpop("stringlists"));
		System.out.println("元素出栈后-stringlists："
				+ shardedJedis.lrange("stringlists", 0, -1));

		System.out.println("=============改=============");
		// 修改列表中指定下标的值
		shardedJedis.lset("stringlists", 0, "hello list!");
		System.out.println("下标为0的值修改后-stringlists："
				+ shardedJedis.lrange("stringlists", 0, -1));
		System.out.println("=============查=============");
		// 数组长度
		System.out
				.println("长度-stringlists：" + shardedJedis.llen("stringlists"));
		System.out
				.println("长度-numberlists：" + shardedJedis.llen("numberlists"));
		// 排序
		/*
		 * list中存字符串时必须指定参数为alpha，如果不使用SortingParams，而是直接使用sort("list")，
		 * 会出现"ERR One or more scores can't be converted into double"
		 */
		SortingParams sortingParameters = new SortingParams();
		sortingParameters.alpha();
		sortingParameters.limit(0, 3);
		System.out.println("返回排序后的结果-stringlists："
				+ shardedJedis.sort("stringlists", sortingParameters));
		System.out.println("返回排序后的结果-numberlists："
				+ shardedJedis.sort("numberlists"));
		// 子串： start为元素下标，end也为元素下标；-1代表倒数一个元素，-2代表倒数第二个元素
		System.out.println("子串-第二个开始到结束："
				+ shardedJedis.lrange("stringlists", 1, -1));
		// 获取列表指定下标的值
		System.out.println("获取下标为2的元素：" + shardedJedis.lindex("stringlists", 2)
				+ "\n");
	}

	private void SetOperate() {
		System.out
				.println("======================set==========================");
		// 清空数据
		System.out.println("清空库中所有数据：" + jedis.flushDB());

		System.out.println("=============增=============");
		System.out.println("向sets集合中加入元素element001："
				+ jedis.sadd("sets", "element001"));
		System.out.println("向sets集合中加入元素element002："
				+ jedis.sadd("sets", "element002"));
		System.out.println("向sets集合中加入元素element003："
				+ jedis.sadd("sets", "element003"));
		System.out.println("向sets集合中加入元素element004："
				+ jedis.sadd("sets", "element004"));
		System.out.println("查看sets集合中的所有元素:" + jedis.smembers("sets"));
		System.out.println();

		System.out.println("=============删=============");
		System.out.println("集合sets中删除元素element003："
				+ jedis.srem("sets", "element003"));
		System.out.println("查看sets集合中的所有元素:" + jedis.smembers("sets"));
		/*
		 * System.out.println("sets集合中任意位置的元素出栈："+jedis.spop("sets"));//注：出栈元素位置居然不定
		 * ？--无实际意义
		 * System.out.println("查看sets集合中的所有元素:"+jedis.smembers("sets"));
		 */
		System.out.println();

		System.out.println("=============改=============");
		System.out.println();

		System.out.println("=============查=============");
		System.out.println("判断element001是否在集合sets中："
				+ jedis.sismember("sets", "element001"));
		System.out.println("循环查询获取sets中的每个元素：");
		Set<String> set = jedis.smembers("sets");
		Iterator<String> it = set.iterator();
		while (it.hasNext()) {
			Object obj = it.next();
			System.out.println(obj);
		}
		System.out.println();

		System.out.println("=============集合运算=============");
		System.out.println("sets1中添加元素element001："
				+ jedis.sadd("sets1", "element001"));
		System.out.println("sets1中添加元素element002："
				+ jedis.sadd("sets1", "element002"));
		System.out.println("sets1中添加元素element003："
				+ jedis.sadd("sets1", "element003"));
		System.out.println("sets1中添加元素element002："
				+ jedis.sadd("sets2", "element002"));
		System.out.println("sets1中添加元素element003："
				+ jedis.sadd("sets2", "element003"));
		System.out.println("sets1中添加元素element004："
				+ jedis.sadd("sets2", "element004"));
		System.out.println("查看sets1集合中的所有元素:" + jedis.smembers("sets1"));
		System.out.println("查看sets2集合中的所有元素:" + jedis.smembers("sets2"));
		System.out.println("sets1和sets2交集：" + jedis.sinter("sets1", "sets2"));
		System.out.println("sets1和sets2并集：" + jedis.sunion("sets1", "sets2"));
		System.out.println("sets1和sets2差集：" + jedis.sdiff("sets1", "sets2"));// 差集：set1中有，set2中没有的元素
	}

	private void SortedSetOperate() {
		System.out
				.println("======================zset==========================");
		// 清空数据
		System.out.println(jedis.flushDB());

		System.out.println("=============增=============");
		System.out.println("zset中添加元素element001："
				+ shardedJedis.zadd("zset", 7.0, "element001"));
		System.out.println("zset中添加元素element002："
				+ shardedJedis.zadd("zset", 8.0, "element002"));
		System.out.println("zset中添加元素element003："
				+ shardedJedis.zadd("zset", 2.0, "element003"));
		System.out.println("zset中添加元素element004："
				+ shardedJedis.zadd("zset", 3.0, "element004"));
		System.out
				.println("zset集合中的所有元素：" + shardedJedis.zrange("zset", 0, -1));// 按照权重值排序
		System.out.println();

		System.out.println("=============删=============");
		System.out.println("zset中删除元素element002："
				+ shardedJedis.zrem("zset", "element002"));
		System.out
				.println("zset集合中的所有元素：" + shardedJedis.zrange("zset", 0, -1));
		System.out.println();

		System.out.println("=============改=============");
		System.out.println();

		System.out.println("=============查=============");
		System.out.println("统计zset集合中的元素中个数：" + shardedJedis.zcard("zset"));
		System.out.println("统计zset集合中权重某个范围内（1.0——5.0），元素的个数："
				+ shardedJedis.zcount("zset", 1.0, 5.0));
		System.out.println("查看zset集合中element004的权重："
				+ shardedJedis.zscore("zset", "element004"));
		System.out.println("查看下标1到2范围内的元素值："
				+ shardedJedis.zrange("zset", 1, 2));

	}

	private void HashOperate() {
		System.out
				.println("======================hash==========================");
		// 清空数据
		System.out.println(jedis.flushDB());

		System.out.println("=============增=============");
		System.out.println("hashs中添加key001和value001键值对："
				+ shardedJedis.hset("hashs", "key001", "value001"));
		System.out.println("hashs中添加key002和value002键值对："
				+ shardedJedis.hset("hashs", "key002", "value002"));
		System.out.println("hashs中添加key003和value003键值对："
				+ shardedJedis.hset("hashs", "key003", "value003"));
		System.out.println("新增key004和4的整型键值对："
				+ shardedJedis.hincrBy("hashs", "key004", 4l));
		System.out.println("hashs中的所有值：" + shardedJedis.hvals("hashs"));
		System.out.println();

		System.out.println("=============删=============");
		System.out.println("hashs中删除key002键值对："
				+ shardedJedis.hdel("hashs", "key002"));
		System.out.println("hashs中的所有值：" + shardedJedis.hvals("hashs"));
		System.out.println();

		System.out.println("=============改=============");
		System.out.println("key004整型键值的值增加100："
				+ shardedJedis.hincrBy("hashs", "key004", 100l));
		System.out.println("hashs中的所有值：" + shardedJedis.hvals("hashs"));
		System.out.println();

		System.out.println("=============查=============");
		System.out.println("判断key003是否存在："
				+ shardedJedis.hexists("hashs", "key003"));
		System.out.println("获取key004对应的值："
				+ shardedJedis.hget("hashs", "key004"));
		System.out.println("批量获取key001和key003对应的值："
				+ shardedJedis.hmget("hashs", "key001", "key003"));
		System.out.println("获取hashs中所有的key：" + shardedJedis.hkeys("hashs"));
		System.out.println("获取hashs中所有的value：" + shardedJedis.hvals("hashs"));
		System.out.println();
	}

	private void simpleSetGet() {
		jedis.set("key001", "value001");
		jedis.set("key002", "value002");
		jedis.set("key003", "value003");
		System.out.println("已新增的3个键值对如下：");
		System.out.println(jedis.get("key001"));
		System.out.println(jedis.get("key002"));
		System.out.println(jedis.get("key003"));
	}
	
	@SuppressWarnings("deprecation")
	public void show() {
		simpleSetGet();
		KeyOperate();
		StringOperate();
		ListOperate();
		SetOperate();
		SortedSetOperate();
		HashOperate();
		jedisPool.returnResource(jedis);
		shardedJedisPool.returnResource(shardedJedis);
	}
	
	/************************************Socket模式连接测试******************开始**************/
	private void test1Normal() {
		// 设置值
		long start = System.currentTimeMillis();
		for (int i = 0; i < 10; i++) {
			jedis.set("k" + i, "v" + i);
		}
		long end = System.currentTimeMillis();
		System.out.println("Simple SET: " + ((end - start) / 1000.0)
				+ " seconds");
		// 获取值
		for (int i = 0; i < 10; i++) {
			String result = jedis.get("n" + i);
			System.out.println("Simple GET: " + "k" + i + "=" + result);
		}
	}

	private void test2Trans() {
		long start = System.currentTimeMillis();
		Transaction tx = jedis.multi();
		for (int i = 0; i < 10; i++) {
			tx.set("t" + i, "t" + i);
		}
		// System.out.println(tx.get("t1000").get());

		@SuppressWarnings("unused")
		List<Object> results = tx.exec();
		long end = System.currentTimeMillis();
		System.out.println("Transaction SET: " + ((end - start) / 1000.0)
				+ " seconds");
	}

	private void test3Pipelined() {
		Pipeline pipeline = jedis.pipelined();
		long start = System.currentTimeMillis();
		for (int i = 0; i < 10; i++) {
			pipeline.set("p" + i, "p" + i);
		}
		// System.out.println(pipeline.get("p1000").get());
		@SuppressWarnings("unused")
		List<Object> results = pipeline.syncAndReturnAll();
		long end = System.currentTimeMillis();
		System.out.println("Pipelined SET: " + ((end - start) / 1000.0)
				+ " seconds");
	}

	private void test4combPipelineTrans() {
		long start = System.currentTimeMillis();
		Pipeline pipeline = jedis.pipelined();
		pipeline.multi();
		for (int i = 0; i < 10; i++) {
			pipeline.set("" + i, "" + i);
		}
		pipeline.exec();
		@SuppressWarnings("unused")
		List<Object> results = pipeline.syncAndReturnAll();
		long end = System.currentTimeMillis();
		System.out.println("Pipelined transaction: " + ((end - start) / 1000.0)
				+ " seconds");
	}

	@SuppressWarnings("unused")
	private void test5shardNormal() {
		long start = System.currentTimeMillis();
		for (int i = 0; i < 10; i++) {
			String result = sharded.set("sn" + i, "n" + i);
		}
		long end = System.currentTimeMillis();
		System.out.println("Simple@Sharing SET: " + ((end - start) / 1000.0)
				+ " seconds");
	}

	@SuppressWarnings("unused")
	private void test6shardpipelined() {
		ShardedJedisPipeline pipeline = sharded.pipelined();
		long start = System.currentTimeMillis();
		for (int i = 0; i < 10; i++) {
			pipeline.set("sp" + i, "p" + i);
		}
		List<Object> results = pipeline.syncAndReturnAll();
		long end = System.currentTimeMillis();
		System.out.println("Pipelined@Sharing SET: " + ((end - start) / 1000.0)
				+ " seconds");
	}

	@SuppressWarnings("deprecation")
	private void test7shardSimplePool() {
		ShardedJedis one = shardedJedisPool.getResource();

		long start = System.currentTimeMillis();
		for (int i = 0; i < 10; i++) {
			@SuppressWarnings("unused")
			String result = one.set("spn" + i, "n" + i);
		}
		long end = System.currentTimeMillis();
		shardedJedisPool.returnResource(one);
		System.out.println("Simple@Pool SET: " + ((end - start) / 1000.0)
				+ " seconds");
	}

	@SuppressWarnings("deprecation")
	private void test8shardPipelinedPool() {
		ShardedJedis one = shardedJedisPool.getResource();

		ShardedJedisPipeline pipeline = one.pipelined();

		long start = System.currentTimeMillis();
		for (int i = 0; i < 10; i++) {
			pipeline.set("sppn" + i, "n" + i);
		}
		@SuppressWarnings("unused")
		List<Object> results = pipeline.syncAndReturnAll();
		long end = System.currentTimeMillis();
		shardedJedisPool.returnResource(one);
		System.out.println("Pipelined@Pool SET: " + ((end - start) / 1000.0)
				+ " seconds");
	}
	
	public void testSockets(){
		test1Normal();
		test2Trans();
		test3Pipelined();
		test4combPipelineTrans();
		test5shardNormal();
		test6shardpipelined();
		test7shardSimplePool();
		test8shardPipelinedPool();
	}
	/************************************Socket模式连接测试******************结束**************/

	public static void main(String[] args) {
		RedisClient client = new RedisClient();
		client.show();
		client.testSockets();
	}

}
```



##### 1.3客户端测试示例二：

Test



```java
package com.boonya.redis.singlenode;

import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import org.junit.Before;
import org.junit.Test;
import com.boonya.redis.util.RedisPoolUtil;
import redis.clients.jedis.Jedis;

public class RedisTest {

	private Jedis jedis;

	@Before
	public void setup() {
		// 连接redis服务器，192.168.28.194:6379
		jedis = new Jedis("192.168.28.194", 6379,1000);
		// 权限认证
		// jedis.auth("boonya");
	}

	@Test
	public void testStringSetAndGet() {
		jedis.set("name", "boonya");
		System.out.println("name=" + jedis.get("name"));
	}

	/**
	 * redis存储字符串
	 */
	@Test
	public void testString() {
		// -----添加数据----------
		jedis.set("name", "niuniu");
		System.out.println(jedis.get("name"));

		jedis.append("name", " is my lover"); // 拼接
		System.out.println(jedis.get("name"));

		jedis.del("name"); // 删除某个键
		System.out.println(jedis.get("name"));
		// 设置多个键值对
		jedis.mset("name", "boonya", "age", "23", "qq", "476777XXX");
		jedis.incr("age"); // 进行加1操作
		System.out.println(jedis.get("name") + "-" + jedis.get("age") + "-"
				+ jedis.get("qq"));

	}

	/**
	 * redis操作Map
	 */
	@Test
	public void testMap() {
		// -----添加数据----------
		Map<String, String> map = new HashMap<String, String>();
		map.put("name", "niuniu");
		map.put("age", "22");
		map.put("qq", "123456");
		jedis.hmset("user", map);
		// 取出user中的name，执行结果:[minxr]-->注意结果是一个泛型的List
		// 第一个参数是存入redis中map对象的key，后面跟的是放入map中的对象的key，后面的key可以跟多个，是可变参数
		List<String> rsmap = jedis.hmget("user", "name", "age", "qq");
		System.out.println(rsmap);

		// 删除map中的某个键值
		jedis.hdel("user", "age");
		System.out.println(jedis.hmget("user", "age")); // 因为删除了，所以返回的是null
		System.out.println(jedis.hlen("user")); // 返回key为user的键中存放的值的个数2
		System.out.println(jedis.exists("user"));// 是否存在key为user的记录 返回true
		System.out.println(jedis.hkeys("user"));// 返回map对象中的所有key
		System.out.println(jedis.hvals("user"));// 返回map对象中的所有value

		Iterator<String> iter = jedis.hkeys("user").iterator();
		while (iter.hasNext()) {
			String key = iter.next();
			System.out.println(key + ":" + jedis.hmget("user", key));
		}
	}

	/**
	 * jedis操作List
	 */
	@Test
	public void testList() {
		// 开始前，先移除所有的内容
		jedis.del("java framework");
		System.out.println(jedis.lrange("java framework", 0, -1));
		// 先向key java framework中存放三条数据
		jedis.lpush("java framework", "spring");
		jedis.lpush("java framework", "struts");
		jedis.lpush("java framework", "hibernate");
		// 再取出所有数据jedis.lrange是按范围取出，
		// 第一个是key，第二个是起始位置，第三个是结束位置，jedis.llen获取长度 -1表示取得所有
		System.out.println(jedis.lrange("java framework", 0, -1));

		jedis.del("java framework");
		jedis.rpush("java framework", "spring");
		jedis.rpush("java framework", "struts");
		jedis.rpush("java framework", "hibernate");
		System.out.println(jedis.lrange("java framework", 0, -1));
	}

	/**
	 * jedis操作Set
	 */
	@Test
	public void testSet() {
		// 添加
		jedis.sadd("user", "boonya");
		jedis.sadd("user", "niuniu");
		jedis.sadd("user", "ling");
		jedis.sadd("user", "guoniuniu");
		jedis.sadd("user", "who");
		// 移除noname
		jedis.srem("user", "who");
		System.out.println(jedis.smembers("user"));// 获取所有加入的value
		System.out.println(jedis.sismember("user", "who"));// 判断 who
															// 是否是user集合的元素
		System.out.println(jedis.srandmember("user"));
		System.out.println(jedis.scard("user"));// 返回集合的元素个数
	}

	@Test
	public void test() throws InterruptedException {
		// jedis 排序
		// 注意，此处的rpush和lpush是List的操作。是一个双向链表（但从表现来看的）
		jedis.del("a");// 先清除数据，再加入数据进行测试
		jedis.rpush("a", "1");
		jedis.lpush("a", "6");
		jedis.lpush("a", "3");
		jedis.lpush("a", "9");
		System.out.println(jedis.lrange("a", 0, -1));// [9, 3, 6, 1]
		System.out.println(jedis.sort("a")); // [1, 3, 6, 9] //输入排序后结果
		System.out.println(jedis.lrange("a", 0, -1));
	}

	@Test
	public void testRedisPool() {
		RedisPoolUtil.getJedis().set("username", "boonya");
		System.out.println(RedisUtil.getJedis().get("username"));
	}

}
```



RedisPoolUtil



```java
package com.boonya.redis.util;

import redis.clients.jedis.Jedis;
import redis.clients.jedis.JedisPool;
import redis.clients.jedis.JedisPoolConfig;
/**
 * @packge com.boonya.redis.util.RedisUtil
 * @date   2015年10月27日  下午2:13:12
 * @author wlyd
 * @comment   Redis工具类
 * @update
 */
public class RedisPoolUtil {
	
	 //Redis服务器IP
    private static String ADDR = "192.168.28.194";
    
    //Redis的端口号（用于单个redis节点而非集群模式）
    private static int PORT = 6379;
    
    //访问密码，Redis默认没有密码
    private static String AUTH = "";
    
    //可用连接实例的最大数目，默认值为8；
    //如果赋值为-1，则表示不限制；如果pool已经分配了maxActive个jedis实例，则此时pool的状态为exhausted(耗尽)。
    private static int MAX_ACTIVE = 1024;
    
    //控制一个pool最多有多少个状态为idle(空闲的)的jedis实例，默认值也是8。
    private static int MAX_IDLE = 200;
    
    //等待可用连接的最大时间，单位毫秒，默认值为-1，表示永不超时。如果超过等待时间，则直接抛出JedisConnectionException；
    private static int MAX_WAIT = 10000;
    
    private static int TIMEOUT = 10000;
    
    //在borrow一个jedis实例时，是否提前进行validate操作；如果为true，则得到的jedis实例均是可用的；
    private static boolean TEST_ON_BORROW = true;
    
    private static JedisPool jedisPool = null;
    
    /**
     * 初始化Redis连接池
     */
    static {
        try {
            JedisPoolConfig config = new JedisPoolConfig();
            //config.setMaxActive(MAX_ACTIVE);
            config.setMaxIdle(MAX_IDLE);
            //config.setMaxWait(MAX_WAIT);
            config.setMaxWaitMillis(MAX_WAIT);
            config.setTestOnBorrow(TEST_ON_BORROW);
            jedisPool = new JedisPool(config, ADDR, PORT, TIMEOUT);//, AUTH);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    
    /**
     * 获取Jedis实例
     * @return
     */
    public synchronized static Jedis getJedis() {
        try {
            if (jedisPool != null) {
                Jedis resource = jedisPool.getResource();
                return resource;
            } else {
                return null;
            }
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }
    
    /**
     * 释放jedis资源
     * @param jedis
     */
    @SuppressWarnings("deprecation")
	public static void returnResource(final Jedis jedis) {
        if (jedis != null) {
            jedisPool.returnResource(jedis);
        }
    }

}
```



#### 2.集群模式操作示例



```java
package com.boonya.redis.cluster;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;
import org.junit.Test;
import redis.clients.jedis.HostAndPort;
import redis.clients.jedis.JedisCluster;
import junit.framework.TestCase;
/**
 * 
 * @packge com.boonya.redis.RedisClusterTest
 * @date   2015年10月21日  下午2:31:38
 * @author pengjunlin
 * @comment    jedis客户端的坑.
1)cluster环境下redis的slave不接受任何读写操作，

2)client端不支持keys批量操作,不支持select dbNum操作，只有一个db:select 0

3)JedisCluster 的info()等单机函数无法调用,返回(No way to dispatch this command to Redis Cluster)错误，.
 * @update
 * @from http://blog.csdn.net/myrainblues/article/details/25881535
 */
public class ClusterTest extends TestCase {
	
	private static String host="192.168.28.194";
	
	private static JedisCluster cluster;  
	
	static {
		// 只给集群里一个实例就可以
		Set<HostAndPort> jedisClusterNodes = new HashSet<HostAndPort>();
		jedisClusterNodes.add(new HostAndPort(host, 7000));
		jedisClusterNodes.add(new HostAndPort(host, 7001));
		jedisClusterNodes.add(new HostAndPort(host, 7002));
		jedisClusterNodes.add(new HostAndPort(host, 7003));
		jedisClusterNodes.add(new HostAndPort(host, 7004));
		jedisClusterNodes.add(new HostAndPort(host, 7005));
		
		cluster = new JedisCluster(jedisClusterNodes);
	}

	@Test
	public void testSetString(){
		cluster.set("username", "boonya");
		System.out.println("set username "+cluster.get("username"));
	}
	
	@Test
	public void testAppendString(){
		cluster.append("username", " peng");
		System.out.println("append username "+cluster.get("username"));
	}
	
	@Test
	public void testDelString(){
		cluster.set("username", "boonya xnn");
		System.out.println("del-get username "+cluster.get("username"));
		cluster.del("username");
		System.out.println("del-did username "+cluster.get("username"));
	}
	
	@Test
	public void testMap(){
		// -----添加数据----------
		Map<String, String> map = new HashMap<String, String>();
		map.put("name", "boonya");
		map.put("age", "88");
		map.put("qq", "123456");
		cluster.hmset("user", map);
		// 第一个参数是存入redis中map对象的key，后面跟的是放入map中的对象的key，后面的key可以跟多个，是可变参数
		List<String> rsmap = cluster.hmget("user", "name", "age", "qq");
		System.out.println(rsmap);

		// 删除map中的某个键值
		cluster.hdel("user", "age");
		System.out.println(cluster.hmget("user", "age")); // 因为删除了，所以返回的是null
		System.out.println(cluster.hlen("user")); // 返回key为user的键中存放的值的个数2
		System.out.println(cluster.exists("user"));// 是否存在key为user的记录 返回true
		System.out.println(cluster.hkeys("user"));// 返回map对象中的所有key
		System.out.println(cluster.hvals("user"));// 返回map对象中的所有value

		Iterator<String> iter = cluster.hkeys("user").iterator();
		while (iter.hasNext()) {
			String key = iter.next();
			System.out.println(key + ":" + cluster.hmget("user", key));
		}
	}
	
	@Test
	public void testList() {
		// 开始前，先移除所有的内容
		cluster.del("java framework");
		System.out.println(cluster.lrange("java framework", 0, -1));
		// 先向key java framework中存放三条数据
		cluster.lpush("java framework", "spring");
		cluster.lpush("java framework", "struts");
		cluster.lpush("java framework", "hibernate");
		// 再取出所有数据jedis.lrange是按范围取出，
		// 第一个是key，第二个是起始位置，第三个是结束位置，jedis.llen获取长度 -1表示取得所有
		System.out.println(cluster.lrange("java framework", 0, -1));

		cluster.del("java framework");
		cluster.rpush("java framework", "spring");
		cluster.rpush("java framework", "struts");
		cluster.rpush("java framework", "hibernate");
		System.out.println(cluster.lrange("java framework", 0, -1));
	}

	@Test
	public void testSet() {
		// 添加
		cluster.sadd("user","boonya");
		cluster.sadd("user", "niuniu");
		cluster.sadd("user", "ling");
		cluster.sadd("user", "guoniuniu");
		cluster.sadd("user", "who");
		//cluster.sadd("user",new String []{ "boonya","niuniu","ling","guoniuniu","who"});

		// 移除noname
		cluster.srem("user", "who");
		System.out.println(cluster.smembers("user"));// 获取所有加入的value
		System.out.println(cluster.sismember("user", "who"));// 判断 who
															// 是否是user集合的元素
		System.out.println(cluster.srandmember("user"));
		System.out.println(cluster.scard("user"));// 返回集合的元素个数
	}
	
	@Test
	public void testUserSession(){
		// -----添加数据----------
		Map<String, String> map = new HashMap<String, String>();
		map.put("zhangsan", "zhangsan-v");
		map.put("lisi", "lisi-v");
		map.put("wangwu", "wangwu-v");
		cluster.hmset("usersession", map);
		// 第一个参数是存入redis中map对象的key，后面跟的是放入map中的对象的key，后面的key可以跟多个，是可变参数
		List<String> rsmap = cluster.hmget("usersession","zhangsan","lisi");
		System.out.println(rsmap);
	}
	
}
```

注：此处testSet报redis.clients.jedis.exceptions.JedisDataException: WRONGTYPE Operation against a key holding the wrong kind of value异常，这个应该是redis的问题。另外，redis配置文件默认Slave节点只可读不可写。



#### 3.测试过程中遇到的问题

##### 3.1提示权限验证失败

错误栈内信息：



```
redis.clients.jedis.exceptions.JedisDataException: ERR Client sent AUTH, but no password is set
	at redis.clients.jedis.Protocol.processError(Protocol.java:117)
	at redis.clients.jedis.Protocol.process(Protocol.java:151)
	at redis.clients.jedis.Protocol.read(Protocol.java:205)
	at redis.clients.jedis.Connection.readProtocolWithCheckingBroken(Connection.java:297)
	at redis.clients.jedis.Connection.getStatusCodeReply(Connection.java:196)
	at redis.clients.jedis.BinaryClient.connect(BinaryClient.java:85)
	at redis.clients.jedis.Connection.sendCommand(Connection.java:100)
	at redis.clients.jedis.Connection.sendCommand(Connection.java:91)
	at redis.clients.jedis.BinaryClient.auth(BinaryClient.java:538)
	at redis.clients.jedis.BinaryJedis.auth(BinaryJedis.java:2012)
	at com.boonya.redis.MyJedisTest.setup(MyJedisTest.java:24)
	at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)
	at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:57)
	at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43)
	at java.lang.reflect.Method.invoke(Method.java:606)
	at org.junit.runners.model.FrameworkMethod$1.runReflectiveCall(FrameworkMethod.java:47)
	at org.junit.internal.runners.model.ReflectiveCallable.run(ReflectiveCallable.java:12)
	at org.junit.runners.model.FrameworkMethod.invokeExplosively(FrameworkMethod.java:44)
	at org.junit.internal.runners.statements.RunBefores.evaluate(RunBefores.java:24)
	at org.junit.runners.ParentRunner.runLeaf(ParentRunner.java:271)
	at org.junit.runners.BlockJUnit4ClassRunner.runChild(BlockJUnit4ClassRunner.java:70)
	at org.junit.runners.BlockJUnit4ClassRunner.runChild(BlockJUnit4ClassRunner.java:50)
	at org.junit.runners.ParentRunner$3.run(ParentRunner.java:238)
	at org.junit.runners.ParentRunner$1.schedule(ParentRunner.java:63)
	at org.junit.runners.ParentRunner.runChildren(ParentRunner.java:236)
	at org.junit.runners.ParentRunner.access$000(ParentRunner.java:53)
	at org.junit.runners.ParentRunner$2.evaluate(ParentRunner.java:229)
	at org.junit.runners.ParentRunner.run(ParentRunner.java:309)
	at org.eclipse.jdt.internal.junit4.runner.JUnit4TestReference.run(JUnit4TestReference.java:50)
	at org.eclipse.jdt.internal.junit.runner.TestExecution.run(TestExecution.java:38)
	at org.eclipse.jdt.internal.junit.runner.RemoteTestRunner.runTests(RemoteTestRunner.java:459)
	at org.eclipse.jdt.internal.junit.runner.RemoteTestRunner.runTests(RemoteTestRunner.java:675)
	at org.eclipse.jdt.internal.junit.runner.RemoteTestRunner.run(RemoteTestRunner.java:382)
	at org.eclipse.jdt.internal.junit.runner.RemoteTestRunner.main(RemoteTestRunner.java:192)
```



因为redis默认没有设置密码，所以在访问的时候可以不设置auth.若要使用密码方式访问，需要在redis.conf中设置requirepass  yourpass.

##### 3.2集群宕机提示


` redis.clients.jedis.exceptions.JedisClusterException: CLUSTERDOWN The cluster is down`

当集群不可用时,所有对集群的操作做都不可用，收到CLUSTERDOWN The cluster is down错误，一般可以认为是集群节点挂掉了，另外一种原因是API调用错误。


##### 3.3节点访问转移错误



```
Exception in thread "main" redis.clients.jedis.exceptions.JedisMovedDataException: MOVED 5474 192.168.28.194:7001
    at redis.clients.jedis.Protocol.processError(Protocol.java:108)
    at redis.clients.jedis.Protocol.process(Protocol.java:151)
    at redis.clients.jedis.Protocol.read(Protocol.java:205)
    at redis.clients.jedis.Connection.readProtocolWithCheckingBroken(Connection.java:297)
    at redis.clients.jedis.Connection.getStatusCodeReply(Connection.java:196)
    at redis.clients.jedis.Jedis.hmset(Jedis.java:644)
    at com.boonya.redis.samles.RedisMapJava.main(RedisMapJava.java:19)
```
MOVED indicates that you're using Redis Cluster. ShardedJedis is not for Redis Cluster, so you should use JedisCluster instead. 不应该调用集群节点，API调用方式错误。

From:http://stackoverflow.com/questions/30150704/jedismoveddataexception-when-using-sadd-with-pipeline



3.4错误的数据类型



```
redis.clients.jedis.exceptions.JedisDataException: WRONGTYPE Operation against a key holding the wrong kind of value
	at redis.clients.jedis.Protocol.processError(Protocol.java:117)
	at redis.clients.jedis.Protocol.process(Protocol.java:151)
	at redis.clients.jedis.Protocol.read(Protocol.java:205)
	at redis.clients.jedis.Connection.readProtocolWithCheckingBroken(Connection.java:297)
	at redis.clients.jedis.Connection.getIntegerReply(Connection.java:222)
	at redis.clients.jedis.Jedis.sadd(Jedis.java:1048)
	at redis.clients.jedis.JedisCluster$49.execute(JedisCluster.java:559)
	at redis.clients.jedis.JedisCluster$49.execute(JedisCluster.java:556)
	at redis.clients.jedis.JedisClusterCommand.runWithRetries(JedisClusterCommand.java:56)
	at redis.clients.jedis.JedisClusterCommand.run(JedisClusterCommand.java:29)
	at redis.clients.jedis.JedisCluster.sadd(JedisCluster.java:556)
	at com.boonya.redis.cluster.ClusterTest.testSet(ClusterTest.java:120)
	at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)
	at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:57)
	at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43)
	at java.lang.reflect.Method.invoke(Method.java:606)
	at junit.framework.TestCase.runTest(TestCase.java:176)
	at junit.framework.TestCase.runBare(TestCase.java:141)
	at junit.framework.TestResult$1.protect(TestResult.java:122)
	at junit.framework.TestResult.runProtected(TestResult.java:142)
	at junit.framework.TestResult.run(TestResult.java:125)
	at junit.framework.TestCase.run(TestCase.java:129)
	at junit.framework.TestSuite.runTest(TestSuite.java:255)
	at junit.framework.TestSuite.run(TestSuite.java:250)
	at org.junit.internal.runners.JUnit38ClassRunner.run(JUnit38ClassRunner.java:84)
	at org.eclipse.jdt.internal.junit4.runner.JUnit4TestReference.run(JUnit4TestReference.java:50)
	at org.eclipse.jdt.internal.junit.runner.TestExecution.run(TestExecution.java:38)
	at org.eclipse.jdt.internal.junit.runner.RemoteTestRunner.runTests(RemoteTestRunner.java:459)
	at org.eclipse.jdt.internal.junit.runner.RemoteTestRunner.runTests(RemoteTestRunner.java:675)
	at org.eclipse.jdt.internal.junit.runner.RemoteTestRunner.run(RemoteTestRunner.java:382)
	at org.eclipse.jdt.internal.junit.runner.RemoteTestRunner.main(RemoteTestRunner.java:192)
```

这是操作set sadd出现的问题，设置数据类型和方式并没有错，但是redis提示错误，后期使用中有待关注。











