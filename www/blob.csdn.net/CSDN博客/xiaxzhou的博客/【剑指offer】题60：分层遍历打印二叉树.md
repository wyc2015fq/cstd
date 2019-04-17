# 【剑指offer】题60：分层遍历打印二叉树 - xiaxzhou的博客 - CSDN博客





2017年09月02日 21:35:57[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：147标签：[二叉树](https://so.csdn.net/so/search/s.do?q=二叉树&t=blog)
个人分类：[剑指offer](https://blog.csdn.net/xiaxzhou/article/category/6949916)









```cpp
void Print(TreeNode* pRoot)
{
    queue<TreeNode*> que;
    int cur_count(0);
    int next_count(0);

    if (pRoot == NULL)
    {
        return;
    }

    que.push(pRoot);
    cur_count = 1;

    while (!que.empty())
    {
        for (auto i = 0; i < cur_count;++i)
        {
            TreeNode* ptmp = que.front();
            que.pop();
            cout << (char)ptmp->val << " ";
            if (ptmp->left != NULL)
            {
                que.push(ptmp->left);
                next_count++; 
            }
            if (ptmp->right != NULL)
            {
                que.push(ptmp->right);
                next_count++;
            }
        }
        cout << endl;
        cur_count = next_count;
        next_count = 0;
    }
}
```



