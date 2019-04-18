# LeetCode Binary Tree Level Order Traversal II (二叉树颠倒层序) - xcw0754 - 博客园
# [LeetCode Binary Tree Level Order Traversal II (二叉树颠倒层序)](https://www.cnblogs.com/xcw0754/p/4619796.html)
![](https://images0.cnblogs.com/blog2015/641737/201507/032209452285843.png)
题意:从左到右统计将同一层的值放在同一个容器vector中，要求上下颠倒，左右不颠倒。
思路：广搜逐层添加进来，最后再反转。
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
12     vector< vector<int> > levelOrderBottom(TreeNode* root) {
13         vector< vector<int> >   ans;
14         if(!root)   return ans;
15         vector<int> tmp;
16         deque<TreeNode * >    que;
17         que.push_back(root);
18         while(!que.empty())     //广搜
19         {
20             int siz= que.size();
21             tmp.clear();
22             for(int i=0; i<siz; i++)
23             {
24                 TreeNode* v=que.front();que.pop_front();
25                 tmp.push_back(v->val);
26                 if(v->left)     que.push_back(v->left);
27                 if(v->right)    que.push_back(v->right);
28             }
29             ans.push_back(tmp);
30         }
31         reverse(ans.begin(), ans.end());
32         return ans;
33     }
34 };
AC代码
```

