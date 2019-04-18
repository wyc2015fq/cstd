# LeetCode  Binary Tree Right Side View （DFS/BFS） - xcw0754 - 博客园
# [LeetCode  Binary Tree Right Side View （DFS/BFS）](https://www.cnblogs.com/xcw0754/p/4972800.html)
题意：
　　给一棵二叉树，要求收集每层的最后一个节点的值。按从顶到底装进vector返回。
思路：
　　BFS比较简单，先遍历右孩子就行了。
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
12     vector<int> rightSideView(TreeNode* root) {
13         if(root==NULL)    return vector<int>();
14         deque<TreeNode*> que(1,root);
15         vector<int> ans;
16 
17         while(!que.empty())
18         {
19             ans.push_back(que.front()->val);
20             for(int i=que.size(); i>0; i--)
21             {
22                 TreeNode* p=que.front();
23                 que.pop_front();
24                 if(p->right)    que.push_back(p->right);
25                 if(p->left)        que.push_back(p->left);
26             }
27         }
28         return ans;
29     }
30 };
AC代码
```
　　DFS比较技巧，需要知道其层次。
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
12     vector<int> rightSideView(TreeNode* root) {
13         vector<int> ans;
14         DFS(ans,root,1);    
15         return ans;
16     }
17     void DFS(vector<int>& ans,TreeNode* t,int depth)
18     {
19         if(t==NULL)    return;
20         if(depth>ans.size())    ans.push_back(t->val);
21         DFS(ans,t->right,depth+1);
22         DFS(ans,t->left,depth+1);
23     }
24 };
AC代码
```

