# leetcode-102. Binary Tree Level Order Traversal 二叉树的层次遍历 - 别说话写代码的博客 - CSDN博客





2019年01月24日 14:42:09[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：28标签：[Binary Tree Level Order Traver																[二叉树的层次遍历																[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=二叉树的层次遍历&t=blog)





|Given a binary tree, return the *level order* traversal of its nodes' values. (ie, from left to right, level by level).For example:			Given binary tree `[3,9,20,null,null,15,7]`,    3   / \  9  20    /  \   15   7return its level order traversal as:[  [3],  [9,20],  [15,7]]|给定一个二叉树，返回其按层次遍历的节点值。 （即逐层地，从左到右访问所有节点）。例如:			给定二叉树: `[3,9,20,null,null,15,7]`,    3   / \  9  20    /  \   15   7返回其层次遍历结果：[  [3],  [9,20],  [15,7]]|
|----|----|

思路：

**第一种**：非递归，使用队列就可以了。这里唯一需要注意的就是每一层要放入返回的vector，如何控制每一层的个数呢？可以使用循环，先获取当前队列元素个数，然后逐渐减，这样能控制得到的是每一层的元素。 

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
    vector<vector<int>> levelOrder(TreeNode* root) {
        if(!root) return {};
        vector<vector<int>> res;
        queue<TreeNode*> q;
        q.push(root);
        while(!q.empty())
        {
            vector<int> vec;
            for(int i=q.size();i>0;--i)
            {
                TreeNode * t =q.front(); 
                q.pop();
                vec.push_back(t->val);
                if(t->left) q.push(t->left);
                if(t->right) q.push(t->right);
            }
            res.push_back(vec);
        }return res;
    }
};
```

**第二种**：递归写法，level的作用就是确定当前是第几层。

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
    vector<vector<int>> levelOrder(TreeNode* root) {
        if(!root)  return {};
        vector<vector<int>> res;
        levelord(root,0,res);
        return res;
    }
    void levelord(TreeNode* root,int level,vector<vector<int>> & res)
    {
        if(!root) return;
        if(level == res.size()) res.push_back({});
        res[level].push_back(root->val);
        levelord(root->left,level+1,res);
        levelord(root->right,level+1,res);
    }
};
```](https://so.csdn.net/so/search/s.do?q=Binary Tree Level Order Traver&t=blog)




