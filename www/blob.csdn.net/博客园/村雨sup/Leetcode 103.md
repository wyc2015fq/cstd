# Leetcode 103 - 村雨sup - 博客园








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
//好蠢的写法啊

class Solution {
public:
    vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
        stack<TreeNode*> st;
        vector<vector<int>> res;
        vector<int> first;
        if(root == NULL){
            return res;
        }
        else{
            first.push_back(root->val);
            res.push_back(first);
        }
        
        st.push(root);
        int flag = 0;
        while(!empty(st)){
            vector<int> tres;
            queue<TreeNode*> que;
            while(!empty(st)){
                TreeNode* temp = st.top();
                que.push(temp);
                st.pop();
            }
            if(flag%2 == 0){
                while(!empty(que)){
                    TreeNode* temp = que.front();
                    if(temp->right != NULL){
                        st.push(temp->right);
                        tres.push_back(temp->right->val);
                    }
                    if(temp->left != NULL){
                        st.push(temp->left);
                        tres.push_back(temp->left->val);
                    }
                    que.pop();
                }
            }
            if(flag%2 == 1){
                while(!empty(que)){
                    TreeNode* temp = que.front();
                    if(temp->left != NULL){
                        st.push(temp->left);
                        tres.push_back(temp->left->val);
                    }
                    if(temp->right != NULL){
                        st.push(temp->right);
                        tres.push_back(temp->right->val);
                    }
                    
                    que.pop();
                }
            }
            flag++;
            if(tres.size())
                res.push_back(tres);
        }
        return res;
    }
};
```

上面是把stack转移到一个queue中

下面是利用两个stack互转做

```
/**
 * Definition for binary tree
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    vector<vector<int> > zigzagLevelOrder(TreeNode *root) {
        vector<vector<int> >res;
        if (!root) return res;
        stack<TreeNode*> s1;
        stack<TreeNode*> s2;
        s1.push(root);
        vector<int> out;
        while (!s1.empty() || !s2.empty()) {
            while (!s1.empty()) {
                TreeNode *cur = s1.top();
                s1.pop();
                out.push_back(cur->val);
                if (cur->left) s2.push(cur->left);
                if (cur->right) s2.push(cur->right);
            } 
            if (!out.empty()) res.push_back(out);
            out.clear();
            while (!s2.empty()) {
                TreeNode *cur = s2.top();
                s2.pop();
                out.push_back(cur->val);
                if (cur->right) s1.push(cur->right);
                if (cur->left) s1.push(cur->left);
            }
            if (!out.empty()) res.push_back(out);
            out.clear();
        }
        return res;
    }
};
```













