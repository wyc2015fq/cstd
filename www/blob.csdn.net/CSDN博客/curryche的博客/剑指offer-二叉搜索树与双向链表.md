# 剑指offer - 二叉搜索树与双向链表 - curryche的博客 - CSDN博客





2018年08月31日 15:58:35[curryche](https://me.csdn.net/whwan11)阅读数：21标签：[剑指offer																[c++](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)
个人分类：[笔试](https://blog.csdn.net/whwan11/article/category/7789918)








## 题目

二叉搜索树与双向链表 

时间限制：1秒 空间限制：32768K 热度指数：166414

题目描述 

输入一棵二叉搜索树，将该二叉搜索树转换成一个排序的双向链表。要求不能创建任何新的结点，只能调整树中结点指针的指向。

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
    TreeNode* Convert(TreeNode* pRootOfTree)
    {
        if(pRootOfTree==NULL)
            return (TreeNode*)NULL;
        TreeNode* newlast=NULL;
        TreeNode* newhead=NULL;
        convert_node(pRootOfTree,&newlast);
        newhead=newlast;
        while(newhead!=NULL&&newhead->left!=NULL)
            newhead=newhead->left;
        return newhead; 
    }
    void convert_node(TreeNode* root,TreeNode** newlast)
    {
        if(root==NULL)
            return;
        if(root->left!=NULL)
            convert_node(root->left,newlast);

        root->left=*newlast;

        if(*newlast!=NULL)
            (*newlast)->right=root;

        *newlast=root;

        if(root->right!=NULL)
            convert_node(root->right,newlast);
    }
};
```




