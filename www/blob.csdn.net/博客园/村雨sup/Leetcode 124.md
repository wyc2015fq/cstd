# Leetcode 124 * - 村雨sup - 博客园








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
    int maxPathSum(TreeNode* root) {
        int res = INT_MIN;
        DFS(root,res);
        return res;
    }
    int DFS(TreeNode* root,int &res){
        if(!root) return 0;
        int left = max(DFS(root->left,res),0);
        int right = max(DFS(root->right,res),0);
        res = max(res,left+right+root->val);
        return max(left,right)+root->val; //res是整个树的最大值,return 的是作为单节点的最大值，不矛盾
    }
};
```

_











