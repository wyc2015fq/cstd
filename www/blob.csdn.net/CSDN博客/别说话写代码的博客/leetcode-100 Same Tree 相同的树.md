# leetcode-100. Same Tree 相同的树 - 别说话写代码的博客 - CSDN博客





2019年01月24日 14:03:18[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：25标签：[Same Tree																[相同的树																[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=相同的树&t=blog)





|Given two binary trees, write a function to check if they are the same or not.Two binary trees are considered the same if they are structurally identical and the nodes have the same value.**Example 1:****Input:**     1         1          / \       / \         2   3     2   3        [1,2,3],   [1,2,3]**Output:** true**Example 2:****Input:**     1         1          /           \         2             2        [1,2],     [1,null,2]**Output:** false**Example 3:****Input:**     1         1          / \       / \         2   1     1   2        [1,2,1],   [1,1,2]**Output:** false|给定两个二叉树，编写一个函数来检验它们是否相同。如果两个树在结构上相同，并且节点具有相同的值，则认为它们是相同的。**示例 1:****输入: **      1         1          / \       / \         2   3     2   3        [1,2,3],   [1,2,3]**输出:** true**示例 2:****输入:  **    1          1          /           \         2             2        [1,2],     [1,null,2]**输出:** false**示例 3:****输入:**       1         1          / \       / \         2   1     1   2        [1,2,1],   [1,1,2]**输出:** false|
|----|----|

思路：递归实现， 注意：递归判断里 只写需要返回的一般情况。

```cpp
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
    bool isSameTree(TreeNode* p, TreeNode* q) {
        if(!p && !q) return true;
        if(!p && q || !q && p) return false;
        if(p->val != q->val) return false;  //如果不满足不用再写返回true，
        return isSameTree(p->left,q->left) && isSameTree(p->right,q->right);
    }
};
```](https://so.csdn.net/so/search/s.do?q=Same Tree&t=blog)




