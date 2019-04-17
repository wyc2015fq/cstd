# 【剑指offer】题63：二叉搜索树的第k个节点 - xiaxzhou的博客 - CSDN博客





2017年09月03日 11:33:49[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：164








```
TreeNode* KthNode_core(TreeNode* pRoot, int& k)
{
    TreeNode* target(NULL);
    if (pRoot->left!=NULL)
    {
        target = KthNode_core(pRoot->left, k);
    }
    if (target == NULL)
    {
        k--;
        if (k == 0)
        {
            target = pRoot;
        }
    }
    if (target == NULL && pRoot->right!=NULL)
    {
        target = KthNode_core(pRoot->right, k);
    }
    return target;
}

TreeNode* KthNode(TreeNode* pRoot, int k)
{
    if (pRoot == NULL)
    {
        return pRoot;
    }
    return KthNode_core(pRoot, k);
}
```



