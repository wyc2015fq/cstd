# 剑指offer - 二叉树的深度 - curryche的博客 - CSDN博客





2018年08月21日 16:47:36[curryche](https://me.csdn.net/whwan11)阅读数：28标签：[剑指offer																[c++](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)
个人分类：[笔试](https://blog.csdn.net/whwan11/article/category/7789918)








## 题目

二叉树的深度

时间限制：1秒 空间限制：32768K 热度指数：100782

题目描述 

输入一棵二叉树，求该树的深度。从根结点到叶结点依次经过的结点（含根、叶结点）形成树的一条路径，最长路径的长度为树的深度。

## 解法

代码

```
/*
struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
    TreeNode(int x) :
            val(x), left(NULL), right(NULL) {
    }
};*/
class Solution {
public:
    int TreeDepth(TreeNode* pRoot)
    {
        if(pRoot==NULL)
            return 0;
        else
        {
            return max(TreeDepth(pRoot->left),TreeDepth(pRoot->right))+1;
        }

    }
    int max_num(int a,int b)
    {
        return a>b?a:b;
    }
};
```




