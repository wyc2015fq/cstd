# Leetcode 105 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





![](https://img2018.cnblogs.com/blog/1196023/201901/1196023-20190127134202738-414371666.png)

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
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        return DFS(preorder,0,inorder,0,inorder.size()-1);
    }
    
    TreeNode* DFS(vector<int> preorder,int pos,vector<int> inorder,int ileft,int iright){
        if(ileft > iright) return NULL;
        int i=0;
        for(i=ileft;i <= iright;i++){
            if(preorder[pos] == inorder[i])break;
        }
        TreeNode* cur = new TreeNode(preorder[pos]);
        cur->left = DFS(preorder,pos+1,inorder,ileft,i-1);
        cur->right = DFS(preorder,pos+i-ileft+1,inorder,i+1,iright);
        return cur;
    }
};
```













