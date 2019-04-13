
# 563. Binary Tree Tilt - OraYang的博客 - CSDN博客

2017年08月26日 10:32:58[OraYang](https://me.csdn.net/u010665216)阅读数：125标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given a binary tree, return the tilt of thewhole tree.
The tilt of atree nodeis defined as theabsolute differencebetween the sum of all left subtree node values and the sum of all right subtree node values. Null node has tilt 0.
The tilt of thewhole treeis defined as the sum of all nodes' tilt.
Example:
Input:1
       /   \
      2     3Output:1Explanation:Tilt of node 2 : 0
Tilt of node 3 : 0
Tilt of node 1 : |2-3| = 1
Tilt of binary tree : 0 + 0 + 1 = 1
Note:
The sum of node values in any subtree won't exceed the range of 32-bit integer.
All the tilt values won't exceed the range of 32-bit integer.
思路：本题，本质上求当前左右子树结点和的差的绝对值，所以利用递归的思想
代码：

```python
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    int res =0;
    int findTilt(TreeNode* root) {
        sumTilt(root);
        return res;
    }
private:
    int sumTilt(TreeNode* root)
    {
        if(root==NULL)
            return 0;
        int left = sumTilt(root->left);
        int right = sumTilt(root->right);
        res += abs(left - right); 
        return left+right+root->val;
    }
};
```



