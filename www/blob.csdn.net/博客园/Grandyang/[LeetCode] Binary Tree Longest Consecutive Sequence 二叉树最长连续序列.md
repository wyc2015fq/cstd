# [LeetCode] Binary Tree Longest Consecutive Sequence 二叉树最长连续序列 - Grandyang - 博客园







# [[LeetCode] Binary Tree Longest Consecutive Sequence 二叉树最长连续序列](https://www.cnblogs.com/grandyang/p/5252599.html)







Given a binary tree, find the length of the longest consecutive sequence path.



The path refers to any sequence of nodes from some starting node to any node in the tree along the parent-child connections. The longest consecutive path need to be from parent to child (cannot be the reverse).

For example,
   1
    \
     3
    / \
   2   4
        \
         5

Longest consecutive sequence path is `3-4-5`, so return `3`.
   2
    \
     3
    / 
   2    
  / 
 1

Longest consecutive sequence path is `2-3`,not`3-2-1`, so return `2`.



这道题让我们求二叉树的最长连续序列，关于二叉树的题基本都需要遍历树，而递归遍历写起来特别简单，下面这种解法是用到了递归版的先序遍历，我们对于每个遍历到的节点，我们看节点值是否比参数值(父节点值)大1，如果是则长度加1，否则长度重置为1，然后更新结果res，再递归调用左右子节点即可，参见代码如下： 



解法一：

```
class Solution {
public:
    int longestConsecutive(TreeNode* root) {
        if (!root) return 0;
        int res = 0;
        dfs(root, root->val, 0, res);
        return res;
    }
    void dfs(TreeNode *root, int v, int out, int &res) {
        if (!root) return;
        if (root->val == v + 1) ++out;
        else out = 1;
        res = max(res, out);
        dfs(root->left, root->val, out, res);
        dfs(root->right, root->val, out, res);
    }
};
```



下面这种写法是利用分治法的思想，对左右子节点分别处理，如果左子节点存在且节点值比其父节点值大1，则递归调用函数，如果节点值不是刚好大1，则递归调用重置了长度的函数，对于右子节点的处理情况和左子节点相同，参见代码如下：



解法二：

```
class Solution {
public:
    int longestConsecutive(TreeNode* root) {
        if (!root) return 0;
        int res = 0;
        dfs(root, 1, res);
        return res;
    }
    void dfs(TreeNode *root, int len, int &res) {
        res = max(res, len);
        if (root->left) {
            if (root->left->val == root->val + 1) dfs(root->left, len + 1, res);
            else dfs(root->left, 1, res);
        }
        if (root->right) {
            if (root->right->val == root->val + 1) dfs(root->right, len + 1, res);
            else dfs(root->right, 1, res);
        }
    }
};
```



下面这种递归写法相当简洁，但是核心思想和上面两种方法并没有太大的区别，参见代码如下：



解法三：

```
class Solution {
public:
    int longestConsecutive(TreeNode* root) {
        return helper(root, NULL, 0);
    }
    int helper(TreeNode *root, TreeNode *p, int res) {
        if (!root) return res;
        res = (p && root->val == p->val + 1) ? res + 1 : 1;
        return max(res, max(helper(root->left, root, res), helper(root->right, root, res)));
    }
};
```



上面三种都是递归的写法，下面我们来看看迭代的方法，写法稍稍复杂一些，用的还是DFS的思想，以层序来遍历树，对于遍历到的节点，我们看其左右子节点有没有满足题意的，如果左子节点比其父节点大1，若右子节点存在，则排入queue，指针移到左子节点，反之若右子节点比其父节点大1，若左子节点存在，则排入queue，指针移到右子节点，依次类推直到queue为空，参见代码如下：



解法四：

```
class Solution {
public:
    int longestConsecutive(TreeNode* root) {
        if (!root) return 0;
        int res = 0;
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            int len = 1;
            TreeNode *t = q.front(); q.pop();
            while ((t->left && t->left->val == t->val + 1) || (t->right && t->right->val == t->val + 1)) {
                if (t->left && t->left->val == t->val + 1) {
                    if (t->right) q.push(t->right);
                    t = t->left;
                } else if (t->right && t->right->val == t->val + 1) {
                    if (t->left) q.push(t->left);
                    t = t->right;
                }
                ++len;
            }
            if (t->left) q.push(t->left);
            if (t->right) q.push(t->right);
            res = max(res, len);
        }
        return res;
    }
};
```



类似题目：

[Longest Increasing Subsequence](http://www.cnblogs.com/grandyang/p/4938187.html)



参考资料：

[https://leetcode.com/discuss/67938/my-c-4-lines-recursive-solution-40ms](https://leetcode.com/discuss/67938/my-c-4-lines-recursive-solution-40ms)

[https://leetcode.com/discuss/86742/ac-java-iterative-solution-using-queue](https://leetcode.com/discuss/86742/ac-java-iterative-solution-using-queue)

[https://leetcode.com/discuss/85086/15-lines-concise-and-easy-understand-c-solution](https://leetcode.com/discuss/85086/15-lines-concise-and-easy-understand-c-solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












