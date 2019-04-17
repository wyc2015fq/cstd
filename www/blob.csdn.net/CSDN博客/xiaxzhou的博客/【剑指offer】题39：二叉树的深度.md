# 【剑指offer】题39：二叉树的深度 - xiaxzhou的博客 - CSDN博客





2017年07月04日 21:32:31[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：133








```
int TreeDepth(TreeNode* pRoot)
{
    if (pRoot == NULL)
    {
        return 0;
    }
    return max(TreeDepth(pRoot->left) + 1, TreeDepth(pRoot->right) + 1);
}
```



