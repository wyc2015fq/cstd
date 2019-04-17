# 【剑指offer】题18：树的子结构 - xiaxzhou的博客 - CSDN博客





2017年07月01日 21:28:57[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：135
个人分类：[剑指offer](https://blog.csdn.net/xiaxzhou/article/category/6949916)









```
bool HasSubtree(TreeNode* pRoot1, TreeNode* pRoot2)
{
    if (pRoot2==NULL)
    {
        return true;
    }
    if (pRoot1==NULL) {
        return false;
    }
    if (pRoot1->val == pRoot2->val)
    {
        bool flagleft = HasSubtree(pRoot1->left, pRoot2->left);
        bool flagright= HasSubtree(pRoot1->right, pRoot2->right);
        if (flagright&&flagleft)
        {
            return true;
        }
    }
    bool flagleft = HasSubtree(pRoot1->left, pRoot2);
    bool flagright= HasSubtree(pRoot1->right, pRoot2);
    if (flagleft||flagright)
    {
        return true;
    }
    else
    {
        return false;
    }
}
```



