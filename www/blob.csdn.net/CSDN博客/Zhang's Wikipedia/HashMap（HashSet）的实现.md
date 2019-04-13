
# HashMap（HashSet）的实现 - Zhang's Wikipedia - CSDN博客


2018年02月02日 23:54:18[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：437



## 0. HashMap（TreeMAP）、HashSet、HashTable 的关系
HashMap 的底层则维护着Node<K, V>[] table;一个一维数组用于快速访问（只在初次使用时进行初始化，当需要扩容时，When allocated, length is always a power of two.）
staticclassNode<K,V>implementsMap.Entry<K,V> {finalinthash;// 此处的 hash 相当于 bucketfinalK key;
        V value;
        Node<K,V> next;// 每个节点均链接着一个链表；在调用 get 方法返回该 key 对应的 value 时，先根据 key 对应的 hash 找到 bucket
if( (tab = table) !=null&& 
        (n = tab.length) >0&& 
        (first = tab[(n -1) & hash]) !=null) 
{if(first.hash == hash &&// always check first node((k = first.key) == key || (key !=null&& key.equals(k)))) {returnfirst;// 找到直接返回}// 否则遍历其链表if( (e = first.next) !=null) {do{
            ....
        }while((e = e.next) !=null);
    } 
}
HashMap 在解决冲突时，采用的是冲突链表（Separate chaining with linked lists）的方式；
HashSet 与 HashMap 有着相同的实现，HashSet 底层维护着一个 HashMap 对象，通过适配器模式是对 HashMap 的进一步封装（限制）
HashMap实现了Map接口，允许放入null元素，除该类未实现同步外，其余跟Hashtable大致相同
与 TreeMap 相比，TreeMap 是有序的；
## 1. HashMap 的实现
有两个参数可以影响HashMap的性能：初始容量（inital capacity）和负载系数（load factor，也叫装填因子）。初始容量指定了初始table的大小，负载系数用来指定自动扩容的临界值。
当entry的数量（size）超过capacity*load_factor时，容器将自动扩容并重新哈希。
hashCode() 与 equals() 方法：
hashCode()方法决定了对象会被放到哪个bucket里，当多个对象的哈希值冲突时，equals()方法决定了这些对象是否是“同一个对象”
2. 散列（hash）的一些细节
如果 hashCode 是负数会怎样？负索引可不是你想要的。因此，一个改进的哈希公式会移出符号位（符号为同 0 相与），然后再用取模（即 %）运算符计算剩余部分。
(123&0x7FFFFFFF)% 20 = 3(456&0x7FFFFFFF)% 20 = 16references
[Java HashSet和HashMap源码剖析](http://www.importnew.com/19892.html)

