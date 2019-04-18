# 剑指offer - 树的子结构 - curryche的博客 - CSDN博客





2018年08月18日 20:31:34[curryche](https://me.csdn.net/whwan11)阅读数：39标签：[剑指offer																[c++](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)
个人分类：[笔试](https://blog.csdn.net/whwan11/article/category/7789918)








## 题目

树的子结构 

时间限制：1秒 空间限制：32768K 热度指数：292466

题目描述 

输入两棵二叉树A，B，判断B是不是A的子结构。（ps：我们约定空树不是任意一个树的子结构）

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
    bool HasSubtree(TreeNode* pRoot1, TreeNode* pRoot2)
    {
        if(pRoot1==NULL||pRoot2==NULL)
            return false;
        bool result;
        if(sub_tree_from_root(pRoot1,pRoot2))
            result=true;
        else 
        {
            result=HasSubtree(pRoot1->left,pRoot2);
            if(!result)
                result=HasSubtree(pRoot1->right,pRoot2);
        }
        return result;
    }

    bool sub_tree_from_root(TreeNode* pRoot1, TreeNode* pRoot2)
    {
        if(pRoot2==NULL)
            return true;
        if(pRoot1==NULL)
            return false;

        if(pRoot1->val==pRoot2->val)
        {
            return sub_tree_from_root(pRoot1->left,pRoot2->left)&&
                   sub_tree_from_root(pRoot1->right,pRoot2->right);
        }
        else
            return false;

    }
};
```




