# leetcode-104. Maximum Depth of Binary Tree 二叉树的最大深度 - 别说话写代码的博客 - CSDN博客





2019年01月25日 12:37:40[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：24标签：[二叉树的最大深度																[Maximum Depth of Binary Tree																[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=Maximum Depth of Binary Tree&t=blog)



|Given a binary tree, find its maximum depth.The maximum depth is the number of nodes along the longest path from the root node down to the farthest leaf node.**Note:** A leaf is a node with no children.**Example:**Given binary tree `[3,9,20,null,null,15,7]`,    3   / \  9  20    /  \   15   7return its depth = 3.|给定一个二叉树，找出其最大深度。二叉树的深度为根节点到最远叶子节点的最长路径上的节点数。**说明:** 叶子节点是指没有子节点的节点。**示例：**			给定二叉树 `[3,9,20,null,null,15,7]`，    3   / \  9  20    /  \   15   7返回它的最大深度 3 。|
|----|----|

思路：

**第一种**：dfs，递归

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
    int maxDepth(TreeNode* root) {
        if(!root) return 0;
        return max(maxDepth(root->left),maxDepth(root->right))+1;
    }
};
```

**第二种**：层序遍历二叉树，然后统计层数

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
    int maxDepth(TreeNode* root) {
        if(!root) return 0;
        queue<TreeNode*> q;
        q.push(root);
        int res=0;
        while(!q.empty())
        {
            res++;
            for(int i=q.size();i>0;--i)
            {
                TreeNode *p = q.front();
                q.pop();
                if(p->left) q.push(p->left);
                if(p->right) q.push(p->right);
            }
        }return res;
    }
};
```](https://so.csdn.net/so/search/s.do?q=二叉树的最大深度&t=blog)




