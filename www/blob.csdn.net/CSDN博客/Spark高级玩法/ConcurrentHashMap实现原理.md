# ConcurrentHashMap实现原理 - Spark高级玩法 - CSDN博客
2018年04月01日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：144
ConcurrentHashMap是Java1.5中引用的一个线程安全的支持高并发的HashMap集合类。这篇文章总结了ConcurrentHashMap的内部实现原理，是对于自己理解后的一些整理。
#### **1.HashTable与ConcurrentHashMap的对比**
HashTable本身是线程安全的，写过Java程序的都知道通过加Synchronized关键字实现线程安全，这样对整张表加锁实现同步的一个缺陷就在于使程序的效率变得很低。这就是为什么Java中会在1.5后引入ConcurrentHashMap的原因。
![640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFVacn50auMODzWclnwQZ3CTibtkbAEwvS6e49coWCw4BeaiamtRc2PPW7h1icULu4T8nbYNYfkGSXmcw/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1)
从图中可以看出，HashTable的锁加在整个Hash表上，而ConcurrentHashMap将锁加在segment上（每个段上），这样我们在对segment1操作的时候，同时也可以对segment2中的数据操作，这样效率就会高很多。
#### **2.ConcurrentHashMap的内部结构**
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVacn50auMODzWclnwQZ3CTX2GYx7dwqLUm0ibKqpSvRlDibgpzPbKzeBPuxH5QVakuV1HnSVMprX8g/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
ConcurrentHashMap主要有三大结构：整个Hash表，segment（段），HashEntry（节点）。每个segment就相当于一个HashTable。
##### **（1）HashEntry类**
每个HashEntry代表Hash表中的一个节点，在其定义的结构中可以看到，除了value值没有定义final，其余的都定义为final类型，我们知道Java中关键词final修饰的域成为最终域。用关键词final修饰的变量一旦赋值，就不能改变，也称为修饰的标识为常量。这就意味着我们删除或者增加一个节点的时候，就必须从头开始重新建立Hash链，因为next引用值需要改变。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVacn50auMODzWclnwQZ3CTZ34pBs25nH9wJbGU1UoxtDVPk9Ml3E314p7S1H4drT94kRcSHSJekw/640?wx_fmt=png)
由于这样的特性，所以插入Hash链中的数据都是从头开始插入的。例如将A,B,C插入空桶中，插入后的结构为： 
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFVacn50auMODzWclnwQZ3CTy6ic1sViajvOW8CpH2EQRWQIter0bDHVETBEvUk5fJHZRshuHhXMG8icQ/640?wx_fmt=jpeg)
##### **（2）segment类**
Segment 类继承于 ReentrantLock 类，从而使得 Segment 对象能充当锁的角色。每个 Segment 对象用来守护其（成员对象 table 中）包含的若干个桶。
table 是一个由 HashEntry 对象组成的数组。table 数组的每一个数组成员就是散列映射表的一个桶。
count 变量是一个计数器，它表示每个 Segment 对象管理的 table 数组（若干个 HashEntry 组成的链表）包含的 HashEntry 对象的个数。每一个 Segment 对象都有一个 count 对象来表示本 Segment 中包含的 HashEntry 对象的总数。注意，之所以在每个 Segment 对象中包含一个计数器，而不是在 ConcurrentHashMap 中使用全局的计数器，是为了避免出现“热点域”而影响 ConcurrentHashMap 的并发性。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVacn50auMODzWclnwQZ3CTrMqnh3so0Ir7ncrmE5zOW0PYKJfoA3miciaRE9DUDUIjme8tvjYvHmHw/640?wx_fmt=png)
##### **ConcurrentHashMap 类**
默认的情况下，每个ConcurrentHashMap 类会创建16个并发的segment，每个segment里面包含多个Hash表，每个Hash链都是有HashEntry节点组成的。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVacn50auMODzWclnwQZ3CTW0GIiavMKBYa44yUags2OY1ltzauP9aXkZHPYgcXsnDR0m5eCol3iazg/640?wx_fmt=png)
#### **3.用分离锁实现多个线程间的并发写操作**
##### **（1）Put方法的实现**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVacn50auMODzWclnwQZ3CTGLrBbOy6iaLib8uOtVSO3xibyjQzzfbmZbSdIgDKY1qA5oIuHhJq6NoXg/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVacn50auMODzWclnwQZ3CTQsFVkHwUI0hTVfrnZmoMV0DcKIsoTMjXHRTwHZpQ4Yibcz3pM3mSXnQ/640?wx_fmt=png)
整个代码通过注释很好理解了，稍微要注意的是这里的加锁是针对具体的segment，而不是对整个ConcurrentHashMap。Put方法从源码上可以看出是从链表的头部插入新的数据的。
##### **（2）Get方法的实现**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVacn50auMODzWclnwQZ3CTjqJEXyPISp9tiaeNZX4wuXn1JRcUVpAYvzJWbiaUrjpnsSueMkzX448A/640?wx_fmt=png)
ConcurrentHashMap中的读方法不需要加锁，所有的修改操作在进行结构修改时都会在最后一步写count 变量，通过这种机制保证get操作能够得到几乎最新的结构更新。
##### **（3）Remove方法的实现**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVacn50auMODzWclnwQZ3CTzYMqqdmbdvpSVicuAeZn4XUXKia4rQMbQIGxmibS1ltJ3EliauamylFicQw/640?wx_fmt=png)
整个操作是在持有段锁的情况下执行的，空白行之前的行主要是定位到要删除的节点e。接下来，如果不存在这个节点就直接返回null，否则就要将e前面的结点复制一遍，尾结点指向e的下一个结点。e后面的结点不需要复制，它们可以重用。
中间那个for循环是做什么用的呢？从代码来看，就是将定位之后的所有entry克隆并拼回前面去，但有必要吗？每次删除一个元素就要将那之前的元素克隆一遍？这点其实是由entry的不变性来决定的，仔细观察entry定义，发现除了value，其他所有属性都是用final来修饰的，这意味着在第一次设置了next域之后便不能再改变它，取而代之的是将它之前的节点全都克隆一次。至于entry为什么要设置为不变性，这跟不变性的访问不需要同步从而节省时间有关。
执行删除之前的原链表： 
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFVacn50auMODzWclnwQZ3CTvJoBsTdGIlUmaktQaSjP3ElmIp1KkF36nzazkMUZsU5ws7XTKujlJQ/640?wx_fmt=jpeg)
执行删除之后的新链表 
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFVacn50auMODzWclnwQZ3CTia4cgcibqOoCOx8IncWLHTlIwG2HNStzLJgOS8NtuExfLsrrzs1gYYog/640?wx_fmt=jpeg)
注意：新链表在clone的时候。顺序发生反转，A->B变为B->A。
##### **（4）containsKey方法的实现**
containsKey方法操作相对简单，因为它不需要读取值。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVacn50auMODzWclnwQZ3CTO4kkriakyvnT56QxeSczPS7Q5ROZLkdvcayWRY7BBAHIJW1WdPgwYlQ/640?wx_fmt=png)
#### **4.总结**
在使用锁来协调多线程间并发访问的模式下，减小对锁的竞争可以有效提高并发性。有两种方式可以减小对锁的竞争：
- 
减小请求同一个锁的频率。
- 
减少持有锁的时间。
ConcurrentHashMap 的高并发性主要来自于三个方面：
- 
用分离锁实现多个线程间的更深层次的共享访问。
- 
用 HashEntery 对象的不变性来降低执行读操作的线程在遍历链表期间对加锁的需求。
- 
通过对同一个 Volatile 变量的写 / 读访问，协调不同线程间读 / 写操作的内存可见性。
使用分离锁，减小了请求同一个锁的频率。
**concurrentHashMap在jdk1.8中主要做了2方面的改进**
**改进一：**取消segments字段，直接采用transient volatile HashEntry<K,V>[] table保存数据，采用table数组元素作为锁，从而实现了对每一行数据进行加锁，进一步减少并发冲突的概率。
**改进二：**将原先table数组＋单向链表的数据结构，变更为table数组＋单向链表＋红黑树的结构。对于hash表来说，最核心的能力在于将key hash之后能均匀的分布在数组中。如果hash之后散列的很均匀，那么table数组中的每个队列长度主要为0或者1。但实际情况并非总是如此理想，虽然ConcurrentHashMap类默认的加载因子为0.75，但是在数据量过大或者运气不佳的情况下，还是会存在一些队列长度过长的情况，如果还是采用单向列表方式，那么查询某个节点的时间复杂度为O(n)；因此，对于个数超过8(默认值)的列表，jdk1.8中采用了红黑树的结构，那么查询的时间复杂度可以降低到O(logN)，可以改进性能。
本文综合参考与网络，参考链接如下：
https://www.cnblogs.com/everSeeker/p/5601861.html
http://www.cnblogs.com/ITtangtang/p/3948786.html
**推荐阅读：**
1，[基于java的中文分词工具ANSJ](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484304&idx=1&sn=2c8280e8ec0c008074d2cf958c7d7489&chksm=9f38e0b8a84f69aed49600507e469f42c6697b77ab2edfdf3ef8904f46990d10c77932845d0a&scene=21#wechat_redirect)
2，[Java transient关键字使用小记](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484445&idx=1&sn=c4e2c0a10042c2759eb7186cc05e0bfb&chksm=9f38e735a84f6e232910b89ad80f3486a5a02c772c31ff81eaedb3ca223c7d2cc030c5e568e9&scene=21#wechat_redirect)
3，[Scala语法基础之隐式转换](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483864&idx=1&sn=4e1e742e00b2eabcddd24d52e70a5df9&chksm=9f38e2f0a84f6be6b5201ecfead986c83d1f2f031536cc731ef8323f28c2daa92595cabf8d4f&scene=21#wechat_redirect)
4，[Scala语言基础之结合demo和spark讲实现链式计算](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483854&idx=1&sn=8e517bc2cfef1e4b52e5baa871552380&chksm=9f38e2e6a84f6bf086cd8332f210dbffd6fbe9b7d29681cb0ca0413abe5faa7b921bfdb01d84&scene=21#wechat_redirect)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640?wx_fmt=png)
