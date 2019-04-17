# 【剑指offer】题39：平衡二叉树 - xiaxzhou的博客 - CSDN博客





2017年07月04日 22:01:31[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：94








```
bool func(TreeNode* pRoot, int& depth)
{
    if (pRoot == NULL)
    {
        return true;
    }
    int left(0), right(0);
    bool leftflag = func(pRoot->left, left);
    bool rightflag = func(pRoot->right, right);
    if (leftflag&&rightflag&&abs(left-right)<1)
    {
        depth = max(left, right) + 1;
        return true;
    }
    else
        return false;
}

bool IsBalanced_Solution(TreeNode* pRoot) {
    if (pRoot==NULL)
    {
        return true;
    }
    int depth(0);
    return func(pRoot, depth);
}
```



