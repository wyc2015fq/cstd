# 剑指offer——树的子结构 - 风雪夜归人 - CSDN博客
2018年05月07日 20:36:31[cdjccio](https://me.csdn.net/qq_34624951)阅读数：42
个人分类：[剑指offer](https://blog.csdn.net/qq_34624951/article/category/7623846)
Q：输入两棵二叉树A，B，判断B是不是A的子结构。（ps：我们约定空树不是任意一个树的子结构）
A：要查找树A中是否存在和树B结构一样的子树。我们可以分为两步： 
      第一步在树A中找到和B的根节点一样的值一样的结点R，
      第二步在判断树A中以R为根节点的子树是不是包含和树B一样的结构。 
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
bool HasSubtree(TreeNode* pRoot1, TreeNode* pRoot2)
{
    bool result = false;
    if (pRoot1 != NULL && pRoot2 != NULL)
    {
        if (pRoot1->val == pRoot2->val)
        {
            //找到相同结点后判断是否含有相同结构
            result = isSubTree(pRoot1, pRoot2);
        }
        if (!result)//在左子树中查找相同结点
        {
            result = HasSubtree(pRoot1->left, pRoot2);
        }
        if (!result)//在右子树中找相同结点
        {
            result = HasSubtree(pRoot1->right, pRoot2);
        }
    }
    return result;
}
    
bool isSubTree(TreeNode* pRoot1, TreeNode* pRoot2)
{
    if(NULL == pRoot2)
    {
        return true;
    }
    if(NULL == pRoot1)
    {
        return false;
    }
    
    if(pRoot1->val != pRoot2->val)
    {
        return false;
    }
    return isSubTree(pRoot1->left, pRoot2->left) && isSubTree(pRoot1->right, pRoot2->right);
}
};
```
