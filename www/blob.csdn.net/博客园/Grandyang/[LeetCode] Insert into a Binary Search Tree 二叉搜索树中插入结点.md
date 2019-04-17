# [LeetCode] Insert into a Binary Search Tree 二叉搜索树中插入结点 - Grandyang - 博客园







# [[LeetCode] Insert into a Binary Search Tree 二叉搜索树中插入结点](https://www.cnblogs.com/grandyang/p/9914546.html)







Given the root node of a binary search tree (BST) and a value to be inserted into the tree, insert the value into the BST. Return the root node of the BST after the insertion. It is guaranteed that the new value does not exist in the original BST.

Note that there may exist multiple valid ways for the insertion, as long as the tree remains a BST after insertion. You can return any of them.

For example, 
Given the tree:
        4
       / \
      2   7
     / \
    1   3
And the value to insert: 5

You can return this binary search tree:
         4
       /   \
      2     7
     / \   /
    1   3 5

This tree is also valid:
         5
       /   \
      2     7
     / \   
    1   3
         \
          4


这道题让我们在二叉搜索树中插入结点，当前还需要保持二叉搜索树的性质，那么插入结点的方式就有多种，就像题目中给的那个例子。结点5可以有不同的方法，但是很显然，放在结点7的左子结点比代替结点4成为根结点要来的简单许多。怎么简单我们就怎么来，所以还是按照简单的来吧。由于二叉搜索树自带二分的性质，那么首先根结点比较，如果大于根结点值的话，说明肯定要插入到右子树中。所以接下来跟7比较，对于递归函数来说，结点7也可以当作是一个新的根结点，那么由于结点7的值大于目标值5，所以要去其左子树，我们发现其左子结点为空，那么我们就可以根据目标值来生成一个新的结点，然后连到结点7的左子树上即可。那么在递归函数中，首先判断当前结点是否为空，为空的话就新建一个结点返回。否则就判断当前结点值是否大于目标值，是的话就对左子结点调用递归函数，并将返回值赋给当前结点的左子结点，否则就对右子结点调用递归函数，并将返回值赋给当前结点的右子结点，最后返回当前结点即可，参见代码如下：



解法一：

```
class Solution {
public:
    TreeNode* insertIntoBST(TreeNode* root, int val) {
        if (!root) return new TreeNode(val);
        if (root->val > val) root->left = insertIntoBST(root->left, val);
        else root->right = insertIntoBST(root->right, val);
        return root;
    }
};
```



我们也可以不使用递归来做，而是用迭代。整体思路跟递归并没有太大的区别，但没有递归写法简洁。首先还是判空，若为空，就新建结点返回。然后用一个变量cur来遍历，在while循环中，如果当前值大于目标值，如果其左子结点不存在，那么我们新建结点，并连上其左子结点，并跳出循环；若左子结点存在，则cur指向其左子结点。否则，当前值小于目标值，若其右子结点不存在，新建结点并连上其右子结点，并跳出循环；若右子结点存在，则cur指向其右子结点。最后返回root即可，参见代码如下：



解法二：

```
class Solution {
public:
    TreeNode* insertIntoBST(TreeNode* root, int val) {
        if (!root) return new TreeNode(val);
        TreeNode *cur = root;
        while (true) {
            if (cur->val > val) {
                if (!cur->left) {cur->left = new TreeNode(val); break;}
                cur = cur->left;
            } else {
                if (!cur->right) {cur->right = new TreeNode(val); break;}
                cur = cur->right;
            }
        }
        return root;
    }
};
```



类似题目：

[Search in a Binary Search Tree](https://www.cnblogs.com/grandyang/p/9912434.html)



参考资料：

[https://leetcode.com/problems/insert-into-a-binary-search-tree/](https://leetcode.com/problems/insert-into-a-binary-search-tree/)

[https://leetcode.com/problems/insert-into-a-binary-search-tree/discuss/150757/java-iterative-100](https://leetcode.com/problems/insert-into-a-binary-search-tree/discuss/150757/java-iterative-100)

[https://leetcode.com/problems/insert-into-a-binary-search-tree/discuss/164137/Java-easy-to-understand-solution](https://leetcode.com/problems/insert-into-a-binary-search-tree/discuss/164137/Java-easy-to-understand-solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












