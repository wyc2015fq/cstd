# [LeetCode] Lowest Common Ancestor of a Binary Search Tree 二叉搜索树的最小共同父节点 - Grandyang - 博客园







# [[LeetCode] Lowest Common Ancestor of a Binary Search Tree 二叉搜索树的最小共同父节点](https://www.cnblogs.com/grandyang/p/4640572.html)







Given a binary search tree (BST), find the lowest common ancestor (LCA) of two given nodes in the BST.

According to the [definition of LCA on Wikipedia](https://en.wikipedia.org/wiki/Lowest_common_ancestor): “The lowest common ancestor is defined between two nodes p and q as the lowest node in T that has both p and q as descendants (where we allow a node to be a descendant of itself).”

Given binary search tree:  root = [6,2,8,0,4,7,9,null,null,3,5]

![](https://assets.leetcode.com/uploads/2018/12/14/binarysearchtree_improved.png)



Example 1:
Input: root = [6,2,8,0,4,7,9,null,null,3,5], p = 2, q = 8
Output: 6
Explanation: The LCA of nodes `2` and `8` is `6`.

Example 2:
Input: root = [6,2,8,0,4,7,9,null,null,3,5], p = 2, q = 4
Output: 2
Explanation: The LCA of nodes `2` and `4` is `2`, since a node can be a descendant of itself according to the LCA definition.



Note:
- All of the nodes' values will be unique.
- p and q are different and both values will exist in the BST.



这道题让我们求二叉搜索树的最小共同父节点, LeetCode中关于BST的题有 [Validate Binary Search Tree](http://www.cnblogs.com/grandyang/p/4298435.html)， [Recover Binary Search Tree](http://www.cnblogs.com/grandyang/p/4298069.html)， [Binary Search Tree Iterator](http://www.cnblogs.com/grandyang/p/4231455.html)， [Unique Binary Search Trees](http://www.cnblogs.com/grandyang/p/4299608.html)， [Unique Binary Search Trees II](http://www.cnblogs.com/grandyang/p/4301096.html)，[Convert Sorted Array to Binary Search Tree](http://www.cnblogs.com/grandyang/p/4295245.html) , [Convert Sorted List to Binary Search Tree](http://www.cnblogs.com/grandyang/p/4295618.html) 和 [Kth Smallest Element in a BST](http://www.cnblogs.com/grandyang/p/4620012.html)。这道题我们可以用递归来求解，我们首先来看题目中给的例子，由于二叉搜索树的特点是左<根<右，所以根节点的值一直都是中间值，大于左子树的所有节点值，小于右子树的所有节点值，那么我们可以做如下的判断，如果根节点的值大于p和q之间的较大值，说明p和q都在左子树中，那么此时我们就进入根节点的左子节点继续递归，如果根节点小于p和q之间的较小值，说明p和q都在右子树中，那么此时我们就进入根节点的右子节点继续递归，如果都不是，则说明当前根节点就是最小共同父节点，直接返回即可，参见代码如下：



解法一：

```
class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        if (!root) return NULL;
        if (root->val > max(p->val, q->val)) 
            return lowestCommonAncestor(root->left, p, q);
        else if (root->val < min(p->val, q->val)) 
            return lowestCommonAncestor(root->right, p, q);
        else return root;
    }
};
```



当然，此题也有非递归的写法，用个while循环来代替递归调用即可，然后不停的更新当前的根节点，也能实现同样的效果，代码如下：



解法二：

```
class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        while (true) {
            if (root->val > max(p->val, q->val)) root = root->left;
            else if (root->val < min(p->val, q->val)) root = root->right;
            else break;
        }      
        return root;
    }
};
```



类似题目：

[Lowest Common Ancestor of a Binary Tree](http://www.cnblogs.com/grandyang/p/4641968.html)



参考资料：

[https://leetcode.com/problems/lowest-common-ancestor-of-a-binary-search-tree/](https://leetcode.com/problems/lowest-common-ancestor-of-a-binary-search-tree/)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












