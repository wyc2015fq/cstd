# leetcode-226. Invert Binary Tree 翻转二叉树 - 别说话写代码的博客 - CSDN博客





2019年01月26日 22:24:52[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：16标签：[Invert Binary Tree																[翻转二叉树																[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=翻转二叉树&t=blog)



|Invert a binary tree.**Example:**Input:     4   /   \  2     7 / \   / \1   3 6   9Output:     4   /   \  7     2 / \   / \9   6 3   1**Trivia:**			This problem was inspired by [this original tweet](https://twitter.com/mxcl/status/608682016205344768) by [Max Howell](https://twitter.com/mxcl):> Google: 90% of our engineers use the software you wrote (Homebrew), but you can’t invert a binary tree on a whiteboard so f*** off.|翻转一棵二叉树。**示例：**输入：     4   /   \  2     7 / \   / \1   3 6   9输出：     4   /   \  7     2 / \   / \9   6 3   1**备注:**			这个问题是受到 [Max Howell](https://twitter.com/mxcl)的 [原问题](https://twitter.com/mxcl/status/608682016205344768) 启发的 ：> 谷歌：我们90％的工程师使用您编写的软件(Homebrew)，但是您却无法在面试时在白板上写出翻转二叉树这道题，这太糟糕了。|
|----|----|

思路：递归交换 每个节点的左右节点就完事儿了。

```cpp
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
    TreeNode* invertTree(TreeNode* root) {
        if(!root) return NULL;
        TreeNode* tmp = root->left;
        root->left = invertTree(root->right);
        root->right = invertTree(tmp);
        return root;
    }
};
```](https://so.csdn.net/so/search/s.do?q=Invert Binary Tree&t=blog)




