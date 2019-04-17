# leetcode-222. Count Complete Tree Nodes 完全二叉树的节点个数 - 别说话写代码的博客 - CSDN博客





2019年03月15日 15:43:51[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：9标签：[Count Complete Tree Nodes																[完全二叉树节点个数																[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=完全二叉树节点个数&t=blog)



|Given a **complete** binary tree, count the number of nodes.**Note:****Definition of a complete binary tree from [Wikipedia](http://en.wikipedia.org/wiki/Binary_tree#Types_of_binary_trees):**			In a complete binary tree every level, except possibly the last, is completely filled, and all nodes in the last level are as far left as possible. It can have between 1 and 2h nodes inclusive at the last level h.**Example:****Input:**     1   / \  2   3 / \  /4  5 6**Output:** 6|给出一个**完全二叉树**，求出该树的节点个数。**说明：**[完全二叉树](https://baike.baidu.com/item/%E5%AE%8C%E5%85%A8%E4%BA%8C%E5%8F%89%E6%A0%91/7773232?fr=aladdin)的定义如下：在完全二叉树中，除了最底层节点可能没填满外，其余每层节点数都达到最大值，并且最下面一层的节点都集中在该层最左边的若干位置。若最底层为第 h 层，则该层包含 1~ 2h 个节点。**示例:****输入:**     1   / \  2   3 / \  /4  5 6**输出:** 6|
|----|----|

思路：

**第一种**：暴力，求书中节点个数。O（n），对各种类型的树都适用,下面这种是递归求，也可以用层次遍历那种方法求

```cpp
int count(TreeNode *node)
{
    if(!node) return 0;
    return count(node->left)+count(node->right)+1;
}
```

**第二种**：利用 完全二叉树的性质。完全二叉树只可能在最后一层没满。若为满二叉树则 直接用2^h-1就可以求出元素个数。举个栗子， 题目图中的树，我们只需要判断最左子节点的高度和最右子节点的高度是否相等。若相等也就是满二叉树，则直接返回pow(2,h)-1.若不相等，则分别遍历根节点左子树2和右子树3。左子树2的最左和最右高度一样，说明子树是满的，可以直接用pow(2,h)-1求出。根节点右子树3不是满的，那就继续遍历，知道满足左右子树相等的条件（叶节点）为止。

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
    int countNodes(TreeNode* root) {
        if(!root) return 0;
        int l=0,r=0;
        TreeNode* left=root,*right=root;
        while(left)   //一直往左走并统计左子树高度
        {
            left=left->left;
            l++;
        }
        while(right)    //一直往右走并统计右子树高度
        {
            right=right->right;
            r++;
        }
        if(l==r) return pow(2,l)-1;    //最左和最右子树一样高，为满二叉树
        return countNodes(root->left)+countNodes(root->right)+1;  //返回左子树和右子树之和+1
    }
};
```](https://so.csdn.net/so/search/s.do?q=Count Complete Tree Nodes&t=blog)




