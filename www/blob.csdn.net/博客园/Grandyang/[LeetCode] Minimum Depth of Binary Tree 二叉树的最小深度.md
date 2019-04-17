# [LeetCode] Minimum Depth of Binary Tree 二叉树的最小深度 - Grandyang - 博客园







# [[LeetCode] Minimum Depth of Binary Tree 二叉树的最小深度](https://www.cnblogs.com/grandyang/p/4042168.html)







Given a binary tree, find its minimum depth.

The minimum depth is the number of nodes along the shortest path from the root node down to the nearest leaf node.

Note: A leaf is a node with no children.

Example:

Given binary tree `[3,9,20,null,null,15,7]`,
    3
   / \
  9  20
    /  \
   15   7
return its minimum depth = 2.



二叉树的经典问题之最小深度问题就是就最短路径的节点个数，还是用深度优先搜索DFS来完成，万能的递归啊。首先判空，若当前结点不存在，直接返回0。然后看若左子结点不存在，那么对右子结点调用递归函数，并加1返回。反之，若右子结点不存在，那么对左子结点调用递归函数，并加1返回。若左右子结点都存在，则分别对左右子结点调用递归函数，将二者中的较小值加1返回即可，参见代码如下：



解法一：

```
class Solution {
public:
    int minDepth(TreeNode* root) {
        if (!root) return 0;
        if (!root->left) return 1 + minDepth(root->right);
        if (!root->right) return 1 + minDepth(root->left);
        return 1 + min(minDepth(root->left), minDepth(root->right));
    }
};
```



我们也可以是迭代来做，层序遍历，记录遍历的层数，一旦我们遍历到第一个叶结点，就将当前层数返回，即为二叉树的最小深度，参见代码如下：



解法二：

```
class Solution {
public:
    int minDepth(TreeNode* root) {
        if (!root) return 0;
        int res = 0;
        queue<TreeNode*> q{{root}};
        while (!q.empty()) {
            ++res;
            for (int i = q.size(); i > 0; --i) {
                auto t = q.front(); q.pop();
                if (!t->left && !t->right) return res;
                if (t->left) q.push(t->left);
                if (t->right) q.push(t->right);
            }
        }
        return -1;
    }
};
```





类似题目：

[Binary Tree Level Order Traversal](http://www.cnblogs.com/grandyang/p/4051321.html)

[Maximum Depth of Binary Tree](http://www.cnblogs.com/grandyang/p/4051348.html)



参考资料：

[https://leetcode.com/problems/minimum-depth-of-binary-tree/](https://leetcode.com/problems/minimum-depth-of-binary-tree/)

[https://leetcode.com/problems/minimum-depth-of-binary-tree/discuss/36153/My-concise-c%2B%2B-solution](https://leetcode.com/problems/minimum-depth-of-binary-tree/discuss/36153/My-concise-c%2B%2B-solution)

[https://leetcode.com/problems/minimum-depth-of-binary-tree/discuss/36071/BFS-C%2B%2B-8ms-Beats-99.94-submissions](https://leetcode.com/problems/minimum-depth-of-binary-tree/discuss/36071/BFS-C%2B%2B-8ms-Beats-99.94-submissions)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












