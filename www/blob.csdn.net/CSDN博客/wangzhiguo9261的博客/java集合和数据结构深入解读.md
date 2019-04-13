
# java集合和数据结构深入解读 - wangzhiguo9261的博客 - CSDN博客


2018年02月11日 15:31:17[码里偷闲王果果](https://me.csdn.net/wangzhiguo9261)阅读数：45


参考博客[http://www.cnblogs.com/skywang12345/p/3323085.html](http://www.cnblogs.com/skywang12345/p/3323085.html)
Java集合在java的世界中是非常重要的一部分，主要集合包括List列表，Set集合，Map映射还有Iterator迭代器、Enumeration枚举
集合的数据结构主要有集合、链表、队列、栈、数组、映射等
**一、数组：**
**Vector**
1）、线程安全的，通过加锁的方式实现
2）、长度可变，当长度不够的时候，如果没有初始化capacityIncrement>0，则会将长度扩展为原来的两倍**ArrayList**
1）、线性表，查找速度快，更新速度慢(在线性表前面添加元素会将后边的元素都后移动，如果前面删除，会将后面元素前移，导致性能低)
**LinkedList**
1）、双向链表，记录了当前节点元素和前后节点的元素信息，因此更新速度相对比较快，但是查找速度相对慢点（随机查找一个元素：按照顺序从列表的一端开始检查，直到另外一端）
**Stack**
1）、先进后出(FILO, First In Last Out)。
2）、Stack也是通过数组实现的，而非链表
3）、执行push时(即，将元素推入栈中)，是通过将元素追加的数组的末尾中。
执行peek时(即，取出栈顶元素，不执行删除)，是返回数组末尾的元素。
执行pop时(即，取出栈顶元素，并将该元素从栈中删除)，是取出数组末尾的元素，然后将该元素从数组中删除
**二、map集合**
Map主要接口：
1 ）Map
2 ）Map.Entry
3 ）AbstractMap
4 ）SortedMap
SortedMap的排序方式有两种：自然排序 或者 用户指定比较器
5 ）NavigableMap
NavigableMap是继承于SortedMap的接口。它是一个可导航的键-值对集合，具有了为给定搜索目标报告最接近匹配项的导航方法
NavigableMap除了继承SortedMap的特性外，它的提供的功能可以分为4类：
第1类，提供操作键-值对的方法。
lowerEntry、floorEntry、ceilingEntry 和 higherEntry 方法，它们分别返回与小于、小于等于、大于等于、大于给定键的键关联的 Map.Entry 对象。
firstEntry、pollFirstEntry、lastEntry 和 pollLastEntry 方法，它们返回和/或移除最小和最大的映射关系（如果存在），否则返回 null。
第2类，提供操作键的方法。这个和第1类比较类似
lowerKey、floorKey、ceilingKey 和 higherKey 方法，它们分别返回与小于、小于等于、大于等于、大于给定键的键。
第3类，获取键集。
navigableKeySet、descendingKeySet分别获取正序/反序的键集。
第4类，获取键-值对的子集。
6 ）Dictionary
**HashMap**
1）只允许一条记录的键为Null;允许多条记录的值为Null;HashMap不支持线程的同步
2）HashMap的底层主要是基于数组和链表（单向链表）来实现的
3）HashMap中主要是通过key的hashCode来计算hash值的，只要hashCode相同，计算出来的hash值就一样。如果存储的对象对多了，
就有可能不同的对象所算出来的hash值是相同的，这就出现了所谓的hash冲突。学过数据结构的同学都知道，
解决hash冲突的方法有很多，HashMap底层是通过链表来解决hash冲突的
4）遍历效率问题：
第一种:
Map map = new HashMap();
Iterator iter = map.entrySet().iterator();
while (iter.hasNext()) {
Map.Entry entry = (Map.Entry) iter.next();
Object key = entry.getKey();
Object val = entry.getValue();
}
效率高,以后一定要使用此种方式！
第二种:
Map map = new HashMap();
Iterator iter = map.keySet().iterator();
while (iter.hasNext()) {
Object key = iter.next();
Object val = map.get(key);
}
效率低,以后尽量少使用！原因：遍历了两次
**LinkedHashMap**
1）不支持线程同步,而且键和值都可以是null
2）与HashMap相比LinkedHashMap维护的是一个具有双向链表的HashMap
3）LinkedHashMap支持2中排序一种是插入排序，一种是使用排序，最近使用的会移至尾部例如 M1 M2 M3 M4，使用M3后为 M1 M2 M4 M3了，
LinkedHashMap输出时其元素是有顺序的，而HashMap输出时是随机的，如果Map映射比较复杂而又要求高效率的话，最好使用LinkedHashMap，
但是多线程访问的话可能会造成不同步，所以要用Collections.synchronizedMap来包装一下，从而实现同步。其实现一般为：
Map<String String> map = Collections.synchronizedMap(new LinkedHashMap(<String String));
4）由于的链表的增加、删除操作是常量级的，故并不会带来性能的损失
**WeakHashMap**
1）不支持线程同步，而且键和值都可以是null
2）散列表，它存储的内容也是键值对(key-value)映射
3）WeakHashMap 中，当某个键不再正常使用时，会被从WeakHashMap中被自动移除
4）“弱键”如何被自动从WeakHashMap中删除的步骤
(01) 新建WeakHashMap，将“键值对”添加到WeakHashMap中。
实际上，WeakHashMap是通过数组table保存Entry(键值对)；每一个Entry实际上是一个单向链表，即Entry是键值对链表。
(02) 当某“弱键”不再被其它对象引用，并被GC回收时。在GC回收该“弱键”时，这个“弱键”也同时会被添加到ReferenceQueue(queue)队列中。
(03) 当下一次我们需要操作WeakHashMap时，会先同步table和queue。table中保存了全部的键值对，而queue中保存被GC回收的键值对；同步它们，就是删除table中被GC回收的键值对
5）Demo
public static void main(String[] args) throws Exception {
String a = new String("a");
String b = new String("b");
Map weakmap = new WeakHashMap();
Map map = new HashMap();
map.put(a, "aaa");
map.put(b, "bbb");
weakmap.put(a, "aaa");
weakmap.put(b, "bbb");
map.remove(a);
a=null;
b=null;
System.gc();
Iterator i = map.entrySet().iterator();
while (i.hasNext()) {
Map.Entry en = (Map.Entry)i.next();
System.out.println("map:"+en.getKey()+":"+en.getValue());
}
Iterator j = weakmap.entrySet().iterator();
while (j.hasNext()) {
Map.Entry en = (Map.Entry)j.next();
System.out.println("weakmap:"+en.getKey()+":"+en.getValue());
}
}
**HashTable**
1）它支持线程的同步，它不允许记录的键或者值为空;
2）Hashtable 继承于Dictionary
3）和HashMap一样，通过数组和链表实现。Hashtable是通过"拉链法"实现的哈希表，解决哈希冲突问题
4）依赖了Enumeration，通过Enumeration遍历Hashtable的键
根据keys()获取Hashtable的集合
Enumeration enu = table.keys();
while(enu.hasMoreElements()) {
System.out.println(enu.nextElement());
}
根据elements()获取Hashtable的集合
Enumeration enu = table.elements();
while(enu.hasMoreElements()) {
System.out.println(enu.nextElement());
}
**TreeMap**
1）TreeMap是非同步的，它不允许记录的键或者值为空
2）通过红黑树实现，TreeMap的本质是R-B Tree(红黑树）。
3）记录根据键排序,默认是按升序排序，也可以指定排序的比较器
**IdentityHashMap**
1）非同步的，允许 null 值和 null
2）此为简单的线性探头哈希表
3）当且仅当 (k1==k2) 时，才认为两个键 k1 和 k2 相等（在正常 Map 实现（如 HashMap）中，
当且仅当满足下列条件时才认为两个键 k1 和 k2 相等：(k1==null ? k2==null : e1.equals(e2))）。
4）key允许重复，只要两个对象的地址不相等即可
5）Demo
public static void main(String[] args) {
IdentityHashMap ihm = new IdentityHashMap();
// 下面两行代码向IdentityHashMap对象添加两个key-value对
ihm.put(new String("语文"), 89);
ihm.put(new String("语文"), 78);
// 下面两行代码只会向IdentityHashMap对象添加一个key-value对
ihm.put("java", 93);
ihm.put("java", 98);
System.out.println(ihm);
}
**三、set集合**
(01) Set 是继承于Collection的接口。它是一个不允许有重复元素的集合。
(02) AbstractSet 是一个抽象类，它继承于AbstractCollection，AbstractCollection实现了Set中的绝大部分函数，为Set的实现类提供了便利。
(03) HastSet 和 TreeSet 是Set的两个实现类。
HashSet依赖于HashMap，它实际上是通过HashMap实现的。HashSet中的元素是无序的。
TreeSet依赖于TreeMap，它实际上是通过TreeMap实现的。TreeSet中的元素是有序的
**HashSet**
1）HashSet是非同步的，HashSet允许使用 null 元素
2）通过HashMap实现，hashMap存储的是key-value，hashSet存储数据时候，value用空对象
**LinkedHashSet**
1）不支持线程同步
2）通过LinkedHashMap实现
**TreeSet**
1）TreeSet是非同步的
2）TreeSet是基于TreeMap实现的。TreeMap存储的是key-value，hashSet存储数据时候，value用空对象
3）TreeSet中的元素支持2种排序方式：自然排序 或者 根据创建TreeSet 时提供的 Comparator 进行排序
**四、迭代、枚举**
区别；
(01) 函数接口不同
Enumeration只有2个函数接口。通过Enumeration，我们只能读取集合的数据，而不能对数据进行修改。
Iterator只有3个函数接口。Iterator除了能读取集合的数据之外，也能数据进行删除操作。
(02) Iterator支持fail-fast机制，而Enumeration不支持。
**五、队列**
1、Queue：队列是一种特殊的线性表，它只允许在表的前端（front）进行删除操作，而在表的后端（rear）进行插入操作，FIFO
2、Deque：双端队列是指既可以在表的前端进行插入和删除操作，又可以在表的后端进行插入和删除操作
3、ArrayDeque:数组实现的双线向列
4、PriorityQueue:
1)在PriorityQueue中，当你添加元素到Queue中时，实现了自动排序。根据你使用的PriorityQueue的不同构造器，Queue元素的顺序要么基于他们的自然顺序要么通过PriorirtyQueue构造器传入的Comparator来确定
2)DEMO
PriorityQueue<Object> priorityQueue =  new PriorityQueue(6, new Comparator<Object>()
{
public int compare(Object a, Object  b)
{
System.out.println("Comparing Populations");
if (a.hashCode()>b.hashCode())
return 1;
else if (a.hashCode()<b.hashCode())
return -1;
else
return 0;
}
}
);
**六、常量类**
Arrays
Collections
版权声明：本文为博主原创文章，未经博主允许不得转载。

