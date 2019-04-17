# Leetcode 113 - 村雨sup - 博客园








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
    vector<vector<int>> pathSum(TreeNode* root, int sum) {
        vector<vector<int>> res;
        if(root == NULL) return res;
        vector<int> add;
        add.push_back(root->val);
        DFS(res,add,root,sum,root->val);
        return res;
        
    }
    void DFS(vector<vector<int>>& res,vector<int>& add,TreeNode*root,int sum,int& he){
        if((root->left == NULL)&&(root->right == NULL)){
            if(he == sum) res.push_back(add);
        }
        else if((root->left != NULL)&&(root->right == NULL)){
            add.push_back(root->left->val);
            he += root->left->val;
            DFS(res,add,root->left,sum,he);
            add.pop_back();
            he -= root->left->val;
        }
        else if((root->left == NULL)&&(root->right != NULL)){
            add.push_back(root->right->val);
            he += root->right->val;
            DFS(res,add,root->right,sum,he);
            add.pop_back();
            he -= root->right->val;
        }
        else if((root->left != NULL)&&(root->right != NULL)){
            add.push_back(root->left->val);
            he += root->left->val;
            DFS(res,add,root->left,sum,he);
            add.pop_back();
            he -= root->left->val;
            add.push_back(root->right->val);
            he += root->right->val;
            DFS(res,add,root->right,sum,he);
            add.pop_back();
            he -= root->right->val;
        }
        return;
    }
};
```

_虽然代码丑，但比较好理解

```
class Solution {
public:
    vector<vector<int> > pathSum(TreeNode *root, int sum) {
        vector<vector<int>> res;
        vector<int> out;
        helper(root, sum, out, res);
        return res;
    }
    void helper(TreeNode* node, int sum, vector<int>& out, vector<vector<int>>& res) {
        if (!node) return;
        out.push_back(node->val);
        if (sum == node->val && !node->left && !node->right) {
            res.push_back(out);
        }
        helper(node->left, sum - node->val, out, res);
        helper(node->right, sum - node->val, out, res);
        out.pop_back();
    }
};
```

——这个和上一题对应











