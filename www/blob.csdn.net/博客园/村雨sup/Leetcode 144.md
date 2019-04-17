# Leetcode 144 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    vector<int> preorderTraversal(TreeNode* root) {
        vector<int> res;
        dfs(root,res);
        return res;
    }
    void dfs(TreeNode* root,vector<int>& res){
        if(root == NULL) return;
        res.push_back(root->val);
        dfs(root->left,res);
        dfs(root->right,res);
    }
};
```

迭代遍历：

一般我们提到[树的遍历](http://zh.wikipedia.org/wiki/%E6%A0%91%E7%9A%84%E9%81%8D%E5%8E%86)，最常见的有先序遍历，中序遍历，后序遍历和层序遍历，它们用递归实现起来都非常的简单。而题目的要求是不能使用递归求解，于是只能考虑到用非递归的方法，这就要用到stack来辅助运算。由于先序遍历的顺序是"根-左-右", 算法为：

1. 把根节点push到栈中

2. 循环检测栈是否为空，若不空，则取出栈顶元素，保存其值，然后看其右子节点是否存在，若存在则push到栈中。再看其左子节点，若存在，则push到栈中。

```
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    vector<int> preorderTraversal(TreeNode* root) {
        vector<int> res;
        if(root == NULL) return res;
        stack<TreeNode*> st;
        st.push(root);
        while(!st.empty()){
            TreeNode* p = st.top();
            st.pop();
            res.push_back(p->val);
            if(p->right) st.push(p->right);
            if(p->left) st.push(p->left);
        }
        return res;
    }
    
};
```

——











