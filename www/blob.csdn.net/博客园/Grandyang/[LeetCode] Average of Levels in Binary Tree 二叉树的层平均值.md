# [LeetCode] Average of Levels in Binary Tree 二叉树的层平均值 - Grandyang - 博客园







# [[LeetCode] Average of Levels in Binary Tree 二叉树的层平均值](https://www.cnblogs.com/grandyang/p/7259209.html)








Given a non-empty binary tree, return the average value of the nodes on each level in the form of an array.

Example 1:
Input:
    3
   / \
  9  20
    /  \
   15   7
Output: [3, 14.5, 11]
Explanation:
The average value of nodes on level 0 is 3,  on level 1 is 14.5, and on level 2 is 11. Hence return [3, 14.5, 11].



Note:
- The range of node's value is in the range of 32-bit signed integer.



这道题让我们求一个二叉树每层的平均值，那么一看就是要进行层序遍历了，直接上queue啊，如果熟悉层序遍历的方法，那么这题就没有什么难度了，直接将每层的值累计加起来，除以该层的结点个数，存入结果res中即可，参见代码如下：



解法一：

```
class Solution {
public:
    vector<double> averageOfLevels(TreeNode* root) {
        if (!root) return {};
        vector<double> res;
        queue<TreeNode*> q{{root}};
        while (!q.empty()) {
            int n = q.size();
            double sum = 0;
            for (int i = 0; i < n; ++i) {
                TreeNode *t = q.front(); q.pop();
                sum += t->val;
                if (t->left) q.push(t->left);
                if (t->right) q.push(t->right);
            }
            res.push_back(sum / n);
        }
        return res;
    }
};
```



下面这种方法虽然是利用的递归形式的先序遍历，但是其根据判断当前层数level跟结果res中已经初始化的层数之间的关系对比，能把当前结点值累计到正确的位置，而且该层的结点数也自增1，这样我们分别求了两个数组，一个数组保存了每行的所有结点值，另一个保存了每行结点的个数，这样对应位相除就是我们要求的结果了，参见代码如下：



解法二：

```
class Solution {
public:
    vector<double> averageOfLevels(TreeNode* root) {
        vector<double> res, cnt;
        helper(root, 0, cnt, res);
        for (int i = 0; i < res.size(); ++i) {
            res[i] /= cnt[i];
        }
        return res;
    }
    void helper(TreeNode* node, int level, vector<double>& cnt, vector<double>& res) {
        if (!node) return;
        if (res.size() <= level) {
            res.push_back(0);
            cnt.push_back(0);
        }
        res[level] += node->val;
        ++cnt[level];
        helper(node->left, level + 1, cnt, res);
        helper(node->right, level + 1, cnt, res);
    }
};
```



类似题目：

[Binary Tree Level Order Traversal II](http://www.cnblogs.com/grandyang/p/4051326.html)

[Binary Tree Level Order Traversal](http://www.cnblogs.com/grandyang/p/4051321.html)



参考资料：

[https://discuss.leetcode.com/topic/95567/java-solution-using-dfs-with-full-comments](https://discuss.leetcode.com/topic/95567/java-solution-using-dfs-with-full-comments)




[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












