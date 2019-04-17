# [LeetCode] Path Sum 二叉树的路径和 - Grandyang - 博客园







# [[LeetCode] Path Sum 二叉树的路径和](https://www.cnblogs.com/grandyang/p/4036961.html)







Given a binary tree and a sum, determine if the tree has a root-to-leaf path such that adding up all the values along the path equals the given sum.

For example:
Given the below binary tree and `sum = 22`,
              5
             / \
            4   8
           /   / \
          11  13  4
         /  \      \
        7    2      1

return true, as there exist a root-to-leaf path `5->4->11->2` which sum is 22.



这道求二叉树的路径需要用深度优先算法DFS的思想来遍历每一条完整的路径，也就是利用递归不停找子节点的左右子节点，而调用递归函数的参数只有当前节点和sum值。首先，如果输入的是一个空节点，则直接返回false，如果如果输入的只有一个根节点，则比较当前根节点的值和参数sum值是否相同，若相同，返回true，否则false。 这个条件也是递归的终止条件。下面我们就要开始递归了，由于函数的返回值是Ture/False，我们可以同时两个方向一起递归，中间用或||连接，只要有一个是True，整个结果就是True。递归左右节点时，这时候的sum值应该是原sum值减去当前节点的值。代码如下：



解法一：

```
class Solution {
public:
    bool hasPathSum(TreeNode* root, int sum) {
        if (!root) return false;
        if (!root->left && !root->right && root->val == sum ) return true;
        return hasPathSum(root->left, sum - root->val) || hasPathSum(root->right, sum - root->val);
    }
};
```



我们也可以使用迭代的写法，这里用的也是先序遍历的迭代写法，我们先序遍历二叉树，左右子结点都需要加上其父结点值，这样当遍历到叶结点时，如果和sum相等了，那么就说明一定有一条从root过来的路径。注意这里我们不必一定要先处理右子结点，调换下顺序也是可以的，因为不论是先序遍历的根-左-右，还是根-右-左，并不会影响到我们找路径，参见代码如下：



解法二：

```
class Solution {
public:
    bool hasPathSum(TreeNode* root, int sum) {
        if (!root) return false;
        stack<TreeNode*> st{{root}};
        while (!st.empty()) {
            TreeNode *t = st.top(); st.pop();
            if (!t->left && !t->right) {
                if (t->val == sum) return true;
            }
            if (t->right) {
                t->right->val += t->val;
                st.push(t->right);
            }
            if (t->left) {
                t->left->val += t->val;
                st.push(t->left);
            }
        }
        return false;
    }
};
```



类似题目：

[Path Sum IV](http://www.cnblogs.com/grandyang/p/7570954.html)

[Path Sum III](http://www.cnblogs.com/grandyang/p/6007336.html)

[Binary Tree Maximum Path Sum](http://www.cnblogs.com/grandyang/p/4280120.html)

[Path Sum II](http://www.cnblogs.com/grandyang/p/4042156.html)

[Sum Root to Leaf Numbers](http://www.cnblogs.com/grandyang/p/4273700.html)

[Binary Tree Preorder Traversal](http://www.cnblogs.com/grandyang/p/4146981.html)



参考资料：

[https://leetcode.com/problems/path-sum/](https://leetcode.com/problems/path-sum/)

[https://leetcode.com/problems/path-sum/discuss/36534/My-java-no-recursive-method](https://leetcode.com/problems/path-sum/discuss/36534/My-java-no-recursive-method)

[https://leetcode.com/problems/path-sum/discuss/36378/AcceptedMy-recursive-solution-in-Java](https://leetcode.com/problems/path-sum/discuss/36378/AcceptedMy-recursive-solution-in-Java)

[https://leetcode.com/problems/path-sum/discuss/36382/Accepted-By-using-postorder-traversal](https://leetcode.com/problems/path-sum/discuss/36382/Accepted-By-using-postorder-traversal)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












