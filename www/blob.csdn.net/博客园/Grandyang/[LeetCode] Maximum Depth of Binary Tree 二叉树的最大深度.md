# [LeetCode] Maximum Depth of Binary Tree 二叉树的最大深度 - Grandyang - 博客园







# [[LeetCode] Maximum Depth of Binary Tree 二叉树的最大深度](https://www.cnblogs.com/grandyang/p/4051348.html)







Given a binary tree, find its maximum depth.

The maximum depth is the number of nodes along the longest path from the root node down to the farthest leaf node.

Note: A leaf is a node with no children.

Example:

Given binary tree `[3,9,20,null,null,15,7]`,
    3
   / \
  9  20
    /  \
   15   7
return its depth = 3.



求二叉树的最大深度问题用到深度优先搜索DFS，递归的完美应用，跟求二叉树的最小深度问题原理相同。代码如下：



C++ 解法一：

```
class Solution {
public:
    int maxDepth(TreeNode* root) {
        if (!root) return 0;
        return 1 + max(maxDepth(root->left), maxDepth(root->right));
    }
};
```



Java 解法一：

```
public class Solution {
    public int maxDepth(TreeNode root) {
        return root == null ? 0 : (1 + Math.max(maxDepth(root.left), maxDepth(root.right)));
    }
}
```



我们也可以使用层序遍历二叉树，然后计数总层数，即为二叉树的最大深度，参见代码如下：



C++ 解法二：

```
class Solution {
public:
    int maxDepth(TreeNode* root) {
        if (!root) return 0;
        int res = 0;
        queue<TreeNode*> q{{root}};
        while (!q.empty()) {
            ++res;
            for (int i = q.size(); i > 0; --i) {
                TreeNode *t = q.front(); q.pop();
                if (t->left) q.push(t->left);
                if (t->right) q.push(t->right);
            }
        }
        return res;
    }
};
```



Java 解法二：

```
public class Solution {
    public int maxDepth(TreeNode root) {
        if (root == null) return 0;
        int res = 0;
        Queue<TreeNode> q = new LinkedList<>();
        q.offer(root);
        while (!q.isEmpty()) {
            ++res;
            for (int i = q.size(); i > 0; --i) {
                TreeNode t = q.poll();
                if (t.left != null) q.offer(t.left);
                if (t.right != null) q.offer(t.right);
            }
        }
        return res;
    }
}
```



类似题目：

[Balanced Binary Tree](http://www.cnblogs.com/grandyang/p/4045660.html)

[Minimum Depth of Binary Tree](http://www.cnblogs.com/grandyang/p/4042168.html)

[Maximum Depth of N-ary Tree](https://www.cnblogs.com/grandyang/p/9873496.html)



参考资料：

[https://leetcode.com/problems/maximum-depth-of-binary-tree/](https://leetcode.com/problems/maximum-depth-of-binary-tree/)

[https://leetcode.com/problems/maximum-depth-of-binary-tree/discuss/34207/my-code-of-c-depth-first-search-and-breadth-first-search](https://leetcode.com/problems/maximum-depth-of-binary-tree/discuss/34207/my-code-of-c-depth-first-search-and-breadth-first-search)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












