# 剑指offer - 从上往下打印二叉树 - curryche的博客 - CSDN博客





2018年08月21日 14:55:27[curryche](https://me.csdn.net/whwan11)阅读数：45








## 题目

从上往下打印二叉树 

时间限制：1秒 空间限制：32768K 热度指数：228450

题目描述 

从上往下打印出二叉树的每个节点，同层节点从左至右打印。

## 解法

代码

```cpp
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
    vector<int> PrintFromTopToBottom(TreeNode* root) {
        vector<int> result;
        if(root==NULL)
            return result;

        queue<TreeNode*> nodes;
        TreeNode* current=NULL;

        nodes.push(root);
        while(!nodes.empty())
        {
            current=nodes.front();
            nodes.pop();
            result.push_back(current->val);
            if(current->left!=NULL)
                nodes.push(current->left);
            if(current->right!=NULL)
                nodes.push(current->right);

        }
        return result;

    }
};
```



