# Leetcode 112 - 村雨sup - 博客园








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
    bool hasPathSum(TreeNode* root, int sum) {
        if(root == NULL) return false;
        int flag = 0;
        DFS(root,sum,flag);
        if(flag) return true;
        else return false;
    }
    void DFS(TreeNode* root, int sum,int& flag){
        if((root->left == NULL)&&(root->right == NULL)){
            if(root->val == sum)
                flag = 1;
        }
        else if((root->left != NULL)&&(root->right == NULL)){
            root->left->val += root->val;
            DFS(root->left,sum,flag);
        }
        else if((root->left == NULL)&&(root->right != NULL)){
            root->right->val += root->val;
            DFS(root->right,sum,flag);
        }
        else if((root->left != NULL)&&(root->right != NULL)){
            root->left->val += root->val;
            DFS(root->left,sum,flag);
            root->right->val += root->val;
            DFS(root->right,sum,flag);
        }
        return;
    }
};
```

__

简便写法：

```
class Solution {
public:
    bool hasPathSum(TreeNode* root, int sum) {
        if(!root) return false;
        if(root->left == NULL && root->right == NULL) return root->val == sum;
        return hasPathSum(root->left, sum - root->val) || hasPathSum(root->right, sum - root->val); //sum - root->val很灵性
    }
};
```













