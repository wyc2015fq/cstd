# [LeetCode] Closest Binary Search Tree Value 最近的二分搜索树的值 - Grandyang - 博客园







# [[LeetCode] Closest Binary Search Tree Value 最近的二分搜索树的值](https://www.cnblogs.com/grandyang/p/5237170.html)







Given a non-empty binary search tree and a target value, find the value in the BST that is closest to the target.

Note:
- Given target value is a floating point.
- You are guaranteed to have only one unique value in the BST that is closest to the target.



这道题让我们找一个二分搜索数的跟给定值最接近的一个节点值，由于是二分搜索树，所以我最先想到用中序遍历来做，一个一个的比较，维护一个最小值，不停的更新，实际上这种方法并没有提高效率，用其他的遍历方法也可以，参见代码如下：



解法一：

```
class Solution {
public:
    int closestValue(TreeNode* root, double target) {
        double d = numeric_limits<double>::max();
        int res = 0;
        stack<TreeNode*> s;
        TreeNode *p = root;
        while (p || !s.empty()) {
            while (p) {
                s.push(p);
                p = p->left;
            }
            p = s.top(); s.pop();
            if (d >= abs(target - p->val)) {
                d = abs(target - p->val);
                res = p->val;
            }
            p = p->right;
        }
        return res;
    }
};
```



实际我们可以利用二分搜索树的特点(左<根<右)来快速定位，由于根节点是中间值，我们在往下遍历时，我们根据目标值和根节点的值大小关系来比较，如果目标值小于节点值，则我们应该找更小的值，于是我们到左子树去找，反之我们去右子树找，参见代码如下：



解法二：

```
class Solution {
public:
    int closestValue(TreeNode* root, double target) {
        int res = root->val;
        while (root) {
            if (abs(res - target) >= abs(root->val - target)) {
                res = root->val;
            }
            root = target < root->val ? root->left : root->right;
        }
        return res;
    }
};
```



以上两种方法都是迭代的方法，下面我们来看递归的写法，下面这种递归的写法和上面迭代的方法思路相同，都是根据二分搜索树的性质来优化查找，但是递归的写法用的是回溯法，先遍历到叶节点，然后一层一层的往回走，把最小值一层一层的运回来，参见代码如下：



解法三：

```
class Solution {
public:
    int closestValue(TreeNode* root, double target) {
        int a = root->val;
        TreeNode *t = target < a ? root->left : root->right;
        if (!t) return a;
        int b = closestValue(t, target);
        return abs(a - target) < abs(b - target) ? a : b;
    }
};
```



再来看另一种递归的写法，思路和上面的都相同，写法上略有不同，用if来分情况，参见代码如下：



解法三：

```
class Solution {
public:
    int closestValue(TreeNode* root, double target) {
        int res = root->val;
        if (target < root->val && root->left) {
            int l = closestValue(root->left, target);
            if (abs(res - target) >= abs(l - target)) res = l;
        } else if (target > root->val && root->right) {
            int r = closestValue(root->right, target);
            if (abs(res - target) >= abs(r - target)) res = r;
        }
        return res;
    }
};
```



最后来看一种分治法的写法，这种方法相当于解法一的递归写法，并没有利用到二分搜索树的性质来优化搜索，参见代码如下：



解法四：

```
class Solution {
public:
    int closestValue(TreeNode* root, double target) {
        double diff = numeric_limits<double>::max();
        int res = 0;
        helper(root, target, diff, res);
        return res;
    }
    void helper(TreeNode *root, double target, double &diff, int &res) {
        if (!root) return;
        if (diff >= abs(root->val - target)) {
            diff = abs(root->val - target);
            res = root->val;
        }
        helper(root->left, target, diff, res);
        helper(root->right, target, diff, res);
    }
};
```



参考资料:

[https://leetcode.com/discuss/84105/c-clean-solution](https://leetcode.com/discuss/84105/c-clean-solution)

[https://leetcode.com/discuss/85514/sharing-my-12ms-c-solution](https://leetcode.com/discuss/85514/sharing-my-12ms-c-solution)

[https://leetcode.com/discuss/54438/4-7-lines-recursive-iterative-ruby-c-java-python](https://leetcode.com/discuss/54438/4-7-lines-recursive-iterative-ruby-c-java-python)





[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)




















