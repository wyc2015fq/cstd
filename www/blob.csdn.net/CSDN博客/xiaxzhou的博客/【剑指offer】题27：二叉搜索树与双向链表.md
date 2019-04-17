# 【剑指offer】题27：二叉搜索树与双向链表 - xiaxzhou的博客 - CSDN博客





2017年07月03日 10:36:32[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：108








```
struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
    TreeNode(int x) :
        val(x), left(NULL), right(NULL) {
    }
};

TreeNode* FindLeftMax(TreeNode* pRoot)
{
    if (pRoot->left==NULL)
    {
        return NULL;
    }
    TreeNode* leftmax = pRoot->left;
    while (leftmax->right != NULL)
    {
        leftmax = leftmax->right;
    }
    return leftmax;
}
TreeNode* FindRightMin(TreeNode* pRoot)
{
    if (pRoot->right==NULL)
    {
        return NULL;
    }
    TreeNode* rightmin= pRoot->right;
    while (rightmin->left!= NULL)
    {
        rightmin = rightmin->left;
    }
    return rightmin;
}
void Convert_core(TreeNode* pRoot)
{
    if (pRoot == NULL)
    {
        return;
    }
    TreeNode* leftmax = FindLeftMax(pRoot);
    TreeNode* RightMin= FindRightMin(pRoot);
    Convert_core(pRoot->left);
    Convert_core(pRoot->right);
    if (leftmax!=NULL)
    {
        pRoot->left = leftmax;
        leftmax->right = pRoot;
    }
    if (RightMin!= NULL)
    {
        pRoot->right= RightMin;
        RightMin->left = pRoot;
    }
}

TreeNode* Convert(TreeNode* pRootOfTree)
{
    if (pRootOfTree == NULL)
    {
        return NULL;
    }
    TreeNode* pmin = pRootOfTree;
    while (pmin->left != NULL)
    {
        pmin = pmin->left;
    }
    Convert_core(pRootOfTree);
    return pmin;
}
```



