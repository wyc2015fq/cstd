# 数据结构和算法系列 - AVL树 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2015年06月30日 14:51:49[initphp](https://me.csdn.net/initphp)阅读数：955








### 概念

AVL树是最早提出的自平衡二叉树，在AVL树中任何节点的两个子树的高度最大差别为一，所以它也被称为高度平衡树。AVL树得名于它的发明者G.M. Adelson-Velsky和E.M. Landis。AVL树种查找、插入和删除在平均和最坏情况下都是O（log n），增加和删除可能需要通过一次或多次树旋转来重新平衡这个树。本文介绍了AVL树的设计思想和基本操作。




### 基本术语



有四种种情况可能导致二叉查找树不平衡，分别为：

（1）LL：插入一个新节点到根节点的左子树（Left）的左子树（Left），导致根节点的平衡因子由1变为2

（2）RR：插入一个新节点到根节点的右子树（Right）的右子树（Right），导致根节点的平衡因子由-1变为-2

（3）LR：插入一个新节点到根节点的左子树（Left）的右子树（Right），导致根节点的平衡因子由1变为2

（4）RL：插入一个新节点到根节点的右子树（Right）的左子树（Left），导致根节点的平衡因子由-1变为-2

针对四种种情况可能导致的不平衡，可以通过旋转使之变平衡。有两种基本的旋转：

（1）左旋转：将根节点旋转到（根节点的）右孩子的左孩子位置

（2）右旋转：将根节点旋转到（根节点的）左孩子的右孩子位置




### 旋转

**1. LL型**

平衡二叉树某一节点的左孩子的左子树上插入一个新的节点，使得该节点不再平衡。这时只需要把树向右旋转一次即可，如图所示，原A的左孩子B变为父结点，A变为其右孩子，而原B的右子树变为A的左子树，注意旋转之后Brh是A的左子树（图上忘在A于Brh之间标实线）

![](https://img-blog.csdn.net/20150630144314687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




**2. RR型**


平衡二叉树某一节点的右孩子的右子树上插入一个新的节点，使得该节点不再平衡。这时只需要把树向左旋转一次即可，如图所示，原A右孩子B变为父结点，A变为其左孩子，而原B的左子树Blh将变为A的右子树。

![](https://img-blog.csdn.net/20150630144815851?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




**3. LR型**

平衡二叉树某一节点的左孩子的右子树上插入一个新的节点，使得该节点不再平衡。这时需要旋转两次，仅一次的旋转是不能够使二叉树再次平衡。如图所示，在B节点按照RR型向左旋转一次之后，二叉树在A节点仍然不能保持平衡，这时还需要再向右旋转一次。

![](https://img-blog.csdn.net/20150630144845821?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




**4. RL型**

平衡二叉树某一节点的右孩子的左子树上插入一个新的节点，使得该节点不再平衡。同样，这时需要旋转两次，旋转方向刚好同LR型相反。

![](https://img-blog.csdn.net/20150630144919920?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




PS：AVL平衡树的代码设计复杂度会比较高，因为插入、删除等操作都需要去检查树是否平衡，如果不平衡就需要进行旋转，在并发设计上的时候，大规模的数据结构的旋转并不是非常合理。而且树越大，需要回溯旋转的节点可能越多。




