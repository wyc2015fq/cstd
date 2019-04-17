# [LeetCode] Convert Sorted Array to Binary Search Tree 将有序数组转为二叉搜索树 - Grandyang - 博客园







# [[LeetCode] Convert Sorted Array to Binary Search Tree 将有序数组转为二叉搜索树](https://www.cnblogs.com/grandyang/p/4295245.html)







Given an array where elements are sorted in ascending order, convert it to a height balanced BST.

For this problem, a height-balanced binary tree is defined as a binary tree in which the depth of the two subtrees of *every*node never differ by more than 1.

Example:
Given the sorted array: [-10,-3,0,5,9],

One possible answer is: [0,-3,9,-10,null,5], which represents the following height balanced BST:

      0
     / \
   -3   9
   /   /
 -10  5


这道题是要将有序数组转为二叉搜索树，所谓[二叉搜索树](http://zh.wikipedia.org/wiki/%E4%BA%8C%E5%85%83%E6%90%9C%E5%B0%8B%E6%A8%B9)，是一种始终满足左<根<右的特性，如果将二叉搜索树按中序遍历的话，得到的就是一个有序数组了。那么反过来，我们可以得知，根节点应该是有序数组的中间点，从中间点分开为左右两个有序数组，在分别找出其中间点作为原中间点的左右两个子节点，这不就是是二分查找法的核心思想么。所以这道题考的就是二分查找法，代码如下：



解法一：

```
class Solution {
public:
    TreeNode* sortedArrayToBST(vector<int>& nums) {
        return helper(nums, 0 , (int)nums.size() - 1);
    }
    TreeNode* helper(vector<int>& nums, int left, int right) {
        if (left > right) return NULL;
        int mid = left + (right - left) / 2;
        TreeNode *cur = new TreeNode(nums[mid]);
        cur->left = helper(nums, left, mid - 1);
        cur->right = helper(nums, mid + 1, right);
        return cur;
    }
};
```



我们也可以不使用额外的递归函数，而是在原函数中完成递归，由于原函数的参数是一个数组，所以当把输入数组的中间数字取出来后，需要把所有两端的数组组成一个新的数组，并且分别调用递归函数，并且连到新创建的cur结点的左右子结点上面，参见代码如下：



解法二：

```
class Solution {
public:
    TreeNode* sortedArrayToBST(vector<int>& nums) {
        if (nums.empty()) return NULL;
        int mid = nums.size() / 2;
        TreeNode *cur = new TreeNode(nums[mid]);
        vector<int> left(nums.begin(), nums.begin() + mid), right(nums.begin() + mid + 1, nums.end());
        cur->left = sortedArrayToBST(left);
        cur->right = sortedArrayToBST(right);
        return cur;
    }
};
```



类似题目：

[Convert Sorted List to Binary Search Tree](http://www.cnblogs.com/grandyang/p/4295618.html)



参考资料：

[https://leetcode.com/problems/convert-sorted-array-to-binary-search-tree/](https://leetcode.com/problems/convert-sorted-array-to-binary-search-tree/)

[https://leetcode.com/problems/convert-sorted-array-to-binary-search-tree/discuss/35220/My-Accepted-Java-Solution](https://leetcode.com/problems/convert-sorted-array-to-binary-search-tree/discuss/35220/My-Accepted-Java-Solution)

[https://leetcode.com/problems/convert-sorted-array-to-binary-search-tree/discuss/35394/6-lines-Java-Accepted-Solution](https://leetcode.com/problems/convert-sorted-array-to-binary-search-tree/discuss/35394/6-lines-Java-Accepted-Solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












