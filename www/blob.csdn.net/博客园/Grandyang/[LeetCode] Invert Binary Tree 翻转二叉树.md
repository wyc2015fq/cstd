# [LeetCode] Invert Binary Tree 翻转二叉树 - Grandyang - 博客园







# [[LeetCode] Invert Binary Tree 翻转二叉树](https://www.cnblogs.com/grandyang/p/4572877.html)







Invert a binary tree.
     4
   /   \
  2     7
 / \   / \
1   3 6   9
to
     4
   /   \
  7     2
 / \   / \
9   6 3   1
**Trivia:**

This problem was inspired by [this original tweet](https://twitter.com/mxcl/status/608682016205344768) by [Max Howell](https://twitter.com/mxcl):

> Google: 90% of our engineers use the software you wrote 
(Homebrew), but you can’t invert a binary tree on a whiteboard so fuck 
off.



这道题让我们翻转二叉树，是树的基本操作之一，不算难题。最下面那句话实在有些木有节操啊，不知道是Google说给谁的。反正这道题确实难度不大，可以用递归和非递归两种方法来解。先来看递归的方法，写法非常简洁，五行代码搞定，交换当前左右节点，并直接调用递归即可，代码如下：



```
// Recursion
class Solution {
public:
    TreeNode* invertTree(TreeNode* root) {
        if (!root) return NULL;
        TreeNode *tmp = root->left;
        root->left = invertTree(root->right);
        root->right = invertTree(tmp);
        return root;
    }
};
```



非递归的方法也不复杂，跟二叉树的层序遍历一样，需要用queue来辅助，先把根节点排入队列中，然后从队中取出来，交换其左右节点，如果存在则分别将左右节点在排入队列中，以此类推直到队列中木有节点了停止循环，返回root即可。代码如下：



```
// Non-Recursion
class Solution {
public:
    TreeNode* invertTree(TreeNode* root) {
        if (!root) return NULL;
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            TreeNode *node = q.front(); q.pop();
            TreeNode *tmp = node->left;
            node->left = node->right;
            node->right = tmp;
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        return root;
    }
};
```



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












