# Leetcode 98 - 村雨sup - 博客园








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
    bool isValidBST(TreeNode* root) {
        vector<int> res;
        int flag = 1;
        long num = LONG_MIN;
        dfs(root,num,flag);
        return flag;
    }
    
    void dfs(TreeNode* root,long& maxnum,int& flag){
        if(root == NULL) return;
        if(root->left) dfs(root->left,maxnum,flag);
        if(root->val > maxnum){
            maxnum = root->val;
        }
        else{
            flag = 0;
        }
        if(root->right) dfs(root->right,maxnum,flag);
    }
};
```

__卡边界有点恶心，改成LONG就好了













