# jetty9优化的两处地方 - zhanlijun - 博客园
# [jetty9优化的两处地方](https://www.cnblogs.com/LBSer/p/3637387.html)
#### [http://www.cnblogs.com/LBSer/p/3637387.html](http://www.cnblogs.com/LBSer/p/3637387.html)
jetty 9两个优化：
https://webtide.intalio.com/2013/01/jetty-9-goes-fast-with-mechanical-sympathy/?utm_source=tuicool
1. False Sharing in Queues
原先使用了[BlockingArrayQueue](http://git.eclipse.org/c/jetty/org.eclipse.jetty.project.git/tree/jetty-util/src/main/java/org/eclipse/jetty/util/BlockingArrayQueue.java)，这个queue有头尾两个指针，生产和消费是独立的，但是会产生这样一个问题；“However because of the layout in memory of the class, it turned out that the head and tail pointers and locks were all within a single CPU cache row。This is bad because when different threads running on different cores are trying to independently work on the head and tail, it turns out that they are both hitting the same area of memory and are thus repeatedly invalidating each others caches in a pattern called [false sharing](http://en.wikipedia.org/wiki/False_sharing)”
解决方法：
“The solution is to be aware of the memory layout of the class when considering what threads will be accessing which fields and to space them out so that you can avoid this[false sharing](http://en.wikipedia.org/wiki/False_sharing) of cache rows. ”
从代码上看（http://git.eclipse.org/c/jetty/org.eclipse.jetty.project.git/tree/jetty-util/src/main/java/org/eclipse/jetty/util/BlockingArrayQueue.java）：
```
public class BlockingArrayQueue<E> extends AbstractList<E> implements BlockingQueue<E>
{
    /**
     * The head offset in the {@link #_indexes} array, displaced by 15 slots to avoid false sharing with the array length (stored before the first element of
     * the array itself).
     */
    private static final int HEAD_OFFSET = MemoryUtils.getIntegersPerCacheLine() - 1;
    /**
     * The tail offset in the {@link #_indexes} array, displaced by 16 slots from the head to avoid false sharing with it.
     */
    private static final int TAIL_OFFSET = HEAD_OFFSET + MemoryUtils.getIntegersPerCacheLine();
    /**
     * Default initial capacity, 128.
     */
    public static final int DEFAULT_CAPACITY = 128;
    /**
     * Default growth factor, 64.
     */
    public static final int DEFAULT_GROWTH = 64;
    private final int _maxCapacity;
    private final int _growCapacity;
    /**
     * Array that holds the head and tail indexes, separated by a cache line to avoid false sharing
     */
    private final int[] _indexes = new int[TAIL_OFFSET + 1];
    private final Lock _tailLock = new ReentrantLock();
    private final AtomicInteger _size = new AtomicInteger();
    private final Lock _headLock = new ReentrantLock();
    private final Condition _notEmpty = _headLock.newCondition();
    private Object[] _elements;
...
}
```
2. Time and Space Efficient Trie
　　解析HTTP Header的时候，我们常常需要将在ByteBuffer的数据（ByteBuffer不在jvm内存中）转换成String类，通常还放到一个hashmap中，这样会产生大量的开销（创建对象耗时，内存消耗）。jetty9为了不将ByteBuffer的数据进行转换，不采用hashmap，而使用trie这种数据结构。
　　jetty代码提交者们使用了不同类型的trie进行尝试。1）首先使用了[TreeTrie](http://git.eclipse.org/c/jetty/org.eclipse.jetty.project.git/tree/jetty-util/src/main/java/org/eclipse/jetty/util/TreeTrie.java)，但是[TreeTrie](http://git.eclipse.org/c/jetty/org.eclipse.jetty.project.git/tree/jetty-util/src/main/java/org/eclipse/jetty/util/TreeTrie.java)缺乏空间局部性，性能较差；2）之后使用[ArrayTrie](http://git.eclipse.org/c/jetty/org.eclipse.jetty.project.git/tree/jetty-util/src/main/java/org/eclipse/jetty/util/ArrayTrie.java) ，但是[ArrayTrie](http://git.eclipse.org/c/jetty/org.eclipse.jetty.project.git/tree/jetty-util/src/main/java/org/eclipse/jetty/util/ArrayTrie.java) 内存开销过大，当有成千上万请求时可能会造成GC问题；3）最终使用了[ArrayTernaryTrie](http://git.eclipse.org/c/jetty/org.eclipse.jetty.project.git/tree/jetty-util/src/main/java/org/eclipse/jetty/util/ArrayTernaryTrie.java)（http://grepcode.com/file/repo1.maven.org/maven2/org.eclipse.jetty/jetty-util/9.1.0.v20131115/org/eclipse/jetty/util/ArrayTernaryTrie.java），[ArrayTernaryTrie](http://git.eclipse.org/c/jetty/org.eclipse.jetty.project.git/tree/jetty-util/src/main/java/org/eclipse/jetty/util/ArrayTernaryTrie.java)节省空间，且查找速度不低于hashmap。
3. 性能评测
　　“Thus for a small increase in static heap usage (0.5MB in the static Tries), jetty-9 out performs jetty-8 by 30% faster (33.5s vs 48.6s) and 50% less YG garbage (1409MB vs 2914MB) which trigger less than half the YG collections.”
ternary-search-tree参考文献：
http://igoro.com/archive/efficient-auto-complete-with-a-ternary-search-tree/ 

