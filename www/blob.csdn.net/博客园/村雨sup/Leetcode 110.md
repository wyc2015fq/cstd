# Leetcode 110 - 村雨sup - 博客园








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
    bool isBalanced(TreeNode* root) {
        int flag = 1;
        depth(root,0,flag);
        if(flag) return true;
        else return false;
    }
    
    int depth(TreeNode* root,int cnt,int& flag){
        if(root == NULL) return cnt;
        int a = depth(root->left,cnt+1,flag);
        int b = depth(root->right,cnt+1,flag);
        if(abs(a-b) > 1) flag = 0;
        return max(a,b);
    }
};
```

_











