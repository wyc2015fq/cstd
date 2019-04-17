# [LintCode] Invert Binary Tree 翻转二叉树 - Grandyang - 博客园







# [[LintCode] Invert Binary Tree 翻转二叉树](https://www.cnblogs.com/grandyang/p/4790476.html)







Given *n* points on a 2D plane, find the maximum number of points that lie on the same straight line.

**Example**

Given 4 points: `(1,2)`, `(3,6)`, `(0,0)`, `(1,3)`.

The maximum number is `3`.





LeeCode上的原题，可参见我之前的博客[Invert Binary Tree 翻转二叉树](http://www.cnblogs.com/grandyang/p/4572877.html)。



解法一：

```
// Recursion
class Solution {
public:
    /**
     * @param root: a TreeNode, the root of the binary tree
     * @return: nothing
     */
    void invertBinaryTree(TreeNode *root) {
        if (!root) return;
        TreeNode *tmp = root->left;
        root->left = root->right;
        root->right = tmp;
        invertBinaryTree(root->left);
        invertBinaryTree(root->right);
    }
};
```



解法二：

```
// Non-Recursion
class Solution {
public:
    /**
     * @param root: a TreeNode, the root of the binary tree
     * @return: nothing
     */
    void invertBinaryTree(TreeNode *root) {
        if (!root) return;
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            TreeNode* node = q.front(); q.pop();
            TreeNode *tmp = node->left;
            node->left = node->right;
            node->right = tmp;
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
    }
};
```














