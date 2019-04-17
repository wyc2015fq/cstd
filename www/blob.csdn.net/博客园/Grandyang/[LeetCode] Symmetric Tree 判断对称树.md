# [LeetCode] Symmetric Tree 判断对称树 - Grandyang - 博客园







# [[LeetCode] Symmetric Tree 判断对称树](https://www.cnblogs.com/grandyang/p/4051715.html)







Given a binary tree, check whether it is a mirror of itself (ie, symmetric around its center).

For example, this binary tree is symmetric:
    1
   / \
  2   2
 / \ / \
3  4 4  3



But the following is not:
    1
   / \
  2   2
   \   \
   3    3



Note:
Bonus points if you could solve it both recursively and iteratively.



判断二叉树是否是平衡树，比如有两个节点n1, n2，我们需要比较n1的左子节点的值和n2的右子节点的值是否相等，同时还要比较n1的右子节点的值和n2的左子结点的值是否相等，以此类推比较完所有的左右两个节点。我们可以用递归和迭代两种方法来实现，写法不同，但是算法核心都一样。



解法一：

```
class Solution {
public:
    bool isSymmetric(TreeNode *root) {
        if (!root) return true;
        return isSymmetric(root->left, root->right);
    }
    bool isSymmetric(TreeNode *left, TreeNode *right) {
        if (!left && !right) return true;
        if (left && !right || !left && right || left->val != right->val) return false;
        return isSymmetric(left->left, right->right) && isSymmetric(left->right, right->left);
    }
    
};
```



迭代写法需要借助两个队列queue来实现，我们首先判空，如果root为空，直接返回true。否则将root的左右两个子结点分别装入两个队列，然后开始循环，循环条件是两个队列都不为空。在while循环中，我们首先分别将两个队列中的队首元素取出来，如果两个都是空结点，那么直接跳过，因为我们还没有比较完，有可能某个结点没有左子结点，但是右子结点仍然存在，所以这里只能continue。然后再看，如果有一个为空，另一个不为空，那么此时对称性已经被破坏了，不用再比下去了，直接返回false。若两个结点都存在，但是其结点值不同，这也破坏了对称性，返回false。否则的话将node1的左子结点和右子结点排入队列1，注意这里要将node2的右子结点和左子结点排入队列2，注意顺序的对应问题。最后循环结束后直接返回true，这里不必再去check两个队列是否同时为空，因为循环结束后只可能是两个队列均为空的情况，其他情况比如一空一不空的直接在循环内部就返回false了，参见代码如下：



解法二：

```
class Solution {
public:
    bool isSymmetric(TreeNode* root) {
        if (!root) return true;
        queue<TreeNode*> q1, q2;
        q1.push(root->left);
        q2.push(root->right);
        while (!q1.empty() && !q2.empty()) {
            TreeNode *node1 = q1.front(); q1.pop();
            TreeNode *node2 = q2.front(); q2.pop();
            if (!node1 && !node2) continue;
            if((node1 && !node2) || (!node1 && node2)) return false;
            if (node1->val != node2->val) return false;
            q1.push(node1->left);
            q1.push(node1->right);
            q2.push(node2->right);
            q2.push(node2->left);
        }
        return true;
    }
};
```



参考资料：

[https://leetcode.com/problems/symmetric-tree/](https://leetcode.com/problems/symmetric-tree/)

[https://leetcode.com/problems/symmetric-tree/discuss/33054/Recursive-and-non-recursive-solutions-in-Java](https://leetcode.com/problems/symmetric-tree/discuss/33054/Recursive-and-non-recursive-solutions-in-Java)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












