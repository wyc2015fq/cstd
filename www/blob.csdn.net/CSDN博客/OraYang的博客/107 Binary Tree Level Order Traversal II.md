
# 107.Binary Tree Level Order Traversal II - OraYang的博客 - CSDN博客

2017年07月19日 11:32:12[OraYang](https://me.csdn.net/u010665216)阅读数：170所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given a binary tree, return thebottom-up level ordertraversal of its nodes' values. (ie, from left to right, level by level from leaf to root).
For example:
Given binary tree`[3,9,20,null,null,15,7]`,
3
   / \
  9  20
    /  \
   15   7
return its bottom-up level order traversal as:
[
  [15,7],
  [9,20],
  [3]
]思路：本题思路是层次遍历，只需对前一篇博客**[Maximum Depth of Binary Tree ](http://blog.csdn.net/u010665216/article/details/75349532)**的函数稍作修改，直接调用即可代码：

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
    vector<vector<int>> levelOrderBottom(TreeNode* root) {
    vector<vector<int>> res;
    vector<int> temp;
    if(root == NULL)
    {
        temp.push_back(0);
        res.push_back(temp);
        res.clear();
        temp.clear();
       return res; 
    }
          
    queue<TreeNode *> q;  
    q.push(root);
    while(!q.empty())  
    {
        
        for(int i = 0, n = q.size(); i < n; ++ i)  
        {  
            TreeNode *p = q.front();
            temp.push_back(p->val);
            q.pop();    
            if(p -> left != NULL)  
                q.push(p -> left);  
            if(p -> right != NULL)  
                q.push(p -> right);  
        }  
        res.insert(res.begin(),temp);
        temp.clear();
    }  
  
    return res;  
    }
};
```



