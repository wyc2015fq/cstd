# [LeetCode] Diameter of Binary Tree 二叉树的直径 - Grandyang - 博客园







# [[LeetCode] Diameter of Binary Tree 二叉树的直径](https://www.cnblogs.com/grandyang/p/6607318.html)







Given a binary tree, you need to compute the length of the diameter of the tree. The diameter of a binary tree is the length of the longestpath between any two nodes in a tree. This path may or may not pass through the root.

Example:
Given a binary tree 
          1
         / \
        2   3
       / \     
      4   5    



Return 3, which is the length of the path [4,2,1,3] or [5,2,1,3].

Note: The length of path between two nodes is represented by the number of edges between them.



这道题让我们求二叉树的直径，并告诉了我们直径就是两点之间的最远距离，根据题目中的例子也不难理解题意。我们再来仔细观察例子中的那两个最长路径[4,2,1,3] 和 [5,2,1,3]，我们转换一种角度来看，是不是其实就是根结点1的左右两个子树的深度之和呢。那么我们只要对每一个结点求出其左右子树深度之和，这个值作为一个候选值，然后再对左右子结点分别调用求直径对递归函数，这三个值相互比较，取最大的值更新结果res，因为直径不一定会经过根结点，所以才要对左右子结点再分别算一次。为了减少重复计算，我们用哈希表建立每个结点和其深度之间的映射，这样某个结点的深度之前计算过了，就不用再次计算了，参见代码如下：



解法一：

```
class Solution {
public:
    int diameterOfBinaryTree(TreeNode* root) {
        if (!root) return 0;
        int res = getHeight(root->left) + getHeight(root->right);
        return max(res, max(diameterOfBinaryTree(root->left), diameterOfBinaryTree(root->right)));
    }
    int getHeight(TreeNode* node) {
        if (!node) return 0;
        if (m.count(node)) return m[node];
        int h = 1 + max(getHeight(node->left), getHeight(node->right));
        return m[node] = h;
    }

private:
    unordered_map<TreeNode*, int> m;
};
```



上面的方法貌似有两个递归函数，其实我们只需要用一个递归函数就可以了，我们再求深度的递归函数中顺便就把直径算出来了，而且貌似不用进行优化也能通过OJ，参见代码如下：



解法二：

```
class Solution {
public:
    int diameterOfBinaryTree(TreeNode* root) {
        int res = 0;
        maxDepth(root, res);
        return res;
    }
    int maxDepth(TreeNode* node, int& res) {
        if (!node) return 0;
        int left = maxDepth(node->left, res);
        int right = maxDepth(node->right, res);
        res = max(res, left + right);
        return max(left, right) + 1;
    }
};
```



虽说不用进行优化也能通过OJ，但是毕竟还是优化一下好一点啊，参见代码如下：



解法三：

```
class Solution {
public:
    int diameterOfBinaryTree(TreeNode* root) {
        int res = 0;
        maxDepth(root, res);
        return res;
    }
    int maxDepth(TreeNode* node, int& res) {
        if (!node) return 0;
        if (m.count(node)) return m[node];
        int left = maxDepth(node->left, res);
        int right = maxDepth(node->right, res);
        res = max(res, left + right);
        return m[node] = (max(left, right) + 1);
    }

private:
    unordered_map<TreeNode*, int> m;
};
```



参考资料：

[https://leetcode.com/problems/diameter-of-binary-tree/description/](https://leetcode.com/problems/diameter-of-binary-tree/description/)

[https://leetcode.com/problems/diameter-of-binary-tree/discuss/101132/java-solution-maxdepth](https://leetcode.com/problems/diameter-of-binary-tree/discuss/101132/java-solution-maxdepth)

[https://leetcode.com/problems/diameter-of-binary-tree/discuss/101115/543-diameter-of-binary-tree-c_recursive_with-brief-explanation](https://leetcode.com/problems/diameter-of-binary-tree/discuss/101115/543-diameter-of-binary-tree-c_recursive_with-brief-explanation)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












