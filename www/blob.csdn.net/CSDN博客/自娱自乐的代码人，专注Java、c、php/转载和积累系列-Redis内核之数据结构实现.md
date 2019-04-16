# 转载和积累系列 - Redis内核之数据结构实现 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2015年05月21日 13:49:59[initphp](https://me.csdn.net/initphp)阅读数：814








## redis基础数据结构

**简单动态字符串（SDS）**

Redis 没有直接使用 C 语言传统的字符串表示，而是自己构建了一种名为简单动态字符串（simple dynamic string，SDS）的抽象类型， 并将 SDS 用作 Redis 的默认字符串表示。

**定义：**

![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAdutiaG7ugkiaQJkH8C4mnL7waicEr2zy8cxEOoqWdSKbP7zBnjwkuTQH4Eg/0?wx_fmt=png)

**SDS与C字符串的区别**

1、 常数复杂度获取字符串长度（C 字符串不记录自身的长度信息，要获取其长度需遍历整个字符串，复杂度O(N)；SDS的len属性记录了长度，故复杂度为O(1)）。

2、 杜绝缓冲区溢出（C 字符串不记录自身长度容易造成缓冲区溢出；SDS API对SDS修改时，会先检查空间是否满足需求，不满足，先进行扩展，再执行实际修改，避免了缓冲区溢出问题）。

3、 减少修改字符串长度时所需的内存重分配次数（C字符串长度变化时每次需要重新分配内存；SDS通过空间预分配和惰性空间释放两种策略，减少了内存重新分配的次数）。

4、 二进制安全（SDS不像C字符串局限于保持文本数据，也可以保持二进制数据，是二进制安全的）。

5、 兼容部分 C 字符串函数（SDS 可以直接重用一部分 C 字符串函数库里面的函数）。****

**链表**

作为一种常用数据结构，链表内置在很多高级的编程语言里面，因为Redis使用的C语言并没有内置这种数据结构，所以 Redis 构建了自己的链表实现。

**链表节点(**adlist.h/listNode**)结构**

![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAdu6sGUDw9c08NXLJnCqelTSnhSNbbW23ytBPjqLjQSfKy8wqmJFibf9AQ/0?wx_fmt=png)
****
**链表（****adlist.h/list****）结构**

![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAduibmC7wticLzMBpcJW772WKib5qngJMmgJV1oibysKayhek0rAVqQmjI02Q/0?wx_fmt=png)********

list结构为链表提供了表头指针head、表尾指针tail，以及链表长度计数器len，而dup、 free和match成员则是用于实现多态链表所需的类型特定函数：

· dup 函数用于复制链表节点所保存的值；

· free 函数用于释放链表节点所保存的值；

· match 函数则用于对比链表节点所保存的值和另一个输入值是否相等

![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAduPnLtGibYdolyIIR8Tzg7BVHy4icgibiboCzunaaZ2vVEYxFfFPDnD0AXVQ/0?wx_fmt=jpeg)






Redis链表实现的特性如下：

双端：链表节点带有prev和next指针，获取某个节点的前置节点和后置节点的复杂度都是 O(1)。

无环：表头节点的prev指针和表尾节点的next指针都指向NULL，对链表的访问以NULL为终点。

带表头指针和表尾指针：通过list结构的head指针和tail指针，程序获取链表的表头节点和表尾节点的复杂度为O(1)。

带链表长度计数器：程序使用list结构的len属性来对list持有的链表节点进行计数，程序获取链表中节点数量的复杂度为O(1)。

多态：链表节点使用void*指针来保存节点值，并且可以通过list结构的dup、free、match三个属性为节点值设置类型特定函数，所以链表可以用于保存各种不同类型的值。

**字典**

字典，又称符号表（symbol table）、关联数组（associative array）或者映射（map），是一种用于保存键值对（key-value pair）的抽象数据结构。字典中的每个键都是独一无二的， 程序可以在字典中根据键查找与之关联的值， 或者通过键来更新值， 又或者根据键来删除整个键值对等。

字典在Redis中的应用相当广泛，比如Redis的数据库就是使用字典来作为底层实现的，对数据库的增、删、改、查也是构建在对字典的操作之上的。

**字典的实现**

Redis 的字典使用哈希表作为底层实现，一个哈希表里面可以有多个哈希表节点，而每个哈希表节点就保存了字典中的一个键值对。

**哈希表（**dict.h/dictht**）**

![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAduF64ux5pZ5ts5AibMZJJMk3XoBuPQZMy9TicfTKVrceXSkEKPNvVFlq6A/0?wx_fmt=png)****


table属性是一个数组，数组中的每个元素都是一个指向dict.h/dictEntry结构的指针，每个 dictEntry结构保存着一个键值对。


size属性记录了哈希表的大小，也即是table数组的大小，而used属性则记录了哈希表目前已有节点（键值对）的数量。


sizemask属性的值总是等于size-1，这个属性和哈希值一起决定一个键应该被放到table数组的哪个索引上面。



![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAduaFUd84xPeEOHQvRbTJ1spicLolQ5WSSk9pzgnx273C3sQRzdTrbhQgQ/0?wx_fmt=jpeg)

**哈希表节点**

![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAduyhQ3bQ6aFjCLic1aoTjlptWIgXrZm3FYicwJegpCn54ibG1LCpdBDoAsg/0?wx_fmt=png)

哈希表节点使用dictEntry结构表示，每个dictEntry结构都保存一个键值对。

![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAdua1AbqzeDu3G1jf47VovQUIGyfwbSwtCTFOKZPWRa5ibsryJjDlFPhQQ/0?wx_fmt=jpeg)

**字典（**dict.h/dict**）**

![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAduwZILjQefibUDeEMJoxXoR9xfyPzgOzWasvXceib1g8ibeLPib7RPAd5ibGg/0?wx_fmt=png)

