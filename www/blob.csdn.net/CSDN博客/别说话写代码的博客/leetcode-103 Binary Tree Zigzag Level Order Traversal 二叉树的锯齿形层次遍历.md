# leetcode-103. Binary Tree Zigzag Level Order Traversal 二叉树的锯齿形层次遍历 - 别说话写代码的博客 - CSDN博客





2019年01月25日 12:52:51[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：28标签：[Binary Tree Zigzag Level Order																[二叉树的锯齿形层次遍历																[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=二叉树的锯齿形层次遍历&t=blog)



|Given a binary tree, return the *zigzag level order* traversal of its nodes' values. (ie, from left to right, then right to left for the next level and alternate between).For example:			Given binary tree `[3,9,20,null,null,15,7]`,```3   / \  9  20    /  \   15   7```return its zigzag level order traversal as:```[  [3],  [20,9],  [15,7]]```|给定一个二叉树，返回其节点值的锯齿形层次遍历。（即先从左往右，再从右往左进行下一层遍历，以此类推，层与层之间交替进行）。例如：			给定二叉树 `[3,9,20,null,null,15,7]`,```3   / \  9  20    /  \   15   7```返回锯齿形层次遍历如下：```[  [3],  [20,9],  [15,7]]```|
|----|----|

思路：

**第一种**：和 [上一题](https://blog.csdn.net/qq_21997625/article/details/86626096)很类似，直接 层序遍历，每次遍历到偶数层时翻转就行

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
    vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
        if(!root) return {};
        vector<vector<int>> res;
        queue<TreeNode*> q{{root}};
        bool b=false;
        while(!q.empty())
        {
            vector<int> vec;
            for(int i =q.size();i>0;--i)
            {
                TreeNode *p=q.front();  q.pop();
                vec.push_back(p->val);
                if(p->left) q.push(p->left);
                if(p->right) q.push(p->right);
            }
            if(b) reverse(vec.begin(),vec.end());
            b=!b;
            res.push_back(vec);
        }return res;
    }
};
```

**第二种**：用两个栈，相邻两个栈分别存到两个栈中，一个从左到右入栈，一个从右到左入栈](https://so.csdn.net/so/search/s.do?q=Binary Tree Zigzag Level Order&t=blog)




