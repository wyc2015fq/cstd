# Leetcode 116 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
/**
 * Definition for binary tree with next pointer.
 * struct TreeLinkNode {
 *  int val;
 *  TreeLinkNode *left, *right, *next;
 *  TreeLinkNode(int x) : val(x), left(NULL), right(NULL), next(NULL) {}
 * };
 */
class Solution {
public:
    void connect(TreeLinkNode *root) {
        if(root == NULL) return;
        queue<TreeLinkNode *> que;
        que.push(root);
        while(!empty(que)){
            int lens = que.size();
            for(int i=0;i < lens-1;i++){
                TreeLinkNode *temp = que.front();
                que.pop();
                temp->next = que.front();
                if(temp->left) que.push(temp->left);
                if(temp->right) que.push(temp->right);
            }
            TreeLinkNode *temp2 = que.front();
            temp2->next = NULL;
            que.pop();
            if(temp2->left) que.push(temp2->left);
            if(temp2->right) que.push(temp2->right);
        }
    }
};
```

_











