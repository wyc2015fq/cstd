# 通过Hash用锁控制并发 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年05月11日 14:41:46[boonya](https://me.csdn.net/boonya)阅读数：1897








From：[http://m.oschina.net/blog/673008](http://m.oschina.net/blog/673008)

**1. 分段锁**

        借鉴concurrentHashMap的分段思想，先生成一定数量的锁，具体使用的时候再根据key来返回对应的lock。这是几个实现里最简单，性能最高，也是最终被采用的锁策略，代码如下：



```java
/**
 * 分段锁，系统提供一定数量的原始锁，根据传入对象的哈希值获取对应的锁并加锁
 * 注意：要锁的对象的哈希值如果发生改变，有可能导致锁无法成功释放!!!
 */
public class SegmentLock<T> {
    private Integer segments = 16;//默认分段数量
    private final HashMap<Integer, ReentrantLock> lockMap = new HashMap<>();

    public SegmentLock() {
        init(null, false);
    }

    public SegmentLock(Integer counts, boolean fair) {
        init(counts, fair);
    }

    private void init(Integer counts, boolean fair) {
        if (counts != null) {
            segments = counts;
        }
        for (int i = 0; i < segments; i++) {
            lockMap.put(i, new ReentrantLock(fair));
        }
    }

    public void lock(T key) {
        ReentrantLock lock = lockMap.get((key.hashCode()>>>1) % segments);
        lock.lock();
    }

    public void unlock(T key) {
        ReentrantLock lock = lockMap.get((key.hashCode()>>>1) % segments);
        lock.unlock();
    }
}
```



**    2. 哈希锁**

        上述分段锁的基础上发展起来的第二种锁策略，目的是实现真正意义上的细粒度锁。每个哈希值不同的对象都能获得自己独立的锁。在测试中，在被锁住的代码执行速度飞快的情况下，效率比分段锁慢 30% 左右。如果有长耗时操作，感觉表现应该会更好。代码如下：




```java
public class HashLock<T> {
    private boolean isFair = false;
    private final SegmentLock<T> segmentLock = new SegmentLock<>();//分段锁
    private final ConcurrentHashMap<T, LockInfo> lockMap = new ConcurrentHashMap<>();

    public HashLock() {
    }

    public HashLock(boolean fair) {
        isFair = fair;
    }

    public void lock(T key) {
        LockInfo lockInfo;
        segmentLock.lock(key);
        try {
            lockInfo = lockMap.get(key);
            if (lockInfo == null) {
                lockInfo = new LockInfo(isFair);
                lockMap.put(key, lockInfo);
            } else {
                lockInfo.count.incrementAndGet();
            }
        } finally {
            segmentLock.unlock(key);
        }
        lockInfo.lock.lock();
    }

    public void unlock(T key) {
        LockInfo lockInfo = lockMap.get(key);
        if (lockInfo.count.get() == 1) {
            segmentLock.lock(key);
            try {
                if (lockInfo.count.get() == 1) {
                    lockMap.remove(key);
                }
            } finally {
                segmentLock.unlock(key);
            }
        }
        lockInfo.count.decrementAndGet();
        lockInfo.unlock();
    }

    private static class LockInfo {
        public ReentrantLock lock;
        public AtomicInteger count = new AtomicInteger(1);

        private LockInfo(boolean fair) {
            this.lock = new ReentrantLock(fair);
        }

        public void lock() {
            this.lock.lock();
        }

        public void unlock() {
            this.lock.unlock();
        }
    }
}
```


**    3. 弱引用锁**

        哈希锁因为引入的分段锁来保证锁创建和销毁的同步，总感觉有点瑕疵，所以写了第三个锁来寻求更好的性能和更细粒度的锁。这个锁的思想是借助java的弱引用来创建锁，把锁的销毁交给jvm的垃圾回收，来避免额外的消耗。

        有点遗憾的是因为使用了ConcurrentHashMap作为锁的容器，所以没能真正意义上的摆脱分段锁。这个锁的性能比 HashLock 快10% 左右。锁代码：



```java
/**
 * 弱引用锁，为每个独立的哈希值提供独立的锁功能
 */
public class WeakHashLock<T> {
    private ConcurrentHashMap<T, WeakLockRef<T, ReentrantLock>> lockMap = new ConcurrentHashMap<>();
    private ReferenceQueue<ReentrantLock> queue = new ReferenceQueue<>();

    public ReentrantLock get(T key) {
        if (lockMap.size() > 1000) {
            clearEmptyRef();
        }
        WeakReference<ReentrantLock> lockRef = lockMap.get(key);
        ReentrantLock lock = (lockRef == null ? null : lockRef.get());
        while (lock == null) {
            lockMap.putIfAbsent(key, new WeakLockRef<>(new ReentrantLock(), queue, key));
            lockRef = lockMap.get(key);
            lock = (lockRef == null ? null : lockRef.get());
            if (lock != null) {
                return lock;
            }
            clearEmptyRef();
        }
        return lock;
    }

    @SuppressWarnings("unchecked")
    private void clearEmptyRef() {
        Reference<? extends ReentrantLock> ref;
        while ((ref = queue.poll()) != null) {
            WeakLockRef<T, ? extends ReentrantLock> weakLockRef = (WeakLockRef<T, ? extends ReentrantLock>) ref;
            lockMap.remove(weakLockRef.key);
        }
    }

    private static final class WeakLockRef<T, K> extends WeakReference<K> {
        final T key;

        private WeakLockRef(K referent, ReferenceQueue<? super K> q, T key) {
            super(referent, q);
            this.key = key;
        }
    }
}
```

**后记**

    最开始想借助 locksupport 和 AQS 来实现细粒度锁，写着写着发现正在实现的东西和java 原生的锁区别不大，于是放弃改为对java自带锁的封装，浪费了不少时间。

    实际上在实现了这些细粒度锁之后，又有了新的想法，比如可以通过分段思想将数据提交给专门的线程来处理，可以减少大量线程的阻塞时间，留待日后探索...



