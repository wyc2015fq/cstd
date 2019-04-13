
# 404. Sum of Left Leaves - OraYang的博客 - CSDN博客

2017年08月10日 10:38:30[OraYang](https://me.csdn.net/u010665216)阅读数：106标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Find the sum of all left leaves in a given binary tree.
Example:
3
   / \
  9  20
    /  \
   15   7
There are two left leaves in the binary tree, with values9and15respectively. Return24.思路：本题用递归的思路，临界条件是当前结点的左节点为叶子节点
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
    int sumOfLeftLeaves(TreeNode* root) {
        int sum=0;
        sumres(root,sum);
        return sum;
        
    }
private:
    void sumres(TreeNode* root,int& sum)
    {
        if(root==NULL)
            return ;
        else if(root->left&&!root->left->left&&!root->left->right)
            sum+=root->left->val;
        sumres(root->left,sum);
        sumres(root->right,sum);
        return ;
    }
};
```


