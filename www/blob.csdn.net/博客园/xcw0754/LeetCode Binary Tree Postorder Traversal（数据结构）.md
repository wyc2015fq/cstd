# LeetCode Binary Tree Postorder Traversal（数据结构） - xcw0754 - 博客园
# [LeetCode Binary Tree Postorder Traversal（数据结构）](https://www.cnblogs.com/xcw0754/p/4931438.html)
题意：
　　用迭代法输出一棵二叉树的后序遍历结果。
思路：
　　（1）用两个栈，一个存指针，一个存标记，表示该指针当前已经访问过哪些孩子了。
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
12     vector<int> postorderTraversal(TreeNode* root) {
13         vector<int> ans;
14         if(root==NULL)    return ans;
15     
16         stack<TreeNode*> stac1;    stac1.push(root);
17         stack<int> stac2;        stac2.push(-1);//表示哪个孩子已经被遍历,-1表示其孩子未被遍历
18     
19         while(!stac2.empty())
20         {
21             TreeNode *top=stac1.top();
22             int &dir=stac2.top();
23             if(dir<1)
24             {
25                 if(dir==-1 && top->left)    
26                 {
27                     stac1.push(top->left);
28                     dir=0;
29                     stac2.push(-1);
30                 }
31                 else if(top->right)
32                 {
33                     stac1.push(top->right);
34                     dir=1;
35                     stac2.push(-1);
36                 }
37                 else    dir=1;
38             }
39             else
40             {
41                 ans.push_back(top->val);
42                 stac2.pop();
43                 stac1.pop();
44             }
45         }
46     }
47 };
AC代码
```
　　（2）用一个栈，模拟逆过程，先输出根，再输出右子树，再输出左子树，最后将输出结果反置即可。这样的好处就是不需要记录根节点了，这和层次遍历的过程差不多。
```
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
    vector<int> postorderTraversal(TreeNode* root) {
        vector<int> ans;
        if(root==NULL)    return ans;
    
        stack<TreeNode*> stac;    
        stac.push(root);
    
        while(!stac.empty())
        {
            TreeNode *top=stac.top();
            ans.push_back(top->val);
            stac.pop();
            if(top->left)    stac.push(top->left);
            if(top->right)    stac.push(top->right);
        }
        reverse(ans.begin(),ans.end());
    }
};
AC代码
```
　　（3）O(1)的空间，依然O(n)的复杂度。待写。。。。

