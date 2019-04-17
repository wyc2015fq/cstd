# [CareerCup] 4.6 Find Next Node in a BST 寻找二叉搜索树中下一个节点 - Grandyang - 博客园







# [[CareerCup] 4.6 Find Next Node in a BST 寻找二叉搜索树中下一个节点](https://www.cnblogs.com/grandyang/p/4709487.html)







4.6 Write an algorithm to find the'next'node (i.e., in-order successor) of a given node in a binary search tree. You may assume that each node has a link to its parent.



这道题实际上考察的是一种叫[线索二叉树](https://zh.wikipedia.org/wiki/%E7%BA%BF%E7%B4%A2%E4%BA%8C%E5%8F%89%E6%A0%91)的数据结构，而构造这种树的方法称之为Morris遍历法，在我之前的博客[Binary Tree Inorder Traversal 二叉树的中序遍历](http://www.cnblogs.com/grandyang/p/4297300.html)有详细的介绍，然而并没什么卵用，因为这道题给了个条件，说每个节点都可以链接到其父节点，这样一来就大大的简化了问题。首先我们知道二叉搜索树的性质的左<=根<右，那么其中序遍历就是一个递增的有序数列，那么我们如何找到一个节点的下一个节点呢。思路是这样的，首先我们判断输入节点是否为空，若为空直接返回NULL，若不为空，在看其右子节点是否存在，存在的话找到右子树中最左的左子节点返回。如果右子节点不存在，则说明该点的子树全遍历完了，就要往其父节点上去找未被遍历完全的节点，参见代码如下：



```
class Solution {
public:
    TreeNode* inorderSucc(TreeNode *n) {
        if (!n) return NULL;
        if (n->right) {
            TreeNode *p = n->right;
            while (p->left) p = p->left;
            return p;
        } else {
            TreeNode *q = n, *x = q->parent;
            while (x && x->left != q) {
                q = x;
                x = x->parent;
            }
            return x;
        }
    }
};
```














