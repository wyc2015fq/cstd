# Leetcode 106 - 村雨sup - 博客园








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
    TreeNode* buildTree(vector<int>& inorder, vector<int>& postorder) {
        return DFS(inorder,0,inorder.size()-1,postorder,postorder.size()-1);
    }
    TreeNode* DFS(vector<int> inorder,int ileft,int iright,vector<int> postorder,int pos){
        if(ileft > iright) return NULL;
        int i=0;
        for(i=ileft;i < iright;i++){
            if(postorder[pos] == inorder[i])break;
        }
        TreeNode* cur = new TreeNode(postorder[pos]);
        cur->right = DFS(inorder,i+1,iright,postorder,pos-1);
        cur->left = DFS(inorder,ileft,i-1,postorder,pos-iright+i-1);
        return cur;
    }
};
```

__











