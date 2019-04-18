# 剑指offer - 二叉树中和为某一值的路径 - curryche的博客 - CSDN博客





2018年08月26日 20:24:34[curryche](https://me.csdn.net/whwan11)阅读数：30标签：[剑指offer																[c++](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)
个人分类：[笔试](https://blog.csdn.net/whwan11/article/category/7789918)








## 题目

二叉树中和为某一值的路径 

时间限制：1秒 空间限制：32768K 热度指数：234191

题目描述 

输入一颗二叉树的跟节点和一个整数，打印出二叉树中结点值的和为输入整数的所有路径。路径定义为从树的根结点开始往下一直到叶结点所经过的结点形成一条路径。(注意: 在返回值的list中，数组长度大的数组靠前)

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
};*/
class Solution {
public:
    vector<vector<int> > FindPath(TreeNode* root,int expectNumber) {
        vector<vector<int>> result;
        if(root==NULL)
            return result;

        result=find_path_recursive(root,expectNumber);
        //quick_sort(result,0,result.size()-1);
        vector<int> temp;
        for(int i=result.size()-1;i>0;i--)
        {
            for(int j=0;j<i;j++)
            {
                if(result[j].size()<result[j+1].size())
                {
                    temp=result[j];
                    result[j]=result[j+1];
                    result[j+1]=temp;
                }
            }
        }
        for(int i=0;i<result.size();i++)
        {
            reverse(result[i].begin(),result[i].end());
        }
        return result;

    }

    vector<vector<int>> find_path_recursive(TreeNode* root,int expectNumber)
    {
        vector<vector<int>> result;
        if(root->left==NULL&&root->right==NULL)
        {
            if(root->val==expectNumber)
            {
                vector<int> temp;
                temp.push_back(root->val);
                result.push_back(temp);
            }
            return result;
        }

        if(root->left!=NULL)
        {
            vector<vector<int>> temp;
            temp=find_path_recursive(root->left,expectNumber-root->val);
            for(int i=0;i<temp.size();i++)
            {
                temp[i].push_back(root->val);
                result.push_back(temp[i]);
            }
        }
        if(root->right!=NULL)
        {
            vector<vector<int>> temp;
            temp=find_path_recursive(root->right,expectNumber-root->val);
            for(int i=0;i<temp.size();i++)
            {
                temp[i].push_back(root->val);
                result.push_back(temp[i]);
            }
        }
        return result;
    }

    void quick_sort(vector<vector<int>> &nums,int low,int high)
    {
        vector<int> pivot_vec=nums[low];
        int pivot=nums[low].size();
        int i,j;
        i=low;
        j=high;
        while(i<j)
        {
            while(i<j&&nums[j].size()>=pivot)
                j--;
            if(i<j)
                nums[i++]=nums[j];
            while(i<j&&nums[i].size()<=pivot)
                i++;
            if(i<j)
                nums[j--]=nums[i];
        }
        nums[i]=pivot_vec;
        quick_sort(nums,low,i-1);
        quick_sort(nums,i+1,high);
    }

};
```




