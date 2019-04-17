# [CareerCup] 4.3 Create Minimal Binary Search Tree 创建最小二叉搜索树 - Grandyang - 博客园







# [[CareerCup] 4.3 Create Minimal Binary Search Tree 创建最小二叉搜索树](https://www.cnblogs.com/grandyang/p/4693427.html)







4.3 Given a sorted (increasing order) array with unique integer elements, write an algorithm to create a binary search tree with minimal height.



这道题给了我们一个有序的数组，让我们来生成一个最小高度的二叉搜索树，为了达到最小高度，肯定是尽可能的填成一个满二叉树，左子树填满，右子树尽可能的填满。而且要注意是二叉搜索树，左<根<右的性质不能忘。既然给了我们一个有序的数组，那么我们可以取中间的数字为根节点，然后左半段为左子树，右半段为右子树，然后再递归去分别再分，有点像二叉搜索法的原理，代码不复杂，也不难懂，如下所示：



```
class Solution {
public:
    TreeNode* createMinimalBST(vector<int> &nums) {
        return createMinimalBST(nums, 0, nums.size() - 1);
    }
    TreeNode* createMinimalBST(vector<int> &nums, int start, int end) {
        if (start > end) return NULL;
        int mid = (start + end) / 2;
        TreeNode *node = new TreeNode(nums[mid]);
        node->left = createMinimalBST(nums, start, mid - 1);
        node->right = createMinimalBST(nums, mid + 1, end);
        return node;
    }
};
```














