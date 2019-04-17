# [LeetCode] Second Minimum Node In a Binary Tree 二叉树中第二小的结点 - Grandyang - 博客园







# [[LeetCode] Second Minimum Node In a Binary Tree 二叉树中第二小的结点](https://www.cnblogs.com/grandyang/p/7590156.html)







Given a non-empty special binary tree consisting of nodes with the non-negative value, where each node in this tree has exactly `two` or `zero` sub-node. If the node has two sub-nodes, then this node's value is the smaller value among its two sub-nodes.

Given such a binary tree, you need to output the second minimum value in the set made of all the nodes' value in the whole tree.

If no such second minimum value exists, output -1 instead.

Example 1:
Input: 
    2
   / \
  2   5
     / \
    5   7

Output: 5
Explanation: The smallest value is 2, the second smallest value is 5.



Example 2:
Input: 
    2
   / \
  2   2

Output: -1
Explanation: The smallest value is 2, but there isn't any second smallest value.


这道题让我们找二叉树中的第二小的结点值，并且给该二叉树做了一些限制，比如对于任意一个结点，要么其没有子结点，要么就同时有两个子结点，而且父结点值是子结点值中较小的那个，当然两个子结点值可以相等。那么直接上暴力搜索呗，根据该树的附加条件可知，根结点一定是最小的结点值first，那么我们只要找出第二小的值second即可，初始化为整型的最大值。然后对根结点调用递归函数，将first和second当作参数传进去即可。在递归函数中，如果当前结点为空，直接返回，若当前结点孩值不等于first，说明其肯定比first要大，然后我们看其是否比second小，小的话就更新second，然后对当前结点的左右子结点分别调用递归函数即可，参见代码如下：



解法一：

```
class Solution {
public:
    int findSecondMinimumValue(TreeNode* root) {
        int first = root->val, second = INT_MAX;
        helper(root, first, second);
        return (second == first || second == INT_MAX) ? -1 : second;
    }
    void helper(TreeNode* node, int& first, int& second) {
        if (!node) return;
        if (node->val != first && node->val < second) {
            second = node->val;
        }
        helper(node->left, first, second);
        helper(node->right, first, second);
    }
};
```



下面这种方法也是用递归来做的，不过现在递归函数有了返回值，在递归函数中，还是先判断当前结点是否为空，为空直接返回-1。然后就是看当前结点是否等于first，不等于直接返回当前结点值。如果等于，我们对其左右子结点分别调用递归函数，分别得到left和right。如果left和right其中有一个为-1了，我们取其中的较大值；如果left和right都不为-1，我们取其中的较小值返回即可，参见代码如下：



解法二：

```
class Solution {
public:
    int findSecondMinimumValue(TreeNode* root) {
        return helper(root, root->val);
    }
    int helper(TreeNode* node, int first) {
        if (!node) return -1;
        if (node->val != first) return node->val;
        int left = helper(node->left, first), right = helper(node->right, first);
        return (left == -1 || right == -1) ? max(left, right) : min(left, right);
    }
};
```



下面这种递归方法更加简洁了，没有再使用专门的递归函数helper，而是对当前根结点判断其左子树是否存在，不存在就返回-1。题目中说了是非空树，所以根结点一定存在。然后我们比较如果左子结点值等于根结点值，我们则对其左子结点调用递归函数；否则left就等于其左子结点值。再比较如果右子结点值等于根结点值，则对其右子结点调用递归函数；否则right就等于其右子结点值。最后我们还是看如果left和right其中有一个为-1了，我们取其中的较大值；如果left和right都不为-1，我们取其中的较小值返回即可，参见代码如下：



解法三：

```
class Solution {
public:
    int findSecondMinimumValue(TreeNode* root) {
        if (!root->left) return -1;
        int left = (root->left->val == root->val) ? findSecondMinimumValue(root->left) : root->left->val;
        int right = (root->right->val == root->val) ? findSecondMinimumValue(root->right) : root->right->val;
        return (left == -1 || right == -1) ? max(left, right) : min(left, right);
    }
};
```



整了三种递归的解法，来看一种迭代的解法吧，用的是层序遍历，但还是用的解法一种的不停更新second的方法，参见代码如下：



解法四：

```
class Solution {
public:
    int findSecondMinimumValue(TreeNode* root) {
        int first = root->val, second = INT_MAX;
        queue<TreeNode*> q{{root}};
        while (!q.empty()) {
            auto t = q.front(); q.pop();
            if (t->val != first && t->val < second) {
                second = t->val;
            }
            if (t->left) q.push(t->left);
            if (t->right) q.push(t->right);
        }
        return (second == first || second == INT_MAX) ? -1 : second;
    }
};
```



类似题目：

[Kth Smallest Element in a BST](http://www.cnblogs.com/grandyang/p/4620012.html)



参考资料：

[https://discuss.leetcode.com/topic/102277/java-4-lines](https://discuss.leetcode.com/topic/102277/java-4-lines)

[https://discuss.leetcode.com/topic/102027/c-dfs-recursion](https://discuss.leetcode.com/topic/102027/c-dfs-recursion)

[https://discuss.leetcode.com/topic/102035/bfs-acc-solution-java-and-c-code](https://discuss.leetcode.com/topic/102035/bfs-acc-solution-java-and-c-code)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












