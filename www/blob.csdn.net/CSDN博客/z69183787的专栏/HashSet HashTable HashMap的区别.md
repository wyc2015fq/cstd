# HashSet HashTable HashMap的区别 - z69183787的专栏 - CSDN博客
2015年07月10日 17:57:18[OkidoGreen](https://me.csdn.net/z69183787)阅读数：521
个人分类：[Java-集合学习](https://blog.csdn.net/z69183787/article/category/5638879)

(1)HashSet是set的一个实现类,hashMap是Map的一个实现类,同时hashMap是hashTable的替代品(为什么后面会讲到).
(2)HashSet以对象作为元素,而HashMap以(key-value)的一组对象作为元素,且HashSet拒绝接受重复的对象.HashMap可以看作三个视图：key的Set，value的Collection，Entry的Set。 这里HashSet就是其实就是HashMap的一个视图。
HashSet内部就是使用Hashmap实现的，和Hashmap不同的是它不需要Key和Value两个值。        
往hashset中插入对象其实只不过是内部做了
            public boolean add(Object o) {
                      return map.put(o, PRESENT)==null;
            }
 现在来看hastTable和hashMap的区别:
(1)Hashtable是基于陈旧的Dictionary类的，HashMap是Java 1.2引进的Map接口的一个实现。
(2)这个不同即是最重要的一点:Hashtable中的方法是同步的，而HashMap方法(在缺省情况下)是非同步的。即是说，在多线程应用程序中，不用专门的操作就安全地可以使用Hashtable了；而对于HashMap，则需要额外的同步机制。但HashMap的同步问题可通过Collections的一个静态方法得到解决：
     Map Collections.synchronizedMap(Map m)
这个方法返回一个同步的Map，这个Map封装了底层的HashMap的所有方法，使得底层的HashMap即使是在多线程的环境中也是安全的。
(3)只有HashMap可以让你将空值作为一个表的条目的key或value。HashMap中只有一条记录可以是一个空的key，但任意数量的条目可以是空的value。这就是说，如果在表中没有发现搜索键，或者如果发现了搜索键，但它是一个空的值，那么get()将返回null。如果有必要，用containKey()方法来区别这两种情况。
其它的一些资料:
1.HashTable的方法是同步的，HashMap未经同步，所以在多线程场合要手动同步HashMap这个区别就像Vector和ArrayList一样。（最主要的区别）
    2.HashTable不允许null值(key和value都不可以),HashMap允许null值(key和value都可以，只容许有一个null值的key，可以有多个null值的value)。
    3.HashTable有一个contains(Object value)，功能和containsValue(Object value)功能一样。
    4.HashTable使用Enumeration，HashMap使用Iterator。
以上只是表面的不同，它们的实现也有很大的不同。
    5.HashTable中hash数组默认大小是11，增加的方式是 old*2+1。HashMap中hash数组的默认大小是16，而且一定是2的指数。
    6.哈希值的使用不同，HashTable直接使用对象的hashCode，代码是这样的：
      int hash = key.hashCode();
      int index = (hash & 0x7FFFFFFF) % tab.length;
而HashMap重新计算hash值，而且用与代替求模：
int hash = hash(k);
int i = indexFor(hash, table.length);
static int hash(Object x) {
　　 int h = x.hashCode();
　　 h += ~(h << 9);
　　 h ^= (h >>> 14);
　　 h += (h << 4);
　　 h ^= (h >>> 10);
　　 return h;
}
static int indexFor(int h, int length) {
　　 return h & (length-1);
}
以上只是一些比较突出的区别，当然他们的实现上还是有很多不同的，比如
HashMap对null的操作。
