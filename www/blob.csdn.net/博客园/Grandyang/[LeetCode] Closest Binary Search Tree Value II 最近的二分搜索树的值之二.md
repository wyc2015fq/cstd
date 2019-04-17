# [LeetCode] Closest Binary Search Tree Value II 最近的二分搜索树的值之二 - Grandyang - 博客园







# [[LeetCode] Closest Binary Search Tree Value II 最近的二分搜索树的值之二](https://www.cnblogs.com/grandyang/p/5247398.html)







Given a non-empty binary search tree and a target value, find *k* values in the BST that are closest to the target.

Note:
- Given target value is a floating point.
- You may assume *k* is always valid, that is: *k* ≤ total nodes.
- You are guaranteed to have only one unique set of *k* values in the BST that are closest to the target.



Follow up:
Assume that the BST is balanced, could you solve it in less than *O*(*n*) runtime (where *n* = total nodes)?

Hint:

1. Consider implement these two helper functions:
　　i. getPredecessor(N), which returns the next smaller node to N.
　　ii. getSuccessor(N), which returns the next larger node to N.
2. Try to assume that each node has a parent pointer, it makes the problem much easier.
3. Without parent pointer we just need to keep track of the path from the root to the current node using a stack.
4. You would need two stacks to track the path in finding predecessor and successor node separately.



