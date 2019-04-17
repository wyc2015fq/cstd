# Leetcode 102 - 村雨sup - 博客园








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
    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> res;
        if(root == NULL)
            return res;
        queue<TreeNode*> que;
        que.push(root);
        int gcnt = 1;
        while(!empty(que)){
            int cnt = 0;
            vector<int> tres;
            for(int i=0;i < gcnt;i++){
                TreeNode* temp = que.front();
                if(temp->left != NULL){
                    que.push(temp->left);
                    cnt++;
                }
                if(temp->right != NULL){
                    que.push(temp->right);
                    cnt++;
                }
                tres.push_back(temp->val);
                que.pop();
            }
            gcnt = cnt;
            res.push_back(tres);
        }             
        return res;
    }
};
```

_













