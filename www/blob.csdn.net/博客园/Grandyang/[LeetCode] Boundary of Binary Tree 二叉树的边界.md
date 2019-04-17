# [LeetCode] Boundary of Binary Tree 二叉树的边界 - Grandyang - 博客园







# [[LeetCode] Boundary of Binary Tree 二叉树的边界](https://www.cnblogs.com/grandyang/p/6833459.html)









Given a binary tree, return the values of its boundary in anti-clockwise direction starting from root. Boundary includes left boundary, leaves, and right boundary in order without duplicate nodes.

Left boundary is defined as the path from root to the left-most node. Right boundary is defined as the path from root to the right-most node. If the root doesn't have left subtree or right subtree, then the root itself is left boundary or right boundary. Note this definition only applies to the input binary tree, and not applies to any subtrees.

The left-most node is defined as a leaf node you could reach when you always firstly travel to the left subtree if exists. If not, travel to the right subtree. Repeat until you reach a leaf node.

The right-most node is also defined by the same way with left and right exchanged.

Example 1
Input:
  1
   \
    2
   / \
  3   4

Ouput:
[1, 3, 4, 2]

Explanation:
The root doesn't have left subtree, so the root itself is left boundary.
The leaves are node 3 and 4.
The right boundary are node 1,2,4. Note the anti-clockwise direction means you should output reversed right boundary.
So order them in anti-clockwise without duplicates and we have [1,3,4,2].



Example 2
Input:
    ____1_____
   /          \
  2            3
 / \          / 
4   5        6   
   / \      / \
  7   8    9  10  
       
Ouput:
[1,2,4,7,8,9,10,6,3]

Explanation:
The left boundary are node 1,2,4. (4 is the left-most node according to definition)
The leaves are node 4,7,8,9,10.
The right boundary are node 1,3,6,10. (10 is the right-most node).
So order them in anti-clockwise without duplicate nodes we have [1,2,4,7,8,9,10,6,3].



这道题给了我们一棵二叉树，让我们以逆时针的顺序来输出树的边界，按顺序分别为左边界，叶结点和右边界。题目中给的例子也能让我们很清晰的明白哪些算是边界上的结点。那么最直接的方法就是分别按顺序求出左边界结点，叶结点，和右边界结点。那么如何求的，对于树的操作肯定是用递归最简洁啊，所以我们可以写分别三个递归函数来分别求左边界结点，叶结点，和右边界结点。首先我们先要处理根结点的情况，当根结点没有左右子结点时，其也是一个叶结点，那么我们一开始就将其加入结果res中，那么再计算叶结点的时候又会再加入一次，这样不对。所以我们判断如果根结点至少有一个子结点，我们才提前将其加入结果res中。然后再来看求左边界结点的函数，如果当前结点不存在，或者没有子结点，我们直接返回。否则就把当前结点值加入结果res中，然后看如果左子结点存在，就对其调用递归函数，反之如果左子结点不存在，那么对右子结点调用递归函数。而对于求右边界结点的函数就反过来了，如果右子结点存在，就对其调用递归函数，反之如果右子结点不存在，就对左子结点调用递归函数，注意在调用递归函数之后才将结点值加入结果res，因为我们是需要按逆时针的顺序输出。最后就来看求叶结点的函数，没什么可说的，就是看没有子结点存在了就加入结果res，然后对左右子结点分别调用递归即可，参见代码如下：



解法一：

```
class Solution {
public:
    vector<int> boundaryOfBinaryTree(TreeNode* root) {
        if (!root) return {};
        vector<int> res;
        if (root->left || root->right) res.push_back(root->val);
        leftBoundary(root->left, res);
        leaves(root, res);
        rightBoundary(root->right, res);
        return res;
    }
    void leftBoundary(TreeNode* node, vector<int>& res) {
        if (!node || (!node->left && !node->right)) return;
        res.push_back(node->val);
        if (!node->left) leftBoundary(node->right, res);
        else leftBoundary(node->left, res);
    }
    void rightBoundary(TreeNode* node, vector<int>& res) {
        if (!node || (!node->left && !node->right)) return;
        if (!node->right) rightBoundary(node->left, res);
        else rightBoundary(node->right, res);
        res.push_back(node->val);
    }
    void leaves(TreeNode* node, vector<int>& res) {
        if (!node) return;
        if (!node->left && !node->right) {
            res.push_back(node->val);
        }
        leaves(node->left, res);
        leaves(node->right, res);
    }
};
```



