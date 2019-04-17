# [LeetCode] Flatten Binary Tree to Linked List 将二叉树展开成链表 - Grandyang - 博客园







# [[LeetCode] Flatten Binary Tree to Linked List 将二叉树展开成链表](https://www.cnblogs.com/grandyang/p/4293853.html)







Given a binary tree, flatten it to a linked list in-place.

For example,
Given
         1
        / \
       2   5
      / \   \
     3   4   6



The flattened tree should look like:

   1
    \
     2
      \
       3
        \
         4
          \
           5
            \
             6

[click to show hints.](https://oj.leetcode.com/problems/flatten-binary-tree-to-linked-list/)

Hints:

If you notice carefully in the flattened tree, each node's right child points to the next node of a pre-order trave




这道题要求把二叉树展开成链表，根据展开后形成的链表的顺序分析出是使用先序遍历，那么只要是数的遍历就有递归和非递归的两种方法来求解，这里我们也用两种方法来求解。首先来看递归版本的，思路是先利用DFS的思路找到最左子节点，然后回到其父节点，把其父节点和右子节点断开，将原左子结点连上父节点的右子节点上，然后再把原右子节点连到新右子节点的右子节点上，然后再回到上一父节点做相同操作。代码如下：



解法一：

```
// Recursion
class Solution {
public:
    void flatten(TreeNode *root) {
        if (!root) return;
        if (root->left) flatten(root->left);
        if (root->right) flatten(root->right);
        TreeNode *tmp = root->right;
        root->right = root->left;
        root->left = NULL;
        while (root->right) root = root->right;
        root->right = tmp;
    }
};
```



例如，对于下面的二叉树，上述算法的变换的过程如下：

```
1
    / \
   2   5
  / \   \
 3   4   6

     1
    / \
   2   5
    \   \
     3   6
      \    
       4

   1
    \
     2
      \
       3
        \
         4
          \
           5
            \
             6
```



下面我们再来看非迭代版本的实现，这个方法是从根节点开始出发，先检测其左子结点是否存在，如存在则将根节点和其右子节点断开，将左子结点及其后面所有结构一起连到原右子节点的位置，把原右子节点连到元左子结点最后面的右子节点之后。代码如下：



解法二：

```
// Non-recursion
class Solution {
public:
    void flatten(TreeNode *root) {
        TreeNode *cur = root;
        while (cur) {
            if (cur->left) {
                TreeNode *p = cur->left;
                while (p->right) p = p->right;
                p->right = cur->right;
                cur->right = cur->left;
                cur->left = NULL;
            }
            cur = cur->right;
        }
    }
};
```



例如，对于下面的二叉树，上述算法的变换的过程如下：

```
1
    / \
   2   5
  / \   \
 3   4   6

   1
    \
     2
    / \
   3   4
        \
         5
          \
           6
           
   1
    \
     2
      \
       3
        \
         4
          \
           5
            \
             6
```



前序迭代解法如下：



解法三：

```
class Solution {
public:
    void flatten(TreeNode* root) {
        if (!root) return;
        stack<TreeNode*> s;
        s.push(root);
        while (!s.empty()) {
            TreeNode *t = s.top(); s.pop();
            if (t->left) {
                TreeNode *r = t->left;
                while (r->right) r = r->right;
                r->right = t->right;
                t->right = t->left;
                t->left = NULL;
            }
            if (t->right) s.push(t->right);
        }
    }
};
```



此题还可以延伸到用中序，后序，层序的遍历顺序来展开原二叉树，分别又有其对应的递归和非递归的方法，有兴趣的童鞋可以自行实现。



参考资料：

[https://leetcode.com/problems/flatten-binary-tree-to-linked-list/](https://leetcode.com/problems/flatten-binary-tree-to-linked-list/)

[https://leetcode.com/problems/flatten-binary-tree-to-linked-list/discuss/37182/my-recursive-solution-is-easy-and-clean](https://leetcode.com/problems/flatten-binary-tree-to-linked-list/discuss/37182/my-recursive-solution-is-easy-and-clean)

[https://leetcode.com/problems/flatten-binary-tree-to-linked-list/discuss/36977/my-short-post-order-traversal-java-solution-for-share](https://leetcode.com/problems/flatten-binary-tree-to-linked-list/discuss/36977/my-short-post-order-traversal-java-solution-for-share)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












