# 分布式锁实现方案(REDIS,ZOOKEEPER,TAIR) - z69183787的专栏 - CSDN博客
2017年03月30日 13:25:12[OkidoGreen](https://me.csdn.net/z69183787)阅读数：7059
# Zookeeper
1、原生ZK方案
Zookeeper中有一种节点叫做顺序节点，假如我们在/lock/目录下创建节3个点，ZooKeeper集群会按照提起创建的顺序来创建节点，节点分别为/lock/0000000001、/lock/0000000002、/lock/0000000003。
ZooKeeper中还有一种名为临时节点的节点，临时节点由某个客户端创建，当客户端与ZooKeeper集群断开连接，则开节点自动被删除。
EPHEMERAL_SEQUENTIAL为临时顺序节点
实现分布式锁的基本逻辑：
- 客户端调用create()方法创建名为“*locknode*/guid-lock-”的节点，需要注意的是，这里节点的创建类型需要设置为EPHEMERAL_SEQUENTIAL。
- 客户端调用getChildren(“*locknode*”)方法来获取所有已经创建的子节点。
- 客户端获取到所有子节点path之后，如果发现自己在步骤1中创建的节点是所有节点中序号最小的，那么就认为这个客户端获得了锁。
- 如果创建的节点不是所有节点中需要最小的，那么则监视比自己创建节点的序列号小的最大的节点，进入等待。直到下次监视的子节点变更的时候，再进行子节点的获取，判断是否获取锁。
释放锁的过程相对比较简单，就是删除自己创建的那个子节点即可。
以下是流程图：
![](https://img-blog.csdn.net/20170330132315615?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
读写锁：读写锁的实现与互斥锁类似，不同的地方在于创建自节点时读锁和写锁要区分类型。例如读锁的前缀可以设置为read,写锁的前缀可以设置为write。创建读锁的时候，检查是否有编号小于自己的写锁存在，若存在则对编号刚好小于自己的写锁节点进行监听。创建写锁时，检查创建的节点编号是否为最小，如不是最小，则需要对编号刚好小于自己的节点进行监听（此时不区分读锁和写锁)
2、Curator方案
封装了zk的客户端，其分布式实现方式和上面的基本相同。同时还提供了不同的锁类型：
可重入锁：实现类为InterProcessMutex，将线程对象，节点，锁对象相关联。InterProcessMutex内部维护了一个使用线程为key，{thread,path}为值的map，所以对不同的线程和请求加锁的节点进行一一对应。提供方法acquire 和 release。
不可重入锁：实现类为InterProcessSemaphoreMutex，类似InterProcessMutex，只是没有维护线程的map。
可重入读写锁：类似JDK的`ReentrantReadWriteLock`.一个读写锁管理一对相关的锁。 主要由两个类实现：
- InterProcessReadWriteLock
- InterProcessLock
使用时首先创建一个`InterProcessReadWriteLock`实例，然后再根据你的需求得到读锁或者写锁， 读写锁的类型是`InterProcessLock`。
读写锁的实现与互斥锁类似，不同的地方在于创建自节点时读锁和写锁要区分类型。例如读锁的前缀可以设置为read,写锁的前缀可以设置为write。创建读锁的时候，检查是否有编号小于自己的写锁存在，若存在则对编号刚好小于自己的写锁节点进行监听。创建写锁时，检查创建的节点编号是否为最小，如不是最小，则需要对编号刚好小于自己的节点进行监听（此时不区分读锁和写锁)
还有信号量和多锁对象。
3、menagerie方案
menagerie基于Zookeeper实现了java.util.concurrent包的一个分布式版本。这个封装是更大粒度上对各种分布式一致性使用场景的抽象。其中最基础和常用的是一个分布式锁的实现：
org.menagerie.locks.ReentrantZkLock，通过ZooKeeper的全局有序的特性和EPHEMERAL_SEQUENTIAL类型znode的支持，实现了分布式锁。
# Redis
最常见互斥锁方案：
Redis的**SETNX**（即SET if Not eXists）和**GETSET**（先写新值，返回旧值，原子性操作，可以用于分辨是不是首次操作）可以用于分布式锁：
- C3发送SETNX lock.{orderid} 想要获得锁，由于C0还持有锁，所以Redis返回给C3一个0，
- C3发送GET lock.{orderid} 以检查锁是否超时了，如果没超时，则等待或重试。
- 反之，如果已超时，C3通过下面的操作来尝试获得锁：
**GETSET lock.{orderid} <current Unix time + lock timeout + 1>**
- 通过GETSET，C3拿到的时间戳如果仍然是超时的，那就说明，C3如愿以偿拿到锁了。
- 如果在C3之前，有个叫C4的客户端比C3快一步执行了上面的操作，那么C3拿到的时间戳是个未超时的值，这时，C3没有如期获得锁，需要再次等待或重试。留意一下，尽管C3没拿到锁，但它改写了C4设置的锁的超时值，不过这一点非常微小的误差带来的影响可以忽略不计。
jeffkit的伪码参考：
- 
# get lock
- 
lock = 0
- 
while lock != 1:
- 
    timestamp = current Unix time + lock timeout + 1
- 
    lock = SETNX lock.orderid timestamp
- 
if lock == 1or(now()>(GET lock.orderid)and now()>(GETSET lock.orderid timestamp)):
- 
break
- 
else:
- 
        sleep(10ms)
- 
- 
do_your_job()
- 
- 
# release lock
- 
if now()< GET lock.orderid:
- 
DEL lock.orderid
# Tair
设计思路和Medis类似，但实现略有不同。
美团维护的Tair中增加了expireLock和expireUnlock接口，通过锁状态和过期时间戳来共同判断锁是否存在：只有锁已经存在且没有过期的状态才判定为有锁状态。在有锁状态下，不能加锁，能通过大于过期时间的时间戳进行解锁；在无锁状态下，可以加锁，加锁成功会返回过期时间戳，用于解锁使用。重要的是，expireLock的原子性可以保证加锁和解锁时不会因为线程抢占引起错误。
不可重入锁：在加锁时调用expireLock，解锁时调用expireUnlock接口。传入的参数为过期时间或者过期时间戳。可以防止当线程拿到锁之后阻塞或者宕机，锁可以在过期之后释放出来。同时可以满足解锁动作安全，当自己的锁过期时不会误删别人的锁。
可重入锁：类似不可重入锁，维护类似zk的一个线程数和锁名的map。
可重入读写锁：
读线程：先用当前时间进行一次解锁expireUnlock，如果能解开则说明没有线程在写，可以进行读操作，同时incr，将计数器加1；完成读之后进行decr。
写线程：getCount读取计数器，如果为0，则说明没有线程在读，否则则需要等待；再expireLock，如果成功说明获取到了写锁，否则则说明已经有线程在写了；完成写之后进行解锁expireUnlock
缺陷：均有两步操作，但无法保证原子性。
![](https://img-blog.csdn.net/20170330132442281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
