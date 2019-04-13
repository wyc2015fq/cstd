
# 637. Average of Levels in Binary Tree - OraYang的博客 - CSDN博客

2017年08月30日 08:58:17[OraYang](https://me.csdn.net/u010665216)阅读数：216标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given a non-empty binary tree, return the average value of the nodes on each level in the form of an array.
Example 1:
Input:3
   / \
  9  20
    /  \
   15   7Output:[3, 14.5, 11]Explanation:The average value of nodes on level 0 is 3,  on level 1 is 14.5, and on level 2 is 11. Hence return [3, 14.5, 11].
Note:
The range of node's value is in the range of 32-bit signed integer.
思路：本题具体思路是使用二叉树的层次遍历法，具体可以参考以前写的一篇博客：[107.Binary Tree Level Order Traversal II](http://blog.csdn.net/u010665216/article/details/75353897)要注意的是，在求平均值时，求和可能会溢出，所以代码中将数据类型定义为long,详情请查看[accumulate](http://en.cppreference.com/w/cpp/algorithm/accumulate)的相关用法及注意点
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
    vector<double> averageOfLevels(TreeNode* root) {     
    vector<double> res;  
    vector<long> temp;  
    if(root == NULL)  
    {  
        temp.push_back(0);  
        res.push_back(0.0);  
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
        long sum = accumulate(temp.begin(),temp.end(),(long)0);
        res.push_back(sum*1.0/temp.size());  
        temp.clear();  
    }    
    return res;   
    }
    
};
```


