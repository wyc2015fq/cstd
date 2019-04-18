# LeetCode Binary Tree Preorder Traversal  先根遍历 - xcw0754 - 博客园
# [LeetCode Binary Tree Preorder Traversal  先根遍历](https://www.cnblogs.com/xcw0754/p/4674723.html)
题意：给一棵树，求其先根遍历的结果。
思路：
（1）深搜法：
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
12     vector<int> preorderTraversal(TreeNode* root) {
13         if(!root)  return vector<int>();
14         vector<int> ans(1,root->val);
15         vector<int> tmp=preorderTraversal(root->left);
16         ans.insert(ans.end(),tmp.begin(),tmp.end());
17         tmp=preorderTraversal(root->right);
18         ans.insert(ans.end(),tmp.begin(),tmp.end());
19         return ans;
20     }
21 
22 };
AC代码
```
（2）依然是深搜法：
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
12     vector<int> ans;
13     void DFS(TreeNode* t)
14     {
15         if(!t)  return;
16         ans.push_back(t->val);
17         DFS(t->left);
18         DFS(t->right);
19     }
20     vector<int> preorderTraversal(TreeNode* root) {
21         DFS(root);
22         return ans;
23     }
24 };
AC代码
```
（3）迭代法：
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
12     vector<int> preorderTraversal(TreeNode* root) {
13         if(!root)  return vector<int>();
14 
15         stack<pair<TreeNode*,int>>  stac;
16         stac.push(make_pair(root,0));//第二个参数用于记录其已经遍历了左/右孩子
17         vector<int> ans;
18         while( !stac.empty() )
19         {
20             TreeNode* cur=stac.top().first;
21             if( stac.top().second==0 )  ans.push_back(cur->val);
22 
23             if(cur->left && stac.top().second==0)//没有遍历过孩子的进来
24             {
25                 cur=cur->left;
26                 stac.top().second=1;
27                 stac.push(make_pair(cur,0));
28             }
29             else if(cur->right && stac.top().second<2)//遍历过左孩子或者没有左孩子的才进来
30             {
31                 cur=cur->right;
32                 stac.top().second=2;
33                 stac.push(make_pair(cur,0));
34             }
35             else    stac.pop();//以上两个都进不去，要么遍历完，要么没有孩子
36         }
37         return ans;
38     }
39 };
AC代码
```
（4）更叼的迭代法：
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
12     vector<int> preorderTraversal(TreeNode* root) {
13         if(!root)  return vector<int>();
14 
15         vector<int> ans;
16         stack<TreeNode *>  stac;
17         stac.push(root);
18 
19         while( !stac.empty() )
20         {
21             TreeNode *t=stac.top();
22             ans.push_back(t->val);
23             stac.pop();//只需要孩子都压栈，父亲无用
24             if(t->right)    stac.push(t->right);
25             if(t->left)     stac.push(t->left);
26         }
27         return ans;
28     }
29 };
AC代码
```

