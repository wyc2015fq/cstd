
# 501. Find Mode in Binary Search Tree - OraYang的博客 - CSDN博客

2017年08月22日 14:56:17[OraYang](https://me.csdn.net/u010665216)阅读数：136标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given a binary search tree (BST) with duplicates, find all the[mode(s)](https://en.wikipedia.org/wiki/Mode_(statistics))(the most frequently occurred element)
 in the given BST.
Assume a BST is defined as follows:
The left subtree of a node contains only nodes with keysless than or equal tothe node's key.
The right subtree of a node contains only nodes with keysgreater than or equal tothe node's key.
Both the left and right subtrees must also be binary search trees.
For example:
Given BST`[1,null,2,2]`,
1
    \
     2
    /
   2
return`[2]`.
Note:If a tree has more than one mode, you can return them in any order.
Follow up:Could you do that without using any extra space? (Assume that the implicit stack space incurred due to recursion does not count).
思路：本题主要考查的是二叉树的遍历，在遍历过程中，维护一个map数据结构来统计二叉树结点的频繁项，再求出众数
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
    vector<int> findMode(TreeNode* root) {
        unordered_map<int,int> res;
        vector<int> out;
        PreOrder(root,res);
        int max =0;
        unordered_map<int,int>::iterator itr=res.begin();
        while(itr!=res.end())
        {
            max = itr->second>max?itr->second:max;
            itr++;
        }   
        itr = res.begin();
        while(itr!=res.end())
        {
            if(itr->second==max)
            {
                out.push_back(itr->first);
            }
            itr++;
        }
        return out;
        
    }
private:
    void PreOrder(TreeNode* T,unordered_map<int,int>& mp){
    if(T != NULL){  
        //访问根节点  
        mp[T->val]++;  
        //访问左子结点  
        PreOrder(T->left,mp);  
        //访问右子结点  
        PreOrder(T->right,mp);  
    }
}
};
```


