# LeetCode  Populating Next Right Pointers in Each Node （技巧） - xcw0754 - 博客园
# [LeetCode  Populating Next Right Pointers in Each Node （技巧）](https://www.cnblogs.com/xcw0754/p/4934614.html)
题意：
　　给一棵满二叉树，要求将每层的节点从左到右用next指针连起来，层尾指向NULL即可。
思路：
　　可以递归也可以迭代。需要观察到next的左孩子恰好就是本节点的右孩子的next啦。
　　（1）递归：这个更快。
```
1 /**
 2  * Definition for binary tree with next pointer.
 3  * struct TreeLinkNode {
 4  *  int val;
 5  *  TreeLinkNode *left, *right, *next;
 6  *  TreeLinkNode(int x) : val(x), left(NULL), right(NULL), next(NULL) {}
 7  * };
 8  */
 9 class Solution {
10 public:
11     void DFS(TreeLinkNode* root,TreeLinkNode* t)
12     {
13         if(root->next==NULL && t!=NULL)    root->next=t->left;
14         if(root->left)
15         {
16             root->left->next=root->right;
17             DFS(root->left, NULL);
18             DFS(root->right,root->next);
19         }
20     }
21     void connect(TreeLinkNode *root) {
22         if(root)    DFS(root,NULL);
23     }
24 };
AC代码
```
　　（2）递归：这个简洁。
```
1 /**
 2  * Definition for binary tree with next pointer.
 3  * struct TreeLinkNode {
 4  *  int val;
 5  *  TreeLinkNode *left, *right, *next;
 6  *  TreeLinkNode(int x) : val(x), left(NULL), right(NULL), next(NULL) {}
 7  * };
 8  */
 9 class Solution {
10 public:
11     void connect(TreeLinkNode *root) {
12         if(root && root->left)
13         {
14             root->left->next=root->right;
15             if(root->next)
16                 root->right->next=root->next->left;
17             connect(root->left);
18             connect(root->right);
19         }
20 
21         
22     }
23 };
AC代码
```
　　（3）迭代：这个更吊。
```
1 /**
 2  * Definition for binary tree with next pointer.
 3  * struct TreeLinkNode {
 4  *  int val;
 5  *  TreeLinkNode *left, *right, *next;
 6  *  TreeLinkNode(int x) : val(x), left(NULL), right(NULL), next(NULL) {}
 7  * };
 8  */
 9 class Solution {
10 public:
11     void connect(TreeLinkNode *root) {
12         while(root&&root->left)
13         {
14             TreeLinkNode *p=root;
15             while(p)
16             {
17                 p->left->next=p->right;
18                 if(p->next)
19                     p->right->next=p->next->left;
20                 p=p->next;
21             }
22             root=root->left;
23         }
24     }
25 };
AC代码
```

