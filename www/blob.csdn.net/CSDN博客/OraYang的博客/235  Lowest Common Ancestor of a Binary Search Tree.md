
# 235. Lowest Common Ancestor of a Binary Search Tree - OraYang的博客 - CSDN博客

2017年08月02日 09:28:52[OraYang](https://me.csdn.net/u010665216)阅读数：123标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given a binary search tree (BST), find the lowest common ancestor (LCA) of two given nodes in the BST.
According to the[definition of LCA on Wikipedia](https://en.wikipedia.org/wiki/Lowest_common_ancestor): “The lowest common ancestor is defined between two
 nodes v and w as the lowest node in T that has both v and w as descendants (where we allowa node to be a descendant of itself).”
_______6______
       /              \
    ___2__          ___8__
   /      \        /      \
   0      _4       7       9
         /  \
         3   5For example, the lowest common ancestor (LCA) of nodes`2`and`8`is`6`.
 Another example is LCA of nodes`2`and`4`is`2`,
 since a node can be a descendant of itself according to the LCA definition.
思路：本题题目出得花里胡哨的，其实这类BTS的题目都是毫无意义的，只要抓住递归的本质，首先做这类与数据结构密切相关的题目时，一定要熟悉BTS的性质：
（1）若左子树不空，则左子树上所有结点的值均小于或等于它的[根结](https://baike.baidu.com/item/%E6%A0%B9%E7%BB%93)点的值；
（2）若右子树不空，则右子树上所有结点的值均大于或等于它的根结点的值；
（3）左、右子树也分别为二叉排序树；
代码：

```python
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        if(p->val<root->val&&q->val<root->val)
            return lowestCommonAncestor(root->left,p,q);
        else if(p->val>root->val&&q->val>root->val)
            return lowestCommonAncestor(root->right,p,q);
        else
            return root;
        
    }
};
```


