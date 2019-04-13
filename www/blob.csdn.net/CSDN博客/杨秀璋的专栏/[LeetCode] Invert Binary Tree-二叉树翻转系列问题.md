
# [LeetCode] Invert Binary Tree - 二叉树翻转系列问题 - 杨秀璋的专栏 - CSDN博客

2015年09月12日 05:33:35[Eastmount](https://me.csdn.net/Eastmount)阅读数：1818


**目录：**
1.Invert Binary Tree - 二叉树翻转 [递归]

**题目概述：**
Invert a binary tree.
4
   /   \
  2     7
 / \   / \
1   3 6   9to4
   /   \
  7     2
 / \   / \
9   6 3   1Trivia:This problem was inspired by[this
 original tweet](https://twitter.com/mxcl/status/608682016205344768)by[Max Howell](https://twitter.com/mxcl):
> Google: 90% of our engineers use the software you wrote (Homebrew), but you can’t invert a binary tree on a whiteboard so fuck off.
**题目分析：**
题目背景是MaxHowell(他是苹果电脑最受欢迎的homebrew程序作者)去Google面试，面试官说：“虽然在Google有90%的工程师用你写的Homebrew，但是你居然不能再白板上写出翻转二叉树的代码，所以滚带吧”！
该题最初想法就是通过递归依次交换左右结点，但是想得太多，如“是否需要再建一颗树”、“是否需要引入队列或BFS”，最终没有AC。
**我的代码：**

```python
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
 //思路：二叉树递归 左右结点交换 在递归翻转左右子树
class Solution {
public:
    TreeNode* invertTree(TreeNode* root) {
        TreeNode *left, *right;
        if(root==NULL)
            return root;
        left = root->left;
        right = root->right;
        root->left = invertTree(right);
        root->right = invertTree(left);
        return root;
    }
};
```

**其他代码：**
推荐阅读和采用二叉树非递归层次遍历算法实现如下。
[你会翻转二叉树吗？--谈程序员的招聘](http://blog.devtang.com/blog/2015/06/16/talk-about-tech-interview/)
[
http://blog.csdn.net/sunao2002002/article/details/46482559](http://blog.csdn.net/sunao2002002/article/details/46482559)
```python
//第二种方法 通过队列层次遍历队列实现
class Solution {
public:
    TreeNode* invertTree(TreeNode* root) {
        queue<TreeNode*> q;
        if(root==NULL)
            return root;
        q.push(root);
        int size=q.size();
        while(size>0)
        {
            TreeNode *p=q.front();
            q.pop();
            //交换左右结点
            TreeNode *LNode=p->left;
            TreeNode *RNode=p->right;
            p->left=RNode;
            p->right=LNode;
            //进入队列
            if(p->left) {
                q.push(p->left);
            }
            if(p->right) {
                q.push(p->right);
            }
            size=q.size();
        }
        return root;
    }
};
```

**其他题目：**

(By:Eastmount 2015-9-12 凌晨5点半[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))