type属性和privdata属性是针对不同类型的键值对，为创建多态字典而设置的：

type属性是一个指向dictType结构的指针，每个dictType结构保存了一簇用于操作特定类型键值对的函数，Redis会为用途不同的字典设置不同的类型特定函数。

privdata属性保存了需要传给那些类型特定函数的可选参数。

![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAdub6SiatwYNLgPkP8AZ8gkJKG4IWcczSZCjO0U9k2FmAQfwFRBJSgUchw/0?wx_fmt=png)

ht属性是一个包含两个项的数组，数组中的每个项都是一个dictht 哈希表，一般情况下， 字典只使用ht[0]哈希表，ht[1]哈希表只会在对ht[0]哈希表进行 rehash 时使用。

除了ht[1]之外，另一个和rehash 有关的属性就是rehashidx ：它记录了rehash的进度， 如果没有进行rehash，那么它的值为-1。

![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAduYG1txZ0MsUkCakibw12LiarnY1I1Y7kKcQJUV1Jnvpry6xp74ll2qtiag/0?wx_fmt=jpeg)

**哈希算法**

Redis 计算哈希值和索引值的方法如下：

![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAduxcljQ4AIRkYFYy9Xzwh3jNpGeoWqSZUSmDuiaJxn7Uat1pO2UlIypCQ/0?wx_fmt=png)



![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAdulIwicjM9n7mLNVVd0cgk8yYTnRf4ibckbvluJsqV7gH8fPtInmEFDocA/0?wx_fmt=jpeg)![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAduHdIEknDujaS7gCoiaMNC4uZo5jNxnrO67QpgHJQLZiaOGKMKP3G8svaQ/0?wx_fmt=jpeg)



上图示意了，一个键值对k0和v0通过哈希算法得到对应位置后添加到哈希表的过程。

当字典被用作数据库的底层实现，或者哈希键的底层实现时，Redis使用MurmurHash2算法来计算键的哈希值。



**解决键冲突**

当有两个或以上数量的键被分配到了哈希表数组的同一个索引上面时，我们称这些键发生了冲突（collision）。

Redis 的哈希表使用链地址法（separate chaining）来解决键冲突：每个哈希表节点都有一个 next 指针，多个哈希表节点可以用next指针构成一个单向链表，被分配到同一个索引上的

多个节点可以用这个单向链表连接起来，这就解决了键冲突的问题。



![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAduqBicgCiaWgjtIsHvgc9XTBS7q2WibClia5sy7OxckAZluSmHibhzWdibbfnA/0?wx_fmt=jpeg)

因为dictEntry节点组成的链表没有指向链表表尾的指针，所以为了速度考虑，程序总是将新节点添加到链表的表头位置（复杂度为 O(1)），排在其他已有节点的前面。



**rehash**（重新散列）

随着操作的不断执行，哈希表保存的键值对会逐渐地增多或者减少，为了让哈希表的负载因子（load factor）维持在一个合理的范围之内，当哈希表保存的键值对数量太多或者太少时，程序需要对哈希表的大小进行相应的扩展或者收缩。

扩展和收缩哈希表的工作可以通过执行rehash（重新散列）操作来完成，Redis对字典的哈希表执行 rehash 的步骤如下：

1.为字典的ht[1]哈希表分配空间，这个哈希表的空间大小取决于要执行的操作，以及 ht[0]当前包含的键值对数量（也即是 ht[0].used 属性的值）：

如果执行的是扩展操作， 那么 ht[1] 的大小为第一个大于等于 ht[0].used * 2 的 （2 的 n 次方幂）；

如果执行的是收缩操作， 那么 ht[1] 的大小为第一个大于等于 ht[0].used 的 。

2.将保存在ht[0]中的所有键值对rehash到ht[1]上面：rehash指的是重新计算键的哈希值和索引值，然后将键值对放置到ht[1]哈希表的指定位置上。

3.当ht[0]包含的所有键值对都迁移到了ht[1]之后（ht[0] 变为空表），释放ht[0]，将ht[1]设置为 ht[0]，并在ht[1]新创建一个空白哈希表，为下一次 rehash 做准备。

以下四张图展示了rehash的过程：



![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAdu8D5foDibhXiarmGtR3buunwTjYuqItUYq0NEsgYEx0L1xTxhfqrQictSQ/0?wx_fmt=jpeg)



![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAduYjnuy4naZ1jicZ4U3I9BMMBPspWUnMAjh1jRicoxKq5Nic3FlvjkG8qBw/0?wx_fmt=jpeg)

![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAdu8ywua56khMCEKhJaCA1lOvFbc45TBO45icbnApdHFNWFqVOJoorvcZQ/0?wx_fmt=jpeg)



![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAduUxtIJaG2icZYgciaMblp51meUMnAvqln5icQlnZGfRBVu0WssYV3fXItg/0?wx_fmt=jpeg)





**哈希表的扩展与收缩**

当以下条件中的任意一个被满足时，程序会自动开始对哈希表执行扩展操作：

1.服务器目前没有在执行BGSAVE命令或者BGREWRITEAOF命令，并且哈希表的负载因子大于等于1；

2.服务器目前正在执行BGSAVE命令或者BGREWRITEAOF命令，并且哈希表的负载因子大于等于5；

其中哈希表的负载因子可以通过公式计算得出：

![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAduGJzVhHrTFy1ucicyC1JZERb2jBnm9kLDuPSknr0tAqaxK2Pnnl3htnA/0?wx_fmt=png)

