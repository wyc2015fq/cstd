# 剑指offer——二叉树的镜像 - 风雪夜归人 - CSDN博客
2018年05月07日 21:09:01[cdjccio](https://me.csdn.net/qq_34624951)阅读数：54
个人分类：[剑指offer](https://blog.csdn.net/qq_34624951/article/category/7623846)
Q：题目描述
操作给定的二叉树，将其变换为源二叉树的镜像。
输入描述:
二叉树的镜像定义：源二叉树 
     8
    /  \
  6   10
/ \    / \
5  7 9 11
镜像二叉树
      8
     /  \
  10   6
  / \    / \
11 9 7  5
A：步骤：1.交换根结点的左右子结点；2.将左右子结点看作根结点进行1的操作
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
    void Mirror(TreeNode *pRoot) {
        if(NULL == pRoot || (NULL == pRoot->left && NULL == pRoot->right))
        {
            return;
        }
        
        swap(pRoot->left, pRoot->right);
        
        if(pRoot->left)
        {
            Mirror(pRoot->left);
        }
        if(pRoot->right)
        {
            Mirror(pRoot->right);
        }
    }
    /*
    void swap(TreeNode *pRoot1, TreeNode *pRoot2)
    {
        TreeNode *tmp = NULL;
        tmp = pRoot1;
        pRoot1 = pRoot2;
        pRoot2 = tmp;
    }
    */
};
```
