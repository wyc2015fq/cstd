# [LeetCode] N-ary Tree Level Order Traversal N叉树层序遍历 - Grandyang - 博客园







# [[LeetCode] N-ary Tree Level Order Traversal N叉树层序遍历](https://www.cnblogs.com/grandyang/p/9672233.html)







Given an n-ary tree, return the level order traversal of its nodes' values. (ie, from left to right, level by level).

For example, given a `3-ary` tree:



![](https://leetcode.com/static/images/problemset/NaryTreeExample.png)



We should return its level order traversal:




[
     [1],
     [3,2,4],
     [5,6]
]



Note:
- The depth of the tree is at most `1000`.
- The total number of nodes is at most `5000`.



这道题给了我们一棵N叉树，让我们对其进行层序遍历。我们做过之前二叉树的层序遍历的那道题的话[Binary Tree Level Order Traversal](http://www.cnblogs.com/grandyang/p/4051321.html)，那么这道题也就不难了。虽说现在每一个结点可能有很多个子结点，但其实处理的思路的都是一样的。子结点放到了一个children数组中，我们访问的时候只要遍历数组就行了。先来看迭代的写法，用到了队列queue来辅助，首先判断root是否为空，为空直接返回空数组，否则加入queue中。然后遍历queue，这里用的trick就是，要加个for循环，要将当前queue中的结点的个数统计下来，因为再加入下一层的结点时，queue的结点个数会增加，而在加入下一层结点之前，当前queue中的结点个数全都属于一层，所以我们要把层与层区分开来，将同一层的结点都放到一个数组out中，之后再放入结果res中，这种层序遍历的思想在迷宫遍历找最短路径的时候应用的也很多，是个必须要掌握的方法呢，参见代码如下：



解法一：

```
class Solution {
public:
    vector<vector<int>> levelOrder(Node* root) {
        if (!root) return {};
        vector<vector<int>> res;
        queue<Node*> q{{root}};
        while (!q.empty()) {
            vector<int> out;
            for (int i = q.size(); i > 0; --i) {
                auto t = q.front(); q.pop();
                out.push_back(t->val);
                if (!t->children.empty()) {
                    for (auto a : t->children) q.push(a);
                }
            }
            res.push_back(out);
        }
        return res;
    }
};
```



下面再来看递归的写法，其实层序遍历天然适合迭代的写法，但我们强行递归也是可以的，就是有点秀。由于递归DFS的设定是一条路走到黑再返回，那么必然会跨越不同的层数，所以为了区别当前的层，我们需要一个变量level来标记当前的层数，根结点root就是第0层，依此类推往上加。然后还有个trick就是关于结果res的大小，由于我们并不知道树的深度，所以一旦我们遍历的层数超过了当前res的大小，我们需要resize一下，这样才不会出错。之后，我们将当前遍历到的结点加到res中的第level层中，然后遍历子结点数组，对每一个子结点调用递归函数即可，参见代码如下：



解法二：

```
class Solution {
public:
    vector<vector<int>> levelOrder(Node* root) {
        vector<vector<int>> res;
        helper(root, 0, res);
        return res;
    }
    void helper(Node* node, int level, vector<vector<int>>& res) {
        if (!node) return;
        if (res.size() <= level) res.resize(res.size() + 1);
        res[level].push_back(node->val);
        for (auto a : node->children) {
            helper(a, level + 1, res);
        }
    }
};
```



类似题目：

[Binary Tree Level Order Traversal](http://www.cnblogs.com/grandyang/p/4051321.html)

N-ary Tree Preorder Traversal

N-ary Tree Postorder Traversal



参考资料：

[https://leetcode.com/problems/n-ary-tree-level-order-traversal/description/](https://leetcode.com/problems/n-ary-tree-level-order-traversal/description/)

[https://leetcode.com/problems/n-ary-tree-level-order-traversal/discuss/156218/Typical-C++-recursive-solution](https://leetcode.com/problems/n-ary-tree-level-order-traversal/discuss/156218/Typical-C++-recursive-solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












