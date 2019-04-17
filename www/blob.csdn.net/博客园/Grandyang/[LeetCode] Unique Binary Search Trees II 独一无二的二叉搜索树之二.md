# [LeetCode] Unique Binary Search Trees II 独一无二的二叉搜索树之二 - Grandyang - 博客园







# [[LeetCode] Unique Binary Search Trees II 独一无二的二叉搜索树之二](https://www.cnblogs.com/grandyang/p/4301096.html)







Given *n*, generate all structurally unique BST's (binary search trees) that store values 1...*n*.

For example,
Given *n* = 3, your program should return all 5 unique BST's shown below.
   1         3     3      2      1
    \       /     /      / \      \
     3     2     1      1   3      2
    /     /       \                 \
   2     1         2                 3



confused what `"{1,#,2,3}"` means? [> read more on how binary tree is serialized on OJ.](https://oj.leetcode.com/problems/unique-binary-search-trees-ii/)


OJ's Binary Tree Serialization:

The serialization of a binary tree follows a level order traversal, where '#' signifies a path terminator where no node exists below.

Here's an example:
   1
  / \
 2   3
    /
   4
    \
     5

The above binary tree is serialized as `"{1,2,3,#,#,4,#,#,5}"`.



这道题是之前的[Unique Binary Search Trees 独一无二的二叉搜索树](http://www.cnblogs.com/grandyang/p/4299608.html)的延伸，之前那个只要求算出所有不同的二叉搜索树的个数，这道题让把那些二叉树都建立出来。这种建树问题一般来说都是用递归来解，这道题也不例外，划分左右子树，递归构造。至于递归函数中为啥都用的是指针，是参考了网友[水中的鱼的博客](http://fisherlei.blogspot.com/2013/03/leetcode-unique-binary-search-trees-ii.html)，若不用指针，全部实例化的话会存在大量的对象拷贝，要调用拷贝构造函数，具体我也不太懂，反正感觉挺有道理的，不明觉厉啊-.-!!!



```
class Solution {
public:
    vector<TreeNode *> generateTrees(int n) {
        if (n == 0) return {};
        return *generateTreesDFS(1, n);
    }
    vector<TreeNode*> *generateTreesDFS(int start, int end) {
        vector<TreeNode*> *subTree = new vector<TreeNode*>();
        if (start > end) subTree->push_back(NULL);
        else {
            for (int i = start; i <= end; ++i) {
                vector<TreeNode*> *leftSubTree = generateTreesDFS(start, i - 1);
                vector<TreeNode*> *rightSubTree = generateTreesDFS(i + 1, end);
                for (int j = 0; j < leftSubTree->size(); ++j) {
                    for (int k = 0; k < rightSubTree->size(); ++k) {
                        TreeNode *node = new TreeNode(i);
                        node->left = (*leftSubTree)[j];
                        node->right = (*rightSubTree)[k];
                        subTree->push_back(node);
                    }
                }
            }
        }
        return subTree;
    }
};
```



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












