# 【剑指offer】题61：之字打印二叉树 - xiaxzhou的博客 - CSDN博客





2017年09月02日 22:00:54[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：146标签：[二叉树](https://so.csdn.net/so/search/s.do?q=二叉树&t=blog)
个人分类：[剑指offer																[数据结构](https://blog.csdn.net/xiaxzhou/article/category/7119623)](https://blog.csdn.net/xiaxzhou/article/category/6949916)








```cpp
vector<vector<int>> Print(TreeNode* pRoot)
{
    vector<vector<int>> vec;
    if (pRoot== NULL)
        return vec;

    stack<TreeNode*> my_stack[2];

    int cur_deep(0);
    int next_deep(1);

    my_stack[cur_deep % 2].push(pRoot);

    while (!my_stack[cur_deep%2].empty())
    {
        vec.push_back(vector<int>());
        while (!my_stack[cur_deep%2].empty())
        {
            TreeNode* tmp = my_stack[cur_deep % 2].top();
            vec.back().push_back(tmp->val);
            my_stack[cur_deep % 2].pop();
            if (cur_deep % 2 == 0)
            {
                if (tmp->left != NULL)
                {
                    my_stack[next_deep % 2].push(tmp->left);
                }
                if (tmp->right != NULL)
                {
                    my_stack[next_deep % 2].push(tmp->right);
                }
            }
            else
            {
                if (tmp->right != NULL)
                {
                    my_stack[next_deep % 2].push(tmp->right);
                }
                if (tmp->left != NULL)
                {
                    my_stack[next_deep % 2].push(tmp->left);
                }
            }
        }
        cur_deep++;
        next_deep++;
    }
    return vec;
}
```




