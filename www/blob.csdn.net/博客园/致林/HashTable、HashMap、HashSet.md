
# HashTable、HashMap、HashSet - 致林 - 博客园






# [HashTable、HashMap、HashSet](https://www.cnblogs.com/bincoding/p/5687061.html)
1. HashMap
1)  hashmap的数据结构
Hashmap是一个数组和链表的结合体（在数据结构称“链表散列“），如下图示：
![](https://pic002.cnblogs.com/images/2012/267603/2012011313064510.jpg)
当我们往hashmap中put元素的时候，先根据key的hash值得到这个元素在数组中的位置（即下标），然后就可以把这个元素放到对应的位置中了。如果这个元素所在的位子上已经存放有其他元素了，那么在同一个位子上的元素将以链表的形式存放，新加入的放在链头，最先加入的放在链尾。
2)使用
![复制代码](https://common.cnblogs.com/images/copycode.gif)
Map map = new HashMap();
map.put("Rajib Sarma","100");
map.put("Rajib Sarma","200");//The value "100" is replaced by "200".
map.put("Sazid Ahmed","200");
Iterator iter = map.entrySet().iterator();
while (iter.hasNext()) {
Map.Entry entry = (Map.Entry) iter.next();
Object key = entry.getKey();
Object val = entry.getValue();
}![复制代码](https://common.cnblogs.com/images/copycode.gif)
2. HashTable和HashMap区别
第一，继承不同。
public class Hashtable extends Dictionary implements Map
public class HashMap  extends AbstractMap implements Map
第二
Hashtable 中的方法是同步的，而HashMap中的方法在缺省情况下是非同步的。在多线程并发的环境下，可以直接使用Hashtable，但是要使用HashMap的话就要自己增加同步处理了。
第三
Hashtable中，key和value都不允许出现null值。
在HashMap中，null可以作为键，这样的键只有一个；可以有一个或多个键所对应的值为null。当get()方法返回null值时，即可以表示 HashMap中没有该键，也可以表示该键所对应的值为null。因此，在HashMap中不能由get()方法来判断HashMap中是否存在某个键， 而应该用containsKey()方法来判断。
第四，两个遍历方式的内部实现上不同。
Hashtable、HashMap都使用了 Iterator。而由于历史原因，Hashtable还使用了Enumeration的方式 。
第五
哈希值的使用不同，HashTable直接使用对象的hashCode。而HashMap重新计算hash值。
第六
Hashtable和HashMap它们两个内部实现方式的数组的初始大小和扩容的方式。HashTable中hash数组默认大小是11，增加的方式是 old*2+1。HashMap中hash数组的默认大小是16，而且一定是2的指数。

和HashSet的区别：
HashSet在储存钱会检查是否重复，要先确保对象重写equals()和hashCode()方法，这样才能比较元素是否相等。
HashSet类似Map的<key,value>，其中value = null