根据BGSAVE命令或BGREWRITEAOF命令是否正在执行，服务器执行扩展操作所需的负载因子并不相同，这是因为在执行BGSAVE命令或BGREWRITEAOF命令的过程中，Redis需要创建当前服务器进程的子进程，而大多数操作系统都采用写时复制（copy-on-write）技术来优化子进程的使用效率，所以在子进程存在期间，服务器会提高执行扩展操作所需的负载因子， 从而尽可能地避免在子进程存在期间进行哈希表扩展操作，这可以避免不必要的内存写入操作，最大限度地节约内存。

当哈希表的负载因子小于 0.1 时， 程序自动开始对哈希表执行收缩操作。





**渐进式rehash**

****

为了避免rehash对服务器性能造成影响，服务器不是一次性将ht[0]里面的所有键值对全部 rehash到ht[1] ，而是分多次、渐进式地将ht[0]里面的键值对慢慢地rehash到ht[1] 。



以下是哈希表渐进式 rehash 的详细步骤：

1.为ht[1]分配空间，让字典同时持有ht[0]和ht[1]两个哈希表。

2.在字典中维持一个索引计数器变量rehashidx ，并将它的值设置为0 ，表示rehash工作正式开始。

3.在rehash进行期间，每次对字典执行添加、删除、查找或者更新操作时，程序除了执行指定的操作以外，还会顺带将ht[0]哈希表在rehashidx索引上的所有键值对rehash到ht[1] ， 当rehash工作完成之后，程序将rehashidx属性的值增一。

4.随着字典操作的不断执行，最终在某个时间点上，ht[0]的所有键值对都会被rehash至 ht[1] ，这时程序将rehashidx属性的值设为-1 ，表示rehash操作已完成。

渐进式 rehash 的好处在于它采取分而治之的方式， 将 rehash 键值对所需的计算工作均滩到对字典的每个添加、删除、查找和更新操作上，从而避免了集中式rehash而带来的庞大计算量。



**渐进式****rehash****执行期间的哈希表操作**




因为在进行渐进式 rehash 的过程中， 字典会同时使用 ht[0] 和 ht[1] 两个哈希表， 所以在渐进式 rehash 进行期间， 字典的删除（delete）、查找（find）、更新（update）等操作会在两个哈希表上进行： 比如说， 要在字典里面查找一个键的话， 程序会先在 ht[0] 里面进行查找， 如果没找到的话， 就会继续到 ht[1] 里面进行查找， 诸如此类。

另外， 在渐进式 rehash 执行期间， 新添加到字典的键值对一律会被保存到 ht[1] 里面， 而 ht[0] 则不再进行任何添加操作： 这一措施保证了 ht[0] 包含的键值对数量会只减不增， 并随着 rehash 操作的执行而最终变成空表。



**跳跃表**

跳跃表（skiplist）是一种有序数据结构，它通过在每个节点中维持多个指向其他节点的指针， 从而达到快速访问节点的目的。

跳跃表支持平均O(log N)最坏O(N)复杂度的节点查找，还可以通过顺序性操作来批量处理节点。

在大部分情况下，跳跃表的效率可以和平衡树相媲美，并且因为跳跃表的实现比平衡树要来得更为简单，所以有不少程序都使用跳跃表来代替平衡树。

Redis只在两个地方用到了跳跃表，一个是实现有序集合键，另一个是在集群节点中用作内部数据结构。



**跳跃表的实现**

Redis 的跳跃表由redis.h/zskiplistNod和redis.h/zskiplist两个结构定义，其中zskiplistNode结构用于表示跳跃表节点，而zskiplist结构则用于保存跳跃表节点的相关信息，比如节点的数量，以及指向表头节点和表尾节点的指针，等等。

![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAduh31ptSd88Krq49SxMug0rxvRmrXWPg77BwHUWLl2zDpCTf0QO0h6VQ/0?wx_fmt=jpeg)

展示了一个跳跃表示例，位于图片最左边的是zskiplist结构，该结构包含以下属性：

    header：指向跳跃表的表头节点。

    tail：指向跳跃表的表尾节点。

    level：记录目前跳跃表内，层数最大的那个节点的层数（表头节点的层数不计算在内）。

    length：记录跳跃表的长度，也即是，跳跃表目前包含节点的数量（表头节点不计算在内）。



位于zskiplist结构右方的是四个zskiplistNode结构，该结构包含以下属性：

    层（level）：节点中用 L1 、 L2 、 L3 等字样标记节点的各个层， L1 代表第一层， L2 代表第二层，以此类推。每个层都带有两个属性：前进指针和跨度。前进指针用于访问位于表尾方向的其他节点，而跨度则记录了前进指针所指向节点和当前节点的距离。在上面的图片中，连线上带有数字的箭头就代表前进指针，而那个数字就是跨度。当程序从表头向表尾进行遍历时，访问会沿着层的前进指针进行。

    后退（backward）指针：节点中用 BW 字样标记节点的后退指针，它指向位于当前节点的前一个节点。后退指针在程序从表尾向表头遍历时使用。

    分值（score）：各个节点中的 1.0 、 2.0 和 3.0 是节点所保存的分值。在跳跃表中，节点按各自所保存的分值从小到大排列。

    成员对象（obj）：各个节点中的 o1 、 o2 和 o3 是节点所保存的成员对象。



注意表头节点和其他节点的构造是一样的： 表头节点也有后退指针、分值和成员对象， 不过表头节点的这些属性都不会被用到， 所以图中省略了这些部分， 只显示了表头节点的各个层。



**跳跃表节点（**redis.h/zskiplistNode**）**

****

![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAdudWOGl1eWCIL4caaia1qgBdghWfmeIneOe1LoxlASL4ZBjcMHN7MLYHg/0?wx_fmt=png)



**层**

跳跃表节点的 level 数组可以包含多个元素， 每个元素都包含一个指向其他节点的指针， 程序可以通过这些层来加快访问其他节点的速度， 一般来说， 层的数量越多， 访问其他节点的速度就越快。

