
# 538. Convert BST to Greater Tree - OraYang的博客 - CSDN博客

2017年08月24日 11:32:36[OraYang](https://me.csdn.net/u010665216)阅读数：97标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given a Binary Search Tree (BST), convert it to a Greater Tree such that every key of the original BST is changed to the original key plus sum of all keys greater than the original key in BST.
Example:
Input:The root of a Binary Search Tree like this:
              5
            /   \
           2     13Output:The root of a Greater Tree like this:
             18
            /   \
          20     13思路：对于树结构的题目，本题使用递归的方法，步骤如下：
step1:中序遍历，存储得升序的数组
step2;中序遍历，赋值
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
    TreeNode* convertBST(TreeNode* root) {
        vector<int> res;
        InOrder(root,res);
        for(int i =0;i<res.size();i++)
        {
            for(int j=i+1;j<res.size();j++)
            {
                res[i] +=res[j]; 
            }
        }
        #sort(res.begin(),res.end());
        int i= 0;
        InOrder_change(root,res,i);
        return root;
        
    }
private:  
    void InOrder(TreeNode* T,vector<int>& res){    
    if(T != NULL){    
        //访问左子结点    
        InOrder(T->left,res);    
        //访问根节点    
        res.push_back(T->val);  
        //访问右子结点    
        InOrder(T->right,res);    
    }    
}  
    void InOrder_change(TreeNode* T, vector<int>& res,int& i){
    if(T != NULL){    
        //访问左子结点
        
        InOrder_change(T->left,res,i);    
        //访问根节点 
        T->val = res[i];
        i++;
        //访问右子结点
        InOrder_change(T->right,res,i);    
    }    
} 
};
```


