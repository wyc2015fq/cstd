# 入门B-树的世界 - bigfacesafdasgfewgf - CSDN博客





2014年11月11日 21:46:33[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：1064








# 入门B-树的世界

    很久之前，看过一篇关于外存磁盘数据搜索的讲解稿，偶然看到B树的知识。当时青涩地觉着：高大上的数据结构啊，渊博的data structure啊~~哈哈哈，今天我终于可以来了解一下这种外存数据结构：B树。

## 1. B树的引入

    前面我们介绍的二叉搜索树（二叉查找树）、AVL树等等都是当数据存储在内存中对应的搜索结构。当我们在内存数据中搜索的时候，用AVL树表示就可以获得很好的搜索性能了。但是，当数据量很大的时候，内存已经无法容纳了，我们就只好把数据存储在外存（e.g. 磁盘）中，这个时候由于磁盘读取数据非常耗时。磁盘的读写时间远远慢于内存访问的时间。如果我们可以减少磁盘存取操作的次数，那么就可以提高外搜索算法的性能。

    我们必须使用一些外存数据结构来配合搜索算法，这样才能取得很好的性能。Ｂ树就是常用的一种外存数据结构。

## 2. m阶搜索树

　首先解释一下ｍ阶：就是该树上的结点，最多只能有ｍ个子树，而且每个结点上允许有多个关键字存储在那里。更加详细的说，最多只能存放ｍ－１个元素和ｍ个指向子树的指针。还有，每个结点中的元素都按关键字递增排序，一个元素的关键字值大于它的左子树，小于右子树。每个结点中的元素的个数总是指针的个数少１，空树除外。所以说，对于一个４阶的搜索树而言，每个结点最多有３个元素，４个指向子树的指针。

- 　已知ｍ阶搜索树的高度为ｈ，该树上结点的个数最多有：


![](https://img-blog.csdn.net/20141111214655781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



- 　已知ｍ阶搜索树的高度为ｈ，该树上元素的个数最多有ｍ＾ｈ－１个；
- 一个有N个元素的m阶搜索树的高度范围为：


![](https://img-blog.csdn.net/20141111214819382?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





    从上面几个结论就可以看出，普通的m阶搜索树即使元素个数确定了，高度也会有较大范围的变化。这就是和普通的二叉搜索树一样，会发生退化。B树就可以很好的解决这个问题。




## 3. B树的基本知识

    B树的定义如下：

一棵m阶B树至少是一棵m阶搜索树，或者是一棵空树，它需要满足下面几个条件：

1）根节点至少有两个孩子；

2）除根节点和失败结点外的其余结点至少有ceil（m/2）个孩子，最多有m个孩子；

3）所有失败结点均在同一层上。




从定义可以看出，Ｂ树相比于上面的普通的ｍ阶搜索树，有两点不同：每个结点规定了最少孩子的个数；规定了失败结点（空树）需要在同一层上。这两个也是防止Ｂ树退化的原因。




有几个结论要记住的：

１）Ｂ树的元素总数Ｎ等于失败结点的总数－１；

２）有Ｎ个元素的ｍ阶Ｂ树的最大高度为：

![](https://img-blog.csdn.net/20141112073656112?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


也就是说在含有N个元素的B树上搜索一个关键字的时候，假如从根节点开始到关键字所在的结点的搜索路径上，涉及的结点个数不超过上面那个数，这也是B树的最大高度（不含失败结点）。




举个例子：

![](https://img-blog.csdn.net/20141112075111524?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


我们需要记住的是，对于一棵5阶的B树而言，起每个结点的关键字个数为2~4个，子树个数为3~5个。







## 4. B树的搜索

    B树搜索和普通的二叉搜索树或者是m阶搜索树的过程是一样的，只不过它要分为两个阶段：

1）磁盘搜索阶段：这部分和二叉搜索树的搜索过程一样的，如果比左子树的最大元素小，那么就进入左子树搜索，如果比右子树的最小值大，进入右子树继续搜索。这该搜索阶段，磁盘被访问的次数最多等于树的最高高度（同上值）；

2）结点内部搜索阶段：当找到了哪个结点时候，需要在结点内部搜索，因为结点内部有多个元素。因为B树的结点元素可以看成一个有序表，所以在一个B树的结点中搜索其实是在内存在搜索，可以采用顺序搜索和二分搜索等内搜索算法实现。




## 5. B树的插入

    我先总的说一下B树插入的方法：

1）在B树种先搜索给定的关键字，如果搜索成功，表示有重复元素，插入运算失败；否则将新元素和一个空指针插入搜索失败处的叶子结点上；

2）如果插入新元素（和一个指针）后，该结点没有溢出，即结点中包含的元素个数没有超过m-1个，指针个数没有超过m个，那么插入成功；

3）如果插入之后结点溢出了，这必须进行结点的分裂操作。将结点一分为三。分裂发生在位置ceil(m/2)处，它之前的元素保留在原来的结点q中，它之后的元素放在一个新建的结点（设该结点地址为q'）中，关键字K{ceil(m/2)}和指向q'将插入结点q的父亲结点中。当然，如果这个父亲结点也溢出了，这继续一分为三操作，继续判断是否溢出；

4）入如果一直寻找根节点来插入的操作，直到发现根节点也溢出了，这个时候树的高度为+1；




下面做个图示说明：（july的博客上已经给出了一个很好的说明了，这儿就直接盗图啦~~嘻嘻）




![](https://img-blog.csdn.net/20141112082048390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)














![](https://img-blog.csdn.net/20141112082129093?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20141112082202810?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)








![](https://img-blog.csdn.net/20141112082233262?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)








![](https://img-blog.csdn.net/20141112082307828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





## 6. B树的删除

    还是先说一下B树删除的方法：

1）首先搜索被删除的元素，如果不存在被删除的元素，这删除运算失败终止；如果搜索成功，且被删除的元素在叶子结点中，则从该叶子结点中删除该元素；如果被删除的元素不在叶子结点中，那么由它的右侧子树上的最小元素取代之，这个最小元素一定在叶子结点中，然后从叶子结点中伤删除该替代元素。

2）如果删除元素后，当前结点中包含至少ceil(m/2)-1个元素，删除运算成功结束；

3）如果删除元素后，当前结点中包含不足ceil(m/2)-1个元素，这称发生下溢。处理的方法首先是借元素：如果其左侧兄弟包含多余ceil(m/2)-1元素，则可以向其左兄弟“借”一个元素；否则如果其右侧兄弟有多余的元素，则向其右侧兄弟借元素。借元素的过程是循环进行的；

4）如果删除元素后，当前结点产生下溢，且左右两侧的兄弟结点都只有ceil(m/2)-1个元素，则只能连接。若当前结点有左侧兄弟，则将该结点与其左侧兄弟连成一个结点，否则与右侧兄弟连接。连接是将两个结点中的元素，连同它们的双亲结点中用来分割它们的元素组合在一个结点中，另一个结点将撤销。这意味着从其双亲结点中删除分割元素和一个指向被撤销的结点的指针，这可能导致双亲结点的下溢，所以就需要继续检查其双亲结点。




下面用图示来说明B树的删除结点的过程：




![](https://img-blog.csdn.net/20141112195250352?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20141112195345513?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20141112195457859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





![](https://img-blog.csdn.net/20141112195531749?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





![](https://img-blog.csdn.net/20141112195605383?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





![](https://img-blog.csdn.net/20141112195727125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20141112195757781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





![](https://img-blog.csdn.net/20141112195829605?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20141112195915796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20141112195941116?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





## 7. B+树和B*树

    B+树中，所有的叶子结点中包含了全部关键字的信息，及指向含有这些关键字记录的指针，且叶子结点本身依关键字的大小自小而大的顺序链接。 而B 树的叶子节点并没有包括全部需要查找的信息。

    B+树所有的非终端结点可以看成是索引部分，结点中仅含有其子树根结点中最大（或最小）关键字。 而B 树的非终节点也包含需要查找的有效信息。![](https://img-blog.csdn.net/20141112200411140?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


这一段实在是找不到很好的B+树介绍资料了，摘录一段july的博客吧，写的不错~

“

a) 为什么说B+-tree比B 树更适合实际应用中操作系统的文件索引和数据库索引？

1) B+-tree的磁盘读写代价更低

B+-tree的内部结点并没有指向关键字具体信息的指针。因此其内部结点相对B 树更小。如果把所有同一内部结点的关键字存放在同一盘块中，那么盘块所能容纳的关键字数量也越多。一次性读入内存中的需要查找的关键字也就越多。相对来说IO读写次数也就降低了。

举个例子，假设磁盘中的一个盘块容纳16bytes，而一个关键字2bytes，一个关键字具体信息指针2bytes。一棵9阶B-tree(一个结点最多8个关键字)的内部结点需要2个盘快。而B+ 树内部结点只需要1个盘快。当需要把内部结点读入内存中的时候，B 树就比B+ 树多一次盘块查找时间(在磁盘中就是盘片旋转的时间)。

2) B+-tree的查询效率更加稳定

由于非终结点并不是最终指向文件内容的结点，而只是叶子结点中关键字的索引。所以任何关键字的查找必须走一条从根结点到叶子结点的路。所有关键字查询的路径长度相同，导致每一个数据的查询效率相当。

读者点评

本文评论下第149楼，fanyy1991针对上文所说的两点，道：个人觉得这两个原因都不是主要原因。数据库索引采用B+树的主要原因是 B树在提高了磁盘IO性能的同时并没有解决元素遍历的效率低下的问题。正是为了解决这个问题，B+树应运而生。B+树只要遍历叶子节点就可以实现整棵树的遍历。而且在数据库中基于范围的查询是非常频繁的，而B树不支持这样的操作（或者说效率太低）。

b) B+-tree的应用: VSAM(虚拟存储存取法)文件(来源论文 the ubiquitous Btree 作者：D COMER - 1979 )





”

B*-tree是B+-tree的变体，在B+树的基础上(所有的叶子结点中包含了全部关键字的信息，及指向含有这些关键字记录的指针)，B*树中非根和非叶子结点再增加指向兄弟的指针；B*树定义了非叶子结点关键字个数至少为(2/3)*M，即块的最低使用率为2/3（代替B+树的1/2）。


B+树的分裂：当一个结点满时，分配一个新的结点，并将原结点中1/2的数据复制到新结点，最后在父结点中增加新结点的指针；B+树的分裂只影响原结点和父结点，而不会影响兄弟结点，所以它不需要指向兄弟的指针。

B*树的分裂：当一个结点满时，如果它的下一个兄弟结点未满，那么将一部分数据移到兄弟结点中，再在原结点插入关键字，最后修改父结点中兄弟结点的关键字（因为兄弟结点的关键字范围改变了）；如果兄弟也满了，则在原结点与兄弟结点之间增加新结点，并各复制1/3的数据到新结点，最后在父结点增加新结点的指针。

所以，B*树分配新结点的概率比B+树要低，空间使用率更高；



