每次创建一个新跳跃表节点的时候， 程序都根据幂次定律 （power law，越大的数出现的概率越小） 随机生成一个介于 1 和 32 之间的值作为 level 数组的大小， 这个大小就是层的“高度”。

![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAdu4hicPxVP5EzNeg29Bm0FH6TLOJWvfDdzeJ2iaiafy00IckNfSwuW7DF9Q/0?wx_fmt=jpeg)



上图分别展示了三个高度为 1 层、 3 层和 5 层的节点。

**前进指针**

每个层都有一个指向表尾方向的前进指针（level[i].forward 属性）， 用于从表头向表尾方向访问节点。

![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAduKP6CNXia3HicKo7aImiaics9BFO25fFxUKujfh7LW808u3wpRGScddXmGA/0?wx_fmt=jpeg)



上图用虚线表示出了程序从表头向表尾方向， 遍历跳跃表中所有节点的路径：

1. 迭代程序首先访问跳跃表的第一个节点（表头）， 然后从第四层的前进指针移动到表中的第二个节点。

2. 在第二个节点时， 程序沿着第二层的前进指针移动到表中的第三个节点。

3. 在第三个节点时， 程序同样沿着第二层的前进指针移动到表中的第四个节点。

4. 当程序再次沿着第四个节点的前进指针移动时， 它碰到一个 NULL ， 程序知道这时已经到达了跳跃表的表尾， 于是结束这次遍历。



**跨度**

层的跨度（level[i].span 属性）用于记录两个节点之间的距离：

两个节点之间的跨度越大， 它们相距得就越远。

指向 NULL 的所有前进指针的跨度都为 0 ， 因为它们没有连向任何节点。

遍历操作只使用前进指针就可以完成， 跨度实际上是用来计算排位（rank）的： 

在查找某个节点的过程中， 将沿途访问过的所有层的跨度累计起来， 得到的结果就是目标节点在跳跃表中的排位。

![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAduRnCzG01CSkBO44JKK9qdDWDdgo35FW1ibF60K31upAiaGQD8BC3gY3Aw/0?wx_fmt=jpeg)



上图用虚线标记了在跳跃表中查找分值为 3.0 、成员对象为o3的节点时，沿途经历的层： 查找的过程只经过了一个层，并且层的跨度为3 ，所以目标节点在跳跃表中的排位为3。



**后退指针**

节点的后退指针（backward 属性）用于从表尾向表头方向访问节点： 

跟可以一次跳过多个节点的前进指针不同， 因为每个节点只有一个后退指针， 所以每次只能后退至前一个节点。



![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAduIqInspG3B7BzDdAwY4eaFicRYtaJCuKbdBqzFPiaTnq9gb9ibBW8J8qRw/0?wx_fmt=jpeg)



上图用虚线展示了如果从表尾向表头遍历跳跃表中的所有节点：

程序首先通过跳跃表的tail指针访问表尾节点，然后通过后退指针访问倒数第二个节点，之后再沿着后退指针访问倒数第三个节点，再之后遇到指向NULL的后退指针，于是访问结束。



**分值和成员**

节点的分值（score 属性）是一个 double 类型的浮点数， 跳跃表中的所有节点都按分值从小到大来排序。

节点的成员对象（obj 属性）是一个指针， 它指向一个字符串对象， 而字符串对象则保存着一个 SDS 值。

在同一个跳跃表中，各个节点保存的成员对象必须是唯一的，但是多个节点保存的分值却可以是相同的： 

分值相同的节点将按照成员对象在字典序中的大小来进行排序，成员对象较小的节点会排在前面（靠近表头的方向），而成员对象较大的节点则会排在后面（靠近表尾的方向）。





**跳跃表**

虽然仅靠多个跳跃表节点就可以组成一个跳跃表，但通过使用一个zskiplist结构来持有这些节点，程序可以更方便地对整个跳跃表进行处理，比如快速访问跳跃表的表头节点和表尾节点，又或者快速地获取跳跃表节点的数量（也即是跳跃表的长度）等信息。

![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAdurN6iaLTSYTrvfwzOuk0cNnMxHJX60TlyMH7JwG0PUqhlnsPGklvJQuA/0?wx_fmt=png)

header和tail指针分别指向跳跃表的表头和表尾节点，通过这两个指针，程序定位表头节点和表尾节点的复杂度为O(1)。

通过使用length属性来记录节点的数量，程序可以在O(1)复杂度内返回跳跃表的长度。

level属性则用于在O(1)复杂度内获取跳跃表中层高最大的那个节点的层数量，注意表头节点的层高并不计算在内。

![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAduBlX1rb9kzibl6bcJU8hbez4H4RJQVpYIpabwF0l8xYPiaZEzOW1JHe1w/0?wx_fmt=jpeg)





**整数集合**

整数集合（intset）是集合键的底层实现之一：

当一个集合只包含整数值元素，并且这个集合的元素数量不多时，Redis就会使用整数集合作为集合键的底层实现。





**整数集合的实现**

整数集合（intset）是 Redis 用于保存整数值的集合抽象数据结构， 它可以保存类型为 int16_t 、 int32_t 或者 int64_t 的整数值， 并且保证集合中不会出现重复元素。

![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAdurpHEcIX0ZqBZyX3T8222SicHNmz4MNGeuk9vLUlxYYjroNEUmkAKncQ/0?wx_fmt=png)



**升级**

我们要将一个新元素添加到整数集合里面， 并且新元素的类型比整数集合现有所有元素的类型都要长时， 整数集合需要先进行升级（upgrade）， 然后才能将新元素添加到整数集合里面。

升级整数集合并添加新元素共分为三步进行：

1.根据新元素的类型， 扩展整数集合底层数组的空间大小，并为新元素分配空间。

