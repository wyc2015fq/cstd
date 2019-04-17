# [LeetCode] Binary Tree Zigzag Level Order Traversal  二叉树的之字形层序遍历 - Grandyang - 博客园







# [[LeetCode] Binary Tree Zigzag Level Order Traversal  二叉树的之字形层序遍历](https://www.cnblogs.com/grandyang/p/4297009.html)







Given a binary tree, return the *zigzag level order* traversal of its nodes' values. (ie, from left to right, then right to left for the next level and alternate between).

For example:
Given binary tree `{3,9,20,#,#,15,7}`,
    3
   / \
  9  20
    /  \
   15   7



return its zigzag level order traversal as:
[
  [3],
  [20,9],
  [15,7]
]



confused what `"{1,#,2,3}"` means? [> read more on how binary tree is serialized on OJ.](https://oj.leetcode.com/problems/binary-tree-zigzag-level-order-traversal/)



这道二叉树的之字形层序遍历是之前那道[[LeetCode] Binary Tree Level Order Traversal 二叉树层序遍历](http://www.cnblogs.com/grandyang/p/4051321.html)的变形，不同之处在于一行是从左到右遍历，下一行是从右往左遍历，交叉往返的之字形的层序遍历。根据其特点我们用到栈的后进先出的特点，这道题我们维护两个栈，相邻两行分别存到两个栈中，进栈的顺序也不相同，一个栈是先进左子结点然后右子节点，另一个栈是先进右子节点然后左子结点，这样出栈的顺序就是我们想要的之字形了，代码如下：



```
/**
 * Definition for binary tree
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    vector<vector<int> > zigzagLevelOrder(TreeNode *root) {
        vector<vector<int> >res;
        if (!root) return res;
        stack<TreeNode*> s1;
        stack<TreeNode*> s2;
        s1.push(root);
        vector<int> out;
        while (!s1.empty() || !s2.empty()) {
            while (!s1.empty()) {
                TreeNode *cur = s1.top();
                s1.pop();
                out.push_back(cur->val);
                if (cur->left) s2.push(cur->left);
                if (cur->right) s2.push(cur->right);
            } 
            if (!out.empty()) res.push_back(out);
            out.clear();
            while (!s2.empty()) {
                TreeNode *cur = s2.top();
                s2.pop();
                out.push_back(cur->val);
                if (cur->right) s1.push(cur->right);
                if (cur->left) s1.push(cur->left);
            }
            if (!out.empty()) res.push_back(out);
            out.clear();
        }
        return res;
    }
};
```



比如对于题干中的那个例子：
    3
   / \
  9  20
    /  \
   15   7
我们来看每一层两个栈s1, s2的情况：

s1:　　3

s2:



s1:　　

s2:　　9　　20



s1:　　7　　15

s2: 



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












