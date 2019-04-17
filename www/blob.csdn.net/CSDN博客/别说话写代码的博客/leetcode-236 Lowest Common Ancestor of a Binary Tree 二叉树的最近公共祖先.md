# leetcode-236. Lowest Common Ancestor of a Binary Tree 二叉树的最近公共祖先 - 别说话写代码的博客 - CSDN博客





2019年01月21日 16:47:43[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：22标签：[二叉树的最近公共祖先																[Lowest Common Ancestor of a Bi																[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=Lowest Common Ancestor of a Bi&t=blog)



|Given a binary tree, find the lowest common ancestor (LCA) of two given nodes in the tree.According to the [definition of LCA on Wikipedia](https://en.wikipedia.org/wiki/Lowest_common_ancestor): “The lowest common ancestor is defined between two nodes p and q as the lowest node in T that has both p and q as descendants (where we allow **a node to be a descendant of itself**).”Given the following binary tree:  root = [3,5,1,6,2,0,8,null,null,7,4]![](https://assets.leetcode.com/uploads/2018/12/14/binarytree.png)**Example 1:****Input:** root = [3,5,1,6,2,0,8,null,null,7,4], p = 5, q = 1**Output:** 3**Explanation: **The LCA of nodes `5` and `1` is `3.`**Example 2:****Input:** root = [3,5,1,6,2,0,8,null,null,7,4], p = 5, q = 4**Output:** 5**Explanation: **The LCA of nodes `5` and `4` is `5`, since a node can be a descendant of itself according to the LCA definition.**Note:**- All of the nodes' values will be unique.- p and q are different and both values will exist in the binary tree.|给定一个二叉树, 找到该树中两个指定节点的最近公共祖先。[百度百科](https://baike.baidu.com/item/%E6%9C%80%E8%BF%91%E5%85%AC%E5%85%B1%E7%A5%96%E5%85%88/8918834?fr=aladdin)中最近公共祖先的定义为：“对于有根树 T 的两个结点 p、q，最近公共祖先表示为一个结点 x，满足 x 是 p、q 的祖先且 x 的深度尽可能大（**一个节点也可以是它自己的祖先**）。”例如，给定如下二叉树:  root = [3,5,1,6,2,0,8,null,null,7,4]![](https://assets.leetcode-cn.com/aliyun-lc-upload/uploads/2018/12/15/binarytree.png)**示例 1:****输入:** root = [3,5,1,6,2,0,8,null,null,7,4], p = 5, q = 1**输出:** 3**解释: **节点 `5 `和节点 `1 `的最近公共祖先是节点 `3。`**示例 2:****输入:** root = [3,5,1,6,2,0,8,null,null,7,4], p = 5, q = 4**输出:** 5**解释: **节点 `5 `和节点 `4 `的最近公共祖先是节点 `5。`因为根据定义最近公共祖先节点可以为节点本身。**说明:**- 所有节点的值都是唯一的。- p、q 为不同节点且均存在于给定的二叉树中。|
|----|----|

思路：

 第一种：递归寻找。先看当前 节点是否为空，若为空直接返回空，若为p或者q的任意一个，也直接返回当前节点，否则分别对左右子树调用 递归函数。两个节点在树中有3中可能，p和q都在根左边，都在根右边或者一个左一个右。

1.若p和q要么分别位于左右子树中，那么对左右子结点调用递归函数，会分别返回p和q结点的位置，而当前结点正好就是p和q的最小共同父结点，直接返回当前结点即可，这就是题目中的例子1的情况。

2.若p和q同时位于左子树，这里有两种情况，一种情况是left会返回p和q中较高的那个位置，而right会返回空，所以我们最终返回非空的left即可，这就是题目中的例子2的情况。还有一种情况是会返回p和q的最小父结点，就是说当前结点的左子树中的某个结点才是p和q的最小父结点，会被返回。

3.若p和q同时位于右子树，同样这里有两种情况，一种情况是right会返回p和q中较高的那个位置，而left会返回空，所以我们最终返回非空的right即可，还有一种情况是会返回p和q的最小父结点，就是说当前结点的右子树中的某个结点才是p和q的最小父结点，会被返回，写法很简洁，代码如下

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
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        if(!root || root==p || root == q) return root;
        TreeNode *left = lowestCommonAncestor(root->left,p,q);
        //if(left && left !=p && left!=q) return left;
        TreeNode *right = lowestCommonAncestor(root->right,p,q);
        if(left && right) return root;
        return left ? left : right;
    }
};
```](https://so.csdn.net/so/search/s.do?q=二叉树的最近公共祖先&t=blog)




