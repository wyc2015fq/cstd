# Leetcode 107 - 村雨sup - 博客园








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
    vector<vector<int>> levelOrderBottom(TreeNode* root) {
        vector<vector<int>> res;
        if(root == NULL) return res;
        queue<TreeNode*> que;
        que.push(root);
        while(!empty(que)){
            vector<int> add;
            int len = que.size();
            for(int i=0;i < len;i++){
                TreeNode* temp = que.front();
                add.push_back(temp->val);
                if(temp->left != NULL)que.push(temp->left);
                if(temp->right != NULL)que.push(temp->right);
                que.pop();
            }
            res.push_back(add);
            add.clear();
        }
        return reversevector(res);
    }
    vector<vector<int>> reversevector(vector<vector<int>> res){
        vector<vector<int>> reverseres;
        for(int i=res.size()-1;i >=0;i--){
            reverseres.push_back(res[i]);
        }
        return reverseres;
    }
};
```

_











