# 剑指Offer - 重建二叉树 - curryche的博客 - CSDN博客





2018年08月01日 09:45:18[curryche](https://me.csdn.net/whwan11)阅读数：23标签：[剑指offer																[二叉树](https://so.csdn.net/so/search/s.do?q=二叉树&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)
个人分类：[笔试](https://blog.csdn.net/whwan11/article/category/7789918)








## 题目

重建二叉树 

时间限制：1秒 空间限制：32768K 热度指数：394864

题目描述 

输入某二叉树的前序遍历和中序遍历的结果，请重建出该二叉树。假设输入的前序遍历和中序遍历的结果中都不含重复的数字。例如输入前序遍历序列{1,2,4,7,3,5,6,8}和中序遍历序列{4,7,2,1,5,3,8,6}，则重建二叉树并返回。

## 解法

代码

```cpp
/**
 * Definition for binary tree
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    TreeNode* reConstructBinaryTree(vector<int> pre,vector<int> vin) {      
        if(pre.empty()||vin.empty())
            return NULL;

        TreeNode* root=new TreeNode(pre[0]);

        int i;
        for(i=0;i<vin.size();i++)
        {
            if(vin[i]==pre[0])
            {
                break;
            }
        }

        vector<int> pre_right,vin_right,pre_left,vin_left;

        pre_left.insert(pre_left.begin(), pre.begin()+1, pre.begin() + i+1);
        vin_left.insert(vin_left.begin(), vin.begin(),vin.begin()+i);

        pre_right.insert(pre_right.begin(), pre.begin()+i+1, pre.end());
        vin_right.insert(vin_right.begin(), vin.begin()+i+1, vin.end());

        root->left=reConstructBinaryTree(pre_left,vin_left);
        root->right=reConstructBinaryTree(pre_right,vin_right);

        return root;

    }
};
```




