# [LeetCode] Merge Two Binary Trees 合并二叉树 - Grandyang - 博客园







# [[LeetCode] Merge Two Binary Trees 合并二叉树](https://www.cnblogs.com/grandyang/p/7058935.html)







Given two binary trees and imagine that when you put one of them to cover the other, some nodes of the two trees are overlapped while the others are not.

You need to merge them into a new binary tree. The merge rule is that if two nodes overlap, then sum node values up as the new value of the merged node. Otherwise, the NOT null node will be used as the node of new tree.

Example 1:
Input: 
	Tree 1                     Tree 2                  
          1                         2                             
         / \                       / \                            
        3   2                     1   3                        
       /                           \   \                      
      5                             4   7                  
Output: 
Merged tree:
	     3
	    / \
	   4   5
	  / \   \ 
	 5   4   7



Note: The merging process must start from the root nodes of both trees.



这道题给了我们两个二叉树，让我们合并成一个，规则是，都存在的结点，就将结点值加起来，否则空的位置就由另一个树的结点来代替。那么根据过往经验，处理二叉树问题的神器就是递归，那么我们来看递归函数如何去写。根据题目中的规则，我们知道如果要处理的相同位置上的两个结点都不存在的话，直接返回即可，如果t1存在，t2不存在，那么我们就以t1的结点值建立一个新结点，然后分别对t1的左右子结点和空结点调用递归函数，反之，如果t1不存在，t2存在，那么我们就以t2的结点值建立一个新结点，然后分别对t2的左右子结点和空结点调用递归函数。如果t1和t2都存在，那么我们就以t1和t2的结点值之和建立一个新结点，然后分别对t1的左右子结点和t2的左右子结点调用递归函数，参见代码如下：



解法一：

```
class Solution {
public:
    TreeNode* mergeTrees(TreeNode* t1, TreeNode* t2) {
        TreeNode *res = NULL;
        helper(t1, t2, res);
        return res;
    }
    void helper(TreeNode* t1, TreeNode* t2, TreeNode*& res) {
        if (!t1 && !t2) return;
        else if (t1 && !t2) {
            res = new TreeNode(t1->val);
            helper(t1->left, NULL, res->left);
            helper(t1->right, NULL, res->right);   
        } else if (!t1 && t2) {
            res = new TreeNode(t2->val);
            helper(NULL, t2->left, res->left);
            helper(NULL, t2->right, res->right);
        } else {
            res = new TreeNode(t1->val + t2->val);
            helper(t1->left, t2->left, res->left);
            helper(t1->right, t2->right, res->right);
        }
    }
};
```



其实远不用写的像上面那么复杂，我们连额外的函数都不用写，直接递归调用给定的函数即可，我们首先判断，如果t1不存在，则直接返回t2，反之，如果t2不存在，则直接返回t1。如果上面两种情况都不满足，那么以t1和t2的结点值之和建立新结点t，然后对t1和t2的左子结点调用递归并赋给t的左子结点，再对t1和t2的右子结点调用递归并赋给t的右子结点，返回t结点即可，参见代码如下：



解法二：

```
class Solution {
public:
    TreeNode* mergeTrees(TreeNode* t1, TreeNode* t2) {
        if (!t1) return t2;
        if (!t2) return t1;
        TreeNode *t = new TreeNode(t1->val + t2->val);
        t->left = mergeTrees(t1->left, t2->left);
        t->right = mergeTrees(t1->right, t2->right);
        return t;
    }
};
```



参考资料：

[https://discuss.leetcode.com/topic/92105/java-solution-6-lines-tree-traversal](https://discuss.leetcode.com/topic/92105/java-solution-6-lines-tree-traversal)

[https://discuss.leetcode.com/topic/92222/runtime-beat-100-6-lines-java-recursive-solution](https://discuss.leetcode.com/topic/92222/runtime-beat-100-6-lines-java-recursive-solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












