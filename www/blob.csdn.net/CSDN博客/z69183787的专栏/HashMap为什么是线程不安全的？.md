# HashMap为什么是线程不安全的？ - z69183787的专栏 - CSDN博客
2017年01月19日 11:34:16[OkidoGreen](https://me.csdn.net/z69183787)阅读数：666
一直以来只是知道HashMap是线程不安全的，但是到底HashMap为什么线程不安全，多线程并发的时候在什么情况下可能出现问题？
HashMap底层是一个Entry数组，当发生hash冲突的时候，hashmap是采用链表的方式来解决的，在对应的数组位置存放链表的头结点。对链表而言，新加入的节点会从头结点加入。
javadoc中关于hashmap的一段描述如下：
**此实现不是同步的。**如果多个线程同时访问一个哈希映射，而其中至少一个线程从结构上修改了该映射，则它*必须* 保持外部同步。（结构上的修改是指添加或删除一个或多个映射关系的任何操作；仅改变与实例已经包含的键关联的值不是结构上的修改。）这一般通过对自然封装该映射的对象进行同步操作来完成。如果不存在这样的对象，则应该使用 [`Collections.synchronizedMap`](http://www.ostools.net/uploads/apidocs/jdk-zh/java/util/Collections.html#synchronizedMap(java.util.Map)) 方法来“包装”该映射。最好在创建时完成这一操作，以防止对映射进行意外的非同步访问，如下所示：
   Map m = Collections.synchronizedMap(new HashMap(...));
1、
**[java]**[view
 plain](http://blog.csdn.net/mydreamongo/article/details/8960667#)[copy](http://blog.csdn.net/mydreamongo/article/details/8960667#)
- void addEntry(int hash, K key, V value, int bucketIndex) {  
-     Entry<K,V> e = table[bucketIndex];  
-         table[bucketIndex] = new Entry<K,V>(hash, key, value, e);  
- if (size++ >= threshold)  
-             resize(2 * table.length);  
-     }  
在hashmap做put操作的时候会调用到以上的方法。现在假如A线程和B线程同时对同一个数组位置调用addEntry，两个线程会同时得到现在的头结点，然后A写入新的头结点之后，B也写入新的头结点，那B的写入操作就会覆盖A的写入操作造成A的写入操作丢失
2、
**[java]**[view
 plain](http://blog.csdn.net/mydreamongo/article/details/8960667#)[copy](http://blog.csdn.net/mydreamongo/article/details/8960667#)
- final Entry<K,V> removeEntryForKey(Object key) {  
- int hash = (key == null) ? 0 : hash(key.hashCode());  
- int i = indexFor(hash, table.length);  
-         Entry<K,V> prev = table[i];  
-         Entry<K,V> e = prev;  
- 
- while (e != null) {  
-             Entry<K,V> next = e.next;  
-             Object k;  
- if (e.hash == hash &&  
-                 ((k = e.key) == key || (key != null && key.equals(k)))) {  
-                 modCount++;  
-                 size--;  
- if (prev == e)  
-                     table[i] = next;  
- else
-                     prev.next = next;  
-                 e.recordRemoval(this);  
- return e;  
-             }  
-             prev = e;  
-             e = next;  
-         }  
- 
- return e;  
-     }  
删除键值对的代码如上：
当多个线程同时操作同一个数组位置的时候，也都会先取得现在状态下该位置存储的头结点，然后各自去进行计算操作，之后再把结果写会到该数组位置去，其实写回的时候可能其他的线程已经就把这个位置给修改过了，就会覆盖其他线程的修改
3、addEntry中当加入新的键值对后键值对总数量超过门限值的时候会调用一个resize操作，代码如下：
**[java]**[view
 plain](http://blog.csdn.net/mydreamongo/article/details/8960667#)[copy](http://blog.csdn.net/mydreamongo/article/details/8960667#)
- void resize(int newCapacity) {  
-         Entry[] oldTable = table;  
- int oldCapacity = oldTable.length;  
- if (oldCapacity == MAXIMUM_CAPACITY) {  
-             threshold = Integer.MAX_VALUE;  
- return;  
-         }  
- 
-         Entry[] newTable = new Entry[newCapacity];  
-         transfer(newTable);  
-         table = newTable;  
-         threshold = (int)(newCapacity * loadFactor);  
-     }  
这个操作会新生成一个新的容量的数组，然后对原数组的所有键值对重新进行计算和写入新的数组，之后指向新生成的数组。
当多个线程同时检测到总数量超过门限值的时候就会同时调用resize操作，各自生成新的数组并rehash后赋给该map底层的数组table，结果最终只有最后一个线程生成的新数组被赋给table变量，其他线程的均会丢失。而且当某些线程已经完成赋值而其他线程刚开始的时候，就会用已经被赋值的table作为原始数组，这样也会有问题。
