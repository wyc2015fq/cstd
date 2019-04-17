# [LeetCode] Binary Tree Level Order Traversal II 二叉树层序遍历之二 - Grandyang - 博客园







# [[LeetCode] Binary Tree Level Order Traversal II 二叉树层序遍历之二](https://www.cnblogs.com/grandyang/p/4051326.html)







Given a binary tree, return the *bottom-up level order* traversal of its nodes' values. (ie, from left to right, level by level from leaf to root).

For example:
Given binary tree `{3,9,20,#,#,15,7}`,
    3
   / \
  9  20
    /  \
   15   7



return its bottom-up level order traversal as:
[
  [15,7],
  [9,20],
  [3]
]


从底部层序遍历其实还是从顶部开始遍历，只不过最后存储的方式有所改变，可以参见我之前的博文 [Binary Tree Level Order Traversal](http://www.cnblogs.com/grandyang/p/4051321.html)， 代码如下：



解法一：

```
class Solution {
public:
    vector<vector<int> > levelOrderBottom(TreeNode* root) {
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
            res.insert(res.begin(), oneLevel);
        }
        return res;
    }
};
```



下面我们来看递归的解法，由于递归的特性，我们会一直深度优先去处理左子结点，那么势必会穿越不同的层，所以当要加入某个结点的时候，我们必须要知道当前的深度，所以使用一个变量level来标记当前的深度，初始化带入0，表示根结点所在的深度。由于需要返回的是一个二维数组res，开始时我们又不知道二叉树的深度，不知道有多少层，所以无法实现申请好二维数组的大小，只有在遍历的过程中不断的增加。那么我们什么时候该申请新的一层了呢，当level等于二维数组的大小的时候，为啥是等于呢，不是说要超过当前的深度么，这是因为level是从0开始的，就好比一个长度为n的数组A，你访问A[n]是会出错的，当level等于数组的长度时，就已经需要新申请一层了，我们新建一个空层，继续往里面加数字，参见代码如下：



解法二：

```
class Solution {
public:
    vector<vector<int>> levelOrderBottom(TreeNode* root) {
        vector<vector<int>> res;
        levelorder(root, 0, res);
        return vector<vector<int>> (res.rbegin(), res.rend());
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

[Average of Levels in Binary Tree](http://www.cnblogs.com/grandyang/p/7259209.html)

[Binary Tree Zigzag Level Order Traversal](http://www.cnblogs.com/grandyang/p/4297009.html)

[Binary Tree Level Order Traversal](http://www.cnblogs.com/grandyang/p/4051321.html)



类似题目：

[https://leetcode.com/problems/binary-tree-level-order-traversal-ii/](https://leetcode.com/problems/binary-tree-level-order-traversal-ii/)

[https://leetcode.com/problems/binary-tree-level-order-traversal-ii/discuss/35089/Java-Solution.-Using-Queue](https://leetcode.com/problems/binary-tree-level-order-traversal-ii/discuss/35089/Java-Solution.-Using-Queue)

[https://leetcode.com/problems/binary-tree-level-order-traversal-ii/discuss/34981/My-DFS-and-BFS-java-solution](https://leetcode.com/problems/binary-tree-level-order-traversal-ii/discuss/34981/My-DFS-and-BFS-java-solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












