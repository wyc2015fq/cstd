# 剑指Offer - 二叉树的下一个结点 - curryche的博客 - CSDN博客





2018年08月01日 11:16:09[curryche](https://me.csdn.net/whwan11)阅读数：33








## 题目

二叉树的下一个结点 

时间限制：1秒 空间限制：32768K 热度指数：91716

题目描述 

给定一个二叉树和其中的一个结点，请找出中序遍历顺序的下一个结点并且返回。注意，树中的结点不仅包含左右子结点，同时包含指向父结点的指针。

## 解法

时间有限，先只放代码，日后有时间再写下解题思路 

代码

```
/*
struct TreeLinkNode {
    int val;
    struct TreeLinkNode *left;
    struct TreeLinkNode *right;
    struct TreeLinkNode *next;
    TreeLinkNode(int x) :val(x), left(NULL), right(NULL), next(NULL) {

    }
};
*/
class Solution {
public:
    TreeLinkNode* GetNext(TreeLinkNode* pNode)
    {
        if(!pNode)
            return NULL;

        TreeLinkNode* i;
        i=pNode->right;

        if(i)
        {
            while(i)
            {
                if(i->left)
                    i=i->left;
                else
                    break;
            }
            return i;
        }         
        else
        {
            TreeLinkNode* current;
            current=pNode;
            i=pNode->next;
            while(i)
            {
                if(current==i->left)
                    return i;
                else
                {
                    current=i;
                    i=current->next;
                }
            }
            return NULL;
        }

    }
};
```



