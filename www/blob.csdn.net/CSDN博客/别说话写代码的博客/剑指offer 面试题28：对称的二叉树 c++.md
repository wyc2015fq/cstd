# 剑指offer 面试题28：对称的二叉树 c++ - 别说话写代码的博客 - CSDN博客





2018年11月19日 16:55:28[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：21标签：[剑指offer																[面试题28																[对称的二叉树																[二叉树的镜像](https://so.csdn.net/so/search/s.do?q=二叉树的镜像&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[剑指offer刷题c++/python](https://blog.csdn.net/column/details/29881.html)](https://so.csdn.net/so/search/s.do?q=对称的二叉树&t=blog)




题目：请实现一个函数，用来判断一颗二叉树是不是对称的。注意，如果一个二叉树同此二叉树的镜像是同样的，定义其为对称的。（要想象一下他的结构）

思路：递归判断，左子树的左子树和右子树的右子树比较，左子树的右子树和右子树的左子树比较，若相等则是，否则不是。

```cpp
class Solution {
public:
    bool isSym(TreeNode* left,TreeNode *right)
    {
        if(right==nullptr && left==nullptr) return true;
        if(right==nullptr || left==nullptr) return false;
        return isSym(left->left,right->right) &&isSym(left->right,right->left)
            && (left->val == right->val);
    }
    bool isSymmetrical(TreeNode* pRoot)
    {
        if (pRoot==nullptr) return true;
        return isSym(pRoot->left,pRoot->right);
    }

};
```](https://so.csdn.net/so/search/s.do?q=面试题28&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)




