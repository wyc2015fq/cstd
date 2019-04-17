# 剑指offer 面试题：树的子结构 - 别说话写代码的博客 - CSDN博客





2019年03月09日 09:35:31[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：10标签：[剑指offer																[树的子结构](https://so.csdn.net/so/search/s.do?q=树的子结构&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[剑指offer刷题c++/python](https://blog.csdn.net/column/details/29881.html)








题目：输入两棵二叉树A，B，判断B是不是A的子结构。（ps：我们约定空树不是任意一个树的子结构）

思路： 将要匹配的子树根节点一个个在树中查找，子树根等于当前树某节点，则一一匹配每个值，否则就遍历树中所有节点

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
    bool findall(TreeNode* r1,TreeNode* r2)
    {
        if(!r2) return true;
        if(!r1) return false;
        if(r1->val == r2->val) 
            return findall(r1->left,r2->left)
            && findall(r1->right,r2->right);
        return false;
    }
    bool HasSubtree(TreeNode* pRoot1, TreeNode* pRoot2)
    {
        if(!pRoot1 || !pRoot2) return false;
        if(pRoot1->val == pRoot2->val)
            return findall(pRoot1,pRoot2) || 
            HasSubtree(pRoot1->left,pRoot2)||
            HasSubtree(pRoot1->right,pRoot2);
        return false;
    }
};
```






