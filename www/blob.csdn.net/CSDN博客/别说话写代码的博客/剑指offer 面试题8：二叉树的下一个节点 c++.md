# 剑指offer 面试题8：二叉树的下一个节点 c++ - 别说话写代码的博客 - CSDN博客





2018年11月20日 12:47:25[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：42
所属专栏：[剑指offer刷题c++/python](https://blog.csdn.net/column/details/29881.html)









题目：给定一个二叉树和其中的一个结点，请找出中序遍历顺序的下一个结点并且返回。注意，树中的结点不仅包含左右子结点，同时包含指向父结点的指针。

解析：三种情况：1.若节点有右子树，中序下一个就是 右子树最左孩子节点  2.若无右子树，且当前节点是父节点的左子节点，下一个就是父节点   3.无右子树，且当前节点是父节点的右子节点，那下一个就是 父节点的父节点，一直往上找， 一直找到根，说明当时节点是树的最右节点，中序 遍历无下一个节点。写法上第2中和第3中可以合并一起写

```cpp
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
        if(!pNode) return nullptr;
        if(pNode->right)
        {
            TreeLinkNode* tmp=pNode->right;
            while(tmp->left) tmp=tmp->left;
            return tmp;
        }else if(pNode->next)
        {
            TreeLinkNode* tmp=pNode->next; //父节点
            TreeLinkNode* cur=pNode;
            while(tmp && tmp->right == cur)  //找当前节点是父节点的左子节点
            {
                tmp = tmp->next;
                cur=cur->next;
            }
            return tmp;  
        }return nullptr;
    }
};
```

[https://www.nowcoder.com/profile/9695725/codeBookDetail?submissionId=37384915](https://www.nowcoder.com/profile/9695725/codeBookDetail?submissionId=37384915)



