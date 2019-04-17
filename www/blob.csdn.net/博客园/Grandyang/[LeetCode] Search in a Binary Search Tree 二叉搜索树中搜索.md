# [LeetCode] Search in a Binary Search Tree 二叉搜索树中搜索 - Grandyang - 博客园







# [[LeetCode] Search in a Binary Search Tree 二叉搜索树中搜索](https://www.cnblogs.com/grandyang/p/9912434.html)







Given the root node of a binary search tree (BST) and a value. You need to find the node in the BST that the node's value equals the given value. Return the subtree rooted with that node. If such node doesn't exist, you should return NULL.

For example, 
Given the tree:
        4
       / \
      2   7
     / \
    1   3

And the value to search: 2

You should return this subtree:
      2     
     / \   
    1   3

In the example above, if we want to search the value `5`, since there is no node with value `5`, we should return `NULL`.

Note that an empty tree is represented by `NULL`, therefore you would see the expected output (serialized tree format) as `[]`, not `null`.



这道题让我们搜索一个二叉搜索树，既然是二叉搜索树，而不是普通的二叉树，那么我们肯定要利用二叉搜索树特定的性质来解题，即左<根<右。那么就是说任意一个结点的左子树中的所有结点均小于当前结点，其右子树中的所有结点均大于当前结点。那么这不就是一个天然的二分么，当仁不让的二分搜索法呼之欲出啊。首先判空，如果当前结点不存在，直接返回空。如果当前结点值等于目标值，返回当前结点。接下来就看如果当前结点值大于目标值，则对左子结点调用递归函数，否则就对右子结点调用递归函数，参见代码如下：



解法一：

```
class Solution {
public:
    TreeNode* searchBST(TreeNode* root, int val) {
        if (!root) return NULL;
        if (root->val == val) return root;
        return (root->val > val) ? searchBST(root->left, val) : searchBST(root->right, val);
    }
};
```



我们也可以使用迭代形式来解，使用一个while循环，思路都是一样的，如果当前结点存在，且结点值不等于目标值，那么若结点值大于目标值，则当前结点指向其左子结点，否则指向其右子结点，参见代码如下：



解法二：

```
class Solution {
public:
    TreeNode* searchBST(TreeNode* root, int val) {
        while (root && root->val != val) {
            root = (root->val > val) ? root->left : root->right;
        }
        return root;
    }
};
```



类似题目：

[Closest Binary Search Tree Value](http://www.cnblogs.com/grandyang/p/5237170.html)

[Insert into a Binary Search Tree](https://www.cnblogs.com/grandyang/p/9914546.html)



参考资料：

[https://leetcode.com/problems/search-in-a-binary-search-tree/](https://leetcode.com/problems/search-in-a-binary-search-tree/)

[https://leetcode.com/problems/search-in-a-binary-search-tree/discuss/139687/Concise-iterative-solution-(C%2B%2B)](https://leetcode.com/problems/search-in-a-binary-search-tree/discuss/139687/Concise-iterative-solution-(C%2B%2B))

[https://leetcode.com/problems/search-in-a-binary-search-tree/discuss/149274/Java-beats-100-concise-method-using-recursion-and-iteration](https://leetcode.com/problems/search-in-a-binary-search-tree/discuss/149274/Java-beats-100-concise-method-using-recursion-and-iteration)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