2.将底层数组现有的所有元素都转换成与新元素相同的类型，并将类型转换后的元素放置到正确的位上，而且在放置元素的过程中，需要继续维持底层数组的有序性质不变。

3.将新元素添加到底层数组里面。

整数集合的升级策略有两个好处， 一个是提升整数集合的灵活性， 另一个是尽可能地节约内存。




**降级**

整数集合不支持降级操作，一旦对数组进行了升级，编码就会一直保持升级后的状态。



**压缩列表**

压缩列表（ziplist）是列表键和哈希键的底层实现之一。

当一个列表键只包含少量列表项，并且每个列表项要么就是小整数值，要么就是长度比较短的字符串，那么Redis就会使用压缩列表来做列表键的底层实现。



**压缩列表的构成**

压缩列表是 Redis 为了节约内存而开发的， 由一系列特殊编码的连续内存块组成的顺序型（sequential）数据结构。

一个压缩列表可以包含任意多个节点（entry）， 每个节点可以保存一个字节数组或者一个整数值。

![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAduuht66uA1kriaPWGkVttBj04qUib7DIaHZ2VxTJYKIS3Fo6Iib4k5cxBsA/0?wx_fmt=jpeg)



压缩列表各个组成部分的详细说明
|**属性**|**类型**|**长度**|**用途**|
|----|----|----|----|
|zlbytes|uint32_t|4 字节|记录整个压缩列表占用的内存字节数：在对压缩列表进行内存重分配，或者计算zlend的位置时使用。|
|zltail|uint32_t|4 字节|记录压缩列表表尾节点距离压缩列表的起始地址有多少字节： 通过这个偏移量，程序无须遍历整个压缩列表就可以确定表尾节点的地址。|
|zllen|uint16_t|2 字节|记录了压缩列表包含的节点数量： 当这个属性的值小于 UINT16_MAX （65535）时， 这个属性的值就是压缩列表包含节点的数量； 当这个值等于 UINT16_MAX 时， 节点的真实数量需要遍历整个压缩列表才能计算得出。|
|entryX|列表节点|不定|压缩列表包含的各个节点，节点的长度由节点保存的内容决定。|
|zlend|uint8_t|1 字节|特殊值 0xFF （十进制 255 ），用于标记压缩列表的末端。|



**压缩列表节点**

每个压缩列表节点可以保存一个字节数组或者一个整数值，其中，字节数组可以是以下三种长度的其中一种：

    长度小于等于 63 （2^{6}-1）字节的字节数组；

    长度小于等于 16383 （2^{14}-1） 字节的字节数组；

    长度小于等于 4294967295 （2^{32}-1）字节的字节数组；

而整数值则可以是以下六种长度的其中一种：

    4 位长，介于 0 至 12 之间的无符号整数；

    1 字节长的有符号整数；

    3 字节长的有符号整数；

    int16_t 类型整数；

    int32_t 类型整数；

int64_t 类型整数。



每个压缩列表节点都由 previous_entry_length 、 encoding 、 content 三个部分组成。



![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAdueIKKdicnZhfxAE2qQ0sZ4ZgSUfupkOn3rLicWiaItfqFFIqOw7WKzsEyw/0?wx_fmt=jpeg)



节点的previous_entry_length属性以字节为单位，记录了压缩列表中前一个节点的长度。

previous_entry_length 属性的长度可以是 1 字节或者 5 字节：

    如果前一节点的长度小于 254 字节， 那么 previous_entry_length 属性的长度为1字节： 前一节点的长度就保存在这一个字节里面。

    如果前一节点的长度大于等于254字节，那么previous_entry_length属性的长度为5字节：其中属性的第一字节会被设置为 0xFE （十进制值 254）， 而之后的四个字节则用于保存前一节点的长度。



节点的encoding属性记录了节点的content属性所保存数据的类型以及长度：

一字节、两字节或者五字节长，值的最高位为00、01或者10的是字节数组编码：这种编码表示节点的content属性保存着字节数组，数组的长度由编码除去最高两位之后的其他位记录；

一字节长，值的最高位以11开头的是整数编码：这种编码表示节点的content属性保存着整数值，整数值的类型和长度由编码除去最高两位之后的其他位记录；



节点的 content 属性负责保存节点的值， 节点值可以是一个字节数组或者整数， 值的类型和长度由节点的 encoding 属性决定。



![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAduXFdl0DmTckDvHjcDRibGkBoYTss78nIibqjM6CmUYvTE0B0KiaYuNnicDA/0?wx_fmt=jpeg)



上图展示了一个保存字节数组的节点示例：

    编码的最高两位 00 表示节点保存的是一个字节数组；

    编码的后六位 001011 记录了字节数组的长度 11 ；

content 属性保存着节点的值 "hello world" 。



![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAduCics89zIic4Rl0wO7P9oj1QsYlEWeqgibBU0NtkaQmoBaTZlyeSM0iaibsA/0?wx_fmt=jpeg)





展示了一个保存整数值的节点示例：

    编码 11000000 表示节点保存的是一个int16_t 类型的整数值；

    content 属性保存着节点的值10086。







## Redis对象系统

前面介绍了 Redis 用到的所有主要数据结构， 比如简单动态字符串（SDS）、双端链表、字典、压缩列表、整数集合， 等等。

Redis并没有直接使用这些数据结构来实现键值对数据库，而是基于这些数据结构创建了一个对象系统， 这个系统包含字符串对象、列表对象、哈希对象、集合对象和有序集合对象这五种类型的对象， 每种对象都用到了至少一种前面所介绍的数据结构。

通过这五种不同类型的对象，Redis可以在执行命令之前，根据对象的类型来判断一个对象是否可以执行给定的命令。使用对象的另一个好处是，可以针对不同的使用场景，为对象设置多种不同的数据结构实现，从而优化对象在不同场景下的使用效率。

