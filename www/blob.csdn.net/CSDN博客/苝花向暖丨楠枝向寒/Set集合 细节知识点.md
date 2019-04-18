# Set集合 细节知识点 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年01月18日 22:10:00[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：71



|Set 实现类|特点|
|----|----|
|HashSet|内部数据结构是哈希表去重无序 jdk1.2线程不安全,效率高|
|TreeSet|内部数据结构红黑二叉树 无序      ____自然排序(针对实现了Comparable接口的引用类型)     （jdk中的包装类和String都实现了Comparable）     红黑二叉树排序     ____自定义排序(针对于实现了Comparator比较器的引用类型） 去重 不能添加null 线程不安全,效率高|
|LinkedHashSet|内部数据结构是链表去重有序 jdk1.4线程不安全,效率高|
去重需要在自定义类中重写hashcode和equals方法，因为hashset的add()内部调用的是 hashmap的 put()方法，而put()方法是判断 两个对象的hashcode 是否一致，如一致，再 用equals 判断 内容是否一致。  
哈希表  --- 数组和链表的结合，查到，增删快。

链表  --  查找慢、增删快         （链表每一个数据单元，分为两部分，前部分存储数据、后部分存储下一单元的地址，因为没有下标，所以查找需从头开始找。增删只需在中间生成一个单元，或去除一个单元，然后改变数据单元后半部分所指向的地址）

数组 --   查找快、增删慢  （数组有索引位，所以查找快，但增删需要创建新的数组，因为数组一经创建则大小固定，所以很耽误时间）

对于 TreeSet 内部的数据结构是 红黑二叉树，数据结构决定了插入的数据最终的输出顺序   

-- 分为两类，一类是String 以及 基本类型的包装类，通过查看API 可以发现 他们分别 实现了Comparable 接口 。所以当向集合中插入的元素 为String 或 基本类型包装 类 的时候 可以直接 通过add()方法 插入。

但如果插入的是 自定义类的元素 时 

第一种方法：自定义类必须 实现comparable 接口且重写compareTo(Object o)方法

第二种方法：创建一个类且实现 comparator接口，然后重写public int compare(Object o1, Object o2)方法。

第三种方法：无需单独创建一个类，在创建TreeSet实例时直接调用有参构造方法并重写public int compare(Object o1, Object o2)方法。

否则会在运行时出现错误    // java.lang.ClassCastException:


