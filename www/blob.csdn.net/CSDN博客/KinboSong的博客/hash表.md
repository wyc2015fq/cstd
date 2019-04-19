# hash表 - KinboSong的博客 - CSDN博客
2017年09月24日 22:49:51[songjinbo3](https://me.csdn.net/KinboSong)阅读数：152
个人分类：[数据结构与算法](https://blog.csdn.net/KinboSong/article/category/6791889)

# 哈希、HashMap原理及源码、Hash的一些应用面试题
原创
2015年08月14日 23:20:01
- 3249
- 2
- 1
```
<li class="edit" style="display: none;">
                    <a class="btn-noborder" href="http://write.blog.csdn.net/postedit/47670047">
                        <i class="icon iconfont icon-bianji"></i><span class="txt">编辑</span>
                    </a>
                </li>
                <li class="del" style="display: none;">
                    <a class="btn-noborder" onclick="javascript:deleteArticle(fileName);return false;">
                        <i class="icon iconfont icon-shanchu"></i><span class="txt">删除</span>
                    </a>
                </li>
            </ul>
        </div>
        <div id="article_content" class="article_content csdn-tracking-statistics" data-mod="popu_307" data-dsm="post" style="overflow: hidden;">
```
**一、哈希定义**
    Hash，一般翻译做“散列”，也有直接音译为”哈希”的，就是把任意长度的输入（又叫做预映射， pre-image），通过散列算法，变换成固定长度的输出，该输出就是散列值。这种转换是一种压缩映射，也就是，散列值的空间通常远小于输入的空间，不 同的输入可能会散列成相同的输出，而不可能从散列值来唯一的确定输入值。
    数学表述为：h = H(M) ，其中H( )–单向散列函数，M–任意长度明文，h–固定长度散列值。
**二、基本概念**
    1、若结构中存在和关键字K相等的记录，则必定在f(K)的存储位置上。由此，不需比较便可直接取得所查记录。称这个对应关系f为散列函数(Hash function)，按这个思想建立的表为散列表。
    对不同的关键字可能得到同一散列地址，即key1≠key2，而f(key1)=f(key2)，这种现象称冲突。具有相同函数值的关键字对该散列函数来说称做同义词。综上所述，根据散列函数H(key)和处理冲2、突的方法将一组关键字映象到一个有限的连续的地址集（区间）上，并以关键字在地址集中的“象” 作为记录在表中的存储位置，这种表便称为散列表，这一映象过程称为散列造表或散列，所得的存储位置称散列地址。
    3、若对于关键字集合中的任一个关键字，经散列函数映象到地址集合中任何一个地址的概率是相等的，则称此类散列函数为均匀散列函数(Uniform Hash function)，这就是使关键字经过散列函数得到一个“随机的地址”，从而减少冲突。
**三、常用的构造散列函数的方法**
　　散列函数能使对一个数据序列的访问过程更加迅速有效，通过散列函数，数据元素将被更快地定位ǐ
　　1. 直接寻址法：取关键字或关键字的某个线性函数值为散列地址。即H(key)=key或H(key) = a·key + b，其中a和b为常数（这种散列函数叫做自身函数）
　　2. 数字分析法
　　3. 平方取中法
　　4. 折叠法
　　5. 随机数法
　　6. 除留余数法：取关键字被某个不大于散列表表长m的数p除后所得的余数为散列地址。即 H(key) = key MOD p, p<=m。不仅可以对关键字直接取模，也可在折叠、平方取中等运算之后取模。对p的选择很重要，一般取素数或m，若p选的不好，容易产生同义词。
**四、处理冲突的方法**
　　1. 开放寻址法；Hi=(H(key) + di) MOD m, i=1,2,…, k(k<=m-1)，其中H(key)为散列函数，m为散列表长，di为增量序列，可有下列三种取法：
　　1. di=1,2,3,…, m-1，称线性探测再散列；
　　2. di=1^2, (-1)^2, 2^2,(-2)^2, (3)^2, …, ±(k)^2,(k<=m/2)称二次探测再散列;
　　3. di=伪随机数序列，称伪随机探测再散列。 ==
　　2. 再散列法：Hi=RHi(key), i=1,2,…,k RHi均是不同的散列函数，即在同义词产生地址冲突时计算另一个散列函数地址，直到冲突不再发生，这种方法不易产生“聚集”，但增加了计算时间。
　　3. 链地址法(拉链法)
　　4. 建立一个公共溢出区
**五、Hash算法的时间复杂度**
    无冲突的hash table复杂度是O(1)，一般是O(c)，c为哈希关键字冲突时查找的平均长度，最坏情况仍然是O(N)。
**六、HashMap**
1、一个对象当HashMap的key时，必须覆盖hashCode()和equals()方法，hashCode()的返回值尽可能的分散。
    2、当HashMap的entry的数组足够大，key的hash值足够分散时，即是可以实现一个entry数组下标最多只对应了一个entry，此时get方法的时间复杂度可以达到O(1)。
    3、在数组长度和get方法的速度上要达到一个平衡。数组比较长碰撞出现的概率就比较小，所以get方法获取值时就比较快，但浪费了比较多的空间；当数组长度没有冗余时，碰撞出现的概率比较大，虽然节省了空间，但会牺牲get方法的时间。
    4、HashMap有默认的装载因子loadFactor=0.75，默认的entry数组的长度为16。装载因子的意义在于使得entry数组有冗余，默认即允许25%的冗余，当HashMap的数据的个数超过12（16*0.75）时即会对entry数组进行第一次扩容，后面的再次扩容依次类推。
    5、HashMap每次扩容一倍，resize时会将已存在的值从新进行数组下标的计算，这个是比较浪费时间的。在平时使用中，如果能估计出大概的HashMap的容量，可以合理的设置装载因子loadFactor和entry数组初始长度即可以避免resize操作，提高put的效率。
    6、HashMap不是线程安全的，多线程环境下可以使用Hashtable或ConcurrentHashMap。
    7、我们能否让HashMap同步？
         HashMap可以通过下面的语句进行同步：
         Map m = Collections.synchronizeMap(hashMap);
七、HashMap源码分析
    1、map是一种key、value形式的键值对，将hash表和map结合即形成了HashMap。
    2、Java中HashMap的数据是以Entry数组的形式存放的，HashMap通过对key进行hash运算得到一个数组下标，然后将数据存放到Entry数组对应的位置，又因为不同的key进行hash运算可能会得到一个相同的数组下标，为了解决碰撞覆盖冲突，所以Entry本身又是一个链表的结构，即以后不同的key相同数组下标的数据的next会被赋值为已存在Entry链表，新的Entry会替换数组值。
    3、HashMap的存储数据的示例图如下：
    图1：
![](https://img-blog.csdn.net/20150814231720067)
    图2：
![](https://img-blog.csdn.net/20150814231724770)
    4、HashMap 的put方法的源码解析
**[java]**[view plain](#)[copy](#)
[print](#)[?](#)
- public V put(K key, V value) {  
- if (table == EMPTY_TABLE) {  
-             inflateTable(threshold);  
-         }  
- if (key == null)  
- return putForNullKey(value);// HashMap接收key为null的数据
- int hash = hash(key);//对key的hashCode再进行hash运算
- int i = indexFor(hash, table.length);//根据hash值和entry数组的大小计算出新增数据应该存放的数组位置
- for (Entry<K,V> e = table[i]; e != null; e = e.next) {  
- // for循环遍历找到的数组下标的entry，如果hash值和key都相等，则覆盖原来的value值       
-             Object k;  
- if (e.hash == hash && ((k = e.key) == key || key.equals(k))) {  
-                 V oldValue = e.value;  
-                 e.value = value;  
-                 e.recordAccess(this);  
- return oldValue;  
-             }  
-         }  
-         modCount++;  
- //如果上面for循环没有找到相同的hash和key，则增加一个entry
-         addEntry(hash, key, value, i);  
- returnnull;  
-     }  
- 
- void addEntry(int hash, K key, V value, int bucketIndex) {  
- if ((size >= threshold) && (null != table[bucketIndex])) {  
-             resize(2 * table.length); //如果size超过threshold，则扩充table大小。再散列
-             hash = (null != key) ? hash(key) : 0;  
-             bucketIndex = indexFor(hash, table.length);  
-         }  
- 
-         createEntry(hash, key, value, bucketIndex);  
-     }  
- 
- void createEntry(int hash, K key, V value, int bucketIndex) {  
-         Entry<K,V> e = table[bucketIndex];  
- //new 一个新的entry，赋值给当前下标数组
-         table[bucketIndex] = new Entry<>(hash, key, value, e); //参数e, 是Entry.next
-         size++;  
-     }  
- 
-     Entry(int h, K k, V v, Entry<K,V> n) {  
-             value = v;  
- //在HashMap的Entry中有四个变量，key、value、hash、next，其中next用于在hash方法添加值冲突时候，所指向的下一个值。
-             next = n;//即将原来数组下标对应的entry赋值给新的entry的next
-             key = k;  
-             hash = h;  
-         }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
public V put(K key, V value) {
        if (table == EMPTY_TABLE) {
            inflateTable(threshold);
        }
        if (key == null)
            return putForNullKey(value);// HashMap接收key为null的数据
        int hash = hash(key);//对key的hashCode再进行hash运算
        int i = indexFor(hash, table.length);//根据hash值和entry数组的大小计算出新增数据应该存放的数组位置
        for (Entry<K,V> e = table[i]; e != null; e = e.next) {
            // for循环遍历找到的数组下标的entry，如果hash值和key都相等，则覆盖原来的value值       
            Object k;
            if (e.hash == hash && ((k = e.key) == key || key.equals(k))) {
                V oldValue = e.value;
                e.value = value;
                e.recordAccess(this);
                return oldValue;
            }
        }
        modCount++;
        //如果上面for循环没有找到相同的hash和key，则增加一个entry
        addEntry(hash, key, value, i);
        return null;
    }
    void addEntry(int hash, K key, V value, int bucketIndex) {
        if ((size >= threshold) && (null != table[bucketIndex])) {
            resize(2 * table.length); //如果size超过threshold，则扩充table大小。再散列
            hash = (null != key) ? hash(key) : 0;
            bucketIndex = indexFor(hash, table.length);
        }
        createEntry(hash, key, value, bucketIndex);
    }
    void createEntry(int hash, K key, V value, int bucketIndex) {
        Entry<K,V> e = table[bucketIndex];
        //new 一个新的entry，赋值给当前下标数组
        table[bucketIndex] = new Entry<>(hash, key, value, e); //参数e, 是Entry.next
        size++;
    }
    Entry(int h, K k, V v, Entry<K,V> n) {
            value = v;
            //在HashMap的Entry中有四个变量，key、value、hash、next，其中next用于在hash方法添加值冲突时候，所指向的下一个值。
            next = n;//即将原来数组下标对应的entry赋值给新的entry的next
            key = k;
            hash = h;
        }
```
（1）对key的hashcode进行hash计算，获取应该保存到数组中的index。
    （2）判断index所指向的数组元素是否为空，如果为空则直接插入。
    （3）如果不为空，则依次查找entry中next所指定的元素，判读key是否相等，如果相等，则替换久的值，返回。
    （4）如果都不相等，则将此链表头元素赋值给待插入entry的next变量，让后将待插入元素插入到entry数组中去。
    （5）当然HashMap里面也包含一些优化方面的实现，这里也说一下。比如：Entry[]的长度一定后，随着map里面数据的越来越长，这样同一个index的链就会很长，会不会影响性能？HashMap里面设置一个因子，随着map的size越来越大，Entry[]会以一定的规则加长长度。
PS: HashMap，也是先判断hashcode（hashcode相同，所以它们的table位置相同，‘碰撞’会发生。因为HashMap使用链表存储对象，这个Entry(包含有键值对的Map.Entry对象)会存储在链表中。），再判断equals，如果都相同，则表示：在集合添加中，认为是同一个”东西”，覆盖旧值。【[==、equals、hashcode的区别和联系](http://blog.csdn.net/paincupid/article/details/47699735)】
 5、HashMap的get方法源码解析 
**[java]**[view plain](#)[copy](#)
[print](#)[?](#)
- public V get(Object key) {  
- if (key == null)//key为null时特别处理
- return getForNullKey();  
-         Entry<K,V> entry = getEntry(key);  
- 
- returnnull == entry ? null : entry.getValue();  
-     }  
- 
- final Entry<K,V> getEntry(Object key) {  
- if (size == 0) {  
- returnnull;  
-         }  
- 
- int hash = (key == null) ? 0 : hash(key);  
- //indexFor(hash, table.length) 根据hash值和数组长度计算出下标，然后遍历Entry链表
- for (Entry<K,V> e = table[indexFor(hash, table.length)];  
-              e != null;  
-              e = e.next) {  
-             Object k;  
- if (e.hash == hash &&  
-                 ((k = e.key) == key || (key != null && key.equals(k))))  
- return e;  
-         }  
- returnnull;  
-     }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
public V get(Object key) { 
        if (key == null)//key为null时特别处理 
            return getForNullKey(); 
        Entry<K,V> entry = getEntry(key);
```
```
return null == entry ? null : entry.getValue();
}
final Entry<K,V> getEntry(Object key) {
    if (size == 0) {
        return null;
    }
    int hash = (key == null) ? 0 : hash(key);
    //indexFor(hash, table.length) 根据hash值和数组长度计算出下标，然后遍历Entry链表
    for (Entry<K,V> e = table[indexFor(hash, table.length)];
         e != null;
         e = e.next) {
        Object k;
        if (e.hash == hash &&
            ((k = e.key) == key || (key != null && key.equals(k))))
            return e;
    }
    return null;
}</pre><br>
```
6、确定数组index：hashcode % table.length取模
    按位取并，作用上相当于取模mod或者取余%。这意味着数组下标相同，并不表示hashCode相同。
**[java]**[view plain](#)[copy](#)
[print](#)[?](#)
- staticint indexFor(int h, int length) {  
- return h & (length-1);  
-     }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
static int indexFor(int h, int length) { 
        return h & (length-1); 
    }
```
   7、解决hash冲突的办法
    开放定址法（线性探测再散列，二次探测再散列，伪随机探测再散列）
    再哈希法
链地址法
    建立一个公共溢出区
    Java中hashmap的解决办法就是采用的链地址法。
    8、再散列rehash过程
    当哈希表的容量超过默认容量时，必须调整table的大小。当容量已经达到最大可能值时，那么该方法就将容量调整到Integer.MAX_VALUE返回，这时，需要创建一张新表，将原表的映射到新表中。
**[java]**[view plain](#)[copy](#)
[print](#)[?](#)
- void resize(int newCapacity) {  
-         Entry[] oldTable = table;  
- int oldCapacity = oldTable.length;  
- if (oldCapacity == MAXIMUM_CAPACITY) {  
-             threshold = Integer.MAX_VALUE;  
- return;  
-         }  
- 
-         Entry[] newTable = new Entry[newCapacity];  
-         transfer(newTable, initHashSeedAsNeeded(newCapacity));  
-         table = newTable;  
-         threshold = (int)Math.min(newCapacity * loadFactor, MAXIMUM_CAPACITY + 1);  
-     }  
- 
- void transfer(Entry[] newTable, boolean rehash) {  
- int newCapacity = newTable.length;  
- for (Entry<K,V> e : table) {  
- while(null != e) {  
-                 Entry<K,V> next = e.next;  
- if (rehash) {  
-                     e.hash = null == e.key ? 0 : hash(e.key);  
-                 }  
- int i = indexFor(e.hash, newCapacity);  
-                 e.next = newTable[i];  
-                 newTable[i] = e;  
-                 e = next;  
-             }  
-         }  
-     }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
void resize(int newCapacity) { 
        Entry[] oldTable = table; 
        int oldCapacity = oldTable.length; 
        if (oldCapacity == MAXIMUM_CAPACITY) { 
            threshold = Integer.MAX_VALUE; 
            return; 
        }
```
```
Entry[] newTable = new Entry[newCapacity];
    transfer(newTable, initHashSeedAsNeeded(newCapacity));
    table = newTable;
    threshold = (int)Math.min(newCapacity * loadFactor, MAXIMUM_CAPACITY + 1);
}
void transfer(Entry[] newTable, boolean rehash) {
    int newCapacity = newTable.length;
    for (Entry<K,V> e : table) {
        while(null != e) {
            Entry<K,V> next = e.next;
            if (rehash) {
                e.hash = null == e.key ? 0 : hash(e.key);
            }
            int i = indexFor(e.hash, newCapacity);
            e.next = newTable[i];
            newTable[i] = e;
            e = next;
        }
    }
}</pre><br>
```
**八、Hash应用例子**
搜索引擎会通过日志文件把用户每次检索使用的所有检索串都记录下来，每个查询串的长度为1-255字节。假设目前有一千万个记录（这些查询串的重复度比较高，虽然总数是1千万，但如果除去重复后，不超过3百万个。一个查询串的重复度越高，说明查询它的用户越多，也就是越热门。），请你统计最热门的10个查询串，要求使用的内存不能超过1G。
第一步：Query统计
    1、直接排序法
首先我们最先想到的的算法就是排序了，首先对这个日志里面的所有Query都进行排序，然后再遍历排好序的Query，统计每个Query出现的次数了。 
　　但是题目中有明确要求，那就是内存不能超过1G，一千万条记录，每条记录是255Byte，很显然要占据2.375G内存，这个条件就不满足要求了。
 　　让我们回忆一下数据结构课程上的内容，当数据量比较大而且内存无法装下的时候，我们可以采用外排序的方法来进行排序，这里我们可以采用归并排序，因为归并排序有一个比较好的时间复杂度O(nlogn)。
　　排完序之后我们再对已经有序的Query文件进行遍历，统计每个Query出现的次数，再次写入文件中。 
　　综合分析一下，排序的时间复杂度是O(nlogn)，而遍历的时间复杂度是O(n)，因此该算法的总体时间复杂度就是O(n+nlogn)=O(nlogn)。
 　　2、Hash Table法 
　　在第1个方法中，我们采用了排序的办法来统计每个Query出现的次数，时间复杂度是O(nlogn)，那么能不能有更好的方法来存储，而时间复杂度更低呢？
 　　题目中说明了，虽然有一千万个Query，但是由于重复度比较高，因此事实上只有300万的Query，每个Query 255Byte，因此我们可以考虑把他们都放进内存中去，而现在只是需要一个合适的数据结构，在这里，Hash Table绝对是我们优先的选择，因为Hash Table的查询速度非常的快，几乎是O(1)的时间复杂度。
 　　那么，我们的算法就有了：维护一个Key为Query字串，Value为该Query出现次数的HashTable，每次读取一个Query，如果该字串不在Table中，那么加入该字串，并且将Value值设为1；如果该字串在Table中，那么将该字串的计数加一即可。最终我们在O(n)的时间复杂度内完成了对该海量数据的处理。
 　　本方法相比算法1：在时间复杂度上提高了一个数量级，为O(n)，但不仅仅是时间复杂度上的优化，该方法只需要IO数据文件一次，而算法1的IO次数较多的，因此该算法2比算法1在工程上有更好的可操作性。
第二步：找出Top 10
        算法一：普通排序 
    我想对于排序算法大家都已经不陌生了，这里不在赘述，我们要注意的是排序算法的时间复杂度是O(nlogn)，在本题目中，三百万条记录，用1G内存是可以存下的。
　　算法二：部分排序 
　　题目要求是求出Top 10，因此我们没有必要对所有的Query都进行排序，我们只需要维护一个10个大小的数组，初始化放入10个Query，按照每个Query的统计次数由大到小排序，然后遍历这300万条记录，每读一条记录就和数组最后一个Query对比，如果小于这个Query，那么继续遍历，否则，将数组中最后一条数据淘汰，加入当前的Query。最后当所有的数据都遍历完毕之后，那么这个数组中的10个Query便是我们要找的Top10了。
　　不难分析出，这样，算法的最坏时间复杂度是N*K， 其中K是指top多少。
　　算法三：堆 
　　在算法二中，我们已经将时间复杂度由NlogN优化到NK，不得不说这是一个比较大的改进了，可是有没有更好的办法呢？
 　　分析一下，在算法二中，每次比较完成之后，需要的操作复杂度都是K，因为要把元素插入到一个线性表之中，而且采用的是顺序比较。这里我们注意一下，该数组是有序的，一次我们每次查找的时候可以采用二分的方法查找，这样操作的复杂度就降到了logK，可是，随之而来的问题就是数据移动，因为移动数据次数增多了。不过，这个算法还是比算法二有了改进。
 　　基于以上的分析，我们想想，有没有一种既能快速查找，又能快速移动元素的数据结构呢？回答是肯定的，那就是堆。
 　　借助堆结构，我们可以在log量级的时间内查找和调整/移动。因此到这里，我们的算法可以改进为这样，维护一个K(该题目中是10)大小的小根堆，然后遍历300万的Query，分别和根元素进行对比。
　　思想与上述算法二一致，只是算法在算法三，我们采用了最小堆这种数据结构代替数组，把查找目标元素的时间复杂度有O(K)降到了O(logK)。
 　　那么这样，采用堆数据结构，算法三，最终的时间复杂度就降到了N‘logK，和算法二相比，又有了比较大的改进。 
 总结：
 　　至此，算法就完全结束了，经过上述第一步、先用Hash表统计每个Query出现的次数，O(N)；然后第二步、采用堆数据结构找出Top 10，N*O(logK)。所以，我们最终的时间复杂度是：O(N)+N’*O(logK)。（N为1000万，N’为300万）。如果各位有什么更好的算法，欢迎留言评论。
**九、参考：**
[](http://www.cnblogs.com/wangjy/archive/2011/09/08/2171638.html)[http://www.cnblogs.com/wangjy/archive/2011/09/08/2171638.html](http://www.cnblogs.com/wangjy/archive/2011/09/08/2171638.html)
Java集合类之HashMap源码分析:    [](http://www.open-open.com/lib/view/open1363245226500.html#)[http://www.open-open.com/lib/view/open1363245226500.html#](http://www.open-open.com/lib/view/open1363245226500.html#)
[](http://kb.cnblogs.com/page/189480/)[http://kb.cnblogs.com/page/189480/](http://kb.cnblogs.com/page/189480/)
[http://blog.csdn.net/paincupid/article/details/47699735](http://blog.csdn.net/paincupid/article/details/47699735)
**十、转载请注明：**[](http://blog.csdn.net/paincupid/article/details/47670047)[http://blog.csdn.net/paincupid/article/details/47670047](http://blog.csdn.net/paincupid/article/details/47670047)
