# [LeetCode] Binary Tree Level Order Traversal 二叉树层序遍历 - Grandyang - 博客园







# [[LeetCode] Binary Tree Level Order Traversal 二叉树层序遍历](https://www.cnblogs.com/grandyang/p/4051321.html)







Given a binary tree, return the *level order* traversal of its nodes' values. (ie, from left to right, level by level).

For example:
Given binary tree `{3,9,20,#,#,15,7}`,
    3
   / \
  9  20
    /  \
   15   7



return its level order traversal as:
[
  [3],
  [9,20],
  [15,7]
]


层序遍历二叉树是典型的广度优先搜索BFS的应用，但是这里稍微复杂一点的是，我们要把各个层的数分开，存到一个二维向量里面，大体思路还是基本相同的，建立一个queue，然后先把根节点放进去，这时候找根节点的左右两个子节点，这时候去掉根节点，此时queue里的元素就是下一层的所有节点，用一个for循环遍历它们，然后存到一个一维向量里，遍历完之后再把这个一维向量存到二维向量里，以此类推，可以完成层序遍历。代码如下：



解法一：

```
class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        if (!root) return {};
        vector<vector<int>> res;
        queue<TreeNode*> q{{root}};
        while (!q.empty()) {
            vector<int> oneLevel;
            for (int i = q.size(); i > 0; --i) {
                TreeNode *t = q.front(); q.pop();
                oneLevel.push_back(t->val);
                if (t->left) q.push(t->left);
                if (t->right) q.push(t->right);
            }
            res.push_back(oneLevel);
        }
        return res;
    }
};
```



下面我们来看递归的写法，核心就在于我们需要一个二维数组，和一个变量level，关于level的作用可以参见博主的另一篇博客 [Binary Tree Level Order Traversal II](http://www.cnblogs.com/grandyang/p/4051326.html) 中的讲解，参见代码如下：



解法二：

```
class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> res;
        levelorder(root, 0, res);
        return res;
    }
    void levelorder(TreeNode* node, int level, vector<vector<int>>& res) {
        if (!node) return;
        if (res.size() == level) res.push_back({});
        res[level].push_back(node->val);
        if (node->left) levelorder(node->left, level + 1, res);
        if (node->right) levelorder(node->right, level + 1, res);
    }
};
```



类似题目：

[Binary Tree Level Order Traversal II](http://www.cnblogs.com/grandyang/p/4051326.html)

[Binary Tree Zigzag Level Order Traversal](http://www.cnblogs.com/grandyang/p/4297009.html)

[Minimum Depth of Binary Tree](http://www.cnblogs.com/grandyang/p/4042168.html)

[Binary Tree Vertical Order Traversal](http://www.cnblogs.com/grandyang/p/5278930.html)

[Average of Levels in Binary Tree](http://www.cnblogs.com/grandyang/p/7259209.html)

[N-ary Tree Level Order Traversal](https://www.cnblogs.com/grandyang/p/9672233.html)



参考资料：

[https://leetcode.com/problems/binary-tree-level-order-traversal/](https://leetcode.com/problems/binary-tree-level-order-traversal/)

[https://leetcode.com/problems/binary-tree-level-order-traversal/discuss/33445/Java-Solution-using-DFS](https://leetcode.com/problems/binary-tree-level-order-traversal/discuss/33445/Java-Solution-using-DFS)

[https://leetcode.com/problems/binary-tree-level-order-traversal/discuss/33450/Java-solution-with-a-queue-used](https://leetcode.com/problems/binary-tree-level-order-traversal/discuss/33450/Java-solution-with-a-queue-used)

[https://leetcode.com/problems/binary-tree-level-order-traversal/discuss/114449/A-general-approach-to-level-order-traversal-questions-in-Java](https://leetcode.com/problems/binary-tree-level-order-traversal/discuss/114449/A-general-approach-to-level-order-traversal-questions-in-Java)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












