# Java深入 - 深入理解Java集合 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年12月06日 11:46:59[initphp](https://me.csdn.net/initphp)阅读数：3294
所属专栏：[Java学习笔记](https://blog.csdn.net/column/details/javago.html)[Java深入](https://blog.csdn.net/column/details/myjava.html)









### 集合

集合类存放于java.util包中。

**集合类型主要有3种：set(集）、list(列表包含Queue）和map(映射)。**

![](https://img-blog.csdn.net/20160411103651336?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

Collection：Collection是集合的基本接口，List、Set、Queue的最基本的接口。

Iterator：迭代器，可以通过迭代器遍历集合中的数据

Map：是映射表的基础接口







### List 有序集合

Java的List是非常常用的数据类型。List是有序的Collection。Java List一共三个实现类：分别是ArrayList、Vector和LinkedList。

**ArrayList**：ArrayList是最常用的List实现类，内部是通过数组实现的，它允许对元素进行快速随机访问。数组的缺点是每个元素之间不能有间隔，当数组大小不满足时需要增加存储能力，就要讲已经有数组的数据复制到新的存储空间中。当从ArrayList的中间位置插入或者删除元素时，需要对数组进行复制、移动、代价比较高。因此，它适合随机查找和遍历，不适合插入和删除。

**Vector：**Vector与ArrayList一样，也是通过数组实现的，不同的是它支持线程的同步，即某一时刻只有一个线程能够写Vector，避免多线程同时写而引起的不一致性，但实现同步需要很高的花费，因此，访问它比访问ArrayList慢。

**LinkedList：**LinkedList是用链表结构存储数据的，很适合数据的动态插入和删除，随机访问和遍历速度比较慢。另外，他还提供了List接口中没有定义的方法，专门用于操作表头和表尾元素，可以当作堆栈、队列和双向队列使用。




**说明：**

1. ArrayList在内存不够时默认是扩展50% + 1个，Vector是默认扩展1倍。

2. Vector属于线程安全级别的，但是大多数情况下不使用Vector，因为线程安全需要更大的系统开销。

3. 一般使用ArrayList和LinkedList比较多

**4. 对于随机访问get和set，ArrayList觉得优于LinkedList，因为LinkedList要移动指针**

**5. 对于新增和删除操作add和remove，LinedList比较占优势，因为ArrayList要移动数据**





#### ArrayList

ArrayList是最常用的List实现类。ArrayList内部是通过数组实现的。所以只适合遍历或者随机查找。

**常用的接口：**



```java
list.add(int index,E element) 将数据新增到指定位置
list.add(E element) 将指定的元素添加到此列表的尾部。
list.addAll(Collection<? extendsE> c) 按照指定 collection 的迭代器所返回的元素顺序，将该 collection 中的所有元素添加到此列表的尾部。
list.addAll(int index,Collection<? extendsE> c) 从指定的位置开始，将指定 collection 中的所有元素插入到此列表中。
list.clear() 移除此列表中的所有元素。
list.set(int index,E element)  用指定的元素替代此列表中指定位置上的元素。
list.get(int index) 返回此列表中指定位置上的元素。
list.size() 查看元素总个数
list.contains() 包含
```






**例子：**



```java
public class Test2 {

    /**
     * 入口函数
     * @param args
     */
    public static void main(String[] args) throws Exception {

        /* 初始化一个数组 */
        List<Integer> list = new ArrayList<Integer>();

        /* 新增数据 */
        for (int j = 0; j < 5; j++) {
            list.add(j);
        }

        /* 总长度 */
        System.out.println(list.size());

        /* 删除一条数据 key=1 */
        list.remove(1);

        /* 遍历 */
        for (int i = 0; i < list.size(); i++) {
            System.out.println("for:" + list.get(i)); //获取值  
        }

        /* 将一个集合合并 */
        List<Integer> list2 = new ArrayList<Integer>();
        list2.add(99);
        list2.add(100);
        list.addAll(list2);

        /* 遍历2 */
        Iterator it = list.iterator();
        while (it.hasNext()) {
            System.out.println("Iterator:" + it.next()); //获取值  
        }

        /* 是否包含 */
        if (list.contains(2)) {
            System.out.println("包含");
        }

        /* 清空列表 */
        list.clear(); //清除整个列表  

    }
}
```




#### LinkedList

是以链表的结构进行存储对象的，动态新增和删除是很快，但是遍历就很慢，并且不存在get()的操作，不能单个定位。说白了，ArrayList是顺序存储结构，LinkedList是链表存储结构。

****

**常用接口：**



```java
public LinkedList() 生成空的链表
public LinkedList(Collection col):  复制构造函数
public boolean add(Object element) 添加元素
public boolean add(int index, Object element)
public boolean addFirst(Object element)
public boolean addLast(Object element)
list.addAll(Collection<? extendsE> c) 按照指定 collection 的迭代器所返回的元素顺序，将该 collection 中的所有元素添加到此列表的尾部。
list.removeFirst();
list.removeLast();
list.clear();
list.subList(2, 5).clear();
list.remove("2"); #删除特定元素
List<String> myList = new ArrayList<String>(list);  #转ArrayList
lList.indexOf("2")  查找元素位置
lList.lastIndexOf("2");
lList.set(3, "Replaced");
lList.contains("4"); 确认是否存在
```






**例子：**



```java
public class Test2 {

    /**
     * 入口函数
     * @param args
     */
    public static void main(String[] args) throws Exception {

        /* 生成一个LinkedList */
        LinkedList<String> list = new LinkedList<String>();
        list.add("a");
        list.add("b");
        list.add("c");
        list.add("d");

        /* list的size */
        int size = list.size();
        System.out.println(size);

        /* 获取第一个和最后一个元素 */
        String first = list.getFirst();
        String last = list.getLast();
        System.out.println("First:" + first + " Last:" + last);

        /* 新增第一个和最后一个元素 */
        list.addFirst("first");
        list.addLast("last");
        System.out.println("List内容 ：" + list);

        /* 移除最后一个和第一个 */
        list.removeFirst();
        list.removeLast();
        System.out.println("List内容 ：" + list);

        /* 删除特定元素 */
        list.remove("b");
        System.out.println("List内容 ：" + list);

        /* 查找元素位置 */
        int i = list.indexOf("c");
        System.out.println("位置 ：" + i);

        /* 是否包含某个元素 */
        if (list.contains("c")) {
            System.out.println("包含");
        }

        /* 设置某个元素 */
        list.set(1, "sadsad");
        System.out.println("List内容 ：" + list);

        /* 转为ArrayList */
        List<String> aList = new ArrayList<String>(list);
        for (String s : aList) {
            System.out.println("s = " + s);
        }

        /* 转为数组 */
        String[] my = list.toArray(new String[list.size()]);
        for (int j = 0; j < my.length; j++) {
            System.out.println(my[j]);
        }

        /* 组装list */
        LinkedList<String> list2 = new LinkedList<String>();
        list.add("ddd");
        list.add("111");
        list.addAll(list2);
        System.out.println("List内容 ：" + list);

        /* 遍历 */
        Iterator lit = list.iterator();
        while (lit.hasNext()) {
            System.out.println(lit.next());
        }

        /* 清除 */
        list.clear();
        System.out.println("List内容 ：" + list);
    }
}
```





### Set 集合

Set集合的几个特点：

1. Set集合不允许出现重复数据

2. 允许包含值为null的元素，但最多只能有一个null元素。




#### TreeSet

TreeSet的几个特点：

1. TreeSet中不能有重复的元素；

2. TreeSet具有排序功能，缺省是按照自然排序进行排列

3. **TreeSet中的元素必须实现Comparable接口并重写compareTo()方法**，TreeSet判断元素是否重复 、以及确定元素的顺序 靠的都是这个方法

4. 基于TreeMap实现


**例子：**



```java
public class Test {

    public static void main(String[] agrs) {
        /**
         * TreeSet中不能有重复的元素；
         * TreeSet具有排序功能；
         * TreeSet中的元素必须实现Comparable接口并重写compareTo()方法，TreeSet判断元素是否重复 、以及确定元素的顺序 靠的都是这个方法
         * 如果自定义类，则可以实现Comparable接口，并且实现compareTo，完成自定义去重
         */
        TreeSet<String> set = new TreeSet<String>();

        /* 新增数据 */
        set.add("abc");
        set.add("xyz");
        set.add("bcd");
        set.add("bac");

        TreeSet<String> set2 = new TreeSet<String>();
        set2.add("TTT");
        set2.add("zzz");
        set2.add("zzz");

        /* 新增一个集合 */
        set.addAll(set2);

        Iterator it = set.iterator();
        while (it.hasNext()) {
            System.out.println(it.next());
        }

        /* 获取第一个元素 */
        String first = set.first();
        System.out.println("first:" + first);

        /* 获取最后一个元素 */
        String last = set.last();
        System.out.println("last:" + last);

        /* 是否包含某个元素 */
        if (set2.contains("TTT")) {
            System.out.println("contains:true");
        }

        /* 判断是否为空 */
        if (set.isEmpty()) {
            System.out.println("空");
        }

        /* 元素个数 */
        System.out.println("元素个数：" + set.size());

        /* 清空集合 */
        set.clear();

    }
}
```



#### HashSet

HashSet的几个特点：

1. HashSet中不能有重复的元素；

2. HashSet是无序的

3. HashSet也是基于HashMap实现

**例子：**



```java
public class Test {

    public static void main(String[] agrs) {
        /**
         * HashSet中不能有重复的元素；
         * HashSet是无序的
         * HashSet也是基于HashMap实现
         */
        Set<String> set = new HashSet<String>();

        /* 新增数据 */
        set.add("abc");
        set.add("xyz");
        set.add("bcd");
        set.add("bac");

        Set<String> set2 = new HashSet<String>();
        set2.add("TTT");
        set2.add("zzz");
        set2.add("zzz");

        /* 新增一个集合 */
        set.addAll(set2);

        /* 判断是否为空 */
        if (set.isEmpty()) {
            System.out.println("空");
        }

        /* 元素个数 */
        System.out.println("元素个数：" + set.size());

        /* 移除元素 */
        set.remove("zzz");

        Iterator it = set.iterator();
        while (it.hasNext()) {
            System.out.println(it.next());
        }

        /* 是否包含某个元素 */
        if (set2.contains("TTT")) {
            System.out.println("contains:true");
        }

        /* 清空集合 */
        set.clear();

    }
}
```



### Map

Map集合主要有：HashMap，TreeMap




#### HashMap

HashMap特点：

1. HashMap是无序的散列映射表；

2. HashMap通过Hash 算法来决定存储位置

**3. 底层实现是哈希表**


**例子：**



```java
public class Test {

    public static void main(String[] agrs) {
        /**
         * HashMap是无序的散列映射表；
         * HashMap通过Hash 算法来决定存储位置
         */
        HashMap<String, String> map = new HashMap<String, String>();

        /* 填充数据 */
        map.put("username", "initphp");
        map.put("age", "100");

        /* 获取元素个数 */
        System.out.println(map.size());

        /* put all */
        HashMap<String, String> map2 = new HashMap<String, String>();
        map2.put("username2", "initphp2");
        map2.put("age2", "1002");
        map.putAll(map2);

        /* 通过Key遍历HashMap */
        Iterator it = map.keySet().iterator();
        while (it.hasNext()) {
            String key = (String) it.next();
            System.out.println("key:" + key + " value:" + map.get(key));
        }

        /* 是否包含某个key */
        if (map.containsKey("age")) {
            System.out.println("是否包含某个key");
        }

        /* 判断是否为空 */
        if (map.isEmpty()) {
            System.out.println("空");
        }

        /* 删除某个元素 */
        map.remove("age");

        /* 清空Map表 */
        map.clear();

    }
}
```




#### TreeMap

TreeMap的特点：

1. 适用于按自然顺序或自定义顺序遍历键(key)。

**2. 底层是二叉树**

3. 提供compareTo，可以定义排序方法



```java
public class Test {

    public static void main(String[] agrs) {
        /**
         * 1. 适用于按自然顺序或自定义顺序遍历键(key)。
         * 2. 底层是二叉树
         * 3. 提供compareTo，可以定义排序方法
         */
        TreeMap<String, String> map = new TreeMap<String, String>();

        /* 填充数据 */
        map.put("username", "initphp");
        map.put("age", "100");

        /* 获取元素个数 */
        System.out.println(map.size());

        /* put all */
        TreeMap<String, String> map2 = new TreeMap<String, String>();
        map2.put("username2", "initphp2");
        map2.put("age2", "1002");
        map.putAll(map2);

        /* 通过Key遍历HashMap，是有序的 */
        Iterator it = map.keySet().iterator();
        while (it.hasNext()) {
            String key = (String) it.next();
            System.out.println("key:" + key + " value:" + map.get(key));
        }

        /* 是否包含某个key */
        if (map.containsKey("age")) {
            System.out.println("是否包含某个key");
        }

        /* 判断是否为空 */
        if (map.isEmpty()) {
            System.out.println("空");
        }

        String first = map.firstKey();
        String last = map.lastKey();
        System.out.println("first:" + first);
        System.out.println("last" + last);

        /* 删除某个元素 */
        map.remove("age");

        /* 清空Map表 */
        map.clear();

    }
}
```



### Queue

LinkedList就是一个Queue。

常用的Queue有：PriorityQueue、ConcurrentLinkedQueue、ArrayBlockingQueue、LinkedBlockingQueue、PriorityBlockingQueue




#### PriorityQueue

例子：



```java
public class Test {

    public static void main(String[] agrs) {

        /* 生成一个LinkedList */
        PriorityQueue<String> queue = new PriorityQueue<String>();
        queue.add("a");
        queue.add("b");
        queue.add("c");
        queue.add("d");

        /* queue的size */
        int size = queue.size();
        System.out.println(size);

        /* 删除特定元素 */
        queue.remove("b");
        System.out.println("List内容 ：" + queue);

        /* 是否包含某个元素 */
        if (queue.contains("c")) {
            System.out.println("包含");
        }

        /* 组装list */
        PriorityQueue<String> queue2 = new PriorityQueue<String>();
        queue2.add("ddd");
        queue2.add("111");
        queue.addAll(queue2);
        System.out.println("List内容 ：" + queue);

        /* 遍历 */
        Iterator lit = queue.iterator();
        while (lit.hasNext()) {
            System.out.println(lit.next());
        }

        /* 从队列头部弹出一个元素 */
        String string = queue.poll();
        System.out.println("poll:" + string);
        string = queue.poll();
        System.out.println("poll:" + string);

        /* 从尾部头部弹出一个元素 */
        string = queue.peek();
        System.out.println("peek:" + string);

        System.out.println("List内容 ：" + queue);

        /* 清除 */
        queue.clear();
        System.out.println("List内容 ：" + queue);
    }
}
```
















****


****





