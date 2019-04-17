# [LeetCode] Count Complete Tree Nodes 求完全二叉树的节点个数 - Grandyang - 博客园







# [[LeetCode] Count Complete Tree Nodes 求完全二叉树的节点个数](https://www.cnblogs.com/grandyang/p/4567827.html)







Given a **complete** binary tree, count the number of nodes.

**Definition of a complete binary tree from [Wikipedia](http://en.wikipedia.org/wiki/Binary_tree#Types_of_binary_trees):**

In a complete binary tree every level, except possibly the last, is 
completely filled, and all nodes in the last level are as far left as 
possible. It can have between 1 and 2h nodes inclusive at the last level h.



这道题给定了一棵完全二叉树，让我们求其节点的个数。很多人分不清完全二叉树和满二叉树的区别，下面让我们来看看维基百科上对二者的定义：

[完全二叉树 (Complete Binary Tree)](https://zh.wikipedia.org/wiki/%E4%BA%8C%E5%8F%89%E6%A0%91)：

A Complete Binary Tree （CBT) is a binary tree in which every level, except possibly the last, is completely filled, and all nodes are as far left as possible.

对于一颗二叉树，假设其深度为d（d>1）。除了第d层外，其它各层的节点数目均已达最大值，且第d层所有节点从左向右连续地紧密排列，这样的二叉树被称为完全二叉树；

换句话说，完全二叉树从根结点到倒数第二层满足完美二叉树，最后一层可以不完全填充，其叶子结点都靠左对齐。

[完美二叉树 (Perfect Binary Tree)](https://zh.wikipedia.org/wiki/%E4%BA%8C%E5%8F%89%E6%A0%91)：

A Perfect Binary Tree(PBT) is a tree with all leaf nodes at the same depth. All internal nodes have degree 2.

二叉树的第i层至多拥有![2^{i-1}](https://wikimedia.org/api/rest_v1/media/math/render/svg/de838b503259acc792dd682654445984ea6e4c6d)个节点数；深度为k的二叉树至多总共有![{\displaystyle 2^{\begin{aligned}k+1\end{aligned}}-1}](https://wikimedia.org/api/rest_v1/media/math/render/svg/f24729d4eae59094b7ed114e09dcbf142f32cde8)个节点数，而总计拥有节点数匹配的，称为“满二叉树”；

[完满二叉树 (Full Binary Tree):](http://www.cnblogs.com/idorax/p/6441043.html)

A Full Binary Tree (FBT) is a tree in which every node other than the leaves has two children.

换句话说，所有非叶子结点的度都是2。（只要你有孩子，你就必然是有两个孩子。）

![Image result for 满二叉树](data:image/png;base64,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)


[500 × 200 - zy.swust.net.cn](http://www.google.com/imgres?imgurl=http://zy.swust.net.cn/02/2/sjjg/images/chap06/C6P3.gif&imgrefurl=http://zy.swust.net.cn/02/2/sjjg/page/chap06/060201.asp&h=200&w=500&tbnid=ASz3gK5eek0jvM:&zoom=1&docid=u6zq2P7Qe63VCM&ei=JmR4VfHKOMPWsAWwn4LICw&tbm=isch&ved=0CCEQMygEMARqFQoTCPH1y7HEhcYCFUMrrAodsI8AuQ)




通过上面的定义，我们可以看出二者的关系是，完美二叉树一定是完全二叉树，而完全二叉树不一定是完美二叉树。那么这道题给的完全二叉树就有可能是完美二叉树，若是完美二叉树，节点个数很好求，为2的h次方-1，h为该完美二叉树的高度。这道题可以用递归和非递归两种方法来解。我们先来看递归的方法，思路是分别找出以当前节点为根节点的左子树和右子树的高度并对比，如果相等，则说明是满二叉树，直接返回节点个数，如果不相等，则节点个数为左子树的节点个数加上右子树的节点个数再加1(根节点)，其中左右子树节点个数的计算可以使用递归来计算，参见代码如下：



```
class Solution {
public:
    int countNodes(TreeNode* root) {
        int hLeft = 0, hRight = 0;
        TreeNode *pLeft = root, *pRight = root;
        while (pLeft) {
            ++hLeft;
            pLeft = pLeft->left;
        }
        while (pRight) {
            ++hRight;
            pRight = pRight->right;
        }
        if (hLeft == hRight) return pow(2, hLeft) - 1;
        return countNodes(root->left) + countNodes(root->right) + 1;
    }
};
```



递归的解法还有一种解法如下所示：



```
class Solution {
public:
    int countNodes(TreeNode* root) {
        int hLeft = leftHeight(root);
        int hRight = rightHeight(root);
        if (hLeft == hRight) return pow(2, hLeft) - 1;
        return countNodes(root->left) + countNodes(root->right) + 1;
    }
    int leftHeight(TreeNode* root) {
        if (!root) return 0;
        return 1 + leftHeight(root->left);
    }
    int rightHeight(TreeNode* root) {
        if (!root) return 0;
        return 1 + rightHeight(root->right);
    }
};
```



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












