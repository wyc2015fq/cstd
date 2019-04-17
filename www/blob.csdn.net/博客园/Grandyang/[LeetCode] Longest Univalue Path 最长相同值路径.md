# [LeetCode] Longest Univalue Path 最长相同值路径 - Grandyang - 博客园







# [[LeetCode] Longest Univalue Path 最长相同值路径](https://www.cnblogs.com/grandyang/p/7636259.html)







Given a binary tree, find the length of the longest path where each node in the path has the same value. This path may or may not pass through the root.

Note: The length of path between two nodes is represented by the number of edges between them.

Example 1:

Input:
              5
             / \
            4   5
           / \   \
          1   1   5
Output:
2
Example 2:

Input:
              1
             / \
            4   5
           / \   \
          4   4   5
Output:
2
Note: The given binary tree has not more than 10000 nodes. The height of the tree is not more than 1000.



这道题让我们求最长的相同值路径，跟之前那道 [Count Univalue Subtrees](http://www.cnblogs.com/grandyang/p/5206862.html) 十分的类似，解法也很类似。对于这种树的路径问题，递归是不二之选。在递归函数中，我们首先对其左右子结点调用递归函数，得到其左右子树的最大相同值路径长度，下面就要来看当前结点和其左右子结点之间的关系了，如果其左子结点存在且和当前节点值相同，则left自增1，否则left重置0；同理，如果其右子结点存在且和当前节点值相同，则right自增1，否则right重置0。然后用left+right来更新结果res。而调用当前节点值的函数只能返回left和right中的较大值，因为如果还要跟父节点组path，就只能在左右子节点中选一条path，当然选值大的那个了，什么意思呢，举个例子来说吧，比如下面的这棵二叉树：

```
1
     / \
    4   5
   / \   \
  4   4   5
 /
4
```

若此时的node是只有两个结点的第二层的那个结点4，那么分别对其左右子结点调用递归，会得到 left = 1, right = 0，因为此时要跟结点4组成path，所以肯定挑左子结点（有两个4的那条边），那你会问为啥不能连上右子结点的那个4，这整条长度为3的path（left+right，此时的left和right已经分别自增1了，left=2，right=1）其实我们已经用来更新过结果res了。需要注意的是我们的递归函数helper返回值的意义，并不是经过某个结点的最长路径的长度，最长路径长度保存在了结果res中，不是返回值，返回的是以该结点为终点的最长路径长度，这样回溯的时候，我们还可以继续连上其父结点，比如若根结点也是4的话，那么回溯到根结点的时候，路径长度又可以增加了，参见代码如下：



解法一：

```
class Solution {
public:
    int longestUnivaluePath(TreeNode* root) {
        int res = 0;
        helper(root, res);
        return res;
    }
    int helper(TreeNode* node, int& res) {
        if (!node) return 0;
        int left = helper(node->left, res);
        int right = helper(node->right, res);
        left = (node->left && node->val == node->left->val) ? left + 1 : 0;
        right = (node->right && node->val == node->right->val) ? right + 1 : 0;
        res = max(res, left + right);
        return max(left, right);
    }
};
```



下面这种解法跟上面的方法很类似，区别在于递归函数中多了一个参数，parent保存的是父结点值，其实仔细比较下两种解法，发现就是加1的地方略有不同，那么这里helper的返回值意义就发生了变化，这里的返回值表示的是以当前结点的父结点为路径终点的最大相同值路径长度，这样我们递归出来的left和right就不用再加1，直接可以求和并更新结果res了，由于当前结点的父结点值知道，那么我们和父结点值比较一下，如果相同，返回left和right中较大值并再加1，如果不同，则返回0，这是因为之前说的必须要以父结点为路径终点，那么既然父结点的值不同，所以长度肯定是0了，参见代码如下：



解法二：

```
class Solution {
public:
    int longestUnivaluePath(TreeNode* root) {
        int res = 0;
        if (root) helper(root, root->val, res);
        return res;
    }
    int helper(TreeNode* node, int parent, int& res) {
        if (!node) return 0;
        int left = helper(node->left, node->val, res);
        int right = helper(node->right, node->val, res);
        res = max(res, left + right);
        if (node->val == parent) return max(left, right) + 1;
        return 0;
    }
};
```



下面这种解法使用了两个递归函数，使得写法更加简洁了，首先还是先判断root是否为空，是的话返回0。然后对左右子节点分别调用当前函数，取其中较大值保存到变量sub中，表示左右子树中最长的相同值路径，然后就是要跟当前树的最长相同值路径比较，计算方法是对左右子结点调用一个helper函数，并把当前结点值传进去，把返回值加起来和sub比较，取较大值返回。顺便提一下，这里的helper函数的返回值的意义跟解法二中的是一样的。在helper函数里，若当前结点为空，或者当前节点值不等于父结点值的话，返回0。否则结返回对左右子结点分别调用helper递归函数中的较大值加1，我们发现这种写法跟求树的最大深度很像，参见代码如下：



解法三：

```
class Solution {
public:
    int longestUnivaluePath(TreeNode* root) {
        if (!root) return 0;
        int sub = max(longestUnivaluePath(root->left), longestUnivaluePath(root->right));
        return max(sub, helper(root->left, root->val) + helper(root->right, root->val));
    }
    int helper(TreeNode* node, int parent) {
        if (!node || node->val != parent) return 0;
        return 1 + max(helper(node->left, node->val), helper(node->right, node->val));
    }
};
```



类似题目：

[Binary Tree Maximum Path Sum](http://www.cnblogs.com/grandyang/p/4280120.html)

[Count Univalue Subtrees](http://www.cnblogs.com/grandyang/p/5206862.html)

[Path Sum III](http://www.cnblogs.com/grandyang/p/6007336.html)



参考资料：

[https://leetcode.com/problems/longest-univalue-path/](https://leetcode.com/problems/longest-univalue-path/)

[https://leetcode.com/problems/longest-univalue-path/discuss/108136/JavaC%2B%2B-Clean-Code](https://leetcode.com/problems/longest-univalue-path/discuss/108136/JavaC%2B%2B-Clean-Code)

[https://leetcode.com/problems/longest-univalue-path/discuss/108175/java-solution-with-global-variable](https://leetcode.com/problems/longest-univalue-path/discuss/108175/java-solution-with-global-variable)

[https://leetcode.com/problems/longest-univalue-path/discuss/108146/Concise-DFS-solution-with-no-global-variables](https://leetcode.com/problems/longest-univalue-path/discuss/108146/Concise-DFS-solution-with-no-global-variables)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












