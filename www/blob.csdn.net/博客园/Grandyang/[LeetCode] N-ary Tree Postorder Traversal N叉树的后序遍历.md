# [LeetCode] N-ary Tree Postorder Traversal N叉树的后序遍历 - Grandyang - 博客园







# [[LeetCode] N-ary Tree Postorder Traversal N叉树的后序遍历](https://www.cnblogs.com/grandyang/p/9881420.html)







Given an n-ary tree, return the *postorder* traversal of its nodes' values.

For example, given a `3-ary` tree:



![](https://assets.leetcode.com/uploads/2018/10/12/narytreeexample.png)



Return its postorder traversal as: `[5,6,3,2,4,1]`.



Note:

Recursive solution is trivial, could you do it iteratively?



这道题让我们求N叉树的后序遍历，由于有了之前那道[Binary Tree Postorder Traversal](http://www.cnblogs.com/grandyang/p/4251757.html)的基础，了解了二叉树的后序遍历，则N叉树的后序遍历也就没有那么难了。首先还是用递归来做，在递归函数中，判空后，遍历子结点数组，对所有的子结点调用递归函数，然后在for循环之外在将当前结点值加入结果res数组，这样才能保证是后序遍历的顺序，参见代码如下：



```
class Solution {
public:
    vector<int> postorder(Node* root) {
        vector<int> res;
        helper(root, res);
        return res;
    }
    void helper(Node* node, vector<int>& res) {
        if (!node) return;
        for (Node* child : node->children) {
            helper(child, res);
        }
        res.push_back(node->val);
    }
};
```



我们也可以使用迭代的方法来做，这里有个小trick，写法跟先序遍历十分的像，不同的就是每次把从stack中取的结点的值都加到结果res的最前面，还有就是遍历子结点数组的顺序是正常的顺序，而前序遍历是从子结点数组的后面往前面遍历，这点区别一定要注意，参见代码如下：



解法二：

```
class Solution {
public:
    vector<int> postorder(Node* root) {
        if (!root) return {};
        vector<int> res;
        stack<Node*> st{{root}};
        while (!st.empty()) {
            Node *t = st.top(); st.pop();
            res.insert(res.begin(), t->val);
            for (Node* child : t->children) {
                if (child) st.push(child);
            }
        }
        return res;
    }
};
```



类似题目：

[Binary Tree Postorder Traversal](http://www.cnblogs.com/grandyang/p/4251757.html)

[N-ary Tree Preorder Traversal](https://www.cnblogs.com/grandyang/p/9881753.html)

[N-ary Tree Level Order Traversal](https://www.cnblogs.com/grandyang/p/9672233.html)



参考资料：

[https://leetcode.com/problems/n-ary-tree-preorder-traversal/](https://leetcode.com/problems/n-ary-tree-preorder-traversal/)

[https://leetcode.com/problems/n-ary-tree-postorder-traversal/discuss/147959/Java-Iterative-and-Recursive-Solutions](https://leetcode.com/problems/n-ary-tree-postorder-traversal/discuss/147959/Java-Iterative-and-Recursive-Solutions)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












