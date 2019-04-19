# Tarjan’s off-line lowest common ancestors algorithm - 电影旅行敲代码 - CSDN博客
2017年11月26日 18:29:51[电影旅行敲代码](https://me.csdn.net/dashuniuniu)阅读数：289
## 并查集
关于最近公共父节点（lowest common ancestors algorithm，LCA）问题，我暂时只了解了Tarjan的离线方法，这里做个记录，Tarjan基于并查集实现（我的理解看来，Tarjan方法的思想本质不是并查集，而在于树的遍历方式）。
并查集是在线等价类问题[6]，
> 
在线等价类问题中，初始时有 *n* 个元素，每个元素都属于一个独立的等价类。需要执行以下的操作：
1） *combine(a, b)* 操作，把包含 *a* 和 *b* 的等价类合并成一个独立的等价类。
2）*find(theElement)*，确定元素 *theElement*在哪一个类，目的是对给定的两个元素，确定是否是同一个类。
并查集从字面意思上看就是合并查找集合，并查集是“**没有最好的方法，只有合适的方法**”最好的体现，并查集操作对象是集合，除了维护某个元素属于某个集合之外不需要维护元素之间的其他关系。并查集的实现有两种[7]，*quick-find* 以及 *quick-union*，较为常用的方法是基于 *quick-union* ，而 *quick-union* 有两个维度的改进，“**union by rank**” 以及 “**path compression**”，该方法称作 **weighted quick-union with path compression**[7][8]，感兴趣的可以查看并查集的可视化实现[9]。
基于Tarjan的LCA实现基于并查集实现，那么在Tarjan的LCA实现中哪里需要 *union* 操作，哪里需要 *find* 操作？
在解释这个问题之前，先介绍一下Tarjan’s LCA的主要思想。Tarjan’s LCA方法需要通过DFS实现，如果要获得 *P* 和 *Q* 两个节点的LCA，在DFS遍历过程中（假如先遍历到 *P* 节点，再遍历到 *Q* ，并且是中序遍历），那么在遍历完 *Q* 时，**两者的LCA就是 *P* 节点沿着 *parent* 域一直向上直到第一个未遍历完成的节点**。如图-2所示，*root* 为节点7， *P* 节点为 5，*Q*节点为6，我们沿着 *root* 节点通过中序DFS遍历该树，在遍历完 *Q* （序号6）节点时，沿着 *P* 节点 *parent* 域向上寻找，找到第一个未遍历完的节点就是节点9，节点9就是节点5和节点6的LCA节点。
![DFS of LCA](https://img-blog.csdn.net/20171126155102638?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZGFzaHVuaXVuaXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
 图-2  Inorder DFS of Tarjan's LCA
从上图中我们可以看出，{5，0，11}构成一个等价类，等价关系是**它们沿着parent域向上寻找的第一个未完成的节点相同**，它们的未完成节点是9。同理节点{4，6}构成一个等价类，它们的未完成节点是3。当节点3遍历完成后，3首先与{4，6}合并，因为它们的未完成节点是9，然后{4，6，3}要与{5，0，11}，也就是进行 *union* 操作，整个过程如图-3所示。最后{5，0，11，9，3，4，8}构成一个等价类，它们的共同的未完成节点为7。
![Tarjan-union](https://img-blog.csdn.net/20171126160638012?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZGFzaHVuaXVuaXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
 图-3  Union of Tarjan's LCA
Tarjan’s LCA中的 *union* 操作，是在处理完一个节点时，将其与已经处理过的其它同一层的子节点所代表的等价类进行合并。当然上面的描述有些绕口，主要原因在于我们的实现方式不是很自然。更加自然的方法是，**每次处理完一个子节点时，将该节点所属的等价类与它的父节点进行 *union* 操作**。这样的方法如图-4所示。
- 每次访问到新的节点，创建其“等价类”集合。并将其设置为该集合的代表节点。
- 访问完一个节点时，使用 *find* 操作查找其“等价类”集合，也就是找到该“等价类”集合的代表节点。
- 将该节点的“等价类”集合与其父节点的“等价类”集合进行 *union*。设置新集合的代表节点。
![Tarjan's LCA1](https://img-blog.csdn.net/20171126174657990?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZGFzaHVuaXVuaXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![Tarjan's LCA2](https://img-blog.csdn.net/20171126174507643?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZGFzaHVuaXVuaXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![Tarjan's LCA3](https://img-blog.csdn.net/20171126175208631?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZGFzaHVuaXVuaXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
 图-4  Tarjan's LCA 
*注：粉色椭圆表示一个“等价类”集合。图中的棕色节点表示其对应集合的代表节点。图中绿色为等价集合-并查集森林*
*union* 操作用于在访问完一个节点以后，将该节点代表的“等价类”集合与其父节的“等价类”集合进行合并。*find* 操作主要用于查找某个节点所在的“等价类”集合。
## Tarjan’s off-line lowest common ancestors algorithm
Tarjan’s LCA方法在算法导论中有一些介绍，见[21-3 Tarjan’s off-line least-common-ancestors algorithm]，该算法如图-5所示。该算法由Make-Set，Union，Find-Set三个方法组成，该方法的完整实现见Tarjan’s off-line lowest common ancestors algorithm[10]，该实现通过path compression和union by rank进行了优化。
![这里写图片描述](https://img-blog.csdn.net/20171126181815422?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZGFzaHVuaXVuaXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
 图-5  Tarjan's LCA of CLRS
图-5算法是递归实现，但是如果树高很大的话，有可能会爆栈，leetcode-236是关于LCA的一道题，其中有对爆栈的测试，所以最好将该递归实现改为循环。
**future blog：**
- *LCA的其它算法*
- *递归改循环的实现*
[1]. [Is there any overhead to declaring a variable within a loop?](https://stackoverflow.com/questions/982963/is-there-any-overhead-to-declaring-a-variable-within-a-loop-c)
[2]. [What is the difference between declaring a variable outside of loop and declaring static inside loop?](https://arduino.stackexchange.com/questions/533/what-is-the-difference-between-declaring-a-variable-outside-of-loop-and-declarin)
[3]. [Is it better to declare a repeatedly-used variable inside a loop or outside?](https://www.quora.com/Is-it-better-to-declare-a-repeatedly-used-variable-inside-a-loop-or-outside)
[4]. [Least common ancestor of multiple nodes in DAG?](https://stackoverflow.com/questions/11449534/least-common-ancestor-of-multiple-nodes-in-dag)
[5]. [如何在 DAG 中找多个点的 LCA ?](https://www.zhihu.com/question/46440863)
[6]. 萨尼. 数据结构, 算法与应用: C++ 语言描述[M]. 机械工业出版社, 2015.
[7]. [Union-Find Algorithms](https://www.cs.princeton.edu/~rs/AlgsDS07/01UnionFind.pdf)
[8]. [Disjoint-set Data Structures](https://www.topcoder.com/community/data-science/data-science-tutorials/disjoint-set-data-structures/)
[9]. [UNION-FIND DISJOINT SETS (UFDS)](https://visualgo.net/en/ufds)
[10]. [Tarjan’s off-line lowest common ancestors algorithm](http://www.geeksforgeeks.org/tarjans-off-line-lowest-common-ancestors-algorithm/)
[11]. [236. Lowest Common Ancestor of a Binary Tree](https://leetcode.com/problems/lowest-common-ancestor-of-a-binary-tree/description/)
