# 转载和积累系列 - hashmap 详细实现 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年09月27日 10:13:42[initphp](https://me.csdn.net/initphp)阅读数：1043








                
众所周知，HashMap是基于has表实现是的Map。那么，现在，我们首先来分析下什么交hash表。


         1.首先我们来看下哈希表的作用以及它的基本概念


         我们平时查找数据可能会用到折半查找、二叉排序树查找‘或者是B-树查找，在查找数据时进行=、>、<的比较，所以查找的效率会依赖于查找过程中进行的比较次数。


         我们理想的情况是不经过任何比较，一次存取便能得到所查记录。这就要在记录的储存位置和它的关键字之间建立一个确定的对应关系f，使每个关键字和结构中一个惟一的存储位置相对应。这个对应关系我们就称之为哈希函数，根据关键字key和f找到数据的存储位置。对于不同的关键字，可能经过哈希函数的映射后会得到同一个值，即key1!=key2 , f(key1)= f(key2) ，这就得不到惟一的存储位置了。对于这种情况，我们称之为冲突。在一般情况下冲突只能尽可能的少，而不能完全避免。


         对于哈希表来说，还有一个中重要的概念，即哈希表的填充因子：a=表中填入的记录数/哈希表的长度。a标志哈希表的装满程度。直观的看，a越小，发生冲突的可能就越小；反之，a越大，表中已经填入的记录越多，要再填入数据时，发生冲突的可能性就越大，则查找时，给定值需要与之进行比较的关键字的个数也就也多。


         2。哈希表的构造


              对于哈希表来说，它主要由三部分构成：哈希函数、哈希表、冲突处理


             （1）哈希函数的构造方法：


                    1）直接定址法


                          取关键字或关键字的某个线性函数值为哈希地址。即：


                          H（key）=key或H（key）a*key+b


                          其中a和b为常数


                          例如：有一个从1岁到100岁的人口数字统计表，其中，年龄作为关键字，哈希函数取关键字自身，如下表：

