# HashMap在高并发下引起的死循环 - z69183787的专栏 - CSDN博客
2018年07月25日 17:58:57[OkidoGreen](https://me.csdn.net/z69183787)阅读数：245
[https://www.cnblogs.com/yjbjingcha/p/6957909.htm](https://www.cnblogs.com/yjbjingcha/p/6957909.html)
[http://www.cnblogs.com/wfq9330/p/9023892.html](http://www.cnblogs.com/wfq9330/p/9023892.html)
HashMap事实上并非线程安全的，在高并发的情况下，是非常可能发生死循环的，由此造成CPU 100%，这是非常可怕的。所以在多线程的情况下，用HashMap是非常不妥当的行为，应採用线程安全类ConcurrentHashMap进行取代。
# HashMap死循环原因
HashMap进行存储时，假设size超过当前最大容量*负载因子时候会发生resize。首先看一下resize原代码
```
void resize(int newCapacity) {
        Entry[] oldTable = table;
        int oldCapacity = oldTable.length;
        if (oldCapacity == MAXIMUM_CAPACITY) {
            threshold = Integer.MAX_VALUE;
            return;
        }
        Entry[] newTable = new Entry[newCapacity];
        transfer(newTable);
        table = newTable;
        threshold = (int)(newCapacity * loadFactor);
    }
```
而这段代码中又调用了transfer()方法，而这种方法实现的机制就是将每一个链表转化到新链表，而且链表中的位置发生反转，而这在多线程情况下是非常easy造成链表回路。从而发生get()死循环。我们看一下他的源码
```
void transfer(Entry[] newTable) {
        Entry[] src = table;
        int newCapacity = newTable.length;
        for (int j = 0; j < src.length; j++) {
            Entry<K,V> e = src[j];
            if (e != null) {
                src[j] = null;
                do {
                    Entry<K,V> next = e.next;
                    int i = indexFor(e.hash, newCapacity);
                    e.next = newTable[i];
                    newTable[i] = e;
                    e = next;
                } while (e != null);
            }
        }
    }
```
# HashMap死循环演示
假如有两个线程P1、P2，以及链表 a=》b=》null
1、P1先运行，运行完"Entry<K,V> next = e.next;"代码后发生堵塞，或者其它情况不再运行下去，此时e=a。next=b
2、而P2已经运行完整段代码，于是当前的新链表newTable[i]为b=》a=》null
3、P1又继续运行"Entry<K,V> next = e.next;"之后的代码，则运行完"e=next;"后，newTable[i]为a《=》b。则造成回路，while(e!=null)一直死循环
# 总结
HashMap并不是线程安全，所以在多线程情况下，应该首先考虑用ConcurrentHashMap。避免悲剧的发生
图示：
今天研读Java并发容器和框架时，看到为什么要使用ConcurrentHashMap时，其中有一个原因是：线程不安全的HashMap, HashMap在并发执行put操作时会引起死循环，是因为多线程会导致HashMap的Entry链表形成环形数据结构，查找时会陷入死循环。纠起原因看了其他的博客，都比较抽象，所以这里以图形的方式展示一下，希望支持！
 （1）当往HashMap中添加元素时，会引起HashMap容器的扩容，原理不再解释，直接附源代码，如下：
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
1 /**  
 2     *  
 3     * 往表中添加元素，如果插入元素之后，表长度不够，便会调用resize方法扩容  
 4     */    
 5    void addEntry(int hash, K key, V value, int bucketIndex) {    
 6 Entry<K,V> e = table[bucketIndex];    
 7        table[bucketIndex] = new Entry<K,V>(hash, key, value, e);    
 8        if (size++ >= threshold)    
 9            resize(2 * table.length);    
10    }    
11     
12    /**  
13     * resize()方法如下，重要的是transfer方法，把旧表中的元素添加到新表中 
14     */    
15    void resize(int newCapacity) {    
16        Entry[] oldTable = table;    
17        int oldCapacity = oldTable.length;    
18        if (oldCapacity == MAXIMUM_CAPACITY) {    
19            threshold = Integer.MAX_VALUE;    
20            return;    
21        }    
22     
23        Entry[] newTable = new Entry[newCapacity];    
24        transfer(newTable);    
25        table = newTable;    
26        threshold = (int)(newCapacity * loadFactor);    
27    }
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
（2）参考上面的代码，便引入到了transfer方法，（引入重点）这就是HashMap并发时，会引起死循环的根本原因所在，下面结合transfer的源代码，说明一下产生死循环的原理，先列transfer代码（这是里JDK7的源偌），如下：
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
1 /** 
 2      * Transfers all entries from current table to newTable. 
 3      */  
 4     void transfer(Entry[] newTable, boolean rehash) {  
 5         int newCapacity = newTable.length;  
 6         for (Entry<K,V> e : table) {  
 7   
 8             while(null != e) {  
 9                 Entry<K,V> next = e.next;            ---------------------(1)  
10                 if (rehash) {  
11                     e.hash = null == e.key ? 0 : hash(e.key);  
12                 }  
13                 int i = indexFor(e.hash, newCapacity);   
14                 e.next = newTable[i];  
15                 newTable[i] = e;  
16                 e = next;  
17             } // while  
18   
19         }  
20     }
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
（3）假设：
```
1 Map<Integer> map = new HashMap<Integer>(2);  // 只能放置两个元素，其中的threshold为1（表中只填充一个元素时），即插入元素为1时就扩容（由addEntry方法中得知）  
2 //放置2个元素 3 和 7，若要再放置元素8（经hash映射后不等于1）时，会引起扩容
```
假设放置结果图如下：     
![](https://images2018.cnblogs.com/blog/1363081/201805/1363081-20180511122032563-398997032.png)
 现在有两个线程A和B，都要执行put操作，即向表中添加元素，即线程A和线程B都会看到上面图的状态快照
执行顺序如下：
               执行一：  线程A执行到transfer函数中（1）处挂起（transfer函数代码中有标注）。此时在线程A的栈中
```
1 e = 3  
2 next = 7
```
 执行二：线程B执行 transfer函数中的while循环，即会把原来的table变成新一table（线程B自己的栈中），再写入到内存中。如下图（假设两个元素在新的hash函数下也会映射到同一个位置）
![](https://images2018.cnblogs.com/blog/1363081/201805/1363081-20180511122154042-1075170253.png)
执行三： 线程A解挂，接着执行（看到的仍是旧表），即从transfer代码（1）处接着执行，当前的 e = 3, next = 7, 上面已经描述。
                             1. 处理元素 3 ， 将 3 放入 线程A自己栈的新table中（新table是处于线程A自己栈中，是线程私有的，不肥线程2的影响），处理3后的图如下：
![](https://images2018.cnblogs.com/blog/1363081/201805/1363081-20180511122223900-1445391254.png)
  2.  线程A再复制元素 7 ，当前 e = 7 ,而next值由于线程 B 修改了它的引用，所以next 为 3 ，处理后的新表如下图
![](https://images2018.cnblogs.com/blog/1363081/201805/1363081-20180511122248844-1039586860.png)
   3. 由于上面取到的next = 3, 接着while循环，即当前处理的结点为3， next就为null ，退出while循环，执行完while循环后，新表中的内容如下图：
![](https://images2018.cnblogs.com/blog/1363081/201805/1363081-20180511122314655-457688730.png)
  4. 当操作完成，执行查找时，会陷入死循环！
