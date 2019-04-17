# [LeetCode] Binary Tree Paths 二叉树路径 - Grandyang - 博客园







# [[LeetCode] Binary Tree Paths 二叉树路径](https://www.cnblogs.com/grandyang/p/4738031.html)







Given a binary tree, return all root-to-leaf paths.

For example, given the following binary tree:


   1
 /   \
2     3
 \
  5

All root-to-leaf paths are:
["1->2->5", "1->3"]


这道题给我们一个二叉树，让我们返回所有根到叶节点的路径，跟之前那道[Path Sum II](http://www.cnblogs.com/grandyang/p/4042156.html)很类似，比那道稍微简单一些，不需要计算路径和，只需要无脑返回所有的路径即可，那么思路还是用递归来解，博主之前就强调过，玩树的题目，十有八九都是递归，而递归的核心就是不停的DFS到叶结点，然后在回溯回去。在递归函数中，当我们遇到叶结点的时候，即没有左右子结点，那么此时一条完整的路径已经形成了，我们加上当前的叶结点后存入结果res中，然后回溯。注意这里结果res需要reference，而out是不需要引用的，不然回溯回去还要删除新添加的结点，很麻烦。为了减少判断空结点的步骤，我们在调用递归函数之前都检验一下非空即可，代码而很简洁，参见如下：



解法一：

```
class Solution {
public:
    vector<string> binaryTreePaths(TreeNode* root) {
        vector<string> res;
        if (root) helper(root, "", res);
        return res;
    }
    void helper(TreeNode* node, string out, vector<string>& res) {
        if (!node->left && !node->right) res.push_back(out + to_string(node->val));
        if (node->left) helper(node->left, out + to_string(node->val) + "->", res);
        if (node->right) helper(node->right, out + to_string(node->val) + "->", res);
    }
};
```



下面再来看一种递归的方法，这个方法直接在一个函数中完成递归调用，不需要另写一个helper函数，核心思想和上面没有区别，参见代码如下：



解法二：

```
class Solution {
public:
    vector<string> binaryTreePaths(TreeNode* root) {
        if (!root) return {};
        if (!root->left && !root->right) return {to_string(root->val)};
        vector<string> left = binaryTreePaths(root->left);
        vector<string> right = binaryTreePaths(root->right);
        left.insert(left.end(), right.begin(), right.end());
        for (auto &a : left) {
            a = to_string(root->val) + "->" + a;
        }
        return left;
    }
};
```



还是递归写法，从论坛中扒下来的解法，核心思路都一样啦，写法各有不同而已，参见代码如下：



解法三：

```
class Solution {
public:
    vector<string> binaryTreePaths(TreeNode* root) {
        if (!root) return {};
        if (!root->left && !root->right) return {to_string(root->val)};
        vector<string> res;
        for (string str : binaryTreePaths(root->left)) {
            res.push_back(to_string(root->val) + "->" + str);
        }
        for (string str : binaryTreePaths(root->right)) {
            res.push_back(to_string(root->val) + "->" + str);
        }
        return res;
    }
};
```



类似题目：

[Path Sum II](http://www.cnblogs.com/grandyang/p/4042156.html)



参考资料：

[https://leetcode.com/problems/binary-tree-paths/discuss/68258/Accepted-Java-simple-solution-in-8-lines](https://leetcode.com/problems/binary-tree-paths/discuss/68258/Accepted-Java-simple-solution-in-8-lines)

[https://leetcode.com/problems/binary-tree-paths/discuss/68282/Clean-Java-solution-(Accepted)-without-any-helper-recursive-function](https://leetcode.com/problems/binary-tree-paths/discuss/68282/Clean-Java-solution-(Accepted)-without-any-helper-recursive-function)





[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












