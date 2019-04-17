# leetcode-105. Construct Binary Tree from Preorder and Inorder Traversal 从前序与中序遍历序列构造二叉树 - 别说话写代码的博客 - CSDN博客





2019年01月25日 13:22:37[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：36标签：[从前序与中序遍历序列构造二叉树																[Construct Binary Tree from Pre																[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=Construct Binary Tree from Pre&t=blog)



|Given preorder and inorder traversal of a tree, construct the binary tree.**Note:**			You may assume that duplicates do not exist in the tree.For example, givenpreorder = [3,9,20,15,7]inorder = [9,3,15,20,7]Return the following binary tree:    3   / \  9  20    /  \   15   7|根据一棵树的前序遍历与中序遍历构造二叉树。**注意:**			你可以假设树中没有重复的元素。例如，给出前序遍历 preorder = [3,9,20,15,7]中序遍历 inorder = [9,3,15,20,7]返回如下的二叉树：    3   / \  9  20    /  \   15   7|
|----|----|

思路：中序遍历与其他任何一种遍历（先序后序层序）都能唯一确定一颗 二叉树！先序遍历第一个节点肯定是二叉树的跟节点，在中序遍历中找出跟节点（题目说每个节点唯一）， 中序根节点左边就是左子树节点，右边就是右子树节点。通过这样递归一步步就能找出来

```cpp
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        if(preorder.size()==0 || inorder.size()==0) return NULL;
        return buildTree(preorder,0,preorder.size()-1,inorder,0,inorder.size()-1);
    }
    TreeNode* buildTree(vector<int> & preorder,int preleft,int preright,
                        vector<int>& inorder,int inleft,int inright)
    {
        if(preleft>preright || inleft>inright) return NULL;
        int i=0;
        for(i=inleft;i<=inright;++i)   //在中序遍历中找出根节点位置
            if(preorder[preleft]==inorder[i])
                break;
        TreeNode * node = new TreeNode(preorder[preleft]);  //新建一颗节点挂到树上
       //下面左右子树这里，注意先序的左子树的范围(preleft+1,preleft+i-left),这里i-left表示左子树有几个节点
        node->left = buildTree(preorder,preleft+1,preleft+i-inleft,inorder,inleft,i-1);
        node->right=buildTree(preorder,preleft+i-inleft+1,preright,inorder,i+1,inright);
        return node;
    }
};
```](https://so.csdn.net/so/search/s.do?q=从前序与中序遍历序列构造二叉树&t=blog)




