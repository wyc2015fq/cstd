# [LeetCode] House Robber III 打家劫舍之三 - Grandyang - 博客园







# [[LeetCode] House Robber III 打家劫舍之三](https://www.cnblogs.com/grandyang/p/5275096.html)







The thief has found himself a new place for his thievery again. There is only one entrance to this area, called the "root." Besides the root, each house has one and only one parent house. After a tour, the smart thief realized that "all houses in this place forms a binary tree". It will automatically contact the police if two directly-linked houses were broken into on the same night.

Determine the maximum amount of money the thief can rob tonight without alerting the police.

Example 1:
3
    / \
   2   3
    \   \ 
     3   1
Maximum amount of money the thief can rob = 3 + 3 + 1 = 7.



Example 2:
     3
    / \
   45
  / \   \ 
 1   3   1

Maximum amount of money the thief can rob = 4 + 5 = 9.



Credits:
Special thanks to [@dietpepsi](https://leetcode.com/discuss/user/dietpepsi) for adding this problem and creating all test cases.



这道题是之前那两道[House Robber II](http://www.cnblogs.com/grandyang/p/4518674.html)和[House Robber](http://www.cnblogs.com/grandyang/p/4383632.html)的拓展，这个小偷又偷出新花样了，沿着二叉树开始偷，碉堡了，题目中给的例子看似好像是要每隔一个偷一次，但实际上不一定只隔一个，比如如下这个例子：

```
4
       /
      1
     /
    2
   /
  3
```

如果隔一个偷，那么是4+2=6，其实最优解应为4+3=7，隔了两个，所以说纯粹是怎么多怎么来，那么这种问题是很典型的递归问题，我们可以利用回溯法来做，因为当前的计算需要依赖之前的结果，那么我们对于某一个节点，如果其左子节点存在，我们通过递归调用函数，算出不包含左子节点返回的值，同理，如果右子节点存在，算出不包含右子节点返回的值，那么此节点的最大值可能有两种情况，一种是该节点值加上不包含左子节点和右子节点的返回值之和，另一种是左右子节点返回值之和不包含当期节点值，取两者的较大值返回即可，但是这种方法无法通过OJ，超时了，所以我们必须优化这种方法，这种方法重复计算了很多地方，比如要完成一个节点的计算，就得一直找左右子节点计算，我们可以把已经算过的节点用哈希表保存起来，以后递归调用的时候，现在哈希表里找，如果存在直接返回，如果不存在，等计算出来后，保存到哈希表中再返回，这样方便以后再调用，参见代码如下：



解法一：

```
class Solution {
public:
    int rob(TreeNode* root) {
        unordered_map<TreeNode*, int> m;
        return dfs(root, m);
    }
    int dfs(TreeNode *root, unordered_map<TreeNode*, int> &m) {
        if (!root) return 0;
        if (m.count(root)) return m[root];
        int val = 0;
        if (root->left) {
            val += dfs(root->left->left, m) + dfs(root->left->right, m);
        }
        if (root->right) {
            val += dfs(root->right->left, m) + dfs(root->right->right, m);
        }
        val = max(val + root->val, dfs(root->left, m) + dfs(root->right, m));
        m[root] = val;
        return val;
    }
};
```



下面再来看一种方法，这种方法的递归函数返回一个大小为2的一维数组res，其中res[0]表示不包含当前节点值的最大值，res[1]表示包含当前值的最大值，那么我们在遍历某个节点时，首先对其左右子节点调用递归函数，分别得到包含与不包含左子节点值的最大值，和包含于不包含右子节点值的最大值，那么当前节点的res[0]就是左子节点两种情况的较大值加上右子节点两种情况的较大值，res[1]就是不包含左子节点值的最大值加上不包含右子节点值的最大值，和当前节点值之和，返回即可，参见代码如下：



解法二：

```
class Solution {
public:
    int rob(TreeNode* root) {
        vector<int> res = dfs(root);
        return max(res[0], res[1]);
    }
    vector<int> dfs(TreeNode *root) {
        if (!root) return vector<int>(2, 0);
        vector<int> left = dfs(root->left);
        vector<int> right = dfs(root->right);
        vector<int> res(2, 0);
        res[0] = max(left[0], left[1]) + max(right[0], right[1]);
        res[1] = left[0] + right[0] + root->val;
        return res;
    }
};
```



下面这种解法由网友[edyyy](http://home.cnblogs.com/u/1090659/)提供，仔细看了一下，也非常的巧妙，思路和解法二有些类似。这里的helper函数返回当前结点为根结点的最大rob的钱数，里面的两个参数l和r表示分别从左子结点和右子结点开始rob，分别能获得的最大钱数。在递归函数里面，如果当前结点不存在，直接返回0。否则我们对左右子结点分别调用递归函数，得到l和r。另外还得到四个变量，ll和lr表示左子结点的左右子结点的最大rob钱数，rl和rr表示右子结点的最大rob钱数。那么我们最后返回的值其实是两部分的值比较，其中一部分的值是当前的结点值加上ll, lr, rl, 和rr这四个值，这不难理解，因为抢了当前的房屋，那么左右两个子结点就不能再抢了，但是再下一层的四个子结点都是可以抢的；另一部分是不抢当前房屋，而是抢其左右两个子结点，即l+r的值，返回两个部分的值中的较大值即可，参见代码如下：



解法三：

```
class Solution {
public:
    int rob(TreeNode* root) {
        int l = 0, r = 0;
        return helper(root, l, r);
    }
    int helper(TreeNode* node, int& l, int& r) {
        if (!node) return 0;
        int ll = 0, lr = 0, rl = 0, rr = 0;
        l = helper(node->left, ll, lr);
        r = helper(node->right, rl, rr);
        return max(node->val + ll + lr + rl + rr, l + r);
    }
};
```



类似题目：

[House Robber II](http://www.cnblogs.com/grandyang/p/4518674.html)

[House Robber](http://www.cnblogs.com/grandyang/p/4383632.html)



参考资料：

[https://discuss.leetcode.com/topic/39834/step-by-step-tackling-of-the-problem](https://discuss.leetcode.com/topic/39834/step-by-step-tackling-of-the-problem)

[https://discuss.leetcode.com/topic/40847/simple-c-solution](https://discuss.leetcode.com/topic/40847/simple-c-solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












