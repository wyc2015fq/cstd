# Leetcode 993. 二叉树的堂兄弟节点 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)






### 993. 二叉树的堂兄弟节点

 显示英文描述
[我的提交](https://leetcode-cn.com/contest/weekly-contest-124/problems/cousins-in-binary-tree/submissions/)[返回竞赛](https://leetcode-cn.com/contest/weekly-contest-124/)







- 用户通过次数195
- 用户尝试次数229
- 通过次数195
- 提交次数462
- 题目难度Easy



在二叉树中，根节点位于深度 `0` 处，每个深度为 `k` 的节点的子节点位于深度 `k+1` 处。

如果二叉树的两个节点深度相同，但父节点不同，则它们是一对*堂兄弟节点*。

我们给出了具有唯一值的二叉树的根节点 `root`，以及树中两个不同节点的值 `x` 和 `y`。

只有与值 `x` 和 `y` 对应的节点是堂兄弟节点时，才返回 `true`。否则，返回 `false`。



示例 1：
![](https://assets.leetcode-cn.com/aliyun-lc-upload/uploads/2019/02/16/q1248-01.png)
输入：root = [1,2,3,4], x = 4, y = 3
输出：false

示例 2：
![](https://assets.leetcode-cn.com/aliyun-lc-upload/uploads/2019/02/16/q1248-02.png)
输入：root = [1,2,3,null,4,null,5], x = 5, y = 4
输出：true

示例 3：

![](https://assets.leetcode-cn.com/aliyun-lc-upload/uploads/2019/02/16/q1248-03.png)
输入：root = [1,2,3,null,4], x = 2, y = 3
输出：false


提示：
- 二叉树的节点数介于 `2` 到 `100` 之间。
- 每个节点的值都是唯一的、范围为 `1` 到 `100` 的整数。



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
    map<int,int> mp;
    
    void createparent(TreeNode* root){
        if(root->left){mp[root->left->val] = root->val; createparent(root->left);}
        if(root->right){mp[root->right->val] = root->val; createparent(root->right);}
        return;
    }
    
    int treelen(TreeNode* root,int cnt,int x){
        if(!root) return -1;
        if(root->val == x)return cnt;
        return max(treelen(root->left,cnt+1,x),treelen(root->right,cnt+1,x));
    }
    
    
    
    bool isCousins(TreeNode* root, int x, int y) {
        createparent(root);
        int a = treelen(root,0,x);
        int b = treelen(root,0,y);
        // cout << a << " " << b;
        // cout << mp[x] << " " << mp[y];
        if(a == b&&mp[x]!=mp[y])return true;
        return false;
    }
};
```

下手之前先理清思路，一开始写了个层序遍历才发现不对。。














