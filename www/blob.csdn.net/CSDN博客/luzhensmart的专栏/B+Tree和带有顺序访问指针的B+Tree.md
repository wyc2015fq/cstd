
# B+Tree和带有顺序访问指针的B+Tree - luzhensmart的专栏 - CSDN博客


2019年03月07日 17:32:34[芦震](https://me.csdn.net/luzhensmart)阅读数：13


### B+Tree
B-Tree有许多变种，其中最常见的是B+Tree，例如MySQL就普遍使用B+Tree实现其索引结构。
与B-Tree相比，B+Tree有以下不同点：
每个节点的指针上限为2d而不是2d+1。
内节点不存储data，只存储key；叶子节点不存储指针。
图3是一个简单的B+Tree示意。
![](http://blog.codinglabs.org/uploads/pictures/theory-of-mysql-index/3.png)
图3
由于并不是所有节点都具有相同的域，因此B+Tree中叶节点和内节点一般大小不同。这点与B-Tree不同，虽然B-Tree中不同节点存放的key和指针可能数量不一致，但是每个节点的域和上限是一致的，所以在实现中B-Tree往往对每个节点申请同等大小的空间。
一般来说，B+Tree比B-Tree更适合实现外存储索引结构，具体原因与外存储器原理及计算机存取原理有关，将在下面讨论。
### 带有顺序访问指针的B+Tree
**一般在数据库系统或文件系统中使用的B+Tree结构都在经典B+Tree的基础上进行了优化，增加了顺序访问指针。**
![](http://blog.codinglabs.org/uploads/pictures/theory-of-mysql-index/4.png)
图4
如图4所示，在B+Tree的每个叶子节点增加一个指向相邻叶子节点的指针，就形成了带有顺序访问指针的B+Tree。做这个优化的目的是为了提高区间访问的性能，例如图4中如果要查询key为从18到49的所有数据记录，当找到18后，只需顺着节点和指针顺序遍历就可以一次性访问到所有数据节点，极大提到了区间查询效率。
这一节对B-Tree和B+Tree进行了一个简单的介绍，下一节结合存储器存取原理介绍为什么目前B+Tree是数据库系统实现索引的首选数据结构。


