# Leetcode 863. 二叉树中所有距离为 K 的结点 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)






### 863. 二叉树中所有距离为 K 的结点

 显示英文描述
[我的提交](https://leetcode-cn.com/contest/weekly-contest-91/problems/all-nodes-distance-k-in-binary-tree/submissions/)[返回竞赛](https://leetcode-cn.com/contest/weekly-contest-91/)







- 用户通过次数39
- 用户尝试次数59
- 通过次数39
- 提交次数174
- 题目难度Medium



给定一个二叉树（具有根结点 `root`）， 一个目标结点 `target` ，和一个整数值 `K` 。

返回到目标结点 `target` 距离为 `K` 的所有结点的值的列表。 答案可以以任何顺序返回。



示例 1：
输入：root = [3,5,1,6,2,0,8,null,null,7,4], target = 5, K = 2

输出：[7,4,1]

解释：
所求结点为与目标结点（值为 5）距离为 2 的结点，
值分别为 7，4，以及 1

![](https://s3-lc-upload.s3.amazonaws.com/uploads/2018/06/28/sketch0.png)注意，输入的 "root" 和 "target" 实际上是树上的结点。
上面的输入仅仅是对这些对象进行了序列化描述。



提示：
- 给定的树是非空的，且最多有 `K` 个结点。
- 树上的每个结点都具有唯一的值 `0 <= node.val <= 500` 。
- 目标结点 `target` 是树上的结点。
- `0 <= K <= 1000`.

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
    map<TreeNode*,TreeNode*> mp;
    void findparent(TreeNode* root){
        if(root == NULL)return;
        if(root->left){
            mp[root->left] = root;
            findparent(root->left);
        }
        if(root->right){
            mp[root->right] = root;
            findparent(root->right);
        }
        return;
    }
    
    void dfs(TreeNode* target,int k,set<TreeNode*> vis,vector<int>& res){
        if(vis.find(target)!=vis.end()) return;//visit.end()表示集合中没有找到
        vis.insert(target);
        if(k == 0){res.push_back(target->val);return;}
        if(target->left) dfs(target->left,k-1,vis,res);
        if(target->right) dfs(target->right,k-1,vis,res);
        TreeNode* p = mp[target];
        if(p) dfs(p,k-1,vis,res);
    }
    vector<int> distanceK(TreeNode* root, TreeNode* target, int K) {
        if(root == NULL) return{};
        findparent(root);
        vector<int> res;
        set<TreeNode*> vis;
        dfs(target,K,vis,res);
        return res;
    }
};
```

__学习了set用法，用map保存根节点。














