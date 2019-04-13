
# 112. Path Sum - OraYang的博客 - CSDN博客

2017年07月23日 10:30:13[OraYang](https://me.csdn.net/u010665216)阅读数：165标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[二叉树																](https://so.csdn.net/so/search/s.do?q=二叉树&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
[
																					](https://so.csdn.net/so/search/s.do?q=C++&t=blog)所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)



题目：
Given a binary tree and a sum, determine if the tree has a root-to-leaf path such that adding up all the values along the path equals the given sum.
For example:
Given the below binary tree and`sum
 = 22`,5
             / \
            4   8
           /   / \
          11  13  4
         /  \      \
        7    2      1return true, as there exist a root-to-leaf path`5->4->11->2`which sum is 22.
思路：本题依旧用递归实现，采用dfs，判断是否相等的临界条件：当前点为叶子结点。
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
    bool hasPathSum(TreeNode* root, int sum) {
        return sumRes(root,sum,0);
    }
private:
    bool sumRes(TreeNode* root,int sum,int cur){
        if(root==NULL)
            return false;
        if(root->left==NULL&&root->right==NULL)
            return (cur+root->val)==sum;
        return sumRes(root->left,sum,cur+root->val)||sumRes(root->right,sum,cur+root->val);
        
    }
};
```



