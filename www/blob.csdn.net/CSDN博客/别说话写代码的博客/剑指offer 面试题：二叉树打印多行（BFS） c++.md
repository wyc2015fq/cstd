# 剑指offer 面试题：二叉树打印多行（BFS） c++ - 别说话写代码的博客 - CSDN博客





2018年11月29日 15:58:21[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：24标签：[二叉树打印为多行																[分行打印二叉树																[剑指offer																[面试题32](https://so.csdn.net/so/search/s.do?q=面试题32&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[剑指offer刷题c++/python](https://blog.csdn.net/column/details/29881.html)](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)




题目：从上到下按层打印二叉树，同一层结点从左至右输出。每一层输出一行。

思路：用bfs，借助队列操作。

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
            if(pRoot == nullptr) return res;
            queue<TreeNode*> nodes;
            nodes.push(pRoot);
            int i=0, len=0;
            TreeNode *pnode;
            while(!nodes.empty())
            {
                vector<int> vec;
                len = nodes.size();
                for(i=0;i<len;++i)
                {
                    pnode = nodes.front();
                    vec.push_back(pnode->val);
                    if(pnode->left) nodes.push(pnode->left);
                    if(pnode->right) nodes.push(pnode->right);
                    nodes.pop();
                }
                res.push_back(vec);
            }return res;
        }
};
```](https://so.csdn.net/so/search/s.do?q=分行打印二叉树&t=blog)](https://so.csdn.net/so/search/s.do?q=二叉树打印为多行&t=blog)




