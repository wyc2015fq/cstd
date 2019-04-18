# Leetcode:convert_sorted_array_to_binary_search_tree - weixin_33985507的博客 - CSDN博客
2015年10月17日 16:00:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4

一、     称号
排序后的数组成二叉搜索树。
二、     分析
   BST的中序遍历是一个sorted-array，再构造回去成一个BST,先将中间的元素作为根节点，这个节点的左右各自是左子树和右子树。如此递归地进行就可以。
```cpp
/**
 * Definition for binary tree
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public: 
	   TreeNode *sortedArrayToBST(vector<int> &num) {
	    return addNode(num, 0, num.size()-1);
	   }
	   TreeNode *addNode(vector<int> &num, int start, int end){
	    if(start > end) return NULL;
	    
	    int mid = (start + end)/2;
	    TreeNode *root = new TreeNode(num[mid]);
	    root->left = addNode(num, start, mid-1);
	    root->right = addNode(num, mid+1, end);
	    return root;
	   }
};
```
版权声明：本文博主原创文章。博客，未经同意不得转载。
