# 剑指offer - 按之字形顺序打印二叉树 - curryche的博客 - CSDN博客





2018年08月21日 16:24:49[curryche](https://me.csdn.net/whwan11)阅读数：38








## 题目

按之字形顺序打印二叉树

时间限制：1秒 空间限制：32768K 热度指数：106091

题目描述 

请实现一个函数按照之字形打印二叉树，即第一行按照从左到右的顺序打印，第二层按照从右至左的顺序打印，第三行按照从左到右的顺序打印，其他行以此类推。

## 解法

代码

```cpp
/*
struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
    TreeNode(int x) :
            val(x), left(NULL), right(NULL) {
    }
};
*/
class Solution {
public:
    vector<vector<int> > Print(TreeNode* pRoot) 
    {
        vector<vector<int>> result;
        if(pRoot==NULL)
            return result;

        vector<int> current_layer;
        queue<TreeNode*> nodes;
        queue<bool> labels;
        bool current_label;
        TreeNode* current=NULL;

        current_label=true;
        labels.push(true);
        nodes.push(pRoot);
        while(!nodes.empty())
        {
            current=nodes.front();
            if(labels.front()==current_label)
            {
                current_layer.push_back(current->val);
            }
            else
            {
                if(current_label)
                    reverse(current_layer.begin(),current_layer.end());

                result.push_back(current_layer);
                current_layer.clear();
                current_layer.push_back(current->val);
                current_label=(!current_label);
            }

           if(current->right!=NULL)
           {
               nodes.push(current->right);
               labels.push(!labels.front());
           }
           if(current->left!=NULL)
           {
               nodes.push(current->left);
               labels.push(!labels.front());
           }


            nodes.pop();
            labels.pop();
        }
        if(!current_layer.empty())
        {
            if(current_label)
                reverse(current_layer.begin(),current_layer.end());

            result.push_back(current_layer);
        }

        return result;
    }
};
```



