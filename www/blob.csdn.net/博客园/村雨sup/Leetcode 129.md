# Leetcode 129 - 村雨sup - 博客园








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
    int sumNumbers(TreeNode* root) {
        if(root == NULL) return 0;
        int res = 0;
        DFS(root,res,0);
        return res;
    }
    
    void DFS(TreeNode* root,int &res,int add){
        if(root->left == NULL && root->right == NULL){
            add = add*10 + root->val;
            res += add;
            return;
        }
        add = add*10 + root->val;
        if(root->left != NULL){
            DFS(root->left,res,add);
        }
        if(root->right != NULL){
            DFS(root->right,res,add);
        }
    }
};
```

EZ











