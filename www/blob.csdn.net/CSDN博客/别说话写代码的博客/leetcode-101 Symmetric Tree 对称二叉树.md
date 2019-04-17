# leetcode-101. Symmetric Tree 对称二叉树 - 别说话写代码的博客 - CSDN博客





2019年01月24日 13:55:45[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：26标签：[对称二叉树																[Symmetric Tree																[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=Symmetric Tree&t=blog)



|Given a binary tree, check whether it is a mirror of itself (ie, symmetric around its center).For example, this binary tree `[1,2,2,3,4,4,3]` is symmetric:    1   / \  2   2 / \ / \3  4 4  3But the following `[1,2,2,null,3,null,3]` is not:    1   / \  2   2   \   \   3    3**Note:**			Bonus points if you could solve it both recursively and iteratively.|给定一个二叉树，检查它是否是镜像对称的。例如，二叉树 `[1,2,2,3,4,4,3]` 是对称的。    1   / \  2   2 / \ / \3  4 4  3但是下面这个 `[1,2,2,null,3,null,3]` 则不是镜像对称的:    1   / \  2   2   \   \   3    3**说明:**如果你可以运用递归和迭代两种方法解决这个问题，会很加分。|
|----|----|

思路：

**方法一**：递归，左子树的左子节点和右子树的右子节点相比，左子树的右子节点和右子树的左子节点比

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
    bool isSymmetric(TreeNode* root) {
        if(!root) return true;
        return issym(root->left,root->right);
    }
    bool issym(TreeNode* left,TreeNode *right)
    {
        if(!left && !right) return true;
        if(!left && right || left && !right || left->val!=right->val)
            return false;
        return issym(left->left,right->right) && issym(left->right,right->left);
    }
};
```

**方法二**：使用两个队列，一个 从左往右放 根节点左子树，一个从右往左放右子树节点。注意：非递归一般在循环里判断符合的条件或者不符合的条件

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
    bool isSymmetric(TreeNode* root) {
        if(!root) return true;
        queue<TreeNode*> q1,q2;
        q1.push(root->left);
        q2.push(root->right);
        while(!q1.empty() && !q2.empty())
        {
            TreeNode* p1=q1.front();   q1.pop();
            TreeNode* p2=q2.front();   q2.pop();
            if(!p1 && !p2) continue;
            if(p1 && !p2 || !p1 && p2) return false;
            if(p1->val != p2->val) return false;
            q1.push(p1->left);
            q1.push(p1->right);
            q2.push(p2->right);
            q2.push(p2->left);
        }
        return true;
    }
};
```](https://so.csdn.net/so/search/s.do?q=对称二叉树&t=blog)




