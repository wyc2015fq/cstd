# Leetcode 145 - 村雨sup - 博客园








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
    vector<int> postorderTraversal(TreeNode* root) {
        vector<int> res;
        dfs(root,res);
        return res;
    }
    void dfs(TreeNode* root,vector<int>& res){
        if(root == NULL) return;
        dfs(root->left,res);
        dfs(root->right,res);
        res.push_back(root->val);
    }
};
```

迭代遍历：

head表示的是上一次处理完的节点，如果处理完的节点是栈头节点的子节点，就说明可以处理根节点了。（放的时候都是根右左，根在最下面，最后才会处理根）

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
    vector<int> postorderTraversal(TreeNode* root) {
        vector<int> res;
        if(root == NULL) return res;
        stack<TreeNode*> st{{root}};
        TreeNode* head = root;
        while(!st.empty()){
            TreeNode* p = st.top();
            if((!p->left&&!p->right)||p->left == head||p->right == head){
                res.push_back(p->val);
                head = p;
                st.pop();  
            }
            else{
                if(p->right) st.push(p->right);
                if(p->left) st.push(p->left);
            }
        }
        return res;
    }
    
};
```













