
# 108. Convert Sorted Array to Binary Search Tree - OraYang的博客 - CSDN博客

2017年07月20日 10:59:01[OraYang](https://me.csdn.net/u010665216)阅读数：418所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given an array where elements are sorted in ascending order, convert it to a height balanced BST.
思路：
做这道题目前得理解高度平衡二叉树的定义：[点击打开链接](https://en.wikipedia.org/wiki/Self-balancing_binary_search_tree)，
![](https://img-blog.csdn.net/20170720110953829?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170720111006869?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
本题解题思路还是从递归出发，但是要结合二叉查找树的定义，每次将数组二分中间值作为根节点，依次递归二分并将中间值赋值给根节点，最后返回根结点，递归返回的临界条件是：左索引大于右索引
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
    TreeNode* sortedArrayToBST(vector<int>& nums) {
        if(nums.size()==0)
            return NULL;
        return sortedSub(nums,0,nums.size()-1);
        
    }
private: 
    TreeNode* sortedSub(vector<int>& nums, int l, int r)  {  
    if(l>r)  
        return NULL;  
    int m = (l+r)/2;  
    TreeNode* root = new TreeNode(nums[m]);  
    root->left = sortedSub(nums,l,m-1);  
    root->right = sortedSub(nums,m+1,r);  
    return root;  
}  
};
```



