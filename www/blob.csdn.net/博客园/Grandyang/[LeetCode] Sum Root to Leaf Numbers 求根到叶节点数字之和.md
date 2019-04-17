# [LeetCode] Sum Root to Leaf Numbers 求根到叶节点数字之和 - Grandyang - 博客园







# [[LeetCode] Sum Root to Leaf Numbers 求根到叶节点数字之和](https://www.cnblogs.com/grandyang/p/4273700.html)







Given a binary tree containing digits from `0-9` only, each root-to-leaf path could represent a number.

An example is the root-to-leaf path `1->2->3` which represents the number `123`.

Find the total sum of all root-to-leaf numbers.

Note: A leaf is a node with no children.

Example:
Input: [1,2,3]
    1
   / \
  2   3
Output: 25
Explanation:
The root-to-leaf path `1->2` represents the number `12`.
The root-to-leaf path `1->3` represents the number `13`.
Therefore, sum = 12 + 13 = `25`.
Example 2:
Input: [4,9,0,5,1]
    4
   / \
  9   0
 / \
5   1
Output: 1026
Explanation:
The root-to-leaf path `4->9->5` represents the number 495.
The root-to-leaf path `4->9->1` represents the number 491.
The root-to-leaf path `4->0` represents the number 40.
Therefore, sum = 495 + 491 + 40 = `1026`.


这道求根到叶节点数字之和的题跟之前的求 [Path Sum](http://www.cnblogs.com/grandyang/p/4036961.html)很类似，都是利用DFS递归来解，这道题由于不是单纯的把各个节点的数字相加，而是每遇到一个新的子结点的数字，要把父结点的数字扩大10倍之后再相加。如果遍历到叶结点了，就将当前的累加结果sum返回。如果不是，则对其左右子结点分别调用递归函数，将两个结果相加返回即可，参见代码如下：



解法一：

```
class Solution {
public:
    int sumNumbers(TreeNode* root) {
        return sumNumbersDFS(root, 0);
    }
    int sumNumbersDFS(TreeNode* root, int sum) {
        if (!root) return 0;
        sum = sum * 10 + root->val;
        if (!root->left && !root->right) return sum;
        return sumNumbersDFS(root->left, sum) + sumNumbersDFS(root->right, sum);
    }
};
```



我们也可以采用迭代的写法，这里用的是先序遍历的迭代写法，使用栈来辅助遍历，首先将根结点压入栈，然后进行while循环，取出栈顶元素，如果是叶结点，那么将其值加入结果res。如果其右子结点存在，那么其结点值加上当前结点值的10倍，再将右子结点压入栈。同理，若左子结点存在，那么其结点值加上当前结点值的10倍，再将左子结点压入栈，是不是跟之前的 [Path Sum](http://www.cnblogs.com/grandyang/p/4036961.html) 极其类似呢，参见代码如下：



解法二：

```
class Solution {
public:
    int sumNumbers(TreeNode* root) {
        if (!root) return 0;
        int res = 0;
        stack<TreeNode*> st{{root}};
        while (!st.empty()) {
            TreeNode *t = st.top(); st.pop();
            if (!t->left && !t->right) {
                res += t->val;
            }
            if (t->right) {
                t->right->val += t->val * 10;
                st.push(t->right);
            }
            if (t->left) {
                t->left->val += t->val * 10;
                st.push(t->left);
            }
        }
        return res;
    }
};
```



类似题目：

[Path Sum](http://www.cnblogs.com/grandyang/p/4036961.html)

[Binary Tree Maximum Path Sum](http://www.cnblogs.com/grandyang/p/4280120.html)



参考资料：

[https://leetcode.com/problems/sum-root-to-leaf-numbers/](https://leetcode.com/problems/sum-root-to-leaf-numbers/)

[https://leetcode.com/problems/sum-root-to-leaf-numbers/discuss/41367/Non-recursive-preorder-traverse-Java-solution](https://leetcode.com/problems/sum-root-to-leaf-numbers/discuss/41367/Non-recursive-preorder-traverse-Java-solution)

[https://leetcode.com/problems/sum-root-to-leaf-numbers/discuss/41452/Iterative-C%2B%2B-solution-using-stack-(similar-to-postorder-traversal)](https://leetcode.com/problems/sum-root-to-leaf-numbers/discuss/41452/Iterative-C%2B%2B-solution-using-stack-(similar-to-postorder-traversal))



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












