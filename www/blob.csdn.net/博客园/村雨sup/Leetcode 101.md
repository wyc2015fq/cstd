# Leetcode 101 - 村雨sup - 博客园








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
    bool isSymmetric(TreeNode* root) {
        if(root == NULL)
            return true;
        return DFS(root->left,root->right);
    }
    
    bool DFS(TreeNode* rtl,TreeNode* rtr){ //这样写法也是各种二叉树DFS的一种很好的写法
        if((!rtl)&&(!rtr)) return true;  // 和上面平衡二叉树写法类似
        if((rtl&&!rtr)||(!rtl&&rtr)||rtl->val != rtr->val) return false;
        return DFS(rtl->left,rtr->right)&&DFS(rtl->right,rtr->left);
    }
};
```













