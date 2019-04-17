# [LeetCode] Maximum Depth of N-ary Tree N叉树的最大深度 - Grandyang - 博客园







# [[LeetCode] Maximum Depth of N-ary Tree N叉树的最大深度](https://www.cnblogs.com/grandyang/p/9873496.html)







Given a n-ary tree, find its maximum depth.

The maximum depth is the number of nodes along the longest path from the root node down to the farthest leaf node.

For example, given a `3-ary` tree:



![](https://assets.leetcode.com/uploads/2018/10/12/narytreeexample.png)



We should return its max depth, which is 3.



Note:
- The depth of the tree is at most `1000`.
- The total number of nodes is at most `5000`.



这道题让我们求一个N叉树的最大深度，由于之前做过 [Maximum Depth of Binary Tree](http://www.cnblogs.com/grandyang/p/4051348.html) 那道题，所以有了求二叉树的基础后，求N叉树也就不难了，无非就是稍稍变换了一下吗，由固定的左右子结点变成了一个一堆子结点，但是方法还是没有变。首先来看一种常见的递归解法，就是需要有一个当前深度，然后带一个全局变量res进去。在递归函数中，如果node为空，直接返回。若子结点数组为空，那么结果res和cur比较取较大值。否则就遍历子结点数组，对每个子结点调用递归函数，这里的cur要自增1，参见代码如下：



解法一：

```
class Solution {
public:
    int maxDepth(Node* root) {
        int res = 0;
        helper(root, 1, res);
        return res;
    }
    void helper(Node* node, int cur, int& res) {
        if (!node) return;
        if (node->children.empty()) res = max(res, cur);
        for (Node* child : node->children) {
            helper(child, cur + 1, res);
        }
    }
};
```



我们也可以不使用其他的函数，直接主函数中递归，首先判空，否则就是遍历子结点数组，然后对每个子结点调用递归函数的返回值加1后跟res相比，取较大值更新结果res，参见代码如下：



解法二：

```
class Solution {
public:
    int maxDepth(Node* root) {
        if (!root) return 0;
        int res = 1;
        for (Node* child : root->children) {
            res = max(res, maxDepth(child) + 1);
        }
        return res;
    }
};
```



我们也可以不使用递归，而是用迭代的形式，这里借助队列queue来做，就是BFS的经典写法，不算难，参见代码如下：



解法三：

```
class Solution {
public:
    int maxDepth(Node* root) {
        if (!root) return 0;
        int res = 0;
        queue<Node*> q{{root}};
        while (!q.empty()) {
            for (int i = q.size(); i > 0; --i) {
                auto t = q.front(); q.pop();
                for (auto child : t->children) {
                    if (child) q.push(child);
                }
            }
            ++res;
        }
        return res;
    }
};
```



类似题目：

[Maximum Depth of Binary Tree](http://www.cnblogs.com/grandyang/p/4051348.html)



参考资料：

[https://leetcode.com/problems/maximum-depth-of-n-ary-tree/](https://leetcode.com/problems/maximum-depth-of-n-ary-tree/)

[https://leetcode.com/problems/maximum-depth-of-n-ary-tree/discuss/148544/Java-Top-down-DFS-solutions](https://leetcode.com/problems/maximum-depth-of-n-ary-tree/discuss/148544/Java-Top-down-DFS-solutions)

[https://leetcode.com/problems/maximum-depth-of-n-ary-tree/discuss/167010/DFS-and-BFS-solutions-in-C%2B%2B.](https://leetcode.com/problems/maximum-depth-of-n-ary-tree/discuss/167010/DFS-and-BFS-solutions-in-C%2B%2B.)

[https://leetcode.com/problems/maximum-depth-of-n-ary-tree/discuss/151804/Solution-Python-C%2B%2B-Simple-with-explanation](https://leetcode.com/problems/maximum-depth-of-n-ary-tree/discuss/151804/Solution-Python-C%2B%2B-Simple-with-explanation)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












