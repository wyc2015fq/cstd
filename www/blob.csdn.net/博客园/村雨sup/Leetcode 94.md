# Leetcode 94 - 村雨sup - 博客园








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
    vector<int> inorderTraversal(TreeNode* root) {
        vector<int> res;
        DFS(root,res);
        return res;
        
    }
    void DFS(TreeNode* root,vector<int>& res){
        if(root == NULL)return;
        DFS(root->left,res);
        res.push_back(root->val);
        DFS(root->right,res);
        return;
    }
};
```

___stack__

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
    vector<int> inorderTraversal(TreeNode* root) {
        vector<int> res;
        stack<TreeNode*> st;
        TreeNode* p = root;
        while(p || !empty(st)){  //p的右面没有节点了，然后stack中也没有点了，说明结束遍历了
            while(p){
                st.push(p);
                p = p->left;
            }
            p = st.top();
            st.pop();
            res.push_back(p->val);
            p = p->right;
        }
        return res;
    }
};
```

_











