# 【剑指offer】题25：二叉树路径和为某一值 - xiaxzhou的博客 - CSDN博客





2017年07月02日 16:43:19[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：117
个人分类：[剑指offer](https://blog.csdn.net/xiaxzhou/article/category/6949916)









## 递归

```cpp
class Solution {
public:
    vector<vector<int> > FindPath(TreeNode* root, int expectNumber) {
        vector<vector<int>> result;
        if (root == NULL)
        {
            return result;
        }
        stack<int> my_stack;
        FindPath_Core(root, expectNumber,my_stack, result);
        return result;
    }
    void FindPath_Core(TreeNode* root, int & sum,stack<int>& my_stack, vector<vector<int>>& result)
    {
        sum -= root->val;
        my_stack.push(root->val);
        if (root->left==NULL&&root->right==NULL&&sum ==0)
        {
            auto tmp = my_stack;
            vector<int> vec;
            while (!tmp.empty())
            {
                vec.insert(vec.begin(),tmp.top());
                tmp.pop();
            }
            result.push_back(vec);
        }
        if (root->left!=NULL)
        {
            FindPath_Core(root->left, sum, my_stack, result);
        }
        if (root->right!=NULL)
        {
            FindPath_Core(root->right, sum, my_stack, result);
        }
        sum += root->val;
        my_stack.pop();
    }
};
```



