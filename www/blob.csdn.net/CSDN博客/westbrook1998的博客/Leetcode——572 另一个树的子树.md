# Leetcode——572. 另一个树的子树 - westbrook1998的博客 - CSDN博客





2018年05月02日 23:21:57[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：281








> 
给定两个非空二叉树 s 和 t，检验 s 中是否包含和 t 具有相同结构和节点值的子树。s 的一个子树包括 s 的一个节点和这个节点的所有子孙。s 也可以看做它自身的一棵子树。


> 
示例 1: 

  给定的树 s: 

       3 

      / \ 

     4   5 

    / \ 

   1   2 

  给定的树 t： 

     4  

    / \ 

   1   2 

  返回 true，因为 t 与 s 的一个子树拥有相同的结构和节点值。 

  示例 2: 

  给定的树 s： 

       3 

      / \ 

     4   5 

    / \ 

   1   2 

      / 

     0 

     给定的树 t： 

     4 

    / \ 

   1   2 

  返回 false。
树的题目，查找大树中是否有匹配的子树，用递归可以解决

代码：

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
    bool isSubtree(TreeNode* s, TreeNode* t) {
        if(s==NULL || t==NULL){
            return s==NULL && t==NULL;
        }
        return isSametree(s,t) || isSubtree(s->left,t) || isSubtree(s->right,t);
    }
    bool isSametree(TreeNode* s,TreeNode* t){
        if(s==NULL || t==NULL){
            return s==NULL && t==NULL;
        }
        else{
            return (s->val==t->val) && isSametree(s->left,t->left) && isSametree(s->right,t->right);
        }
    }
};
```

主要思路就是再写一个`isSametree`函数来判断两个树是否相同，首先如果都为空，则true，如果一个为空，则`false`，如果都不为空，比较该节点的`值（val）`和递归比较左右子树，将这三个比较结果进行`与（&&）`运算，即得出结果 

在`isSubtree`函数中，也是先判断空，然后递归判断当前树是否相同，以及大树左子树与小树，大树右子树与小树，注意这里后面两个递归判断是调用`isSubtree`而不是`isSametree`，因为这里还是判断`t`是不是`s->left`或`s->right`的子树，然后返回这三个判断的`或（||）`运算





