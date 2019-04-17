# [LeetCode] Binary Tree Preorder Traversal 二叉树的先序遍历 - Grandyang - 博客园







# [[LeetCode] Binary Tree Preorder Traversal 二叉树的先序遍历](https://www.cnblogs.com/grandyang/p/4146981.html)







Given a binary tree, return the *preorder* traversal of its nodes' values.

For example:
Given binary tree `{1,#,2,3}`,
   1
    \
     2
    /
   3



return `[1,2,3]`.

Note: Recursive solution is trivial, could you do it iteratively?



一般我们提到[树的遍历](http://zh.wikipedia.org/wiki/%E6%A0%91%E7%9A%84%E9%81%8D%E5%8E%86)，最常见的有先序遍历，中序遍历，后序遍历和层序遍历，它们用递归实现起来都非常的简单。而题目的要求是不能使用递归求解，于是只能考虑到用非递归的方法，这就要用到stack来辅助运算。由于先序遍历的顺序是"根-左-右", 算法为：

1. 把根节点push到栈中

2. 循环检测栈是否为空，若不空，则取出栈顶元素，保存其值，然后看其右子节点是否存在，若存在则push到栈中。再看其左子节点，若存在，则push到栈中。

代码如下：



解法一：

```
class Solution {
public:
    vector<int> preorderTraversal(TreeNode* root) {
        if (!root) return {};
        vector<int> res;
        stack<TreeNode*> s{{root}};
        while (!s.empty()) {
            TreeNode *t = s.top(); s.pop();
            res.push_back(t->val);
            if (t->right) s.push(t->right);
            if (t->left) s.push(t->left);
        }
        return res;
    }
};
```



下面这种写法使用了一个辅助结点p，这种写法其实可以看作是一个模版，对应的还有中序和后序的模版写法，形式很统一，方便于记忆。辅助结点p初始化为根结点，while循环的条件是栈不为空或者辅助结点p不为空，在循环中首先判断如果辅助结点p存在，那么先将p加入栈中，然后将p的结点值加入结果res中，此时p指向其左子结点。否则如果p不存在的话，表明没有左子结点，我们取出栈顶结点，将p指向栈顶结点的右子结点，参见代码如下：



解法二：

```
class Solution {
public:
    vector<int> preorderTraversal(TreeNode* root) {
        vector<int> res;
        stack<TreeNode*> s;
        TreeNode *p = root;
        while (!s.empty() || p) {
            if (p) {
                s.push(p);
                res.push_back(p->val);
                p = p->left;
            } else {
                TreeNode *t = s.top(); s.pop();
                p = t->right;
            }
        }
        return res;
    }
};
```



类似题目：

[Binary Tree Inorder Traversal](http://www.cnblogs.com/grandyang/p/4297300.html)

[Binary Tree Postorder Traversal](http://www.cnblogs.com/grandyang/p/4251757.html)

[Binary Tree Level Order Traversal](http://www.cnblogs.com/grandyang/p/4051321.html)

[Verify Preorder Sequence in Binary Search Tree](http://www.cnblogs.com/grandyang/p/5327635.html)

[Verify Preorder Serialization of a Binary Tree](http://www.cnblogs.com/grandyang/p/5174738.html)



参考资料：

[https://discuss.leetcode.com/topic/30632/preorder-inorder-and-postorder-iteratively-summarization](https://discuss.leetcode.com/topic/30632/preorder-inorder-and-postorder-iteratively-summarization)

[https://discuss.leetcode.com/topic/44231/preorder-inorder-and-postorder-traversal-iterative-java-solution](https://discuss.leetcode.com/topic/44231/preorder-inorder-and-postorder-traversal-iterative-java-solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