下面这种方法把上面三种不同的递归揉合到了一个递归中，并用bool型变量来标记当前是求左边界结点还是求右边界结点，同时还有加入叶结点到结果res中的功能。如果左边界标记为true，那么将结点值加入结果res中，下面就是调用对左右结点调用递归函数了。根据上面的解题思路我们知道，如果是求左边界结点，优先调用左子结点，当左子结点不存在时再调右子结点，而对于求右边界结点，优先调用右子结点，当右子结点不存在时再调用左子结点。综上考虑，在对左子结点调用递归函数时，左边界标识设为leftbd && node->left，而对右子结点调用递归的左边界标识设为leftbd && !node->left，这样左子结点存在就会被优先调用。而右边界结点的情况就正好相反，调用左子结点的右边界标识为rightbd && !node->right, 调用右子结点的右边界标识为 rightbd && node->right，这样就保证了右子结点存在就会被优先调用，参见代码如下：



解法二：

```
class Solution {
public:
    vector<int> boundaryOfBinaryTree(TreeNode* root) {
        if (!root) return {};
        vector<int> res{root->val};
        helper(root->left, true, false, res);
        helper(root->right, false, true, res);
        return res;
    }
    void helper(TreeNode* node, bool leftbd, bool rightbd, vector<int>& res) {
        if (!node) return;
        if (!node->left && !node->right) {
            res.push_back(node->val);
            return;
        }
        if (leftbd) res.push_back(node->val);
        helper(node->left, leftbd && node->left, rightbd && !node->right, res);
        helper(node->right, leftbd && !node->left, rightbd && node->right, res);
        if (rightbd) res.push_back(node->val);
    }
};
```



下面这种解法实际上时解法一的迭代形式，整体思路基本一样，只是没有再用递归的写法，而是均采用while的迭代写法，注意在求右边界结点时迭代写法很难直接写出逆时针的顺序，我们可以先反过来保存，最后再调个顺序即可，参见代码如下：



解法三：

```
class Solution {
public:
    vector<int> boundaryOfBinaryTree(TreeNode* root) {
        if (!root) return {};
        vector<int> res, right;
        TreeNode *l = root->left, *r = root->right, *p = root;
        if (root->left || root->right) res.push_back(root->val);
        while (l && (l->left || l->right)) {
            res.push_back(l->val);
            if (l->left) l = l->left;
            else l = l->right;
        }
        stack<TreeNode*> st;
        while (p || !st.empty()) {
            if (p) {
                st.push(p);
                if (!p->left && !p->right) res.push_back(p->val);
                p = p->left;
            } else {
                p = st.top(); st.pop();
                p = p->right;
            }
        }
        while (r && (r->left || r->right)) {
            right.push_back(r->val);
            if (r->right) r = r->right;
            else r = r->left;
        }
        res.insert(res.end(), right.rbegin(), right.rend());
        return res;
    }
};
```



参考资料：

[https://discuss.leetcode.com/topic/87069/java-recursive-solution-beats-94/2](https://discuss.leetcode.com/topic/87069/java-recursive-solution-beats-94/2)

[https://discuss.leetcode.com/topic/84295/verbose-java-solution-easy-to-understand](https://discuss.leetcode.com/topic/84295/verbose-java-solution-easy-to-understand)



[https://discuss.leetcode.com/topic/84275/java-12ms-left-boundary-left-leaves-right-leaves-right-boundary](https://discuss.leetcode.com/topic/84275/java-12ms-left-boundary-left-leaves-right-leaves-right-boundary)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












