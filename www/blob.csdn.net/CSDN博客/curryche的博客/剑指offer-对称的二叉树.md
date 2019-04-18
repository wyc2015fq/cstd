# 剑指offer - 对称的二叉树 - curryche的博客 - CSDN博客





2018年08月21日 09:41:46[curryche](https://me.csdn.net/whwan11)阅读数：48








## 题目

对称的二叉树 

时间限制：1秒 空间限制：32768K 热度指数：93183

题目描述 

请实现一个函数，用来判断一颗二叉树是不是对称的。注意，如果一个二叉树同此二叉树的镜像是同样的，定义其为对称的。

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
};
*/
class Solution {
public:
    bool isSymmetrical(TreeNode* pRoot)
    {
        if(pRoot==NULL)
            return true;

        return isSymRecursive(pRoot->left,pRoot->right);

    }

    bool isSymRecursive(TreeNode* pRoot1,TreeNode* pRoot2)
    {
        if(pRoot1==NULL&&pRoot2==NULL)
            return true;
        if(pRoot1==NULL||pRoot2==NULL)
            return false;

        if(pRoot1->val==pRoot2->val)
        {
            return isSymRecursive(pRoot1->left,pRoot2->right)&&
                   isSymRecursive(pRoot1->right,pRoot2->left);
        }
        else
            return false;

    }

};
```



