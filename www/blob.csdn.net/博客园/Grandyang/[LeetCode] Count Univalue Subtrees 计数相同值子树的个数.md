# [LeetCode] Count Univalue Subtrees 计数相同值子树的个数 - Grandyang - 博客园







# [[LeetCode] Count Univalue Subtrees 计数相同值子树的个数](https://www.cnblogs.com/grandyang/p/5206862.html)












Given a binary tree, count the number of uni-value subtrees.

A Uni-value subtree means all nodes of the subtree have the same value.

For example:
Given binary tree,
              5
             / \
            1   5
           / \   \
          5   5   5



return `4`.



这道题让我们求相同值子树的个数，就是所有节点值都相同的子树的个数，之前有道求最大BST子树的题[Largest BST Subtree](http://www.cnblogs.com/grandyang/p/5188938.html)，感觉挺像的，都是关于子树的问题，解题思路也可以参考一下，我们可以用递归来做，第一种解法的思路是先序遍历树的所有的节点，然后对每一个节点调用判断以当前节点为根的字数的所有节点是否相同，判断方法可以参考之前那题[Same Tree](http://www.cnblogs.com/grandyang/p/4053384.html)，用的是分治法的思想，分别对左右字数分别调用递归，参见代码如下：



解法一：

```
class Solution {
public:
    int res = 0;
    int countUnivalSubtrees(TreeNode* root) {
        if (!root) return res;
        if (isUnival(root, root->val)) ++res;
        countUnivalSubtrees(root->left);
        countUnivalSubtrees(root->right);
        return res;
    }
    bool isUnival(TreeNode *root, int val) {
        if (!root) return true;
        return root->val == val && isUnival(root->left, val) && isUnival(root->right, val);
    }
};
```



但是上面的那种解法不是很高效，含有大量的重复check，我们想想能不能一次遍历就都搞定，我们这样想，符合条件的相同值的字数肯定是有叶节点的，而且叶节点也都相同(注意单独的一个叶节点也被看做是一个相同值子树)，那么我们可以从下往上check，采用后序遍历的顺序，左右根，我们还是递归调用函数，对于当前遍历到的节点，如果对其左右子节点分别递归调用函数，返回均为true的话，那么说明当前节点的值和左右子树的值都相同，那么又多了一棵树，所以结果自增1，然后返回当前节点值和给定值(其父节点值)是否相同，从而回归上一层递归调用。这里特别说明一下在子函数中要使用的那个单竖杠或，为什么不用双竖杠的或，因为单竖杠的或是位或，就是说左右两部分都需要被计算，然后再或，C++这里将true当作1，false当作0，然后进行Bit OR 运算。不能使用双竖杠或的原因是，如果是双竖杠或，一旦左半边为true了，整个就直接是true了，右半边就不会再计算了，这样的话，一旦右子树中有值相同的子树也不会被计算到结果res中了，参见代码如下：








解法二：

```
class Solution {
public:
    int countUnivalSubtrees(TreeNode* root) {
        int res = 0;
        isUnival(root, -1, res);
        return res;
    }
    bool isUnival(TreeNode* root, int val, int& res) {
        if (!root) return true;
        if (!isUnival(root->left, root->val, res) | !isUnival(root->right, root->val, res)) {
            return false;
        }
        ++res;
        return root->val == val;
    }
};
```



我们还可以变一种写法，让递归函数直接返回以当前节点为根的相同值子树的个数，然后参数里维护一个引用类型的布尔变量，表示以当前节点为根的子树是否为相同值子树，我们首先对当前节点的左右子树分别调用递归函数，然后把结果加起来，我们现在要来看当前节点是不是和其左右子树节点值相同，当前我们首先要确认左右子节点的布尔型变量均为true，这样保证左右子节点分别都是相同值子树的根，然后我们看如果左子节点存在，那么左子节点值需要和当前节点值相同，如果右子节点存在，那么右子节点值要和当前节点值相同，若上述条件均满足的话，说明当前节点也是相同值子树的根节点，返回值再加1，参见代码如下：



解法三：

```
class Solution {
public:
    int countUnivalSubtrees(TreeNode* root) {
        bool b = true;
        return isUnival(root, b);
    }
    int isUnival(TreeNode *root, bool &b) {
        if (!root) return 0;
        bool l = true, r = true;
        int res = isUnival(root->left, l) + isUnival(root->right, r);
        b = l && r && (root->left ? root->val == root->left->val : true) && (root->right ? root->val == root->right->val : true);
        return res + b;
    }
};
```



上面三种都是令人看得头晕的递归写法，那么我们也来看一种迭代的写法，迭代写法是在后序遍历[Binary Tree Postorder Traversal](http://www.cnblogs.com/grandyang/p/4251757.html)的基础上修改而来，我们需要用set来保存所有相同值子树的根节点，对于我们遍历到的节点，如果其左右子节点均不存在，那么此节点为叶节点，符合题意，加入结果set中，如果左子节点不存在，那么右子节点必须已经在结果set中，而且当前节点值需要和右子节点值相同才能将当前节点加入结果set中，同样的，如果右子节点不存在，那么左子节点必须已经存在set中，而且当前节点值要和左子节点值相同才能将当前节点加入结果set中。最后，如果左右子节点均存在，那么必须都已经在set中，并且左右子节点值都要和根节点值相同才能将当前节点加入结果set中，其余部分跟后序遍历的迭代写法一样，参见代码如下：



解法四：

```
class Solution {
public:
    int countUnivalSubtrees(TreeNode* root) {
        set<TreeNode*> res;
        if (!root) return 0;
        stack<TreeNode*> s;
        s.push(root);
        TreeNode *head = root;
        while (!s.empty()) {
            TreeNode *t = s.top(); 
            if ((!t->left && !t->right) || t->left == head || t->right == head) {
                if (!t->left && !t->right) {
                    res.insert(t);
                } else if (!t->left && res.find(t->right) != res.end() && t->right->val == t->val) {
                    res.insert(t);
                } else if (!t->right && res.find(t->left) != res.end() && t->left->val == t->val) {
                    res.insert(t);
                } else if (t->left && t->right && res.find(t->left) != res.end() && res.find(t->right) != res.end() && t->left->val == t->val && t->right->val == t->val) {
                    res.insert(t);
                }
                s.pop();
                head = t;
            } else {
                if (t->right) s.push(t->right);
                if (t->left) s.push(t->left);
            }
        }
        return res.size();
    }
};
```



类似题目：

[Subtree of Another Tree](http://www.cnblogs.com/grandyang/p/6828687.html)

[Longest Univalue Path](http://www.cnblogs.com/grandyang/p/7636259.html)

[Largest BST Subtree](http://www.cnblogs.com/grandyang/p/5188938.html)

[Binary Tree Postorder Traversal](http://www.cnblogs.com/grandyang/p/4251757.html)

[Same Tree](http://www.cnblogs.com/grandyang/p/4053384.html)



参考资料：

[https://leetcode.com/discuss/66705/c-7-lines-solution](https://leetcode.com/discuss/66705/c-7-lines-solution)

[https://leetcode.com/discuss/69376/my-c-solution-easy-to-understand](https://leetcode.com/discuss/69376/my-c-solution-easy-to-understand)

[https://leetcode.com/discuss/54295/my-accepted-java-iterative-solution](https://leetcode.com/discuss/54295/my-accepted-java-iterative-solution)

[https://leetcode.com/discuss/64078/recursive-and-iterative-solution-with-detailed-explanation](https://leetcode.com/discuss/64078/recursive-and-iterative-solution-with-detailed-explanation)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