这道题是之前那道[Closest Binary Search Tree Value](http://www.cnblogs.com/grandyang/p/5237170.html)的拓展，那道题只让我们找出离目标值最近的一个节点值，而这道题让我们找出离目标值最近的k个节点值，难度瞬间增加了不少，我最先想到的方法是用中序遍历将所有节点值存入到一个一维数组中，由于二分搜索树的性质，这个一维数组是有序的，然后我们再在有序数组中需要和目标值最近的k个值就简单的多，参见代码如下：



解法一：

```
class Solution {
public:
    vector<int> closestKValues(TreeNode* root, double target, int k) {
        vector<int> res, v;
        inorder(root, v);
        int idx = 0;
        double diff = numeric_limits<double>::max();
        for (int i = 0; i < v.size(); ++i) {
            if (diff >= abs(target - v[i])) {
                diff = abs(target - v[i]);
                idx = i;
            }
        }
        int left = idx - 1, right = idx + 1;
        for (int i = 0; i < k; ++i) {
            res.push_back(v[idx]);
            if (left >= 0 && right < v.size()) {
                if (abs(v[left] - target) > abs(v[right] - target)) {
                    idx = right;
                    ++right;
                } else {
                    idx = left;
                    --left;
                }
            } else if (left >= 0) {
                idx = left;
                --left;
            } else if (right < v.size()) {
                idx = right;
                ++right;
            }
        }
        return res;
    }
    void inorder(TreeNode *root, vector<int> &v) {
        if (!root) return;
        inorder(root->left, v);
        v.push_back(root->val);
        inorder(root->right, v);
    }
};
```



还有一种解法是直接在中序遍历的过程中完成比较，当遍历到一个节点时，如果此时结果数组不到k个，我们直接将此节点值加入res中，如果该节点值和目标值的差值的绝对值小于res的首元素和目标值差值的绝对值，说明当前值更靠近目标值，则将首元素删除，末尾加上当前节点值，反之的话说明当前值比res中所有的值都更偏离目标值，由于中序遍历的特性，之后的值会更加的遍历，所以此时直接返回最终结果即可，参见代码如下：



解法二：

```
class Solution {
public:
    vector<int> closestKValues(TreeNode* root, double target, int k) {
        vector<int> res;
        inorder(root, target, k, res);
        return res;
    }
    void inorder(TreeNode *root, double target, int k, vector<int> &res) {
        if (!root) return;
        inorder(root->left, target, k, res);
        if (res.size() < k) res.push_back(root->val);
        else if (abs(root->val - target) < abs(res[0] - target)) {
            res.erase(res.begin());
            res.push_back(root->val);
        } else return;
        inorder(root->right, target, k, res);
    }
};
```



下面这种方法是上面那种方法的迭代写法，原理一模一样，参见代码如下：



解法三：

```
class Solution {
public:
    vector<int> closestKValues(TreeNode* root, double target, int k) {
        vector<int> res;
        stack<TreeNode*> s;
        TreeNode *p = root;
        while (p || !s.empty()) {
            while (p) {
                s.push(p);
                p = p->left;
            }
            p = s.top(); s.pop();
            if (res.size() < k) res.push_back(p->val);
            else if (abs(p->val - target) < abs(res[0] - target)) {
                res.erase(res.begin());
                res.push_back(p->val);
            } else break;
            p = p->right;
        }
        return res;
    }
};
```



在来看一种利用最大堆来解题的方法，堆里保存的一个差值diff和节点值的pair，我们中序遍历二叉树(也可以用其他遍历方法)，然后对于每个节点值都计算一下和目标值之差的绝对值，由于最大堆的性质，diff大的自动拍到最前面，我们维护k个pair，如果超过了k个，就把堆前面大的pair删掉，最后留下的k个pair，我们将pair中的节点值取出存入res中返回即可，参见代码如下： 



解法四：

```
class Solution {
public:
    vector<int> closestKValues(TreeNode* root, double target, int k) {
        vector<int> res;
        priority_queue<pair<double, int>> q;
        inorder(root, target, k, q);
        while (!q.empty()) {
            res.push_back(q.top().second);
            q.pop();
        }
        return res;
    }
    void inorder(TreeNode *root, double target, int k, priority_queue<pair<double, int>> &q) {
        if (!root) return;
        inorder(root->left, target, k, q);
        q.push({abs(root->val - target), root->val});
        if (q.size() > k) q.pop();
        inorder(root->right, target, k, q);
    }
};
```



下面的这种方法用了两个栈，pre和suc，其中pre存小于目标值的数，suc存大于目标值的数，开始初始化pre和suc的时候，要分别将最接近目标值的稍小值和稍大值压入pre和suc，然后我们循环k次，每次比较pre和suc的栈顶元素，看谁更接近目标值，将其存入结果res中，然后更新取出元素的栈，依次类推直至取完k个数返回即可，参见代码如下：



解法五：

```
class Solution {
public:
    vector<int> closestKValues(TreeNode* root, double target, int k) {
        vector<int> res;
        stack<TreeNode*> pre, suc;
        while (root) {
            if (root->val <= target) {
                pre.push(root);
                root = root->right;
            } else {
                suc.push(root);
                root = root->left;
            }
        }
        while (k-- > 0) {
            if (suc.empty() || !pre.empty() && target - pre.top()->val < suc.top()->val - target) {
                res.push_back(pre.top()->val);
                getPredecessor(pre);
            } else {
                res.push_back(suc.top()->val);
                getSuccessor(suc);
            }
        }
        return res;
    }
    void getPredecessor(stack<TreeNode*> &pre) {
        TreeNode *t = pre.top(); pre.pop();
        if (t->left) {
            pre.push(t->left);
            while (pre.top()->right) pre.push(pre.top()->right);
        }
    }
    void getSuccessor(stack<TreeNode*> &suc) {
        TreeNode *t = suc.top(); suc.pop();
        if (t->right) {
            suc.push(t->right);
            while (suc.top()->left) suc.push(suc.top()->left);
        }
    }
};
```



类似题目：

[Closest Binary Search Tree Value](http://www.cnblogs.com/grandyang/p/5237170.html)



参考资料：

[https://leetcode.com/discuss/69220/2-ms-o-n-and-6-ms-o-logn-java-solution](https://leetcode.com/discuss/69220/2-ms-o-n-and-6-ms-o-logn-java-solution)

[https://leetcode.com/discuss/77954/easy-o-n-c-solution-using-priority_queue](https://leetcode.com/discuss/77954/easy-o-n-c-solution-using-priority_queue)

[https://leetcode.com/discuss/64713/clear-java-solution-with-one-stack-one-linkedlist](https://leetcode.com/discuss/64713/clear-java-solution-with-one-stack-one-linkedlist)

[https://leetcode.com/discuss/83431/c-simple-inorder-solution-with-o-n-runtime-and-o-k-memory](https://leetcode.com/discuss/83431/c-simple-inorder-solution-with-o-n-runtime-and-o-k-memory)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












