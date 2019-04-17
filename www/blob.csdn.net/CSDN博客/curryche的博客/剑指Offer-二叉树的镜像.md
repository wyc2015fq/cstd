# 剑指Offer - 二叉树的镜像 - curryche的博客 - CSDN博客





2018年08月18日 19:11:52[curryche](https://me.csdn.net/whwan11)阅读数：29标签：[剑指offer																[c++](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)
个人分类：[笔试](https://blog.csdn.net/whwan11/article/category/7789918)








## 题目

二叉树的镜像 

时间限制：1秒 空间限制：32768K 热度指数：173401

题目描述 

操作给定的二叉树，将其变换为源二叉树的镜像。 

输入描述: 

二叉树的镜像定义：源二叉树  

            8 

           /  \ 

          6   10 

         / \  / \ 

        5  7 9 11 

        镜像二叉树 

            8 

           /  \ 

          10   6 

         / \  / \ 

        11 9 7  5
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
    void Mirror(TreeNode *pRoot) {
        if(pRoot==NULL)
            return;
        TreeNode* temp=pRoot->left;
        pRoot->left=pRoot->right;
        pRoot->right=temp;
        Mirror(pRoot->left);
        Mirror(pRoot->right);


    }
};
```





