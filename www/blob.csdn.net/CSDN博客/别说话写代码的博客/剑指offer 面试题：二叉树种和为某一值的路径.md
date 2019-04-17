# 剑指offer 面试题：二叉树种和为某一值的路径 - 别说话写代码的博客 - CSDN博客





2019年03月03日 13:29:12[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：11标签：[二叉树种和为某一值的路径																[剑指offer																[打印二叉树和为某值的所有路径](https://so.csdn.net/so/search/s.do?q=打印二叉树和为某值的所有路径&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[剑指offer刷题c++/python](https://blog.csdn.net/column/details/29881.html)](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)




题目：输入一颗二叉树的跟节点和一个整数，打印出二叉树中结点值的和为输入整数的所有路径。路径定义为从树的根结点开始往下一直到叶结点所经过的结点形成一条路径。(注意: 在返回值的list中，数组长度大的数组靠前)

思路：再写一个函数，用一个值cur 保存根节点到当前路径的和，如果cur==n并且当前是叶子节点，则算 一种成功情况。遍历完所有情况就行了。

```cpp
/*
struct TreeNode {
	int val;
	struct TreeNode *left;
	struct TreeNode *right;
	TreeNode(int x) :
			val(x), left(NULL), right(NULL) {
	}
};*/
class Solution {
public:
    void findpath(TreeNode* root,vector<vector<int>> &res,vector<int> &vec,int n,int cur)
    {
        vec.push_back(root->val);
        cur+=root->val;
        if(cur==n && root->left==nullptr && root->right==nullptr) res.push_back(vec);
        if(root->left) findpath(root->left,res,vec,n,cur);
        if(root->right) findpath(root->right,res,vec,n,cur);
        vec.pop_back();
    }
    vector<vector<int> > FindPath(TreeNode* root,int expectNumber) {
        if(!root) return {};
        vector<vector<int>> res;
        vector<int> vec;
        findpath(root,res,vec,expectNumber,0);
        return res;
    }
};
```](https://so.csdn.net/so/search/s.do?q=二叉树种和为某一值的路径&t=blog)




