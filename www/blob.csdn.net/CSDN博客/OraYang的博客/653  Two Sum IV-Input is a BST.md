
# 653. Two Sum IV - Input is a BST - OraYang的博客 - CSDN博客

2017年08月31日 11:20:39[OraYang](https://me.csdn.net/u010665216)阅读数：229标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given a Binary Search Tree and a target number, return true if there exist two elements in the BST such that their sum is equal to the given target.
Example 1:
Input:5
   / \
  3   6
 / \   \
2   4   7
Target = 9Output:True
Example 2:
Input:5
   / \
  3   6
 / \   \
2   4   7
Target = 28Output:False思路：本题，先利用先序遍历将结点值存储在数组里，再用find函数查找（k-当前结点）是否存在
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
vector<int> res;
    bool findTarget(TreeNode* root, int k) {   
        Preorder(root);
        for(int i=0;i<res.size();i++)
        {
            int temp =k - res[i];
            if(find(res.begin(),res.end(),temp)!=res.begin()+i&&find(res.begin(),res.end(),temp)!=res.end())
                return true;
        }
        return false;
    }
private:
void Preorder(TreeNode* root){
    if(root!=NULL)
    {
        res.push_back(root->val);
        Preorder(root->left);
        Preorder(root->right);
    }
    
}
};
```


