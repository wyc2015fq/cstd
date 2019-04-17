# [LeetCode] Binary Tree Tilt  二叉树的坡度 - Grandyang - 博客园







# [[LeetCode] Binary Tree Tilt  二叉树的坡度](https://www.cnblogs.com/grandyang/p/6786643.html)







Given a binary tree, return the tilt of the whole tree.

The tilt of a tree node is defined as the absolute difference between the sum of all left subtree node values and the sum of all right subtree node values. Null node has tilt 0.

The tilt of the whole tree is defined as the sum of all nodes' tilt.

Example:
Input: 
         1
       /   \
      2     3
Output: 1
Explanation: 
Tilt of node 2 : 0
Tilt of node 3 : 0
Tilt of node 1 : |2-3| = 1
Tilt of binary tree : 0 + 0 + 1 = 1



Note:
- The sum of node values in any subtree won't exceed the range of 32-bit integer.
- All the tilt values won't exceed the range of 32-bit integer.



这道题让我们求二叉树的坡度，某个结点的坡度的定义为该结点的左子树之和与右子树之和的差的绝对值，这道题让我们求所有结点的坡度之和。我开始的想法就是老老实实的按定义去做，用先序遍历，对于每个遍历到的结点，先计算坡度，根据定义就是左子树之和与右子树之和的差的绝对值，然后返回的是当前结点的tilt加上对其左右子结点调用求坡度的递归函数即可。其中求子树之和用另外一个函数来求，也是用先序遍历来求结点之和，为了避免重复运算，这里用哈希表来保存已经算过的结点，参见代码如下：



解法一：

```
class Solution {
public:
    unordered_map<TreeNode*, int> m;
    int findTilt(TreeNode* root) {
        if (!root) return 0;
        int tilt = abs(getSum(root->left, m) - getSum(root->right, m));
        return tilt + findTilt(root->left) + findTilt(root->right);
    }
    int getSum(TreeNode* node, unordered_map<TreeNode*, int>& m) {
        if (!node) return 0;
        if (m.count(node)) return m[node];
        return m[node] = getSum(node->left, m) + getSum(node->right, m) + node->val;
    }
};
```



但是在论坛中看了大神们的帖子后，发现这道题最好的解法应该是用后序遍历来做，因为后序遍历的顺序是左-右-根，那么就会从叶结点开始处理，这样我们就能很方便的计算结点的累加和，同时也可以很容易的根据子树和来计算tilt，参见代码如下：



解法二：

```
class Solution {
public:
    int findTilt(TreeNode* root) {
        int res = 0;
        postorder(root, res);
        return res;
    }
    int postorder(TreeNode* node, int& res) {
        if (!node) return 0;
        int leftSum = postorder(node->left, res);
        int rightSum = postorder(node->right, res);
        res += abs(leftSum - rightSum);
        return leftSum + rightSum + node->val;
    }
};
```



参考资料：

[https://discuss.leetcode.com/topic/87191/java-o-n-postorder-traversal](https://discuss.leetcode.com/topic/87191/java-o-n-postorder-traversal)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












