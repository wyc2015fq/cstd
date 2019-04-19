# NoSQL之Cassandra - zhanlijun - 博客园
# [NoSQL之Cassandra](https://www.cnblogs.com/LBSer/p/3328841.html)
#### [http://www.cnblogs.com/LBSer/p/3328841.html](http://www.cnblogs.com/LBSer/p/3328841.html)
  　9月初听了一个讲座，演讲者是张月同学，他给我们分享了Cassandra nosql数据库，讲得很精彩，听完之后收益良多。
　　Cassandra是一个noSQL数据库，在国外被广泛使用，比如FaceBook、Twitter、Intel等，国内用的较少，只有奇虎360等公司在大规模使用。张月首先讲了Cassandra怎么来的，之后讲了Cassandra的一些具体细节，让我印象比较深刻的有数据分布以及通讯协议两个部分，最后讲了Cassandra的效率以及使用情况。
**1. Cassandra怎么来的**
他山之石可以攻玉，Cassandra从Dynamo和BigTable中借鉴了相应的思想。
|**类似Dynamo的特性**|**类似bigTable的特性**|
|----|----|
|对称、P2P的结构，没有主节点|稀疏、列存储模型|
|基于 Gossip的集群管理|commit log，内存有 MemTable，SSTable Files|
|分布式哈西表|支持Hadoop|
|最终一致性||
**2. Cassandra中的数据是怎样分布的？**
         1.2版本之前使用的是普通一致性哈西算法，如图上部所示，一共6个节点，每个节点计算Hash值a，之后各个节点根据各自的hash值映射到一段区间上，比如Node1对应着A，此外，各个节点保存着其它节点的两个副本，比如Node1还保存着Node5的E，以及Node6的F。
         普通的一致性哈西有个缺点，当增加或删除节点时会导致数据分布不均衡，比如Node2宕机后，原先Node2的数据全部分布到Node3上，造成Node3的数据量大大多余其它节点；同理，当在区间B插入一个节点Node7时，会使得Node2和Node7的数据量大大小于其它节点。
为了解决上述问题，1.2版本使用带有虚拟节点的一致性哈西算法，如图下部所示，一共6个节点，每个节点需要计算两次hash，首先与传统一样计算Hash值a，然后根据这个值a计算多个hash值a1,a2,a3...，即将一个节点映射到多个hash值，从而每个节点不再仅仅对应一个区间，而是对应着许多小区间，如果保证各个hash值是随机的，则能使得这些小区间的分布也是随机的，从而确保了数据分布的均衡性
![](https://images0.cnblogs.com/blog/522490/201309/21115024-ec6db8e616164ceba442416a6040265b.png)
** 3. Gossip协议管理集群**
           每隔T秒，集群中各个节点将都会将自己的Heartbeat信息通过Gossip传递给其他节点。Gossip协议效率较高，只需要Log(N)次就能将所有节点状态信息传递给各个节点。
![](https://images0.cnblogs.com/blog/522490/201309/21115054-dc950d3c0bb44ebfb3fe5d0e65dbeea9.png)
**4. Cassandra的效率以及使用情况**
       张月给了张图，上面显示Cassandra读写速度远远大于Hbase和MongoDB，但是也有很多人质疑这个图，因为他没有给出这张图的实验条件，比如多大数据量，各个数据库是否在同一集群上等等。不过我后来查看了些文献，发现Cassandra的性能还是很不错的。
![](https://images0.cnblogs.com/blog/522490/201309/21115134-291aa53b84cf40b0bdc3bab44ac53f60.png)
**5. 讨论**
         会后有个问题：既然Cassandra性能这么出色， 为什么国内很少有公司使用Cassandra？ 为什么国外的facebook和twitter等都弃用Cassandra而转用HBase了，Cassandra有什么不足？
         张月的回答是国内的宣传不够，至于facebook和twitter为什么转用HBase，可能和HBase天生支持Hadoop有关系，可以结合mapreduce，虽然Cassandra也支持Hadoop，但是配置起来特别麻烦。
         后来我网上查了下，Cassandra还是有一些缺点，或许因为这些使得很多大公司转向HBase。
         a）Cassandra来源自Facebook，但是在Facebook内部Cassandra 目前只用在inbox search产品上，容量大约有一两百T，且Inbox Search在Facebook的也不是核心应用；
         b）Cassandra出身较晚，自身还存在不少问题。

