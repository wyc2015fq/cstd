# 剑指offer 面试题37：序列化二叉树 c++ - 别说话写代码的博客 - CSDN博客





2018年11月29日 15:16:02[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：122
所属专栏：[剑指offer刷题c++/python](https://blog.csdn.net/column/details/29881.html)









题目：请实现两个函数，分别用来序列化和反序列化二叉树。

思路：用dfs的思想，用前序遍历来序列化和反序列化二叉树

参考思路：[https://blog.csdn.net/u011475210/article/details/78889876](https://blog.csdn.net/u011475210/article/details/78889876)

（1）用vector，不用考虑逗号操作

（2）用string

（3）用stringstream，代码更加简洁



基于vector代码 如下

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
    vector<int> buf;
    void dfs1(TreeNode *root)
    {
        if(root == nullptr)
        {
            buf.push_back(0xFFFFFFFF);
            return ;
        }
        buf.push_back(root->val);
        dfs1(root->left);
        dfs1(root->right);
    }
    TreeNode *dfs2(int* &n)
    {
        if(*n==0xFFFFFFFF)
        {
            n++;
            return nullptr;
        }
        TreeNode *p = new TreeNode(*n);
        n++;
        p->left = dfs2(n);
        p->right = dfs2(n);
        return p;
    }
    char* Serialize(TreeNode *root) {    
        dfs1(root);
        int bufSize = buf.size();
        int *res = new int[bufSize];
        for(int i = 0;i < bufSize; i++) res[i] = buf[i];
        return (char*)res;
    }
    TreeNode* Deserialize(char *str) {
        int *p = (int*) str;
        return dfs2(p);
    }
};
```