除此之外，Redis的对象系统还实现了基于引用计数技术的内存回收机制：当程序不再使用某个对象的时候，这个对象所占用的内存就会被自动释放；另外，Redis 还通过引用计数技术实现了对象共享机制，这一机制可以在适当的条件下，通过让多个数据库键共享同一个对象来节约内存。





**对象的类型和编码**

Redis 中的每个对象都由一个 redisObject 结构表示， 该结构中和保存数据有关的三个属性分别是 type 属性、 encoding 属性和 ptr 属性：

![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAdugO1yZbdicQgn9sSBib4GE7u6V3UBoNgDaGHKhYGLL5yPuIk0kS0TBO3A/0?wx_fmt=png)





对象的 type 属性记录了对象的类型，这个属性的值可以是以下列出的常量的其中一个。
|**类型常量**|**对象的名称**|
|----|----|
|REDIS_STRING|字符串对象|
|REDIS_LIST|列表对象|
|REDIS_HASH|哈希对象|
|REDIS_SET|集合对象|
|REDIS_ZSET|有序集合对象|

对于 Redis 数据库保存的键值对来说，键总是一个字符串对象，而值则可以是字符串对象、列表对象、哈希对象、集合对象或者有序集合对象的其中一种， 因此：

    当称呼一个数据库键为“字符串键”时，指的是“这个数据库键所对应的值为字符串对象”；

    当称呼一个键为“列表键”时，指的是“这个数据库键所对应的值为列表对象”。

下表列出了TYPE 命令在面对不同类型的值对象时所产生的输出。
|**对象**|**对象 type 属性的值**|***TYPE***** 命令的输出**|
|----|----|----|
|字符串对象|REDIS_STRING|"string"|
|列表对象|REDIS_LIST|"list"|
|哈希对象|REDIS_HASH|"hash"|
|集合对象|REDIS_SET|"set"|
|有序集合对象|REDIS_ZSET|"zset"|

****

**编码和底层实现**

对象的ptr指针指向对象的底层实现数据结构，而这些数据结构由对象的encoding属性决定。

对象的编码表：
|**编码常量**|**编码所对应的底层数据结构**|
|----|----|
|REDIS_ENCODING_INT|long 类型的整数|
|REDIS_ENCODING_EMBSTR|embstr 编码的简单动态字符串|
|REDIS_ENCODING_RAW|简单动态字符串|
|REDIS_ENCODING_HT|字典|
|REDIS_ENCODING_LINKEDLIST|双端链表|
|REDIS_ENCODING_ZIPLIST|压缩列表|
|REDIS_ENCODING_INTSET|整数集合|
|REDIS_ENCODING_SKIPLIST|跳跃表和字典|

不同类型和编码的对象表：
|**类型**|**编码**|**对象**|
|----|----|----|
|REDIS_STRING|REDIS_ENCODING_INT|使用整数值实现的字符串对象。|
|REDIS_STRING|REDIS_ENCODING_EMBSTR|使用 embstr 编码的简单动态字符串实现的字符串对象。|
|REDIS_STRING|REDIS_ENCODING_RAW|使用简单动态字符串实现的字符串对象。|
|REDIS_LIST|REDIS_ENCODING_ZIPLIST|使用压缩列表实现的列表对象。|
|REDIS_LIST|REDIS_ENCODING_LINKEDLIST|使用双端链表实现的列表对象。|
|REDIS_HASH|REDIS_ENCODING_ZIPLIST|使用压缩列表实现的哈希对象。|
|REDIS_HASH|REDIS_ENCODING_HT|使用字典实现的哈希对象。|
|REDIS_SET|REDIS_ENCODING_INTSET|使用整数集合实现的集合对象。|
|REDIS_SET|REDIS_ENCODING_HT|使用字典实现的集合对象。|
|REDIS_ZSET|REDIS_ENCODING_ZIPLIST|使用压缩列表实现的有序集合对象。|
|REDIS_ZSET|REDIS_ENCODING_SKIPLIST|使用跳跃表和字典实现的有序集合对象。|

*OBJECT ENCODING* 对不同编码的输出
|**对象所使用的底层数据结构**|**编码常量**|***OBJECT ENCODING***** 命令输出**|
|----|----|----|
|整数|REDIS_ENCODING_INT|"int"|
|embstr编码的简单动态字符串（SDS）|REDIS_ENCODING_EMBSTR|"embstr"|
|简单动态字符串|REDIS_ENCODING_RAW|"raw"|
|字典|REDIS_ENCODING_HT|"hashtable"|
|双端链表|REDIS_ENCODING_LINKEDLIST|"linkedlist"|
|压缩列表|REDIS_ENCODING_ZIPLIST|"ziplist"|
|整数集合|REDIS_ENCODING_INTSET|"intset"|
|跳跃表和字典|REDIS_ENCODING_SKIPLIST|"skiplist"|



通过 encoding 属性来设定对象所使用的编码， 而不是为特定类型的对象关联一种固定的编码， 极大地提升了 Redis 的灵活性和效率， 因为 Redis 可以根据不同的使用场景来为一个对象设置不同的编码， 从而优化对象在某一场景下的效率。





**字符串对象**

字符串对象的编码可以是 int 、 raw 或者 embstr 。

如果一个字符串对象保存的是整数值， 并且这个整数值可以用 long 类型来表示， 那么字符串对象会将整数值保存在字符串对象结构的 ptr 属性里面（将 void* 转换成 long ）， 并将字符串对象的编码设置为 int 。

![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAduo2iaD2icbxrqBNyT2uwfNl6KeC6bBKtMDWRNKu6SIkNY5jFtC2ty8ibEA/0?wx_fmt=jpeg)





如果字符串对象保存的是一个字符串值，并且这个字符串值的长度大于 39字节，那么字符串对象将使用一个简单动态字符串（SDS）来保存这个字符串值，并将对象的编码设置为 raw 。

