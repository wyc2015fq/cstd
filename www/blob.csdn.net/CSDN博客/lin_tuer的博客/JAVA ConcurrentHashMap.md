
# JAVA ConcurrentHashMap - lin_tuer的博客 - CSDN博客


2018年03月24日 12:13:44[lin_tuer](https://me.csdn.net/lin_tuer)阅读数：79


隔了这么久 才把 concurrenthashmap 的文给补上，我表示深深的忏悔，不过，学习贵在坚持，不管有多少事情等待着我，我总要花时间去做我坚持要做的。
首先，先列举一下，关于 ConcurrentHashMap 必须知道的几个点：
1. 它 是线程安全的，通过采用 分段锁 来提高并发操作的效率，（效率比 HashTable高）。不支持 key, value 为 null。
2. 它 通过 二次 hash 来先定位到 segment, 再定位到某个 segment 中的 bucket 中
3. Segment 是一种可重入锁 ReentrantLock
4. 其 resize() 只对 segment 扩容，整个concurrenthashmap 的长度是不可变的。

这是 我是先看了博客才去看源码的，发现 这样操作的结果是，自己总结就有些无从下手，所以，以后还是希望自己尽量先看源码，不清楚的地方再参考别的博客。

一、结构图
来自 blog: https://blog.csdn.net/justloveyou_/article/details/72783008

![](https://img-blog.csdn.net/20180324111051601?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpbl90dWVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
二、HashMap 线程不安全的典型表现
HashMap是一个数组链表，当一个key/Value对被加入时，首先会通过Hash算法定位出这个键值对要被放入的桶，然后就把它插到相应桶中。如果这个桶中已经有元素了，那么发生了碰撞，这样会在这个桶中形成一个链表。一般来说，当有数据要插入HashMap时，都会检查容量有没有超过设定的thredhold，如果超过，需要增大HashMap的尺寸，但是这样一来，就需要对整个HashMap里的节点进行重哈希操作。
在 多线程 环境下，重哈希操作可能引起死循环。（过程有些多，就不在这里描述了）
三、HashEntry
HashEntry用来封装具体的键值对，是个典型的四元组。与HashMap中的Entry类似，HashEntry也包括同样的四个域，分别是key、hash、value和next。**在HashEntry类中，key，hash和next域都被声明为final的，value域被volatile所修饰，因此HashEntry对象几乎是不可变的，其插入操作只能在头部进行，而不可以从末尾或者中间进行插入。****由于value域被volatile修饰，所以其可以确保被读线程读到最新的值，这是ConcurrentHashmap读操作并不需要加锁的另一个重要原因。**实际上，ConcurrentHashMap完全允许多个读操作并发进行，读操作并不需要加锁。
源码，来自[点击打开链接](https://blog.csdn.net/justloveyou_/article/details/72783008)
```java
/**
     * ConcurrentHashMap 中的 HashEntry 类
     * 
     * ConcurrentHashMap list entry. Note that this is never exported
     * out as a user-visible Map.Entry.
     *
     * Because the value field is volatile, not final, it is legal wrt
     * the Java Memory Model for an unsynchronized reader to see null
     * instead of initial value when read via a data race.  Although a
     * reordering leading to this is not likely to ever actually
     * occur, the Segment.readValueUnderLock method is used as a
     * backup in case a null (pre-initialized) value is ever seen in
     * an unsynchronized access method.
     */
    static final class HashEntry<K,V> {
       final K key;                       // 声明 key 为 final 的
       final int hash;                   // 声明 hash 值为 final 的
       volatile V value;                // 声明 value 被volatile所修饰
       final HashEntry<K,V> next;      // 声明 next 为 final 的
        HashEntry(K key, int hash, HashEntry<K,V> next, V value) {
            this.key = key;
            this.hash = hash;
            this.next = next;
            this.value = value;
        }
        @SuppressWarnings("unchecked")
        static final <K,V> HashEntry<K,V>[] newArray(int i) {
        return new HashEntry[i];
        }
    }
```
四、并发 写 put, get, remove, clear 等操作
1. 通过 HashEntry 对象的不变性降低 读 操作的加锁需求
clear操作只是把ConcurrentHashMap中所有的桶置空，每个桶之前引用的链表依然存在，只是桶不再引用这些链表而已，而链表本身的结构并没有发生任何修改。因此，正在遍历某个链表的读线程依然可以正常执行对该链表的遍历。
关于put操作的细节我们在上文已经单独介绍过，我们知道put操作如果需要插入一个新节点到链表中时会在链表头部插入这个新节点，此时链表中的原有节点的链接并没有被修改。也就是说，插入新的健/值对到链表中的操作不会影响读线程正常遍历这个链表。
Segment的remove操作和前面提到的get操作类似，首先根据散列码找到具体的链表，然后遍历这个链表找到要删除的节点，最后把待删除节点之后的所有节点原样保留在新链表中，把待删除节点之前的每个节点克隆到新链表中。假设写线程执行remove操作，要删除链表的C节点，另一个读线程同时正在遍历这个链表![](https://img-blog.csdn.net/20180324113114232?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpbl90dWVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![](https://img-blog.csdn.net/20180324113059143?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpbl90dWVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
其前面的部分 被反转了。

2. volatile 变量保证数据的线程间可见性
假设线程M在写入了volatile变量count后，线程N读取了这个volatile变量，写线程M对链表做的结构性修改对读线程N是可见的。虽然线程N是在未加锁的情况下访问链表，但Java的内存模型可以保证：**只要之前对链表做结构性修改操作的写线程M在退出写方法前写volatile变量count，读线程N就能读取到这个volatile变量count的最新值。**
每次修改曹志伟做了结构上的改变，比如 增加或者删除，都要写 count 值，每次读取操作开始的时候，都要读 count 的值。
3. 若读时，发生指令重排序现象，则加锁重新读取。ConcurrentHashMap 是不允许存储 null 值的，如果读到了 null 值，说明发生了指令重排序，需要加锁重新读取。

总之，其读操作不需要加锁，主要是由下面的特性决定的：
用HashEntery对象的不变性来降低读操作对加锁的需求；
用Volatile变量协调读写线程间的内存可见性；
若读时发生指令重排序现象，则加锁重读；

2.ConcurrentHashMap对Segment的put操作是加锁完成的。Segment是ReentrantLock的子类，因此Segment本身就是一种可重入的Lock，所以我们可以直接调用其继承而来的lock()方法和unlock()方法对代码进行上锁/解锁。这里的加锁操作是针对某个具体的Segment，锁定的也是该Segment而不是整个ConcurrentHashMap。因为插入键/值对操作只是在这个Segment包含的某个桶中完成，不需要锁定整个ConcurrentHashMap。因此，其他写线程对另外15个Segment的加锁并不会因为当前线程对这个Segment的加锁而阻塞。
五、跨 segment 操作
比如 Size() 操作
如果我们要统计整个ConcurrentHashMap里元素的大小，那么就必须统计所有Segment里元素的大小后求和。虽然，Segment里的全局变量count是一个volatile变量，那但并不是直接把所有Segment的count相加就可以得到整个ConcurrentHashMap大小。
安全的做法，是在统计size的时候把所有Segment的put，remove和clean方法全部锁住，但是这种做法显然非常低效。
jdk 中是这样做的，size方法主要思路是，先在没有锁的情况下对所有段大小求和，这种求和策略最多执行RETRIES_BEFORE_LOCK次(默认是两次)。如果统计的过程中，容器的count发生了变化，则再采用加锁的方式来统计所有Segment的大小。
那么 ConcurrentHashMap 是怎么知道容器的结构发生了变化呢，segmeng 包含一个modCount的成员变量，在会引起段发生结构性改变的所有操作(put操作、 remove操作和clean操作)里，都会将变量modCount进行加1，因此，**JDK只需要在统计size前后比较modCount是否发生变化就可以得知容器的大小是否发生变化。**

