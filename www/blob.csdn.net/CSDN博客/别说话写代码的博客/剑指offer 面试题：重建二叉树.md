# 剑指offer 面试题：重建二叉树 - 别说话写代码的博客 - CSDN博客





2018年12月29日 22:42:32[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：36
所属专栏：[剑指offer刷题c++/python](https://blog.csdn.net/column/details/29881.html)









题目：输入某二叉树的前序遍历和中序遍历的结果，请重建出该二叉树。假设输入的前序遍历和中序遍历的结果中都不含重复的数字。例如输入前序遍历序列{1,2,4,7,3,5,6,8}和中序遍历序列{4,7,2,1,5,3,8,6}，则重建二叉树并返回。

思路：二叉树先序是根左右，中序 是左根右。所以先找到二叉树的根节点，然后中序遍历根节点左边就是左子树，右边就是右子树 （补充中序遍历与其他任何一种遍历都能唯一构造一颗二叉树）

下面代码用递归写得，注意先序的范围（中序遍历根节点左边的节点一定都紧跟在 先序遍历根节点后面，比如例子中中序遍历根节点1左子树3个节点，在前序遍历中一定紧跟在前序遍历后面，所以有i-inb+preb）和中序的范围

```cpp
/**
 * Definition for binary tree
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    TreeNode* restructtree(vector<int> pre,int preb,int pree,vector<int>
                          in,int inb,int ine)
    {
        if(preb>pree || inb>ine) return nullptr;
        TreeNode *head=new TreeNode(pre[preb]);
        int i;
        for(i=inb;i<=ine;++i)
            if(head->val == in[i])
                break;
        //下面i-inb表示根节点左子树有多少个节点
        head->left=restructtree(pre,preb+1,preb+i-inb,in,inb,i-1);
        head->right=restructtree(pre,preb+i-inb+1,pree,in,i+1,ine);
        return head;
    }
    TreeNode* reConstructBinaryTree(vector<int> pre,vector<int> vin) {
        return restructtree(pre,0,pre.size()-1,vin,0,vin.size()-1);
    }
};
```





