# 剑指offer 面试题：之字形打印二叉树（BFS） c++ - 别说话写代码的博客 - CSDN博客





2018年11月29日 16:57:56[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：25标签：[剑指offer																[之字形打印二叉树](https://so.csdn.net/so/search/s.do?q=之字形打印二叉树&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[剑指offer刷题c++/python](https://blog.csdn.net/column/details/29881.html)








题目：请实现一个函数按照之字形打印二叉树，即第一行按照从左到右的顺序打印，第二层按照从右至左的顺序打印，第三行按照从左到右的顺序打印，其他行以此类推。

思路：与 上一题分行打印二叉树[https://blog.csdn.net/qq_21997625/article/details/84634299](https://blog.csdn.net/qq_21997625/article/details/84634299) 一样。不过多了个标志位。

用bfs，然后逐行打印，设个标志flag翻转

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
    vector<vector<int> > Print(TreeNode* pRoot) {
        vector<vector<int>> res;
        if (pRoot == nullptr) return res;
        queue<TreeNode *> nodes;
        nodes.push(pRoot);
        bool flag = false;
        while(!nodes.empty())
        {
            vector<int> vec;
            int len = nodes.size();
            for(int i=0;i<len;++i)
            {
                TreeNode *p=nodes.front();
                vec.push_back(p->val);
                if(p->left) nodes.push(p->left);
                if(p->right) nodes.push(p->right);
                nodes.pop();
            }
            if(flag) std::reverse(vec.begin(),vec.end());
            flag = !flag;
            res.push_back(vec);
        }return res;
    }
    
};
```






