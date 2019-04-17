# [LeetCode] Trim a Binary Search Tree 修剪一棵二叉搜索树 - Grandyang - 博客园







# [[LeetCode] Trim a Binary Search Tree 修剪一棵二叉搜索树](https://www.cnblogs.com/grandyang/p/7583185.html)







Given a binary search tree and the lowest and highest boundaries as `L` and `R`, trim the tree so that all its elements lies in `[L, R]` (R >= L). You might need to change the root of the tree, so the result should return the new root of the trimmed binary search tree.

Example 1:
Input: 
    1
   / \
  0   2

  L = 1
  R = 2

Output: 
    1
      \
       2



Example 2:
Input: 
    3
   / \
  0   4
   \
    2
   /
  1

  L = 1
  R = 3

Output: 
      3
     / 
   2   
  /
 1


这道题让我们修剪一棵二叉搜索树，给了个边界范围[L, R], 所有不在这个范围内的结点应该被移除掉，但是仍需要保留二叉搜索树的性质，即左<根<右，有时候是小于等于。博主最开始的想法是先遍历一遍二叉树，将在返回内的结点值都放到一个数组后，遍历结束后再根据数组重建一棵二叉搜索树。这种方法会在某些test case上fail掉，可能会改变原来的二叉搜索树的结构，所以我们只能换一种思路。正确方法其实应该是在遍历的过程中就修改二叉树，移除不合题意的结点。当然对于二叉树的题，十有八九都是要用递归来解的。首先判断如果root为空，那么直接返回空即可。然后就是要看根结点是否在范围内，如果根结点值小于L，那么返回对其右子结点调用递归函数的值；如果根结点大于R，那么返回对其左子结点调用递归函数的值。如果根结点在范围内，将其左子结点更新为对其左子结点调用递归函数的返回值，同样，将其右子结点更新为对其右子结点调用递归函数的返回值。最后返回root即可，参见代码如下：



解法一：

```
class Solution {
public:
    TreeNode* trimBST(TreeNode* root, int L, int R) {
        if (!root) return NULL;
        if (root->val < L) return trimBST(root->right, L, R);
        if (root->val > R) return trimBST(root->left, L, R);
        root->left = trimBST(root->left, L, R);
        root->right = trimBST(root->right, L, R);
        return root;
    }
};
```



下面这种方法是迭代的写法，虽然树的题一般都是用递归来写，简洁又美观。但是我们也可以强行用while来代替递归，比如下面这种写法：



解法二：

```
class Solution {
public:
    TreeNode* trimBST(TreeNode* root, int L, int R) {
        if (!root) return NULL;
        while (root->val < L || root->val > R) {
            root = (root->val < L) ? root->right : root->left;
        }
        TreeNode *cur = root;
        while (cur) {
            while (cur->left && cur->left->val < L) {
                cur->left = cur->left->right;
            }
            cur = cur->left;
        }
        cur = root;
        while (cur) {
            while (cur->right && cur->right->val > R) {
                cur->right = cur->right->left;
            }
            cur = cur->right;
        }
        return root;
    }
};
```



参考资料：

[https://discuss.leetcode.com/topic/102034/java-solution-6-liner](https://discuss.leetcode.com/topic/102034/java-solution-6-liner)

[https://discuss.leetcode.com/topic/104140/java-solution-iteration-version](https://discuss.leetcode.com/topic/104140/java-solution-iteration-version)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












