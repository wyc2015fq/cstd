# LeetCode Binary Tree Level Order Traversal  (按层收集元素) - xcw0754 - 博客园
# [LeetCode Binary Tree Level Order Traversal  (按层收集元素)](https://www.cnblogs.com/xcw0754/p/4638455.html)
题意：按层，将元素收集在一个二维数组中。
思路：广搜应该是普遍的方法了。还能避免栈溢出，多好用。搭配deque，因为要经常删除。
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
12     vector< vector<int> > levelOrder(TreeNode* root) {
13         vector< vector<int> > ans;
14         if(!root)   return ans;
15         deque<TreeNode*> que(1,root);
16         while(!que.empty())
17         {
18             ans.push_back(vector<int>());
19             int siz=que.size();
20             while(siz--)
21             {
22                 TreeNode* tmp=que.front();
23 
24                 if(tmp->left)   que.push_back(tmp->left);
25                 if(tmp->right)  que.push_back(tmp->right);
26 
27                 ans[ans.size()-1].push_back(tmp->val);
28                 que.pop_front();
29             }
30         }
31         return ans;
32     }
33 };
AC代码
```

