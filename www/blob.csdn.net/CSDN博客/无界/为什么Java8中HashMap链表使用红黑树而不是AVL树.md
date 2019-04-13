
# 为什么Java8中HashMap链表使用红黑树而不是AVL树 - 无界 - CSDN博客

置顶2019年04月01日 01:51:46[21aspnet](https://me.csdn.net/21aspnet)阅读数：368


在Jdk1.8版本后，Java对HashMap做了改进，在链表长度大于8的时候，将后面的数据存在红黑树中，以加快检索速度。
那么很多人就有疑问为什么是使用红黑树而不是AVL树，AVL树是完全平衡二叉树阿？
*最主要的一点是：*
## 在CurrentHashMap中是加锁了的，实际上是读写锁，如果写冲突就会等待，
## 如果插入时间过长必然等待时间更长，而红黑树相对AVL树他的插入更快！
**第一个问题****为什么不一直使用树？**
参考《[为什么HashMap包含LinkedList而不是AVL树？](https://stackoverflow.com/questions/42422469/why-does-a-hashmap-contain-a-linkedlist-instead-of-an-avl-tree)》
我想这是内存占用与存储桶内查找复杂性之间的权衡。请记住，大多数哈希函数将产生非常少的冲突，因此为大小为3或4的桶维护树将是非常昂贵的，没有充分的理由。
作为参考，这是一个HashMap的Java 8 impl（它实际上有一个很好的解释，整个事情如何工作，以及为什么他们选择8和6，作为“TREEIFY”和“UNTREEIFY”阈值）
**第二个问题为什么hash冲突使用红黑树而不是AVL树呢**
参考：[AVL树和红黑树之间有什么区别？](https://www.quora.com/Whats-the-difference-between-AVL-trees-and-Red-Black-trees)
![](https://img-blog.csdnimg.cn/20190401020750843.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190401020804925.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
# [红黑树和AVL树之间的区别](https://stackoverflow.com/questions/16257761/difference-between-red-black-trees-and-avl-trees)
AVL树比红黑树保持更加严格的平衡。AVL树中从根到最深叶的路径最多为~1.44 lg（n + 2），而在红黑树中最多为~2 lg（n + 1）。
因此，在AVL树中查找通常更快，但这是以更多旋转操作导致更慢的插入和删除为代价的。因此，如果您希望查找次数主导树的更新次数，请使用AVL树。
AVL以及RedBlack树是高度平衡的树数据结构。它们非常相似，真正的区别在于在任何添加/删除操作时完成的旋转操作次数。
两种实现都缩放为a`O(lg N)`，其中N是叶子的数量，但实际上AVL树在查找密集型任务上更快：利用更好的平衡，树遍历平均更短。另一方面，插入和删除方面，AVL树速度较慢：需要更高的旋转次数才能在修改时正确地重新平衡数据结构。
对于通用实现（即先验并不清楚查找是否是操作的主要部分），RedBlack树是首选：它们更容易实现，并且在常见情况下更快 - 无论数据结构如何经常被搜索修改。一个例子，`TreeMap`而`TreeSet`在Java中使用一个支持RedBlack树。
**对于小数据**：
**insert**：RB tree＆avl tree具有恒定的最大旋转次数，但RB树会更快，因为平均RB树使用较少的旋转。
**查找**：AVL树更快，因为AVL树的深度较小。
**删除**：RB树具有恒定的最大旋转次数，但AVL树可以将O（log N）次旋转视为最差。并且平均而言，RB树也具有较少的旋转次数，因此RB树更快。
**对于大数据**：
**insert**：AVL树更快。因为您需要在插入之前查找特定节点。当您有更多数据时，查找特定节点的时间差异与O（log N）成比例增长。但在最坏的情况下，AVL树和RB树仍然只需要恒定的旋转次数。因此，瓶颈将成为您查找该特定节点的时间。
**查找**：AVL树更快。（与小数据情况相同）
**删除**：AVL树平均速度更快，但在最坏的情况下，RB树更快。因为您还需要在删除之前查找非常深的节点以进行交换（类似于插入的原因）。平均而言，两棵树都有恒定的旋转次数。但RB树有一个恒定的旋转上限。
--------------
参考：[AVL树与红黑树？](https://discuss.fogcreek.com/joelonsoftware/default.asp?cmd=show&ixPost=22948)
在AVL树中，从根到任何叶子的最短路径和最长路径之间的差异最多为1。在红黑树中，差异可以是2倍。
这两个都给O（log n）查找，但平衡AVL树可能需要O（log n）旋转，而红黑树将需要最多两次旋转使其达到平衡（尽管可能需要检查O（log n）节点以确定旋转的位置）。旋转本身是O（1）操作，因为你只是移动指针。
=========================
这里可以动态演示红黑树和AVL树以及其他数据结构和算法，强烈推荐：
[https://www.cs.usfca.edu/~galles/visualization/Algorithms.html](https://www.cs.usfca.edu/~galles/visualization/Algorithms.html)
[https://www.cs.usfca.edu/~galles/visualization/RedBlack.html](https://www.cs.usfca.edu/~galles/visualization/RedBlack.html)
![](https://img-blog.csdnimg.cn/2019040101544551.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
[https://www.cs.usfca.edu/~galles/visualization/AVLtree.html](https://www.cs.usfca.edu/~galles/visualization/AVLtree.html)
![](https://img-blog.csdnimg.cn/20190401015600168.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)

