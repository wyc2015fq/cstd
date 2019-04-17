# [CareerCup] 4.9 All Paths Sum 所有路径和 - Grandyang - 博客园







# [[CareerCup] 4.9 All Paths Sum 所有路径和](https://www.cnblogs.com/grandyang/p/4714640.html)







4.9 You are given a binary tree in which each node contains a value. Design an algorithm to print all paths which sum to a given value. The path does not need to start or end at the root or a leaf.



这道题给我们一个二叉树，让我们找出所有的路径，其和为给定的值，而且说了路径不必起始于根，终止于叶节点，但必须是向下的一条路径。LeetCode中相似的题有[Path Sum 二叉树的路径和](http://www.cnblogs.com/grandyang/p/4036961.html) 和 [Path Sum II 二叉树路径之和之二](http://www.cnblogs.com/grandyang/p/4042156.html)。但是那题要找的是起始于根，终止于叶节点的路径，而这题是找出所有的路径。所以要稍稍复杂一些。这题的解题思路是先求出给定二叉树的深度，关于求二叉树的深度可以参见我之前的博客[Maximum Depth of Binary Tree 二叉树的最大深度](http://www.cnblogs.com/grandyang/p/4051348.html)。然后我们建立一个大小为树的最大深度的一维向量，用来存每一层路径上的值。然后从第一层开始递归，对每一个节点，更新当前层的path，然后从此层向第一层遍历，将path各层值加起来，如果等于sum的话，就把这道路径打印或者保存起来，然后在对当前节点的左右子节点分别递归调用。时间复杂度为O(nlgn)，空间复杂度为O(lgn)，参见代码如下：



解法一：

```
class Solution {
public:
    vector<vector<int> > pathSum(TreeNode *root, int sum) {
        if (!root) return vector<vector<int> >();
        int depth = getDepth(root);
        vector<vector<int> > res;
        vector<int> path(depth, INT_MIN);
        pathSumDFS(root, sum, 0, path, res);
        return res;
    }
    void pathSumDFS(TreeNode *root, int sum, int level, vector<int> &path, vector<vector<int> > &res) {
        if (!root) return;
        path[level] = root->val;
        int t = 0;
        for (int i = level; i >= 0; i--) {
            t += path[i];
            if (t == sum) {
                savePath(path, i, level, res);
            }
        }
        pathSumDFS(root->left, sum, level + 1, path, res);
        pathSumDFS(root->right, sum, level + 1, path, res);
        path[level] = INT_MIN;
    }
    void savePath(vector<int> &path, int start, int end, vector<vector<int> > &res) {
        vector<int> out;
        for (int i = start; i <= end; ++i) {
            out.push_back(path[i]);
        }
        res.push_back(out);
    }
    int getDepth(TreeNode *root) {
        if (!root) return 0;
        return 1 + max(getDepth(root->left), getDepth(root->right));
    }
};
```



然而书上的解法并不是最简洁的，下面这种方法是评论区的网友提出来的，感觉很简洁很好，赞一个～



解法二：

```
class Solution {
public:
    vector<vector<int> > pathSum(TreeNode *root, int sum) {
        if (!root) return {};
        vector<vector<int>> res;
        vector<int> path;
        helper(root, sum, path, res);
        return res;
    }
    void helper(TreeNode* node, int sum, vector<int>& path, vector<vector<int>>& res) {
        if (!node) return;
        path.push_back(node->val);
        int curSum = 0;
        for (int i = path.size() - 1; i >= 0; --i) {
            curSum += path[i];
            if (curSum == sum) res.push_back(vector<int>(path.begin() + i, path.end()));
        }
        helper(node->left, sum, path, res);
        helper(node->right, sum, path, res);
        path.pop_back();
    }
};
```














