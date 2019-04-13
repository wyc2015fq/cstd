
# BST树即二叉搜索树 - luzhensmart的专栏 - CSDN博客


2019年03月16日 23:01:01[芦震](https://me.csdn.net/luzhensmart)阅读数：8


**[BST](https://www.baidu.com/s?wd=BST&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)****树**
即二叉搜索树：
1.所有非叶子结点至多拥有两个儿子（Left和Right）；
2.所有结点存储一个关键字；
3.非叶子结点的左指针指向小于其关键字的子树，右指针指向大于其关键字的子树；
如：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/manesking/1.JPG)
BST树的搜索，从根结点开始，如果查询的关键字与结点的关键字相等，那么就命中；
否则，如果查询关键字比结点关键字小，就进入左儿子；如果比结点关键字大，就进入
右儿子；如果左儿子或右儿子的指针为空，则报告找不到相应的关键字；
如果BST树的所有非叶子结点的左右子树的结点数目均保持差不多（平衡），那么B树
的搜索性能逼近二分查找；但它比连续内存空间的二分查找的优点是，改变BST树结构
（插入与删除结点）不需要移动大段的内存数据，甚至通常是常数开销；
如：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/manesking/2.JPG)
**但BST树在经过多次插入与删除后，有可能导致不同的结构：**
![](https://p-blog.csdn.net/images/p_blog_csdn_net/manesking/31.JPG)
右边也是一个BST树，但它的搜索性能已经是线性的了；同样的关键字集合有可能导致不同的
树结构索引；所以，使用BST树还要考虑尽可能让BST树保持左图的结构，和避免右图的结构，也就
是所谓的“平衡”问题；


