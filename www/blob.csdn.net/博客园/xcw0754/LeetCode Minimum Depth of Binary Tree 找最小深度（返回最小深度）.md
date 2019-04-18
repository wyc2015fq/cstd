# LeetCode  Minimum Depth of Binary Tree 找最小深度（返回最小深度） - xcw0754 - 博客园
# [LeetCode  Minimum Depth of Binary Tree 找最小深度（返回最小深度）](https://www.cnblogs.com/xcw0754/p/4085939.html)
题意：找到离根结点最近的叶子结点的那一层（设同一层上的结点与根结点的距离相等），返回它所在的层数。
方法有：
1、递归深度搜索
2、层次搜索
方法一：递归（无优化）
```
1 /**
 2  * Definition for binary tree
 3  * struct TreeNode {
 4  *     int val;
 5  *     TreeNode *left;
 6  *     TreeNode *right;
 7  *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 8  * };
 9  */
10 class Solution {
11 public:
12 int minDepth(TreeNode *root) {
13     if( root==0 )    //针对树根
14         return 0;
15     if(root->left==0&&root->right==0)    //是叶子，立即返回1
16         return 1;
17     int zuo,you,com;
18     if(root->left!=0&&root->right!=0){    //左右孩子均存在
19         zuo=minDepth(root->left);
20         you=minDepth(root->right);
21         return zuo<=you?++zuo:++you;
22     }
23     else{
24         if(root->left!=0&&root->right==0){    //只有左孩子，那么只需要沿着左孩子方向搜索
25             com=minDepth(root->left);
26         }
27         else{    //即(root->left==0&&root->right!=0)    只有右孩子，那么只需要沿着左孩子方向搜索
28             com=minDepth(root->right);
29         }
30         return ++com;
31     }
32 }
33 };
```
思路：递归寻找叶子结点。
注意：如果一个结点只有一个孩子，不能调用两次函数来分别搜索左右子树，不然空的一边会返回0，那么结果就错了。
方法二：递归（稍微优化）
```
1 /**
 2  * Definition for binary tree
 3  * struct TreeNode {
 4  *     int val;
 5  *     TreeNode *left;
 6  *     TreeNode *right;
 7  *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 8  * };
 9  */
10 class Solution {
11 public:
12 int mini(TreeNode *root) {
13     if( root->left==0 && root->right==0 )    //叶子结点，返回
14         return 1;
15     int zuo,you,com;
16     if(root->left!=0 && root->right!=0 ){    //左右孩子均存在
17         if( root->left->left==0 && root->left->right==0 ){    //左孩子是叶子，右孩子不一定是叶子，无必要再搜索右孩子了
18             zuo=mini( root->left );
19             return ++zuo;
20         }
21         else if( root->right->left==0 && root->right->right==0 ){    //右孩子是叶子，左孩子不一定是叶子，无必要再搜索左孩子了
22             you=mini( root->right );
23             return ++you;
24         }
25         else{        //左右孩子均不是叶子，都需要继续向下搜索
26             zuo=mini( root->left );
27             you=mini( root->right );
28             return zuo<=you?++zuo:++you;
29         }
30     }
31     else{    //只有一个孩子
32         if(root->left!=0)        //只有左孩子
33             com=mini( root->left );
34         else                    //只有右孩子
35             com=mini( root->right );
36         return ++com;
37     }
38 }
39 int minDepth(TreeNode *root) {
40     if( root==0 )    //只针对空树
41         return 0;
42     return mini(root);
43 }
44 };
```
思想是：若一个结点有两个孩子，而其中一个孩子是叶子，而另一个不是，那么就无需再搜索那个“不是叶子”的孩子了，因为它算出来的深度肯定比那个叶子的长。（具体自己画图领会）
吐槽：只是优化这么一点就需要考虑挺多东西。怪自己整体把握能力偏弱，总是需要先把代码打出来，修改，再修改才能解决。若能做到还没打代码之前就大概掌控整体结构，打出来就快多了。
方法三：层次遍历（暂时没空写）

