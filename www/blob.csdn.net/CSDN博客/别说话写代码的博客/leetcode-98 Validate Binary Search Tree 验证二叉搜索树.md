# leetcode-98. Validate Binary Search Tree 验证二叉搜索树 - 别说话写代码的博客 - CSDN博客





2019年01月24日 11:20:22[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：36标签：[Validate Binary Search Tree																[验证二叉搜索树																[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=验证二叉搜索树&t=blog)



|Given a binary tree, determine if it is a valid binary search tree (BST).Assume a BST is defined as follows:- The left subtree of a node contains only nodes with keys **less than** the node's key.- The right subtree of a node contains only nodes with keys **greater than** the node's key.- Both the left and right subtrees must also be binary search trees.**Example 1:****Input:**    2   / \  1   3**Output:** true**Example 2:**    5   / \  1   4     / \    3   6**Output:** false**Explanation:** The input is: [5,1,4,null,null,3,6]. The root node's value             is 5 but its right child's value is 4.|给定一个二叉树，判断其是否是一个有效的二叉搜索树。假设一个二叉搜索树具有如下特征：- 节点的左子树只包含**小于**当前节点的数。- 节点的右子树只包含**大于**当前节点的数。- 所有左子树和右子树自身必须也是二叉搜索树。**示例 1:****输入:**    2   / \  1   3**输出:** true**示例 2:****输入:**    5   / \  1   4     / \    3   6**输出:** false**解释:** 输入为: [5,1,4,null,null,3,6]。     根节点的值为 5 ，但是其右子节点值为 4 。|
|----|----|

思路：

**第一种**：递归找，因为二叉搜索树中序遍历有序，左子节点<根节点<右子节点值。自己写一个递归函数，函数的两个参数分别为树已遍历的最小值和最大值。注意，这里设置Long是为了避免处理int的最大值和最小值。速度很快，超越100%

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
    bool isValidBST(TreeNode* root) {
        return isvalid(root,LONG_MIN,LONG_MAX);
    }
    bool isvalid(TreeNode* root,long mn,long mx)
    {
        if(!root) return true;
        if(root->val <=mn || root->val >=mx) return false;
        return isvalid(root->left,mn,root->val) &&
            isvalid(root->right,root->val,mx);
    }
};
```

**第二种**：既然它 中序遍历是有序的，所以将其按照中序遍历有序存到数组 ，然后遍历数组看数组是否是有序的。速度一般，超越65%

```cpp
class Solution {
public:
    bool isValidBST(TreeNode* root) {
        if(!root) return true;
        vector<int> vec;
        inorder(root,vec);
        for(int i=1;i<vec.size();++i)
            if(vec[i]<=vec[i-1])
                return false;
        return true;
    }
    void inorder(TreeNode* root,vector<int> &vec)
    {
        if(!root) return ;
        inorder(root->left,vec);
        vec.push_back(root->val);
        inorder(root->right,vec);
    }
    
};
```](https://so.csdn.net/so/search/s.do?q=Validate Binary Search Tree&t=blog)




