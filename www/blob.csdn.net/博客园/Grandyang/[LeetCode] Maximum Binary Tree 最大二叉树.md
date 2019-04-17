# [LeetCode] Maximum Binary Tree 最大二叉树 - Grandyang - 博客园







# [[LeetCode] Maximum Binary Tree 最大二叉树](https://www.cnblogs.com/grandyang/p/7513099.html)








Given an integer array with no duplicates. A maximum tree building on this array is defined as follow:
- The root is the maximum number in the array.
- The left subtree is the maximum tree constructed from left part subarray divided by the maximum number.
- The right subtree is the maximum tree constructed from right part subarray divided by the maximum number.



Construct the maximum tree by the given array and output the root node of this tree.

Example 1:
Input: [3,2,1,6,0,5]
Output: return the tree root node representing the following tree:

      6
    /   \
   3     5
    \    / 
     2  0   
       \
        1



Note:
- The size of the given array will be in the range [1,1000].



这道题给了我们一个数组，让我们创建一个最大二叉树，创建规则是数组中的最大值为根结点，然后分隔出的左右部分再分别创建最大二叉树。那么明眼人一看就知道这是分治法啊，果断上递归啊。首先就是要先找出数组中的最大值，由于数组是无序的，所以没啥好的办法，就直接遍历吧，找到了最大值，就创建一个结点，然后将左右两个子数组提取出来，分别调用递归函数并将结果连到该结点上，最后将结点返回即可，参见代码如下：



解法一：

```
class Solution {
public:
    TreeNode* constructMaximumBinaryTree(vector<int>& nums) {
        if (nums.empty()) return NULL;
        int mx = INT_MIN, mx_idx = 0;
        for (int i = 0; i < nums.size(); ++i) {
            if (mx < nums[i]) {
                mx = nums[i];
                mx_idx = i;
            }
        }
        TreeNode *node = new TreeNode(mx);
        vector<int> leftArr = vector<int>(nums.begin(), nums.begin() + mx_idx);
        vector<int> rightArr = vector<int>(nums.begin() + mx_idx + 1, nums.end());
        node->left = constructMaximumBinaryTree(leftArr);
        node->right = constructMaximumBinaryTree(rightArr);
        return node;
    }
};
```



下面这种方法也是递归的解法，和上面的解法稍有不同的是不必提取子数组，而是用两个变量来指定子数组的范围，其他部分均和上面的解法相同，参见代码如下：



解法二：

```
class Solution {
public:
    TreeNode* constructMaximumBinaryTree(vector<int>& nums) {
        if (nums.empty()) return NULL;
        return helper(nums, 0, nums.size() - 1);
    }
    TreeNode* helper(vector<int>& nums, int left, int right) {
        if (left > right) return NULL;
        int mid = left;
        for (int i = left + 1; i <= right; ++i) {
            if (nums[i] > nums[mid]) {
                mid = i;
            }
        }
        TreeNode *node = new TreeNode(nums[mid]);
        node->left = helper(nums, left, mid - 1);
        node->right = helper(nums, mid + 1, right);
        return node;
    }
};
```



下面这种解法是论坛上的高分解法，使用到了一个辅助数组v来让保持降序。我们遍历数组，对于每个遍历到的数字，创建一个结点，然后进行循环，如果数组v不空，且末尾结点值小于当前数字，那么将末尾结点连到当前结点的左子结点，并且移除数组中的末尾结点，这样可以保证子结点都会小于父结点。循环结束后，如果此时数组v仍不为空，说明结点值很大，那么将当前结点连到数组末尾结点的右子结点上。之后别忘了将当前结点加入数组v中，最后返回数组v的首结点即可，如果不太容易理解的话，就把题目中的例子带入一步一步运行看一下吧，参见代码如下：



解法三：

```
class Solution {
public:
    TreeNode* constructMaximumBinaryTree(vector<int>& nums) {
        vector<TreeNode*> v;
        for (int num : nums) {
            TreeNode *cur = new TreeNode(num);
            while (!v.empty() && v.back()->val < num) {
                cur->left = v.back();
                v.pop_back();
            }
            if (!v.empty()) {
                v.back()->right = cur;
            }
            v.push_back(cur);
        }
        return v.front();
    }
};
```



参考资料： 


[https://discuss.leetcode.com/topic/98509/c-o-n-solution](https://discuss.leetcode.com/topic/98509/c-o-n-solution)

[https://discuss.leetcode.com/topic/98457/java-c-simple-recursive-method](https://discuss.leetcode.com/topic/98457/java-c-simple-recursive-method)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












