# [LeetCode] Two Sum IV - Input is a BST 两数之和之四 - 输入是二叉搜索树 - Grandyang - 博客园







# [[LeetCode] Two Sum IV - Input is a BST 两数之和之四 - 输入是二叉搜索树](https://www.cnblogs.com/grandyang/p/7508169.html)







Given a Binary Search Tree and a target number, return true if there exist two elements in the BST such that their sum is equal to the given target.

Example 1:
Input: 
    5
   / \
  3   6
 / \   \
2   4   7

Target = 9

Output: True



Example 2:
Input: 
    5
   / \
  3   6
 / \   \
2   4   7

Target = 28

Output: False


这道题又是一道2sum的变种题，博主一直强调，平生不识TwoSum，刷尽LeetCode也枉然！只要是两数之和的题，一定要记得先尝试用HashSet来做，这道题只不过是把数组变成了一棵二叉树而已，换汤不换药，我们遍历二叉树就行，然后用一个HashSet，在递归函数函数中，如果node为空，返回false。如果k减去当前结点值在HashSet中存在，直接返回true；否则就将当前结点值加入HashSet，然后对左右子结点分别调用递归函数并且或起来返回即可，参见代码如下：



解法一：

```
class Solution {
public:
    bool findTarget(TreeNode* root, int k) {
        unordered_set<int> st;
        return helper(root, k, st);
    }
    bool helper(TreeNode* node, int k, unordered_set<int>& st) {
        if (!node) return false;
        if (st.count(k - node->val)) return true;
        st.insert(node->val);
        return helper(node->left, k, st) || helper(node->right, k, st);
    }
};
```



我们也可以用层序遍历来做，这样就是迭代的写法了，但是利用HashSet的精髓还是没变的，参见代码如下：



解法二：

```
class Solution {
public:
    bool findTarget(TreeNode* root, int k) {
        if (!root) return false;
        unordered_set<int> st;
        queue<TreeNode*> q{{root}};
        while (!q.empty()) {
            auto t = q.front(); q.pop();
            if (st.count(k - t->val)) return true;
            st.insert(t->val);
            if (t->left) q.push(t->left);
            if (t->right) q.push(t->right);
        }
        return false;
    }
};
```



由于输入是一棵二叉搜索树，那么我们可以先用中序遍历得到一个有序数组，然后在有序数组中找两数之和就很简单了，直接用双指针进行遍历即可，参见代码如下：



解法三：

```
class Solution {
public:
    bool findTarget(TreeNode* root, int k) {
        vector<int> nums;
        inorder(root, nums);
        for (int i = 0, j = (int)nums.size() - 1; i < j;) {
            if (nums[i] + nums[j] == k) return true;
            (nums[i] + nums[j] < k) ? ++i : --j;
        }
        return false;
    }
    void inorder(TreeNode* node, vector<int>& nums) {
        if (!node) return;
        inorder(node->left, nums);
        nums.push_back(node->val);
        inorder(node->right, nums);
    }
};
```



类似题目：

[Two Sum III - Data structure design](http://www.cnblogs.com/grandyang/p/5184143.html)

[Two Sum II - Input array is sorted](http://www.cnblogs.com/grandyang/p/5185815.html)

[Two Sum](http://www.cnblogs.com/grandyang/p/4130379.html)



参考资料：

[https://leetcode.com/problems/two-sum-iv-input-is-a-bst/](https://leetcode.com/problems/two-sum-iv-input-is-a-bst/)

[https://leetcode.com/problems/two-sum-iv-input-is-a-bst/discuss/106090/my-c-python-solution](https://leetcode.com/problems/two-sum-iv-input-is-a-bst/discuss/106090/my-c-python-solution)

[https://leetcode.com/problems/two-sum-iv-input-is-a-bst/discuss/106059/JavaC%2B%2B-Three-simple-methods-choose-one-you-like](https://leetcode.com/problems/two-sum-iv-input-is-a-bst/discuss/106059/JavaC%2B%2B-Three-simple-methods-choose-one-you-like)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