![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAdufxWDxVwocqibzksAzNVXQ90GwHHxmnfeJvicLu7hZspaauQ6SthmNFsg/0?wx_fmt=jpeg)

****

****

****

如果字符串对象保存的是一个字符串值， 并且这个字符串值的长度小于等于 39 字节， 那么字符串对象将使用 embstr 编码的方式来保存这个字符串值。

embstr 编码是专门用于保存短字符串的一种优化编码方式，这种编码和 raw 编码一样， 都使用 redisObject 结构和 sdshdr 结构来表示字符串对象，但 raw 编码会调用两次内存分配函数来分别创建 redisObject 结构和 sdshdr 结构， 而 embstr 编码则通过调用一次内存分配函数来分配一块连续的空间，空间中依次包含 redisObject 和 sdshdr 两个结构。

![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAduicGBXUx2ryJubArKxhJyvXS0Yx4PydYuk1tMAp7zpoVx3EwQY0GdTCg/0?wx_fmt=jpeg)****

****

**编码转换**


int 编码的字符串对象和 embstr 编码的字符串对象在条件满足的情况下（int执行append字符串，embstr执行修改操作等），会被转换为 raw 编码的字符串对象。



**字符串命令**

字符串键的值为字符串对象， 所以用于字符串键的所有命令都是针对字符串对象来构建的，针对不同的编码，命令有具体的不同实现，此处就不详细列举了。



**列表对象**

列表对象的编码可以是 ziplist 或者 linkedlist。


ziplist 编码的列表对象使用压缩列表作为底层实现，每个压缩列表节点（entry）保存了一个列表元素。

![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAduibwHumr2GJZDdjicZq0QqPIxGKI0bKOkj8AlJkKmPb1XJGpTcJoTbQpQ/0?wx_fmt=jpeg)






linkedlist 编码的列表对象使用双端链表作为底层实现， 每个双端链表节点（node）都保存了一个字符串对象， 而每个字符串对象都保存了一个列表元素。



![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAdufkSSJib2HyogGNbLPO5ibgpsic9z0wo42aJiaAKvpic78AcEzKjqhamgfZg/0?wx_fmt=jpeg)

****

****

**编码转换**

当列表对象可以同时满足以下两个条件时， 列表对象使用 ziplist 编码：

1. 列表对象保存的所有字符串元素的长度都小于 64 字节；

2. 列表对象保存的元素数量小于 512 个；

不能满足这两个条件的列表对象需要使用 linkedlist 编码。





**哈希对象**

哈希对象的编码可以是 ziplist 或者 hashtable 。


ziplist 编码的哈希对象使用压缩列表作为底层实现， 每当有新的键值对要加入到哈希对象时， 程序会先将保存了键的压缩列表节点推入到压缩列表表尾， 然后再将保存了值的压缩列表节点推入到压缩列表表尾：

保存了同一键值对的两个节点总是紧挨在一起，保存键的节点在前，保存值的节点在后；

先添加到哈希对象中的键值对会被放在压缩列表的表头方向，而后来添加到哈希对象中的键值对会被放在压缩列表的表尾方向。

![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAduicB34Tdm4KiaIPPUE1Znc3InLibH6D0NiaFf1boTARPyPdkmxZxeZaaJrQ/0?wx_fmt=jpeg)




hashtable 编码的哈希对象使用字典作为底层实现， 哈希对象中的每个键值对都使用一个字典键值对来保存：

字典的每个键都是一个字符串对象，对象中保存了键值对的键；

字典的每个值都是一个字符串对象，对象中保存了键值对的值。



![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAduUA3FJrx9j7Qz69tpBFKM7qsQhTpHcRwt79ztgTLVVMOZnE6a5UCCWQ/0?wx_fmt=jpeg)

****

****

**编码转换**

当哈希对象可以同时满足以下两个条件时， 哈希对象使用 ziplist 编码：

1.哈希对象保存的所有键值对的键和值的字符串长度都小于 64 字节；

2.哈希对象保存的键值对数量小于 512 个；

不能满足这两个条件的哈希对象需要使用 hashtable 编码。



**集合对象**

集合对象的编码可以是 intset 或者 hashtable 。


intset 编码的集合对象使用整数集合作为底层实现， 集合对象包含的所有元素都被保存在整数集合里面。

![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAduAnzsAjHibv7T5Mqa3bMXTJr8pxcsG4pryiaxr7I56gTseZQJf4AA78ibA/0?wx_fmt=jpeg)




hashtable 编码的集合对象使用字典作为底层实现， 字典的每个键都是一个字符串对象， 每个字符串对象包含了一个集合元素， 而字典的值则全部被设置为 NULL 。



![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAdusHhOQ3zRKiaLnew3Qapb4gIJYg5hVrs77og9YNS5RjqueNFXtezpTMQ/0?wx_fmt=jpeg)



**编码转换**

当集合对象可以同时满足以下两个条件时， 对象使用 intset 编码：

1.集合对象保存的所有元素都是整数值；

2.集合对象保存的元素数量不超过 512 个；

不能满足这两个条件的集合对象需要使用 hashtable 编码。



**有序集合对象**

有序集合的编码可以是 ziplist 或者 skiplist 。




ziplist 编码的有序集合对象使用压缩列表作为底层实现， 每个集合元素使用两个紧挨在一起的压缩列表节点来保存， 第一个节点保存元素的成员（member）， 而第二个元素则保存元素的分值（score）。

压缩列表内的集合元素按分值从小到大进行排序， 分值较小的元素被放置在靠近表头的方向， 而分值较大的元素则被放置在靠近表尾的方向。



![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAduuYae1sJoEvdAb595vReFAxpRhvwV1IiaIUopFMlpiaDZa0s6bDTDZ23A/0?wx_fmt=jpeg)




