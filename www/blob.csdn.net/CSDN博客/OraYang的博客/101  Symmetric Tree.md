
# 101. Symmetric Tree - OraYang的博客 - CSDN博客

2017年07月19日 09:45:04[OraYang](https://me.csdn.net/u010665216)阅读数：170标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given a binary tree, check whether it is a mirror of itself (ie, symmetric around its center).
For example, this binary tree`[1,2,2,3,4,4,3]`is symmetric:
1
   / \
  2   2
 / \ / \
3  4 4  3
But the following`[1,2,2,null,3,null,3]`is not:
1
   / \
  2   2
   \   \
   3    3
Note:
Bonus points if you could solve it both recursively and iteratively.
思路：这道题目，其实很简单，只要稍稍修改我前一篇博客的函数，并调用就好了**[Same Tree](http://blog.csdn.net/u010665216/article/details/75282068)**
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
    bool isSameTree(TreeNode* p, TreeNode* q) {  
        if(p==NULL&&q==NULL)  
            return true;  
        else if(p==NULL&&q!=NULL)  
            return false;  
        else if(p!=NULL&&q==NULL)  
            return false;  
        else  
        {  
            if(p->val!=q->val)  
                return false;  
            else  
                return isSameTree(p->left,q->right)&&isSameTree(p->right,q->left);  
        }  
          
    }  
    bool isSymmetric(TreeNode* root) {
        TreeNode* temp = root;
        return isSameTree(root,temp);       
    }
};
```


