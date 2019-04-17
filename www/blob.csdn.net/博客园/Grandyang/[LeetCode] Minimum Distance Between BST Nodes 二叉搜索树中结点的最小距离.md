# [LeetCode] Minimum Distance Between BST Nodes 二叉搜索树中结点的最小距离 - Grandyang - 博客园







# [[LeetCode] Minimum Distance Between BST Nodes 二叉搜索树中结点的最小距离](https://www.cnblogs.com/grandyang/p/9062143.html)







Given a Binary Search Tree (BST) with the root node `root`, return the minimum difference between the values of any two different nodes in the tree.

Example :
Input: root = [4,2,6,1,3,null,null]
Output: 1
Explanation:
Note that root is a TreeNode object, not an array.

The given tree [4,2,6,1,3,null,null] is represented by the following diagram:

          4
        /   \
      2      6
     / \    
    1   3  

while the minimum difference in this tree is 1, it occurs between node 1 and node 2, also between node 3 and node 2.

Note:
- The size of the BST will be between 2 and `100`.
- The BST is always valid, each node's value is an integer, and each node's value is different.



这道题跟之前那道[Minimum Absolute Difference in BST](http://www.cnblogs.com/grandyang/p/6540165.html)没有任何区别，解法完全可以共用，讲解也可以参见之前的帖子，这里就简略的说一下。第一种方法很直接，通过中序遍历按顺序从小到大将所有的结点值都存入到一个数组中，然后就遍历这个数组，找相邻的两个的差值最小的返回即可，参见代码如下：



解法一：

```
class Solution {
public:
    int minDiffInBST(TreeNode* root) {
        int res = INT_MAX;
        vector<int> v;
        helper(root, v);
        for (int i = 1; i < v.size(); ++i) {
            res = min(res, v[i] - v[i - 1]);
        }
        return res;
    }
    void helper(TreeNode* node, vector<int>& vals) {
        if (!node) return;
        helper(node->left, vals);
        vals.push_back(node->val);
        helper(node->right, vals);
    }
};
```



我们可以优化上面解法的空间复杂度，并不记录所有的结点值，而是只记录之前的结点值，然后做差值更新结果res即可。



解法二：

```
class Solution {
public:
    int minDiffInBST(TreeNode* root) {
        int res = INT_MAX, pre = -1;
        helper(root, pre, res);
        return res;
    }
    void helper(TreeNode* node, int& pre, int& res) {
        if (!node) return;
        helper(node->left, pre, res);
        if (pre != -1) res = min(res, node->val - pre);
        pre = node->val;
        helper(node->right, pre, res);
    }
};
```



其实我们也不必非要用中序遍历不可，用先序遍历同样可以利用到BST的性质，我们带两个变量low和high来分别表示上下界，初始化为int的极值，然后我们在递归函数中，分别用上下界和当前节点值的绝对差来更新结果res，参见代码如下：



解法三：

```
class Solution {
public:
    int minDiffInBST(TreeNode* root) {
        int res = INT_MAX;
        helper(root, INT_MIN, INT_MAX, res);
        return res;
    }
    void helper(TreeNode* node, int low, int high, int& res) {
        if (!node) return;
        if (low != INT_MIN) res = min(res, node->val - low);
        if (high != INT_MAX) res = min(res, high - node->val);
        helper(node->left, low, node->val, res);
        helper(node->right, node->val, high, res);
    }
};
```



下面这种方法是解法一的迭代的写法，思路跟之前的解法没有什么区别，参见代码如下：



解法四：

```
class Solution {
public:
    int minDiffInBST(TreeNode* root) {
        int res = INT_MAX, pre = -1;
        stack<TreeNode*> st;
        TreeNode* p = root;
        while (!st.empty() || p) {
            if (p) {
                st.push(p);
                p = p->left;
            } else {
                p = st.top(); st.pop();
                if (pre != -1) res = min(res, p->val - pre);
                pre = p->val;
                p = p->right;
            }
        }
        return res;
    }
};
```



类似题目：

[Minimum Absolute Difference in BST](http://www.cnblogs.com/grandyang/p/6540165.html)

[Binary Tree Inorder Traversal](http://www.cnblogs.com/grandyang/p/4297300.html)



参考资料：

[https://leetcode.com/problems/minimum-distance-between-bst-nodes/solution/](https://leetcode.com/problems/minimum-distance-between-bst-nodes/solution/)

[https://leetcode.com/problems/minimum-distance-between-bst-nodes/discuss/114834/Inorder-Traversal-O(N)-time-Recursion-C++JavaPython](https://leetcode.com/problems/minimum-distance-between-bst-nodes/discuss/114834/Inorder-Traversal-O(N)-time-Recursion-C++JavaPython)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












