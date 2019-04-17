# Android 面试题整理总结（二）Java 集合 - 我的学习笔记 - CSDN博客





2018年12月01日 22:19:51[zouzhiheng](https://me.csdn.net/u011330638)阅读数：48








# 目录

1、Java集合框架中有哪些类？都有什么特点？区别是什么?

2、集合、数组、泛型的关系和区别？

3、HashMap 相关：

3.1 解释一下 HashMap，Java为什么要推出 HashMap？

3.2 底层实现了什么数据结构？

3.3 put、get 元素的过程？体现了什么数据结构？

3.4 HashMap 是有序的吗？如何实现有序？

3.5 是采用什么方法处理冲突的？分离链接法的优缺点？

3.6 Hashmap 的参数及影响性能的关键参数：加载因子和初始容量？

3.7 Resize操作的过程？如何避免扩容？

3.8 容量为 2 次幂的原因？

3.9 如何保证 HashMap 线程安全？
4、HashSet、HashMap、HashTable、ConcurrentHashMap 的区别？为什么 Hashtable 被弃用？

5、容器类中 fastfail 的概念。

# 题解

## 1、Java 集合框架中有哪些类？都有什么特点？区别是什么？

可以分为两类：Collection 和 Map，Map 和 Collection 唯一重叠的地方是 Map 可以使用 entrySet() 和 values() 方法来产生 Collection

Collection：

|-----List

|-----|----ArrayList

|-----|----LinkedList

|-----Set

|-----|----HashSet

|-----|----LinkedHashSet

|-----|----TreeSet

|-----Queue

|-----|----BlockingQueue
Map

|-----HashMap

|-----LinkedHashMap

|-----TreeMap

|-----WeakHashMap
List、Set、Queue 的区别：

List 是有序列表，可以通过下标访问，成员可以重复，允许拥有多个 null 元素

Set 的成员不可重复，最多拥有一个 null 元素

Queue 是队列，先进先出
ArryList 和 LinkedList 的区别：

ArrayList 内部使用数组实现，随机访问效率高

LinkedList  内部使用链表实现，插入删除效率高
HashMap、LinkedHashMap、TreeMap、WeakHashMap 的区别：

HashM 基于散列查找算法实现，效率最好

LinkedHashMap 维护着一个遍历所有条目的双向链表，因此可以按照插入顺序迭代元素，效率次之

TreeMap 内部基于红黑树实现，按升序（Comparable）排列元素，效率最慢

WeakHashMap 的内部类 Entry 继承自 WeakReference，因此 GC 时，元素可能会被自动删除

ConcurrentHashMap 是并发安全的 HashMap
HashSet、 LinkedHashSet、TreeSet 的区别：

HashSet 内部基于 HashMap 实现，效率最好

LinkedHashSet 继承自 HashSet，维护着一个遍历所有条目的双向链表，因此可以按照插入顺序迭代元素，效率次之

TreeSet 内部基于 TreeMap(红黑树) 实现，按升序（Comparable）排列元素，效率最慢
## 2、集合、数组、泛型的关系和区别？

数组自创建起，其尺寸就不允许被修改，不能配合泛型使用

集合可以动态地修改尺寸，可以配合泛型使用，用于在编译差检查元素类型，但在运行时，参数类型不同的两个集合，实际上还是同一个类型

## 3、HashMap 相关

### 3.1 解释一下 HashMap，Java为什么要推出 HashMap？
- 基于散列查找算法实现，put 和 get 都可以提供时间复杂度为 O(1) 的性能
- 内部维护一个 Node 数组，Node 除了用于存储 key & value 之外，还有指向下一个元素的指针，即 HashMap 采用的冲突处理方法是分离链接法
- 初始容量是 16，加载因子是 0.75，即如果存储的元素个数大于容量 * 0.75 时，就会执行扩容操作，扩容会在原来的基础上扩大 2 倍，

以前 Java 使用的是 HashTable，但这个类无论是单线程还是多线程场景下的性能都不好，因为它的每个成员方法都会简单粗暴地添加 synchronized 修饰符，这种方法的代价是严重降低了程序的可伸缩性，当多个线程竞争容器的锁时，吞吐量将严重下降。因此推出了 HashMap 和 ConcurrentHashMap，HashMap 线程不安全，适合单线程下使用，ConcurrentHashMap 是线程安全的，同时因为使用了分段锁机制，效率比 HashTable 高很多

### 3.2 底层实现了什么数据结构？

底层实现了一个桶数组，数组中的每个元素都是一个链表，链表中的节点使用内部了 Node 表示

### 3.3 put、get 元素的过程？

put 会把 key & value 封装为 Node(如果是 LinkedHashMap，那么是 TreeNode)，然后计算哈希值，找到存储该对象的数组下标，然后 put 进链表末尾，如果 put 之后 size > capacity * loadFactor，那么会调整 Node 数组。

```
public class HashMap<K,V> extends AbstractMap<K,V>
    implements Map<K,V>, Cloneable, Serializable {

    static final int DEFAULT_INITIAL_CAPACITY = 1 << 4; // aka 16

    static final float DEFAULT_LOAD_FACTOR = 0.75f;
    
    static class Node<K,V> implements Map.Entry<K,V> {
        final int hash;
        final K key;
        V value;
        Node<K,V> next;

        ...
    }
    
    transient Node<K,V>[] table;

    public V put(K key, V value) {
        return putVal(hash(key), key, value, false, true);
    }

    final V putVal(int hash, K key, V value, boolean onlyIfAbsent,
                   boolean evict) {
        Node<K,V>[] tab; Node<K,V> p; int n, i;
        if ((tab = table) == null || (n = tab.length) == 0)
            n = (tab = resize()).length; // 如果 table 为空，则 resize 扩容

        if ((p = tab[i = (n - 1) & hash]) == null) // 如果数组对应的位置上没有存储元素，则直接存储
            tab[i] = newNode(hash, key, value, null);
        else {
            Node<K,V> e; K k;
            if (p.hash == hash &&
                ((k = p.key) == key || (key != null && key.equals(k)))) // put 进来的对象和头结点是同一个对象，更新值即可
                e = p;
            else if (p instanceof TreeNode) // LinkedHashMap 使用，这里略过
                e = ((TreeNode<K,V>)p).putTreeVal(this, tab, hash, key, value);
            else {
                for (int binCount = 0; ; ++binCount) { // 遍历链表
                    if ((e = p.next) == null) { // 如果没找到相同的元素，则创建新的元素并放到链表的尾部
                        p.next = newNode(hash, key, value, null); 
                        if (binCount >= TREEIFY_THRESHOLD - 1) // 如果数组同一位置上存储的元素过多，则扩容
                            treeifyBin(tab, hash);
                        break;
                    }
                    if (e.hash == hash &&
                        ((k = e.key) == key || (key != null && key.equals(k)))) // 如果在链表中找到了和对象相同的元素，更新值即可
                        break;
                    p = e;
                }
            }
            if (e != null) { // existing mapping for key
                V oldValue = e.value;
                if (!onlyIfAbsent || oldValue == null)
                    e.value = value; // 更新链表中和对象相同的元素的值
                afterNodeAccess(e);
                return oldValue;
            }
        }
        ++modCount; // 用于判断是否抛出 ConcurrentModification
        if (++size > threshold) // 扩容
            resize();
        afterNodeInsertion(evict);
        return null;
    }
    
}
```

get 方法会逐个比较元素的 hash 值和 equals 方法，直到找到相同的元素：

```
public V get(Object key) {
        Node<K,V> e;
        return (e = getNode(hash(key), key)) == null ? null : e.value;
    }

    final Node<K,V> getNode(int hash, Object key) {
        Node<K,V>[] tab; Node<K,V> first, e; int n; K k;
        if ((tab = table) != null && (n = tab.length) > 0 &&
            (first = tab[(n - 1) & hash]) != null) {
            // 首先比较头结点
            if (first.hash == hash && 
                ((k = first.key) == key || (key != null && key.equals(k))))
                return first;
            if ((e = first.next) != null) {
                if (first instanceof TreeNode)
                    return ((TreeNode<K,V>)first).getTreeNode(hash, key);
                do {
                    if (e.hash == hash &&
                        ((k = e.key) == key || (key != null && key.equals(k))))
                        return e;
                } while ((e = e.next) != null); // 循环链表，逐个比较 hash 值和 equals 方法
            }
        }
        return null;
    }
```

### 3.4 HashMap 是有序的吗？如何实现有序？

不是。

如果要维护元素的插入顺序，则使用 LinkedHashMap，LinkedHashMap 内部维护了一个双向链表

如果要维护元素的自然顺序（Comparable），则使用 TreeMap，TreeMap 基于红黑树实现

### 3.5 是采用什么方法处理冲突的？分离链接法的优缺点？

分离链接法

散列查找的冲突处理方法常用的有 2 种：开放地址法、链地址法。它们的优缺点如下：

开放地址法（平方探测法）：
- 散列表是一个数组，存储、随机查找的效率高
- 但容易出现“聚集”现象，从而降低查找、存储的效率，
- 而且不能直接删除元素，要用标记代替，否则平方探测法进行不下去

链地址法（分离链接法）：
- 数组+链表实现，存储和查找的效率相对较低（要遍历链表）
- 元素可以直接删除，即不会存储垃圾
- 加载因子设置不合理，或链表长度不均匀都会导致时间效率的严重下降

### 3.6 Hashmap 的参数及影响性能的关键参数：加载因子和初始容量？

初始容量为 16，加载因子为 0.75，

### 3.7 Resize 操作的过程？如何避免扩容？

resize 会新建一个 size 为原来的两倍的 table，并根据元素的哈希值是否大于等于原来的 table 的容量，而选择是否移动到新位置。哈希值较小的按照顺序依然保存在原位置，哈希值较大的按照顺序移动到新位置（index + capacity）。

避免扩容：
- 加载因子要设置合理，太小会导致空间浪费，太大会付出更多的时间代价
- 类的 hashCode 返回值要合理，equals 和 hashCode 要同时复写，尽量保证 equals 和 hashCode 要么同时相等，要么同时不相等

```
final Node<K,V>[] resize() {
        Node<K,V>[] oldTab = table;
        int oldCap = (oldTab == null) ? 0 : oldTab.length;
        int oldThr = threshold;
        int newCap, newThr = 0;
        if (oldCap > 0) {
            if (oldCap >= MAXIMUM_CAPACITY) {
                threshold = Integer.MAX_VALUE;
                return oldTab;
            }
            else if ((newCap = oldCap << 1) < MAXIMUM_CAPACITY &&
                     oldCap >= DEFAULT_INITIAL_CAPACITY)
                newThr = oldThr << 1; // double threshold
        }
        else if (oldThr > 0) // initial capacity was placed in threshold
            newCap = oldThr;
        else {               // zero initial threshold signifies using defaults
            newCap = DEFAULT_INITIAL_CAPACITY;
            newThr = (int)(DEFAULT_LOAD_FACTOR * DEFAULT_INITIAL_CAPACITY);
        }
        if (newThr == 0) {
            float ft = (float)newCap * loadFactor;
            newThr = (newCap < MAXIMUM_CAPACITY && ft < (float)MAXIMUM_CAPACITY ?
                      (int)ft : Integer.MAX_VALUE);
        }
        threshold = newThr;
        @SuppressWarnings({"rawtypes","unchecked"})
            Node<K,V>[] newTab = (Node<K,V>[])new Node[newCap];
        table = newTab;
        if (oldTab != null) {
            for (int j = 0; j < oldCap; ++j) {
                Node<K,V> e;
                if ((e = oldTab[j]) != null) {
                    oldTab[j] = null;
                    if (e.next == null) // 只有一个元素，直接复制
                        newTab[e.hash & (newCap - 1)] = e;
                    else if (e instanceof TreeNode)
                        ((TreeNode<K,V>)e).split(this, newTab, j, oldCap);
                    else { // 找到链表的头尾结点，以保留元素顺序
                        Node<K,V> loHead = null, loTail = null; // 用于保存到原位置
                        Node<K,V> hiHead = null, hiTail = null; // 用于移动到新位置
                        Node<K,V> next;
                        do {
                            next = e.next;
                            // 在这个位置上的元素，要么是取余之后的值等于 index，要么本身等于 index
                            // 因此如果这个判断条件成立，则说明元素 e 的哈希值大于等于原来的桶的容量，应该移动到新位置
                            if ((e.hash & oldCap) == 0) { 
                                if (loTail == null)
                                    loHead = e;
                                else
                                    loTail.next = e;
                                loTail = e;
                            }
                            else { // 否则说明元素 e 的哈希值小于等于桶的容量，依然在原位置
                                if (hiTail == null)
                                    hiHead = e;
                                else
                                    hiTail.next = e;
                                hiTail = e;
                            }
                        } while ((e = next) != null);
                        if (loTail != null) {
                            loTail.next = null;
                            newTab[j] = loHead; // 哈希值较小的保存回原位置
                        }
                        if (hiTail != null) {
                            hiTail.next = null;
                            newTab[j + oldCap] = hiHead; // 哈希值较大的移动到新位置
                        }
                    }
                }
            }
        }
        return newTab;
    }
```

### 3.8 容量为 2 次幂的原因？

这样可以使用位运算来简化实现并提高性能。比如扩容时，右移一位即可，不需要做额外的计算；寻找对象存储的位置时，可以通过 hash & (SIZE - 1)，而不是 hash % SIZE。

### 3.9 如何保证 HashMap 线程安全？

可以通过 Collections.synchronizedMap 返回一个线程安全的 Map

也可以使用 ConcurrentHashMap，不保证绝对的线程安全（size 和 empty 返回的结果可能是过时的），但效率更高

## 4、HashSet、HashMap、HashTable、ConcurrentHashMap 的区别？为什么 Hashtable 被弃用？

HashTable 和 HashMap 的区别：
- 继承关系。HashTable 继承于旧的 Directory 类，HashMap 继承 AbstractMap，而 AbstractMap 是 Map 接口的实现
- 线程安全。HashTable 是同步的，HashMap 是非同步的
- null 值。HashTable 的 Key-Value 都不允许 null 值，HashMap 都允许
- 遍历方式。HashTable 使用 Enumeration 遍历，HashMap 使用 Iterator 遍历
- hash 值计算。HashTable 直接调用对象的 hashcode 方法计算 hash 值，而 HashMap 重新计算 hash 值
- 扩容。HashTable 默认大小为 11，扩容方式是 old*2+1，HashMap 默认大小为 16，每次扩充为原来的两倍大小

HashSet 内部基于 HashMap 实现，可以认为是 HashMap 的 KeySet。

ConcurrentMap 并不是将每个方法都在同一个锁上同步（这种情况下每次只能有一个线程访问容器），而是使用分段锁机制来实现线程安全性，使得任意数量的读取线程可以并发地访问 Map，执行读取操作的线程和执行写入操作的线程可以并发地访问 Map，并且一定数量的写入线程可以并发的修改 Map，因此，ConcurrentMap 可以在并发访问环境下将实现更高的吞吐量，而在单线程环境中只损失非常小的性能。但对于一些需要在整个 Map 上进行计算的方法，例如 size 和 isEmpty，返回的结果可能已经过期了。分段锁等可以参考 Java 并发编程实战，顾名思义，不同区域的数据会通过不同的锁进行同步控制，而在 ConcurrentMap 的实现中，会以链表的头结点作为同步对象，这意味着可以并发地修改不同链表上的元素。

HashTable 简单粗暴地在每个方法的声明上使用了 synchronized 修饰，这种方法的代价是严重降低了程序的可伸缩性，当多个线程竞争容器的锁时，吞吐量将严重下降。

## 5、容器类中 fail-fast 的概念

faile-fast 是 Java 集合的一种错误检测机制，当多个线程对同一个集合进行修改时，有可能抛出异常 ConcurrentModificationException。比如有两个线程同时运行，线程 1 用于遍历集合 A 中的元素，线程 2  对集合 A 进行修改，那么这个时候就有可能会抛出异常 ConcurrentModificationException。

这种检查是在没有同步的情况下进行的，因此并不保证出现错误时一定会被捕捉到，这是一种设计上的权衡，从而降低并发修改操作的检测代码对程序性能带来的影响。

以 ArrayList 为例：

```
public class ArrayList<E> extends AbstractList<E>
        implements List<E>, RandomAccess, Cloneable, java.io.Serializable
{

    /**
     * The number of times this list has been <i>structurally modified</i>.
     * Structural modifications are those that change the size of the
     * list, or otherwise perturb it in such a fashion that iterations in
     * progress may yield incorrect results. (结构性修改：尺寸的改变，或其它尺寸被扰乱的方式)
     */
    protected transient int modCount = 0;

    public E remove(int index) {
        if (index >= size)
            throw new IndexOutOfBoundsException(outOfBoundsMsg(index));

        modCount++; // 记录修改次数
        E oldValue = (E) elementData[index];

        int numMoved = size - index - 1;
        if (numMoved > 0)
            System.arraycopy(elementData, index+1, elementData, index,
                             numMoved);
        elementData[--size] = null; // clear to let GC do its work

        return oldValue;
    }
    
    @Override
    public void forEach(Consumer<? super E> action) {
        Objects.requireNonNull(action);
        final int expectedModCount = modCount; // 保存原修改次数
        @SuppressWarnings("unchecked")
        final E[] elementData = (E[]) this.elementData;
        final int size = this.size;
        for (int i=0; modCount == expectedModCount && i < size; i++) {
            action.accept(elementData[i]);
        }

        // 比较修改次数是否和原来的一致，如果不一致，则抛出异常
        if (modCount != expectedModCount) {
            throw new ConcurrentModificationException();
        }
    }

}
```

即 fast-fail 会在 modCount != expectedModCount 的时候抛出异常 ConcurrentModificationException。










