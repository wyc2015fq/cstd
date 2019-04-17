# [LeetCode] N-ary Tree Preorder Traversal N叉树的前序遍历 - Grandyang - 博客园







# [[LeetCode] N-ary Tree Preorder Traversal N叉树的前序遍历](https://www.cnblogs.com/grandyang/p/9881753.html)







Given an n-ary tree, return the *preorder* traversal of its nodes' values.

For example, given a `3-ary` tree:



![](https://assets.leetcode.com/uploads/2018/10/12/narytreeexample.png)



Return its preorder traversal as: `[1,3,5,6,2,4]`.



Note:

Recursive solution is trivial, could you do it iteratively?



这道题让我们求N叉树的前序遍历，有之前那道[Binary Tree Preorder Traversal](http://www.cnblogs.com/grandyang/p/4146981.html)的基础，知道了二叉树的前序遍历的方法，很容易就可以写出N叉树的前序遍历。先来看递归的解法，主要实现一个递归函数即可，判空之后，将当前结点值加入结果res中，然后遍历子结点数组中所有的结点，对每个结点都调用递归函数即可，参见代码如下：



解法一：

```
class Solution {
public:
    vector<int> preorder(Node* root) {
        vector<int> res;
        helper(root, res);
        return res;
    }
    void helper(Node* node, vector<int>& res) {
        if (!node) return;
        res.push_back(node->val);
        for (Node* child : node->children) {
            helper(child, res);
        }
    }
};
```



我们也可以使用迭代的解法来做，使用栈stack来辅助，需要注意的是，如果使用栈的话，我们遍历子结点数组的顺序应该是从后往前的，因为栈是后进先出的顺序，所以需要最先遍历的子结点应该最后进栈，参见代码如下：



解法二：

```
class Solution {
public:
    vector<int> preorder(Node* root) {
        if (!root) return {};
        vector<int> res;
        stack<Node*> st{{root}};
        while (!st.empty()) {
            Node* t = st.top(); st.pop();
            res.push_back(t->val);
            for (int i = (int)t->children.size() - 1; i >= 0; --i) {
                st.push(t->children[i]);
            }
        }
        return res;
    }
};
```



类似题目：

[Binary Tree Preorder Traversal](http://www.cnblogs.com/grandyang/p/4146981.html)

[N-ary Tree Level Order Traversal](https://www.cnblogs.com/grandyang/p/9672233.html)

[N-ary Tree Postorder Traversal](https://www.cnblogs.com/grandyang/p/9881420.html)



参考资料：

[https://leetcode.com/problems/n-ary-tree-preorder-traversal/](https://leetcode.com/problems/n-ary-tree-preorder-traversal/)

[https://leetcode.com/problems/n-ary-tree-preorder-traversal/discuss/147955/Java-Iterative-and-Recursive-Solutions](https://leetcode.com/problems/n-ary-tree-preorder-traversal/discuss/147955/Java-Iterative-and-Recursive-Solutions)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












