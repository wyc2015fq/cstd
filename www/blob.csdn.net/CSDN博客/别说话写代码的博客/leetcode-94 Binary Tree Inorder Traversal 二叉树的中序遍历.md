# leetcode-94. Binary Tree Inorder Traversal 二叉树的中序遍历 - 别说话写代码的博客 - CSDN博客





2019年01月24日 11:40:58[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：24标签：[二叉树的中序遍历																[Binary Tree Inorder Traversal																[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=Binary Tree Inorder Traversal&t=blog)



|Given a binary tree, return the *inorder* traversal of its nodes' values.**Example:****Input:** [1,null,2,3]   1    \     2    /   3**Output:** [1,3,2]**Follow up:** Recursive solution is trivial, could you do it iteratively?|给定一个二叉树，返回它的*中序 *遍历。**示例:****输入:** [1,null,2,3]   1    \     2    /   3**输出:** [1,3,2]**进阶:** 递归算法很简单，你可以通过迭代算法完成吗？|
|----|----|

思路：

**第一种**：当然就是递归，但是很慢

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
    vector<int> inorderTraversal(TreeNode* root) {
        if(!root) return {};
        vector<int> vec;
        inorder(root,vec);
        return vec;
    }
    void inorder(TreeNode* root,vector<int> & vec)
    {
        if(!root) return ;
        inorder(root->left,vec);
        vec.push_back(root->val);
        inorder(root->right,vec);
    }
};
```

**第二种**：非递归，题目要求。 非递归就是用栈一个个将元素放进去，然后 读出来

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
    vector<int> inorderTraversal(TreeNode* root) {
        if(!root) return {};
        stack<TreeNode*> s;
        vector<int> vec;
        TreeNode* p = root;
        while(p || !s.empty())
        {
            if(p)
            {
                s.push(p);
                p=p->left;
            }else
            {
                p = s.top();
                s.pop();
                vec.push_back(p->val);
                p=p->right;
            }
        }
        return vec;
    }
};
```](https://so.csdn.net/so/search/s.do?q=二叉树的中序遍历&t=blog)




