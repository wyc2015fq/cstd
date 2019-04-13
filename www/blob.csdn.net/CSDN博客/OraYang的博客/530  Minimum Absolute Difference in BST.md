
# 530. Minimum Absolute Difference in BST - OraYang的博客 - CSDN博客

2017年08月24日 09:02:52[OraYang](https://me.csdn.net/u010665216)阅读数：187标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目:
Given a binary search tree with non-negative values, find the minimum[absolute difference](https://en.wikipedia.org/wiki/Absolute_difference)between values of any two nodes.
Example:
Input:1
    \
     3
    /
   2Output:1Explanation:The minimum absolute difference is 1, which is the difference between 2 and 1 (or between 2 and 3).思路：本题思路比较简单，求任意结点间的差值绝对值，可以先利用中序遍历存储结点，因为中序遍历的结点是按升序排列的，所以只要比较相邻点的差值即可
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
    int getMinimumDifference(TreeNode* root) {
        vector<int> res;
        InOrder(root,res);
        int min =res.back(),diff;
        for(int i = 1;i<res.size();i++)
        {
            diff = res[i] - res[i-1];
            min = min < diff?min:diff;
        }
        return min;
        
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
};
```


