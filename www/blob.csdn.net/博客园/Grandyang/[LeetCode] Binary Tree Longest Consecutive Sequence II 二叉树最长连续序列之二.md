# [LeetCode] Binary Tree Longest Consecutive Sequence II 二叉树最长连续序列之二 - Grandyang - 博客园







# [[LeetCode] Binary Tree Longest Consecutive Sequence II 二叉树最长连续序列之二](https://www.cnblogs.com/grandyang/p/6864398.html)







Given a binary tree, you need to find the length of Longest Consecutive Path in Binary Tree.

Especially, this path can be either increasing or decreasing. For example, [1,2,3,4] and [4,3,2,1] are both considered valid, but the path [1,2,4,3] is not valid. On the other hand, the path can be in the child-Parent-child order, where not necessarily be parent-child order.

Example 1:
Input:
        1
       / \
      2   3
Output: 2
Explanation: The longest consecutive path is [1, 2] or [2, 1].



Example 2:
Input:
        2
       / \
      1   3
Output: 3
Explanation: The longest consecutive path is [1, 2, 3] or [3, 2, 1].



Note: All the values of tree nodes are in the range of [-1e7, 1e7].



这道题是之前那道[Binary Tree Longest Consecutive Sequence](http://www.cnblogs.com/grandyang/p/5252599.html)的拓展，那道题只让从父结点到子结点这种顺序来找最长连续序列，而这道题没有这个顺序限制，我们可以任意的拐弯，这样能找到最长的递增或者递减的路径。这道题利用回溯的思想比较容易，因为当一个结点没有子结点点时，它只需要跟其父结点进行比较，这种情况最容易处理，而且一旦叶结点处理完了，我们可以一层一层的回溯，直到回到根结点，然后再遍历的过程中不断的更新结果res即可。由于题目中说了要么是递增，要么是递减，我们不能一会递增一会递减，所以我们递增递减的情况都要统计，只是最后取最长的路径。所以我们要知道每一个结点的最长递增和递减路径的长度，当然是从叶结点算起，这样才方便往根结点回溯。当某个结点比其父结点值大1的话，说明这条路径是递增的，那么当我们知道其左右子结点各自的递增路径长度，那么当前结点的递增路径长度就是左右子结点递增路径长度中的较大值加上1，同理如果是递减路径，那么当前结点的递减路径长度就是左右子结点递减路径长度中的较大值加上1，通过这种方式我们可以更新每个结点的递增递减路径长度。在回溯的过程中，一旦我们知道了某个结点的左右子结点的最长递增递减路径长度，那么我们可以算出当前结点的最长连续序列的长度，要么是左子结点的递增路径跟右子结点的递减路径之和加1，要么事左子结点的递减路径跟右子结点的递增路径之和加1，二者中取较大值即可，参见代码如下：



解法一：

```
class Solution {
public:
    int longestConsecutive(TreeNode* root) {
        int res = 0;
        helper(root, root, res);
        return res;
    }
    pair<int, int> helper(TreeNode* node, TreeNode* parent, int& res) {
        if (!node) return {0, 0};
        auto left = helper(node->left, node, res);
        auto right = helper(node->right, node, res);
        res = max(res, left.first + right.second + 1);
        res = max(res, left.second + right.first + 1);
        int inc = 0, dec = 0;
        if (node->val == parent->val + 1) {
            inc = max(left.first, right.first) + 1;
        } else if (node->val + 1 == parent->val) {
            dec = max(left.second, right.second) + 1;
        }
        return {inc, dec};
    }
};
```



上面的方法把所有内容都写到了一个递归函数中，看起来有些臃肿。而下面这种方法分了两个递归来写，相对来说简洁一些。因为每个结点的最长连续序列长度等于其最长递增路径长度跟最长递减路径之和加1，然后分别对其左右子结点调用递归函数，取三者最大值，相当于对二叉树进行了先序遍历，参见代码如下：



解法二：

```
class Solution {
public:
    int longestConsecutive(TreeNode* root) {
        if (!root) return 0;
        int res = helper(root, 1) + helper(root, -1) + 1;
        return max(res, max(longestConsecutive(root->left), longestConsecutive(root->right)));
    }
    int helper(TreeNode* node, int diff) {
        if (!node) return 0;
        int left = 0, right = 0;
        if (node->left && node->val - node->left->val == diff) {
            left = 1 + helper(node->left, diff);
        }
        if (node->right && node->val - node->right->val == diff) {
            right = 1 + helper(node->right, diff);
        }
        return max(left, right);
    }
};
```



类似题目：

[Binary Tree Longest Consecutive Sequence](http://www.cnblogs.com/grandyang/p/5252599.html)



参考资料：

[https://discuss.leetcode.com/topic/85808/c-solution](https://discuss.leetcode.com/topic/85808/c-solution)

[https://discuss.leetcode.com/topic/85778/dfs-c-python-solutions](https://discuss.leetcode.com/topic/85778/dfs-c-python-solutions)

[https://discuss.leetcode.com/topic/85764/neat-java-solution-single-pass-o-n](https://discuss.leetcode.com/topic/85764/neat-java-solution-single-pass-o-n)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












