# [LeetCode] Find Duplicate Subtrees 寻找重复树 - Grandyang - 博客园







# [[LeetCode] Find Duplicate Subtrees 寻找重复树](https://www.cnblogs.com/grandyang/p/7500082.html)








Given a binary tree, return all duplicate subtrees. For each kind of duplicate subtrees, you only need to return the root node of any oneof them.

Two trees are duplicate if they have the same structure with same node values.

Example 1: 
        1
       / \
      2   3
     /   / \
    4   2   4
       /
      4

The following are two duplicate subtrees:
      2
     /
    4

and
    4

Therefore, you need to return above trees' root in the form of a list.



这道题让我们寻找重复树，博主开始的思路是遍历每个结点，将结点值相同的结点放到一起，如果再遇到相同的结点值，则调用一个判断是否是相同树的子函数，但是这样会有大量的重复运算，会TLE。后来去网上看大神们的解法，发现果然是很叼啊，用到了后序遍历，还有数组序列化，并且建立序列化跟其出现次数的映射，这样如果我们得到某个结点的序列化字符串，而该字符串正好出现的次数为1，说明之前已经有一个重复树了，我们将当前结点存入结果res，这样保证了多个重复树只会存入一个结点，参见代码如下：



```
class Solution {
public:
    vector<TreeNode*> findDuplicateSubtrees(TreeNode* root) {
        vector<TreeNode*> res;
        unordered_map<string, int> m;
        helper(root, m, res);
        return res;
    }
    string helper(TreeNode* node, unordered_map<string, int>& m, vector<TreeNode*>& res) {
        if (!node) return "#";
        string str = to_string(node->val) + "," + helper(node->left, m, res) + "," + helper(node->right, m, res);
        if (m[str] == 1) res.push_back(node);
        ++m[str];
        return str;
    }
};
```



参考资料：

[https://discuss.leetcode.com/topic/97584/java-concise-postorder-traversal-solution](https://discuss.leetcode.com/topic/97584/java-concise-postorder-traversal-solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













