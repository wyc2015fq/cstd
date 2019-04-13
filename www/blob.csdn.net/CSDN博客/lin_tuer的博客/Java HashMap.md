
# Java HashMap - lin_tuer的博客 - CSDN博客


2018年03月22日 10:50:29[lin_tuer](https://me.csdn.net/lin_tuer)阅读数：64标签：[java base																](https://so.csdn.net/so/search/s.do?q=java base&t=blog)个人分类：[JAVA																](https://blog.csdn.net/lin_tuer/article/category/6407722)


阅读源码是个优雅的习惯~
今天主要看了  HashMap jdk1.8， 因为，，同学面试被问到了，唉，，又到一年春招季。
首先关于 HashMap 我们需要知道以下内容：
HashMap 是线程不安全的，它允许 key, value 的值为 null，在不要求线程安全时，它在效率上比 HashTable 要好一些（这是它与 HashTable 的基本区别）；
在 jdk1.7 的时候，它是 数组 + 链表 实现的，now 1.8中, 它是 数组 + 链表 + RBT 实现的 （好像还有 hash()函数算法简化了一下，table 数组的类型 从 Entry 变成了 Node）
上面说的 table 在逻辑索引上叫做 “桶”(bucket), 它存储链表的第一个元素
key 的 hashcode 方法用来找到 Entry/Node 对象所在的 bucket, 而两个key有相同的hash值的话，他们被放在table数组的同一个桶里面
在 HashMap 中有一个常量，TREEIFY_THRESHOLD, 当链表中元素个数大于这个常量时，就要进行树形优化，就是从链表结构变成 红黑树结构

HashMap 中的重要函数
一、put
public[V](https://docs.oracle.com/javase/8/docs/api/java/util/HashMap.html)put([K](https://docs.oracle.com/javase/8/docs/api/java/util/HashMap.html)key,[V](https://docs.oracle.com/javase/8/docs/api/java/util/HashMap.html)value)Associates the specified value with the specified key in this map. If the map previously contained a mapping for the key, the old value is replaced.
源代码: 来自 https://www.cnblogs.com/jzb-blog/p/6637823.html
```java
public V put(K key, V value) {
        return putVal(hash(key), key, value, false, true); 
}
```
```java
final V putVal(int hash, K key, V value, boolean onlyIfAbsent,
                   boolean evict) {
        Node<K,V>[] tab; Node<K,V> p; int n, i;
        if ((tab = table) == null || (n = tab.length) == 0) // 如果map为空时，调用resize()进行初始化！
            n = (tab = resize()).length;
        if ((p = tab[i = (n - 1) & hash]) == null) // 如果没有在数组中找到对应的节点，则直接插入一个Node (未发生碰撞)
            tab[i] = newNode(hash, key, value, null);
        else {     // 找到了(n - 1) & hash 对应下标的数组（tab）中的节点 ,也就是发生了碰撞
            Node<K,V> e; K k;
            // 1. hash值一样，key值一样，则找到目标Node
            if (p.hash == hash &&
                ((k = p.key) == key || (key != null && key.equals(k))))
            // 2. 数组中找到的这个节点p是TreeNode类型，则需要插入到RBT里面一个节点
            else if (p instanceof TreeNode)
                e = ((TreeNode<K,V>)p).putTreeVal(this, tab, hash, key, value);
            else {
            // 3. 不是TreeNode类型，则表示是一个链表，这里就类似与jdk1.7中的操作
                for (int binCount = 0; ; ++binCount) { // 遍历链表
                    if ((e = p.next) == null) {
                        // 4. 此时查找当前链表的次数已经超过7个，则需要链表RBT化！
                        if (binCount >= TREEIFY_THRESHOLD - 1) // -1 for 1st
                            treeifyBin(tab, hash);
                        break;
                    }
                    if (e.hash == hash &&
                        ((k = e.key) == key || (key != null && key.equals(k)))) // 5. 找到链表中对应的节点
                        break;
                    p = e;
                }
            }
            // 如果e不为空，则表示在HashMap中找到了对应的节点
            if (e != null) { // existing mapping for key
                V oldValue = e.value;
                // 当onlyIfAbsent=false 或者key对应的旧value为空时，用新的value替换旧value
                if (!onlyIfAbsent || oldValue == null)
                    e.value = value;
                afterNodeAccess(e);
                return oldValue;
            }
        }
        ++modCount; // 操作次数+1
        if (++size > threshold) // hashmap节点个数+1，并判断是否超过阈值，如果超过则重建结构！
            resize();
        afterNodeInsertion(evict);
        return null;
    }
```
从上面的源码，我们可以总结出，HashMap 方法的 put 函数的执行流程
![](//img-blog.csdn.net/20180322102707438?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L2xpbl90dWVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

来自：http://blog.csdn.net/crazy1235/article/details/75579654
![](//img-blog.csdn.net/20180322103540887?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L2xpbl90dWVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
二、get
public[V](https://docs.oracle.com/javase/8/docs/api/java/util/HashMap.html)get([Object](https://docs.oracle.com/javase/8/docs/api/java/lang/Object.html)key)Returns the value to which the specified key is mapped, or`null`if this map contains no mapping for the key.

源码：来自 https://www.cnblogs.com/jzb-blog/p/6637823.html
```java
public V get(Object key) {
        Node<K,V> e;
        return (e = getNode(hash(key), key)) == null ? null : e.value;　　　　　　//根据key及其hash值查询node节点，如果存在，则返回该节点的value值。
    }
    final Node<K,V> getNode(int hash, Object key) {　　　　　　　　　　　　　　　　  //根据key搜索节点的方法。记住判断key相等的条件：hash值相同 并且 符合equals方法。
        Node<K,V>[] tab; Node<K,V> first, e; int n; K k;
        if ((tab = table) != null && (n = tab.length) > 0 &&　　　　　　　　　　　　//根据输入的hash值，可以直接计算出对应的下标（n - 1）& hash，缩小查询范围，如果存在结果，则必定在table的这个位置上。
            (first = tab[(n - 1) & hash]) != null) {
            if (first.hash == hash && // always check first node
                ((k = first.key) == key || (key != null && key.equals(k))))　　　 //判断第一个存在的节点的key是否和查询的key相等。如果相等，直接返回该节点。
                return first;
            if ((e = first.next) != null) {　　　　　　　　　　　　　　　　　　　　　　 //遍历该链表/红黑树直到next为null。
                if (first instanceof TreeNode)　　　　　　　　　　　　　　　　　　     //当这个table节点上存储的是红黑树结构时，在根节点first上调用getTreeNode方法，在内部遍历红黑树节点，查看是否有匹配的TreeNode。
                    return ((TreeNode<K,V>)first).getTreeNode(hash, key);
                do {
                    if (e.hash == hash &&　　　　　　　　　　　　　　　　　　　　　　　　//当这个table节点上存储的是链表结构时，用跟第11行同样的方式去判断key是否相同。
                        ((k = e.key) == key || (key != null && key.equals(k))))
                        return e;
                } while ((e = e.next) != null);　　　　　　　　　　　　　　　　　　  　 //如果key不同，一直遍历下去直到链表尽头，e.next == null。
            }
        }
        return null;
    }
```
get 函数相对比较简单。需要注意的是，判断 key 相等的条件是：
hashcode 相等 并且 满足 .equals() 成立
三、entrySet
public[Set](https://docs.oracle.com/javase/8/docs/api/java/util/Set.html)<[Map.Entry](https://docs.oracle.com/javase/8/docs/api/java/util/Map.Entry.html)<[K](https://docs.oracle.com/javase/8/docs/api/java/util/HashMap.html),[V](https://docs.oracle.com/javase/8/docs/api/java/util/HashMap.html)>> entrySet()Returns a`Set`view of the mappings contained in this map. The set is backed by the map, so changes to the map are reflected in the set, and vice-versa. If the map is modified while an iteration over the set is in progress (except through the iterator's ownremoveoperation, or through thesetValueoperation on a map entry returned by the iterator) the results of the iteration are undefined. The set supports element removal, which removes the corresponding mapping from the map, via theIterator.remove,Set.remove,removeAll,retainAllandclearoperations. It does not support theaddoraddAlloperations.

entrySet 存在的作用主要是 为了遍历 hashmap, 因为 map 没有 iterator
Example: 来自：https://www.cnblogs.com/qducn/p/6230905.html
```java
package map;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map.Entry;
import java.util.Set;
/**
 * @author Administrator
 *注意：HashMap不支持Iterator所以要通过其他方式迭代Map中的Key和Value
 */
public class EntrySetDemo {
    public static void main(String[] args) {
        HashMap<String,String> hs=new HashMap<String,String>();
        //添加元素
        hs.put("key1", "value1");
        hs.put("key2", "value2");
        hs.put("key3", "value3");
        hs.put("key4", "value4");
        hs.put("key5", "value5");
        /**
         * 获取hs返回的Set视图
         * 非常注意：返回此映射中包含的映射关系的Set视图，对此Set视图的操作会反映到原来的HashMap
         */
        Set<Entry<String,String>> set=hs.entrySet();
        /**
         * 对Set进行迭代，返回的Entry包含key和value，格式为：key=value
         */
        Iterator<Entry<String,String>> iter=set.iterator();
        //这种做法输出的格式为key=value
//        while(iter.hasNext()){
//            System.out.println(iter.next());
//        }
        
        //对entry进行迭代
        while(iter.hasNext()){
            Entry<String,String> entry=iter.next();//得到一个entry对象
            entry.setValue("123");//将当前迭代的Entry的value替换为123
            String key=entry.getKey();//获取当前迭代的entry对象（返回的Set视图中的Entry）的key
            String value=entry.getValue();//获取当前迭代Entry对象的value
            System.out.println(key+"="+value);
            entry.setValue("1");//在此替换当前迭代对象的Value，将映射到HashMap<String,String>
        }
        System.out.println("===========================");
        //对Set视图的编辑会反应到Entry
        for(Entry<String,String> entry:set){
            //每个Entry就是map中的一个key及其它对应的value被重新封装的对象
            for(String key:hs.keySet()){
                System.out.println(key+"="+hs.get(key));
            }
        }
    }
    
}
```
这里，比较重要的是 对返回的 set 视图的操作会反应到原 map 对象。

四、clone
public[Object](https://docs.oracle.com/javase/8/docs/api/java/lang/Object.html)clone()Returns a shallow copy of thisHashMapinstance: the keys and values themselves are not cloned.

hashmap 的 clone 是浅拷贝。
浅拷贝指的是，对一个对象执行 clone 操作，新对象会开辟一块新的存储空间，其中的基本数据类型以及 String 类型也都会开辟新的存储空间，但是，对于引用性变量，这个引用类型的属性还是会指向旧对象的引用性属性指向的内存空间。新旧对象对该属性的操作会相互影响。
但是，hashmap 的 浅 copy 有点不一样，
源码：http://blog.csdn.net/wangbiao007/article/details/52625099
```java
/** 
    * Returns a shallow copy of this <tt>HashMap</tt> instance: the keys and 
    * values themselves are not cloned. 
    *key和value不被复制 
    * @return a shallow copy of this map 
    */  
   public Object clone() {  
       HashMap<K,V> result = null;  
       try {  
           result = (HashMap<K,V>)super.clone();  
       } catch (CloneNotSupportedException e) {  
           // assert false;  
       }  
    //新申明了一个数组，这说明两个hashMap的数组不是指向的通一个对象  
       result.table = new Entry[table.length];  
       result.entrySet = null;  
       result.modCount = 0;  
       result.size = 0;  
       result.init();  
    //将原HashMap中存储的元素复制到新的HashMap里面  
       result.putAllForCreate(this);  
  
       return result;  
   }  
  
   //遍历原HashMap  
private void putAllForCreate(Map<? extends K, ? extends V> m) {  
       for (Map.Entry<? extends K, ? extends V> e : m.entrySet())  
           putForCreate(e.getKey(), e.getValue());  
   }  
  
   //将key-value放入新的数组中  
private void putForCreate(K key, V value) {  
       int hash = null == key ? 0 : hash(key);  
       int i = indexFor(hash, table.length);  
  
       /** 
        * Look for preexisting entry for key.  This will never happen for 
        * clone or deserialize.  It will only happen for construction if the 
        * input Map is a sorted map whose ordering is inconsistent w/ equals. 
        */  
       for (Entry<K,V> e = table[i]; e != null; e = e.next) {  
           Object k;  
           if (e.hash == hash &&  
               ((k = e.key) == key || (key != null && key.equals(k)))) {  
               e.value = value;  
               return;  
           }  
       }  
  
       createEntry(hash, key, value, i);  
   }  
  
  
 void createEntry(int hash, K key, V value, int bucketIndex) {  
       Entry<K,V> e = table[bucketIndex];  
    //这里如果value是引用类型，这里就是引用传递  
       table[bucketIndex] = new Entry<>(hash, key, value, e);  
       size++;  
   }
```
从源码中可以看出clone方法虽然生成了新的HashMap对象，新的HashMap中的table数组虽然也是新生成的，但是数组中的元素还是引用以前的HashMap中的元素。这就导致在对HashMap中的元素进行修改的时候，即对数组中元素进行修改，会导致原对象和clone对象都发生改变，但进行新增或删除就不会影响对方，因为这相当于是对数组做出的改变，clone对象新生成了一个数组。

=====================================================================
影响 HashMap 性能的参数
An instance ofHashMaphas two parameters that affect its performance:initial capacityandload factor.Thecapacityis the number of buckets in the hash table, and the initial capacity is simply the capacity at the time the hash table is created. Theload factoris a measure of how full the hash table is allowed to get before its capacity is automatically increased.When the number of entries in the hash table exceeds the product of the load factor and the current capacity, the hash table isrehashed(that is, internal data structures are rebuilt) so that the hash table has approximately twice the number of buckets.

As a general rule, the default load factor (.75) offers a good tradeoff between time and space costs. Higher values decrease the space overhead but increase the lookup cost (reflected in most of the operations of theHashMapclass, includinggetandput). The expected number of entries in the map and its load factor should be taken into account when setting its initial capacity, so as to minimize the number of rehash operations. If the initial capacity is greater than the maximum number of entries divided by the load factor, no rehash operations will ever occur.
对 HashMap 有影响的参数主要用 initial capacity 和 load factor 两个。
其中 initial capacity 值的是 bucket 的个数， load factor 指的是 衡量一个 bucket 是否满了的度量.
当 NumOfEntries >= initial capacity * load factor 的时候，hashmap 会自动扩容。


暂时总结这么多，有新的再补充，还是很多要看。。。

