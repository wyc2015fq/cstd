# leetcode-106. Construct Binary Tree from Inorder and Postorder Traversal 从中序与后序遍历序列构造二叉树 - 别说话写代码的博客 - CSDN博客





2019年01月25日 13:47:50[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：33标签：[Construct Binary Tree from Ino																[从中序与后序遍历序列构造二叉树																[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=从中序与后序遍历序列构造二叉树&t=blog)



|Given inorder and postorder traversal of a tree, construct the binary tree.**Note:**			You may assume that duplicates do not exist in the tree.For example, giveninorder = [9,3,15,20,7]postorder = [9,15,7,20,3]Return the following binary tree:    3   / \  9  20    /  \   15   7|根据一棵树的中序遍历与后序遍历构造二叉树。**注意:**			你可以假设树中没有重复的元素。例如，给出中序遍历 inorder = [9,3,15,20,7]后序遍历 postorder = [9,15,7,20,3]返回如下的二叉树：    3   / \  9  20    /  \   15   7|
|----|----|

思路：如[上一题](https://blog.csdn.net/qq_21997625/article/details/86644516)，中序遍历与其他遍历（先序后序层序）都能唯一构造一颗二叉树！后序 遍历的最后一个节点是根节点，在中序遍历中找到根节点，然后中序根左边就是左子树，右边就是右子树。递归执行就oK

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
    TreeNode* buildTree(vector<int>& inorder, vector<int>& postorder) {
        return buildTree(inorder,0,inorder.size()-1,postorder,0,postorder.size()-1);
    }
    TreeNode* buildTree(vector<int>& inorder,int inleft,int inright,
                        vector<int>& postorder,int postleft,int postright)
    {
        if(inleft>inright || postleft>postright) return NULL;
        int i=0;
        for(i=inleft;i<=inright;++i)  //在中序遍历中找到根节点
            if(postorder[postright]==inorder[i]) 
                break;
        TreeNode *node = new TreeNode(postorder[postright]);  //新建一个节点挂上
        //注意后序遍历的位置，i-inleft表示左子节点个数，比如左子树只有一个节点i-inleft=1,但是postleft+1就相当于有了两个节点，所以要-1
        node->left=buildTree(inorder,inleft,i-1,postorder,postleft,postleft+i-inleft-1);
        //右子树注意后序遍历的位置，后序postleft好理解，postright就是去掉最后一个根节点的位置
        node->right=buildTree(inorder,i+1,inright,postorder,postleft+i-inleft,postright-1);
        return node;
    }
};
```](https://so.csdn.net/so/search/s.do?q=Construct Binary Tree from Ino&t=blog)