![](http://dl.iteye.com/upload/attachment/440825/2f1588a6-9f5e-3245-8951-9fe30c40a3b0.png)




            由于直接定址所得地址集合和关键字集合的大小相同，因此，对于不同的关键字不会发生冲突。但是，实际中使用这种方法的情况很少，因为随着关键字的增多，哈希表会变得很庞大。


                    2）平方去中位法：


                         取关键字平方后的中间几位为哈希地址。取的位数由表长决定。例子：

![](http://dl.iteye.com/upload/attachment/440831/8c59a1d9-d807-3637-9c68-0c98ba2d0acb.png)


                  3）还有折叠法、数字分析法、除留余数法、随机数法等


              （2）处理冲突的方法


                      这里只介绍两种方法：


                       1）开放定址法

![](http://dl.iteye.com/upload/attachment/440839/013da8c0-290b-3d44-9f1c-86d23e1b012b.png)


                          其中i=1，2，3。。。。，k(k<=m-1),H(key)为哈希函数，m为哈希表表长，di为增量序列，可能有下列三种情况：di=1,2,3....,m-1,称线性探测在散列；（2）

![](http://dl.iteye.com/upload/attachment/440843/e501f05e-c846-3321-876d-e4b3720a7578.png)


称二次探测再散列；（3）di=伪随机数序列，称伪随机探测再散列。


                          例如，在长度为11的哈希表中已填有关键字分别为17，60，29的记录，（哈希函数H(key)=key MOD 11），现在有第四个记录，其关键字为38，由哈希函数得到哈希地址为5，产生冲突。若用线性探测再散列的方法处理，得到下个地址是6，仍冲突，再求下个地址7，仍冲突，直到哈希地址为8的 位置为“空”时止，处理冲突的过程结束，记录填入哈希表中序号为8的位置。若用二次探测再散列，则应该填入序号为4的位置。类似的可以得到伪随机再散列的地址。如下图

![](http://dl.iteye.com/upload/attachment/440969/cd37023d-58cf-3226-9ddb-bb3985de2a78.png)


            （a）插入前（b）线性探测再散列（c）二次探测再散列（d）伪随机探测再散列，伪随机数是9




                    2）链地址法


                         将所有关键字为同义词的记录存储在同一线性表中。假设某哈希函数产生的哈希地址在区间[0,m-1]上，则设立一个指针型向量


                           Chain Chain Hash[m];


其每个分量的初始状态都是空指针。凡是哈希地址为i的记录都插入到头指针为ChainHash[i]的链表中。在列表中的插入位置可以在表头或表尾；也可以在中间，以保持同义词在同一线性表中按关键字有序。


                       例如：已知一组关键字为（19，14，23，01，68，20，84，27，55，11，10，79），则按哈希函数H(key)=key MOD 13 和链地址法处理冲突构造所得的哈希表，如下图所示：

![](http://dl.iteye.com/upload/attachment/440967/c8e5ba7b-f14f-32ed-96ce-4ccc922a9bfb.png)


———————————————————————————————————————————————————————————




现在我们来解释下java是如何实现HashMap的


1.HashMap的哈希函数：






Java代码  [![收藏代码](http://panlianghui-126-com.iteye.com/images/icon_star.png)](http://panlianghui-126-com.iteye.com/blog/968057)

- staticint hash(int h) {  
- // This function ensures that hashCodes that differ only by
- // constant multiples at each bit position have a bounded
- // number of collisions (approximately 8 at default load factor).
-       h ^= (h >>> 20) ^ (h >>> 12);  
- return h ^ (h >>> 7) ^ (h >>> 4);  
-   }  



   其中^是异或运算，>>>是无符号右移运算，则这个哈希函数主要是进行 的移位和异或运算。








Java代码  [![收藏代码](http://panlianghui-126-com.iteye.com/images/icon_star.png)](http://panlianghui-126-com.iteye.com/blog/968057)

- staticint indexFor(int h, int length) {  
- return h & (length-1);  
-   }  



 经过该函数得到哈希地址，其中&是对二进制的与运算




2.HashMap是用链地址法法来处理冲突








Java代码  [![收藏代码](http://panlianghui-126-com.iteye.com/images/icon_star.png)](http://panlianghui-126-com.iteye.com/blog/968057)

- public V put(K key, V value) {  
- if (key == null)  
- return putForNullKey(value);  
- int hash = hash(key.hashCode());  
- int i = indexFor(hash, table.length);  
- for (Entry<K,V> e = table[i]; e != null; e = e.next) {  
-             Object k;  
- if (e.hash == hash && ((k = e.key) == key || key.equals(k))) {  
-                 V oldValue = e.value;  
-                 e.value = value;  
-                 e.recordAccess(this);  
- return oldValue;  
-             }  
-         }  
- 
-         modCount++;  
-         addEntry(hash, key, value, i);  
- returnnull;  
-     }  





   这段代码是把一对映射数据存入HashMap中，如果存入的数据的键值存在，就返回键对应的值；如果不存在，就进行

addEntry(hash, key, value, i);操作，并且返回null，表示该数据还未在HashMap中。


  值得说明下，输入的KEY是一个类型如何变成整形数据呢，这里的关键在： int hash = hash(key.hashCode());

hashCode是超类Object的方法，它能取到对象的内部地址并转换成一个整数。

其中addEntry(hash, key, value, i)为：








Java代码  [![收藏代码](http://panlianghui-126-com.iteye.com/images/icon_star.png)](http://panlianghui-126-com.iteye.com/blog/968057)

- void addEntry(int hash, K key, V value, int bucketIndex) {  
-     Entry<K,V> e = table[bucketIndex];  
-         table[bucketIndex] = new Entry<K,V>(hash, key, value, e);  
- if (size++ >= threshold)  
-             resize(2 * table.length);  
-     }  



 而Entry<K,V>(hash, key, value, e);的代码为：






Java代码  [![收藏代码](http://panlianghui-126-com.iteye.com/images/icon_star.png)](http://panlianghui-126-com.iteye.com/blog/968057)

- staticclass Entry<K,V> implements Map.Entry<K,V> {  
- final K key;  
-        V value;  
-        Entry<K,V> next;  
- finalint hash;  
- 
- /**
-         * Creates new entry.
-         */
-        Entry(int h, K k, V v, Entry<K,V> n) {  
-            value = v;  
-            next = n;  
-            key = k;  
-            hash = h;  
-        }  



   由此我们可以看出，经过哈希函数映射后发生冲突的数据会存在Entry的列表中。


3.HashMap的哈希表






Java代码  [![收藏代码](http://panlianghui-126-com.iteye.com/images/icon_star.png)](http://panlianghui-126-com.iteye.com/blog/968057)

- transient Entry[] table;  



   table就是我们上面提到的初始状态都是空指针的表，它的大小为：






Java代码  [![收藏代码](http://panlianghui-126-com.iteye.com/images/icon_star.png)](http://panlianghui-126-com.iteye.com/blog/968057)

- staticfinalint MAXIMUM_CAPACITY = 1 <<    30;  



4.装载因子






Java代码  [![收藏代码](http://panlianghui-126-com.iteye.com/images/icon_star.png)](http://panlianghui-126-com.iteye.com/blog/968057)

- staticfinalfloat DEFAULT_LOAD_FACTOR = 0.75f;  



   当装入table的数据超出上限（与装载因子有关），则要重构table








Java代码  [![收藏代码](http://panlianghui-126-com.iteye.com/images/icon_star.png)](http://panlianghui-126-com.iteye.com/blog/968057)

- void resize(int newCapacity) {  
-        Entry[] oldTable = table;  
- int oldCapacity = oldTable.length;  
- if (oldCapacity == MAXIMUM_CAPACITY) {  
-            threshold = Integer.MAX_VALUE;  
- return;  
-        }  
- 
-        Entry[] newTable = new Entry[newCapacity];  
-        transfer(newTable);  
-        table = newTable;  
-        threshold = (int)(newCapacity * loadFactor);  
-    }  



    --------------------------------------------------------------------------------------------------------------------


   上述为对hash表的简单介绍以及HashMap的实现过程。




   对hash表介绍参考严蔚敏老师 的《数据结构》



