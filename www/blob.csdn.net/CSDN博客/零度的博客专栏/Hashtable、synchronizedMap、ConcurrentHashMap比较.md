# Hashtable、synchronizedMap、ConcurrentHashMap比较 - 零度的博客专栏 - CSDN博客
2016年10月11日 17:31:36[零度anngle](https://me.csdn.net/zmx729618)阅读数：1036
       在开始之前，先介绍下Map是什么？javadoc中对Map的解释如下：
       An objectthatmaps keys to values . Amapcannot contain duplicatekeys; each key can map to at most one value.
       This interface takes the place of the Dictionary class, which was atotally abstract class rather than an interface.
       The Map interface provides three collection views, which allow amap's contents to be viewed as a set of keys, collection of values,or set of key-value mappings.
       从上可知，Map用于存储“key-value”元素对，它将一个key映射到一个而且只能是唯一的一个value。
Map可以使用多种实现方式，HashMap的实现采用的是hash表；而TreeMap采用的是红黑树。
**1. Hashtable 和 HashMap**
   这两个类主要有以下几方面的不同：
   Hashtable和HashMap都实现了Map接口，但是Hashtable的实现是基于Dictionary抽象类。
在HashMap中，null可以作为键，这样的键只有一个；可以有一个或多个键所对应的值为null。当get()方法返回null值时，即可以表示HashMap中没有该键，也可以表示该键所对应的值为null。因此，在HashMap中不能由get()方法来判断HashMap中是否存在某个键，而应该用containsKey()方法来判断。而在Hashtable中，无论是key还是value都不能为null。
  这两个类最大的不同在于Hashtable是线程安全的，它的方法是同步了的，可以直接用在多线程环境中。而HashMap则不是线程安全的。在多线程环境中，需要手动实现同步机制。因此，在Collections类中提供了一个方法返回一个同步版本的HashMap用于多线程的环境：
Java代码
- **public****static** <K,V> Map<K,V> synchronizedMap(Map<K,V> m) {  
- **return****new** SynchronizedMap<K,V>(m);  
-  }  
public static <K,V> Map<K,V> synchronizedMap(Map<K,V> m) {
        return new SynchronizedMap<K,V>(m);
 }
该方法返回的是一个SynchronizedMap的实例。SynchronizedMap类是定义在Collections中的一个静态内部类。它实现了Map接口，并对其中的每一个方法实现，通过synchronized关键字进行了同步控制。
**2. 潜在的线程安全问题**
上面提到Collections为HashMap提供了一个并发版本SynchronizedMap。这个版本中的方法都进行了同步，但是这并不等于这个类就一定是线程安全的。在某些时候会出现一些意想不到的结果。
如下面这段代码：
Java代码 
- // shm是SynchronizedMap的一个实例
- **if**(shm.containsKey('key')){  
-         shm.remove(key);  
- }  
// shm是SynchronizedMap的一个实例
if(shm.containsKey('key')){
        shm.remove(key);
}
 这段代码用于从map中删除一个元素之前判断是否存在这个元素。这里的containsKey和reomve方法都是同步的，但是整段代码却不是。考虑这么一个使用场景：线程A执行了containsKey方法返回true，准备执行remove操作；这时另一个线程B开始执行，同样执行了containsKey方法返回true，并接着执行了remove操作；然后线程A接着执行remove操作时发现此时已经没有这个元素了。要保证这段代码按我们的意愿工作，一个办法就是对这段代码进行同步控制，但是这么做付出的代价太大。
在进行迭代时这个问题更改明显。Map集合共提供了三种方式来分别返回键、值、键值对的集合：
Java代码 
- Set<K> keySet()；  
- 
- Collection<V> values()；  
- 
- Set<Map.Entry<K,V>> entrySet()；  
Set<K> keySet()；
Collection<V> values()；
Set<Map.Entry<K,V>> entrySet()；
 在这三个方法的基础上，我们一般通过如下方式访问Map的元素：
Java代码 
- Iterator keys = map.keySet().iterator();  
- 
- **while**(keys.hasNext()){  
-         map.get(keys.next());  
- }  
Iterator keys = map.keySet().iterator();
while(keys.hasNext()){
        map.get(keys.next());
}
在这里，有一个地方需要注意的是：得到的keySet和迭代器都是Map中元素的一个“视图”，而不是“副本”。问题也就出现在这里，当一个线程正在迭代Map中的元素时，另一个线程可能正在修改其中的元素。此时，在迭代元素时就可能会抛出`ConcurrentModificationException异常。为了解决这个问题通常有两种方法，一是直接返回元素的副本，而不是视图。这个可以通过`
集合类的 toArray()方法实现，但是创建副本的方式效率比之前有所降低，特别是在元素很多的情况下；另一种方法就是在迭代的时候锁住整个集合，这样的话效率就更低了。
**3. 更好的选择：ConcurrentHashMap**
##### 效率低下的HashTable容器
HashTable容器使用synchronized来保证线程安全，但在线程竞争激烈的情况下HashTable的效率非常低下。因为当一个线程访问HashTable的同步方法时，其他线程访问HashTable的同步方法时，可能会进入阻塞或轮询状态。如线程1使用put进行添加元素，线程2不但不能使用put方法添加元素，并且也不能使用get方法来获取元素，所以竞争越激烈效率越低。
##### 锁分段技术
HashTable容器在竞争激烈的并发环境下表现出效率低下的原因是所有访问HashTable的线程都必须竞争同一把锁，那假如容器里有多把锁，每一把锁用于锁容器其中一部分数据，那么当多线程访问容器里不同数据段的数据时，线程间就不会存在锁竞争，从而可以有效的提高并发访问效率，这就是ConcurrentHashMap所使用的锁分段技术，首先将数据分成一段一段的存储，然后给每一段数据配一把锁，当一个线程占用锁访问其中一个段数据的时候，其他段的数据也能被其他线程访问。
java5中新增了ConcurrentMap接口和它的一个实现类ConcurrentHashMap。ConcurrentHashMap提供了和Hashtable以及SynchronizedMap中所不同的锁机制。Hashtable中采用的锁机制是一次锁住整个hash表，从而同一时刻只能由一个线程对其进行操作；而ConcurrentHashMap中则是一次锁住一个桶。ConcurrentHashMap默认将hash表分为16个桶，诸如get,put,remove等常用操作只锁当前需要用到的桶。这样，原来只能一个线程进入，现在却能同时有16个写线程执行，并发性能的提升是显而易见的。
上面说到的16个线程指的是写线程，而读操作大部分时候都不需要用到锁。只有在size等操作时才需要锁住整个hash表。
在迭代方面，ConcurrentHashMap使用了一种不同的迭代方式。在这种迭代方式中，当iterator被创建后集合再发生改变就不再是抛出ConcurrentModificationException，取而代之的是在改变时new新的数据从而不影响原有的数据，iterator完成后再将头指针替换为新的数据，这样iterator线程可以使用原来老的数据，而写线程也可以并发的完成改变。
