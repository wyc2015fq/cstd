
# 100. Same Tree - OraYang的博客 - CSDN博客

2017年07月18日 10:50:52[OraYang](https://me.csdn.net/u010665216)阅读数：117标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given two binary trees, write a function to check if they are equal or not.
Two binary trees are considered equal if they are structurally identical and the nodes have the same value.
思路：本题主要考察的是二叉树的遍历，核心实现就是递归，但是要判断退出递归的条件，一共有三个：
1、两棵树对应值不相等
2、一棵树遍历完，另一棵还没遍历完
3、两棵树同时遍历完
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
    bool isSameTree(TreeNode* p, TreeNode* q) {
        if(p==NULL&&q==NULL)
            return true;
        else if(p==NULL&&q!=NULL)
            return false;
        else if(p!=NULL&&q==NULL)
            return false;
        else
        {
            if(p->val!=q->val)
                return false;
            else
                return isSameTree(p->left,q->left)&&isSameTree(p->right,q->right);
        }
        
    }
};
```


