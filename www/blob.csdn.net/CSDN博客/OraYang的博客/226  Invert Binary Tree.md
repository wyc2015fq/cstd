
# 226. Invert Binary Tree - OraYang的博客 - CSDN博客

2017年08月01日 11:26:28[OraYang](https://me.csdn.net/u010665216)阅读数：116标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Invert a binary tree.
4
   /   \
  2     7
 / \   / \
1   3 6   9to4
   /   \
  7     2
 / \   / \
9   6 3   1思路：1.层次遍历法，每层遍历就替换左右结点；
2.递归法
代码（层次遍历法）：

```python
class Solution {
public:
    TreeNode* invertTree(TreeNode* root) {
        if(root==NULL)
            return root;
        queue<TreeNode *> q;
        q.push(root);
        while(!q.empty())
        {
            for(int i = 0, n = q.size(); i < n; ++ i)  
        {  
            TreeNode *temp;
            TreeNode *p = q.front(); 
            q.pop();
            temp = p->left;
            p->left = p->right;
            p->right = temp;
            if(p -> left != NULL){
                q.push(p -> left);
            }  
                  
            if(p -> right != NULL) 
            {
                q.push(p -> right);
            }
                  
        }
        }
        return root;
        
    }
};
```
代码（递归法）：
```python
class Solution {
public:
    TreeNode* invertTree(TreeNode* root) {
        if(root==NULL)  
            return NULL;  
        TreeNode * temp = root->left;  
        root->left = invertTree(root->right);  
        root->right = invertTree(temp);  
        return root;
    }
};
```



