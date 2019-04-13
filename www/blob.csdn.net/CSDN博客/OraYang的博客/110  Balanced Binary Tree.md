
# 110. Balanced Binary Tree - OraYang的博客 - CSDN博客

2017年07月23日 09:20:07[OraYang](https://me.csdn.net/u010665216)阅读数：134所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given a binary tree, determine if it is height-balanced.
For this problem, a height-balanced binary tree is defined as a binary tree in which the depth of the two subtrees ofeverynode never differ by more than 1.
思路：首先明确高度平衡二叉树的定义，利用递归的思路即左右子节点的高度不超过1
代码：

```python
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
    bool isBalanced(TreeNode* root) {
        return Trueorfalse(root)>=0?true:false;
        
    }
private: 
    static int Trueorfalse(TreeNode* root){
        if(root==NULL)
            return 0;
        int leftDepth = Trueorfalse(root->left);
        int rightDepth = Trueorfalse(root->right);
        if(leftDepth<0||rightDepth<0||abs(leftDepth-rightDepth)>1)
            return -1;
        return max(leftDepth,rightDepth)+1;
    }
};
```


