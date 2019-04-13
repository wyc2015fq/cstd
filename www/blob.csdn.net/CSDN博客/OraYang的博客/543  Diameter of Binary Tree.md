
# 543. Diameter of Binary Tree - OraYang的博客 - CSDN博客

2017年08月26日 09:16:29[OraYang](https://me.csdn.net/u010665216)阅读数：296标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



本题：
Given a binary tree, you need to compute the length of the diameter of the tree. The diameter of a binary tree is the length of thelongestpath between any two
 nodes in a tree. This path may or may not pass through the root.
Example:
Given a binary tree
1
         / \
        2   3
       / \     
      4   5
Return3, which is the length of the path [4,2,1,3] or [5,2,1,3].
Note:The length of path between two nodes is represented by the number of edges between them.
思路：
本题依然采用递归的思路，本题本质上求左右子树的最大深度之和，临界条件为，遍历到的结点为空，本题为了防止在遍历过程中，结点值的重复计算，利用哈希表建立树结点与当前结点左右子树深度和的映射
代码：

```python
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    int diameterOfBinaryTree(TreeNode* root) {
        int res = 0;
        maxDepth(root,res);
        return res;       
    }
private:
    unordered_map<TreeNode*,int> m;
    int maxDepth(TreeNode* node, int& res) {
    if (!node) return 0;
    if (m.count(node)) return m[node];
    int left = maxDepth(node->left, res);
    int right = maxDepth(node->right, res);
    res = max(res, left + right);
    return m[node] = (max(left, right) + 1);
    }
};
```



