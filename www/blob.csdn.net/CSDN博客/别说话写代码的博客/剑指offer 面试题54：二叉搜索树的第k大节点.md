# 剑指offer 面试题54：二叉搜索树的第k大节点 - 别说话写代码的博客 - CSDN博客





2018年11月23日 15:27:13[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：36
所属专栏：[剑指offer刷题c++/python](https://blog.csdn.net/column/details/29881.html)









题目：给定一棵二叉搜索树，请找出其中的第k小的结点。例如， （5，3，7，2，4，6，8）    中，按结点数值大小顺序第三小结点的值为4。

思路：二叉 搜索树的中序遍历是有序的，意思是找第k个数

```cpp
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
    TreeNode* bin(TreeNode *pRoot,int &k)
    {
        TreeNode* target ;
        if(pRoot->left!=nullptr) target = bin(pRoot->left,k);
        if(target == nullptr)
        {
            if(k==1)target = pRoot;
             k--;
        }
        if(target == nullptr && pRoot->right!=nullptr)
            target = bin(pRoot->right,k);
        return target;
    }
    TreeNode* KthNode(TreeNode* pRoot, int k)
    {
        if (pRoot==nullptr || k==0) return nullptr;
        return bin(pRoot,k);
    }
};
```





