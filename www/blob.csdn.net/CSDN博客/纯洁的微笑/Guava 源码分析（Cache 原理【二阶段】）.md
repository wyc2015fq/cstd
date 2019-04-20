# Guava 源码分析（Cache 原理【二阶段】） - 纯洁的微笑 - CSDN博客
2018年07月17日 07:36:00[微笑很纯洁](https://me.csdn.net/ityouknow)阅读数：985
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/PgqYrEEtEnqtxFKbO8ZDxgwt8nZX73aEf9AP8fRfbvedQxIjiccHicQlpNwOo6ibgCk4ZwbceX3MEYHxCcbYJpUZQ/640?wx_fmt=jpeg)
## 前言
在上文「[Guava 源码分析（Cache 原理）](http://mp.weixin.qq.com/s?__biz=MzIyMzgyODkxMQ==&mid=2247483791&idx=1&sn=de9c44bdef7ad98573a70f54112b305a&chksm=e8190f4fdf6e865977ee55277de32677a162af9e48470a50773eab38c69f0764ad77f1629e9e&scene=21#wechat_redirect)」中分析了 `Guava Cache` 的相关原理。
文末提到了**回收机制、移除时间通知**等内容，许多朋友也挺感兴趣，这次就这两个内容再来分析分析。
> 
在开始之前先补习下 Java 自带的两个特性，Guava 中都有具体的应用。
## Java 中的引用
首先是 Java 中的**引用**。
在之前分享过 JVM 是根据可达性分析算法找出需要回收的对象，判断对象的存活状态都和`引用`有关。
在 JDK1.2 之前这点设计的非常简单：一个对象的状态只有**引用**和**没被引用**两种区别。
这样的划分对垃圾回收不是很友好，因为总有一些对象的状态处于这两之间。
因此 1.2 之后新增了四种状态用于更细粒度的划分引用关系：
- 
强引用（Strong Reference）:这种对象最为常见，比如 **`A a = new A();`**这就是典型的强引用；这样的强引用关系是不能被垃圾回收的。
- 
软引用（Soft Reference）:这样的引用表明一些有用但不是必要的对象，在将发生垃圾回收之前是需要将这样的对象再次回收。
- 
弱引用（Weak Reference）:这是一种比软引用还弱的引用关系，也是存放非必须的对象。当垃圾回收时，无论当前内存是否足够，这样的对象都会被回收。
- 
虚引用（Phantom Reference）:这是一种最弱的引用关系，甚至没法通过引用来获取对象，它唯一的作用就是在被回收时可以获得通知。
## 事件回调
事件回调其实是一种常见的设计模式，比如之前讲过的 Netty 就使用了这样的设计。
这里采用一个 demo，试下如下功能：
- 
Caller 向 Notifier 提问。
- 
提问方式是异步，接着做其他事情。
- 
Notifier 收到问题执行计算然后回调 Caller 告知结果。
在 Java 中利用接口来实现回调，所以需要定义一个接口：
```java
1public interface CallBackListener {
2
3    /**
4     * 回调通知函数
5     * @param msg
6     */
7    void callBackNotify(String msg) ;
8}
```
Caller 中调用 Notifier 执行提问，调用时将接口传递过去：
```java
1public class Caller {
 2
 3    private final static Logger LOGGER = LoggerFactory.getLogger(Caller.class);
 4
 5    private CallBackListener callBackListener ;
 6
 7    private Notifier notifier ;
 8
 9    private String question ;
10
11    /**
12     * 使用
13     */
14    public void call(){
15
16        LOGGER.info("开始提问");
17
18        //新建线程，达到异步效果 
19        new Thread(new Runnable() {
20            @Override
21            public void run() {
22                try {
23                    notifier.execute(Caller.this,question);
24                } catch (InterruptedException e) {
25                    e.printStackTrace();
26                }
27            }
28        }).start();
29
30        LOGGER.info("提问完毕，我去干其他事了");
31    }
32
33    //隐藏 getter/setter
34
35}
```
Notifier 收到提问，执行计算（耗时操作），最后做出响应（回调接口，告诉 Caller 结果）。
```java
1public class Notifier {
 2
 3    private final static Logger LOGGER = LoggerFactory.getLogger(Notifier.class);
 4
 5    public void execute(Caller caller, String msg) throws InterruptedException {
 6        LOGGER.info("收到消息=【{}】", msg);
 7
 8        LOGGER.info("等待响应中。。。。。");
 9        TimeUnit.SECONDS.sleep(2);
10
11
12        caller.getCallBackListener().callBackNotify("我在北京！");
13
14    }
15
16}
```
模拟执行：
```java
1    public static void main(String[] args) {
 2        Notifier notifier = new Notifier() ;
 3
 4        Caller caller = new Caller() ;
 5        caller.setNotifier(notifier) ;
 6        caller.setQuestion("你在哪儿！");
 7        caller.setCallBackListener(new CallBackListener() {
 8            @Override
 9            public void callBackNotify(String msg) {
10                LOGGER.info("回复=【{}】" ,msg);
11            }
12        });
13
14        caller.call();
15    }
```
最后执行结果：
```
12018-07-15 19:52:11.105 [main] INFO  c.crossoverjie.guava.callback.Caller - 开始提问
22018-07-15 19:52:11.118 [main] INFO  c.crossoverjie.guava.callback.Caller - 提问完毕，我去干其他事了
32018-07-15 19:52:11.117 [Thread-0] INFO  c.c.guava.callback.Notifier - 收到消息=【你在哪儿！】
42018-07-15 19:52:11.121 [Thread-0] INFO  c.c.guava.callback.Notifier - 等待响应中。。。。。
52018-07-15 19:52:13.124 [Thread-0] INFO  com.crossoverjie.guava.callback.Main - 回复=【我在北京！】
```
这样一个模拟的异步事件回调就完成了。
## Guava 的用法
Guava 就是利用了上文的两个特性来实现了**引用回收**及**移除通知**。
### 引用
可以在初始化缓存时利用：
- 
CacheBuilder.weakKeys()
- 
CacheBuilder.weakValues()
- 
CacheBuilder.softValues()
来自定义键和值的引用关系。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/csD7FygBVl2VHia7yFl3aNOZmtRCZ9LwPat3ymSHh7IkIT26UbTGBvA2kgIPLP2qrsP7U1I6icytHAiad9Fyxpteg/640?wx_fmt=jpeg)
在上文的分析中可以看出 Cache 中的 `ReferenceEntry` 是类似于 HashMap 的 Entry 存放数据的。
来看看 ReferenceEntry 的定义：
```java
1  interface ReferenceEntry<K, V> {
 2    /**
 3     * Returns the value reference from this entry.
 4     */
 5    ValueReference<K, V> getValueReference();
 6
 7    /**
 8     * Sets the value reference for this entry.
 9     */
10    void setValueReference(ValueReference<K, V> valueReference);
11
12    /**
13     * Returns the next entry in the chain.
14     */
15    @Nullable
16    ReferenceEntry<K, V> getNext();
17
18    /**
19     * Returns the entry's hash.
20     */
21    int getHash();
22
23    /**
24     * Returns the key for this entry.
25     */
26    @Nullable
27    K getKey();
28
29    /*
30     * Used by entries that use access order. Access entries are maintained in a doubly-linked list.
31     * New entries are added at the tail of the list at write time; stale entries are expired from
32     * the head of the list.
33     */
34
35    /**
36     * Returns the time that this entry was last accessed, in ns.
37     */
38    long getAccessTime();
39
40    /**
41     * Sets the entry access time in ns.
42     */
43    void setAccessTime(long time);
44}
```
包含了很多常用的操作，如值引用、键引用、访问时间等。
根据 `ValueReference<K, V> getValueReference();` 的实现：
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/csD7FygBVl2VHia7yFl3aNOZmtRCZ9LwPB3ibTybsOTShIzUicvyibia5Q5yVzrhRXVCs9JhKEs7XGfLUYrDdyEZG2A/640?wx_fmt=jpeg)
具有强引用和弱引用的不同实现。
key 也是相同的道理：
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/csD7FygBVl2VHia7yFl3aNOZmtRCZ9LwP6qUfUGrNIndHBy00Pz9iaMibWJbNf7yabeWkwDykr4H1DOmjPYI1caqg/640?wx_fmt=jpeg)
当使用这样的构造方式时，弱引用的 key 和 value 都会被垃圾回收。
当然我们也可以显式的回收：
```cpp
1  /**
 2   * Discards any cached value for key {@code key}.
 3   * 单个回收
 4   */
 5  void invalidate(Object key);
 6
 7  /**
 8   * Discards any cached values for keys {@code keys}.
 9   *
10   * @since 11.0
11   */
12  void invalidateAll(Iterable<?> keys);
13
14  /**
15   * Discards all entries in the cache.
16   */
17  void invalidateAll();
```
### 回调
改造了之前的例子：
```java
1loadingCache = CacheBuilder.newBuilder()
 2        .expireAfterWrite(2, TimeUnit.SECONDS)
 3        .removalListener(new RemovalListener<Object, Object>() {
 4            @Override
 5            public void onRemoval(RemovalNotification<Object, Object> notification) {
 6                LOGGER.info("删除原因={}，删除 key={},删除 value={}",notification.getCause(),notification.getKey(),notification.getValue());
 7            }
 8        })
 9        .build(new CacheLoader<Integer, AtomicLong>() {
10            @Override
11            public AtomicLong load(Integer key) throws Exception {
12                return new AtomicLong(0);
13            }
14        });
```
执行结果：
```
12018-07-15 20:41:07.433 [main] INFO  c.crossoverjie.guava.CacheLoaderTest - 当前缓存值=0,缓存大小=1
22018-07-15 20:41:07.442 [main] INFO  c.crossoverjie.guava.CacheLoaderTest - 缓存的所有内容={1000=0}
32018-07-15 20:41:07.443 [main] INFO  c.crossoverjie.guava.CacheLoaderTest - job running times=10
42018-07-15 20:41:10.461 [main] INFO  c.crossoverjie.guava.CacheLoaderTest - 删除原因=EXPIRED，删除 key=1000,删除 value=1
52018-07-15 20:41:10.462 [main] INFO  c.crossoverjie.guava.CacheLoaderTest - 当前缓存值=0,缓存大小=1
62018-07-15 20:41:10.462 [main] INFO  c.crossoverjie.guava.CacheLoaderTest - 缓存的所有内容={1000=0}
```
可以看出当缓存被删除的时候会回调我们自定义的函数，并告知删除原因。
那么 Guava 是如何实现的呢？
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/csD7FygBVl2VHia7yFl3aNOZmtRCZ9LwPS2dm4l5dZ20NqYcTTNMZhw0kM4390kVe2EkA1pboPUxicP77ukV5JQw/640?wx_fmt=jpeg)
根据 LocalCache 中的 `getLiveValue()` 中判断缓存过期时，跟着这里的调用关系就会一直跟到：
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/csD7FygBVl2VHia7yFl3aNOZmtRCZ9LwP7h5Ptt4OX5fqBHfHoXMjDd75Cd5djNaHaLpZqe2LCjP3CnicbNicVGDA/640?wx_fmt=jpeg)
`removeValueFromChain()` 中的：
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/csD7FygBVl2VHia7yFl3aNOZmtRCZ9LwPZuRibeoH55wd469iaKfyKWQDYMQ4aSMgsv71IUxdlvGxAhqiaLvhYnhIA/640?wx_fmt=jpeg)
`enqueueNotification()` 方法会将回收的缓存（包含了 key，value）以及回收原因包装成之前定义的事件接口加入到一个**本地队列**中。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/csD7FygBVl2VHia7yFl3aNOZmtRCZ9LwPcS3nO0xibwIXFDKFlo2CTNEoTjweU9yd6uYefMgDSPe40icvM8Kd4Kng/640?wx_fmt=jpeg)
这样一看也没有回调我们初始化时候的事件啊。
不过用过队列的同学应该能猜出，既然这里写入队列，那就肯定就有消费。
我们回到获取缓存的地方：
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/csD7FygBVl2VHia7yFl3aNOZmtRCZ9LwPDwica0vpcKicWOwLtxC6SCd8ichYHelMXhtGo3dFTcib50pNeCaHpXwFqw/640?wx_fmt=jpeg)
在 finally 中执行了 `postReadCleanup()` 方法；其实在这里面就是对刚才的队列进行了消费：
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/csD7FygBVl2VHia7yFl3aNOZmtRCZ9LwPdBhP0rSMcFUVawIHxCNafrSupfJ4PXAoc9ccUebhv6YHjozuYIf2xQ/640?wx_fmt=jpeg)
一直跟进来就会发现这里消费了队列，将之前包装好的移除消息调用了我们自定义的事件，这样就完成了一次事件回调。
## 总结
以上所有源码：
https://github.com/crossoverJie/Java-Interview/blob/master/src/main/java/com/crossoverjie/guava/callback/Main.java
通过分析 Guava 的源码可以让我们学习到顶级的设计及实现方式，甚至自己也能尝试编写。
Guava 里还有很多强大的增强实现，值得我们再好好研究。
END
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/PgqYrEEtEnpcR0YpEk9cSITT9srthjLfnyo5a7BroaQnXA0uMk8tvhZtMVLkQDYSsXP1ibtlmYibkvBLRRe6ibD7g/640?wx_fmt=png)
