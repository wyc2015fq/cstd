# Redis分布式客户端-Redisson介绍 - z69183787的专栏 - CSDN博客
2018年02月22日 11:17:56[OkidoGreen](https://me.csdn.net/z69183787)阅读数：529
[http://blog.csdn.net/csujiangyu/article/details/51005342](http://blog.csdn.net/csujiangyu/article/details/51005342)
## 简介
Redisson - 是一个高级的分布式协调Redis客服端，能帮助用户在分布式环境中轻松实现一些Java的对象 (Bloom filter, BitSet, Set, SetMultimap, ScoredSortedSet, SortedSet, Map, ConcurrentMap, List, ListMultimap, Queue, BlockingQueue, Deque, BlockingDeque, Semaphore, Lock, ReadWriteLock, AtomicLong, CountDownLatch, Publish / Subscribe, HyperLogLog)。
## 支持Redis多种连接模式
### 集群模式
```
Config config = new Config();
config.useClusterServers()
    .setScanInterval(2000) // cluster state scan interval in milliseconds
    .addNodeAddress("127.0.0.1:7000", "127.0.0.1:7001")
    .addNodeAddress("127.0.0.1:7002");
RedissonClient redisson = Redisson.create(config);
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
### 单例模式
```
// connects to 127.0.0.1:6379 by default
RedissonClient redisson = Redisson.create();
Config config = new Config();
config.useSingleServer().setAddress("myredisserver:6379");
RedissonClient redisson = Redisson.create(config);
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
### 哨兵模式
```
Config config = new Config();
config.useSentinelServers()
    .setMasterName("mymaster")
    .addSentinelAddress("127.0.0.1:26389", "127.0.0.1:26379")
    .addSentinelAddress("127.0.0.1:26319");
RedissonClient redisson = Redisson.create(config);
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
### 主从模式
```
Config config = new Config();
config.useMasterSlaveServers()
    .setMasterAddress("127.0.0.1:6379")
    .addSlaveAddress("127.0.0.1:6389", "127.0.0.1:6332", "127.0.0.1:6419")
    .addSlaveAddress("127.0.0.1:6399");
RedissonClient redisson = Redisson.create(config);
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
## 操作执行
Redisson支持自动重试策略，默认是重试3次，间隔为1000ms。除了支持同步操作外，还支持异步方式和响应方式。
```
RedissonClient client = Redisson.create(config);
RAtomicLong longObject = client.getAtomicLong('myLong');
// sync way
longObject.compareAndSet(3, 401);
// async way
longObject.compareAndSetAsync(3, 401);
RedissonReactiveClient client = Redisson.createReactive(config);
RAtomicLongReactive longObject = client.getAtomicLong('myLong');
// reactive way
longObject.compareAndSet(3, 401);
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
## 多种序列号方式
Codec class name Description 
org.redisson.codec.JsonJacksonCodec Jackson JSON codec. Default codec 
org.redisson.codec.CborJacksonCodec CBOR binary json codec 
org.redisson.codec.MsgPackJacksonCodec MsgPack binary json codec 
org.redisson.codec.KryoCodec Kryo binary codec 
org.redisson.codec.SerializationCodec JDK Serialization codec 
org.redisson.codec.FstCodec FST up to 10 times faster and 100% JDK Serialization compatible codec 
org.redisson.codec.LZ4Codec LZ4 compression codec 
org.redisson.codec.SnappyCodec Snappy compression codec 
org.redisson.client.codec.StringCodec String codec 
org.redisson.client.codec.LongCodec Long codec
## 分布式对象
### 分布式Object
```
RBucket<AnyObject> bucket = redisson.getBucket("anyObject");
bucket.set(new AnyObject(1));
AnyObject obj = bucket.get();
bucket.trySet(new AnyObject(3));
bucket.compareAndSet(new AnyObject(4), new AnyObject(5));
bucket.getAndSet(new AnyObject(6));
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
### 分布式BitSet
```
RBitSet set = redisson.getBitSet("simpleBitset");
set.set(0, true);
set.set(1812, false);
set.clear(0);
set.addAsync("e");
set.xor("anotherBitset");
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
### 分布式Lock
```
Redisson redisson = Redisson.create();
RLock lock = redisson.getLock("anyLock");
// Most familiar locking method
lock.lock();
// Lock time-to-live support
// releases lock automatically after 10 seconds
// if unlock method not invoked
lock.lock(10, TimeUnit.SECONDS);
// Wait for 100 seconds and automatically unlock it after 10 seconds
boolean res = lock.tryLock(100, 10, TimeUnit.SECONDS);
...
lock.unlock();
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
### 分布式MultiLock
```
RLock lock1 = redissonInstance1.getLock("lock1");
RLock lock2 = redissonInstance2.getLock("lock2");
RLock lock3 = redissonInstance3.getLock("lock3");
RedissonMultiLock lock = new RedissonMultiLock(lock1, lock2, lock3);
lock.lock();
```
- 1
- 2
- 3
- 4
- 5
- 6
### 分布式ReadWriteLock
```
RReadWriteLock rwlock = redisson.getLock("anyRWLock");
// Most familiar locking method
rwlock.readLock().lock();
// or
rwlock.writeLock().lock();
// Lock time-to-live support
// releases lock automatically after 10 seconds
// if unlock method not invoked
rwlock.readLock().lock(10, TimeUnit.SECONDS);
// or
rwlock.writeLock().lock(10, TimeUnit.SECONDS);
// Wait for 100 seconds and automatically unlock it after 10 seconds
boolean res = rwlock.readLock().tryLock(100, 10, TimeUnit.SECONDS);
// or
boolean res = rwlock.writeLock().tryLock(100, 10, TimeUnit.SECONDS);
...
lock.unlock();
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
### 分布式Semaphore
```
RSemaphore semaphore = redisson.getSemaphore("semaphore");
semaphore.acquire();
semaphore.acquire(23);
semaphore.tryAcquire();
semaphore.tryAcquire(23, TimeUnit.SECONDS);
semaphore.release(10);
semaphore.release();
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
### 分布式AtomicLong
```
RAtomicLong atomicLong = redisson.getAtomicLong("myAtomicLong");
atomicLong.set(3);
atomicLong.incrementAndGet();
atomicLong.get();
```
- 1
- 2
- 3
- 4
- 5
### 分布式AtomicDouble
```
RAtomicDouble atomicDouble = redisson.getAtomicDouble("myAtomicDouble");
atomicDouble.set(2.81);
atomicDouble.addAndGet(4.11);
atomicDouble.get();
```
- 1
- 2
- 3
- 4
- 5
### 分布式CountDownLatch
```
RCountDownLatch latch = redisson.getCountDownLatch("anyCountDownLatch");
latch.trySetCount(1);
latch.await();
// in other thread or other JVM
RCountDownLatch latch = redisson.getCountDownLatch("anyCountDownLatch");
latch.countDown();
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
### Topic
```
RTopic<SomeObject> topic = redisson.getTopic("anyTopic");
topic.addListener(new MessageListener<SomeObject>() {
    @Override
    public void onMessage(String channel, SomeObject message) {
        //...
    }
});
// in other thread or JVM
RTopic<SomeObject> topic = redisson.getTopic("anyTopic");
long clientsReceivedMessage = topic.publish(new SomeObject());
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
### Topic patttern
```java
// subscribe to all topics by `topic1.*` pattern
RPatternTopic<Message> topic1 = redisson.getPatternTopic("topic1.*");
int listenerId = topic1.addListener(new PatternMessageListener<Message>() {
    @Override
    public void onMessage(String pattern, String channel, Message msg) {
         Assert.fail();
    }
});
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
## 分布式集合
### 分布式Map
除此之外，还支持Multimap，这里不列出
```
RMap<String, SomeObject> map = redisson.getMap("anyMap");
SomeObject prevObject = map.put("123", new SomeObject());
SomeObject currentObject = map.putIfAbsent("323", new SomeObject());
SomeObject obj = map.remove("123");
map.fastPut("321", new SomeObject());
map.fastRemove("321");
Future<SomeObject> putAsyncFuture = map.putAsync("321");
Future<Void> fastPutAsyncFuture = map.fastPutAsync("321");
map.fastPutAsync("321", new SomeObject());
map.fastRemoveAsync("321");
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
### Map eviction
现在Redis没有过期清空Map中的某个entry的功能，只能是清空Map所有的entry。Redission提供了这种功能。
```
RMapCache<String, SomeObject> map = redisson.getMapCache("anyMap");
// ttl = 10 minutes, 
map.put("key1", new SomeObject(), 10, TimeUnit.MINUTES);
// ttl = 10 minutes, maxIdleTime = 10 seconds
map.put("key1", new SomeObject(), 10, TimeUnit.MINUTES, 10, TimeUnit.SECONDS);
// ttl = 3 seconds
map.putIfAbsent("key2", new SomeObject(), 3, TimeUnit.SECONDS);
// ttl = 40 seconds, maxIdleTime = 10 seconds
map.putIfAbsent("key2", new SomeObject(), 40, TimeUnit.SECONDS, 10, TimeUnit.SECONDS);
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
### 分布式Set
```
RSet<SomeObject> set = redisson.getSet("anySet");
set.add(new SomeObject());
set.remove(new SomeObject());
```
- 1
- 2
- 3
- 4
除此之外还有，还支持Set eviction， SortedSet, ScoredSortedSet, LexSortedSet
### 分布式List
```php
RList<SomeObject> list = redisson.getList("anyList");
list.add(new SomeObject());
list.get(0);
list.remove(new SomeObject());
```
- 1
- 2
- 3
- 4
- 5
### 分布式Blocking Queue
```
RBlockingQueue<SomeObject> queue = redisson.getBlockingQueue("anyQueue");
queue.offer(new SomeObject());
SomeObject obj = queue.peek();
SomeObject someObj = queue.poll();
SomeObject ob = queue.poll(10, TimeUnit.MINUTES);
```
- 1
- 2
- 3
- 4
- 5
- 6
除此之外，还支持Queue, Deque, Blocking Deque
## 其他功能
### 执行批量命令
```
RBatch batch = redisson.createBatch();
batch.getMap("test").fastPutAsync("1", "2");
batch.getMap("test").fastPutAsync("2", "3");
batch.getMap("test").putAsync("2", "5");
batch.getAtomicLongAsync("counter").incrementAndGetAsync();
batch.getAtomicLongAsync("counter").incrementAndGetAsync();
List<?> res = batch.execute();
```
