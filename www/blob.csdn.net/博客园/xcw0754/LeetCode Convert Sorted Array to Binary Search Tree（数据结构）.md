# LeetCode  Convert Sorted Array to Binary Search Tree（数据结构） - xcw0754 - 博客园
# [LeetCode  Convert Sorted Array to Binary Search Tree（数据结构）](https://www.cnblogs.com/xcw0754/p/4924166.html)
题意：
　　将一个有序的数组建成一棵平衡的BST树。
思路：
　　因为数组已经有序，每次可以从中点开始建根，再递归下去分别处理左/右子树。
```
1 /**
 2  * Definition for a binary tree node.
 3  * struct TreeNode {
 4  *     int val;
 5  *     TreeNode *left;
 6  *     TreeNode *right;
 7  *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 8  * };
 9  */
10 class Solution {
11 public:
12     
13     TreeNode* DFS(vector<int>& nums,int L,int R)
14     {
15         if(L>R)        return NULL;
16         int mid=(L+R)>>1;
17         TreeNode* t=new TreeNode(nums[mid]);
18         t->left=DFS(nums, L, mid-1);
19         t->right=DFS(nums, mid+1, R);    
20         return t;
21     }
22 
23     TreeNode* sortedArrayToBST(vector<int>& nums) {
24         return DFS(nums,0,nums.size()-1);        
25     }
26 };
AC代码
```