skiplist 编码的有序集合对象使用 zset 结构作为底层实现， 一个 zset 结构同时包含一个字典和一个跳跃表：

![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAduLiamuicpt926Qa5icKfm4qhlFlIJibuEVvl3K9KIT5OeIJ5GMMYvxKeNOg/0?wx_fmt=png)








zset 结构中的 zsl 跳跃表按分值从小到大保存了所有集合元素， 每个跳跃表节点都保存了一个集合元素： 跳跃表节点的 object 属性保存了元素的成员， 而跳跃表节点的 score 属性则保存了元素的分值。 通过这个跳跃表， 程序可以对有序集合进行范围型操作， 比如 ZRANK 、 ZRANGE 等命令就是基于跳跃表 API 来实现的。



zset 结构中的 dict 字典为有序集合创建了一个从成员到分值的映射， 字典中的每个键值对都保存了一个集合元素： 字典的键保存了元素的成员， 而字典的值则保存了元素的分值。 通过这个字典， 程序可以用 O(1) 复杂度查找给定成员的分值， ZSCORE 命令就是根据这一特性实现的， 而很多其他有序集合命令都在实现的内部用到了这一特性。



有序集合每个元素的成员都是一个字符串对象， 而每个元素的分值都是一个 double 类型的浮点数。 值得一提的是， 虽然 zset 结构同时使用跳跃表和字典来保存有序集合元素， 但这两种数据结构都会通过指针来共享相同元素的成员和分值， 所以同时使用跳跃表和字典来保存集合元素不会产生任何重复成员或者分值， 也不会因此而浪费额外的内存。

![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAdut5p4kLo8nLHCVL0v5FqSCUzED2X5IibzhaW1JKs4QPN33sJEPSGsuhg/0?wx_fmt=jpeg)



**编码转换**

当有序集合对象可以同时满足以下两个条件时， 对象使用 ziplist 编码：

1.有序集合保存的元素数量小于 128 个；

2.有序集合保存的所有元素成员的长度都小于 64 字节；



不能满足以上两个条件的有序集合对象将使用 skiplist 编码。



**类型检查与命令多态**

Redis 中用于操作键的命令基本上可以分为两种类型：

1. 可以对任何类型的键执行， 比如说 DEL 命令、 EXPIRE 命令、 RENAME 命令、 TYPE 命令、 OBJECT 命令， 等等

2. 只能对特定类型的键执行， 比如：

SET 、 GET 、 APPEND 、 STRLEN 等命令只能对字符串键执行；

HDEL 、 HSET 、 HGET 、 HLEN 等命令只能对哈希键执行；

RPUSH 、 LPOP 、 LINSERT 、 LLEN 等命令只能对列表键执行；

SADD 、 SPOP 、 SINTER 、 SCARD 等命令只能对集合键执行；

ZADD 、 ZCARD 、 ZRANK 、 ZSCORE 等命令只能对有序集合键执行；



**类型检查的实现**

为了确保只有指定类型的键可以执行某些特定的命令， 在执行一个类型特定的命令之前， Redis 会先检查输入键的类型是否正确， 然后再决定是否执行给定的命令。

类型特定命令所进行的类型检查是通过 redisObject 结构的 type 属性来实现的：

1.在执行一个类型特定命令之前， 服务器会先检查输入数据库键的值对象是否为执行命令所需的类型， 如果是的话， 服务器就对键执行指定的命令；

2.否则， 服务器将拒绝执行命令， 并向客户端返回一个类型错误。



**多态命令的实现**

Redis 除了会根据值对象的类型来判断键是否能够执行指定命令之外， 还会根据值对象的编码方式， 选择正确的命令实现代码来执行命令。





**内存回收**

****

因为 C 语言并不具备自动的内存回收功能， 所以 Redis 在自己的对象系统中构建了一个引用计数（reference counting）技术实现的内存回收机制， 通过这一机制， 程序可以通过跟踪对象的引用计数信息， 在适当的时候自动释放对象并进行内存回收。



每个对象的引用计数信息由 redisObject 结构的 refcount 属性记录：

![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAdu4U0LLibXFIPyMXbeVg7OMlYrqCZhYdHusS9ElP0zYLoD5hjdVWWW0Cw/0?wx_fmt=png)





对象的引用计数信息会随着对象的使用状态而不断变化：

    在创建一个新对象时， 引用计数的值会被初始化为 1 ；

    当对象被一个新程序使用时， 它的引用计数值会被增一；

    当对象不再被一个程序使用时， 它的引用计数值会被减一；

当对象的引用计数值变为 0 时， 对象所占用的内存会被释放。





**对象共享**

除了用于实现引用计数内存回收机制之外， 对象的引用计数属性还带有对象共享的作用。

在 Redis 中， 让多个键共享同一个值对象需要执行以下两个步骤：

1.将数据库键的值指针指向一个现有的值对象；

2.将被共享的值对象的引用计数增一。

共享对象机制对于节约内存非常有帮助， 数据库中保存的相同值对象越多， 对象共享机制就能节约越多的内存。





**对象的空转时长**

除了前面介绍过的 type 、 encoding 、 ptr 和 refcount 四个属性之外， redisObject 结构包含的最后一个属性为 lru 属性， 该属性记录了对象最后一次被命令程序访问的时间：

![](https://mmbiz.qlogo.cn/mmbiz/wFTDMH6f01FYQTIQLPx2WpImTUDmfAdubn4I4GTupF8J8DtRUazicnDbC0qBjSRGVCribnhO7NU78dDtTZwibEDmw/0?wx_fmt=png)

OBJECT IDLETIME 命令可以打印出给定键的空转时长， 这一空转时长就是通过将当前时间减去键的值对象的 lru 时间计算得出的。































