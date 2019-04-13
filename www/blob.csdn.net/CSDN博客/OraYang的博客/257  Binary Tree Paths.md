
# 257. Binary Tree Paths - OraYang的博客 - CSDN博客

2017年08月03日 09:29:48[OraYang](https://me.csdn.net/u010665216)阅读数：91标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given a binary tree, return all root-to-leaf paths.
For example, given the following binary tree:
1
 /   \
2     3
 \
  5
All root-to-leaf paths are:
["1->2->5", "1->3"]思路：本题还是利用递归的思想，具体思路可以参照以前写的一篇博客[Path Sum](http://blog.csdn.net/u010665216/article/details/75862002)，此处唯一要注意的是整形转换成字符串型to_sting()的使用
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
    vector<string> binaryTreePaths(TreeNode* root) {
        vector<string> str(0);
        if (root==NULL)
            return str;        
        treetolist(root,str,to_string(root->val));
        return str;
    }
private:
    void treetolist(TreeNode* root,vector<string>& res,string s){
        if(root->left==NULL&&root->right==NULL)
        {
            res.push_back(s);  
            return;
        }
        if(root->left)
            treetolist(root->left,res, s + "->" + to_string(root->left->val));
        if(root->right)
            treetolist(root->right,res,s + "->" + to_string(root->right->val));
    }
};
```



