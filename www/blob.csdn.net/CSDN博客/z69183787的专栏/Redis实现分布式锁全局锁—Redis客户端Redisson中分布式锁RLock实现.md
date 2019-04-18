# Redis实现分布式锁全局锁—Redis客户端Redisson中分布式锁RLock实现 - z69183787的专栏 - CSDN博客
2018年02月22日 16:13:16[OkidoGreen](https://me.csdn.net/z69183787)阅读数：136
[https://my.oschina.net/haogrgr/blog/469439](https://my.oschina.net/haogrgr/blog/469439)
摘要: Redis客户端Redisson中分布式锁RLock实现分析
1. 前因
以前实现过一个Redis实现的全局锁, 虽然能用, 但是感觉很不完善, 不可重入, 参数太多等等.
最近看到了一个新的Redis客户端Redisson, 看了下源码, 发现了一个比较好的锁实现RLock, 于是记录下.
2. Maven依赖
```
<dependency>
    <groupId>org.redisson</groupId>
    <artifactId>redisson</artifactId>
    <version>1.2.1</version>
</dependency>
```
3. 初试
Redisson中RLock的使用很简单, 来看看一个最简单的例子.
```java
import org.redisson.Redisson;
import org.redisson.core.RLock;
public class Temp {
    public static void main(String[] args) throws Exception {
        Redisson redisson = Redisson.create();
        RLock lock = redisson.getLock("haogrgr");
        lock.lock();
        try {
            System.out.println("hagogrgr");
        }
        finally {
            lock.unlock();
        }
        redisson.shutdown();
    }
}
```
4. RLock接口
    通过上面的例子可以看出, 使用起来和juc里面的Lock接口使用很类似, 那么来看看RLock这个接口.
```java
Rlock
|
----------Lock
          |
          ----------void lock()
          |
          ----------void lockInterruptibly()
          |
          ----------boolean tryLock()
          |
          ----------boolean tryLock(long time, TimeUnit unit)
          |
          ----------void unlock()
          |
          ----------Condition newCondition()
|
----------RObject
          |
          ----------String getName()
          |
          ----------void delete()
|
----------void lockInterruptibly(long leaseTime, TimeUnit unit)
|
----------boolean tryLock(long waitTime, long leaseTime, TimeUnit unit)
|
----------void lock(long leaseTime, TimeUnit unit)
|
----------void forceUnlock()
|
----------boolean isLocked();
|
----------boolean isHeldByCurrentThread()
|
----------int getHoldCount()
```
可以看到, 该接口主要继承了Lock接口, 然后扩展了部分方法, 比如: 
    boolean tryLock(long waitTime, long leaseTime, TimeUnit unit)
    新加入的leaseTime主要是用来设置锁的过期时间, 形象的解释就是, 如果超过leaseTime还没有解锁的话, 我就强制解锁. 
5. RLock接口的实现
    具体的实现类是RedissonLock, 下面来大概看看实现原理. 先看看 (3) 中例子执行时, 所运行的命令(通过monitor命令):
```java
127.0.0.1:6379> monitor
OK
1434959509.494805 [0 127.0.0.1:57911] "SETNX" "haogrgr" "{\"@class\":\"org.redisson.RedissonLock$LockValue\",\"counter\":1,\"id\":\"c374addc-523f-4943-b6e0-c26f7ab061e3\",\"threadId\":1}"
1434959509.494805 [0 127.0.0.1:57911] "GET" "haogrgr"
1434959509.524805 [0 127.0.0.1:57911] "MULTI"
1434959509.529805 [0 127.0.0.1:57911] "DEL" "haogrgr"
1434959509.529805 [0 127.0.0.1:57911] "PUBLISH" "redisson__lock__channel__{haogrgr}" "0"
1434959509.529805 [0 127.0.0.1:57911] "EXEC"
```
   可以看到, 大概原理是, 通过判断Redis中是否有某一key, 来判断是加锁还是等待, 最后的publish是一个解锁后, 通知阻塞在lock的线程.
    分布式锁的实现依赖的单点, 这里Redis就是单点, 通过在Redis中维护状态信息来实现全局的锁. 那么来看看RedissonLock如何
    实现可重入, 保证原子性等等细节.
6. 加锁源码分析
    从最简单的无参数的lock参数来看源码.
```java
public void lock() {
    try {
        lockInterruptibly();
    } catch (InterruptedException e) {
        Thread.currentThread().interrupt();
        return;
    }
}
public void lockInterruptibly() throws InterruptedException {
    lockInterruptibly(-1, null);    //leaseTime : -1 表示key不设置过期时间
}
public void lockInterruptibly(long leaseTime, TimeUnit unit) throws InterruptedException {
    Long ttl;
    if (leaseTime != -1) {
        ttl = tryLockInner(leaseTime, unit);
    } else {
        ttl = tryLockInner();
    }
    // lock acquired
    if (ttl == null) {
        return;
    }
    subscribe().awaitUninterruptibly();
    try {
        while (true) {
            if (leaseTime != -1) {
                ttl = tryLockInner(leaseTime, unit);
            } else {
                ttl = tryLockInner();
            }
            // lock acquired
            if (ttl == null) {
                break;
            }
            // waiting for message
            RedissonLockEntry entry = ENTRIES.get(getEntryName());
            if (ttl >= 0) {
                entry.getLatch().tryAcquire(ttl, TimeUnit.MILLISECONDS);
            } else {
                entry.getLatch().acquire();
            }
        }
    } finally {
        unsubscribe();
    }
}
```
    代码有点多, 但是没关系, 慢慢分解, 由于这里我们是调用无参数的lock方法, 所以最后执行到的方法是:
```java
private Long tryLockInner() {
    final LockValue currentLock = new LockValue(id, Thread.currentThread().getId());    //保存锁的状态: 客户端UUID+线程ID来唯一标识某一JVM实例的某一线程
    currentLock.incCounter();    //用来保存重入次数, 实现可重入功能, 初始情况是1
    //Redisson封装了交互的细节, 具体的逻辑为execute方法逻辑.
    return connectionManager.write(getName(), new SyncOperation<LockValue, Long>() {
        @Override
        public Long execute(RedisConnection<Object, LockValue> connection) {
            Boolean res = connection.setnx(getName(), currentLock);    //如果key:haogrgr不存在, 就set并返回true, 否则返回false
            if (!res) {    //如果设置失败, 那么表示有锁竞争了, 于是获取当前锁的状态, 如果拥有者是当前线程, 就累加重入次数并set新值
                connection.watch(getName());    //通过watch命令配合multi来实现简单的事务功能
                LockValue lock = (LockValue) connection.get(getName());
                if (lock != null && lock.equals(currentLock)) {    //LockValue的equals实现为比较客户id和threadid是否一样
                    lock.incCounter();    //如果当前线程已经获取过锁, 则累加加锁次数, 并set更新
                    connection.multi();
                    connection.set(getName(), lock);
                    if (connection.exec().size() == 1) {
                        return null;    //set成功, 
                    }
                }
                connection.unwatch();
                //走到这里, 说明上面set的时候, 其他客户端在  watch之后->set之前 有其他客户端修改了key值
                //则获取key的过期时间, 如果是永不过期, 则返回-1, 具体处理后面说明
                Long ttl = connection.pttl(getName());
                return ttl;
            }
            return null;
        }
    });
}
```
    tryLockInner的逻辑已经看完了,  可以知道, 有三种情况:
    (1) key不存在, 加锁: 
         当key不存在时, 设置锁的初始状态并set, 具体来看就是 setnx   haogrgr   LockValue{ id: Redisson对象的id,  threadId: 当前线程id,  counter: 当前重入次数,这里为第一次获取,所以为1}  
         通过上面的操作. 达到获取锁的目的, 通过setnx来达到实现类似于  if(map.get(key) == null) { map.put(key) } 的功能, 防止多个客户端同时set时, 新值覆盖老值.
    (2)key存在, 且获取锁的当前线程, 重入:
         这里就是锁重入的情况, 也就是锁的拥有者第二次调用lock方法, 这时, 通过先get, 然后比较客户端ID和当前线程ID来判断拥有锁的线程是不是当前线程.(客户端ID+线程ID才能唯一定位锁拥有者线程)
         判断发现当前是重入情况, 则累加LockValue的counter, 然后重新set回去, 这里使用到了watch和multi命令, 防止   get -> set   期间其他客户端修改了key的值.
    (3)key存在, 且是其他线程获取的锁, 等待:
         首先尝试获取锁(setnx), 失败后发现锁拥有者不是当前线程, 则获取key的过期时间, 返回过期时间
    那么接下来看看tryLockInner调用完成后的处理代码.
```java
public void lockInterruptibly(long leaseTime, TimeUnit unit) throws InterruptedException {
    Long ttl;
    if (leaseTime != -1) {
        ttl = tryLockInner(leaseTime, unit);
    } else {
        ttl = tryLockInner();   //lock()方法调用会走的逻辑
    }
    // lock acquired
    if (ttl == null) {   //加锁成功(新获取锁, 重入情况) tryLockInner会返回null, 失败会返回key超时时间, 或者-1(key未设置超时时间)
        return;   //加锁成功, 返回
    }
    //subscribe这个方法代码有点多, Redisson通过netty来和redis通讯, 然后subscribe返回的是一个Future类型,
    //Future的awaitUninterruptibly()调用会阻塞, 然后Redisson通过Redis的pubsub来监听unlock的topic(getChannelName())
    //例如, 5中所看到的命令 "PUBLISH" "redisson__lock__channel__{haogrgr}" "0"
    //当解锁时, 会向名为 getChannelName() 的topic来发送解锁消息("0")
    //而这里 subscribe() 中监听这个topic, 在订阅成功时就会唤醒阻塞在awaitUninterruptibly()的方法. 
    //所以线程在这里只会阻塞很短的时间(订阅成功即唤醒, 并不代表已经解锁)
    subscribe().awaitUninterruptibly();
    try {
        while (true) {    //循环, 不断重试lock
            if (leaseTime != -1) {
                ttl = tryLockInner(leaseTime, unit);
            } else {
                ttl = tryLockInner();   //不多说了
            }
            // lock acquired
            if (ttl == null) {
                break;
            }
            
            // 这里才是真正的等待解锁消息, 收到解锁消息, 就唤醒, 然后尝试获取锁, 成功返回, 失败则阻塞在acquire().
            // 收到订阅成功消息, 则唤醒阻塞上面的subscribe().awaitUninterruptibly();
            // 收到解锁消息, 则唤醒阻塞在下面的entry.getLatch().acquire();
            RedissonLockEntry entry = ENTRIES.get(getEntryName());
            if (ttl >= 0) {
                entry.getLatch().tryAcquire(ttl, TimeUnit.MILLISECONDS);
            } else {
                entry.getLatch().acquire();
            }
        }
    } finally {
        unsubscribe();  //加锁成功或异常,解除订阅
    }
}
```
    主要的代码都加上了详细的注释, subscribe() 方法的代码复杂些, 但具体就是利用redis的pubsub提供一个通知机制来减少不断的重试.
    很多的Redis锁实现都是失败后sleep一定时间后重试, 在锁被占用时间较长时, 不断的重试是浪费, 而sleep也会导致不必要的时间浪费(在sleep期间可能已经解锁了), sleep时间太长, 时间浪费, 太短, 重试次数会增加~~~.
    到这里lock的逻辑已经看完了, 其他的比如tryLock方法逻辑和lock类似, 不过加了超时时间, 然后还有一种lock方法就是对key加上了过期时间.
7. 解锁源码
    unlock的逻辑相对简单.
```java
public void unlock() {
    connectionManager.write(getName(), new SyncOperation<Object, Void>() {
        @Override
        public Void execute(RedisConnection<Object, Object> connection) {
            LockValue lock = (LockValue) connection.get(getName());
            if (lock != null) {
                LockValue currentLock = new LockValue(id, Thread.currentThread().getId());
                if (lock.equals(currentLock)) {
                    if (lock.getCounter() > 1) {
                        lock.decCounter();
                        connection.set(getName(), lock);
                    } else {
                        unlock(connection);
                    }
                } else {
                    throw new IllegalMonitorStateException("Attempt to unlock lock, not locked by current id: "
                            + id + " thread-id: " + Thread.currentThread().getId());
                }
            } else {
                // could be deleted
            }
            return null;
        }
    });
}
private void unlock(RedisConnection<Object, Object> connection) {
    int counter = 0;
    while (counter < 5) {
        connection.multi();
        connection.del(getName());
        connection.publish(getChannelName(), unlockMessage);
        List<Object> res = connection.exec();
        if (res.size() == 2) {
            return;
        }
        counter++;
    }
    throw new IllegalStateException("Can't unlock lock after 5 attempts. Current id: "
            + id + " thread-id: " + Thread.currentThread().getId());
}
```
    具体的逻辑比较简单, 我就不注释了, 大概就是, 如果是多次重入的, 就以此递减然后 set, 如果是只lock一次的, 就删除, 然后publish一条解锁的message到getChannelName() tocpic.
    这里解锁会重试五次, 失败就抛异常.
8.总结
    逻辑并不复杂, 但是通过记录客户端ID和线程ID来唯一标识线程, 实现重入功能, 通过pub sub功能来减少空转.
    优点: 实现了Lock的大部分功能, 提供了特殊情况方法(如:强制解锁, 判断当前线程是否已经获取锁, 超时强制解锁等功能), 可重入, 减少重试.
    缺点: 使用依赖Redisson, 而Redisson依赖netty, 如果简单使用, 引入了较多的依赖, pub sub的实时性需要测试, 没有监控等功能, 查问题麻烦, 统计功能也没有(例如慢lock日志, 2333333).
    感觉靠谱, 可以用,233333333.
